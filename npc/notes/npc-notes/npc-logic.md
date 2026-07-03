# NPC 基础设施环境设计

> 以 `make ARCH=riscv32I-npc run` 为入口，整套 NPC 环境搭建的完整记录。
> 按模块分部分组织。

---

## 目录

- [第一部分：编译脚本链](#第一部分编译脚本链)
  - [1.1 总览：三层 Makefile 链路](#11-总览三层-makefile-链路)
  - [1.2 GCC march / mabi 参数辨析](#12-gcc-march--mabi-参数辨析)
  - [1.3 第一层：cpu-tests/Makefile（测试调度层）](#13-第一层cpu-testsmakefile测试调度层)
  - [1.4 第二层：abstract-machine/Makefile（编译核心层）](#14-第二层abstract-machinemakefile编译核心层)
  - [1.5 第三层：platform/npc.mk（平台定制层）](#15-第三层platformnpcmk平台定制层)
  - [1.6 riscv32I-npc.mk 参数说明](#16-riscv32i-npcmk-参数说明)
  - [1.7 完整链路图](#17-完整链路图)
  - [1.8 与 NPC RTL 的对接点](#18-与-npc-rtl-的对接点)
- [第二部分：Verilator 仿真程序](#第二部分verilator-仿真程序) ✅
- [第三部分：内存架构重构 — 哈佛→冯诺依曼 (DPI-C 统一内存)](#第三部分内存架构重构--哈佛冯诺依曼-dpi-c-统一内存)
  - [3.1 架构对比](#31-架构对比)
  - [3.2 核心设计决策](#32-核心设计决策)
  - [3.3 逐文件代码对比](#33-逐文件代码对比)
  - [3.4 迁移 Bug 修复记录](#34-迁移-bug-修复记录)
  - [3.5 改造效果对比](#35-改造效果对比)
- [第四部分：difftest 差分测试对接](#第四部分difftest-差分测试对接)
  - [4.1 架构总览：dlopen 动态库模型](#41-架构总览dlopen-动态库模型)
  - [4.2 为什么用 dlopen 封装 NEMU API](#42-为什么用-dlopen-封装-nemu-api)
  - [4.3 NEMU API 封装对照表](#43-nemu-api-封装对照表)
  - [4.4 ref.c 中 5 个函数的实现逻辑](#44-refc-中-5-个函数的实现逻辑)
  - [4.5 dut.cpp 调用链路](#45-dutcpp-调用链路)
  - [4.6 dlopen 机制的核心理解](#46-dlopen-机制的核心理解)
  - [4.7 Difftest 事件分类与同步策略](#47-difftest-事件分类与同步策略)
    - [4.7.1 difftest 的基本假设](#471-difftest-的基本假设)
    - [4.7.2 四类事件处理总表](#472-四类事件处理总表)
    - [4.7.3 同步异常：正常 compare](#473-同步异常正常-compare)
    - [4.7.4 异步中断：skip compare + sync](#474-异步中断skip-compare--sync)
    - [4.7.5 MMIO 外设：skip compare + sync](#475-mmio-外设skip-compare--sync)
    - [4.7.6 single_cycle 决策图](#476-single_cycle-决策图)
    - [4.7.7 源码索引](#477-源码索引)
  - [4.8 M 扩展乘除指令：显式语义与调试记录](#48-m-扩展乘除指令显式语义与调试记录)
- [第五部分：波形与调试](#第五部分波形与调试) 🔜
- [第六部分：中断响应的核心设计逻辑](#第六部分中断响应的核心设计逻辑)
  - [6.1 问题起点：中断在哪个时刻被检测？](#61-问题起点中断在哪个时刻被检测)
  - [6.2 核心设计决策：kill 当前指令，而非下一条指令](#62-核心设计决策kill-当前指令而非下一条指令)
  - [6.3 为什么 kill 下一周期是错的？](#63-为什么-kill-下一周期是错的)
  - [6.4 RISC-V 规范的要求：精确中断](#64-risc-v-规范的要求精确中断)
  - [6.5 逐周期还原：完整的硬件状态迁移](#65-逐周期还原完整的硬件状态迁移)
  - [6.6 优先级链条](#66-优先级链条)
  - [6.7 源码验证清单](#67-源码验证清单)
  - [6.8 核心设计原则总结](#68-核心设计原则总结)

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

# 软乘除模拟（仅在 RTL 还没实现 M 扩展时需要）
AM_SRCS += riscv/npc/libgcc/div.S \
           riscv/npc/libgcc/muldi3.S \
           riscv/npc/libgcc/multi3.c \
           riscv/npc/libgcc/ashldi3.c \
           riscv/npc/libgcc/unused.c
```

| 参数 | 含义 | 原因 |
|------|------|------|
| `-march=rv32i_zicsr` | RV32I 基础 + CSR 扩展，无 M | 适用于尚未启用 M 扩展硬件的构建 |
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

## 第三部分：内存架构重构 — 哈佛→冯诺依曼 (DPI-C 统一内存)

> 日期：2026-06-02  
> 目标：从 Verilog reg 大数组的哈佛架构，迁移到 DPI-C + C++ 数组的冯诺依曼统一内存

### 3.1 架构对比

#### 改造前：哈佛架构 + Verilog 内存 + 外部信号 MMIO

```
┌────────────────────────────────────────────────────────────┐
│                      Verilog RTL                           │
│  ┌──────────┐     ┌──────────────────┐                     │
│  │ if_stage │────→│ imem[33M]        │ 128MB Verilog reg   │
│  └──────────┘     └──────────────────┘                     │
│  ┌──────────┐     ┌──────────────────┐                     │
│  │mem_stage │────→│ dmem[33M]        │ 又一个 128MB reg    │
│  │          │     └──────────────────┘                     │
│  │          │────→│ dev_req/dev_addr ... │ MMIO 信号通道    │
│  └──────────┘     └──────────────────┘                     │
│  问题: ① IMEM/DMEM 重复加载 ② .bss 段困难 ③ MMIO 需额外信号│
│  ④ Verilator 大数组仿真慢 ⑤ 不利于后续接总线               │
└────────────────────────────────────────────────────────────┘
```

#### 改造后：冯诺依曼 + DPI-C 统一内存

```
┌──────────────────────┐        ┌────────────────────────────┐
│     Verilog RTL      │  DPI-C │      C++ Testbench         │
│  ┌──────────┐        │  read  │  npc_pmem[128MB]           │
│  │ if_stage │────────┼───────→│  ┌──────────────────────┐  │
│  └──────────┘        │        │  │ 物理内存 [0x80000000) │  │
│  ┌──────────┐        │  read  │  ├──────────────────────┤  │
│  │mem_stage │────────┼───────→│  │ MMIO 路由             │  │
│  │          │        │  write │  │ RTC / UART / ...      │  │
│  └──────────┘        │═══════→│  └──────────────────────┘  │
│  Verilog 中:          │        │  加载: fread 一条语句       │
│  零个大数组！          │        │  读写: 地址范围内判断       │
│  零个 dev_* 端口！     │        │  调试: printf + gdb        │
└──────────────────────┘        └────────────────────────────┘
```

优势：① 统一内存，指令和数据走同一入口 ② .bin 直接 fread，天然包含所有段
③ MMIO 在同一读写函数内 if/else 分支处理 ④ C++ 原生数组，速度快 ⑤ 后续接总线模型天然适配

### 3.2 核心设计决策

#### DPI-C 通信模型

| 方向 | 函数 | Verilog 调用位置 | C++ 实现 |
|------|------|-----------------|----------|
| RTL→C++ 读 | `dpi_mem_read(addr)` | 组合逻辑 `assign` | `memory.cpp` |
| RTL→C++ 写 | `dpi_mem_write(addr, data, wmask)` | 时序逻辑 `always @(posedge)` | `memory.cpp` |

- **读用 combinational**：`if_stage` 取指和 `mem_stage` 加载必须在同一 `eval()` 内返回数据给 WB 阶段
- **写用 sequential**：写入是 `posedge clk` 触发，副作用操作（修改 C++ 数组、printf 输出字符）时机明确

#### 文件组织

```
npc/
├── vsrc/
│   ├── dpi_imports.vh          ← DPI-C 声明（Verilog 侧）
│   ├── if_stage.v              ← 删除了 imem 数组
│   ├── mem_stage.v             ← 删除了 dmem 数组 + dev_* 端口
│   └── core_top.v              ← 删除了 dev_* 端口
├── include/
│   ├── memory.h                ← [NEW] 内存 API + 常量
│   ├── dpi_export.h            ← DPI-C 声明（C++ 侧）
│   ├── device.h                ← 仅保留 npc_serial_putc
│   └── npc.h                   ← 删除 imem/dmem 访问器
└── csrc/
    ├── memory/
    │   └── memory.cpp           ← [NEW] 统一内存 + MMIO 路由
    ├── main.cpp                 ← 删除 npc_pmem、MMIO 拦截
    ├── monitor/monitor.cpp      ← load_bin 简化
    ├── difftest/dut.cpp         ← img_size → npc_img_size
    └── monitor/sdb/npc_isa.cpp  ← 改用 pmem_read/write
```

### 3.3 逐文件代码对比

#### 3.3.1 `vsrc/dpi_imports.vh` — DPI-C 声明

**改造前**：
```verilog
import "DPI-C" function int dpi_mmio_read(input int addr);
// 只管 MMIO，物理内存还在 Verilog imem/dmem 中
```

**改造后**：
```verilog
import "DPI-C" function int  dpi_mem_read(input int addr);
import "DPI-C" function void dpi_mem_write(input int addr, input int data, input int wmask);
// 两个函数覆盖全部地址空间（物理内存 + MMIO），不需要区分
```

#### 3.3.2 `vsrc/if_stage.v` — 取指：IMEM → DPI-C

**改造前**：
```verilog
parameter IMEM_SIZE = 33554432;     // 128MB / 4B
reg [31:0] imem [0:IMEM_SIZE - 1];  // ← 128MB Verilog reg 数组
wire [31:0] pc_off = pc - RESET_VECTOR;
wire [24:0] imem_idx = pc_off[26:2];
assign instr = imem[imem_idx];      // ← 从 Verilog 数组读
```

**改造后**：
```verilog
`include "dpi_imports.vh"
// 删除 IMEM_SIZE, imem 数组, pc_off, imem_idx
assign instr = dpi_mem_read(pc);    // ← 一行！DPI-C 从 C++ 读
```

#### 3.3.3 `vsrc/mem_stage.v` — 访存：DMEM + dev_* → DPI-C

**改造前（关键结构）**：
```verilog
parameter MMEM_SIZE = 33554432;
reg [31:0] dmem [0:MMEM_SIZE-1];       // ← 128MB Verilog reg
wire in_pmem = (mem_addr >= DATA_ADDR_START) && (mem_addr < DATA_ADDR_END);

// 设备信号（6 个输出端口）
output wire dev_req, dev_r_w;
output wire [31:0] dev_addr, dev_wdata;
output wire [3:0] dev_wmask;

// 读路径：物理内存走 dmem，MMIO 走 DPI-C
assign dev_rdata = dpi_mmio_read(mem_addr);
assign mem_rdata_raw = in_pmem ? dmem[mem_idx] : dev_rdata;

// 写路径：只在物理内存范围内写 DMEM
always @(posedge clk) begin
    if (mem_write && in_pmem) begin
        if (mem_wmask[0]) dmem[mem_idx][7:0]   <= mem_wdata[7:0];
        if (mem_wmask[1]) dmem[mem_idx][15:8]  <= mem_wdata[15:8];
        if (mem_wmask[2]) dmem[mem_idx][23:16] <= mem_wdata[23:16];
        if (mem_wmask[3]) dmem[mem_idx][31:24] <= mem_wdata[31:24];
    end
end
```

> 问题：① dmem 第二个 128MB 大数组 ② 范围判断+译码在 Verilog 中 ③ 6 个设备端口需层层透传
> ④ 物理内存/设备双路径 ⑤ MMIO 写还需 C++ 再拦截一次

**改造后**：
```verilog
`include "dpi_imports.vh"
// 删除 dmem 数组、DATA_ADDR_*、PMEM_BASE、MMEM_SIZE、mem_off、mem_idx、in_pmem
// 删除 6 个 dev_* 输出端口

// 读：组合逻辑内通过 DPI-C 从 C++ 统一内存读取
assign mem_rdata_raw = dpi_mem_read(mem_addr);

// 写：时序逻辑内通过 DPI-C 写入 C++ 统一内存
always @(posedge clk) begin
    if (mem_write) begin
        dpi_mem_write(mem_addr, mem_wdata, mem_wmask);
    end
end
// LSU 子模块完全保留不变
```

#### 3.3.4 `vsrc/core_top.v` — 删除 dev_* 端口

改造前后：删除 `dev_req, dev_r_w, dev_addr, dev_wdata, dev_wmask` 全部 5 个 output 声明和内部 wire，`mem_stage` 实例化也同步删除对应端口连接。

#### 3.3.5 `include/memory.h` — [NEW] 内存模块头文件

```c
#ifndef __NPC_MEMORY_H__
#define __NPC_MEMORY_H__

#define PMEM_BASE 0x80000000U
#define PMEM_SIZE (128U * 1024 * 1024)
#define PMEM_END  (PMEM_BASE + PMEM_SIZE)

#ifdef __cplusplus
extern "C" {
#endif
int  dpi_mem_read(int addr);
void dpi_mem_write(int addr, int data, int wmask);
#ifdef __cplusplus
}
extern uint8_t npc_pmem[PMEM_SIZE];
extern long npc_img_size;
void pmem_init();
void pmem_load_bin(const char *path);
uint32_t pmem_read(uint32_t addr, int len);
void     pmem_write(uint32_t addr, int len, uint32_t data);
#endif
#endif
```

关键设计：
- `dpi_mem_read/write` 用 `extern "C"`（DPI-C 必须是 C 链接）
- 物理常量集中定义，取代散落在 Kconfig/Verilog 中的定义
- `npc_pmem[]` 暴露给其他模块（itrace/difftest 需要）

#### 3.3.6 `csrc/memory/memory.cpp` — [NEW] 统一内存实现

**加载简化**（对比改造前的逐字循环）：
```cpp
// 改造前：逐 4 字节循环，分别写 IMEM、DMEM、C++ 数组（三次写）
while (fread(buf,1,4,fp) == 4){
    word = buf[0] | ...; npc_imem(top,idx,word,WRITE);
    npc_dmem(top,idx,word,WRITE); memcpy(&npc_pmem[idx*4],buf,4); idx++;
}
// 改造后：一句搞定
fread(npc_pmem, 1, npc_img_size, fp);
```

**DPI-C 统一读写实现**：
```cpp
int dpi_mem_read(int addr) {
    uint32_t paddr = (uint32_t)addr;
    if (paddr >= PMEM_BASE && paddr < PMEM_END) {
        uint32_t word_addr = paddr & ~3U;         // ← 字对齐（关键！）
        return *(uint32_t*)(npc_pmem + (word_addr - PMEM_BASE));
    }
    // MMIO 设备读
    switch (paddr) {
        case NPC_SERIAL_PORT: return 0;            // UART 只写
        case NPC_RTC_ADDR:    return rtc_read();   // RTC
        default: return 0;
    }
}

void dpi_mem_write(int addr, int data, int wmask) {
    uint32_t paddr = (uint32_t)addr;
    if (paddr >= PMEM_BASE && paddr < PMEM_END) {
        uint32_t word_addr = paddr & ~3U;         // ← 字对齐
        uint32_t offset = word_addr - PMEM_BASE;
        for (int i = 0; i < 4; i++)
            if (wmask & (1 << i))
                npc_pmem[offset + i] = ((uint32_t)data >> (i*8)) & 0xFF;
        return;
    }
    // MMIO 设备写
    switch (paddr) {
        case NPC_SERIAL_PORT: npc_serial_putc(data & 0xFF); break;
        default: break;
    }
}
```

#### 3.3.7 其他文件清理

| 文件 | 改动 |
|------|------|
| `main.cpp` | 删除 `#include <device.h>`、`npc_pmem[]` 定义、MMIO 拦截块；`npc_imem(...)` → `top->instr` |
| `monitor.cpp` | `load_bin()` 删除，改用 `pmem_init()` + `pmem_load_bin()` |
| `npc.h` | 删除 `npc_imem()`/`npc_dmem()` 访问器，`extern npc_pmem` 由 `memory.h` 提供 |
| `device.h` | 仅保留 `npc_serial_putc` 声明 |
| `sdb/npc_isa.cpp` | `npc_dmem(top,idx,0,READ)` → `pmem_read(addr, len)` |
| `difftest/dut.cpp` | `img_size` → `npc_img_size`（来自 memory.cpp） |

### 3.4 迁移 Bug 修复记录

#### Bug #1：C/C++ 类型提升导致的符号扩展陷阱

**影响**：所有地址 `>= 0x80000000` 的判断全部失败，指令取指返回全零。

**根因**：DPI-C 接口传 `int`（32-bit signed），`PMEM_BASE` 为 `unsigned long`（64-bit）。比较时 `int` 先符号扩展到 64-bit 再比较：

```
addr 从 Verilog 传来:  0x80000004 (32-bit 位模式)
DPI-C → C int:         -2147483644  (signed 32-bit)
比较时 → unsigned long: 0xFFFFFFFF80000004  ← 符号扩展！
PMEM_BASE = 0x0000000080000000 → 0xFFFFFFFF80000004 >= 0x0000000080000000 → true ✅
但是: 0xFFFFFFFF80000004 < 0x0000000088000000 → FALSE ❌（远大于 PMEM_END！）

结论: 所有合法地址被判断为"不在物理内存范围" → return 0 → CPU 拿 NOP 指令
```

**修复**：
```cpp
// memory.h — 常量用 32-bit unsigned
#define PMEM_BASE 0x80000000U           // unsigned int (32-bit)
#define PMEM_SIZE (128U * 1024 * 1024)

// memory.cpp — 显式转换去掉符号位
int dpi_mem_read(int addr) {
    uint32_t paddr = (uint32_t)addr;     // ← 关键！禁止符号扩展
    if (paddr >= PMEM_BASE && paddr < PMEM_END) { ... }
}
```

> **教训**：DPI-C Verilog `int` 对应 C `int`（signed 32-bit）。处理硬件地址（unsigned 语义）时，C++ 侧必须先 `(uint32_t)` 转换再参与比较。

#### Bug #2：字节地址 vs 字对齐不匹配

**影响**：所有涉及内存读写（`lw/lh/lb/sw/sh/sb`）的测试失败。`add` 测试出现 `gpr[2] mismatch at pc=0x80000008`。

**根因**：旧 DMEM 是 word-indexed（`dmem[(addr>>2)]`），新 `dpi_mem_read` 从精确字节地址读 `*(uint32_t*)`，LSU 拿到错误的字：

```
旧 DMEM (word-indexed):
  lb 0x80000001 → idx = (0x80000001-0x80000000)>>2 = 0
                → dmem[0] (包含 bytes 0,1,2,3)
                → LSU 从 word 中提取 byte 1 ✅

新 dpi_mem_read 修复前 (byte-indexed):
  lb 0x80000001 → offset = 0x80000001 - 0x80000000 = 1
                → *(uint32_t*)(npc_pmem+1) = bytes 1,2,3,4 ❌
                → LSU 从错字中提取 → 错数据
```

**修复**：读写都用 `paddr & ~3U` 做字对齐，匹配 LSU 的 wmask 语义。

### 3.5 改造效果对比

| 维度 | 改造前 | 改造后 |
|------|-------|--------|
| Verilog 大数组 | 2 个 (imem + dmem, 各 128MB) | 0 个 |
| `core_top` 端口数 | 6 基础 + 5 设备 = 11 | 6 基础 |
| 加载代码行数 | ~15 行（逐字循环） | 2 行（fread） |
| MMIO 新增设备 | 改 RTL + 改 C++ | 只改 C++ 的 switch |
| 内存调试 | 波形中翻找 | printf + gdb |
| 总线接口准备 | 从零开始 | 天然适配 |

MMIO 后续添加新设备只需在 `memory.cpp` 中各加一个 `case`，不需要改任何 Verilog 代码。

---

## 第四部分：difftest 差分测试对接

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

### 4.2 为什么用 dlopen 封装 NEMU API

| 问题 | 答案 |
|------|------|
| NPC 想用 NEMU 的哪些功能？ | `cpu_exec`（执行指令）、读写 GPR/PC、读写物理内存 |
| 为什么不直接 `#include` NEMU 头文件？ | NPC 和 NEMU 是**两个独立进程**，不能直接链接 |
| 怎么跨进程调用？ | 把 NEMU 编译成 `.so` 动态库，加载到 NPC 的进程空间中 |
| `.so` 里暴露什么？ | 5 个接口函数——足够驱动 NEMU 引擎、同步状态 |

### 4.3 NEMU API 封装对照表

> 5 个 `__EXPORT` 函数在 `ref.c` 中实现，编译进 `.so` 供 `dut.cpp` 调用。

| 封装函数 | 内部调用的 NEMU API | 作用 | 调用时机 |
|---------|-------------------|------|---------|
| `difftest_init(port)` | `init_mem()` + `init_isa()` | 初始化 NEMU 内存 + ISA 状态 | NPC 启动时 |
| `difftest_memcpy(addr, buf, n, TO_REF)` | `guest_to_host(addr)` + `memcpy` | 把 NPC 的 `.bin` 程序拷进 NEMU 内存 | 初始化时 |
| `difftest_regcpy(buf, TO_REF)` | `memcpy(&cpu, buf, 132)` | 把 NPC 的 32 GPR + PC 同步给 NEMU | 初始化时 |
| `difftest_exec(1)` | `cpu_exec(1)` | NEMU 解释器执行 1 条指令 | 每条指令后 |
| `difftest_regcpy(buf, TO_DUT)` | `memcpy(buf, &cpu, 132)` | 从 NEMU 读回 32 GPR + PC（标准答案） | 每条指令后 |
| `difftest_raise_intr(NO)` | 暂空 | 注入异常（阶段 3 用） | 阶段 3 |

### 4.4 ref.c 中 5 个函数的实现逻辑

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

### 4.5 dut.cpp 调用链路

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

### 4.6 dlopen 机制的核心理解

| 概念 | 说明 |
|------|------|
| `.so` 本质 | 编译好的 NEMU 解释器，不含 `main()`，只导出 5 个函数 |
| `dlopen` | 把 `.so` 加载到当前进程的内存空间——相当于"把 NEMU 引擎装进 NPC" |
| `dlsym` | 在加载的 `.so` 中按函数名查找地址，返回函数指针 |
| `__EXPORT` | 告诉编译器"这个函数要暴露给 dlopen 的人用" |
| 函数指针调用 | 拿到函数指针后，调用方式和普通函数完全一样 |

> **一句话**：`dlopen` 是把 NEMU 的代码"借"过来用——你不需要改 NPC 的 RTL，不需要改 NEMU 的 main，只需要通过 5 个函数接口驱动 NEMU 引擎，和你的 NPC 硬件同步执行并逐指令比对。

### 4.7 Difftest 事件分类与同步策略

> 核心问题不是“遇到 trap 就跳过”，而是判断 REF 和 NPC 对“一步执行”的定义是否一致。
> difftest 只适合验证确定性 CPU 执行；凡是外部设备、异步中断、宿主时间参与进来，就要先把事件分类，再决定 compare 还是 sync。

#### 4.7.1 difftest 的基本假设

`npc/csrc/difftest/dut.cpp` 中正常路径是：

```cpp
void difftest_step(Vcore_top* top, int idx, uint32_t npc_exec_pc) {
    ref_difftest_regcpy(&ref_s, DIFFTEST_TO_DUT);
    uint32_t ref_exec_pc = ref_s.pc;

    ref_difftest_exec(1);             // REF 执行 1 条

    ref_difftest_regcpy(&ref_s, DIFFTEST_TO_DUT);
    npc_state_data(top);              // NPC 执行 1 拍后的状态

    npc_s.pc = npc_exec_pc;
    ref_s.pc = ref_exec_pc;
    diff_log_write(&npc_s, &ref_s, idx);
    difftest_compare();
}
```

这段代码隐含一个前提：

```text
NPC 当前周期提交的架构状态
    ==
REF 从同一 PC 执行一条确定性指令后的架构状态
```

只要这个前提成立，就应该普通 compare；如果这个前提不成立，继续 compare 只会得到假失败。

#### 4.7.2 四类事件处理总表

| 事件类型 | 例子 | 来源 | REF 是否能独立复现 | difftest 处理 |
|----------|------|------|-------------------|---------------|
| 普通确定性指令 | `add/lw/sw/csrw/andi` | 当前指令语义 | 能 | `difftest_step()` 普通 compare |
| 同步异常 / 同步 trap | `ecall`、非法指令、对齐异常 | 当前指令确定触发 | 应该能 | 普通 compare，不能用 skip 掩盖 |
| 异步中断 | timer interrupt、external interrupt | 外部设备状态 + 指令边界采样 | 不能稳定复现 | skip compare + sync REF 到 NPC |
| MMIO 外设访问 | UART 写 `0xa00003f8`、RTC 读 `0xa0000048` | 设备地址空间副作用 | 不能稳定复现，甚至可能越界 | skip compare + sync REF 到 NPC |

一个简单判断原则：

```text
由当前指令唯一决定的状态变化：compare
由外部时间/设备/中断 pending 决定的状态变化：sync
```

#### 4.7.3 同步异常：正常 compare

`ecall` 这类不是“同步中断”，更准确叫同步异常或同步 trap。它是当前指令语义的一部分，所以 NPC 和 REF 执行到同一条 `ecall` 时，都应该进入同一个 trap 路径：

```text
ecall 指令提交语义
    ├─ mepc   ← 当前 ecall PC
    ├─ mcause ← 11
    ├─ mstatus 按 trap 规则更新
    └─ pc     ← mtvec
```

NPC 的硬件路径在 `core_top.v` 和 `csr.v`：

```verilog
// core_top.v
assign trap_enter_eff = trap_enter | interrupt_valid;
assign trap_code_eff  = interrupt_valid ? interrupt_cause : trap_code;

// csr.v
if (trap_enter) begin
    csr_mepc    <= trap_pc;
    csr_mcause  <= trap_code;
    csr_mstatus <= {csr_mstatus[31:13], 2'b11, csr_mstatus[10:8],
                    csr_mstatus[3], csr_mstatus[6:4], 1'b0, csr_mstatus[2:0]};
end
```

同步异常的关键点是：它不依赖宿主时间，也不依赖另一个独立设备模型。因此如果 `ecall` 后 CSR/PC 对不上，说明异常处理逻辑本身有问题，应该修 RTL 或 REF，而不是 skip。

#### 4.7.4 异步中断：skip compare + sync

异步中断和同步异常最大的区别是：异步中断不是当前指令的语义，而是在指令边界检查外部状态。

timer interrupt 的响应条件是：

```text
mstatus.MIE == 1
mie.MTIE    == 1
mip.MTIP    == 1
```

NPC 在 `interrupt_check()` 中检查异步中断：

```cpp
void interrupt_check() {
    top->interrupt_valid = 0;
    top->interrupt_cause = 0;
    difftest_sync_needed = false;

    if (alarm_fired) {
        alarm_fired = 0;
        word_t mip = npc_csr(top, CSR_MIP, 0, READ);
        npc_csr(top, CSR_MIP, mip | M_TIME_MASK, WRITE);
        difftest_sync_needed = true;      // 只置 pending，也要同步
    }

    word_t trap_cause = isa_query_intr();
    if (trap_cause != INTR_EMPTY) {
        top->interrupt_valid = 1;         // 硬件 trap 信号
        top->interrupt_cause = trap_cause;
        difftest_sync_needed = true;      // 本周期将响应异步中断
    }
}
```

如果 `interrupt_valid=1`，`core_top.v` 会 kill 当前指令所有副作用，并强制进入 trap：

```verilog
assign mem_read_eff   = interrupt_valid ? 1'b0 : mem_read;
assign mem_write_eff  = interrupt_valid ? 1'b0 : mem_write;
assign reg_write_eff  = interrupt_valid ? 1'b0 : reg_write;
assign csr_write_eff  = interrupt_valid ? 1'b0 : csr_write;
assign mret_eff       = interrupt_valid ? 1'b0 : mret;
assign trap_enter_eff = trap_enter | interrupt_valid;
```

这类周期不能让 REF 自己执行一条普通指令，因为 REF 不知道 NPC 在这个指令边界收到了哪个异步事件。因此处理方式是：

```text
NPC 响应异步中断
    → 本拍不 compare
    → npc_state_data(top)
    → ref_difftest_regcpy(&npc_s, DIFFTEST_TO_REF)
```

**CSR 打开中断门的特殊情况**

`csrs mstatus, a5` 这类指令本身是普通 CSR 指令，但它可能让 pending 的异步中断在执行后立刻满足条件：

```text
执行前：mstatus.MIE=0, mie.MTIE=1, mip.MTIP=1  → 不能响应
执行后：mstatus.MIE=1, mie.MTIE=1, mip.MTIP=1  → 可以响应
```

NPC 的 `interrupt_check()` 在本周期执行前已经检查过一次，所以本拍只提交 `csrs`；下一周期入口才响应中断。REF 的 `ref_difftest_exec(1)` 可能在执行完这条 CSR 后立即做一次中断查询，于是 REF 的“一步”变成：

```text
csrs mstatus, a5 + timer interrupt
```

而 NPC 的“一步”仍然是：

```text
csrs mstatus, a5
```

所以 `main.cpp` 在 `top->clk=1; top->eval();` 后补一次 post 检查：

```cpp
bool post_intr_pending = !is_trap && (isa_query_intr() != INTR_EMPTY);
```

注意这个判断必须在当前指令执行后。放在 `eval()` 前会漏掉“CSR 刚打开 MIE”的那一拍。

#### 4.7.5 MMIO 外设：skip compare + sync

MMIO 的问题不是 PC 错拍，而是 REF 很可能没有同一个设备模型。以 UART 为例，AM 写串口会访问：

```text
SERIAL_PORT = 0xa00003f8
```

NPC 的 DPI-C 内存路径会把它分发到设备：

```cpp
void dpi_mem_write(int addr, int data, int wmask) {
    if (paddr >= PMEM_BASE && paddr < PMEM_END) {
        // 普通物理内存
        return;
    }

    mmio_accessed = true;
    switch (paddr) {
        case NPC_SERIAL_PORT:
            if (wmask & 0x1) npc_serial_putc(wdata & 0xFF);
            break;
    }
}
```

但 REF 如果没有打开设备，执行同一条 `sb ..., 0xa00003f8` 会走普通物理内存检查，然后报：

```text
address = 0xa00003f8 is out of bound of pmem
```

因此 MMIO 周期必须 skip compare，并把 NPC 状态同步给 REF。这里有一个容易踩的点：`pmem_mmio_accessed()` 是读后清零函数。

```cpp
bool pmem_mmio_accessed() {
    bool v = mmio_accessed;
    mmio_accessed = false;
    return v;
}
```

所以一个周期内只能读一次，必须缓存：

```cpp
bool mmio_accessed = pmem_mmio_accessed();
```

不能在外层 `if` 和内层条件里各调用一次，否则第一次已经清零，第二次会误判为非 MMIO，导致 REF 执行串口访问而崩溃。

#### 4.7.6 single_cycle 决策图

当前 `single_cycle()` 的 difftest 分支可以概括为：

```cpp
void single_cycle() {
    interrupt_check();                         // 执行前异步事件检查
    uint32_t this_pc = npc_pc(top, 0, READ);
    bool is_trap = top->interrupt_valid;

    top->clk = 1; top->eval();                 // NPC 提交一拍
    top->interrupt_valid = 0;
    top->interrupt_cause = 0;
    halt_check();

    bool post_intr_pending = !is_trap && (isa_query_intr() != INTR_EMPTY);
    bool mmio_accessed = pmem_mmio_accessed();

    if (mmio_accessed || difftest_sync_needed || post_intr_pending) {
        if (difftest_sync_needed && !mmio_accessed && !is_trap && !post_intr_pending) {
            ref_difftest_exec(1);
        }
        npc_state_data(top);
        ref_difftest_regcpy(&npc_s, DIFFTEST_TO_REF);
        difftest_sync_needed = false;
    } else {
        difftest_step(top, cycle, this_pc);
    }
}
```

完整决策图：

```text
single_cycle()
    │
    ▼
interrupt_check()
    ├─ alarm_fired?
    │     └─ 置 MTIP, difftest_sync_needed=1
    └─ isa_query_intr()?
          └─ interrupt_valid=1, interrupt_cause=trap_cause, difftest_sync_needed=1

    │
    ▼
posedge clk / top->eval()
    ├─ interrupt_valid=1
    │     └─ core_top kill 当前指令副作用，csr.v 进入 trap
    └─ interrupt_valid=0
          └─ 当前指令正常提交，可能产生 MMIO 或打开 MIE

    │
    ▼
difftest 决策点
    ├─ post_intr_pending=1?
    │     └─ CSR 等指令后异步中断变为可响应 → sync
    ├─ mmio_accessed=1?
    │     └─ 外设读写，REF 不能复现 → sync
    ├─ difftest_sync_needed=1?
    │     ├─ is_trap=1 → NPC 已响应异步中断 → sync
    │     └─ 仅 timer pending，且不是 MMIO/post_intr
    │          └─ REF 先 exec(1) 保留普通指令内存副作用，再 sync
    └─ 三者都为 0
          └─ ref_difftest_exec(1) + compare
```

为什么“仅 timer pending 但没有 trap”时要先 `ref_difftest_exec(1)`？因为 NPC 本周期仍然正常提交了一条普通指令。`ref_difftest_regcpy()` 只同步 GPR/PC/CSR，不同步普通内存，所以如果当前指令是普通 `sw`，必须让 REF 也执行这一条来保留内存副作用，然后再用 NPC 的寄存器/CSR 状态覆盖 REF。

#### 4.7.7 源码索引

| 文件 | 关键内容 |
|------|----------|
| `npc/csrc/difftest/dut.cpp` | `difftest_step()` 的正常一条指令 compare 路径 |
| `npc/csrc/main.cpp` | `single_cycle()` 中 `mmio_accessed / difftest_sync_needed / post_intr_pending` 的分流 |
| `npc/csrc/monitor/interrupt.cpp` | `alarm_fired`、`isa_query_intr()`、`interrupt_valid`、`difftest_sync_needed` |
| `npc/csrc/memory/memory.cpp` | MMIO 读写分发、`pmem_mmio_accessed()` 读后清零标志 |
| `npc/vsrc/core_top.v` | `interrupt_valid` 触发 kill，统一关掉 reg/mem/csr 写副作用 |
| `npc/vsrc/csr.v` | `trap_enter` 时更新 `mepc/mcause/mstatus`，清理 `mip` pending 位 |
| `nemu/src/cpu/cpu-exec.c` | REF 执行一条指令后可能查询异步中断，是 CSR 打开 MIE 错拍的来源 |
| `nemu/src/memory/paddr.c` | REF 未启用设备时，MMIO 地址会被判为 pmem 越界 |

### 4.8 M 扩展乘除指令：显式语义与调试记录

> 这部分放在第四部分，是因为它本质上来自 difftest 失配：先在对拍过程中暴露 M 扩展语义不一致，再把指令行为显式化，让 NPC 的结果和 NEMU/ISA 对齐。
> 这部分对应 `npc/vsrc/control.v` 和 `npc/vsrc/alu.v`。
> 先看指令本身的 ISA 语义，再看 RTL 从“隐式依赖工具行为”到“显式写死语义”的改造。

#### 4.8.1 先看 DIV/REM 指令本身的语义
向零截断的有符号除法和余数，除零和边界单独钉死：也就是保留整数部分的最大范围，避免溢出。
余数的符号跟被除数保持一致，而不是跟除数保持一致。
| 指令 | ISA 语义 | 除零行为 | 边界行为 |
|------|----------|----------|----------|
| DIV  | 有符号除法，商向 0 截断 | 返回 `0xffffffff` | `INT_MIN / -1` 返回 `INT_MIN` |
| DIVU | 无符号除法 | 返回 `0xffffffff` | 无额外有符号溢出边界 |
| REM  | 有符号余数，余数符号跟被除数 | 返回被除数 | `INT_MIN % -1` 返回 `0` |
| REMU | 无符号余数 | 返回被除数 | 无额外有符号溢出边界 |

对于有符号余数，可以用下面这条关系式理解：

$$a = b \times q + r$$

其中 `q` 是 DIV 的商，`r` 是 REM 的余数。RISC-V 要求有符号除法向 0 截断，所以 `r` 的符号必须跟被除数 `a` 保持一致，而不是跟除数 `b` 一致。

#### 4.8.2 MULH 调试记录：高位乘法必须先扩到 64 位

`mul-longlong` 测试曾在下面这条指令上触发 difftest 失配：

```text
ABORT: GPR[15] mismatch at pc=0x800000ac
0x800000ac: 02fc97b3  mulh a5, s9, a5
```

波形和反汇编对应的译码是：

```text
funct7 = 7'b0000001
funct3 = 3'b001
alu_op = 5'b1_0001   // MULH
```

当时 `s9 = a5 = 0xaeb1c2aa`。低 32 位乘法 `mul` 已经得到正确结果 `0xdb1a18e4`，但 `mulh` 写回 `a5` 时 NPC 得到 `0x00000000`，NEMU 参考结果是 `0x19d29ab9`：

```text
signed 32x32 full product = 0x19d29ab9_db1a18e4
MUL  取低 32 位: 0xdb1a18e4
MULH 取高 32 位: 0x19d29ab9
```

出错写法是：

```verilog
5'b1_0001 : result = (($signed(src1) * $signed(src2)) >> 32);  // MULH
5'b1_0010 : result = (($signed(src1) * src2) >> 32);           // MULHSU
5'b1_0011 : result = ((src1 * src2) >> 32);                    // MULHU
```

这里的关键误区是：`$signed(src1)` 只改变 signedness，不改变位宽。`src1/src2` 仍然是 32 位操作数，高位乘法表达式没有明确产生 64 位中间乘积。Verilator 按这个表达式生成 C++ 时，`MULH/MULHSU/MULHU` 的高位结果路径会退化成 0，因此波形里看到 ALU 结果为 0 不是写回通路问题，而是 ALU 的高位乘法语义没有写出来。

正确做法是：先按指令语义把两个 32 位操作数扩展成 64 位，再做 64 位乘法，最后取 `[63:32]`。

```verilog
wire signed [63:0] src1_s64 = {{32{src1[31]}}, src1};
wire signed [63:0] src2_s64 = {{32{src2[31]}}, src2};
wire        [63:0] src1_u64 = {32'b0, src1};
wire        [63:0] src2_u64 = {32'b0, src2};
wire signed [63:0] src2_zext_s64 = {32'b0, src2};

wire signed [63:0] mul_ss = src1_s64 * src2_s64;      // MULH
wire signed [63:0] mul_su = src1_s64 * src2_zext_s64; // MULHSU
wire        [63:0] mul_uu = src1_u64 * src2_u64;      // MULHU

5'b1_0000 : result = src1 * src2;     // MUL，低 32 位不区分 signed/unsigned
5'b1_0001 : result = mul_ss[63:32];   // MULH
5'b1_0010 : result = mul_su[63:32];   // MULHSU
5'b1_0011 : result = mul_uu[63:32];   // MULHU
```

三条高位乘法的扩展规则必须区分清楚：

| 指令 | 乘法语义 | 扩展方式 |
|------|----------|----------|
| MULH | signed x signed，取高 32 位 | `rs1` 符号扩展，`rs2` 符号扩展 |
| MULHSU | signed x unsigned，取高 32 位 | `rs1` 符号扩展，`rs2` 零扩展 |
| MULHU | unsigned x unsigned，取高 32 位 | `rs1` 零扩展，`rs2` 零扩展 |

所以这类 bug 的判断顺序是：先确认 `control.v` 是否把 `funct7=0000001/funct3=001` 译成 `alu_op=1_0001`，再看 `alu.v` 是否真的构造了 64 位乘积。只看 `$signed(...)` 不够，它不会自动把 32 位乘法变成 64 位乘法。

#### 4.8.3 修改前的逻辑：更像“能跑”，但语义不够显式

在改造前，M 扩展常见的写法是直接让 RTL 依赖 Verilog 的 `/` 和 `%`，例如：

```verilog
5'b1_0100 : result = (src2 == 0) ? ~32'd0 : $signed(src1) / $signed(src2);  // DIV
5'b1_0101 : result = (src2 == 0) ? ~32'd0 : src1 / src2;                    // DIVU
5'b1_0110 : result = (src2 == 0) ? src1 : $signed(src1) % $signed(src2);    // REM
5'b1_0111 : result = (src2 == 0) ? src1 : src1 % src2;                      // REMU
```

这类写法的问题不是“完全错”，而是语义太隐式：

- DIV / REM 依赖 `$signed(...)`，DIVU / REMU 依赖默认无符号行为，符号含义被散落在表达式里。
- 除零只做了简单兜底，但没有把 `INT_MIN / -1`、`INT_MIN % -1` 这种边界单独钉死。
- 一旦结果不对，很难快速判断是译码、符号位、还是工具对 `/` 和 `%` 的解释出了问题。

如果再往上看一层，编译链里在没有 M 扩展硬件时，还会把 `/` 和 `%` 降级到 libgcc 的软件实现。这样一来，同一类运算的语义会分散在编译器、库函数和 RTL 三层，difftest 的定位成本很高。

#### 4.8.4 修改后的逻辑：把语义固定在 RTL 里

现在的实现把四条指令拆成了四个显式函数：`signed_div32`、`unsigned_div32`、`signed_rem32`、`unsigned_rem32`。

`control.v` 先用 `funct7 == 7'b00000001` 锁定 M 扩展，再用 `funct3` 把四条指令映射成 `alu_op`：

```verilog
3'b100 : alu_op = 5'b1_0100;  // DIV
3'b101 : alu_op = 5'b1_0101;  // DIVU
3'b110 : alu_op = 5'b1_0110;  // REM
3'b111 : alu_op = 5'b1_0111;  // REMU
```

`alu.v` 里对应的语义是：

- `signed_div32`：
    - 除数为 0 时返回 `0xffffffff`
    - `INT_MIN / -1` 返回 `INT_MIN`
    - 其他情况先把两个操作数取绝对值做无符号除法，再按符号位异或恢复结果符号
- `unsigned_div32`：
    - 除数为 0 时返回 `0xffffffff`
    - 其他情况直接做无符号除法
- `signed_rem32`：
    - 除数为 0 时返回被除数本身
    - `INT_MIN % -1` 返回 0
    - 其他情况先按绝对值取模，再把余数符号恢复成被除数的符号
- `unsigned_rem32`：
    - 除数为 0 时返回被除数本身
    - 其他情况直接做无符号取模

对应的 RTL 结果不再依赖工具对 signedness 的默认理解，而是直接把 ISA 规则写在执行路径里。这样做的好处是：第一条 M 扩展指令一旦算错，问题会立刻暴露在 control/alu 这两层，不会把错误静默传播到后面的寄存器和地址计算里。

#### 4.8.5 修改前后对比

| 维度 | 修改前 | 修改后 |
|------|------|------|
| 语义来源 | 依赖 Verilog `/`、`%` 和 `$signed(...)` 的组合行为 | 由 `signed_div32` / `unsigned_div32` / `signed_rem32` / `unsigned_rem32` 显式实现 |
| 除零处理 | 在表达式里简单三目兜底 | 按每条指令单独写清楚返回值 |
| signed/unsigned | DIV/REM 和 DIVU/REMU 混在同一个表达式风格里 | 先在 control 里分出 alu_op，再在 ALU 里按语义拆分 |
| 边界值 | `INT_MIN / -1`、`INT_MIN % -1` 没有单独钉死 | 两个边界都被显式特判 |
| 调试定位 | 错误可能来自工具行为、表达式类型、或边界值 | 错误只会落在译码或显式语义函数里，定位更直接 |

如果把“编译链”和“RTL 执行”合在一起看，修改前更像“软件先兜底、RTL 只管把表达式写出来”，修改后则是“编译器只负责产出 M 指令，RTL 负责按 ISA 规则逐条执行”。这就是这次改造真正想要的差别。

---

## 第五部分：波形与调试 🔜

> **状态**：待 difftest 部分完全稳定。
> **内容**：VCD 波形输出，GTKWave 查看，断言框架。

---

## 第六部分：中断响应的核心设计逻辑

> 单周期 CPU 中，中断如何抢占当前指令？为什么 kill 的是"当前指令"而非"下一条指令"？
> 本节从时序、硬件行为、RISC-V 规范三方面完整论证。

### 6.1 问题起点：中断在哪个时刻被检测？

```cpp
// main.cpp — single_cycle()
void single_cycle() {
    interrupt_check();                           // ① clk=0 阶段：检测中断，设置 interrupt_valid
    uint32_t npc->pc = npc_pc(top, 0, READ);     // ② 记录当前 PC

    top->clk = 1; top->eval();                    // ③ 上升沿：状态更新
    // ...
}
```

**关键时序**：中断检测发生在 `clk=0` 阶段（组合逻辑稳定期），此时当前指令已经完成了译码和执行——ALU 算出了结果、LSU 准备好了读写数据、控制信号已经广播到各模块。一切就绪，只等 `posedge clk` 提交。

### 6.2 核心设计决策：kill 当前指令，而非下一条指令

```
                            中断检测点（clk=0）
                                  │
    ┌─────────┬─────────┬─────────┼─────────┬─────────┐
    │  取指    │  译码    │  执行   │  提交？  │  写回   │
    │ instr   │ control │  ALU   │  ┌──┐  │ regfile │
    │ 已取出   │ 已生成   │ 已算出  │  │✗│  │ 未写入  │
    └─────────┴─────────┴─────────┴──┴──┴──┴─────────┘
                                        │
                                   posedge clk:
                                   reg_write_eff=0 ← KILL
```

**设计选择**：

| 方案 | 行为 | 结果 |
|------|------|------|
| **kill 当前**（✅ NPC 采用） | 当前指令已完成执行但不提交，mepc=当前PC | mret 后重新执行被中断的指令 |
| kill 下一条 | 当前指令正常提交，下一周期才响应中断 | 违背精确中断模型（当前指令逃逸了） |

### 6.3 为什么 kill 下一周期是错的？

假设 kill 下一周期：

```
周期 N:   pc=0x100, 执行 addi t0,t1,5
          中断到来 → 但"kill 下一条" → addi 正常提交，t0 被写入
          设 mepc=0x104（pc+4）→ mret 跳转到 addi 的下一条指令

周期 N+1: pc=mtvec → 中断处理程序执行
...
mret:     pc=mepc=0x104

问题:    addi 被中断打断了却仍然提交了结果，但程序流中它"看起来没执行过"。
         更致命的是：如果 addi 是 load 指令？store 指令？分支指令？
         这些指令一旦提交，状态就不可逆了。
```

**单周期 CPU 的固有约束**：所有指令在同一周期内完成。不存在"提交了一半"的中间状态。要么全部提交，要么全部 kill。中断在 `clk=0` 检测到，只能在同一个 `posedge clk` 响应——而此刻当前指令也正好在同一个 `posedge clk` 提交。**二者在同一时钟沿竞争，中断必须赢。**

### 6.4 RISC-V 规范的要求：精确中断

RISC-V 特权规范（Volume II, Section 3.1.15）规定：

> 同步异常和中断都是**精确的**（precise）：被 trap 的指令之前的所有指令都已完整执行，被 trap 的指令及之后的指令都没有对架构状态产生任何可见的副作用。

翻译成单周期 CPU 的行为：

| 规范要求 | NPC 实现 |
|---------|---------|
| trap 之前的指令都已完整执行 | ✅ 上一周期 posedge 已经全部提交 |
| trap 的指令没有产生副作用 | ✅ `*_eff=0` kill 所有写操作 |
| mepc 指向被 trap 的指令 | ✅ `mepc <= pc`（当前正在执行的指令） |
| mret 后从 mepc 重新开始 | ✅ `pc_next = mepc` |

### 6.5 逐周期还原：完整的硬件状态迁移

```
══════════════════════════════════════════════════════════════════
周期 N（中断发生周期）
══════════════════════════════════════════════════════════════════

clk=0 阶段（组合逻辑稳定）:
  ├─ if_stage:  DPI-C 取指完成，instr 已稳定
  ├─ id_stage:  control 控制信号已生成（reg_write=1, mem_read=1...）
  ├─ ex_stage:  ALU 结果已算出，br_taken 已确定
  ├─ mem_stage: lsu 已准备好 mem_wdata/mem_wmask
  ├─ wb_stage:  MUX 输出已稳定（reg_wdata 待写入 regfile）
  └─ C++ side:  interrupt_check() 检测到中断 → interrupt_valid=1

        当前指令的"计算结果"全部就绪，
        但尚未提交到任何架构状态（regfile/mem/csr）。

clk=1 上升沿（状态更新，不可逆）:

  ┌─ 仲裁层 ─────────────────────────────────────────────┐
  │ interrupt_valid=1 → 所有 *_eff 信号 kill 当前指令:     │
  │   reg_write_eff  = 0  → 当前指令的 rd 不被写入         │
  │   mem_write_eff  = 0  → 当前 store 不发生              │
  │   csr_write_eff  = 0  → 当前 CSR 写不发生              │
  │   mret_eff       = 0  → 当前指令即使是 mret 也被忽略    │
  │   is_ebreak_eff  = 0  → 当前指令即使是 ebreak 也不停止  │
  │   trap_enter_eff = 1  → 强制进入 trap                  │
  │   trap_code_eff  = interrupt_cause                     │
  └──────────────────────────────────────────────────────┘

  ┌─ csr.v (trap_enter_eff=1) ──────────────────────────┐
  │ mepc    ← pc            ← 记录"被中断指令的地址"      │
  │ mcause  ← trap_code     ← 记录中断原因编号            │
  │ mstatus ← 保存并关中断   ← MIE→MPIE, MPP=3, MIE=0    │
  │ trap_target = mtvec     ← 输出给 if_stage            │
  └──────────────────────────────────────────────────────┘

  ┌─ if_stage ──────────────────────────────────────────┐
  │ pc ← trap_target = mtvec ← PC 跳转到中断向量基址     │
  └──────────────────────────────────────────────────────┘

  周期 N 的本质：完成了一次硬件级自动上下文保存 + 跳转。
  这不需要执行任何指令，全部由组合逻辑 + posedge clk 自动完成。

══════════════════════════════════════════════════════════════════
周期 N+1（中断处理程序第一条指令）
══════════════════════════════════════════════════════════════════

clk=0: DPI-C 从 PMEM[mtvec] 取指 → instr = 中断处理程序入口指令
clk=1: interrupt_valid 已清零，*_eff 恢复为原始控制信号
       中断处理程序第一条指令正常提交

══════════════════════════════════════════════════════════════════
周期 N+2 ~ N+k-1：中断处理程序正常执行
══════════════════════════════════════════════════════════════════

══════════════════════════════════════════════════════════════════
周期 N+k（mret 返回）
══════════════════════════════════════════════════════════════════

clk=1 上升沿:
  ┌─ csr.v (mret=1) ───────────────────────────────────┐
  │ mstatus 恢复:  MIE←MPIE, MPIE←1, MPP←0             │
  │ trap_target = mepc   ← 输出周期 N 保存的返回地址     │
  └─────────────────────────────────────────────────────┘

  ┌─ if_stage ─────────────────────────────────────────┐
  │ pc ← mepc ← 跳回主程序中被打断的那条指令             │
  └─────────────────────────────────────────────────────┘

══════════════════════════════════════════════════════════════════
周期 N+k+1（主程序恢复）
══════════════════════════════════════════════════════════════════

clk=0: DPI-C 从 PMEM[mepc] 取指 → 重新执行被中断的指令
clk=1: 该指令正常提交（这次没有中断干扰）
```

### 6.6 优先级链条

在 PC 选择的 MUX 中，优先级从高到低为：

```
trap_enter / mret           ← 最高优先级：覆盖一切
    │
    ├─ pc_sel = 00 (PC+4)   ← 默认顺序执行
    ├─ pc_sel = 01 (jal)    ← 无条件跳转
    ├─ pc_sel = 10 (jalr)   ← 间接跳转
    └─ pc_sel = 11 (branch) ← 条件分支
```

```verilog
// if_stage.v — 优先级通过 MUX 的排列顺序实现：
assign pc_next = (trap_enter | mret) ? trap_target :   // ← 第一条件分支
                 (pc_sel == 2'b00)   ? pc + 32'd4   :  // ← 第二条件
                 (pc_sel == 2'b01)   ? pc + imm_jal  :
                 (pc_sel == 2'b10)   ? {jump_jalr[31:1], 1'b0} :
                 (pc_sel == 2'b11)   ? (br_taken ? pc + imm_br : pc + 32'd4) :
                                       pc + 32'd4;
```

在状态修改权限上，优先级从高到低为：

```
interrupt_valid = 1
    │
    ├─ *_eff = 0  → kill 当前指令的所有写操作（reg/mem/csr/mret/ebreak）
    │
    └─ *_eff = 原始控制信号  → 正常执行
```

### 6.7 源码验证清单

| 验证点 | 源码位置 | 关键代码 |
|--------|---------|---------|
| 中断检测在 clk=0 | `csrc/main.cpp:single_cycle()` | `interrupt_check()` 在 `top->clk=1` 之前调用 |
| 仲裁层 kill 写操作 | `vsrc/core_top.v` | 7 条 `assign *_eff = interrupt_valid ? 1'b0 : ...` |
| 仲裁层注入 trap | `vsrc/core_top.v` | `assign trap_enter_eff = trap_enter \| interrupt_valid` |
| PC 选择中 trap 优先级最高 | `vsrc/if_stage.v` | `(trap_enter \| mret) ? trap_target : ...` |
| mepc 保存当前 PC | `vsrc/csr.v` | `if (trap_enter) csr_mepc <= trap_pc` |
| trap_target 选择 mtvec/mepc | `vsrc/csr.v` | `trap_enter ? csr_mtvec : mret ? csr_mepc : 0` |
| 中断脉冲仅持续一拍 | `csrc/main.cpp` | `top->interrupt_valid = 0` 在 eval 后立即清除 |
| 中断周期跳过 difftest | `csrc/main.cpp` | `if (pmem_mmio_accessed() \|\| has_interrupt)` |

### 6.8 核心设计原则总结

| # | 原则 | 解释 |
|:--:|------|------|
| 1 | **中断检测在组合逻辑期** | `clk=0` 阶段 poll 中断条件，当前指令此时已计算出结果但未提交 |
| 2 | **kill 当前而非下一条** | 中断抢占的是"提交权"而非"取指权"，符合单周期"全部或全不"的原子性 |
| 3 | **仲裁层统一关门** | 通过 `*_eff` 信号集中管理，避免每个模块自己判断中断 |
| 4 | **mepc 指向被中断指令** | 保证 mret 后精确恢复到断点，符合 RISC-V 精确中断模型 |
| 5 | **中断脉冲单周期有效** | `interrupt_valid` 在 eval 后立即清零，避免下一周期误触发 |
| 6 | **硬件自动保存上下文** | 不需执行任何指令：mepc/mcause/mstatus 全由 csr.v 的时序逻辑自动完成 |
| 7 | **中断与异常共用 trap 路径** | `trap_enter_eff = trap_enter \| interrupt_valid`，统一走 mepc/mtvec/mcause 流程 |

---

## 第七部分：MMIO 外设架构重构 — 统一回调分发机制

> 从硬编码 `switch-case` 到 NEMU 风格的设备注册表 + 回调分发。
> 覆盖 serial（串口）和 RTC（时钟）两个外设的完整设计与实现。

### 7.1 设计动机

#### 7.1.1 旧架构问题

| 问题 | 说明 |
|------|------|
| 地址硬编码 | `NPC_SERIAL_PORT 0xa00003f8` 写死在 `memory.h`，无法 Kconfig 配置 |
| 设备逻辑散落 | RTC 读逻辑 (锁存/时间计算) 混在 `memory.cpp` 的 switch-case 里 |
| 新增设备代价高 | 每加一个设备（键盘、VGA、声卡）要在 2 个 switch 里各加 case |
| 无重叠检查 | 新设备可能和已有设备地址冲突，编译期发现不了 |

#### 7.1.2 新架构目标

```
dpi_mem_read/write → mmio_read/write → 查设备表 → 调回调
                                             ↑
                              maps[] 统一注册表（启动时填充）
```


### 7.2 整体架构图

```
┌─────────────────────────────────────────────────────────────┐
│                      RTL (Verilog)                          │
│                                                             │
│  if_stage.v:  assign instr = dpi_mem_read(pc, 0);          │
│  mem_stage.v: assign rdata = dpi_mem_read(addr, mem_read); │
│  mem_stage.v: dpi_mem_write(addr, wdata, wmask);           │
└───────────────────────┬─────────────────────────────────────┘
                        │  DPI-C (import "DPI-C" function)
                        ▼
┌─────────────────────────────────────────────────────────────┐
│               memory.cpp (统一入口，不区分设备)               │
│                                                             │
│  dpi_mem_read(addr, is_load):                               │
│    if (addr ∈ PMEM) → pmem_read()      // 物理内存          │
│    if (!is_load) return 0;             // 非 load 指令门控  │
│    mmio_accessed = true;                                     │
│    return mmio_read(addr);             // 委托分发           │
│                                                             │
│  dpi_mem_write(addr, data, wmask):                          │
│    if (addr ∈ PMEM) → pmem_write()     // 物理内存          │
│    mmio_accessed = true;                                     │
│    mmio_write(addr, data, wmask);      // 委托分发           │
└───────────────────────┬─────────────────────────────────────┘
                        │
                        ▼
┌─────────────────────────────────────────────────────────────┐
│              device/mmio.cpp (分发层)                        │
│                                                             │
│  maps[] 设备注册表 (NR_MAP=16):                              │
│  ┌──────────┬──────────┬──────────┬──────────┬────────────┐ │
│  │ name     │ space    │ low/high │ wmask    │ read/write │ │
│  ├──────────┼──────────┼──────────┼──────────┼────────────┤ │
│  │"serial"  │ 8B space │0xa00003f8│   0x1    │ NULL / cb  │ │
│  │          │          │ ~3ff     │          │            │ │
│  ├──────────┼──────────┼──────────┼──────────┼────────────┤ │
│  │"rtc"     │ 8B space │0xa0000048│   0xF    │ cb / NULL  │ │
│  │          │          │ ~4f      │          │            │ │
│  └──────────┴──────────┴──────────┴──────────┴────────────┘ │
│                                                             │
│  mmio_read(addr):  遍历 maps → 命中 → 回调(或 space 直读)   │
│  mmio_write(addr): 遍历 maps → 命中 → space 先写 → 再回调   │
└───────────────────┬─────────────────┬───────────────────────┘
                    │                 │
          ┌─────────┘                 └─────────┐
          ▼                                     ▼
┌───────────────────┐                 ┌───────────────────┐
│ device/serial.cpp │                 │ device/rtc.cpp    │
│                   │                 │                   │
│ serial_write_     │                 │ rtc_read_handler  │
│ handler()         │                 │ (回调: 读)        │
│ (回调: 写)        │                 │ + host_time_us()  │
│ + npc_serial_     │                 │ + init_timer_     │
│ putc()            │                 │ alarm() (中断源)  │
└───────┬───────────┘                 └─────────┬─────────┘
        │                                       │
        ▼                                       ▼
   fputc(stderr)                         steady_clock::now()
   终端输出                              宿主微秒时间
```


### 7.3 文件组织

| 文件 | 分类 | 职责 |
|------|------|------|
| `npc/Kconfig` | 配置 | 设备开关 + MMIO 地址 |
| `npc/include/generated/autoconf.h` | 生成 | `make menuconfig` 自动生成 |
| `npc/include/mmio.h` | 框架头 | `MMIODevice` 结构体、回调类型、`mmio_read/write` 声明 |
| `npc/include/device.h` | 设备头 | 各外设回调函数声明 |
| `npc/include/memory.h` | 内存头 | PMEM 宏定义、`in_pmem()`、`dpi_mem_read/write` 声明 |
| `npc/csrc/device/mmio.cpp` | 框架实现 | IO 空间池、设备注册表、统一分发、space 读写 |
| `npc/csrc/device/serial.cpp` | 串口设备 | `npc_serial_putc()` + 回调 `serial_write_handler()` |
| `npc/csrc/device/rtc.cpp` | RTC 设备 | `host_time_us()` + 回调 `rtc_read_handler()` + 中断源 `init_timer_alarm()` |
| `npc/csrc/memory/memory.cpp` | DPI-C 入口 | `dpi_mem_read/write` → `mmio_read/write` 委托 |

### 7.4 Kconfig 配置

```kconfig
menu "Device Configuration"
    config DEVICE
        bool "Enable Device support"
        default y

    config HAS_SERIAL
        bool "Enable Serial (UART)"
        default y
        depends on DEVICE

    config SERIAL_MMIO
        hex "Serial port MMIO base address"
        default 0xa00003f8
        depends on HAS_SERIAL

    config HAS_TIMER
        bool "Enable Timer"
        default y
        depends on DEVICE

    config RTC_MMIO
        hex "RTC MMIO base address"
        default 0xa0000048
        depends on HAS_TIMER
endmenu
```

生成的 `autoconf.h` 关键宏：`CONFIG_DEVICE 1`, `CONFIG_HAS_SERIAL 1`, `CONFIG_SERIAL_MMIO 0xa00003f8`, `CONFIG_HAS_TIMER 1`, `CONFIG_RTC_MMIO 0xa0000048`。

### 7.5 核心数据结构 — `mmio.h`

```cpp
typedef uint32_t (*mmio_read_cb)(MMIODevice *dev, uint32_t offset);
typedef void (*mmio_write_cb)(MMIODevice *dev, uint32_t offset,
                              uint32_t data, uint8_t wmask);

struct MMIODevice {
    const char *name;       // 设备名，调试用
    uint8_t    *space;      // 设备寄存器状态区 (IO 空间池分配)
    uint32_t    addr_start; // MMIO 起始地址 (含)
    uint32_t    addr_end;   // MMIO 结束地址 (含)
    uint32_t    wmask;      // 默认写掩码 (0x1=字节写, 0xF=字写)
    mmio_read_cb  read;     // 读回调 (NULL=space 直读)
    mmio_write_cb write;    // 写回调 (NULL=space 直写)
};
```

**读写分离 vs 读写合一对比：**

| | 读写分离 (NPC) | 读写合一 (NEMU) |
|------|------|------|
| 回调数量 | 2 个函数指针 | 1 个函数指针 |
| 优点 | 参数精确，读有返回值，写有 wmask | 只需一个函数 |
| 缺点 | 结构体存两个指针 | 回调内需 switch(is_write) |

NPC 选择读写分离，因为串口只写、RTC 只读，不对称。

### 7.6 IO 空间管理 — `mmio.cpp` 框架层

#### 7.6.1 预分配池 + 页对齐分配

```cpp
#define NPC_IO_SPACE_MAX (64 * 1024)   // 64KB
#define NPC_PAGE_SIZE  4096
#define NPC_PAGE_MASK  (NPC_PAGE_SIZE - 1)

static uint8_t *io_space = NULL;
static uint8_t *p_space = NULL;

void init_map() {
    io_space = (uint8_t*)malloc(NPC_IO_SPACE_MAX);
    assert(io_space);
    p_space = io_space;
}

uint8_t* new_space(int size) {
    uint8_t *p = p_space;
    size = (size + NPC_PAGE_MASK) & ~NPC_PAGE_MASK;  // 页对齐
    p_space += size;
    assert(p_space - io_space <= NPC_IO_SPACE_MAX);
    memset(p, 0, size);
    return p;
}
```

**页对齐意义：** `new_space(8)` → 实际分配 4096 字节。每个设备独占至少一页，支持未来 MMU 页级权限控制。

#### 7.6.2 设备注册 — `add_mmio_device()`

```cpp
void add_mmio_device(const char *name, void *space,
                     uint32_t addr, uint32_t len, uint8_t wmask,
                     mmio_read_cb read, mmio_write_cb write) {
    paddr_t left = addr, right = addr + len - 1;

    // ① 检查不与 PMEM 重叠
    Assert(!(in_pmem(left) || in_pmem(right)), ...);

    // ② 检查不与已注册设备重叠
    for (int i = 0; i < nr_map; i++)
        if (left <= maps[i].addr_end && right >= maps[i].addr_start)
            report_mmio_overlap(...);  // → panic

    // ③ 写入设备表
    maps[nr_map++] = {.name=name, .space=(uint8_t*)space,
                      .addr_start=left, .addr_end=right,
                      .wmask=wmask, .read=read, .write=write};
}
```

**重叠检测算法：** `[L1,R1]` 与 `[L2,R2]` 重叠 ⇔ `L1 ≤ R2 ∧ R1 ≥ L2`

#### 7.6.3 统一分发逻辑

```
mmio_read(addr):
  for i in 0..nr_map:
    if addr ∈ [maps[i].addr_start, maps[i].addr_end]:
      offset = addr - maps[i].addr_start
      return maps[i].read ? maps[i].read(...) : mmio_space_read(...)
  Assert panic

mmio_write(addr, data, wmask):
  for i in 0..nr_map:
    if addr ∈ [maps[i].addr_start, maps[i].addr_end]:
      offset = addr - maps[i].addr_start
      mmio_space_write(...)           // ① 先写 space
      if (maps[i].write) maps[i].write(...)  // ② 再调回调
      return
  Assert panic
```

> **关键顺序：先 space_write，再回调。** 和 NEMU 的 `host_write → invoke_callback` 一致，回调体可从 `dev->space[offset]` 读刚写入的数据。

#### 7.6.4 Space 读写辅助函数

```cpp
uint32_t mmio_space_read(MMIODevice *dev, uint32_t offset) {
    uint32_t val = 0;
    for (int i = 0; i < 4; i++)
        val |= (uint32_t)dev->space[offset + i] << (i * 8);
    return val;
}

void mmio_space_write(MMIODevice *dev, uint32_t offset,
                      uint32_t data, uint8_t wmask) {
    for (int i = 0; i < 4; i++)
        if (wmask & (1 << i))
            dev->space[offset + i] = (data >> (i * 8)) & 0xFF;
}
```

**wmask 逐字节写：** CPU 执行 `sb t0, 0x0(串口地址)` 时，`wmask=0b0001`，只有 `space[0]` 被写入，其余跳过。

#### 7.6.5 启动注册 — `init_mmio()`

```cpp
void init_mmio() {
    init_map();
    add_mmio_device("serial", new_space(8), CONFIG_SERIAL_MMIO, 8,
                    0x1, NULL, serial_write_handler);  // 只写
    add_mmio_device("rtc", new_space(8), CONFIG_RTC_MMIO, 8,
                    0xF, rtc_read_handler, NULL);      // 只读
}
```

### 7.7 调用链路 — 启动到运行时

```
启动时:
  pmem_init() → init_mmio() → init_map() + add_mmio_device ×2 → pmem_load_bin()

运行时 (每周期):
  single_cycle() → top->clk=1
    if_stage.v:   instr = dpi_mem_read(pc, 0)         ← is_load=0，不走 MMIO
    mem_stage.v:  rdata = dpi_mem_read(addr, mem_read) ← is_load=1 → mmio_read
    mem_stage.v:  dpi_mem_write(addr, data, wmask)     ← mmio_write
```

### 7.8 串口设备 — `serial.cpp`

#### 7.8.1 8250 UART 寄存器布局

| 偏移 | 寄存器 | 读写 | NPC 实现 |
|------|--------|------|----------|
| 0 | RBR / THR | 读/写 | ✅ 写输出字符; 读预留键盘 |
| 5 | LSR | 只读 | ⚠️ 预留键盘状态查询 |
| 1~4,6~7 | 其他寄存器 | — | 忽略 |

`len=8` 预留完整 8250 空间，方便后续扩展。

#### 7.8.2 写回调

```cpp
void serial_write_handler(MMIODevice *dev, uint32_t offset,
                          uint32_t data, uint8_t wmask) {
    switch (offset) {
        case 0:
            if (wmask & 0x1)
                npc_serial_putc(dev->space[0]);  // 从 space 取刚写入的字节
            break;
        default: break;  // 静默忽略
    }
}
```

#### 7.8.3 数据流：`putch('A')` → 终端

```
AM putch('A') → sb 0xa00003f8 → DPI-C dpi_mem_write(0xa00003f8, ..., 0x1)
  → mmio_write → mmio_space_write (space[0]=0x41)
  → serial_write_handler → npc_serial_putc(dev->space[0])
  → fputc('A', stderr) → 终端显示 'A'
```

### 7.9 RTC 时钟设备 — `rtc.cpp`

#### 7.9.1 两条独立路径

```
路径 A (被动应答):                路径 B (主动推送):
host_time_us()                    init_timer_alarm()
  ↓ CPU load 读 0xa0000048         ↓ SIGVTALRM 每 16.7ms
rtc_read_handler()                alarm_handler()
  ↓ 返回微秒时间                   ↓ alarm_fired = 1
软件知道"现在几点"                 interrupt_check()
                                   ↓ MIP.MTIP = 1
                                  CPU 收到时钟中断
```

#### 7.9.2 RTC 读回调

```cpp
static uint64_t rtc_latched_us = 0;

uint32_t rtc_read_handler(MMIODevice *dev, uint32_t offset) {
    switch (offset) {
        case 0:  // 低 32 位
            if (rtc_latched_us == 0) rtc_latched_us = host_time_us();
            return (uint32_t)(rtc_latched_us & 0xFFFFFFFF);
        case 4:  // 高 32 位 — 每次重新锁存，保证时间推进
            rtc_latched_us = host_time_us();
            return (uint32_t)(rtc_latched_us >> 32);
        default: return 0;
    }
}
```

**锁存机制：** 防止 64 位时间撕裂。`case 4` 每次重新锁存确保 AM 读出的高低 32 位来自同一快照且能随周期推进。

#### 7.9.3 时钟中断注入链路

```
init_timer_alarm() → setitimer(SIGVTALRM, 16.7ms)
  每 16.7ms: alarm_handler() → alarm_fired = 1
  每周期: interrupt_check() → if(alarm_fired) { MIP.MTIP=1; alarm_fired=0; }
  isa_query_intr() → MIP.MTIP & MIE.MTIE → 返回 0x80000007
  single_cycle() → top->interrupt_valid=1 → RTL 下周期 trap
```

> `alarm_handler` 只设标志的原因是信号处理器运行在异步上下文，printf/锁/malloc 不安全。

### 7.10 `memory.cpp` — DPI-C 入口改动

```cpp
// 改前: switch(paddr) { case SERIAL... case RTC... }
// 改后:
int dpi_mem_read(int addr, int is_load) {
    if (in_pmem(paddr)) return (int)pmem_read(paddr & ~3U, 4);
    if (!is_load) return 0;
    mmio_accessed = true;
    return mmio_read(paddr);  // 一行委托
}

void dpi_mem_write(int addr, int data, int wmask) {
    if (in_pmem(paddr)) { pmem_write(...); return; }
    mmio_accessed = true;
    mmio_write(paddr, data, (uint8_t)wmask);
}
```

**`is_load` 门控：**

| 调用者 | is_load | MMIO 副作用 |
|--------|:---:|------|
| if_stage (取指) | 0 | ❌ 不触发 RTC 锁存 |
| mem_stage (load) | 1 | ✅ 正常交互 |

### 7.11 启动初始化 — `monitor.cpp`

```cpp
void monitor_init(...) {
    pmem_init();                 // ① 清空 128MB PMEM
    init_mmio();                 // ② init_map + 设备注册
    pmem_load_bin(img_file);     // ③ 加载 .bin
    init_timer_alarm();          // ④ 启动定时器中断源
}
```

### 7.12 mmio_accessed 与 difftest

| 事件 | mmio_accessed | difftest 行为 |
|------|:---:|------|
| 正常指令 | 0 | compare |
| load RTC / store 串口 | 1 | skip compare → sync |
| 定时器中断 | 0 | skip compare → sync (通过 difftest_sync_needed) |

### 7.13 in_pmem() — 无符号回绕技巧

```cpp
static inline bool in_pmem(uint32_t addr) {
    return addr - PMEM_BASE < PMEM_SIZE;
}
```

`addr < PMEM_BASE` 时 `addr - PMEM_BASE` 回绕到巨大正值 → 必然 ≥ `PMEM_SIZE` → false。只有 `[PMEM_BASE, PMEM_BASE+PMEM_SIZE)` 内的地址，差值才 < `PMEM_SIZE`。一次比较顶两次。

### 7.14 Bug 修复记录

| # | 文件 | 问题 | 修复 |
|---|------|------|------|
| 1 | mmio.cpp | `addr < addr_end` 应 `<=` | 改 `<=` |
| 2 | mmio.cpp | `mmio_write` 命中后缺 `return` | 加 `return` |
| 3 | mmio.cpp | `init_mmio` 缺 `init_map()` | 加 `init_map()` |
| 4 | mmio.cpp | `%08` 缺 `x` | 改 `%08x` |
| 5 | npc.h | `#include <memory.h>` 在 `typedef paddr_t` 之前 | 移 include 到 typedef 之后 |
| 6 | device.h | 用 `MMIODevice*` 未 include `mmio.h` | 加 `#include <mmio.h>` |
| 7 | rtc.cpp | switch 后缺 `return 0` | 加 `return 0` |
| 8 | Kconfig | `config  Device Configuration` 有空格 | 改 `config DEVICE` |

### 7.15 源码索引

| 组件 | 文件 |
|------|------|
| 设备配置 | `npc/Kconfig` L120-175 |
| 自动生成宏 | `npc/include/generated/autoconf.h` |
| 结构体 + 回调类型 | `npc/include/mmio.h` |
| 回调声明 | `npc/include/device.h` |
| PMEM + in_pmem | `npc/include/memory.h` |
| DPI-C 入口 | `npc/csrc/memory/memory.cpp` |
| IO 空间池 + 分发 | `npc/csrc/device/mmio.cpp` |
| 串口回调 | `npc/csrc/device/serial.cpp` |
| RTC 回调 + 中断源 | `npc/csrc/device/rtc.cpp` |
| 时基中断注入 | `npc/csrc/monitor/interrupt.cpp` |
| 启动初始化 | `npc/csrc/monitor/monitor.cpp` |

### 7.16 扩展指南 — 新增外设（三步）

```
① Kconfig: 加 HAS_xxx + xxx_MMIO 配置项 → make menuconfig
② device/xxx.cpp: 实现 read/write 回调函数
③ mmio.cpp init_mmio(): 加 add_mmio_device() 注册
```

无需修改 `memory.cpp`、`main.cpp`、RTL——统一分发架构的核心优势。

### 7.17 设计原则总结

| # | 原则 | 说明 |
|:--:|------|------|
| 1 | **状态与行为分离** | space_read/write 管数据，回调管副作用 |
| 2 | **先写 space，再回调** | 回调从 space 读数据，与 NEMU 一致 |
| 3 | **地址可配置** | Kconfig 管理 MMIO 地址，编译期检查重叠 |
| 4 | **读写回调分离** | 适合 RTC 只读、串口只写场景 |
| 5 | **is_load 门控** | 只有 load 指令触发 MMIO 读副作用 |
| 6 | **mmio_accessed 标记** | MMIO → difftest skip compare + sync |
| 7 | **页对齐分配** | IO 空间按 4KB 切分，支持未来 MMU 权限控制 |
| 8 | **新增设备零侵入** | 只加 Kconfig + 回调 + 注册，不改框架代码 |
