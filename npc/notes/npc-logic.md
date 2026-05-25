# NPC 基础设施环境设计

> 以 `make ARCH=riscv32I-npc run` 为入口，整套 NPC 环境搭建的完整记录。
> 按模块分部分组织，当前已完成 **第一部分：编译脚本链**。

---

## 目录

- [第一部分：编译脚本链](#第一部分编译脚本链)
  - [1.1 总览：三层 Makefile 链路](#11-总览三层-makefile-链路)
  - [1.2 GCC march / mabi 参数辨析](#12-gcc-march--mabi-参数辨析)
    - [1.2.1 RV32E vs RV32I vs RV32IM](#121-rv32e-vs-rv32i-vs-rv32im)
    - [1.2.2 libgcc 的作用](#122-libgcc-的作用)
    - [1.2.3 ABI 官方参考](#123-abi-官方参考)
  - [1.3 第一层：cpu-tests/Makefile（测试调度层）](#13-第一层cpu-testsmakefile测试调度层)
    - [1.3.1 文件定位](#131-文件定位)
    - [1.3.2 核心逻辑](#132-核心逻辑)
    - [1.3.3 数据流](#133-数据流)
  - [1.4 第二层：abstract-machine/Makefile（编译核心层）](#14-第二层abstract-machinemakefile编译核心层)
    - [1.4.1 文件定位](#141-文件定位)
    - [1.4.2 七个构建步骤](#142-七个构建步骤)
    - [1.4.3 工具链推导](#143-工具链推导)
  - [1.5 第三层：platform/npc.mk（平台定制层）](#15-第三层platformnpcmk平台定制层)
    - [1.5.1 文件定位](#151-文件定位)
    - [1.5.2 当前状态与 TODO](#152-当前状态与-todo)
    - [1.5.3 未来需要填的坑](#153-未来需要填的坑)
  - [1.6 riscv32I-npc.mk 参数说明](#16-riscv32i-npcmk-参数说明)
  - [1.7 完整链路图](#17-完整链路图)
  - [1.8 与 NPC RTL 的对接点](#18-与-npc-rtl-的对接点)
- [第二部分：Verilator 仿真程序](#第二部分verilator-仿真程序) ✅
- [第三部分：difftest 差分测试对接](#第三部分difftest-差分测试对接)
  - [3.1 架构总览：dlopen 动态库模型](#31-架构总览dlopen-动态库模型)
  - [3.2 为什么用 dlopen 封装 NEMU API](#32-为什么用-dlopen-封装-nemu-api)
  - [3.3 NEMU API 封装对照表](#33-nemu-api-封装对照表)
  - [3.4 ref.c 中 5 个函数的实现逻辑](#34-refc-中-5-个函数的实现逻辑)
  - [3.5 dut.cpp 调用链路](#35-dutcpp-调用链路)
  - [3.6 dlopen 机制的核心理解](#36-dlopen-机制的核心理解)
  - [3.7 与 Spike 做 REF 的对比](#37-与-spike-做-ref-的对比)
- [第四部分：波形与调试](#第四部分波形与调试) 🔜

---

## 第一部分：编译脚本链

> 以 `make ARCH=riscv32I-npc run` 为入口，从 Makefile 调用链逐层拆解。
> 最终目标：理解 AM 如何编译出 `.bin` 镜像，以及 NPC 需要填哪些坑才能跑通。

### 1.1 总览：三层 Makefile 链路

```
make ARCH=riscv32I-npc run
  │
  ├─→ 第 1 层: am-kernels/tests/cpu-tests/Makefile   ← 测试调度
  │     为每个 .c 生成临时 Makefile，委托 AM 编译
  │
  ├─→ 第 2 层: abstract-machine/Makefile             ← 编译核心
  │     .c → .o → .a → .elf → .bin（8 个步骤）
  │
  └─→ 第 3 层: abstract-machine/scripts/platform/npc.mk  ← 平台定制
        run 目标 → 调用 NPC 仿真程序 ← 尚未实现 (TODO)
```

### 1.2 GCC march / mabi 参数辨析

#### 1.2.1 RV32E vs RV32I vs RV32IM

| 属性 | RV32E | RV32I（无M扩展） | RV32IM |
|------|:-----:|:-----:|:-----:|
| 通用寄存器数 | 16 (x0-x15) | 32 (x0-x31) | 32 (x0-x31) |
| GCC `march` | `rv32e_zicsr` | `rv32i_zicsr` | `rv32im_zicsr` |
| GCC `mabi` | **`ilp32e`** | **`ilp32`** | **`ilp32`** |
| 乘除法指令 | 软件模拟 | 软件模拟 | **硬件** (M 扩展) |
| 需要 libgcc | ✅ 是 | ✅ 是 | ❌ 否（ALU 直接算） |
| difftest 寄存器数 | 17 (16 + pc) | 33 (32 + pc) | 33 (32 + pc) |

> **ABI 命名规则**：`ilp32` = **i**nt 32bit, **l**ong 32bit, **p**ointer 32bit（32 个寄存器）
> `ilp32e` = 同上，但仅 16 个通用寄存器（**E**=Embedded）
> **不存在 `ilp32i` 这个 ABI！** `i` 后缀在 RISC-V ABI 中没有定义。

#### 1.2.2 libgcc 的作用

当 CPU 没有乘除法硬件时，编译器不能直接生成 `mul`/`div`/`rem` 指令，需要链接 libgcc 中的软件模拟实现：

```makefile
AM_SRCS += riscv/npc/libgcc/div.S      ← 除法软件模拟
           riscv/npc/libgcc/muldi3.S   ← 64位乘法软件模拟
           riscv/npc/libgcc/multi3.c   ← 多精度乘法
           riscv/npc/libgcc/ashldi3.c  ← 64位移位
           riscv/npc/libgcc/unused.c   ← 占位文件
```

> 当 NPC 实现 M 扩展后，去掉 libgcc 这 5 个文件，改为 `-march=rv32im_zicsr`。

#### 1.2.3 ABI 官方参考

| 资源 | 链接 |
|------|------|
| RISC-V psABI 规范（官方） | [https://github.com/riscv-non-isa/riscv-elf-psabi-doc](https://github.com/riscv-non-isa/riscv-elf-psabi-doc) |
| RISC-V Calling Convention | [https://riscv.org/wp-content/uploads/2015/01/riscv-calling.pdf](https://riscv.org/wp-content/uploads/2015/01/riscv-calling.pdf) |
| 32-bit ABI 命名 | `ilp32`, `ilp32f`, `ilp32d`, `ilp32e` (f=hard-float, d=double-float, e=embedded 16reg) |
| 64-bit ABI 命名 | `lp64`, `lp64f`, `lp64d` |

> `ilp32e` 仅在 `-march=rv32e*` 时有效；`ilp32` 用于 `-march=rv32i*/rv32im*` 等 32 寄存器架构。

---

### 1.3 第一层：cpu-tests/Makefile（测试调度层）

#### 1.3.1 文件定位

```
am-kernels/tests/cpu-tests/Makefile
```

#### 1.3.2 核心逻辑

| 步骤 | 行为 |
|:--:|------|
| ① | 扫描 `tests/` 目录，`ALL = add string hello-str ...` |
| ② | 为每个测试生成临时 `Makefile.<test>`，内容只有 3 行：`NAME=<test>`, `SRCS=tests/<test>.c`, `include ${AM_HOME}/Makefile` |
| ③ | 递归执行 `make -s -f Makefile.<test> ARCH=riscv32I-npc run` |
| ④ | 成功 → 写入 `[   test] PASS`；失败 → `[   test] ***FAIL***` |
| ⑤ | 删除临时 `Makefile.<test>`，汇总 `.result` 并打印 |

#### 1.3.3 数据流

```
tests/add.c
    │
    │  第 1 层 Makefile: 生成 Makefile.add
    ▼
Makefile.add (临时):
    NAME = add
    SRCS = tests/add.c
    include ${AM_HOME}/Makefile
    │
    │  make -f Makefile.add ARCH=riscv32I-npc run
    ▼
abstract-machine/Makefile  ← 第 2 层
```

---

### 1.4 第二层：abstract-machine/Makefile（编译核心层）

#### 1.4.1 文件定位

```
abstract-machine/Makefile
```

#### 1.4.2 七个构建步骤

| 步骤 | 行为 | 关键变量/规则 |
|:--:|------|------|
| **Step 1: 校验** | 检查 `AM_HOME`、`ARCH` 合法性 | `ARCHS = $(basename ...scripts/*.mk)` |
| **Step 2: 拆解 ARCH** | `ISA = riscv32I`, `PLATFORM = npc` | `ARCH_SPLIT = $(subst -, ,$(ARCH))` |
| **Step 3: 设置工具链** | `CC = riscv64-linux-gnu-gcc` | 由 `isa/riscv.mk` 设定 `CROSS_COMPILE` |
| **Step 4: 加载平台脚本** | 拼路径加载：`include scripts/riscv32I-npc.mk` | 进而 `include isa/riscv.mk` + `platform/npc.mk` |
| **Step 5: 编译源码** | `tests/add.c → add.o`<br>`riscv/npc/start.S → start.o` | `$(CC) -std=gnu11 $(CFLAGS) -c` |
| **Step 6: 递归构建库** | `make -C am archive` → `am-riscv32I-npc.a`<br>`make -C klib archive` → `klib-riscv32I-npc.a` | `LIB_TEMPLATE` 宏 |
| **Step 7: 链接** | `ld → add-riscv32I-npc.elf`<br>入口 `_start`, 基址 `0x80000000` | `LDFLAGS += -T linker.ld` |
| **Step 8: 生成 .bin** | `objcopy -O binary .elf → .bin` | `platform/npc.mk` 的 `image` 规则 |

#### 1.4.3 工具链推导

```
include scripts/riscv32I-npc.mk
  │
  ├─→ include scripts/isa/riscv.mk
  │       CROSS_COMPILE := riscv64-linux-gnu-
  │
  └─→ include scripts/platform/npc.mk
          AM_SRCS += riscv/npc/start.S ...  (9 个平台文件)
          LDSCRIPTS += linker.ld
          LDFLAGS += --defsym=_pmem_start=0x80000000
```

**最终编译器参数**：
```
CC  = riscv64-linux-gnu-gcc
CFLAGS  += -march=rv32i_zicsr -mabi=ilp32
LDFLAGS += -melf32lriscv -T linker.ld --defsym=_pmem_start=0x80000000
```

---

### 1.5 第三层：platform/npc.mk（平台定制层）

#### 1.5.1 文件定位

```
abstract-machine/scripts/platform/npc.mk
```

#### 1.5.2 当前状态与 TODO

| 规则 | 当前内容 | 状态 |
|------|---------|:--:|
| `image` | `objcopy -O binary .elf → .bin` | ✅ 已完成 |
| `run` | `echo "TODO: add command here to run simulation"` | ❌ 待实现 |

#### 1.5.3 未来需要填的坑：与 nemu.mk 的 run 行对比

> NPC 的 `run` 目标和 NEMU 的 `run` 目标是同一性质的命令——**用各自的"执行引擎"去跑编译好的 `.bin` 程序**。

**nemu.mk 的 run（已跑通）**：

```makefile
run: insert-arg
    $(MAKE) -C $(NEMU_HOME) ISA=$(ISA) run ARGS="$(NEMUFLAGS)"
```

展开后的实际命令：

```bash
make -C /path/nemu ISA=riscv32 run \
  ARGS="-b -l /path/nemu-log.txt -e NAME.elf -i NAME.bin"
```

这行做的事：**进入 NEMU 目录，执行 NEMU 自己的 `make run`**，NEMU 的 `run` 规则（在 `nemu/scripts/native.mk`）会直接启动 NEMU 可执行文件，把 `.elf`/`.bin` 作为客户程序镜像加载进去跑。**NEMU 编译好的软件模拟器早已存在，`make run` 只是启动它。**

---

**npc.mk 的 run（待实现）**：

```makefile
# 目标写法
run: insert-arg
    @$(NPC_HOME)/build/sim --img=$(IMAGE).bin
```

展开后的实际命令（将来）：

```bash
/path/npc/build/sim --img=NAME-riscv32I-npc.bin
```

这行要做的事：**启动 NPC 的仿真可执行文件 `sim`**，把 `.bin` 作为参数传进去。`sim` 是 **Verilator 把你写的 RTL 编译成的 C++ 仿真程序**。

---

**`sim` 内部需要做的事**：

| 步骤 | 行为 | 对应 NEMU 的什么 |
|:--:|------|------|
| ① | 解析命令行 `--img=<.bin>` | NEMU 的 `-i` 参数 |
| ② | 打开 `.bin` 文件，加载到 CPU 的 `mem[]` | NEMU 的 `init_mem()` |
| ③ | 驱动 `clk`/`rst`，让 CPU 核开始执行 | NEMU 的 `cpu_exec()` |
| ④ | （可选）每周期 difftest 对拍 | NEMU 的 `difftest_step()` |
| ⑤ | （可选）输出 VCD 波形 | NEMU 的 itrace/mtrace 日志 |

---

**NPC 的 `sim` 目前还不存在**，它需要你先完成：

```
1. npc/csrc/sim_main.cpp    ← C++ 仿真主循环
2. npc/Makefile              ← Verilator 编译规则
3. make → Verilator 编译 RTL → 产出 build/sim
```

这三步完成后，`platform/npc.mk` 的 `run` 第 29 行才能从 `echo "TODO"` 改为 `@$(NPC_HOME)/build/sim --img=$(IMAGE).bin`。

---

**nemu.mk vs npc.mk 一句话对比**：

| | nemu.mk | npc.mk |
|---|---|---|
| `run` 启动什么 | 软件模拟器 NEMU（C 写的，早已编译好） | 硬件仿真程序 `sim`（Verilator 编译 RTL 产出的 C++ 可执行文件） |
| 被启动的程序在哪 | `nemu/build/riscv32-nemu-interpreter` | `npc/build/sim`（**还不存在**） |
| 传什么参数 | `.elf` + `.bin` + 日志 + 批处理 | `.bin` |

---

### 1.6 riscv32I-npc.mk 参数说明

```makefile
# riscv32I-npc.mk
include $(AM_HOME)/scripts/isa/riscv.mk         # RISC-V 工具链
include $(AM_HOME)/scripts/platform/npc.mk       # NPC 平台配置
COMMON_CFLAGS += -march=rv32i_zicsr -mabi=ilp32  # RV32I + 无M扩展 + Zicsr
LDFLAGS       += -melf32lriscv                   # 32位 RISC-V ELF 格式

# 软乘除模拟（M 扩展未实现前需要）
AM_SRCS += riscv/npc/libgcc/div.S \
           riscv/npc/libgcc/muldi3.S \
           riscv/npc/libgcc/multi3.c \
           riscv/npc/libgcc/ashldi3.c \
           riscv/npc/libgcc/unused.c
```

| 参数 | 含义 | 原因 |
|------|------|------|
| `-march=rv32i_zicsr` | RV32I 基础 + CSR 扩展，无 M | 当前 NPC 未实现 M 扩展硬件 |
| `-mabi=ilp32` | 32 寄存器标准 ABI | NPC 有 32 个通用寄存器 (x0-x31) |
| `-melf32lriscv` | 32 位小端 RISC-V ELF | 与链接脚本 linker.ld 匹配 |
| libgcc 源文件 | 软乘除模拟 | 编译器遇到 `*` `/` `%` 时链接这些实现 |

---

### 1.7 完整链路图

```
┌─────────────────────────────────────────────────────────────────────┐
│ make ARCH=riscv32I-npc run                                           │
└─────────────────────────────────────────────────────────────────────┘
                                   │
    ┌──────────────────────────────┼──────────────────────────────┐
    ▼                              ▼                              ▼
ARCH=riscv32I-npc             NAME=add                    SRCS=tests/add.c
    │                              │                              │
    ▼                              ▼                              ▼
┌──────────────────────────────────────────────────────────────────┐
│ 第 1 层: cpu-tests/Makefile                                      │
│                                                                    │
│   ALL = add string ...                                             │
│   Makefile.add: NAME=add SRCS=tests/add.c include AM/Makefile      │
│   make -s -f Makefile.add ARCH=riscv32I-npc run                    │
└──────────────────────────────┬───────────────────────────────────┘
                               ▼
┌──────────────────────────────────────────────────────────────────┐
│ 第 2 层: abstract-machine/Makefile                                │
│                                                                    │
│ ① 校验: ARCH=riscv32I-npc ∈ ARCHS                                  │
│ ② 拆解: ISA=riscv32I , PLATFORM=npc                                │
│ ③ 加载: include scripts/riscv32I-npc.mk                            │
│          ├─→ scripts/isa/riscv.mk  ──→ CROSS_COMPILE=riscv64-... │
│          └─→ scripts/platform/npc.mk ──→ AM_SRCS, LDSCRIPTS       │
│ ④ 编译: add.c ──riscv64-linux-gnu-gcc──→ add.o                      │
│ ⑤ 构建库: make -C am archive → am-riscv32I-npc.a                    │
│           make -C klib archive → klib-riscv32I-npc.a                │
│ ⑥ 链接: add.o + *.o + am.a + klib.a                                │
│          ──riscv64-linux-gnu-ld──→ add-riscv32I-npc.elf              │
│          选项: -T linker.ld --defsym=_pmem_start=0x80000000          │
│ ⑦ 生成 .bin: objcopy -O binary .elf → .bin                          │
└──────────────────────────────┬───────────────────────────────────┘
                               ▼
┌──────────────────────────────────────────────────────────────────┐
│ 第 3 层: platform/npc.mk                                          │
│                                                                    │
│   run: insert-arg                                                   │
│       ~~~ TODO ~~~                                                  │
│       未来: @$(NPC_HOME)/build/sim --img=$(IMAGE).bin              │
└──────────────────────────────────────────────────────────────────┘
                               │
                               │ (将来) 传入 NPC 仿真程序
                               ▼
┌──────────────────────────────────────────────────────────────────┐
│ NPC Verilator 仿真程序 (sim_main.cpp + Vcore_top)                 │
│                                                                    │
│   ① 加载 .bin → mem[]  (起始地址 0x80000000)                       │
│   ② 驱动 clk/rst → CPU 执行指令                                    │
│   ③ 每个周期 difftest 对拍 (与 NEMU 比对)                          │
│   ④ 输出波形 VCD + 断言 + 日志                                      │
└──────────────────────────────────────────────────────────────────┘
```

---

### 1.8 与 NPC RTL 的对接点

| 编译链产出 | NPC 仿真需要 | 对接方式 |
|-----------|-------------|---------|
| `.bin` 文件 | 加载到 CPU 内存 | `$readmemh` 或 `fread` + `memcpy` |
| `_pmem_start=0x80000000` | PC 复位向量 | `parameter RESET_VECTOR = 32'h8000_0000` |
| `-mabi=ilp32` (32 GPR) | difftest 期望 33 个 word | `DIFFTEST_REG_SIZE = sizeof(uint32_t) * 33` |
| `.elf` (含符号) | ftrace 函数追踪 | 解析 ELF 符号表 |

---

## 第二部分：Verilator 仿真程序 ✅

> **状态**：已完成（最小可用版本）。
> **内容**：Verilator 编译 RTL → C++ 可执行文件 `build/sim`，加载 .bin 到 imem，驱动时钟，打印 PC 轨迹。

---

## 第三部分：difftest 差分测试对接

> **状态**：NEMU REF 侧 5 个函数正在实现（`ref.c`），NPC DUT 侧待编写（`dut.cpp`）。

### 3.1 架构总览：dlopen 动态库模型

```
你写的代码:                   产出:                      NPC 使用:
──────────                   ────                      ────────

nemu/src/cpu/difftest/ref.c  ──make SHARE=1──→  nemu/build/...-nemu-interpreter-so
  (你实现 5 个函数)                                    │
                                                       ├── difftest_init
                                                       ├── difftest_memcpy
                                                       ├── difftest_regcpy
                                                       ├── difftest_exec
                                                       └── difftest_raise_intr
                                                              │
npc/csrc/difftest/dut.cpp  ──────────────────────────────────┘
  (dlopen + dlsym + 调用)            dut.cpp 拿到这 5 个函数指针后，
                                     在 cpu_step() 后调用它们完成比对
```

**核心机制**：你在 `ref.c` 中实现的函数 → 编译器打包进 `.so` → `dut.cpp` 通过 `dlopen` 加载 → `dlsym` 取得函数指针 → 像调用普通函数一样调用它们。

### 3.2 为什么用 dlopen 封装 NEMU API

| 问题 | 答案 |
|------|------|
| NPC 想用 NEMU 的哪些功能？ | `cpu_exec`（执行指令）、读写 GPR/PC、读写物理内存 |
| 为什么不直接 `#include` NEMU 头文件？ | NPC 和 NEMU 是**两个独立进程**，不能直接链接 |
| 怎么跨进程调用？ | 把 NEMU 编译成 `.so` 动态库，加载到 NPC 的进程空间中 |
| `.so` 里暴露什么？ | 5 个接口函数——足够驱动 NEMU 引擎、同步状态 |

### 3.3 NEMU API 封装对照表

> 5 个 `__EXPORT` 函数在 `ref.c` 中实现，编译进 `.so` 供 `dut.cpp` 调用。

| 封装函数 | 内部调用的 NEMU API | 作用 | 调用时机 |
|---------|-------------------|------|---------|
| `difftest_init(port)` | `init_mem()` + `init_isa()` | 初始化 NEMU 内存 + ISA 状态 | NPC 启动时 |
| `difftest_memcpy(addr, buf, n, TO_REF)` | `guest_to_host(addr)` + `memcpy` | 把 NPC 的 `.bin` 程序拷进 NEMU 内存 | 初始化时 |
| `difftest_regcpy(buf, TO_REF)` | `memcpy(&cpu, buf, 132)` | 把 NPC 的 32 GPR + PC 同步给 NEMU | 初始化时 |
| `difftest_exec(1)` | `cpu_exec(1)` | NEMU 解释器执行 1 条指令 | 每条指令后 |
| `difftest_regcpy(buf, TO_DUT)` | `memcpy(buf, &cpu, 132)` | 从 NEMU 读回 32 GPR + PC（标准答案） | 每条指令后 |
| `difftest_raise_intr(NO)` | 暂空 | 注入异常（阶段 3 用） | 阶段 3 |

### 3.4 ref.c 中 5 个函数的实现逻辑

#### ① `difftest_init(port)`

```
作用: 启动 NEMU 引擎
内部: ① init_mem() — 分配 128MB 物理内存数组
      ② init_isa() — 初始化 cpu 结构体（32 GPR=0, PC=RESET_VECTOR）
调用方: dut.cpp 在 dlopen 后第一个调用
```

#### ② `difftest_memcpy(addr, buf, n, direction)`

```
作用: 在 NPC 内存和 NEMU 内存之间拷贝数据
参数:
  addr     — 物理地址（如 0x80000000）
  buf      — 数据缓冲区指针
  n        — 字节数
  direction — TO_REF: NPC→NEMU, TO_DUT: NEMU→NPC
内部:
  guest_to_host(addr) — 把 NEMU 物理地址转成 C 指针
  memcpy — 执行拷贝
```

#### ③ `difftest_regcpy(dut, direction)`

```
作用: 在 NPC 和 NEMU 之间同步寄存器状态
数据格式:
  uint32_t[33] — 前 32 个是 GPR(x0-x31)，最后 1 个是 PC
  总大小 = 33 × 4 = 132 字节 = DIFFTEST_REG_SIZE
参数:
  dut       — 指向 132 字节数组的指针
  direction — TO_REF: NPC→NEMU, TO_DUT: NEMU→NPC
内部:
  &cpu — NEMU 的全局 CPU_state 变量，布局与 132 字节数组一致
  memcpy — 直接整块拷贝
```

#### ④ `difftest_exec(n)`

```
作用: 让 NEMU 执行 n 条指令
参数: n = 执行条数（通常为 1）
内部: cpu_exec(n) — NEMU 解释器的主循环
     执行后 NEMU 的 cpu.gpr[] 和 cpu.pc 自动更新
```

#### ⑤ `difftest_raise_intr(NO)`

```
作用: 向 NEMU 注入异常（阶段 3 实现）
当前: 空函数
```

### 3.5 dut.cpp 调用链路

```
┌──────────────────────────────────────────────────────────┐
│                  dut.cpp 主流程                           │
│                                                          │
│  init_difftest(so_path, img_size):                       │
│    ① dlopen("nemu-so")            ← 加载 NEMU 动态库     │
│    ② dlsym("difftest_init")       ← 取出 5 个函数指针    │
│       dlsym("difftest_memcpy")                           │
│       dlsym("difftest_regcpy")                           │
│       dlsym("difftest_exec")                             │
│       dlsym("difftest_raise_intr")                       │
│    ③ ref_init(1234)               ← 启动 NEMU 引擎        │
│    ④ ref_memcpy(...)              ← 同步 .bin 程序        │
│    ⑤ ref_regcpy(npc_regs, TO_REF) ← 同步寄存器初始状态    │
│                                                          │
│  主循环 (每条指令后):                                     │
│    cpu_step()                      ← NPC 硬件执行 1 条    │
│    ref_exec(1)                     ← NEMU 执行 1 条       │
│    ref_regcpy(ref_regs, TO_DUT)    ← 读回 NEMU 结果       │
│    逐寄存器比对: npc.rf[i] vs ref_regs[i]                │
│    一致 → 继续; 不一致 → 报错停止                         │
└──────────────────────────────────────────────────────────┘
```

### 3.6 dlopen 机制的核心理解

| 概念 | 说明 |
|------|------|
| `.so` 本质 | 编译好的 NEMU 解释器，不含 `main()`，只导出 5 个函数 |
| `dlopen` | 把 `.so` 加载到当前进程的内存空间——相当于"把 NEMU 引擎装进 NPC" |
| `dlsym` | 在加载的 `.so` 中按函数名查找地址，返回函数指针 |
| `__EXPORT` | 告诉编译器"这个函数要暴露给 dlopen 的人用" |
| 函数指针调用 | 拿到函数指针后，调用方式和普通函数完全一样 |

> **一句话**：`dlopen` 是把 NEMU 的代码"借"过来用——你不需要改 NPC 的 RTL，不需要改 NEMU 的 main，只需要通过 5 个函数接口驱动 NEMU 引擎，和你的 NPC 硬件同步执行并逐指令比对。

### 3.7 与 Spike 做 REF 的对比

| | NEMU 做 REF | Spike 做 REF |
|---|---|---|
| 5 个函数 | 需要自己在 `ref.c` 中实现 | 已在 `spike-diff/difftest.cc` 中实现 |
| 编译方式 | `nemu/` 下 `make SHARE=1` | `cd tools/spike-diff && make` |
| NEMU 代码改动 | 改动 `ref.c` 4 个函数 | 不动 NEMU 代码 |
| NPC 侧代码 | 完全一样（都是 `dlopen` + 5 个函数指针） | 完全一样 |
| 推荐度 | 教学理解用 | 快速验证用 |

## 第四部分：波形与调试 🔜

> **状态**：待第三部分完成。
> **内容**：VCD 波形输出，GTKWave 查看，断言框架。

