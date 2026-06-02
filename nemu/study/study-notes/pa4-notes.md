# PA4 Notes

## 目录

- [第 1 章 上下文切换与进程调度](#第-1-章-上下文切换与进程调度)
  - [1.1 本模块总览](#11-本模块总览)
  - [1.2 核心数据结构与内存布局](#12-核心数据结构与内存布局)
    - [1.2.1 ELF 内存布局与 linker.ld](#121-elf-内存布局与-linkerld)
    - [1.2.2 Context 结构体——CPU 现场快照](#122-context-结构体cpu-现场快照)
    - [1.2.3 RISC-V 通用寄存器 ABI 对照表](#123-risc-v-通用寄存器-abi-对照表)
    - [1.2.4 PCB 联合体——进程控制块设计](#124-pcb-联合体进程控制块设计)
  - [1.3 kcontext —— 伪造的"出生证明"](#13-kcontext--伪造的出生证明)
    - [1.3.1 两种 Context 的本质区别](#131-两种-context-的本质区别)
    - [1.3.2 kcontext 的实现原理](#132-kcontext-的实现原理)
    - [1.3.3 kcontext 需要精确填写的字段](#133-kcontext-需要精确填写的字段)
    - [1.3.4 kcontext 三个参数详解](#134-kcontext-三个参数详解)
  - [1.4 进程切换完整流水线](#14-进程切换完整流水线)
    - [1.4.1 统一处理流水线架构](#141-统一处理流水线架构)
    - [1.4.2 trap.S 中的 sp 切换——进程切换的"扳道岔"](#142-traps-中的-sp-切换进程切换的扳道岔)
    - [1.4.3 __am_irq_handle —— 翻译官](#143-__am_irq_handle--翻译官)
    - [1.4.4 schedule —— 决策者](#144-schedule--决策者)
    - [1.4.5 完整走演：进程 A → 进程 B 寄存器级追踪](#145-完整走演进程-a--进程-b-寄存器级追踪)
  - [1.5 协作式与抢占式调度合并](#15-协作式与抢占式调度合并)
    - [1.5.1 两种触发路径对比](#151-两种触发路径对比)
    - [1.5.2 设计实现：yield + timer 混合调度](#152-设计实现yield--timer-混合调度)
    - [1.5.3 完整执行时序演示](#153-完整执行时序演示)
  - [1.6 对话问题标准回答](#16-对话问题标准回答)
    - [1.6.1 为什么异常处理机制是上下文切换的基础设施](#161-为什么异常处理机制是上下文切换的基础设施)
    - [1.6.2 Context* 返回值的核心意义是什么](#162-context-返回值的核心意义是什么)
    - [1.6.3 mv sp, a0 为什么能完成栈切换](#163-mv-sp-a0-为什么能完成栈切换)
    - [1.6.4 kcontext 和 trap.S 保存的 Context 有什么不同](#164-kcontext-和-traps-保存的-context-有什么不同)
    - [1.6.5 调度是硬件还是软件决定的](#165-调度是硬件还是软件决定的)
    - [1.6.6 为什么 main 进程永不会被调度回来](#166-为什么-main-进程永不会被调度回来)
    - [1.6.7 ev.cause 和 ev.event 的区别](#167-evcause-和-evevent-的区别)
  - [1.7 最终总结](#17-最终总结)
- [第 2 章 PA4.2 在 NEMU 中启动 RT-Thread](#第-2-章-pa42-在-nemu-中启动-rt-thread)
  - [2.1 最终结论](#21-最终结论)
  - [2.2 当前工程已有基础](#22-当前工程已有基础)
  - [2.3 RT-Thread 需要 BSP 提供什么](#23-rt-thread-需要-bsp-提供什么)
  - [2.4 第一轮运行现象与根因定位](#24-第一轮运行现象与根因定位)
  - [2.5 关键修改一：补齐 RT-Thread 上下文切换](#25-关键修改一补齐-rt-thread-上下文切换)
  - [2.6 关键修改二：修正中断开关 ABI](#26-关键修改二修正中断开关-abi)
  - [2.7 关键修改三：修正 AM RISC-V trap frame 大小](#27-关键修改三修正-am-risc-v-trap-frame-大小)
  - [2.8 关键修改四：让 make run 直接进入 guest](#28-关键修改四让-make-run-直接进入-guest)
  - [2.9 完整启动链路复盘](#29-完整启动链路复盘)
  - [2.10 验证方法与结果](#210-验证方法与结果)
  - [2.11 设计注意事项与剩余非阻塞项](#211-设计注意事项与剩余非阻塞项)

---

## 第 1 章 上下文切换与进程调度

### 1.1 本模块总览

本模块围绕 **上下文切换（Context Switch）** 这一操作系统最核心的机制展开，将 PA3 中分别实现的 **yield（协作式）** 和 **timer interrupt（抢占式）** 统一到同一条 `schedule()` 调度框架中。

核心认知链条：

```
异常/中断机制（PA3）  →  Context 保存现场  →  Event 翻译分发
                                                    ↓
                     mret ← trap.S 恢复 ← schedule() 调度决策
```

本模块解决的问题：

| 问题 | 答案 |
|------|------|
| trap 现场怎么变成"可切换的进程状态"？ | `struct Context` 保存完整 CPU 快照 |
| 新进程从没运行过，哪来的 Context？ | `kcontext()` 伪造初始 Context |
| 进程切换在硬件层面是如何发生的？ | trap.S 中 `mv sp, a0` 切换栈 → 恢复不同 Context → `mret` |
| 协作式和抢占式如何统一？ | 共用同一条 trap.S → `__am_irq_handle` → `schedule()` 管线 |

---

### 1.2 核心数据结构与内存布局

#### 1.2.1 ELF 内存布局与 linker.ld

链接脚本 `abstract-machine/scripts/linker.ld` 定义了进程的完整内存空间布局：

```
┌──────────────────────┐  ← 低地址 / _pmem_start
│  .text     代码段     │  ← 程序指令
├──────────────────────┤
│  .rodata   只读数据   │  ← 常量、字符串字面量
├──────────────────────┤
│  .data     数据段     │  ← 已初始化的全局/静态变量
├──────────────────────┤
│  .bss      BSS段     │  ← 未初始化的全局/静态变量（启动时清零）
├──────────────────────┤  ← _stack_top (页对齐)
│                      │
│      栈区 (Stack)     │  ← 0x8000 = 32KB，栈向下增长 ↓
│                      │
├──────────────────────┤  ← _stack_pointer (初始栈顶)
│                      │
│      堆区 (Heap)      │  ← 堆向上增长 ↑，直到 PMEM_END
│                      │
└──────────────────────┘  ← PMEM_END (物理内存末尾)
```

关键理解：**栈和堆在 ELF 中没有对应的 Section**——它们只是通过链接器符号（`_stack_pointer`, `_heap_start`）标记了起始地址，是**运行时概念**。ELf 加载后代码段、数据段、BSS 段占据了低地址区域，剩余空间留给栈和堆动态使用。

#### 1.2.2 Context 结构体——CPU 现场快照

```c
// abstract-machine/am/include/arch/riscv.h
struct Context {
  uintptr_t gpr[NR_REGS],  // 32 个通用寄存器 (x0~x31)
           mcause,          // 触发 trap 的原因
           mstatus,         // 机器状态（中断使能、特权级等）
           mepc;            // 异常返回地址（mret 跳转目标）
  void *pdir;               // 页表指针（VME 用）
};
```

**Context 的本质**：它是 CPU 在 trap 发生那一瞬间的**完整状态快照**。

- 保存 Context = **冻结**进程
- 恢复 Context = **解冻**进程

栈上的 Context 布局（从 sp 到 sp+CONTEXT_SIZE，共 140 字节）：

```
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

#### 1.2.3 RISC-V 通用寄存器 ABI 对照表

| 寄存器 | ABI 名 | 类型 | 用途 | 调用者保存？ |
|--------|--------|------|------|:--:|
| `x0` | `zero` | — | 硬连线为 0，写入被忽略 | — |
| `x1` | `ra` | 返回地址 | 函数调用时保存返回地址 | ✅ |
| `x2` | `sp` | 栈指针 | 指向当前栈顶 | — |
| `x3` | `gp` | 全局指针 | 指向全局数据段基址 | — |
| `x4` | `tp` | 线程指针 | 指向线程局部存储 (TLS) | — |
| `x5` | `t0` | 临时 | 临时变量，调用后可被覆盖 | ✅ |
| `x6` | `t1` | 临时 | 同上 | ✅ |
| `x7` | `t2` | 临时 | 同上 | ✅ |
| `x8` | `s0`/`fp` | 帧指针 | 保存寄存器，通常用作栈帧指针 | ❌ |
| `x9` | `s1` | 保存 | 被调用者必须保存/恢复 | ❌ |
| `x10` | `a0` | 参数/返回值 | **第 1 个函数参数 + 返回值** | ✅ |
| `x11` | `a1` | 参数/返回值 | 第 2 个函数参数 + 大返回值辅助 | ✅ |
| `x12` | `a2` | 参数 | 第 3 个函数参数 | ✅ |
| `x13` | `a3` | 参数 | 第 4 个函数参数 | ✅ |
| `x14` | `a4` | 参数 | 第 5 个函数参数 | ✅ |
| `x15` | `a5` | 参数 | 第 6 个函数参数 | ✅ |
| `x16` | `a6` | 参数 | 第 7 个函数参数 | ✅ |
| `x17` | `a7` | 参数 | 第 8 个函数参数; **yield 中用于传递 -1 标记** | ✅ |
| `x18` | `s2` | 保存 | 被调用者必须保存/恢复 | ❌ |
| `x19` | `s3` | 保存 | 同上 | ❌ |
| `x20` | `s4` | 保存 | 同上 | ❌ |
| `x21` | `s5` | 保存 | 同上 | ❌ |
| `x22` | `s6` | 保存 | 同上 | ❌ |
| `x23` | `s7` | 保存 | 同上 | ❌ |
| `x24` | `s8` | 保存 | 同上 | ❌ |
| `x25` | `s9` | 保存 | 同上 | ❌ |
| `x26` | `s10` | 保存 | 同上 | ❌ |
| `x27` | `s11` | 保存 | 同上 | ❌ |
| `x28` | `t3` | 临时 | 临时变量，调用后可被覆盖 | ✅ |
| `x29` | `t4` | 临时 | 同上 | ✅ |
| `x30` | `t5` | 临时 | 同上 | ✅ |
| `x31` | `t6` | 临时 | 同上 | ✅ |

**项目中使用到的寄存器**：

| 寄存器 | 项目中的用途 |
|--------|-------------|
| `x2` (`sp`) | trap.S 分配栈帧；kcontext 中 `gpr[2] = 栈顶` |
| `x10` (`a0`) | trap.S 传 Context 指针给 C 函数；kcontext 中 `gpr[10] = arg` 传参数给新进程 |
| `x17` (`a7`) | `yield()` 中 `li a7, -1`；`__am_irq_handle` 中读 `gpr[17]` 判断是 yield 还是 syscall |

#### 1.2.4 PCB 联合体——进程控制块设计

```c
#define STACK_SIZE (4096 * 8)  // 32KB

typedef union {
  uint8_t stack[STACK_SIZE];   // 32KB 栈空间
  struct { Context *cp; };     // 指向被挂起时的 Context 指针
} PCB;
```

**union 的巧妙之处**：

| 成员 | 大小 | 用途 |
|------|------|------|
| `stack[]` | 32KB | 进程的运行时栈 |
| `cp` | 4/8 字节 | 进程被中断时 Context 指针的存储槽 |

`cp` 复用栈最底部的 4/8 字节——进程**运行时**这 4/8 字节正常当栈用；进程**被挂起时**被覆写为 Context 指针。两者永不冲突。

PCB 内存布局：

```
PCB 联合体总长度 = STACK_SIZE = 32KB

&pcb[i] (kstack.start)                          &pcb[i] + 1 (kstack.end)
  ↓                                                 ↓
┌────────────────────────────────────────────────────┐
│  cp (4/8B) ← 复用栈底                                │
├────────────────────────────────────────────────────┤
│                                                    │
│              可用栈空间 (从高向低增长)                 │
│                                                    │
├────────────────────────────────────────────────────┤ ← kstack.end - 140
│             Context (140B) ← kcontext 放这里          │
└────────────────────────────────────────────────────┘ ← kstack.end
```

进程被中断时，`schedule()` 执行 `current->cp = prev`，把 Context 指针存进 PCB 开头的 `cp` 槽。进程被恢复时，`sp` 指向栈顶，`cp` 这几个字节自然回到栈的正常用途（被 `sp` 下方的栈帧覆盖也无所谓）。

---

### 1.3 kcontext —— 伪造的"出生证明"

#### 1.3.1 两种 Context 的本质区别

| | trap.S 保存的 Context | kcontext 创建的 Context |
|------|-------------|--------------|
| **来源** | CPU 真的执行到一半，被 ecall/timer 打断 | 进程从未运行过，手工捏造 |
| **数据** | 被打断那一刻 CPU 寄存器的真实值 | 手工填写的"假"值 |
| **mepc** | 被打断的指令地址 | `entry` 函数地址 |
| **sp** | 被打断时的真实栈指针 | 新进程栈顶 `kstack.end` |
| **a0** | 被打断时的真实值 | 传给入口函数的参数 `arg` |
| **本质** | **拍照**——记录真实发生过的事 | **伪造护照**——捏造一个从没存在过的"过去" |
| **生命周期** | 进程每次被中断都产生新的 | 只在进程创建时用一次 |

**第一次是假的，之后就全是真的了**：

```
pcb[0].cp 的生命周期：

  kcontext 后:  pcb[0].cp = 伪造的 Context_A  ← 软件捏造，"出厂设置"
       │
       ▼  第一次被 schedule 选中，mret 到 f(1)
       │
       ▼  f(1) 第一次被中断（yield 或 timer）
       │  trap.S 保存真实 Context_A
  schedule:     pcb[0].cp = 真实的 Context_A  ← 硬件快照！
       │
       ▼  以后每次中断
       │  trap.S 保存新的真实 Context_A
  schedule:     pcb[0].cp = 更新的真实 Context_A  ← 持续更新
```

#### 1.3.2 kcontext 的实现原理

```c
Context* kcontext(Area kstack, void (*entry)(void *), void *arg) {
  // ① 在栈的顶端（高地址）放置 Context 结构体
  Context* ctx = kstack.end - sizeof(Context);   // 放在栈内部顶端，不越界
  
  // ② 全部清零
  *ctx = (Context){0};

  // ③ 伪造关键寄存器 —— 骗 trap.S 的恢复流程
  ctx->mepc    = (uintptr_t)entry;   // mret 后跳到 entry 函数
  ctx->mstatus = MPP_MIE;            // M-mode, MIE=1（使能时钟中断）
  ctx->gpr[2]  = (uintptr_t)kstack.end;  // sp = 栈顶
  ctx->gpr[10] = (uintptr_t)arg;     // a0 = 第一个参数

  return ctx;  // 返回伪造的 Context 指针
}
```

**为什么 Context 放在 `kstack.end - sizeof(Context)` 而不是 `kstack.start`？**

| 方案 | 写法 | 问题 |
|------|------|------|
| 放栈底 | `ctx = kstack.start` | Context 在低地址，栈从高向低增长，栈会**踩坏** Context |
| 放栈顶外 | `ctx = kstack.end` | **越界写入**，破坏相邻 PCB 的内存 |
| **放栈顶内** ✅ | `ctx = kstack.end - sizeof(Context)` | Context 在栈最高处，栈向下增长，永不冲突 |

#### 1.3.3 kcontext 需要精确填写的字段

从 trap.S 恢复代码反推：哪些字段被读取？

```asm
  LOAD t1, OFFSET_STATUS(sp)    # ① 读 Context.mstatus → 写 CSR
  LOAD t2, OFFSET_EPC(sp)       # ② 读 Context.mepc   → 写 CSR
  MAP(REGS, POP)                # ③ 读全部 gpr[0..31] → 恢复寄存器
  addi sp, sp, CONTEXT_SIZE     # sp += 140
  mret                          # pc ← mepc
```

| Context 字段 | trap.S 读了？ | 必须精确？ | 取值 |
|-------------|:--:|:--:|------|
| `gpr[2]` (sp) | ✅ | ✅ 必须 | `kstack.end` — 新进程栈顶 |
| `gpr[10]` (a0) | ✅ | ✅ 必须 | `arg` — 传给 entry 的参数 |
| `mepc` | ✅ | ✅ 必须 | `entry` — mret 的目标地址 |
| `mstatus` | ✅ | ✅ 必须 | `MPP_MIE` — M-mode + 开中断 |
| 其余 gpr | ✅ | ❌ 填 0 即可 | 新进程从 entry 第一行开始不依赖它们 |
| `mcause` | ❌ | ❌ | 恢复阶段根本没读 |
| `pdir` | ❌ | ❌ | CTE 不碰这个字段 |

**`mstatus = 0x1888` 的含义**：

```
0x1888 = 0b 0001 1000 1000 1000

bit 3  (MIE)  = 1   ← 全局中断打开（让新进程能响应时钟中断）
bit 7  (MPIE) = 1   ← 进入 trap 前 MIE 是开的
bit 11-12 (MPP)= 3  ← M-mode
```

**为什么 MIE 必须设为 1？** 如果 MIE=0，时钟中断的 `isa_query_intr()` 第一步检查 `mstatus.MIE==1` 就失败，进程永远不会被时钟中断抢占。

#### 1.3.4 kcontext 三个参数详解

```c
pcb[0].cp = kcontext((Area){ pcb[0].stack, &pcb[0] + 1 }, f, (void *)1L);
//                    └──────── 参数1：Area 栈区间 ───────┘  参数2  参数3
```

| 参数 | 值 | 类型 | 含义 |
|------|-----|------|------|
| 参数1 | `(Area){ pcb[0].stack, &pcb[0] + 1 }` | `Area` | 用复合字面量描述 32KB 栈区间 |
| 参数2 | `f` | `void (*)(void *)` | 新进程的入口函数 |
| 参数3 | `(void *)1L` | `void *` | 传给入口函数的参数 |

**参数1 详解**：

```c
kstack.start = pcb[0].stack    // 数组首地址 = PCB 起始（低地址）
kstack.end   = &pcb[0] + 1     // PCB* 指针 +1 = 越过整个 32KB（高地址）
```

`&pcb[0] + 1` 的指针运算：`&pcb[0]` 类型是 `PCB*`，`+1` 表示 `(char*)&pcb[0] + sizeof(PCB)` = `(char*)&pcb[0] + 32768`，恰好是栈区末尾的下一字节。

**注意**：这是 C99 **复合字面量**语法 `(类型){ 成员1, 成员2 }`，大括号里的逗号是结构体成员分隔符，不是函数参数分隔符。实际只有 3 个参数。

---

### 1.4 进程切换完整流水线

#### 1.4.1 统一处理流水线架构

```
                    ┌──────────────────────────────────────────┐
                    │         同样的处理流水线                    │
                    │                                          │
  ① 触发源头        │  ② 硬件CSR    │  ③ trap.S   │  ④ C层分发   │  ⑤ 调度决策    │  ⑥ 恢复+mret
                    │                                          │
  ecall(同步) ──────┤→ mepc/mcause │→ 保存Context│→ EVENT_YIELD │→ schedule()  │→ 另一个进程
  timer(异步) ──────┤→ mepc/mcause │→ 保存Context│→ EVENT_TIMER │→ schedule()  │→ 另一个进程
                    │                                          │
                    └──────────────────────────────────────────┘
```

**核心**：两条路径的区别仅在①和④，中间②③⑤⑥完全共用。上下文切换是"搭了异常处理的顺风车"。

```
异常机制（intr.c）
    │  提供"进入内核"的唯一入口
    ▼
Context 快照（trap.S）
    │  在栈上冻结 CPU 状态，变成一个可传递的 C 结构体
    ▼
Event 翻译（__am_irq_handle）
    │  把硬件 mcause 翻译成操作系统 Event
    ▼
调度决策（schedule）
    │  保存旧 Context，返回新 Context
    │  不关心你是怎么进来的（yield 还是 timer）
    ▼
Context 恢复（trap.S）+ mret → 新进程开始运行
```

#### 1.4.2 trap.S 中的 sp 切换——进程切换的"扳道岔"

```asm
__am_asm_trap:
  addi sp, sp, -CONTEXT_SIZE        # 在旧进程栈上开空间
  MAP(REGS, PUSH)                    # 保存 32 个通用寄存器
  STORE mcause/mstatus/mepc          # 保存 CSR

  mv a0, sp                          # a0 = 旧 Context*（参数）
  call __am_irq_handle               # → 返回 a0 = 新 Context*
  
  mv sp, a0                          # ★ 核心！sp 切到新 Context 所在的栈

  LOAD t1, OFFSET_STATUS(sp)         # 恢复新 Context 的 mstatus
  LOAD t2, OFFSET_EPC(sp)            # 恢复新 Context 的 mepc
  csrw mstatus, t1
  csrw mepc, t2

  MAP(REGS, POP)                     # 恢复新 Context 的 32 个 GPR
                                     # 包括 gpr[2] → sp (新进程的栈顶)
  addi sp, sp, CONTEXT_SIZE
  mret                               # PC ← mepc → 新进程运行！
```

**关键指令 `mv sp, a0` 的机械含义**：

RISC-V 调用约定：函数返回值存在 `a0`。`call __am_irq_handle` 返回后，`a0` 中存的是 `schedule()` 选出的新 Context 指针。

```
call __am_irq_handle 前：a0 = 0x8fff_ef74 (旧 Context_A 地址，sp 也指向这)
call __am_irq_handle 后：a0 = 0x8fff_8000 (新 Context_B 地址)
                         sp = 0x8fff_ef74 (还在旧栈)

mv sp, a0 执行后：      sp = 0x8fff_8000  → 栈切换完成！
```

**常见错误**：`mv a0, sp` 把旧 sp 覆盖了 a0 中的返回值，导致 Context_B 指针丢失，等于什么都没切换。

#### 1.4.3 __am_irq_handle —— 翻译官

```c
Context* __am_irq_handle(Context *c) {
  Event ev = {0};
  
  switch (c->mcause) {
    case 0xb:                          // ecall（同步异常）
      if (c->gpr[17] == (uintptr_t)-1) // a7 = -1 ?
        ev.event = EVENT_YIELD;         // → 主动让出
      else
        ev.event = EVENT_SYSCALL;       // → 系统调用
      c->mepc += 4;                    // 跳过 ecall 指令
      break;

    case 0x80000007:                   // 时钟中断（异步）
      ev.event = EVENT_IRQ_TIMER;
      break;                           // mepc 不加 4！
  }

  c = user_handler(ev, c);  // 核心：返回的 Context* 决定切不切进程
  return c;
}
```

**mepc 修正规则**：

| trap 类型 | mepc 初始值 | 是否 +4 | 原因 |
|-----------|-----------|:--:|------|
| ecall | ecall 指令地址 | ✅ 需要 | 返回后必须跳过已处理的 ecall |
| timer | 下一条未执行指令地址 | ❌ 不需要 | 本来就是正确的恢复点 |

#### 1.4.4 schedule —— 决策者

```c
// 三轮转调度器
static Context *schedule(Event ev, Context *prev) {
  current->cp = prev;                                    // ① 保存旧进程的 Context

  // ② 轮转选择下一个进程
  if (current == &pcb_boot)
    current = &pcb[0];
  else if (current == &pcb[0])
    current = &pcb[1];
  else if (current == &pcb[1])
    current = &pcb[2];
  else
    current = &pcb[0];

  return current->cp;                                    // ③ 返回新进程的 Context
}
```

**这个函数就是进程切换的全部逻辑**：

| 步骤 | 操作 | 含义 |
|------|------|------|
| `current->cp = prev` | A 的 Context 指针存入 A 的 PCB | **冻结** A 的状态 |
| `current = ...` | 轮转选择下一个进程 | **调度决策** |
| `return current->cp` | 返回 B 的 Context 指针 | 把 B 的状态交给 trap.S 去恢复 |

**为什么 main 永远回不来？** 因为 `schedule()` 只在 `pcb[0]`、`pcb[1]`、`pcb[2]` 之间循环，永远不会再选 `pcb_boot`。main 的 Context 冻结在 `pcb_boot.cp` 中，永远不被恢复。

**每次 `current->cp = prev` 会覆盖之前的值吗？** 会对 `pcb[0]`/`pcb[1]`/`pcb[2]` 覆盖——但这正是需要的：要的永远是**最新**的 Context 快照。`pcb_boot.cp` 只在第一次 main→A 切换时被写入一次，之后再无人触碰。

#### 1.4.5 完整走演：进程 A → 进程 B 寄存器级追踪

以进程 A（arg=1，打印 'A'）调用 yield 切换到进程 B（arg=2，打印 'B'）为例：

**步骤 ①：A 执行 ecall**

```
pc = 0x80001004 (ecall 指令地址)
→ NEMU: isa_raise_intr(0xb, 0x80001004)
  mepc    = 0x80001004
  mcause  = 0xb
  mstatus = 关闭 MIE, 保存 MPP
  PC      = mtvec → __am_asm_trap
```

**步骤 ②：trap.S 在 A 的栈上保存 Context_A**

```asm
sp = 0x8fff_f000 - 140 = 0x8fff_ef74
栈上关键值：
  sp[8]   = gpr[2]  = 0x8fff_f000 (A 的 sp)
  sp[40]  = gpr[10] = 0x00000001 (A 的 arg)
  sp[128] = mcause  = 0x0000000b
  sp[136] = mepc    = 0x80001004 (ecall 地址)
```

**步骤 ③：__am_irq_handle 修改 mepc 并回调**

```c
c->mepc += 4;  // mepc = 0x80001008 (跳过 ecall)
c = user_handler(ev, c);  // = schedule(ev, Context_A*)
```

**步骤 ④：schedule() 抉择**

```c
current = &pcb[0]
pcb[0].cp = 0x8fff_ef74   (保存 Context_A*)
current = &pcb[1]          (切到 B)
return 0x8fff_8000          (返回 Context_B*)
```

**步骤 ⑤：trap.S 切换 sp**

```asm
call __am_irq_handle 返回后：
  a0 = 0x8fff_8000 (Context_B*)
  sp = 0x8fff_ef74 (仍在 A 的栈)

mv sp, a0 → sp = 0x8fff_8000  ★ 切到 B 的栈！
```

**步骤 ⑥：从 Context_B 恢复并 mret**

```asm
LOAD mstatus = Context_B.mstatus  → csrw mstatus
LOAD mepc    = Context_B.mepc     → csrw mepc

MAP(REGS, POP):
  gpr[2]  → sp = 0x8fff_7000 (B 的栈)
  gpr[10] → a0 = 0x00000002 (B 的 arg)

addi sp, sp, 140
mret → PC = Context_B.mepc → f(2) 开始运行！
```

**切换前 vs 切换后**：

```
                Context 指针不变（返回自己）       Context 指针变了（返回别人）
                            │                              │
trap.S 保存                  │  A 的寄存器                    │  A 的寄存器
schedule() 返回              │  Context_A*                   │  Context_B*
sp 切换后                    │  仍在 A 的栈                   │  切到 B 的栈
trap.S 恢复                  │  A 的寄存器 (mepc=A的返回地址)  │  B 的寄存器 (mepc=B的返回地址)
mret 后                      │  A 继续执行                    │  B 继续执行
结果                         │  没有切换                       │  进程切换！
```

---

### 1.5 协作式与抢占式调度合并

#### 1.5.1 两种触发路径对比

| | 协作式 `yield()` | 抢占式 时钟中断 |
|------|------|------|
| **触发者** | 进程自己（软件主动） | 硬件定时器（被动打断） |
| **进程能否拒绝** | 是，不调 yield 就独占 | **否，硬件强制打断** |
| **进入路径** | `ecall` → `mcause=0xb` | `timer IRQ` → `mcause=0x80000007` |
| **Event** | `EVENT_YIELD` | `EVENT_IRQ_TIMER` |
| **mepc 修正** | `c->mepc += 4` | 不动 |
| **调度函数** | **同一个 schedule()** | **同一个 schedule()** |

**同一条 trap.S → `__am_irq_handle` → `schedule()` → trap.S 管线对两种方式完全通用。**

#### 1.5.2 设计实现：yield + timer 混合调度

**文件修改清单**：

| 文件 | 改动 | 原因 |
|------|------|------|
| `cte.c` | `MPP_MIE = 0x00001888` | MIE=1，让新进程能响应时钟中断 |
| `cte.c` | `kcontext` 中 `ctx = kstack.end - sizeof(Context)` | 放在栈内部顶端，避免越界 |
| `cte.c` | `kcontext` 加 `return ctx;` | 修复缺返回值的 bug |
| `yield-os.c` | `PCB pcb[3]` | 支持三个进程（两个协作 + 一个抢占） |
| `yield-os.c` | 新增 `g()` 纯计算函数 | 不调 yield，靠 timer 抢占 |
| `yield-os.c` | `schedule()` 扩展三轮转 | `boot→0→1→2→0→…` |
| `yield-os.c` | `schedule()` 中诊断打印 | `ev.event == EVENT_YIELD` 打印 'Y'，`ev.event == EVENT_IRQ_TIMER` 打印 'T' |

**关键注意点**：

1. `schedule()` 中判断事件类型要用 `ev.event`，**不能**用 `ev.cause`：
   - `ev.event` = Event 枚举值（`EVENT_YIELD=1`, `EVENT_IRQ_TIMER=5`）
   - `ev.cause` = 硬件 mcause 原始值（`0xb=11`, `0x80000007`）
   - `ev.cause == EVENT_YIELD` 即 `11 == 1`，永远为假！

2. 纯计算进程 `g()` 不调用 `yield()`，只能被 timer 强制切走。

3. 建议诊断字符区分来源：Y = yield 切换，T = timer 切换，C = 纯计算进程正在执行。

#### 1.5.3 完整执行时序演示

```
时序    触发方式    schedule诊断  当前进程  输出
─────────────────────────────────────────────
main    yield      (无)          → 0(A)    进程A启动
A       timer打断   T            → 1(B)    T
B       打印B并yield Y           → 2(C)    B Y
C       执行中...               C g()打印  C
C       timer打断   T            → 0(A)    T
A       打印A并yield Y           → 1(B)    A Y
B       打印B并yield Y           → 2(C)    B Y
C       执行中...    timer打断   → 0(A)    C T
...循环往复...

期望输出片段：ATBYC TAYBYCT AYBYCT ...
```

**解读**：
- `Y` 出现 → yield 协作式切换正常工作
- `T` 出现（在 C 之后）→ 时钟中断抢占式切换正常工作
- `C` 能打印出来 → 纯计算进程没有 yield 也能拿到 CPU 时间
- 同一条 `schedule()` 处理两种事件 → 统一调度框架验证通过

---

### 1.6 对话问题标准回答

#### 1.6.1 为什么异常处理机制是上下文切换的基础设施

**问题**：为什么上下文切换要"搭"异常处理的顺风车？不能独立实现吗？

**标准回答**：

在 RISC-V 特权架构中，从用户态/机器态的普通执行流进入内核处理流程，**唯一的硬件入口就是 trap**（即 `mtvec`）。没有别的路。

因此任何需要切换到内核进行"任务级操作"的场景，都必须通过 trap：

| 操作 | 触发方式 | 进入路径 |
|------|---------|---------|
| 进程主动让出 | `ecall` 指令 | 同步异常 |
| 时间片到期切换 | 时钟中断 | 异步中断 |
| 系统调用 | `ecall` + 参数 | 同步异常 |

异常机制提供了"进入内核的门"，Context 提供了"进入时带的门票（进程状态）"，`schedule()` 在门里面决定"让谁出去"。**三者缺一不可，但有严格的分层关系。**

#### 1.6.2 Context* 返回值的核心意义是什么

**问题**：为什么 `user_handler(ev, c)` 和 `__am_irq_handle` 都要返回 `Context*`？

**标准回答**：

因为 trap 处理的目标不只是"处理事件"，还包括：**决定最终恢复哪个执行上下文**。

```c
c = user_handler(ev, c);  // c 的值可能在函数内被替换！
```

| 返回值 | 含义 |
|--------|------|
| 返回原来的 `c` | 不切换进程，恢复原执行流 |
| 返回另一个 `Context*` | 发生调度，恢复另一个进程 |

这个设计在 `simple_trap`（PA3 测试）中看不出威力——它永远返回原 `ctx`。但在 `schedule()` 中，**Context 指针的替换 = 进程的替换**，这是整个上下文切换的精华。

#### 1.6.3 mv sp, a0 为什么能完成栈切换

**问题**：为什么一条 `mv sp, a0` 就能实现进程切换？

**标准回答**：

RISC-V 调用约定规定：函数返回值存在 `a0` 寄存器。`call __am_irq_handle` 最终返回 `schedule()` 选出的新进程 Context 指针，这个指针存在 `a0` 中。

```
call 前：a0 = 旧 Context 地址（参数）
call 后：a0 = 新 Context 地址（返回值）
sp      = 仍在旧栈上

mv sp, a0 → sp 指向新 Context 所在的内存
```

之后所有 `LOAD OFFSET_xxx(sp)` 和 `POP(sp)` 读取的都是新 Context 的字段。新进程的栈、寄存器、mepc 全部由这个新 Context 决定。`mret` 后自然跳转到新进程的 `mepc`。

**进程切换在硬件层面不过是改变了 `sp` 的值**——之后的恢复逻辑完全通用，不关心切没切、切了谁。

#### 1.6.4 kcontext 和 trap.S 保存的 Context 有什么不同

**问题**：两者都是 Context，本质差异是什么？

**标准回答**：

| | trap.S 保存 | kcontext 创建 |
|------|-------------|--------------|
| **时机** | 进程被中断时 | 进程创建时（还没运行过） |
| **数据来源** | CPU 硬件寄存器的真实值 | 手工填写 |
| **mepc** | 被打断的指令地址 | `entry` 函数地址 |
| **目的** | 暂停进程，以后恢复 | 让从未运行过的函数"看起来"像刚从 trap 返回 |
| **生命周期** | 每次中断都更新 | 仅用一次，之后被真实快照覆盖 |

**trap.S 是相机，kcontext 是画家。相机拍的是真实发生过的事，画家画的是一张从没存在过的"假照片"——但 trap.S 的恢复代码分不出真假，它只是机械地恢复寄存器然后 mret。**

#### 1.6.5 调度是硬件还是软件决定的

**问题**：调度逻辑是软件层设定的还是硬件决定的？

**标准回答**：

**硬件提供时基（timer interrupt），软件提供策略（schedule）。**

```
硬件做的事（RISC-V 特权架构规定）：
  ① 时钟设备拉高 MTIP 信号
  ② CPU 在每条指令后检查：mip.MTIP && mie.MTIE && mstatus.MIE
  ③ 条件满足 → 硬件自动写入 mepc/mcause/mstatus，跳 mtvec

硬件到此为止。硬件不知道"进程"这个概念。
它只知道"中断来了，跳 mtvec"。

软件做的事：
  ① trap.S 保存 Context
  ② __am_irq_handle 翻译 Event
  ③ schedule() 决定换谁（优先级？时间片？轮转？）
  ④ trap.S 恢复新 Context，mret
```

当只跑 `yield()` 而没有 timer 时，就是"纯协作式"——调度完全依赖进程自觉。加上 timer 中断，硬件就获得了**强制打断**的权力——这就是"抢占式"。

#### 1.6.6 为什么 main 进程永不会被调度回来

**问题**：main 调用 yield 后去哪了？怎么回来？

**标准回答**：

`schedule()` 的轮转逻辑只在 `pcb[0]`、`pcb[1]`、`pcb[2]` 之间切，永远不选 `pcb_boot`。

```
main → yield → schedule:
  pcb_boot.cp = Context_main   ← main 冻结在这里
  current = &pcb[0]            ← 跳过了 pcb_boot
  返回 pcb[0].cp → 激活 A

之后所有循环：0←→1←→2  永不碰 pcb_boot
```

这是**有意为之**的设计——main 是"开机启动程序"，初始化完成后应该"功成身退"。很多嵌入式 RTOS 的 `main()` 就是在最后启动调度器后永远不回来。

如果想让 main 回来，只需在 `schedule()` 中加入 `if (current == &pcb[2]) current = &pcb_boot;` 即可。**让不让 main 回来，完全是 `schedule()` 中 `current = ?` 的选择问题。**

#### 1.6.7 ev.cause 和 ev.event 的区别

**问题**：在 `schedule()` 中判断事件类型，应该用 `ev.cause` 还是 `ev.event`？

**标准回答**：

**必须用 `ev.event`**。

| 字段 | 存什么 | 例：yield | 例：timer |
|------|--------|-----------|-----------|
| `ev.cause` | 硬件 mcause 原始值 | `0xb` (11) | `0x80000007` |
| `ev.event` | Event 枚举值 | `EVENT_YIELD` (1) | `EVENT_IRQ_TIMER` (5) |

```c
// 在 __am_irq_handle 中：
Event ev = {0};
ev.cause = c->mcause;    // 存的是硬件 mcause 原始值

// 在 switch 中：
case 0xb:
  ev.event = EVENT_YIELD;  // 存的是 Event 枚举
  break;
```

**常见错误**：

```c
if (ev.cause == EVENT_YIELD)    // 0xb == 1 → 永远为假！❌
if (ev.event == EVENT_YIELD)    // 1 == 1 → 正确 ✅
```

---

### 1.7 最终总结

PA4 上下文切换模块最核心的认知可以压缩成下面几句话：

1. **异常处理（intr.c + trap.S）是上下文切换的基础设施**，提供了"进入内核"的唯一硬件入口和"保存/恢复 Context"的通用机制。

2. **`Context` 是一次 trap 现场的软件快照**，既保存了被中断进程的完整状态，也让调度器能够决定恢复哪个进程。Context 指针的替换 = 进程的替换。

3. **`kcontext()` 是进程的"出生证明"**，它手工捏造一份 Context，让从未运行过的进程能够第一次通过 trap.S 的恢复安检。第一次是假的，之后就全是硬件产生的真实快照了。

4. **进程切换在硬件层面就是一条 `mv sp, a0`**——sp 从指向旧进程栈变为指向新进程栈。之后 trap.S 的所有恢复操作自动落在新 Context 上。

5. **协作式（yield）和抢占式（timer）只是触发方式不同**，进入 `__am_irq_handle` → `schedule()` → trap.S 恢复的管线完全通用。硬件提供时基，软件提供策略。

6. **`schedule()` 就是调度器的全部**："保存旧 Context → 旋转 current → 返回新 Context"。调度策略（轮转、优先级、时间片）完全由软件设定。

因此，整个 PA4 模块的本质就是：

> **把 PA3 的异常处理管线（trap.S + __am_irq_handle + Event）扩展为进程调度管线**——通过在 `user_handler` 这一层返回不同的 `Context*`，使得同一个 trap 返回机制既能"返回自己"（不切换），也能"返回他人"（进程切换）。调度策略由软件自定义，但切换的"交通工具"永远是异常/中断机制。

---

## 第 2 章 PA4.2 在 NEMU 中启动 RT-Thread

### 2.1 最终结论

当前 PA4.2 相关主设计已经完成：RT-Thread 可以作为 `am-kernels/rt-thread-am/bsp/abstract-machine` 下的 AM 程序，在 `riscv32-nemu` 上启动，并进入 RT-Thread shell。

最终验证命令：

```bash
cd am-kernels/rt-thread-am/bsp/abstract-machine
make ARCH=riscv32-nemu image
timeout 45s make ARCH=riscv32-nemu run
```

关键启动输出：

```text
heap: [0x800ea000 - 0x88000000]

 \ | /
- RT -     Thread Operating System
 / | \     5.0.1 build May 29 2026 20:13:30
 2006 - 2022 Copyright by RT-Thread team
[I/utest] utest is initialize success.
[I/utest] total utest testcase num: (0)
Hello RISC-V!
msh />
```

shell 自动输入测试也可以执行到：

```text
msh />help
msh />date
msh />version
msh />free
msh />ps
msh />pwd
msh />memtrace
msh />utest_list
```

其中 `ps` 能看到：

```text
thread                   pri  status
tshell                    20  running
sys workq                 23  ready
tidle0                    31  ready
timer                      4  suspend
main                      10  close
```

这说明不是只打印了 banner，而是已经完成了：

1. RT-Thread 内核初始化。
2. 线程栈构造。
3. 第一次调度切换。
4. timer interrupt 触发 tick。
5. shell 线程运行。
6. 线程列表、堆信息等内核对象正常工作。

`timeout` 最后的退出码 `124` 是预期行为，因为 RTOS 正常情况下不会主动退出。

### 2.2 当前工程已有基础

PA4.2 不是从零开始写一个 RTOS，而是把三个已经存在的层次接起来：

| 层次 | 目录/文件 | 已有能力 |
|------|-----------|----------|
| NEMU 硬件模拟 | `nemu/src/isa/riscv32/*`、`nemu/src/device/*` | CSR、异常、中断、timer、serial、MMIO |
| AbstractMachine | `abstract-machine/am/src/riscv/nemu/*` | `cte_init()`、`yield()`、`trap.S`、`Context` |
| RT-Thread AM BSP | `am-kernels/rt-thread-am/bsp/abstract-machine/*` | RT-Thread BSP 框架、串口、入口、配置 |

启动 RT-Thread 的核心不在于重新发明这些层，而是完成它们之间的契约：

```text
NEMU timer interrupt
        ↓
AM trap.S 保存 Context
        ↓
AM __am_irq_handle 翻译 Event
        ↓
RT-Thread BSP ev_handler 处理 tick / context switch
        ↓
RT-Thread scheduler 选择线程
        ↓
AM trap.S 恢复新的 Context
        ↓
mret 进入 RT-Thread 线程
```

这里的关键是：RT-Thread 不直接知道 NEMU 的 `mepc/mcause/mstatus`，NEMU 也不直接知道 RT-Thread 的 `struct rt_thread`。中间的共同语言就是 AM 的 `Context *`。

### 2.3 RT-Thread 需要 BSP 提供什么

RT-Thread 的硬件抽象接口定义在：

```text
am-kernels/rt-thread-am/include/rthw.h
```

对 PA4.2 最关键的是以下接口：

```c
rt_uint8_t *rt_hw_stack_init(void *entry, void *parameter,
                             rt_uint8_t *stack_addr, void *exit);

rt_base_t rt_hw_interrupt_disable(void);
void rt_hw_interrupt_enable(rt_base_t level);

void rt_hw_context_switch_to(rt_ubase_t to);
void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to);
void rt_hw_context_switch_interrupt(rt_ubase_t from, rt_ubase_t to,
                                    rt_thread_t from_thread,
                                    rt_thread_t to_thread);
```

它们分别对应四件事：

| 接口 | RT-Thread 期望 |
|------|----------------|
| `rt_hw_stack_init()` | 为一个从未运行过的线程构造初始栈和初始 CPU 上下文 |
| `rt_hw_interrupt_disable()` | 关闭全局中断，并返回旧中断状态 |
| `rt_hw_interrupt_enable(level)` | 按旧状态恢复中断，而不是无条件开中断 |
| `rt_hw_context_switch_to()` | 启动调度器后切到第一个线程 |
| `rt_hw_context_switch()` | 在线程态从当前线程切到目标线程 |
| `rt_hw_context_switch_interrupt()` | 在中断处理中记录延迟切换请求，等中断返回时切换 |

RT-Thread 的调度器会把 `from` 和 `to` 传成线程对象里 `sp` 字段的地址：

```c
rt_hw_context_switch((rt_ubase_t)&from_thread->sp,
                     (rt_ubase_t)&to_thread->sp);

rt_hw_context_switch_interrupt((rt_ubase_t)&from_thread->sp,
                               (rt_ubase_t)&to_thread->sp,
                               from_thread, to_thread);
```

因此 BSP 侧要理解：

```text
from = &from_thread->sp  // 保存当前 Context* 的位置
to   = &to_thread->sp    // 读取目标 Context* 的位置
```

### 2.4 第一轮运行现象与根因定位

#### 2.4.1 构建可以通过

最开始先验证 RT-Thread 镜像是否能编译：

```bash
make ARCH=riscv32-nemu image
```

构建成功生成：

```text
build/rtthread-riscv32-nemu.elf
build/rtthread-riscv32-nemu.bin
```

说明 C 代码、链接脚本、RT-Thread 配置整体可以静态通过。后续问题集中在运行时。

#### 2.4.2 make run 停在 NEMU monitor

第一次执行：

```bash
make ARCH=riscv32-nemu run
```

NEMU 启动后停在：

```text
Welcome to riscv32-NEMU!
For help, type "help"
(nemu)
```

这不是 RT-Thread 崩溃，而是 NEMU 没有批处理运行。需要 `-b`，否则要手动输入 `c` 才会开始执行 guest。

#### 2.4.3 difftest 与异步 timer interrupt 冲突

打开 batch 后，如果 NEMU 配置里 `CONFIG_DIFFTEST=y`，会出现类似：

```text
pc is different after executing instruction at pc = 0x80056678,
right = 0x800147b0, wrong = 0x8005667c
```

定位：

```text
0x80056678 = __am_asm_trap
0x8005667c = __am_asm_trap + 4
0x800147b0 = rt_interrupt_get_nest 内部返回路径
```

这个差异发生在 timer interrupt 进入 `mtvec` 的瞬间。NEMU 收到了异步时钟中断，PC 跳到 `__am_asm_trap`；参考模型没有同步注入同一个中断，所以继续执行下一条普通指令。两者自然不一致。

这不是 RT-Thread 设计错误，而是异步外设中断下 difftest 需要额外中断同步机制。PA4.2 启动 RT-Thread 时应关闭 difftest，保留设备：

```text
CONFIG_DEVICE=y
CONFIG_HAS_SERIAL=y
CONFIG_HAS_TIMER=y
# CONFIG_DIFFTEST is not set
```

#### 2.4.4 真正的 guest 崩溃：访问 0x64

关闭 difftest 后，早期曾出现：

```text
address (0x00000064) is out of bound at pc = 0x80012780
```

反查地址：

```bash
riscv64-linux-gnu-addr2line \
  -e build/rtthread-riscv32-nemu.elf \
  -f -p 0x80012780
```

得到：

```text
rt_tick_increase at src/clock.c:110
```

反汇编关键位置：

```asm
8001277c: jal ra, rt_thread_self
80012780: lw  a4,100(a0)
```

`0x64 = 100`，说明 `a0 == 0`，也就是：

```c
thread = rt_thread_self();  // 返回 NULL
--thread->remaining_tick;   // 访问 NULL + 100
```

为什么 timer tick 会在 `rt_current_thread` 还没有设置时进来？

根因在 BSP 的中断开关 ABI 实现错误。

原来的实现类似：

```c
void rt_hw_interrupt_enable() {
  iset(1);
}

void rt_hw_interrupt_disable() {
  iset(0);
}
```

但 RT-Thread 期望的是：

```c
level = rt_hw_interrupt_disable();
...
rt_hw_interrupt_enable(level);
```

也就是说 `enable(level)` 必须恢复旧状态。如果旧状态本来是关中断，就不能打开。原实现无条件开中断，导致 RT-Thread 启动期间某些临界区提前打开 MIE，timer interrupt 在调度器尚未启动、`rt_current_thread == NULL` 时进入 `rt_tick_increase()`。

这就是 `0x64` 崩溃的完整因果链：

```text
rtthread_startup()
  rt_hw_interrupt_disable()
    关中断，但没有返回旧状态
  ...
  某个 RT-Thread 内部函数：
    level = rt_hw_interrupt_disable()
    ...
    rt_hw_interrupt_enable(level)
      原 BSP 无条件 iset(1)
        ↓
      启动阶段过早打开 MIE
        ↓
      NEMU timer interrupt 进入 AM trap
        ↓
      ev_handler(EVENT_IRQ_TIMER)
        ↓
      rt_tick_increase()
        ↓
      rt_thread_self() == NULL
        ↓
      访问 NULL + remaining_tick 偏移 0x64
```

### 2.5 关键修改一：补齐 RT-Thread 上下文切换

修改文件：

```text
am-kernels/rt-thread-am/bsp/abstract-machine/src/context.c
```

#### 2.5.1 设计目标

RT-Thread 线程切换需要三类行为：

1. 创建线程时，构造一个初始 `Context`。
2. 普通线程态切换时，用 `yield()` 主动进入 AM trap。
3. timer interrupt 中切换时，不再二次 `yield()`，而是在中断返回前切换到目标 `Context`。

#### 2.5.2 全局切换槽

BSP 增加两个全局变量：

```c
static void  *volatile rt_next_ctx = NULL;
static void **volatile rt_save_ptr = NULL;
```

含义：

| 变量 | 含义 |
|------|------|
| `rt_next_ctx` | 下一个要恢复的 AM `Context *` |
| `rt_save_ptr` | 保存当前 `Context *` 的位置，也就是 `&from_thread->sp` |

普通线程切换：

```text
rt_hw_context_switch(from, to)
  rt_save_ptr = from
  rt_next_ctx = *to
  yield()
    ↓
  AM trap.S 保存当前 Context
    ↓
  ev_handler(EVENT_YIELD)
    *rt_save_ptr = 当前 Context
    return rt_next_ctx
    ↓
  trap.S 恢复目标 Context
```

中断内切换：

```text
timer interrupt
  ↓
ev_handler(EVENT_IRQ_TIMER)
  rt_interrupt_enter()
  rt_tick_increase()
    rt_schedule()
      rt_hw_context_switch_interrupt(from, to, ...)
        rt_save_ptr = from
        rt_next_ctx = *to
  rt_interrupt_leave()
  if (rt_next_ctx != NULL)
    *rt_save_ptr = 当前中断现场 Context
    return rt_next_ctx
```

#### 2.5.3 rt_hw_stack_init()

实现思路与 PA4 的 `kcontext()` 一致：线程从未运行过，所以手工伪造一份 AM `Context`，让 trap.S 的恢复流程把它当成“刚被中断过的现场”。

关键字段：

```c
#define INIT_MSTATUS 0x1888

ctx->mepc    = (uintptr_t)tentry;
ctx->mstatus = INIT_MSTATUS;
ctx->gpr[2]  = (uintptr_t)(stack_addr + sizeof(rt_ubase_t));
ctx->gpr[10] = (uintptr_t)parameter;
ctx->gpr[1]  = (uintptr_t)texit;
```

字段解释：

| 字段 | 值 | 作用 |
|------|----|------|
| `mepc` | 线程入口函数 | `mret` 后从线程入口开始执行 |
| `mstatus` | `0x1888` | M-mode，打开 MIE/MPIE |
| `gpr[2]` | 栈顶 | 恢复线程自己的 `sp` |
| `gpr[10]` | 线程参数 | RISC-V ABI 中 `a0` 是第一个参数 |
| `gpr[1]` | `texit` | 线程函数返回时进入退出路径 |

#### 2.5.4 rt_hw_context_switch_to()

启动第一个线程时没有 `from_thread`，只需要加载目标上下文：

```c
void rt_hw_context_switch_to(rt_ubase_t to) {
  rt_save_ptr = NULL;
  rt_next_ctx = *(void **)to;
  yield();
}
```

调用链：

```text
rt_system_scheduler_start()
  rt_current_thread = to_thread
  rt_hw_context_switch_to(&to_thread->sp)
    yield()
      EVENT_YIELD
        return to_thread->sp
      mret 进入第一个 RT-Thread 线程
```

#### 2.5.5 rt_hw_context_switch()

线程态普通切换：

```c
void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to) {
  rt_save_ptr = (void **)from;
  rt_next_ctx = *(void **)to;
  yield();
}
```

这里必须 `yield()`，因为当前线程正在普通 C 代码里运行，还没有 trap frame。只有进入 AM trap.S 后，才能把当前寄存器保存成 `Context`。

#### 2.5.6 rt_hw_context_switch_interrupt()

中断态切换：

```c
void rt_hw_context_switch_interrupt(rt_ubase_t from, rt_ubase_t to,
                                    rt_thread_t from_thread,
                                    rt_thread_t to_thread) {
  (void)from_thread;
  (void)to_thread;
  rt_save_ptr = (void **)from;
  rt_next_ctx = *(void **)to;
}
```

这里不能再次 `yield()`，因为已经在 timer interrupt 的 trap 里。当前中断现场已经由 trap.S 保存好了，只要在 `ev_handler()` 返回时选择另一个 `Context *` 即可。

### 2.6 关键修改二：修正中断开关 ABI

修改文件：

```text
am-kernels/rt-thread-am/bsp/abstract-machine/src/interrupt.c
```

最终实现：

```c
#include <am.h>
#include <rthw.h>

rt_base_t rt_hw_interrupt_disable(void) {
  rt_base_t level = ienabled();
  iset(false);
  return level;
}

void rt_hw_interrupt_enable(rt_base_t level) {
  iset(level != 0);
}
```

这个修改是 RT-Thread 能稳定启动的关键。

RT-Thread 内核大量使用这种模式：

```c
level = rt_hw_interrupt_disable();
...
rt_hw_interrupt_enable(level);
```

这不是简单的“关中断/开中断”，而是“保存现场/恢复现场”。如果进入临界区之前中断就是关闭的，退出时也必须保持关闭。

正确语义：

| 进入前 MIE | `disable()` 返回 | `enable(level)` 后 MIE |
|------------|------------------|------------------------|
| 0 | 0 | 0 |
| 1 | 1 | 1 |

错误实现会把第一行变成：

| 进入前 MIE | 错误 `enable()` 后 MIE |
|------------|------------------------|
| 0 | 1 |

这会破坏 RT-Thread 启动阶段的临界区，导致 timer interrupt 提前进来。

### 2.7 关键修改三：修正 AM RISC-V trap frame 大小

修改文件：

```text
abstract-machine/am/src/riscv/nemu/trap.S
```

原来：

```asm
#define CONTEXT_SIZE  ((NR_REGS + 3) * XLEN)
```

修改为：

```asm
#define CONTEXT_SIZE  ((NR_REGS + 3 + 1) * XLEN)
```

原因在于 `abstract-machine/am/include/arch/riscv.h` 中的 `Context` 定义是：

```c
struct Context {
  uintptr_t gpr[NR_REGS], mcause, mstatus, mepc;
  void *pdir;
};
```

也就是：

```text
NR_REGS 个 GPR
+ mcause
+ mstatus
+ mepc
+ pdir
```

如果 trap.S 只分配 `NR_REGS + 3` 个 XLEN，那么汇编栈帧大小和 C 结构体大小不一致。对 riscv32 来说：

| 项 | 大小 |
|----|------|
| 原 trap frame | `(32 + 3) * 4 = 140` |
| C `Context` | `(32 + 3 + 1) * 4 = 144` |

这个差异有两个风险：

1. C 代码把 `Context *` 当成 144 字节结构体，汇编只按 140 字节移动 `sp`。
2. `call __am_irq_handle` 时栈不再保持 16 字节对齐，复杂 C 调用路径更容易出隐性问题。

虽然当前 PA4.2 不使用 VME 的 `pdir`，但 `Context` ABI 必须统一。补上 `+1` 后，trap.S 和 C 的结构体布局一致。

### 2.8 关键修改四：让 make run 直接进入 guest

修改文件：

```text
abstract-machine/scripts/platform/nemu.mk
```

原来 `-b` 被注释：

```make
# NEMUFLAGS += -b
```

修改为：

```make
NEMUFLAGS += -b
```

目的：执行

```bash
make ARCH=riscv32-nemu run
```

时直接进入 guest，而不是停在 NEMU monitor 等用户输入 `c`。

这不是 RT-Thread 内核功能的一部分，但它是 PA4.2 验证体验的一部分。否则启动脚本会停在：

```text
(nemu)
```

看起来像“没有启动 RT-Thread”，实际只是没有继续执行。

### 2.9 完整启动链路复盘

#### 2.9.1 构建阶段

在 RT-Thread BSP 目录执行：

```bash
make ARCH=riscv32-nemu image
```

核心构建链路：

```text
rt-thread-am/bsp/abstract-machine/Makefile
  ↓
include $(AM_HOME)/Makefile
  ↓
abstract-machine/scripts/riscv32-nemu.mk
  ↓
编译 AM:
  riscv/nemu/start.S
  riscv/nemu/cte.c
  riscv/nemu/trap.S
  platform/nemu/ioe/*
  ↓
编译 RT-Thread:
  src/*.c
  components/*
  bsp/abstract-machine/src/*
  ↓
链接:
  rtthread-riscv32-nemu.elf
  rtthread-riscv32-nemu.bin
```

#### 2.9.2 NEMU 启动阶段

```text
NEMU init_monitor()
  parse_args()
  init_mem()
  init_device()
    init_serial()
    init_timer()
  init_isa()
  load_img()
  init_sdb()
  welcome()
```

设备初始化中，和 RT-Thread 最相关的是：

| 设备 | NEMU 地址 | 用途 |
|------|-----------|------|
| serial | `0xa00003f8` | `putch()` 输出 |
| rtc/timer | `0xa0000048` | AM timer uptime |
| alarm timer | host alarm | 周期性 `dev_raise_intr()` |

NEMU timer interrupt 的硬件模拟链路：

```text
host alarm
  ↓
timer_intr()
  ↓
dev_raise_intr()
  ↓
cpu.csr[mip] |= MTIP
  ↓
cpu_exec 每条指令后 isa_query_intr()
  ↓
mstatus.MIE && mie.MTIE && mip.MTIP
  ↓
isa_raise_intr(0x80000007, epc)
  ↓
pc = mtvec
```

#### 2.9.3 AM 初始化阶段

RT-Thread BSP 的 `main()`：

```c
int main() {
  ioe_init();
  __am_cte_init();
  entry();
  return 0;
}
```

其中：

```text
ioe_init()
  初始化 AM 设备抽象表

__am_cte_init()
  cte_init(ev_handler)
    csrw mtvec, __am_asm_trap
    csrs mie, MTIE
    user_handler = ev_handler

entry()
  rtthread_startup()
```

注意：`cte_init()` 只打开 `mie.MTIE`，真正能不能接收中断还取决于 `mstatus.MIE`。RT-Thread 启动期间会通过 `rt_hw_interrupt_disable/enable` 控制 MIE。

#### 2.9.4 RT-Thread 启动阶段

RT-Thread 启动入口：

```text
entry()
  ↓
rtthread_startup()
  rt_hw_interrupt_disable()
  rt_hw_board_init()
    rt_hw_uart_init()
    rt_system_heap_init()
    rt_console_set_device("uart")
    rt_components_board_init()
  rt_show_version()
  rt_system_timer_init()
  rt_system_scheduler_init()
  rt_application_init()
    创建 main 线程
  rt_system_timer_thread_init()
    创建 timer 线程
  rt_thread_idle_init()
    创建 idle 线程
  rt_system_scheduler_start()
```

第一次真正进入线程发生在：

```text
rt_system_scheduler_start()
  to_thread = highest ready thread
  rt_current_thread = to_thread
  rt_hw_context_switch_to(&to_thread->sp)
    yield()
      ecall
        ↓
      __am_asm_trap
        ↓
      __am_irq_handle(EVENT_YIELD)
        ↓
      ev_handler()
        return to_thread->sp
        ↓
      trap.S 恢复 main 线程 Context
        ↓
      mret 到 main_thread_entry()
```

#### 2.9.5 timer tick 与抢占

当 NEMU timer interrupt 到来：

```text
CPU 跳到 mtvec = __am_asm_trap
  ↓
trap.S 保存当前线程 Context
  ↓
__am_irq_handle()
  mcause = 0x80000007
  ev.event = EVENT_IRQ_TIMER
  ↓
ev_handler(EVENT_IRQ_TIMER)
  rt_interrupt_enter()
  rt_tick_increase()
    当前线程 remaining_tick--
    如果时间片耗尽：
      rt_schedule()
        rt_hw_context_switch_interrupt(&from->sp, &to->sp, ...)
          rt_save_ptr = &from->sp
          rt_next_ctx = to->sp
  rt_interrupt_leave()
  if (rt_next_ctx)
    *rt_save_ptr = 当前 trap Context
    return rt_next_ctx
  ↓
trap.S 恢复目标线程 Context
  ↓
mret 到目标线程
```

这样 RT-Thread 的抢占式调度就接到了 AM/NEMU 的 timer interrupt 上。

### 2.10 验证方法与结果

#### 2.10.1 构建验证

```bash
make ARCH=riscv32-nemu image
```

结果：

```text
+ LD -> build/rtthread-riscv32-nemu.elf
+ OBJCOPY -> build/rtthread-riscv32-nemu.bin
```

#### 2.10.2 直接运行 NEMU 验证

```bash
timeout 5s ./nemu/build/riscv32-nemu-interpreter \
  -b \
  -l /tmp/rtthread-nodiff-log.txt \
  -e am-kernels/rt-thread-am/bsp/abstract-machine/build/rtthread-riscv32-nemu.elf \
  -i am-kernels/rt-thread-am/bsp/abstract-machine/build/rtthread-riscv32-nemu.bin
```

结果：

```text
RT-Thread Operating System
Hello RISC-V!
msh />help
msh />date
msh />version
msh />free
msh />ps
...
```

5 秒内执行约 1.49 亿条 guest 指令，没有 assert/abort。最终由 `timeout` 结束。

#### 2.10.3 make run 端到端验证

```bash
cd am-kernels/rt-thread-am/bsp/abstract-machine
timeout 45s make ARCH=riscv32-nemu run
```

结果：

```text
/home/.../nemu/build/riscv32-nemu-interpreter ... -b
RT-Thread Operating System
Hello RISC-V!
msh />help
...
msh />ps
...
msh />utest_list
```

最终 `timeout` 终止，属于预期。

### 2.11 设计注意事项与剩余非阻塞项

#### 2.11.1 difftest 需要关闭

RT-Thread 启动依赖异步 timer interrupt。当前 NEMU difftest 没有同步注入同一个外部中断到参考模型，因此打开 difftest 会在 timer interrupt 处出现 PC 不一致。

运行 RT-Thread 推荐配置：

```text
CONFIG_DEVICE=y
CONFIG_HAS_SERIAL=y
CONFIG_HAS_TIMER=y
# CONFIG_DIFFTEST is not set
```

#### 2.11.2 RTC warning 不影响启动

运行 `date` 时可能看到：

```text
[W/time] Cannot find a RTC device!
```

这是 RT-Thread 设备框架里没有注册名为 RTC 的 RT-Thread device。它不影响 NEMU timer interrupt，也不影响 RTOS tick 和线程调度。

换句话说：

| 功能 | 是否已完成 |
|------|------------|
| NEMU timer interrupt 驱动 RT-Thread tick | 已完成 |
| RT-Thread shell `date` 找到 RTC 设备 | 非 PA4.2 必需，当前未完成 |

#### 2.11.3 git_commit 的只读文件系统报错可忽略

运行 `make run` 时可能看到：

```text
fatal: cannot create .git/index.lock: Read-only file system
```

这是课程 Makefile 中自动 `git_commit` 辅助逻辑在当前环境里无法写 `.git`。日志中已经被 Makefile 忽略，不影响 NEMU 构建和 RT-Thread 运行。

#### 2.11.4 工作区已有文件不要混淆

当前工作区可能还有与本次 PA4.2 主链路无关的改动，例如：

```text
nemu/include/trace.h
am-kernels/rt-thread-am/bsp/abstract-machine/src/libc.c
```

本次 PA4.2 的核心设计改动集中在：

```text
am-kernels/rt-thread-am/bsp/abstract-machine/src/context.c
am-kernels/rt-thread-am/bsp/abstract-machine/src/interrupt.c
abstract-machine/am/src/riscv/nemu/trap.S
abstract-machine/scripts/platform/nemu.mk
```

#### 2.11.5 最终设计一句话总结

PA4.2 的本质是：

> 用 AM 的 `Context *` 把 NEMU 的硬件异常/中断机制和 RT-Thread 的线程调度器接起来。NEMU 只负责产生 trap，AM 只负责保存/恢复 CPU 现场，RT-Thread 只负责决定下一个线程是谁。三层职责清楚后，RT-Thread 就可以像普通 AM 程序一样在 NEMU 上启动并接受 timer 抢占。
