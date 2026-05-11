# PA3 Notes

## 目录

- [第 1 章 CTE、Trap、Yield 与 Timer Interrupt](#第-1-章-ctetrapyield-与-timer-interrupt)
  - [1.1 本模块总览](#11-本模块总览)
  - [1.2 核心知识点](#12-核心知识点)
    - [1.2.1 内联汇编与原子级 CSR 操作](#121-内联汇编与原子级-csr-操作)
    - [1.2.2 trap.S 的存在意义与核心功能](#122-traps-的存在意义与核心功能)
    - [1.2.3 Context 上下文结构体的意义](#123-context-上下文结构体的意义)
    - [1.2.4 CTE 的注册与回调机制](#124-cte-的注册与回调机制)
    - [1.2.5 CSR 寄存器总表与关键字段](#125-csr-寄存器总表与关键字段)
    - [1.2.6 特权级异常/中断处理中的 CSR 状态迁移](#126-特权级异常中断处理中的-csr-状态迁移)
  - [1.3 yield 自陷逻辑链](#13-yield-自陷逻辑链)
    - [1.3.1 软件入口与处理函数注册](#131-软件入口与处理函数注册)
    - [1.3.2 yield 到 ecall 的执行机制](#132-yield-到-ecall-的执行机制)
    - [1.3.3 ecall 译码与 isa_raise_intr](#133-ecall-译码与-isa_raise_intr)
    - [1.3.4 trap.S 保存现场并进入 C 处理流程](#134-traps-保存现场并进入-c-处理流程)
    - [1.3.5 __am_irq_handle 到 simple_trap 的回调链](#135-__am_irq_handle-到-simple_trap-的回调链)
    - [1.3.6 yield 返回路径总结](#136-yield-返回路径总结)
  - [1.4 timer 时钟中断逻辑链](#14-timer-时钟中断逻辑链)
    - [1.4.1 时钟设备如何提出中断请求](#141-时钟设备如何提出中断请求)
    - [1.4.2 CPU 为什么在每条指令后轮询](#142-cpu-为什么在每条指令后轮询)
    - [1.4.3 isa_query_intr 的判定逻辑](#143-isa_query_intr-的判定逻辑)
    - [1.4.4 为什么时钟中断传入的是 s.dnpc](#144-为什么时钟中断传入的是-sdnpc)
    - [1.4.5 isa_raise_intr 受理时钟中断](#145-isa_raise_intr-受理时钟中断)
    - [1.4.6 timer interrupt 返回路径总结](#146-timer-interrupt-返回路径总结)
  - [1.5 yield 与 timer interrupt 对照表](#15-yield-与-timer-interrupt-对照表)
  - [1.6 对话问题标准回答](#16-对话问题标准回答)
    - [1.6.1 为什么同步异常传 s->pc，而外部中断传 s.dnpc](#161-为什么同步异常传-spc而外部中断传-sdnpc)
    - [1.6.2 关于 Context、cte_init、user_handler](#162-关于-contextcte_inituser_handler)
    - [1.6.3 关于 mepc 为什么最初保存 trap 发生点](#163-关于-mepc-为什么最初保存-trap-发生点)
    - [1.6.4 关于 timer interrupt 中 pending 与 enable 的关系](#164-关于-timer-interrupt-中-pending-与-enable-的关系)
    - [1.6.5 关于 NEMU 软件模拟与真实硬件的区别](#165-关于-nemu-软件模拟与真实硬件的区别)
    - [1.6.6 关于为什么要返回 Context *](#166-关于为什么要返回-context-)
    - [1.6.7 关于 MTIP、MTIE、MIE 能否集中到一个模块统一处理](#167-关于-mtipmtiemie-能否集中到一个模块统一处理)
    - [1.6.8 关于为什么把中断判定和中断受理拆成两个阶段](#168-关于为什么把中断判定和中断受理拆成两个阶段)
  - [1.7 最终总结](#17-最终总结)

---

## 第 1 章 CTE、Trap、Yield 与 Timer Interrupt

### 1.1 本模块总览

本模块围绕 `CTE(Context Extension)` 的两条典型路径展开：

1. `yield -> ecall` 的同步自陷异常路径
2. `timer interrupt` 的异步外设时钟中断路径

两条路径的共同目标都是：

- 让 CPU 从普通执行流进入 trap 处理流
- 保存当前执行现场
- 在 C 代码中构造统一的事件语义 `Event`
- 回调用户注册的异常/中断处理函数
- 最终恢复某个 `Context`，并通过 `mret` 返回

也就是说，PA3 当前这部分本质上是在理解：

| 模块 | 核心问题 |
| --- | --- |
| `yield/ecall` | 软件如何主动制造一次同步异常 |
| `timer interrupt` | 外设如何提出中断请求并让 CPU 响应 |
| `trap.S` | 硬件 trap 现场如何翻译成 C 可处理的 `Context` |
| `cte_init()` | 如何把 trap 入口和最终处理函数注册好 |
| `__am_irq_handle()` | 如何把底层 `mcause` 翻译成上层 `Event` |
| `Context` | 为什么需要统一的上下文快照结构 |

---

### 1.2 核心知识点

#### 1.2.1 内联汇编与原子级 CSR 操作

在 AM 与 NEMU 中，很多“硬件状态修改”并不是通过普通 C 语句完成，而是通过内联汇编触发对应的 RISC-V 指令。

常见形式如下：

| 写法 | 含义 | 典型用途 |
| --- | --- | --- |
| `asm volatile("csrw mtvec, %0" : : "r"(x));` | 将 `x` 写入 `mtvec` | 注册 trap 入口 |
| `asm volatile("csrs mie, %0" : : "r"(1 << 7));` | 对 `mie` 的指定位执行置位 | 打开 `MTIE` |
| `asm volatile("csrc mstatus, %0" : : "r"(1 << 3));` | 对 `mstatus` 的指定位执行清零 | 关闭 `MIE` |
| `asm volatile("csrr %0, mstatus" : "=r"(x));` | 从 `mstatus` 读出值到变量 | 查询全局中断开关 |
| `asm volatile("li a7, -1; ecall");` | 先写参数寄存器，再执行 `ecall` | 主动制造 yield 自陷 |

需要注意：

- `li a7, -1` 是伪指令，不一定是最终机器指令
- 在当前立即数条件下，汇编器通常会展开为 `addi a7, x0, -1`
- `ecall` 才是真正触发同步异常的机器指令

所以 `yield()` 本质上是：

1. 先在寄存器里放一个“这是 yield”的标志
2. 再执行 `ecall`，让 CPU 进入 trap 流程

---

#### 1.2.2 trap.S 的存在意义与核心功能

`trap.S` 是整个 CTE 机制最底层、最关键的汇编入口。

它的核心意义是：

> 把硬件 trap 现场翻译成 C 可处理的 `Context` 结构，再把 C 决定好的 `Context` 恢复回 CPU，最后 `mret` 返回。

可以把它理解为：

`硬件 trap 世界 <-> C 语言上下文处理世界` 的桥梁

`trap.S` 的核心动作如下：

| 步骤 | 汇编层行为 | 意义 |
| --- | --- | --- |
| 1 | `addi sp, sp, -CONTEXT_SIZE` | 在栈上为 Context 开空间 |
| 2 | `MAP(REGS, PUSH)` | 保存通用寄存器 |
| 3 | `csrr mcause/mstatus/mepc` | 读取 trap 相关 CSR |
| 4 | `STORE ...` | 将 CSR 也写入栈上的 Context |
| 5 | `mv a0, sp` | 把 `Context *` 作为参数传给 C |
| 6 | `call __am_irq_handle` | 进入 C 语言异常/中断分发逻辑 |
| 7 | 恢复 `mstatus/mepc` 和 GPR | 将处理后的 Context 写回 CPU |
| 8 | `mret` | 回到被打断的执行流 |

因此，`trap.S` 不是在“做异常处理策略”，而是在做：

- 现场保存
- 上下文封装
- C/汇编边界切换
- 现场恢复
```
栈上的 Context 布局（从 sp 到 sp+CONTEXT_SIZE）：
高地址 (栈底方向)
┌────────────────────┐  sp + CONTEXT_SIZE
│       mepc          │  ← mret 的返回地址
├────────────────────┤
│      mstatus        │  ← 中断使能状态、特权级
├────────────────────┤
│      mcause         │  ← 为什么进入 trap
├────────────────────┤
│       x31           │
├────────────────────┤
│       x30           │
│       ...           │
├────────────────────┤
│       x1  (ra)      │
├────────────────────┤
│       x0  (zero)    │
└────────────────────┘  ← sp (Context 指针 a0 的值)
```
---

#### 1.2.3 Context 上下文结构体的意义

当前 RISC-V 平台上的 `Context` 定义为：

```c
struct Context {
  uintptr_t gpr[NR_REGS], mcause, mstatus, mepc;
  void *pdir;
};
```

`Context` 的本质不是“CPU 本体”，而是：

> trap 发生那一刻，对 CPU 执行现场做的一份软件快照。

它记录了：

| 字段 | 含义 |
| --- | --- |
| `gpr[]` | 通用寄存器快照 |
| `mcause` | trap 原因 |
| `mstatus` | trap 进入前后的关键状态 |
| `mepc` | trap 返回点 |
| `pdir` | 地址空间/页表扩展预留字段 |

`Context` 存在的意义主要有 3 个：

1. 统一保存 trap 现场
2. 让 C 代码能够读写 trap 状态
3. 让异常处理函数可以决定“恢复哪个上下文”

第三点尤其关键，因为这就为以后调度器的上下文切换留下了接口。

---

#### 1.2.4 CTE 的注册与回调机制

CTE 的关键接口是：

```c
bool cte_init(Context *(*handler)(Event ev, Context *ctx));
```

这个函数签名的正确理解是：

- 参数 `handler` 是一个函数指针
- 这个函数的类型是：

```c
Context *handler(Event ev, Context *ctx)
```

它的语义不是普通通知，而是：

> 当 trap 发生后，把事件 `Event` 和现场 `Context *` 交给该函数处理，并由它返回“最终要恢复的 Context”。

对应的全局静态函数指针：

```c
static Context *(*user_handler)(Event, Context *) = NULL;
```

它的作用是：

- 作为全局回调槽位
- 保存当前注册好的最终异常/中断处理函数

注册过程如下：

| 步骤 | 行为 |
| --- | --- |
| `CTE(simple_trap)` | 宏展开为 `cte_init(simple_trap)` |
| `cte_init(simple_trap)` | 将 `user_handler = simple_trap` |
| trap 发生 | `trap.S -> __am_irq_handle(c)` |
| `__am_irq_handle()` | 调用 `c = user_handler(ev, c)` |
| 当前测试场景 | 实际等价于 `c = simple_trap(ev, c)` |

所以从 `__am_irq_handle()` 到 `simple_trap()` 的跳转，本质上不是“写死调用”，而是：

> 先注册，再通过函数指针回调。

---

#### 1.2.5 CSR 寄存器总表与关键字段

PA3 当前这一部分，最重要的 CSR 包括：

| 寄存器 | 核心作用 | 当前模块中的关键用途 |
| --- | --- | --- |
| `mstatus` | 全局 trap/特权状态控制寄存器 | 控制 `MIE`、保存 `MPIE`、记录 `MPP` |
| `mie` | 机器级中断使能寄存器 | 打开某类中断的使能位，如 `MTIE` |
| `mip` | 机器级中断挂起寄存器 | 记录某类中断当前是否 pending，如 `MTIP` |
| `mtvec` | trap 入口地址寄存器 | 指向 `__am_asm_trap` |
| `mepc` | trap 返回地址寄存器 | 记录恢复执行的位置 |
| `mcause` | trap 原因寄存器 | 标识本次是 `ecall`、timer interrupt 等 |

核心位功能如下：

| 寄存器 | 位 | 名称 | 含义 |
| --- | --- | --- | --- |
| `mstatus` | bit 3 | `MIE` | 机器态全局中断总开关 |
| `mstatus` | bit 7 | `MPIE` | trap 前 `MIE` 的保存位 |
| `mstatus` | bit 12:11 | `MPP` | trap 前特权级保存位 |
| `mie` | bit 7 | `MTIE` | 机器定时器中断使能 |
| `mip` | bit 7 | `MTIP` | 机器定时器中断挂起 |

需要特别区分：

| 名称 | 含义 |
| --- | --- |
| `MTIE` | enable，表示“允许 timer interrupt 进入” |
| `MTIP` | pending，表示“timer interrupt 请求已经挂起” |

---

#### 1.2.6 特权级异常/中断处理中的 CSR 状态迁移

无论是 `yield/ecall` 还是 `timer interrupt`，只要正式进入 `isa_raise_intr()`，都会完成一套类似的 CSR 状态迁移：

| 动作 | 作用 |
| --- | --- |
| `mepc = epc` | 保存 trap 返回地址 |
| `mcause = NO` | 保存 trap 原因号 |
| `MPIE = MIE` | 保存进入 trap 前的全局中断状态 |
| `MIE = 0` | trap 处理中暂时关闭中断 |
| `MPP = M` | 当前进入机器态 trap 处理 |
| `pc = mtvec` | 跳转到 trap 入口 |

这一套动作的意义是：

1. 保存原执行流的恢复信息
2. 给 trap 处理逻辑创造一个受控环境
3. 保证 `mret` 之后能回到正确位置

---

### 1.3 yield 自陷逻辑链

#### 1.3.1 软件入口与处理函数注册

在 `am-tests` 中，`mainargs=i` 对应：

```c
CASE('i', hello_intr, IOE, CTE(simple_trap));
```

逻辑顺序是：

1. `CTE(simple_trap)` 宏展开为 `cte_init(simple_trap)`
2. `cte_init()` 做两件事：
   - `mtvec = __am_asm_trap`
   - `user_handler = simple_trap`
3. 然后再进入 `hello_intr()`

所以，当后面真正发生异常时，系统已经知道：

- trap 入口是谁：`__am_asm_trap`
- 最终事件处理函数是谁：`simple_trap`

---

#### 1.3.2 yield 到 ecall 的执行机制

`hello_intr()` 中进入循环：

```c
while (1) {
  for (volatile int i = 0; i < 10000000; i++) ;
  yield();
}
```

`yield()` 定义为：

```c
void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, -1; ecall");
#endif
}
```

对于当前 `riscv32I` 场景，执行的是：

```asm
li a7, -1
ecall
```

其核心语义是：

1. 先把 `a7 = -1`
2. 再执行 `ecall`

这里 `a7 = -1` 的作用不是触发 trap，而是给 trap handler 一个“这次是 yield”的识别标记。

CPU 为什么最终会执行到 `ecall`？

- 不是某个函数“强行跳转过去”
- 而是 CPU 正常顺序执行 `yield()` 的指令流
- 运行到 `ecall` 这条指令时，自然触发异常

---

#### 1.3.3 ecall 译码与 isa_raise_intr

在 NEMU 中，`ecall` 被如下模式识别：

```c
INSTPAT("0000000 00000 00000 000 00000 11100 11", ecall, N,
  s->dnpc = isa_raise_intr(11, s->pc));
```

这里的关键点：

| 参数 | 含义 |
| --- | --- |
| `11` | M 模式环境调用异常号 |
| `s->pc` | 当前正在执行的 `ecall` 指令地址 |

为什么传的是 `s->pc` 而不是 `s->dnpc`？

因为 `ecall` 属于同步异常，trap 就是由“当前这条指令本身”触发的，所以 `mepc` 应忠实保存出事点，也就是当前 `ecall` 的地址。

接着 `isa_raise_intr()` 完成：

1. `mepc = s->pc`
2. `mcause = 11`
3. 修改 `mstatus`
4. 返回 `mtvec`

返回值 `mtvec` 正是 trap 入口地址，也就是 `__am_asm_trap`。

---

#### 1.3.4 trap.S 保存现场并进入 C 处理流程

进入 `trap.S` 后，主要流程是：

| 步骤 | 作用 |
| --- | --- |
| 栈上开空间 | 为 `Context` 预留内存 |
| 保存 GPR | 冻结当前通用寄存器现场 |
| 保存 `mcause/mstatus/mepc` | 冻结 trap 相关 CSR 现场 |
| `a0 = sp` | 将 `Context *c` 作为参数 |
| `call __am_irq_handle` | 进入 C 级异常/中断分发逻辑 |

此时要明确：

> `Context *c` 并不是某个长期存在的全局结构体，而是 trap.S 在当前栈帧里临时构造出的现场快照指针。

---

#### 1.3.5 __am_irq_handle 到 simple_trap 的回调链

`__am_irq_handle(Context *c)` 是 C 级总入口。

它的职责不是直接处理所有业务，而是：

1. 根据 `c->mcause` 翻译出上层 `Event`
2. 通过 `user_handler` 回调最终注册好的处理函数

`yield/ecall` 场景下的判断逻辑：

```c
case 0xb:
  if (c->gpr[17] == (uintptr_t)-1) {
    ev.event = EVENT_YIELD;
  } else {
    ev.event = EVENT_SYSCALL;
    ev.ref = c->gpr[17];
  }
  c->mepc += 4;
  break;
```

这里最关键的是：

| 动作 | 原因 |
| --- | --- |
| 检查 `a7 == -1` | 判断这次 `ecall` 是 yield 还是普通 syscall |
| `c->mepc += 4` | 避免 `mret` 回去再次执行同一条 `ecall` |

然后执行：

```c
c = user_handler(ev, c);
```

而由于 earlier 已经有：

```c
user_handler = simple_trap;
```

所以这里等价于：

```c
c = simple_trap(ev, c);
```

`simple_trap()` 在当前测试中的行为是：

- `EVENT_YIELD` -> 打印 `y`
- 返回原来的 `ctx`

返回原来的 `ctx` 的含义是：

> 本次 trap 处理完后，继续恢复当前这个上下文，不做任务切换。

---

#### 1.3.6 yield 返回路径总结

完整链条可以总结为：

| 阶段 | 行为 |
| --- | --- |
| 注册阶段 | `cte_init(simple_trap)`，保存 trap 入口与用户 handler |
| 软件触发阶段 | `yield()` 执行 `li a7, -1; ecall` |
| 译码阶段 | `ecall` 命中，调用 `isa_raise_intr(11, s->pc)` |
| trap 受理阶段 | 保存 `mepc/mcause/mstatus`，返回 `mtvec` |
| trap 汇编阶段 | `trap.S` 构造 `Context`，调用 `__am_irq_handle(c)` |
| C 分发阶段 | `mcause=11 + a7=-1` 翻译成 `EVENT_YIELD` |
| 用户处理阶段 | `simple_trap(ev, c)` 打印 `y`，返回原上下文 |
| 返回阶段 | `trap.S` 恢复现场，`mret` 回到 `ecall` 下一条指令 |

---

### 1.4 timer 时钟中断逻辑链

#### 1.4.1 时钟设备如何提出中断请求

在 NEMU 的设备层，时钟初始化时注册了回调：

```c
static void timer_intr() {
  if (nemu_state.state == NEMU_RUNNING) {
    extern void dev_raise_intr();
    dev_raise_intr();
  }
}
```

而 `dev_raise_intr()` 的核心是：

```c
cpu.csr[CSR_IDX_mip] |= (1 << 7);
```

也就是：

> 将 `mip.MTIP` 置 1，表示机器定时器中断请求 pending。

这一层的本质角色是：

- 设备层提出中断请求
- 通过修改 pending 位把请求暴露给 CPU

---

#### 1.4.2 CPU 为什么在每条指令后轮询

在 NEMU 的 CPU 主执行循环中：

```c
IFDEF(CONFIG_DEVICE, device_update());
word_t intr_no = isa_query_intr();
if (intr_no != INTR_EMPTY) {
  cpu.pc = isa_raise_intr(intr_no, s.dnpc);
  s.dnpc = cpu.pc;
}
```

其含义是：

1. 当前指令先正常执行完
2. 更新设备状态
3. 轮询检查是否有外部中断请求
4. 若存在，则转入 trap 处理流程

这是一种“在指令边界检查中断”的简化 CPU 模型。

---

#### 1.4.3 isa_query_intr 的判定逻辑

`isa_query_intr()` 对 timer interrupt 的判定条件是：

1. `mstatus.MIE == 1`
2. `mie.MTIE == 1`
3. `mip.MTIP == 1`

分别对应：

| 条件 | 含义 |
| --- | --- |
| `mstatus.MIE` | 全局中断总开关已开启 |
| `mie.MTIE` | 机器定时器中断被允许进入 |
| `mip.MTIP` | 当前已经存在时钟中断挂起请求 |

只有这三者同时成立，才返回：

```c
0x80000007
```

即 `Machine timer interrupt`

---

#### 1.4.4 为什么时钟中断传入的是 s.dnpc

这是异步中断与同步异常最重要的区别之一。

时钟中断是在：

> 当前指令已经执行完成之后，CPU 在指令边界检查到外部中断请求。

因此保存到 `mepc` 的应该不是“当前指令地址”，而是：

> 本来下一条应该执行的 PC

也就是 `s.dnpc`。

对比：

| 类型 | 传给 `isa_raise_intr()` 的 PC | 原因 |
| --- | --- | --- |
| `ecall` 同步异常 | `s->pc` | 当前指令本身触发异常 |
| `timer interrupt` 异步中断 | `s.dnpc` | 当前指令已完成，中断在指令边界进入 |

---

#### 1.4.5 isa_raise_intr 受理时钟中断

当 `isa_query_intr()` 返回 `0x80000007` 后，CPU 执行：

```c
cpu.pc = isa_raise_intr(intr_no, s.dnpc);
```

在 `isa_raise_intr()` 中，时钟中断和 `ecall` 一样都会：

1. 保存 `mepc`
2. 保存 `mcause`
3. 修改 `mstatus`
4. 返回 `mtvec`

除此之外，当前项目还需要额外做一件重要的事：

> 清掉本次 timer interrupt 的 pending 位 `mip.MTIP`

原因是：

- 如果不清除
- 那么 `isa_query_intr()` 下次还会继续看到同一个 pending
- 就会无限重复进入 timer interrupt

所以在当前简化实现中，合理做法是：

- 在 `isa_raise_intr()` 中根据 `NO` 做 `switch`
- 对 `IRQ_M_TIMER` 清 `MTIP`

其语义是：

> CPU 已经正式受理这次中断，因此本次挂起请求应当被消费掉。

---

#### 1.4.6 timer interrupt 返回路径总结

完整链条如下：

| 阶段 | 行为 |
| --- | --- |
| 初始化阶段 | `cte_init()` 打开 `mie.MTIE`，`iset(1)` 打开 `mstatus.MIE` |
| 设备请求阶段 | `timer_intr()` 调 `dev_raise_intr()`，置位 `mip.MTIP` |
| CPU 轮询阶段 | 每条指令结束后调用 `isa_query_intr()` |
| 判定阶段 | 同时检查 `MIE`、`MTIE`、`MTIP` |
| trap 受理阶段 | `isa_raise_intr(0x80000007, s.dnpc)`，保存现场并清理 pending |
| trap 汇编阶段 | `trap.S` 构造 `Context` |
| C 分发阶段 | `__am_irq_handle()` 将 `mcause` 翻译为 `EVENT_IRQ_TIMER` |
| 用户处理阶段 | `simple_trap()` 打印 `t` 并返回原上下文 |
| 返回阶段 | `trap.S` 恢复现场，`mret` 返回到原本下一条要执行的指令 |

---

### 1.5 yield 与 timer interrupt 对照表

| 对比项 | `yield/ecall` | `timer interrupt` |
| --- | --- | --- |
| 类型 | 同步异常 | 异步中断 |
| 触发源 | 当前执行流主动执行 `ecall` | 外设设备层提出请求 |
| 软件入口 | `yield()` | `timer_intr() -> dev_raise_intr()` |
| trap 进入时机 | 执行到当前 `ecall` 时立即进入 | 当前指令执行完后在指令边界检查到 |
| `mcause` | `11` | `0x80000007` |
| 传给 `isa_raise_intr()` 的 PC | `s->pc` | `s.dnpc` |
| `mepc` 初始记录 | 当前 `ecall` 地址 | 原本下一条应执行地址 |
| 是否需要修正 `mepc` | 需要，`c->mepc += 4` | 一般不需要 |
| 是否依赖 `mip` pending 位 | 否 | 是，依赖 `mip.MTIP` |
| 是否依赖 `mie` 某一路使能 | 否 | 是，依赖 `mie.MTIE` |
| 是否依赖 `mstatus.MIE` | 否，属于同步异常路径 | 是，全局中断总开关 |
| 处理后是否要清 pending | 不涉及 | 需要清 `MTIP` |
| 上层事件 | `EVENT_YIELD` | `EVENT_IRQ_TIMER` |
| 测试现象 | 打印 `y` | 打印 `t` |

---

### 1.6 对话问题标准回答

#### 1.6.1 为什么同步异常传 s->pc，而外部中断传 s.dnpc

**问题：**

在 `isa_raise_intr(NO, epc)` 中，为什么：

- 同步异常传入的是 `s->pc`
- 外部中断传入的是 `s.dnpc`

这两个 `epc` 分别代表什么控制流语义？

**标准回答：**

`isa_raise_intr()` 的 `epc` 参数，本质上表示：

> trap 处理结束后，CPU 应当从哪一个指令地址继续恢复执行。

但这个恢复点必须服从 trap 的触发时机，所以同步异常和异步中断不同。

**同步异常：传 `s->pc`**

例如 `yield -> ecall`。

因为同步异常是由当前正在执行的指令本身触发的，所以异常发生点就是当前指令地址。  
此时传给 `isa_raise_intr()` 的 `epc` 应该是：

```c
epc = s->pc
```

它表示：

- trap 是在当前指令执行过程中产生的
- 硬件应先忠实记录“出事点”
- 后续再由软件决定是否跳过该指令

所以对 `ecall` 这类同步异常：

- 先保存当前 `pc` 到 `mepc`
- 再在 `__am_irq_handle()` 中根据异常类型执行：

```c
c->mepc += 4;
```

这表示“本次异常已经处理完，返回时跳过当前 `ecall`”。

**异步中断：传 `s.dnpc`**

例如 timer interrupt。

因为异步中断不是由当前指令本身触发，而是：

> 当前指令已经执行完成之后，CPU 在指令边界检查到有外部中断请求。

所以这时 trap 返回后，不应该回到“已经执行完的当前指令”，而应该回到：

> 原本下一条应该执行的指令地址

因此传给 `isa_raise_intr()` 的 `epc` 应该是：

```c
epc = s.dnpc
```

它表示：

- 当前指令的执行闭环已经结束
- 外部中断是在指令完成后被 CPU 接收
- trap 返回后，应继续执行原本下一条指令

所以对外部中断：

- 保存的是 `dnpc`
- 通常不需要像 `ecall` 一样再手动修正 `mepc`

**统一设计原则**

`epc` 记录的是：

> trap 被 CPU 正式接收时，正确的恢复执行点。

具体落地为：

| 场景 | trap 触发时机 | 传入 `isa_raise_intr()` 的 `epc` | 原因 |
| --- | --- | --- | --- |
| 同步异常 | 当前指令执行过程中触发 | `s->pc` | 当前指令就是异常发生点 |
| 异步中断 | 当前指令执行结束后被轮询接收 | `s.dnpc` | 应恢复到原本下一条指令 |

---

#### 1.6.2 关于 Context、cte_init、user_handler

**问题：`Context`、`cte_init()`、`user_handler` 各自是什么关系？**

**标准回答：**

| 对象 | 角色 |
| --- | --- |
| `Context` | trap 现场的软件化快照 |
| `cte_init(handler)` | 注册 trap 入口与最终 C 处理函数 |
| `user_handler` | 保存已注册 handler 的全局静态函数指针槽位 |

调用关系是：

1. `cte_init(simple_trap)` 执行 `user_handler = simple_trap`
2. trap 发生后，`trap.S` 调 `__am_irq_handle(c)`
3. `__am_irq_handle()` 再执行 `c = user_handler(ev, c)`
4. 当前场景下实际等价于 `c = simple_trap(ev, c)`

---

#### 1.6.3 关于 mepc 为什么最初保存 trap 发生点

**问题：为什么 `mepc` 最初不直接保存下一条指令地址，而是先保存 trap 发生点？**

**标准回答：**

因为硬件的职责是：

> 忠实记录 trap 发生点

而不是替软件提前决定“处理完后该去哪”。

不同 trap 的返回策略不同：

| trap 类型 | 返回策略 |
| --- | --- |
| `ecall` | 通常要跳过当前指令，所以软件再 `+4` |
| 某些 fault | 可能要重试当前指令 |
| timer interrupt | 通常回到原本下一条执行位置即可 |

因此更合理的分工是：

1. 硬件/底层 trap 入口先保存真实发生点
2. 软件再根据 trap 类型决定是否修正返回位置

这就是为什么：

- `ecall` 会先记当前指令地址
- 后续再由 `__am_irq_handle()` 执行 `c->mepc += 4`

---

#### 1.6.4 关于 timer interrupt 中 pending 与 enable 的关系

**问题：为什么既要 `MTIP` 又要 `MTIE`，还要 `MIE`？**

**标准回答：**

它们不是重复信号，而是不同层级的控制：

| 位 | 层级 | 作用 |
| --- | --- | --- |
| `mip.MTIP` | 请求层 | 当前是否真的有 timer interrupt pending |
| `mie.MTIE` | 通道层 | 是否允许 timer interrupt 进入 CPU |
| `mstatus.MIE` | 全局层 | 当前 CPU 是否整体接受中断 |

只有三者同时满足，CPU 才会真正响应时钟中断。

---

#### 1.6.5 关于 NEMU 软件模拟与真实硬件的区别

**问题：为什么 NEMU 中是软件改 CSR，而真实硬件不是这样？**

**标准回答：**

NEMU 是软件模拟器，所以很多硬件电路行为是用 C 代码显式实现的。

| 场景 | NEMU 中的表现 | 真实硬件中的表现 |
| --- | --- | --- |
| timer 到期 | `timer_intr()` 被调用 | 定时器电路计数到期 |
| pending 置位 | `dev_raise_intr()` 修改 `mip.MTIP` | 中断控制逻辑拉高 pending |
| CPU 检查中断 | `isa_query_intr()` 轮询 | 硬件在指令边界/流水级检测 irq |
| trap 进入 | `isa_raise_intr()` 改 CSR | 硬件自动写 `mepc/mcause/mstatus` |

所以 NEMU 不是“伪造逻辑”，而是：

> 用软件明确实现硬件本应完成的状态迁移。

---

#### 1.6.6 关于为什么要返回 Context *

**问题：为什么 `simple_trap()` / `user_handler()` 还要返回 `Context *`？**

**标准回答：**

因为 trap 处理的目标不只是“处理事件”，还包括：

> 决定最终恢复哪个执行上下文

返回原来的 `ctx` 表示：

- 不切换任务
- 恢复当前 trap 前的执行流

返回另一个 `Context *` 则表示：

- 发生调度
- trap 返回后恢复的是另一个线程/任务

这正是 CTE 为以后上下文切换预留的核心接口。

---

#### 1.6.7 关于 MTIP、MTIE、MIE 能否集中到一个模块统一处理

**问题：像 `MTIP`、`MTIE`、`MIE` 这些位，能不能集中到一个专门的模块里统一处理？当前为什么分散在设备层、CTE 初始化、CPU 查询这些不同代码位置？**

**标准回答：**

可以从两个层面来回答。

**从体系结构语义看，分散是合理的。**

这三个位虽然都和“中断”有关，但它们的职责并不属于同一个层级：

| 位 | 所属语义层 | 含义 |
| --- | --- | --- |
| `mip.MTIP` | 设备/请求层 | 当前是否真的有 timer interrupt 请求挂起 |
| `mie.MTIE` | 中断类型使能层 | 是否允许 timer interrupt 这一类中断进入 |
| `mstatus.MIE` | CPU 全局策略层 | 当前 CPU 是否整体接受中断 |

所以当前实现里把它们放在不同位置是有语义原因的：

| 代码位置 | 作用 |
| --- | --- |
| `dev_raise_intr()` | 模拟设备提出中断请求，置位 `MTIP` |
| `cte_init()` | 初始化 trap 系统时打开 `MTIE` |
| `iset(true)` | 软件按当前执行语义打开全局 `MIE` |
| `isa_query_intr()` | 在 CPU 侧统一检查三者是否同时满足 |

也就是说，这不是“为了测试方便故意分散”，而是：

> 它们本来就分别代表设备状态、某一类中断的许可、以及 CPU 当前的全局接受策略。

**从工程实现看，也完全可以更模块化。**

更正规的实现里，通常可以抽象出一个专门的中断控制模块，统一负责：

1. 接收设备中断请求输入
2. 维护 pending 状态
3. 结合 enable/priority 做仲裁
4. 向 CPU 输出最终 irq 信号

所以如果从工程整洁性出发，完全可以设计成：

- 一个“中断控制/平台中断”模块维护 pending
- CPU 侧只读取汇总后的中断状态

但在当前 NEMU/AM 的教学实现里，采用分散式实现有两个优势：

1. 每一层职责更直白，便于理解
2. 容易把“设备提出请求”和“CPU 接受中断”区分开

因此最终结论是：

> 概念上它们不属于同一层；工程上可以再封装成统一模块；当前代码采用分散实现，是一种更直接、更适合教学观察的设计。

---

#### 1.6.8 关于为什么把中断判定和中断受理拆成两个阶段

**问题：为什么要先 `isa_query_intr()` 判断有没有中断，再 `isa_raise_intr()` 正式受理？能不能直接一个函数做完？**

**标准回答：**

逻辑上当然可以把两者合并，但拆开更清晰，也更贴近硬件语义。

当前拆分后：

| 阶段 | 函数 | 职责 |
| --- | --- | --- |
| 判定阶段 | `isa_query_intr()` | 只回答“现在有没有一个可进入的中断，若有它的编号是什么” |
| 受理阶段 | `isa_raise_intr(NO, epc)` | 真正执行 trap 进入动作，保存 `mepc/mcause/mstatus`，并做 pending 清理等副作用 |

这样拆开的好处有：

1. **职责单一**
   - `query` 不修改 trap 状态
   - `raise` 才真正改变 CPU 状态

2. **语义更接近硬件**
   - 硬件里也可以理解成：先检测是否有合法 irq，再进入 trap 序列

3. **副作用控制更稳**
   - 如果在“只是查询”的阶段就清 pending、改 CSR，很容易把“看到了中断”和“真的接收了中断”混在一起
   - 当前把清 `MTIP` 放到 `isa_raise_intr()`，语义就很干净：只有真正受理时才消费这次请求

4. **便于以后扩展**
   - 后面如果加入优先级、中断嵌套、不同来源仲裁，`query` 层可以专门负责选择“谁先来”
   - `raise` 层仍只负责正式进入 trap

所以：

> `isa_query_intr()` 是“报告候选中断”，`isa_raise_intr()` 是“正式接收中断”。  
> 这两个阶段拆开，比一个函数既查又改状态，更容易保证语义清晰。

---

### 1.7 最终总结

PA3 当前这部分最核心的认知可以压缩成下面几句话：

1. `trap.S` 是整个异常/中断系统的地基，它负责把硬件 trap 现场打包成 `Context`，再把处理后的 `Context` 恢复回 CPU。
2. `Context` 是一次 trap 现场的软件快照，既让 C 代码能看懂 trap 状态，也让调度器能够决定恢复哪个上下文。
3. `cte_init()` 的真正作用不只是“开中断”，而是同时：
   - 注册 trap 汇编入口 `mtvec`
   - 注册最终事件处理函数 `user_handler`
4. `yield/ecall` 是同步异常路径：当前指令本身触发 trap，所以记录 `s->pc`，并在软件层手动 `mepc += 4`。
5. `timer interrupt` 是异步中断路径：当前指令执行完后才进入 trap，所以记录 `s.dnpc`，并且需要消费/清除 pending 位。
6. `__am_irq_handle()` 并不是最终处理函数，它是 `mcause -> Event` 的翻译与分发层；真正的业务处理由 `user_handler(ev, c)` 回调到 `simple_trap()`。
7. 同步异常与异步中断虽然触发方式不同，但进入 `trap.S` 之后，后半段的上下文保存、事件翻译、回调处理、恢复返回机制是统一的。

因此，整个 PA3 当前模块的本质就是：

> 通过 `Context + trap.S + cte_init + __am_irq_handle + user_handler` 这套统一框架，把不同来源的 trap 都收束到同一个可编程的软件处理模型中。
