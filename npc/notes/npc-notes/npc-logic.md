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
  - [4.7 Skip Difftest 设计（MMIO/中断周期跳过）](#47-skip-difftest-设计mmio中断周期跳过)
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

### 4.7 Skip Difftest 设计（MMIO/中断周期跳过）

> 核心矛盾：NPC 有 MMIO 设备和中断，NEMU 没有。MMIO 访存会导致 NEMU 访问越界崩溃，中断会导致 NEMU PC 不同步。

#### 4.7.1 问题分层

| 场景 | NEMU 能执行吗？ | NEMU 知道跳转吗？ | 处理方式 |
|------|:---:|:---:|------|
| MMIO 访存指令（读 RTC / 写 UART） | ❌ 崩溃 | 不需要（下条 PC+4） | 强制同步 NPC→NEMU，跳过对比 |
| 中断触发周期 | ✅ 能执行普通指令 | ❌ 不知道要跳 mtvec | 强制同步 NPC→NEMU，跳过对比 |
| 中断处理程序内访问 MMIO | ❌ 崩溃 | N/A | 强制同步 NPC→NEMU，跳过对比 |

#### 4.7.2 旧方案 Bug：ref_difftest_exec 被跳过

```cpp
// 旧方案（错误）：MMIO 周期跳过整个 difftest_step
if (diff_so_file && !pmem_mmio_accessed()) {
    difftest_step(top, cycle);   // ref_difftest_exec(1) 在里面！
}
// → NEMU PC 停滞，几个 MMIO 周期后 NPC 领先 NEMU 数条指令，PC 永远偏移
```

#### 4.7.3 当前方案：在 single_cycle() 分路径处理

```cpp
void single_cycle(){
    interrupt_check();
    uint32_t npc->pc = npc_pc(top, 0, READ);
    uint32_t this_inst = top->instr;

    bool has_interrupt = top->interrupt_valid;  // ← 保存中断状态

    top->clk = 1; top->eval();
    top->interrupt_valid = 0;
    top->interrupt_cause = 0;
    halt_check();

#ifdef CONFIG_DIFFTEST
    if (diff_so_file){
        if (pmem_mmio_accessed() || has_interrupt){
            // MMIO/中断周期：NEMU 不能自己执行
            // → 读 NPC 完整状态 (PC+GPR+CSR)，灌给 NEMU，跳过对比
            npc_state_data(top);
            ref_difftest_regcpy(&npc_s, DIFFTEST_TO_REF);
        } else{
            // 正常周期：NEMU 自己跑，然后对比
            difftest_step(top, cycle);  // exec(1) + regcpy(TO_DUT) + compare
        }
    }
#endif
    // ...
}
```

#### 4.7.4 MMIO 访问检测机制

```cpp
// memory.cpp — 静态标志位
static bool mmio_accessed = false;

// DPI-C 读写路径中打标
int dpi_mem_read(int addr, int is_load) {
    if (物理内存) return ...;
    if (!is_load) return 0;   // 取指不打标
    mmio_accessed = true;     // ← data load 打标
    // ... MMIO read ...
}
void dpi_mem_write(int addr, int data, int wmask) {
    if (物理内存) return;
    mmio_accessed = true;     // ← MMIO write 打标
    // ... MMIO write ...
}

// 读取并自动清零
bool pmem_mmio_accessed() {
    bool v = mmio_accessed;
    mmio_accessed = false;    // 读取后清零，下周期重新检测
    return v;
}
```

#### 4.7.5 与 Spike 做 REF 的对比

| | NEMU 做 REF | Spike 做 REF |
|---|---|---|
| 5 个函数 | 需要自己在 `ref.c` 中实现 | 已在 `spike-diff/difftest.cc` 中实现 |
| 编译方式 | `nemu/` 下 `make SHARE=1` | `cd tools/spike-diff && make` |
| NEMU 代码改动 | 改动 `ref.c` 4 个函数 | 不动 NEMU 代码 |
| NPC 侧代码 | 完全一样（都是 `dlopen` + 5 个函数指针） | 完全一样 |
| 推荐度 | 教学理解用 | 快速验证用 |

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

