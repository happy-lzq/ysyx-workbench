# NPC 单周期 RV32I 设计笔记

> 按开发阶段组织的完整设计记录。
> 打开方式：`Ctrl+Shift+V` 预览，点击目录链接跳转。

---

## 目录

- [一、阶段 0：基础设施（独立模块）](#一阶段-0基础设施独立模块)
  - [1.1 总览 —— 全部 8 模块 + C++ 环境的数据流关系](#11-总览--全部-8-模块--c-环境的数据流关系)
    - [1.1.1 模块分类](#111-模块分类)
    - [1.1.2 核心数据流全景](#112-核心数据流全景8-模块互联)
    - [1.1.3 六条关键数据流路径](#113-六条关键数据流路径)
    - [1.1.4 模块间信号流向矩阵](#114-模块间信号流向矩阵)
  - [1.2 regfile — 寄存器文件](#12-regfile--寄存器文件)
    - [1.2.1 基本信息 & 端口表](#121-基本信息--端口表)
    - [1.2.2 设计要点（Q1–Q3）](#122-设计要点q1q3)
    - [1.2.3 审查记录](#123-审查记录)
  - [1.3 alu — 算术逻辑单元](#13-alu--算术逻辑单元)
    - [1.3.1 基本信息 & 端口表](#131-基本信息--端口表)
    - [1.3.2 ALU 操作码定义](#132-alu-操作码定义)
    - [1.3.3 设计要点（Q1–Q3）](#133-设计要点q1q3)
    - [1.3.4 审查记录](#134-审查记录)
  - [1.4 imm_gen — 立即数生成](#14-imm_gen--立即数生成)
    - [1.4.1 基本信息 & 端口表](#141-基本信息--端口表)
    - [1.4.2 五种立即数格式](#142-五种立即数格式)
    - [1.4.3 opcode → 格式映射](#143-opcode--格式映射)
    - [1.4.4 设计要点（Q1–Q3）](#144-设计要点q1q3)
    - [1.4.5 审查记录](#145-审查记录)
  - [1.5 br_cond — 分支条件判断](#15-br_cond--分支条件判断)
    - [1.5.1 基本信息 & 端口表](#151-基本信息--端口表)
    - [1.5.2 br_type 编码](#152-br_type-编码)
    - [1.5.3 设计要点（Q1–Q2）](#153-设计要点q1q2)
    - [1.5.4 审查记录](#154-审查记录)
  - [1.6 lsu — 加载/存储单元](#16-lsu--加载存储单元)
    - [1.6.1 基本信息 & 端口表](#161-基本信息--端口表)
    - [1.6.2 lsu_type 编码](#162-lsu_type-编码)
    - [1.6.3 访存指令与地址对齐速查表](#163-访存指令与地址对齐速查表)
    - [1.6.4 内存字节布局（小端序）](#164-内存字节布局小端序)
    - [1.6.5 设计要点（Q1–Q5）](#165-设计要点q1q5)
    - [1.6.6 审查记录](#166-审查记录)
  - [1.7 通用设计原则](#17-通用设计原则)
- [二、阶段 1：单周期核心（stage 集成 + core_top）](#二阶段-1单周期核心stage-集成--core_top)
  - [2.1 架构总览：完整互联与数据流](#21-架构总览5-stage--regfile--csr--halt--完整互联)
    - [2.1.0 模块互联全景图](#210-模块互联全景图)
    - [2.1.1 模块职责与数据流方向速查](#211-模块职责与数据流方向速查)
    - [2.1.2 控制信号扇出全景](#212-控制信号扇出全景)
    - [2.1.3 中断注入到 PC 跳转的完整链路](#213-中断注入到-pc-跳转的完整链路)
    - [2.1.4 CSR 指令完整数据流（csrrw 为例）](#214-csr-指令完整数据流csrrw-为例)
  - [2.2 control — 主控单元](#22-control--主控单元)
    - [2.2.1 基本信息 & 端口表](#221-基本信息--端口表)
    - [2.2.2 默认值设计](#222-默认值设计)
    - [2.2.3 指令分发总表](#223-指令分发总表按指令类型逐信号列出)
    - [2.2.4 审查记录](#224-审查记录)
    - [2.2.5 各信号完整编码速查](#225-各信号完整编码速查)
    - [2.2.6 判断依据的本质：ISA → 控制信号推演](#226-判断依据的本质isa--控制信号的推演方法)
  - [2.3 if_stage — 取指阶段](#23-if_stage--取指阶段)
    - [2.3.1 基本信息 & 端口表](#231-基本信息--端口表)
    - [2.3.2 PC 选择逻辑](#232-pc-选择逻辑)
    - [2.3.3 指令存储器映射](#233-指令存储器映射)
    - [2.3.4 审查记录](#234-审查记录)
  - [2.4 id_stage — 译码阶段](#24-id_stage--译码阶段)
    - [2.4.1 基本信息 & 端口表](#241-基本信息--端口表)
    - [2.4.2 内部结构](#242-内部结构)
    - [2.4.3 审查记录](#243-审查记录)
  - [2.5 ex_stage — 执行阶段](#25-ex_stage--执行阶段)
    - [2.5.1 基本信息 & 端口表](#251-基本信息--端口表)
    - [2.5.2 ALU 源选择 MUX](#252-alu-源选择-mux)
    - [2.5.3 审查记录](#253-审查记录)
  - [2.6 mem_stage — 访存阶段](#26-mem_stage--访存阶段)
    - [2.6.1 基本信息 & 端口表](#261-基本信息--端口表)
    - [2.6.2 统一内存 + LSU 流程](#262-统一内存--lsu-流程)
    - [2.6.3 审查记录](#263-审查记录)
  - [2.7 wb_stage — 写回阶段](#27-wb_stage--写回阶段)
    - [2.7.1 基本信息 & 端口表](#271-基本信息--端口表)
    - [2.7.2 审查记录](#272-审查记录)
  - [2.8 core_top — 顶层连线](#28-core_top--顶层连线)
    - [2.8.1 基本信息](#281-基本信息)
    - [2.8.2 信号总线表](#282-信号总线表)
    - [2.8.3 跨阶段关键 assign](#283-跨阶段关键-assign)
    - [2.8.4 审查记录](#284-审查记录)
  - [2.9 PC 计算来源与路由规则](#29-pc-计算来源与路由规则)
  - [2.10 控制信号安全网：阀门模型](#210-控制信号安全网阀门模型)
  - [2.11 架构分布数据通路总览](#211-架构分布数据通路总览)
    - [2.11.1 if_stage 模块 I/O 表](#2111-if_stage取指-模块-io-表)
    - [2.11.2 id_stage 模块 I/O 表](#2112-id_stage译码-模块-io-表)
    - [2.11.3 ex_stage 模块 I/O 表](#2113-ex_stage执行-模块-io-表)
    - [2.11.4 mem_stage 模块 I/O 表](#2114-mem_stage访存-模块-io-表)
    - [2.11.5 wb_stage 模块 I/O 表](#2115-wb_stage写回-模块-io-表)
    - [2.11.6 regfile 模块 I/O 表](#2116-regfile寄存器文件独立-模块-io-表)
    - [2.11.7 全局信号总线全览](#2117-全局信号总线全览)
    - [2.11.8 跨阶段 assign 别名](#2118-跨阶段-assign-别名)
    - [2.11.9 时序元件分布](#2119-时序元件分布)
    - [2.11.10 数据通路逐指令追踪示例](#21110-数据通路逐指令追踪示例)
- [三、阶段 2：扩展指令（RV32I 完整指令集 ✅）](#三阶段-2扩展指令rv32i-完整指令集--已实现)
  - [3.1 RV32I 指令全表（37 条）](#31-rv32i-指令全表37-条)
    - [3.1.1 R 型指令](#311-r-型指令opcode--0110011--10-条)
    - [3.1.2 I 型算术指令](#312-i-型算术指令opcode--0010011--9-条)
    - [3.1.3 I 型 Load 指令](#313-i-型-load-指令opcode--0000011--5-条)
    - [3.1.4 S 型 Store 指令](#314-s-型-store-指令opcode--0100011--3-条)
    - [3.1.5 B 型分支指令](#315-b-型分支指令opcode--1100011--6-条)
    - [3.1.6 U 型指令](#316-u-型指令opcode--01101110010111--2-条)
    - [3.1.7 J 型指令](#317-j-型指令opcode--1101111--1-条)
    - [3.1.8 I 型 jalr 指令](#318-i-型-jalr-指令opcode--1100111--1-条)
  - [3.2 指令与立即数格式对照](#32-指令与立即数格式对照)
  - [3.3 设计要点](#33-阶段-2-设计要点)
- [四、阶段 3：特权与异常（CSR + trap + 中断 ✅）](#四阶段-3特权与异常csr--trap--中断--已实现)
  - [4.1 csr — 控制状态寄存器模块](#41-csr--控制状态寄存器模块)
    - [4.1.1 基本信息 & 端口表](#411-基本信息--端口表)
    - [4.1.2 CSR 寄存器清单](#412-csr-寄存器清单)
    - [4.1.3 CSR 关键位域](#413-csr-关键位域)
    - [4.1.4 Trap 进入流程](#414-trap-进入流程硬件自动)
    - [4.1.5 MRET 返回流程](#415-mret-返回流程硬件自动)
    - [4.1.6 trap_target 输出逻辑](#416-trap_target-输出逻辑)
  - [4.2 CSR 指令集（6 条）](#42-csr-指令集6-条)
    - [4.2.1 控制信号](#421-控制信号)
    - [4.2.2 指令表](#422-指令表)
    - [4.2.3 ex_stage 中的 CSR 写数据生成](#423-ex_stage-中的-csr-写数据生成)
  - [4.3 ecall / ebreak / mret](#43-ecall--ebreak--mret)
    - [4.3.1 系统指令分发](#431-controlv-中的系统指令分发opcode1110011-funct3000)
    - [4.3.2 ebreak 的 halt 处理](#432-ebreak-的-halt-处理)
  - [4.4 halt — 仿真停止模块](#44-halt--仿真停止模块)
  - [4.5 中断系统](#45-中断系统)
    - [4.5.1 中断类型](#451-中断类型)
    - [4.5.2 C++ 侧中断处理流程](#452-c-侧中断处理流程interruptcpp)
    - [4.5.3 RTL 侧中断注入](#453-rtl-侧中断注入core_top)
    - [4.5.4 中断处理流程图](#454-中断处理流程图)
  - [4.6 core_top 模块更新](#46-core_top-模块更新--中断支持)
  - [4.7 wb_stage 更新 — 四选一 MUX](#47-wb_stage-更新--四选一-mux)
  - [4.8 regfile 更新 — rs_a0 输出](#48-regfile-更新--rs_a0-输出)
  - [4.9 control.v 新增输出信号汇总](#49-controlv-新增输出信号汇总)
  - [4.10 阶段 3 审查记录](#410-阶段-3-审查记录)
- [五、C++ 仿真环境（csrc 全景）](#五c-仿真环境csrc-全景)
  - [5.1 文件与模块映射](#51-文件与模块映射)
  - [5.2 DPI-C 接口](#52-dpi-c-接口)
  - [5.3 内存映射](#53-内存映射)
  - [5.4 single_cycle 执行流程](#54-single_cycle-执行流程)
  - [5.5 Verilator RTL 访问器](#55-verilator-rtl-访问器npch)
- [附录 C：全部模块文件一览](#附录-c全部模块文件一览)
- [附录 D：信号编码速查总表](#附录-d信号编码速查总表)
- [附录：参考资料](#附录参考资料)

---

## 一、阶段 0：基础设施（独立模块）

### 1.1 总览 —— 全部 8 模块 + C++ 环境的数据流关系

> NPC 按模块职能分为三层：**RTL 核心层**（8 个 Verilog 模块）、**C++ 仿真环境层**（内存/外设/中断/difftest）、**DPI-C 边界层**（统一内存接口）。

#### 1.1.1 模块分类

| 层 | 模块 | 类型 | 职能 |
|:--:|------|:--:|------|
| **数据通路** | if_stage, id_stage, ex_stage, mem_stage, wb_stage | 五级流水 | 指令处理主链路 |
| **状态存储** | regfile, csr | 时序 | CPU 架构状态（GPR + CSR） |
| **辅助** | halt | 时序 | 仿真停止检测 |
| **C++ 环境** | memory.cpp, interrupt.cpp, main.cpp, difftest.cpp, serial.cpp | C++ | 内存模拟、外设、中断注入、差分测试 |

#### 1.1.2 核心数据流全景（8 模块互联）

```
                          ┌─────────────────────────┐
                          │     C++ 仿真环境          │
                          │  PMEM(128MB) + MMIO      │
                          │  interrupt.cpp 定时器    │
                          └──┬──────┬──────────┬─────┘
                             │      │          │
                    DPI-C    │      │          │ DPI-C
                 取指/读写   │      │ interrupt│ 读写
                             ▼      │ _valid   ▼
  ┌──────────────────────────────────────────────────────────────┐
  │                       core_top (RTL)                         │
  │                                                              │
  │  ┌─────────┐     ┌──────────┐     ┌──────────┐              │
  │  │if_stage │────→│ id_stage │────→│ ex_stage │              │
  │  │ PC+取指  │     │ 译码+控制 │     │ ALU+分支  │              │
  │  └──┬──▲───┘     └──┬───┬───┘     └──┬───┬───┘              │
  │     │  │            │   │            │   │                  │
  │     │  │   trap_    │   │ 控制信号   │   │  alu_result       │
  │     │  │   target   │   │ 全线广播   │   │  br_taken         │
  │     │  │            │   │            │   │  csr_wdata        │
  │     │  │         ┌──┘   └──────────┐ │   │                  │
  │     │  │         │                 │ │   │                  │
  │     │  │    ┌────┴─────┐     ┌─────┴─┴───┴────┐             │
  │     │  │    │ regfile  │     │    ex_stage    │             │
  │     │  │    │ 32×32 GPR│     │  内部: alu    │             │
  │     │  │    │ + rs_a0  │     │  br_cond      │             │
  │     │  │    └────┬─────┘     │  csr_wdata生成 │             │
  │     │  │         │           └────────┬───────┘             │
  │     │  │    rdata│                    │                     │
  │     │  │         │           ┌────────┴───────┐             │
  │     │  │         │           │   mem_stage    │             │
  │     │  │         │           │ DPI-C 读写     │             │
  │     │  │         │           │ + lsu 格式翻译  │             │
  │     │  │         │           └────────┬───────┘             │
  │     │  │         │                    │ mem_rdata           │
  │     │  │         │           ┌────────┴───────┐             │
  │     │  │         │           │   wb_stage     │             │
  │     │  │         │           │ 4选1 MUX 写回   │             │
  │     │  │         │           └───────┬────────┘             │
  │     │  │         │                   │ reg_wdata            │
  │     │  │         └───────────────────┘ (→ regfile 写口)     │
  │     │  │                                                     │
  │     │  │              ┌──────────┐                          │
  │     │  └──────────────│ csr.v    │←── csr_wdata(EX)         │
  │     │      trap_target│ 8 CSR    │←── csr_addr(ID)         │
  │     │                 │ + mcycle │←── csr_read/csr_write    │
  │     │                 │ + trap/  │──→ csr_rdata→EX→WB      │
  │     │                 │   mret   │──→ trap_target→IF       │
  │     │                 └──────────┘                          │
  │     │                                                        │
  │     │    ┌──────────┐        ┌─────────────────┐            │
  │     │    │ halt.v   │←───────│ 有效控制仲裁层    │            │
  │     │    │ ebreak   │        │ interrupt_valid │            │
  │     │    │ →halt_ret│        │ 时 kill 所有     │            │
  │     │    └──────────┘        │ *_eff 副作用     │            │
  │     │                        └─────────────────┘            │
  └─────┴────────────────────────────────────────────────────────┘
```

#### 1.1.3 六条关键数据流路径

| # | 路径 | 数据流向 | 触发条件 |
|:--:|------|------|------|
| ① | **常规指令** | IF→ID→EX→MEM→WB→regfile | opcode ≠ 系统指令 |
| ② | **Load 指令** | IF→ID→EX(算地址)→MEM(DPI-C读)→LSU(扩展)→WB→regfile | opcode=0000011 |
| ③ | **Store 指令** | IF→ID→EX(算地址)→MEM(LSU格式化+DPI-C写) | opcode=0100011 |
| ④ | **CSR 读** | IF→ID(control→csr_read=1)→csr→csr_rdata→EX(透传)→WB(MUX=11)→regfile | CSR 指令 |
| ⑤ | **CSR 写** | IF→ID(control→csr_write=1)→EX(生成csr_wdata)→csr | CSR 指令 |
| ⑥ | **中断/trap** | C++(interrupt_valid)→core_top仲裁层(kill所有*_eff)→csr(保存mepc/mcause)→IF(pc_next=trap_target=mtvec) | 中断/ecall |
| ⑦ | **mret 返回** | ID(control→mret=1)→csr(更新mstatus)→IF(pc_next=trap_target=mepc) | mret 指令 |
| ⑧ | **ebreak 停止** | ID(control→is_ebreak=1)→halt(保存halt_pc/halt_ret)→C++检测→NPC_END | ebreak 指令 |

#### 1.1.4 模块间信号流向矩阵

> 行 = 驱动者（输出端口在哪个模块），列 = 消费者（输入端口接哪个模块）。"×" 表示存在连接。

| 驱动者 \ 消费者 | if_stage | id_stage | ex_stage | mem_stage | wb_stage | regfile | csr | halt | C++/top |
|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
| **if_stage** | — | instr,pc | pc | — | pc_plus4 | — | — | — | instr |
| **id_stage** | pc_sel,imm_jal,imm_br | — | 全部控制信号,imm_out | mem_read,mem_write,lsu_type | reg_wdata_src | rs1_addr,rs2_addr,rd_addr,reg_write | csr_addr,csr_read,csr_write | — | — |
| **ex_stage** | br_taken,jump_jalr | — | — | alu_result | alu_result | — | csr_wdata | — | — |
| **mem_stage** | — | — | — | — | mem_rdata | — | — | — | — |
| **wb_stage** | — | — | — | — | — | rd_wdata | — | — | — |
| **regfile** | — | rs1_rdata,rs2_rdata | rs1_rdata,rs2_rdata | rs2_rdata | — | — | — | rs_a0 | — |
| **csr** | trap_target | — | csr_rdata | — | csr_rdata | — | — | — | — |
| **halt** | — | — | — | — | — | — | — | — | halt,halt_pc,halt_ret |
| **C++/top** | interrupt_valid| — | — | — | — | — | — | — | — |

### 1.2 regfile — 寄存器文件

| 属性 | 值 |
|------|-----|
| 文件 | `npc/vsrc/regfile.v` |
| 类型 | 组合读 + 时序写 |
| 顶层位置 | core_top 独立模块（非 id_stage 内部） |

#### 1.2.1 基本信息 & 端口表

| 方向 | 信号 | 位宽 | 说明 |
|:--:|------|:--:|------|
| I | `clk` | 1 | 时钟（仅写口用） |
| I | `rs1_addr` | 5 | 读口1地址（来自 id_stage） |
| I | `rs2_addr` | 5 | 读口2地址（来自 id_stage） |
| I | `rd_addr` | 5 | 写口地址（来自 id_stage） |
| I | `rd_wdata` | 32 | 写口数据（来自 wb_stage） |
| I | `reg_write` | 1 | 写使能（来自 control.v, 直连） |
| O | `rs1_rdata` | 32 | 读口1数据 |
| O | `rs2_rdata` | 32 | 读口2数据 |

#### 1.2.2 设计要点（Q1–Q3）

**Q1: 为什么读是组合、写是时序？**
- 读组合：CPU 在同一周期内译码时需要立刻拿到寄存器值
- 写时序：寄存器文件是 CPU 状态存储器，状态只在时钟沿更新。用 `<=` 非阻塞赋值
- 类比 D 触发器：Q 端随时可读，D 端在 clk 上升沿采样

**Q2: x0 为什么不能直接置零？**
- `reg [31:0] rf [0:31]` 数组无法对单个元素钳位
- 正确做法：① 写口封锁 `if (rd_addr != 0)`；② 读口 MUX 旁路 `(rs_addr == 0) ? 0 : rf[rs_addr]`
- `initial rf[0] = 0` 不可综合

**Q3: 为什么放在 core_top 而非 id_stage 内部？**
- 流水线升级时 regfile 不变，只需加 pipe_regs + forward + hazard
- ID 读、WB 写分离，层次清晰
- 是经典 5 级流水线标准拓扑

#### 1.2.3 审查记录

| 检查项 | 结果 |
|--------|:--:|
| 读写分离 | ✅ |
| x0 硬件旁路 | ✅ |
| `<=` 非阻塞赋值 | ✅ |
| 顶层独立模块 | ✅ |

### 1.3 alu — 算术逻辑单元

| 属性 | 值 |
|------|-----|
| 文件 | `npc/vsrc/alu.v` |
| 类型 | 纯组合逻辑 |

#### 1.3.1 基本信息 & 端口表

| 方向 | 信号 | 位宽 | 说明 |
|:--:|------|:--:|------|
| I | `src1` | 32 | 操作数1 |
| I | `src2` | 32 | 操作数2 |
| I | `alu_op` | 5 | 自定义 ALU 操作码（最高位=0→RV32I, =1→M扩展） |
| O | `result` | 32 | 运算结果 |

#### 1.3.2 ALU 操作码定义

| alu_op[4:0] | 操作 | 服务指令 | 阶段 |
|:----------:|------|---------|:--:|
| `0_0000` | SUB | sub | 1 |
| `0_0001` | ADD | add, addi, lw, sw, jal, jalr, auipc | 1 |
| `0_0010` | SLL | sll, slli | 1 |
| `0_0011` | SLT | slt, slti | 1 |
| `0_0100` | SLTU | sltu, sltiu | 1 |
| `0_0101` | XOR | xor, xori | 1 |
| `0_0110` | OR | or, ori | 1 |
| `0_0111` | AND | and, andi | 1 |
| `0_1000` | SRL | srl, srli | 1 |
| `0_1001` | SRA | sra, srai | 1 |
| `0_1010` | LUI | lui | 1 |
| `1_0000~1_0111` | M扩展 | mul, div, rem... | 4 |

#### 1.3.3 设计要点（Q1–Q3）

**Q1: alu_op 是谁定义的？** RISC-V 只定义 opcode/funct3/funct7，`alu_op` 是自定义中间编码，由 control.v 翻译产生。

**Q2: 为什么 ALU 不加 clk/en？** 纯组合逻辑，加了时钟就变成时序——单周期多等一拍。

**Q3: 关键易错点**

| 操作 | 正确 | 错误 |
|------|------|------|
| 移位量 | `src2[4:0]` | `src2`（可能 >31） |
| SLT | `$signed(src1) < $signed(src2)` | 不加 `$signed`（-1 变最大值） |
| SRA | `$signed(src1) >>>` | `>>`（可能逻辑右移） |
| LUI | `result = src2` | 不需要 `<<12`（imm_gen 已做） |

#### 1.3.4 审查记录

| 检查项 | 结果 |
|--------|:--:|
| 移位取低5位 | ✅ |
| SLT 有 `$signed` | ✅ |
| SRA 用 `>>>` | ✅ |
| default = 0 | ✅ |
| 5位编码预留M扩展 | ✅ |

### 1.4 imm_gen — 立即数生成

| 属性 | 值 |
|------|-----|
| 文件 | `npc/vsrc/imm_gen.v` |
| 类型 | 纯组合逻辑 |

#### 1.4.1 基本信息 & 端口表

| 方向 | 信号 | 位宽 | 说明 |
|:--:|------|:--:|------|
| I | `instr` | 32 | 原始指令 |
| O | `imm_out` | 32 | 符号扩展立即数 |

#### 1.4.2 五种立即数格式

| 格式 | 指令 bit 位置 | 位宽 | 使用指令 |
|------|-------------|:--:|---------|
| I型 | `instr[31:20]` | 12 | addi, lw, jalr, ecall, csr* |
| S型 | `{instr[31:25], instr[11:7]}` | 12 | sw, sh, sb |
| B型 | `{instr[31],instr[7],instr[30:25],instr[11:8],1'b0}` | 13 | beq, bne... |
| U型 | `{instr[31:12], 12'b0}` | 32 | lui, auipc |
| J型 | `{instr[31],instr[19:12],instr[20],instr[30:21],1'b0}` | 21 | jal |

#### 1.4.3 opcode → 格式映射

| opcode[6:0] | 格式 |
|:----------:|:----:|
| `0010011`, `0000011`, `1100111`, `1110011` | I型 |
| `0100011` | S型 |
| `1100011` | B型 |
| `0110111`, `0010111` | U型 |
| `1101111` | J型 |
| `0110011` | R型→default |

#### 1.4.4 设计要点（Q1–Q3）

**Q1: 10个opcode怎么只对应5种格式？** 立即数提取只看bit位置，与指令语义无关。

**Q2: 立即数一定是有符号扩展？** 是。`sltiu`/`lbu` 中的"无符号"指比较/扩展方式（ALU/LSU负责）。

**Q3: 需要独立字段拆分模块？** 不需要。`instr[a:b]` 是 wire 切片，零逻辑门。

#### 1.4.5 审查记录

| 检查项 | 结果 |
|--------|:--:|
| I/S/B/U/J 拼接 | ✅ |
| 符号扩展位宽 | ✅ |
| `7'b1110011` 加入I型 | ✅ |
| 与NEMU对比一致 | ✅ |

### 1.5 br_cond — 分支条件判断

| 属性 | 值 |
|------|-----|
| 文件 | `npc/vsrc/br_cond.v` |
| 类型 | 纯组合逻辑 |

#### 1.5.1 基本信息 & 端口表

| 方向 | 信号 | 位宽 | 说明 |
|:--:|------|:--:|------|
| I | `rs1_data` | 32 | 操作数1 |
| I | `rs2_data` | 32 | 操作数2 |
| I | `br_type` | 3 | 自定义分支类型编码 |
| O | `br_taken` | 1 | 分支成立=1 |

#### 1.5.2 br_type 编码

| br_type[2:0] | 指令 | 条件 |
|:----------:|------|------|
| `000` | beq | rs1 == rs2 |
| `001` | bne | rs1 != rs2 |
| `010` | blt | signed rs1 < rs2 |
| `011` | bge | signed rs1 >= rs2 |
| `100` | bltu | unsigned rs1 < rs2 |
| `101` | bgeu | unsigned rs1 >= rs2 |
| `110/111` | — | default→0 |

#### 1.5.3 设计要点（Q1–Q2）

**Q1: 为什么输入是 br_type 而非 funct3？** 分层设计：ISA编码只在control.v内部接触，其他模块用自定义统一编码。

**Q2: `000` = beq，非分支指令有问题吗？** 功能上没问题。`pc_sel=00` 时 IF 选 PC+4，br_taken 被忽略。紧凑编码风格。

#### 1.5.4 审查记录

| 检查项 | 结果 |
|--------|:--:|
| 6种比较 | ✅ |
| 有符号用 `$signed` | ✅ |
| default=0 | ✅ |

### 1.6 lsu — 加载/存储单元

| 属性 | 值 |
|------|-----|
| 文件 | `npc/vsrc/lsu.v` |
| 类型 | 纯组合逻辑 |

#### 1.6.1 基本信息 & 端口表

| 方向 | 信号 | 位宽 | 说明 |
|:--:|------|:--:|------|
| I | `mem_addr` | 32 | 访存地址（来自 ALU） |
| I | `mem_rdata_raw` | 32 | 内存返回的原始32位字 |
| I | `mem_wdata_raw` | 32 | 待存储数据（来自 rs2） |
| I | `lsu_type` | 3 | 访存类型编码 |
| I | `mem_read` | 1 | 读门控 |
| I | `mem_write` | 1 | 写门控 |
| O | `mem_rdata` | 32 | 加载结果（扩展后） |
| O | `mem_wdata` | 32 | 格式化后的存储数据 |
| O | `mem_wmask` | 4 | 字节写掩码 |

#### 1.6.2 lsu_type 编码

| lsu_type | 指令 | 方向 | 操作 |
|:------:|------|:--:|------|
| `000` | lw | Load | 整字透传 |
| `001` | lh | Load | 半字→有符号扩展 |
| `010` | lb | Load | 字节→有符号扩展 |
| `011` | lhu | Load | 半字→零扩展 |
| `100` | lbu | Load | 字节→零扩展 |
| `101` | sw | Store | 存字 wmask=1111 |
| `110` | sh | Store | 存半字 wmask=0011/1100 |
| `111` | sb | Store | 存字节 wmask←addr[1:0] |

#### 1.6.3 访存指令与地址对齐速查表

| 指令 | 宽度 | addr[1:0]允许值 | 取哪块 | 扩展 | wmask |
|------|:--:|:---:|------|:--:|:---:|
| lw | 32 | 00 | 全32位 | 无 | — |
| sw | 32 | 00 | 全32位 | — | 1111 |
| lh | 16 | 00,10 | addr[1]→半字 | 有符号 | — |
| lhu | 16 | 00,10 | 同上 | 零扩展 | — |
| sh | 16 | 00,10 | — | — | 0011/1100 |
| lb | 8 | 00,01,10,11 | addr[1:0]→字节 | 有符号 | — |
| lbu | 8 | 00,01,10,11 | 同上 | 零扩展 | — |
| sb | 8 | 00,01,10,11 | — | — | 0001<<addr[1:0] |

#### 1.6.4 内存字节布局（小端序）

```
字地址 N 返回的32位: mem_rdata_raw[31:0]
  N+0        N+1        N+2        N+3
┌─────────┬─────────┬─────────┬─────────┐
│ byte0   │ byte1   │ byte2   │ byte3   │
│[7:0]    │[15:8]   │[23:16]  │[31:24]  │
└─────────┴─────────┴─────────┴─────────┘
  ↑=00       ↑=01      ↑=10      ↑=11
  ←── half0 ──→        ←── half1 ──→
```

#### 1.6.5 设计要点（Q1–Q5）

**Q1: LSU做存储吗？** 不做。data_mem 是真正的存储器。LSU 是格式翻译器。

**Q2: Load路径** `mem_rdata_raw → 字节/半字选择 → 符号/零扩展 → mem_rdata → WB`

**Q3: Store路径** `rdata2 → 字节复制到正确lane → mem_wdata；生成wmask`

**Q4: 为什么Store时复制数据？** data_mem是32位宽总线。半字/字节需要数据在正确位置+wmask选通。

**Q5: 非Store指令wmask必须为0** Load指令不应写内存。MUX有 `is_sb` 显式条件和default=0。增加了 `mem_read`/`mem_write` 输入做双门控保护。

#### 1.6.6 审查记录

| 检查项 | 结果 |
|--------|:--:|
| Load五种扩展 | ✅ |
| Store数据复制 | ✅ |
| sh_mask/sb_mask addr依赖 | ✅ |
| mem_read/mem_write门控 | ✅ |

### 1.7 通用设计原则

| # | 原则 | 适用模块 |
|:--:|------|---------|
| 1 | 组合逻辑用 `assign` 或 `always @(*)`，无 clock | alu, imm_gen, br_cond, lsu, control |
| 2 | 时序逻辑用 `always @(posedge clk)` + `<=` | regfile(写口), if_stage(PC), csr, mem_stage(DPI-C写), halt |
| 3 | 不需要的端口不加 (clk/en/rst) | alu, imm_gen, br_cond, lsu, control |
| 4 | `instr[a:b]` 是 wire 切片，零成本 | id_stage 整体 |
| 5 | RISC-V 编码只在 control.v 内接触 | 其他模块用自定义编码 |
| 6 | `case` 和并行 `assign`+MUX 综合结果等价 | 风格自由 |
| 7 | `default` 设 0，禁止 X 态传播 | 所有 case |
| 8 | 控制信号默认值 = 安全空操作 | reg_write=0, mem_write=0, pc_sel=00, csr_write=0, trap_enter=0 |
| 9 | 统一内存通过 DPI-C 调用 C++，不在 Verilog 内 `$readmemh` | if_stage, mem_stage |

---

## 二、阶段 1：单周期核心（stage 集成 + core_top）

### 2.1 架构总览：5 Stage + regfile + csr + halt —— 完整互联

#### 2.1.0 模块互联全景图

```
                         ┌─────────────────────────────────────────┐
                         │           C++ 仿真环境                    │
                         │  ┌──────────┐  ┌──────────┐             │
                         │  │ PMEM     │  │ interrupt │             │
                         │  │ 128MB    │  │ .cpp      │             │
                         │  │ + MMIO   │  │ timer_tick│             │
                         │  └────┬─────┘  └─────┬─────┘             │
                         └───────┼──────────────┼───────────────────┘
                                 │ DPI-C        │ top-level input
                                 │ r/w          │ interrupt_valid/cause
        ┌────────────────────────┼──────────────┼───────────────────────┐
        │                  core_top (RTL)        │                       │
        │                        │               ▼                       │
        │   ┌────────────────────┼──────────────────────────────────┐   │
        │   │          有效控制仲裁层 (_eff 后缀)                     │   │
        │   │    interrupt_valid 时 kill: mem_read/write,            │   │
        │   │    reg_write, csr_write, mret → 全部置 0              │   │
        │   │    trap_enter_eff = trap_enter | interrupt_valid       │   │
        │   └────┬───────┬───────┬───────┬───────┬──────────────────┘   │
        │        │       │       │       │       │                      │
        │   ┌────┴───┐   │   ┌───┴───┐   │   ┌───┴───┐                  │
        │   │  csr.v │   │   │ halt.v│   │   │regfile│                  │
        │   │ (时序)  │   │   │ (时序) │   │   │ (时序) │                  │
        │   └──┬──▲──┘   │   └───▲───┘   │   └─┬──▲──┘                  │
        │      │  │      │       │       │     │  │                     │
        │      │  │      │       │       │  rdata │ reg_wdata/reg_write │
        │      │  │      │       │       │     │  │                     │
        │      │  │      │       │       │  ┌──┴──┴───────────────┐    │
        │      │  │      │       │       │  │  五级流水主链路       │    │
        │      │  │      │       │       │  │                      │    │
        │ trap_│  │csr_  │  is_ebreak   │  │ ┌──────┐  ┌──────┐   │    │
        │ target  │wdata │       │       │  │ │  IF  │─→│  ID  │   │    │
        │      │  │      │       │       └──┼─┤      │  │      │   │    │
        │      │  │      │       │  rs_a0 ──┘ │ └──┬───┘  └──┬───┘   │    │
        │      │  │      │       └─────────────┘    │        │       │    │
        │      │  │      │                          │   控制信号全线  │    │
        │      │  │      │                     ┌────┴────────┴───┐   │    │
        │      │  │      │                     │     EX          │   │    │
        │      │  │      │                     │  ALU + br_cond  │   │    │
        │      │  │      │                     │  + csr_wdata    │   │    │
        │      │  │      │                     └──┬──────┬───────┘   │    │
        │      │  │      │                        │      │           │    │
        │      │  │      │               alu_result  br_taken        │    │
        │      │  │      │                   ┌────┴──────┐           │    │
        │      │  │      │                   │    MEM    │           │    │
        │      │  │      │                   │ DPI-C+lsu │           │    │
        │      │  │      │                   └─────┬─────┘           │    │
        │      │  │      │                    mem_rdata              │    │
        │      │  │      │                   ┌─────┴─────┐           │    │
        │      │  │      │                   │    WB     │           │    │
        │      │  │      │                   │ 4选1 MUX  │           │    │
        │      │  │      │                   └─────┬─────┘           │    │
        │      │  │      │                         │                 │    │
        │      │  │      └─────────────────────────┘                 │    │
        │      │  │           (halt_ret from regfile.rs_a0)          │    │
        │      │  │                                                  │    │
        └──────┼──┼──────────────────────────────────────────────────┘    │
               │  │                                                       │
         trap_target│                                                      │
        ┌──────────┴──────────────────────────────────────────────────┐   │
        │  IF 阶段 PC 选择 (最高优先级):                                │   │
        │  pc_next = (trap_enter|mret) ? trap_target : ...            │   │
        │  trap_target = trap_enter ? mtvec : mret ? mepc : 0         │   │
        └─────────────────────────────────────────────────────────────┘   │
                                                                          │
        ┌──────────────────────────────────────────────────────────────┐  │
        │  EX 阶段 CSR 写数据通路:                                       │  │
        │  csr_src = csr_imm ? rs1_rdata : csr_zimm                     │  │
        │  csr_wdata = csr_op=00?csr_src : csr_op=01?csr_rdata|csr_src  │  │
        │                                    : csr_op=10?csr_rdata&~csr_src│  │
        │  → 送入 csr.v 的 csr_wdata 端口                                 │  │
        └──────────────────────────────────────────────────────────────┘  │
                                                                          │
        ┌──────────────────────────────────────────────────────────────┐  │
        │  WB 阶段四选一写回:                                            │  │
        │  reg_wdata = reg_wdata_src==00 ? alu_result :                 │  │
        │              reg_wdata_src==01 ? mem_rdata  :                 │  │
        │              reg_wdata_src==10 ? pc_plus4   : csr_rdata       │  │
        │  → regfile.rd_wdata (posedge clk 写入 rf[rd])                  │  │
        └──────────────────────────────────────────────────────────────┘  │
```

#### 2.1.1 模块职责与数据流方向速查

| 模块 | 输入来自 | 输出去向 | 核心数据流角色 |
|------|---------|---------|-------------|
| **if_stage** | control(pc_sel), ex(alu_result/br_taken), id(imm), csr(trap_target), C++(DPI-C取指) | id(instr,pc), ex(pc), wb(pc_plus4), C++(top-level instr) | 指令流的起点，PC 控制的中枢 |
| **id_stage** | if(instr), regfile(rdata) | **全线广播**控制信号到 EX/MEM/WB/regfile/csr/IF | 控制信号的总源头，数据通路的"大脑" |
| **ex_stage** | id(控制信号), regfile(rdata), if(pc), csr(csr_rdata) | mem(alu_result), wb(alu_result), if(br_taken/jump_jalr), csr(csr_wdata) | 算术/逻辑/分支/CSR写数据的计算中心 |
| **mem_stage** | ex(alu_result=地址), regfile(rdata=存储数据), id(控制), C++(DPI-C数据) | wb(mem_rdata) | 内存访问的翻译层 |
| **wb_stage** | ex(alu_result), mem(mem_rdata), if(pc_plus4), csr(csr_rdata) | regfile(rd_wdata) | 写回数据的汇聚与选择 |
| **regfile** | id(地址), wb(数据+使能) | id/ex/mem(rdata), halt(rs_a0) | GPR 状态的唯一持有者 |
| **csr** | id(地址+使能), ex(csr_wdata), C++(中断trap_code) | if(trap_target), ex/wb(csr_rdata) | 特权状态的唯一持有者 + trap 入口控制 |
| **halt** | id(is_ebreak经仲裁层), if(pc), regfile(rs_a0) | C++(halt/halt_pc/halt_ret) | 仿真生命周期控制 |

#### 2.1.2 控制信号扇出全景

```
                        control.v (id_stage 内部)
                              │
        ┌─────────────────────┼────────────────────────────┐
        │                     │                            │
   ┌────┴────┐          ┌─────┴──────┐              ┌──────┴──────┐
   │ 到 IF   │          │   到 EX    │              │   到 MEM    │
   │ pc_sel  │          │ alu_op     │              │ mem_read    │
   │ (经仲裁)│          │ alu_src_a  │              │ mem_write   │
   └─────────┘          │ alu_src_b  │              │ lsu_type    │
                        │ br_type    │              └─────────────┘
   ┌──────────┐         │ csr_op     │
   │ 到 regfile│         │ csr_imm    │              ┌──────────────┐
   │ reg_write│         │ csr_zimm   │              │   到 WB      │
   │ (经仲裁) │         └────────────┘              │ reg_wdata_src│
   └──────────┘                                    └──────────────┘

   ┌──────────────┐         ┌──────────────┐
   │   到 CSR     │         │ 到 core_top  │
   │ csr_addr     │         │  (系统信号)   │
   │ csr_read     │         │ trap_enter   │
   │ csr_write    │         │ trap_code    │
   │ (经仲裁)     │         │ is_ebreak    │
   └──────────────┘         │ mret         │
                            │ (经仲裁)     │
                            └──────────────┘
```

#### 2.1.3 中断注入到 PC 跳转的完整链路

```
C++ timer_tick() / isa_query_intr()
        │
        ▼
top->interrupt_valid = 1
top->interrupt_cause = trap_code
        │
        ▼ (进入 core_top 仲裁层)
┌───────────────────────────────────────────┐
│  interrupt_valid=1 时所有 *_eff 信号：     │
│    mem_read_eff  = 0    (kill 当前 load)  │
│    mem_write_eff = 0    (kill 当前 store) │
│    reg_write_eff = 0    (kill 当前写回)   │
│    csr_write_eff = 0    (kill 当前 CSR写) │
│    mret_eff      = 0    (kill 当前 mret)  │
│    is_ebreak_eff = 0    (kill 当前 ebreak)│
│    trap_enter_eff = 1                     │
│    trap_code_eff  = interrupt_cause       │
└──────────────┬────────────────────────────┘
               │
               ▼
         ┌──────────┐
         │  csr.v   │  trap_enter=1:
         │          │    mepc    ← trap_pc (当前PC)
         │          │    mcause  ← trap_code
         │          │    mstatus ← 保存+关中断
         │          │    trap_target = mtvec
         └────┬─────┘
              │ trap_target = mtvec (如 0x80000000)
              ▼
         ┌──────────┐
         │ if_stage │  pc_next = trap_target
         │          │  (优先级高于 pc_sel)
         └──────────┘
              │ 下一拍 posedge clk
              ▼
         PC = mtvec  → 开始执行中断处理程序
```

#### 2.1.4 CSR 指令完整数据流（csrrw 为例）

```
csrrw rd, csr, rs1   (opcode=1110011, funct3=001)

IF: pc → DPI-C取指 → instr
        │
ID:     ├─ opcode=7'b1110011, funct3=3'b001
        ├─ control 产生:
        │    csr_op=00, csr_read=1, csr_write=1
        │    csr_imm=1 (选rs1做源), reg_write=1
        │    reg_wdata_src=11 (选CSR数据)
        ├─ csr_addr = instr[31:20] → csr
        └─ rs1_addr → regfile
              │
EX:     ├─ csr_rdata ← csr (旧CSR值，组合读)
        ├─ csr_wdata = rs1_rdata (csr_op=00, csr_imm=1)
        │             → csr (待写入的新CSR值)
        └─ csr_rdata 直通到 alu_result 线上
              │          (csr_rdata→ex→wb 是平行直通)
WB:     reg_wdata_src=11 → 选 csr_rdata
              │
         ┌────┴────┐
         │ regfile │  rf[rd] ← csr_rdata (旧CSR值写回rd)
         └─────────┘
         ┌────┴────┐
         │ csr     │  posedge clk: CSR ← csr_wdata (新值)
         └─────────┘
```

### 2.2 control — 主控单元

#### 2.2.1 基本信息 & 端口表

**control 模块架构**：

```
                    control.v
                   ┌──────────┐
opcode  ──────────→│          │──→ alu_op[4:0]       ──→ EX: alu.v
funct3 ──────────→│  组合    │──→ alu_src_a[0:0]    ──→ EX: ALU 源A MUX
funct7 ──────────→│  逻辑    │──→ alu_src_b[1:0]    ──→ EX: ALU 源B MUX
                   │          │──→ br_type[2:0]      ──→ EX: br_cond.v
                   │          │
                   │          │──→ mem_read          ──→ MEM: data_mem
                   │          │──→ mem_write         ──→ MEM: data_mem
                   │          │──→ lsu_type[2:0]     ──→ MEM: lsu.v
                   │          │
                   │          │──→ reg_write         ──→ WB: regfile 写口
                   │          │──→ reg_wdata_src[1:0]──→ WB: 写回数据 MUX
                   │          │
                   │          │──→ pc_sel[1:0]       ──→ IF: PC 下一拍选择
                   │          │
                   │          │──→ csr_op[1:0]       ──→ EX: CSR 操作类型
                   │          │──→ csr_read          ──→ CSR: 读使能
                   │          │──→ csr_write         ──→ CSR: 写使能
                   │          │──→ csr_imm           ──→ EX: CSR 源选择
                   │          │──→ csr_addr[11:0]    ──→ CSR: 地址
                   │          │──→ csr_zimm[31:0]    ──→ EX: 零扩展立即数
                   │          │
                   │          │──→ trap_enter        ──→ core_top: 异常进入
                   │          │──→ trap_code[31:0]   ──→ core_top: 异常原因
                   │          │──→ is_ebreak         ──→ core_top: ebreak 标志
                   │          │──→ mret              ──→ core_top: mret 标志
                   └──────────┘
```

| 方向 | 信号 | 位宽 | 说明 |
|:--:|------|:--:|------|
| I | `opcode` | 7 | instr[6:0] |
| I | `funct3` | 3 | instr[14:12] |
| I | `funct7` | 7 | instr[31:25] |
| I | `inst` | 32 | 完整指令（用于 csr_addr/csr_zimm 等 wire 提取） |
| O | `alu_op` | 5 | ALU 操作码 |
| O | `alu_src_a` | 1 | ALU 源A选择：0=rs1, 1=pc |
| O | `alu_src_b` | 2 | ALU 源B选择：00=rs2, 01=imm, 10=4 |
| O | `br_type` | 3 | 分支类型 |
| O | `mem_read` | 1 | 读内存使能 |
| O | `mem_write` | 1 | 写内存使能 |
| O | `lsu_type` | 3 | 访存类型 |
| O | `reg_write` | 1 | 写寄存器使能 |
| O | `reg_wdata_src` | 2 | 写回来源：00=ALU, 01=MEM, 10=PC+4, 11=CSR |
| O | `pc_sel` | 2 | PC选择：00=+4, 01=jal, 10=jalr, 11=branch |
| O | `csr_op` | 2 | CSR 操作：00=写, 01=置位, 10=清除 |
| O | `csr_read` | 1 | 读 CSR 使能 |
| O | `csr_write` | 1 | 写 CSR 使能 |
| O | `csr_imm` | 1 | CSR 源：0=zimm, 1=rs1 |
| O | `csr_addr` | 12 | CSR 地址 = instr[31:20]（wire assign） |
| O | `csr_zimm` | 32 | 零扩展立即数 = {27'b0, instr[19:15]}（wire assign） |
| O | `trap_enter` | 1 | 异常/中断进入 |
| O | `trap_code` | 32 | 异常原因：11=ecall, 3=ebreak |
| O | `is_ebreak` | 1 | ebreak 指令标志 |
| O | `mret` | 1 | mret 指令标志 |

#### 2.2.2 默认值设计

```verilog
alu_op        = SUB       // 无害计算，结果被 reg_write=0 丢弃
alu_src_a     = 0         // rs1
alu_src_b     = 00        // rs2
br_type       = 000       // BEQ, 但 pc_sel=00 忽略
mem_read      = 0         // 不读内存
mem_write     = 0         // 不写内存（关键阀门）
lsu_type      = 000       // LW, 但 mem_read=0 门控
reg_write     = 0         // 不写寄存器（关键阀门）
reg_wdata_src = 00        // ALU
pc_sel        = 00        // PC+4（关键阀门）
// CSR
csr_op        = 00        // WRITE（直接写）
csr_read      = 0         // 不读 CSR
csr_write     = 0         // 不写 CSR（关键阀门）
csr_imm       = 0         // zimm
// System
is_ebreak     = 0         // 非 ebreak
trap_enter    = 0         // 不进入 trap（关键阀门）
trap_code     = 0         // 无异常
mret          = 0         // 非 mret
```

> 五道阀门：`reg_write=0`、`mem_write=0`、`pc_sel=00`、`csr_write=0`、`trap_enter=0`。

#### 2.2.3 指令分发总表（按指令类型逐信号列出）

> 每条指令只需覆写与默认值不同的信号，其余靠默认值。

| 指令类型 | 需显式覆写的信号 | 覆写值 | 个数 |
|---------|-----------------|--------|:--:|
| **R 型** | `reg_write` | 1 | 2 |
|  | `alu_op` | 由 funct3/funct7 查表 | |
| **I 算术** | `reg_write` | 1 | 3 |
|  | `alu_src_b` | 01（选 imm） | |
|  | `alu_op` | 由 funct3/funct7 查表 | |
| **I load** | `reg_write` | 1 | 6 |
|  | `mem_read` | 1 | |
|  | `alu_src_b` | 01（选 imm） | |
|  | `reg_wdata_src` | 01（选 MEM 数据） | |
|  | `alu_op` | ADD（地址计算 = rs1+imm） | |
|  | `lsu_type` | funct3 查表（lb/lh/lw/lbu/lhu） | |
| **S store** | `mem_write` | 1 | 4 |
|  | `alu_src_b` | 01（选 imm） | |
|  | `alu_op` | ADD（地址计算） | |
|  | `lsu_type` | funct3 + sw/sh/sb 编码 | |
| **B branch** | `pc_sel` | 11（分支指令） | 2 |
|  | `br_type` | 由 funct3 查表 | |
| **jal** | `reg_write` | 1 | 3 |
|  | `reg_wdata_src` | 10（选 PC+4 作返回地址） | |
|  | `pc_sel` | 01（jal 跳转） | |
| **jalr** | `alu_src_b` | 01（选 imm_I） | 5 |
|  | `alu_op` | ADD（rs1 + imm） | |
|  | `reg_write` | 1 | |
|  | `reg_wdata_src` | 10（选 PC+4 作返回地址） | |
|  | `pc_sel` | 10（jalr 跳转） | |
| **lui** | `reg_write` | 1 | 3 |
|  | `alu_src_b` | 01（选 imm_U） | |
|  | `alu_op` | LUI（透传 src2） | |
| **auipc** | `reg_write` | 1 | 4 |
|  | `alu_src_a` | 1（选 PC） | |
|  | `alu_src_b` | 01（选 imm_U） | |
|  | `alu_op` | ADD（PC + imm） | |
| **ecall** | `trap_enter` | 1 | 2 |
|  | `trap_code` | 11（Environment call from M-mode） | |
| **ebreak** | `is_ebreak` | 1 | 1 |
| **mret** | `mret` | 1 | 1 |
| **CSR** | `csr_op` | 00/01/10（依指令） | 6~7 |
|  | `csr_read` / `csr_write` | 1 | |
|  | `csr_imm` | 0 或 1（依指令） | |
|  | `reg_write` | 1 | |
|  | `reg_wdata_src` | 11（选 CSR 数据） | |
|  | `csr_write`（门控）| rs1≠0 或 zimm≠0 时才写（csrrs/csrrc/csrrsi/csrrci） | |

#### 2.2.4 审查记录

| 检查项 | 结果 |
|--------|:--:|
| 20组信号默认值 | ✅ |
| R/I算术/I Load/S Store/B Branch 覆写 | ✅ |
| jal 不需ALU（pc_plus4直通） | ✅ |
| jalr 需ALU（rs1+imm） | ✅ |
| auipc alu_src_a=1（选PC） | ✅ |
| srli/srai 用 funct7[5] 区分 | ✅ |
| CSR 指令控制信号 | ✅ |
| ecall/ebreak/mret 系统指令 | ✅ |
| csr_write 门控（csrrs/csrrc 源=0 不写） | ✅ |

#### 2.2.5 各信号完整编码速查

| 信号 | 位宽 | 编码值 |
|------|:--:|------|
| `alu_src_a` | 1 | 0 = rdata1（rs1 寄存器值）, 1 = pc |
| `alu_src_b` | 2 | 00 = rdata2（rs2 寄存器值）, 01 = imm（立即数）, 10 = 4（常数） |
| `reg_wdata_src` | 2 | 00 = ALU 结果, 01 = MEM 数据, 10 = PC+4, 11 = CSR 数据 |
| `pc_sel` | 2 | 00 = PC+4, 01 = jal(pc+imm_J), 10 = jalr(ALU结果), 11 = branch |
| `br_type` | 3 | 000 = BEQ, 001 = BNE, 010 = BLT, 011 = BGE, 100 = BLTU, 101 = BGEU |
| `lsu_type` | 3 | 000 = LW, 001 = LH, 010 = LB, 011 = LHU, 100 = LBU, 101 = SW, 110 = SH, 111 = SB |
| `csr_op` | 2 | 00 = WRITE, 01 = SET(OR), 10 = CLEAR(AND~) |
| `csr_imm` | 1 | 0 = zimm(零扩展rs1_addr), 1 = rs1值 |

> 为什么 `alu_src_b` 位宽为 2？因为 ALU.src2 有 3 种来源，至少需要 2 位编码。10=4 为流水线转发预留编码。

#### 2.2.6 判断依据的本质：ISA → 控制信号的推演方法

> **核心认知**：先有数据通路，再有控制信号。控制信号本质上就是"数据通路中所有多路选择器的选择端 + 各模块的使能端"。

**推演链**：

```
ISA 指令语义（伪代码）
        ↓
  数据通路操作（这条指令要让数据怎么流动？）
        ↓
  流动路径上每个 MUX 该选哪一路？
        ↓
  确定每个控制信号的值
```

**从 ISA 伪代码到控制信号：逐条推演**：

| ISA 定义（伪代码） | 翻译为数据通路操作 | 对应的控制信号 |
|-------------------|-------------------|-------------|
| `x[rd] = x[rs1] + x[rs2]` | ALU 做 rs1+rs2，结果写 rf | `alu_src_a=0` `alu_src_b=00` `reg_write=1` `reg_wdata_src=00` |
| `x[rd] = x[rs1] + sext(imm)` | ALU 做 rs1+imm，结果写 rf | `alu_src_a=0` `alu_src_b=01` `reg_write=1` |
| `x[rd] = M[x[rs1] + sext(imm)]` | ALU 算地址，读内存，写 rf | `alu_src_b=01` `mem_read=1` `reg_write=1` `reg_wdata_src=01` |
| `M[x[rs1] + sext(imm)] = x[rs2]` | ALU 算地址，写内存 | `alu_src_b=01` `mem_write=1` `reg_write=0` |
| `if (x[rs1] == x[rs2]) pc += sext(imm)` | br_cond 做比较，条件成立跳转 | `br_type=BEQ` `pc_sel=11` |

**通用判断流程（以 alu_src_a / alu_src_b 为例）**：

```
Q1: 这条指令要算什么？
    ↓
Q2: ALU 需要哪两个操作数？
    ↓
Q3: 这两个操作数在数据通路中分别可以从哪里来？
    ↓
Q4: 查 MUX 输入选项 → 确定 sel 值
```

**实例推演**：

| 指令 | 要算什么 | ALU.src1 来源 | alu_src_a | ALU.src2 来源 | alu_src_b |
|------|---------|:-----------:|:---------:|:-----------:|:---------:|
| `add rd,rs1,rs2` | rs1 + rs2 | rdata1 | 0 | rdata2 | 00 |
| `addi rd,rs1,imm` | rs1 + imm | rdata1 | 0 | imm | 01 |
| `lw rd,off(rs1)` | rs1 + off（地址） | rdata1 | 0 | imm | 01 |
| `auipc rd,imm` | pc + imm_U | **pc** | **1** | imm | 01 |
| `jal rd,label` | 返回地址=pc+4 | — | — | — | — (reg_wdata_src=10直通) |
| `sw rs2,off(rs1)` | rs1 + off（地址） | rdata1 | 0 | imm | 01 |

**默认状态对照：如何判断哪些信号需要覆写**：

| 默认状态 | 含义 | 什么指令会改变它？ |
|---------|------|-------------------|
| `alu_src_a = 0` | ALU.src1 取自 rs1 | auipc（需要用 PC 做源操作数） |
| `alu_src_b = 00` | ALU.src2 取自 rs2 | 几乎所有非 R 型（需要用 imm 或常数） |
| `reg_write = 0` | 不写寄存器 | 所有需要产生结果的指令 |
| `reg_wdata_src = 00` | 写回数据来自 ALU | load（来自内存）、jal/jalr（来自 PC+4） |
| `mem_read = 0` | 不读内存 | load 指令 |
| `mem_write = 0` | 不写内存 | store 指令 |
| `pc_sel = 00` | 下一条 = PC+4 | 跳转/分支指令 |
| `br_type = 000` | 不分支 | B 型指令 |


| 非Store指令 wmask=0 | ✅ |

### 2.3 if_stage — 取指阶段

#### 2.3.1 基本信息 & 端口表

| 方向 | 信号 | 位宽 | 来源/去向 | 说明 |
|:--:|------|:--:|------|------|
| I | `clk` | 1 | 全局 | PC寄存器时钟 |
| I | `rst` | 1 | 全局 | 复位到 `0x80000000` |
| I | `pc_sel` | 2 | id_stage(control) | PC选择 |
| I | `br_taken` | 1 | ex_stage(br_cond) | 分支是否成立 |
| I | `jump_jalr` | 32 | ex_stage(alu) | jalr跳转目标 |
| I | `imm_jal` | 32 | id_stage(imm_gen) | jal跳转偏移 |
| I | `imm_br` | 32 | id_stage(imm_gen) | 分支跳转偏移 |
| I | `trap_enter` | 1 | core_top(仲裁层) | 异常/中断进入 |
| I | `trap_target` | 32 | csr | 陷阱目标地址 |
| I | `mret` | 1 | core_top(仲裁层) | mret 返回 |
| O | `pc` | 32 | ID+EX+halt | 当前PC |
| O | `pc_plus4` | 32 | WB(wb_stage) | 返回地址 |
| O | `instr` | 32 | ID+C++ | 取出的指令 |

#### 2.3.2 PC 选择逻辑

```verilog
// 5路MUX：trap/mret 优先级最高
assign pc_next = (trap_enter | mret)  ? trap_target             :
                 (pc_sel == 2'b00)    ? pc + 32'd4              :
                 (pc_sel == 2'b01)    ? pc + imm_jal            :
                 (pc_sel == 2'b10)    ? {jump_jalr[31:1], 1'b0} :
                 (pc_sel == 2'b11)    ? (br_taken ? pc + imm_br : pc + 32'd4) : pc + 32'd4;
```

| 优先级 | 触发条件 | PC_next | 对应场景 |
|:------:|------|------|------|
| 最高 | `trap_enter \| mret` | `trap_target` (mtvec 或 mepc) | 异常/中断/mret |
| — | `pc_sel=00` | `pc + 4` | 默认顺序 |
| — | `pc_sel=01` | `pc + imm_J` | jal |
| — | `pc_sel=10` | `(alu_result) & ~1` | jalr |
| — | `pc_sel=11` | `br_taken ? pc + imm_B : pc + 4` | 分支 |

#### 2.3.3 指令存储器映射（DPI-C 方式）

- 不再使用 Verilog 内部的 `imem[]` 数组
- 通过 DPI-C 调用 C++ 侧 `dpi_mem_read(pc, 0)` 从统一物理内存读取指令
- 第二个参数 `is_load=0`：标记为取指操作，不触发 MMIO 副作用（difftest 跳过检测）
- 复位向量：`pc <= 32'h8000_0000`（与 PMEM_BASE 一致）
- 仿真前 C++ 侧 `pmem_load_bin()` 把 .bin 加载到 `npc_pmem[0:PMEM_SIZE-1]`

#### 2.3.4 审查记录

| 检查项 | 结果 |
|--------|:--:|
| PC_next 5路MUX（含trap/mret） | ✅ |
| trap/mret 最高优先级 | ✅ |
| jalr `& ~1` 对齐 | ✅ |
| 分支 br_taken 二级判断 | ✅ |
| 复位向量 0x80000000 | ✅ |
| pc_plus4 输出到 WB | ✅ |
| DPI-C 取指（is_load=0） | ✅ |
| 时序 `<=` 非阻塞赋值 | ✅ |

### 2.4 id_stage — 译码阶段

#### 2.4.1 基本信息 & 端口表

| 方向 | 信号 | 位宽 | 来源/去向 | 说明 |
|:--:|------|:--:|------|------|
| I | `instr` | 32 | if_stage | 当前指令 |
| I | `rs1_rdata` | 32 | regfile 读口 | 操作数1（直通到 EX） |
| I | `rs2_rdata` | 32 | regfile 读口 | 操作数2（直通到 EX） |
| O | `rs1_addr` | 5 | → regfile | 读口1地址 = instr[19:15] |
| O | `rs2_addr` | 5 | → regfile | 读口2地址 = instr[24:20] |
| O | `rd_addr` | 5 | → regfile | 写口地址 = instr[11:7] |
| O | `imm_out` | 32 | → EX + IF | 立即数 |
| O | `alu_op` | 5 | → EX | ALU操作码 |
| O | `alu_src_a` | 1 | → EX | ALU源A选择 |
| O | `alu_src_b` | 2 | → EX | ALU源B选择 |
| O | `br_type` | 3 | → EX | 分支类型 |
| O | `mem_read` | 1 | → MEM | 读内存 |
| O | `mem_write` | 1 | → MEM | 写内存 |
| O | `lsu_type` | 3 | → MEM | 访存类型 |
| O | `reg_write` | 1 | → regfile | 写寄存器 |
| O | `reg_wdata_src` | 2 | → WB | 写回来源 |
| O | `pc_sel` | 2 | → IF | PC选择 |

#### 2.4.2 内部结构

```
instr[31:0] ──┬── [31:25] → funct7 ──→ control.v
              ├── [24:20] → rs2_addr ──→ 输出给 regfile
              ├── [19:15] → rs1_addr ──→ 输出给 regfile
              ├── [14:12] → funct3 ──→ control.v
              ├── [11:7]  → rd_addr ──→ 输出给 regfile
              ├── [6:0]   → opcode ──→ control.v
              ├──→ imm_gen.v ──→ imm_out
              └── 直通: rs1_rdata/rs2_rdata → EX

内部例化: imm_gen.v, control.v
不含: regfile (已移至 core_top 层)
```

**regfile 原本在 id_stage 内部，为适配流水线升级方案 B 而移出。** ID 只负责译码+控制+立即数，regfile 作为顶层独立模块供 ID 读和 WB 写。

#### 2.4.3 审查记录

| 检查项 | 结果 |
|--------|:--:|
| 字段切片 wire | ✅ |
| imm_gen 例化 | ✅ |
| control 例化 | ✅ |
| 控制信号全输出 | ✅ |
| regfile 已移出 | ✅ |
| rd_addr 输出 | ✅ |
| rs1/rs2_rdata 直通 | ✅ |

### 2.5 ex_stage — 执行阶段

#### 2.5.1 基本信息 & 端口表

| 方向 | 信号 | 位宽 | 来源/去向 |
|:--:|------|:--:|------|
| I | `rs1_rdata` | 32 | regfile(→id→ex 直通) |
| I | `rs2_rdata` | 32 | regfile(→id→ex 直通) |
| I | `imm_out` | 32 | id_stage |
| I | `pc` | 32 | if_stage |
| I | `alu_op` | 5 | id_stage(control) |
| I | `alu_src_a` | 1 | id_stage(control) |
| I | `alu_src_b` | 2 | id_stage(control) |
| I | `br_type` | 3 | id_stage(control) |
| I | `csr_op` | 2 | id_stage(control) |
| I | `csr_imm` | 1 | id_stage(control) |
| I | `csr_zimm` | 32 | id_stage(control) |
| I | `csr_rdata` | 32 | csr |
| O | `alu_result` | 32 | → MEM + WB + IF(jalr) |
| O | `br_taken` | 1 | → IF |
| O | `csr_wdata` | 32 | → csr |

#### 2.5.2 ALU 源选择 MUX

```
                 ┌─────────┐
    rdata1  ───→│  MUX_A  │──→ src1 → ALU.src1
    pc      ───→│         │
                │sel=alu_ │
                │ src_a   │
                └─────────┘

                 ┌─────────┐
    rdata2  ───→│  MUX_B  │──→ src2 → ALU.src2
    imm     ───→│         │
    4       ───→│sel=alu_ │
                │ src_b   │
                └─────────┘
```

```verilog
assign src1 = (alu_src_a == 0) ? rs1_rdata : pc;
assign src2 = (alu_src_b == 00) ? rs2_rdata :
              (alu_src_b == 01) ? imm_out   : 32'd4;
```

| alu_src_a | src1 = | 用它的指令 |
|:--------:|--------|---------|
| `0` | rs1_rdata | add, addi, lw, sw, jalr, sub... |
| `1` | pc | auipc |

| alu_src_b | src2 = | 用它的指令 |
|:--------:|--------|---------|
| `00` | rs2_rdata | add, sub, sll...（R型） |
| `01` | imm_out | addi, lw, sw, lui, auipc, jalr... |
| `10` | 4 | 预留（流水线转发） |

> **注意**：MUX 选择了 src1/src2 后送给 alu。之前 bug 是直接连 rs1_rdata/rs2_rdata 给 alu，绕过了 MUX——已修复。

#### 2.5.3 审查记录

| 检查项 | 结果 |
|--------|:--:|
| src1/src2 MUX 逻辑 | ✅ |
| br_cond 例化 | ✅ |
| alu 例化（已接 src1/src2 而非 rs1/rs2） | ✅ |
| CSR 写数据生成（csrrw/csrrs/csrrc） | ✅ |
| jump_target 不再需要单独输出 | ✅ |

### 2.6 mem_stage — 访存阶段

#### 2.6.1 基本信息 & 端口表

| 方向 | 信号 | 位宽 | 来源/去向 |
|:--:|------|:--:|------|
| I | `clk` | 1 | 全局 |
| I | `mem_addr` | 32 | core_top assign(=alu_result) |
| I | `mem_wdata_raw` | 32 | core_top assign(=rs2_rdata) |
| I | `lsu_type` | 3 | id_stage(control) |
| I | `mem_read` | 1 | id_stage(control) |
| I | `mem_write` | 1 | id_stage(control) |
| O | `mem_rdata` | 32 | → WB |

#### 2.6.2 统一内存 + LSU 流程（DPI-C 方式）

```
alu_result(=mem_addr) → dpi_mem_read(mem_addr, mem_read) → mem_rdata_raw (组合读)
                              ↓
rs2_rdata(=mem_wdata_raw) → lsu.v → mem_wdata + mem_wmask
                              ↓
                       dpi_mem_write(mem_addr, mem_wdata, mem_wmask) (时序写, posedge clk, mem_write 门控)
                              ↓
                       mem_rdata_raw → lsu.v → mem_rdata (Load: 扩展 → WB)
```

**关键设计**：
- 统一物理内存由 C++ 侧 `npc_pmem[128MB]` 管理
- DPI-C `dpi_mem_read(addr, is_load)`：第二个参数用于区分取指（is_load=0）和数据读（is_load=1），只有数据读才触发 MMIO skip
- DPI-C `dpi_mem_write(addr, data, wmask)`：按字节掩码写入，LSB 优先（小端序）
- **lsu.v 是必经之路**：读时做扩展，写时做对齐+掩码

#### 2.6.3 审查记录

| 检查项 | 结果 |
|--------|:--:|
| DPI-C 组合读（dpi_mem_read） | ✅ |
| DPI-C 时序写（dpi_mem_write, 按 wmask） | ✅ |
| lsu 例化（含 mem_read/mem_write 门控） | ✅ |
| mem_write_eff 启用（中断 kill） | ✅ |

### 2.7 wb_stage — 写回阶段

#### 2.7.1 基本信息 & 端口表

| 方向 | 信号 | 位宽 | 来源/去向 |
|:--:|------|:--:|------|
| I | `reg_wdata_src` | 2 | id_stage(control) |
| I | `alu_result` | 32 | ex_stage |
| I | `mem_rdata` | 32 | mem_stage |
| I | `pc_plus4` | 32 | if_stage |
| I | `csr_rdata` | 32 | csr |
| O | `reg_wdata` | 32 | → regfile.rd_wdata |

纯组合MUX：`00→alu_result, 01→mem_rdata, 10→pc_plus4, 11→csr_rdata`。

**wb_stage = 1个4选1MUX = 数据通路最简单但写回路径最全的模块。**

#### 2.7.2 审查记录

| 检查项 | 结果 |
|--------|:--:|
| 4选1 MUX | ✅ |
| CSR 通道 | ✅ |
| 无时序 | ✅ |
| reg_wdata 输出 | ✅ |

### 2.8 core_top — 顶层连线

#### 2.8.1 基本信息

| 属性 | 值 |
|------|-----|
| 文件 | `npc/vsrc/core_top.v` |
| 输入 | `clk`, `rst`, `interrupt_valid`, `interrupt_cause` |
| 输出 | `halt`, `halt_pc`, `halt_ret`, `instr` |
| 内部模块 | 5 stage + regfile + csr + halt = 8个 |

#### 2.8.2 信号总线表

| bus | 宽度 | 驱动者 | 消费者 |
|-----|:--:|------|--------|
| `pc, pc_plus4, instr` | 96 | if_stage | ID, EX, WB |
| `rs1_addr, rs2_addr, rd_addr` | 15 | id_stage | regfile |
| `rs1_rdata, rs2_rdata` | 64 | regfile | id_stage (→ex_stage 直通) |
| `alu_op, alu_src_a, alu_src_b, br_type` | 11 | id_stage(control) | ex_stage |
| `mem_read, mem_write, lsu_type` | 5 | id_stage(control) | mem_stage |
| `reg_write` | 1 | id_stage(control) | regfile |
| `reg_wdata_src` | 2 | id_stage(control) | wb_stage |
| `pc_sel` | 2 | id_stage(control) | if_stage |
| `alu_result` | 32 | ex_stage | mem_stage, wb_stage, if_stage |
| `br_taken` | 1 | ex_stage | if_stage |
| `imm_out` | 32 | id_stage | ex_stage, if_stage |
| `mem_rdata` | 32 | mem_stage | wb_stage |
| `reg_wdata` | 32 | wb_stage | regfile |

#### 2.8.3 跨阶段关键 assign

```verilog
assign jump_jalr    = alu_result;        // jalr跳转目标 = ALU输出
assign imm_jal      = imm_out;           // jal偏移(imm_J)
assign imm_br       = imm_out;           // 分支偏移(imm_B)
assign mem_addr     = alu_result;        // 访存地址 = ALU输出
assign mem_wdata_raw = rs2_rdata;        // 存储数据 = regfile读口2
assign rd_wdata     = reg_wdata;         // 写回数据 → regfile
```

> **注意**：`imm_jal`/`imm_br`/`imm_out` 本质是同一信号，在 core_top 通过 assign 重命名连接不同模块端口。

#### 2.8.4 审查记录

| 检查项 | 结果 |
|--------|:--:|
| 6个模块例化 | ✅ |
| 所有wire声明 | ✅ |
| `pc` 为 wire（非 reg） | ✅ |
| 5个assign 重命名 | ✅ |
| `rd_wdata` 与 `reg_wdata` 连接 | ✅ |
| wb_stage.reg_wdata 已连接 | ✅ |

### 2.9 PC 计算来源与路由规则

> 关键判断：第二操作数是常量（if_stage能算）还是来自寄存器（必须走ALU）？结果是否要写回寄存器？

| 指令 | PC计算 | 操作数 | 结果去向 | 走哪里 |
|------|------|------|------|:--:|
| 默认 | `pc+4` | PC+常数4 | 仅改PC | if_stage |
| jal | `pc+imm_J` | PC+常数 | 仅改PC | if_stage |
| jal(返回地址) | `pc+4` | PC+常数 | 写rd | 直通pc_plus4(reg_wdata_src=10) |
| jalr | `(rs1+imm_I)&~1` | **寄存器rs1**+常数 | 仅改PC | **ALU** |
| jalr(返回地址) | `pc+4` | PC+常数 | 写rd | 直通pc_plus4 |
| branch | `pc+imm_B` | PC+常数 | 仅改PC | if_stage |
| auipc | `pc+imm_U` | PC+常数 | **写rd** | **ALU**(结果走EX→MEM→WB) |

**简记**：if_stage管纯跳转；ALU 管"操作数含寄存器"或"结果要写回寄存器"的PC计算。

### 2.10 控制信号安全网：阀门模型

> 前面跑得多热闹不重要。阀门关着，数据进不来。

| 阀门 | 控制信号 | 默认值 | 关了=效果 |
|------|---------|:--:|------|
| 阀门1(访存) | `mem_read`/`mem_write` | 0 | 不读写内存 |
| 阀门2(写回) | `reg_write` | 0 | 不修改任何寄存器 |
| 阀门3(跳转) | `pc_sel` | 00 | PC只走+4 |
| 阀门4(CSR) | `csr_write` | 0 | 不修改 CSR |
| 阀门5(异常) | `trap_enter` | 0 | 不进入 trap |

其余信号（alu_op、br_type、lsu_type、csr_op）的默认值虽然等于真实指令，但被阀门挡住无副作用：
- ALU 算出垃圾 → `reg_write=0` 不写回
- br_cond 判断成立 → `pc_sel=00` 不跳转
- LSU 扩展出垃圾 → `mem_read=0` 门控输出0
- CSR 准备好写数据 → `csr_write=0` 不写

### 2.11 架构分布数据通路总览

> 基于 `core_top.v` 完整连线。所有模块的输入来源和输出去向逐端口列出。
> 信号名 = core_top 中声明的 wire 名。

#### 2.11.1 if_stage（取指）→ 模块 I/O 表

| 方向 | 端口 | 来源模块 | 去向模块 | core_top wire |
|:--:|------|---------|---------|:--:|
| I | `clk` | — (全局) | — | `clk` |
| I | `rst` | — (全局) | — | `rst` |
| I | `pc_sel` | id_stage(control) | — | `pc_sel[1:0]` |
| I | `br_taken` | ex_stage(br_cond) | — | `br_taken` |
| I | `jump_jalr` | ex_stage(alu)→assign | — | `jump_jalr = alu_result` |
| I | `imm_jal` | id_stage(imm_gen)→assign | — | `imm_jal = imm_out` |
| I | `imm_br` | id_stage(imm_gen)→assign | — | `imm_br = imm_out` |
| I | `trap_enter` | 仲裁层(trap_enter_eff) | — | `trap_enter_eff` |
| I | `trap_target` | csr | — | `trap_target[31:0]` |
| I | `mret` | 仲裁层(mret_eff) | — | `mret_eff` |
| O | `pc` | — | id_stage, ex_stage, halt | `pc[31:0]` |
| O | `pc_plus4` | — | wb_stage | `pc_plus4[31:0]` |
| O | `instr` | — | id_stage, C++(top) | `instr[31:0]` |

#### 2.11.2 id_stage（译码）→ 模块 I/O 表

| 方向 | 端口 | 来源模块 | 去向模块 | core_top wire |
|:--:|------|---------|---------|:--:|
| I | `instr` | if_stage | — | `instr[31:0]` |
| I | `rs1_rdata` | regfile 读口 | — (直通到 ex) | `rs1_rdata[31:0]` |
| I | `rs2_rdata` | regfile 读口 | — (直通到 ex) | `rs2_rdata[31:0]` |
| O | `rs1_addr` | — | regfile 读口1 | `rs1_addr[4:0]` |
| O | `rs2_addr` | — | regfile 读口2 | `rs2_addr[4:0]` |
| O | `rd_addr` | — | regfile 写口 | `rd_addr[4:0]` |
| O | `imm_out` | — | ex_stage + if_stage | `imm_out[31:0]` |
| O | `alu_op` | — | ex_stage | `alu_op[4:0]` |
| O | `alu_src_a` | — | ex_stage | `alu_src_a` |
| O | `alu_src_b` | — | ex_stage | `alu_src_b[1:0]` |
| O | `br_type` | — | ex_stage | `br_type[2:0]` |
| O | `mem_read` | — | mem_stage | `mem_read` |
| O | `mem_write` | — | mem_stage | `mem_write` |
| O | `lsu_type` | — | mem_stage | `lsu_type[2:0]` |
| O | `reg_write` | — | regfile 写口 | `reg_write` |
| O | `reg_wdata_src` | — | wb_stage | `reg_wdata_src[1:0]` |
| O | `pc_sel` | — | if_stage | `pc_sel[1:0]` |

> id_stage 内部例化：imm_gen.v（1入1出纯组合） + control.v（3入10出纯组合）。控制信号从 control.v 产生后全部输出到 core_top 层。

#### 2.11.3 ex_stage（执行）→ 模块 I/O 表

| 方向 | 端口 | 来源模块 | 去向模块 | core_top wire |
|:--:|------|---------|---------|:--:|
| I | `rs1_rdata` | regfile(→id→ex 直通) | — | `rs1_rdata[31:0]` |
| I | `rs2_rdata` | regfile(→id→ex 直通) | — | `rs2_rdata[31:0]` |
| I | `imm_out` | id_stage(imm_gen) | — | `imm_out[31:0]` |
| I | `pc` | if_stage | — | `pc[31:0]` |
| I | `alu_op` | id_stage(control) | — | `alu_op[4:0]` |
| I | `alu_src_a` | id_stage(control) | — | `alu_src_a` |
| I | `alu_src_b` | id_stage(control) | — | `alu_src_b[1:0]` |
| I | `br_type` | id_stage(control) | — | `br_type[2:0]` |
| O | `alu_result` | — | mem_stage, wb_stage, if_stage | `alu_result[31:0]` |
| O | `br_taken` | — | if_stage | `br_taken` |

> ex_stage 内部例化：alu.v（3入1出纯组合） + br_cond.v（3入1出纯组合） + 2路MUX。`alu_result` 是核心扇出信号——同时送给 mem_stage(访存地址)、wb_stage(写回数据源0)、if_stage(jalr跳转目标)。

#### 2.11.4 mem_stage（访存）→ 模块 I/O 表

| 方向 | 端口 | 来源模块 | 去向模块 | core_top wire |
|:--:|------|---------|---------|:--:|
| I | `clk` | — (全局) | — | `clk` |
| I | `mem_addr` | ex_stage(alu)→assign | — | `mem_addr = alu_result` |
| I | `mem_wdata_raw` | regfile(rs2)→assign | — | `mem_wdata_raw = rs2_rdata` |
| I | `lsu_type` | id_stage(control) | — | `lsu_type[2:0]` |
| I | `mem_read` | id_stage(control)→eff | — | `mem_read_eff` |
| I | `mem_write` | id_stage(control)→eff | — | `mem_write_eff` |
| O | `mem_rdata` | — | wb_stage | `mem_rdata[31:0]` |

> mem_stage 内部：DPI-C 调用 `dpi_mem_read/dpi_mem_write` 访问 C++ 统一内存，lsu.v 做格式翻译。`mem_rdata_raw` 在 mem_stage 内部 wire。

#### 2.11.5 wb_stage（写回）→ 模块 I/O 表

| 方向 | 端口 | 来源模块 | 去向模块 | core_top wire |
|:--:|------|---------|---------|:--:|
| I | `reg_wdata_src` | id_stage(control) | — | `reg_wdata_src[1:0]` |
| I | `alu_result` | ex_stage(alu) | — | `alu_result[31:0]` |
| I | `mem_rdata` | mem_stage(lsu) | — | `mem_rdata[31:0]` |
| I | `pc_plus4` | if_stage | — | `pc_plus4[31:0]` |
| I | `csr_rdata` | csr | — | `csr_rdata[31:0]` |
| O | `reg_wdata` | — | regfile 写口 | `rd_wdata[31:0]` |

> wb_stage = 纯组合 4 选 1 MUX：`00→alu_result, 01→mem_rdata, 10→pc_plus4, 11→csr_rdata`。

#### 2.11.6 regfile（寄存器文件，独立）→ 模块 I/O 表

| 方向 | 端口 | 来源模块 | 去向模块 | core_top wire |
|:--:|------|---------|---------|:--:|
| I | `clk` | — (全局) | — | `clk` |
| I | `rs1_addr` | id_stage | — | `rs1_addr[4:0]` |
| I | `rs2_addr` | id_stage | — | `rs2_addr[4:0]` |
| I | `rd_addr` | id_stage | — | `rd_addr[4:0]` |
| I | `rd_wdata` | wb_stage | — | `rd_wdata[31:0]` |
| I | `reg_write` | id_stage(control) | — | `reg_write` |
| O | `rs1_rdata` | — | id_stage(→ex直通) | `rs1_rdata[31:0]` |
| O | `rs2_rdata` | — | id_stage(→ex直通) | `rs2_rdata[31:0]` |

#### 2.11.7 全局信号总线全览

> core_top 中所有 wire，按功能分组。

| 组 | wire | 宽度 | 驱动者 | 消费者 |
|----|------|:--:|--------|--------|
| **时钟复位** | `clk` / `rst` | 1 | 外部 | if_stage, mem_stage, regfile, csr, halt |
| **中断** | `interrupt_valid` / `interrupt_cause` | 1/32 | C++ | core_top(仲裁层) |
| **指令流** | `instr` | 32 | if_stage | id_stage, C++(top-level output) |
| **PC相关** | `pc` | 32 | if_stage | id_stage, ex_stage, halt |
|  | `pc_plus4` | 32 | if_stage | wb_stage |
|  | `pc_sel` | 2 | id_stage(control) | if_stage |
|  | `jump_jalr` | 32 | (=alu_result) | if_stage |
|  | `imm_jal` | 32 | (=imm_out) | if_stage |
|  | `imm_br` | 32 | (=imm_out) | if_stage |
|  | `br_taken` | 1 | ex_stage | if_stage |
|  | `trap_target` | 32 | csr | if_stage |
| **寄存器地址** | `rs1_addr` / `rs2_addr` / `rd_addr` | 5 | id_stage | regfile |
| **寄存器数据** | `rs1_rdata` / `rs2_rdata` | 32 | regfile | id_stage, ex_stage |
|  | `rd_wdata` | 32 | wb_stage(=reg_wdata) | regfile |
|  | `rs_a0` | 32 | regfile | halt |
| **执行控制** | `alu_op` | 5 | id_stage(control) | ex_stage |
|  | `alu_src_a` | 1 | id_stage(control) | ex_stage |
|  | `alu_src_b` | 2 | id_stage(control) | ex_stage |
|  | `br_type` | 3 | id_stage(control) | ex_stage |
|  | `imm_out` | 32 | id_stage(imm_gen) | ex_stage, if_stage |
| **执行结果** | `alu_result` | 32 | ex_stage | mem_stage, wb_stage, if_stage |
| **CSR 控制** | `csr_op` | 2 | id_stage(control) | ex_stage |
|  | `csr_read` / `csr_write` | 1 | id_stage(control) | csr |
|  | `csr_imm` | 1 | id_stage(control) | ex_stage |
|  | `csr_addr` | 12 | id_stage | csr |
|  | `csr_zimm` | 32 | id_stage(control) | ex_stage |
|  | `csr_wdata` | 32 | ex_stage | csr |
|  | `csr_rdata` | 32 | csr | ex_stage, wb_stage |
| **Trap 控制** | `trap_enter` / `trap_code` | 1/32 | id_stage(control) | 仲裁层→csr |
|  | `mret` | 1 | id_stage(control) | 仲裁层→csr+if_stage |
|  | `is_ebreak` | 1 | id_stage(control) | 仲裁层→halt |
| **访存控制** | `mem_read_eff` / `mem_write_eff` | 1 | 仲裁层 | mem_stage |
|  | `lsu_type` | 3 | id_stage(control) | mem_stage |
|  | `mem_addr` | 32 | (=alu_result) | mem_stage |
|  | `mem_wdata_raw` | 32 | (=rs2_rdata) | mem_stage |
| **访存结果** | `mem_rdata` | 32 | mem_stage | wb_stage |
| **写回控制** | `reg_write_eff` | 1 | 仲裁层 | regfile |
|  | `reg_wdata_src` | 2 | id_stage(control) | wb_stage |
| **停止** | `halt` / `halt_pc` / `halt_ret` | 1/32/32 | halt | C++(top-level output) |

#### 2.11.8 跨阶段 assign 别名

| 别名 | = | 原始信号 | 为什么需要 |
|------|---|---------|----------|
| `jump_jalr` | `alu_result` | ex_stage.alu_result | if_stage 内部 port 用语义化命名 |
| `imm_jal` | `imm_out` | id_stage.imm_out | 同上，区分jal/branch |
| `imm_br` | `imm_out` | id_stage.imm_out | 同上 |
| `mem_addr` | `alu_result` | ex_stage.alu_result | 语义化：访存地址 = ALU结果 |
| `mem_wdata_raw` | `rs2_rdata` | regfile.rs2_rdata | 语义化：存储数据 = rs2值 |

> 这些 assign 不消耗任何逻辑门——只是给同一根 wire 起不同的别名，在不同模块端口之间对接。

#### 2.11.9 时序元件分布

单周期 CPU 中，**状态存储存在于 5 处**：

| 位置 | 模块 | 描述 | 时钟沿 |
|------|------|------|:--:|
| PC 寄存器 | if_stage | `pc <= pc_next` | posedge clk |
| 统一内存写 | mem_stage | `dpi_mem_write()` (mem_write 门控) | posedge clk |
| 寄存器文件 | regfile | `rf[rd] <= rd_wdata` (reg_write 门控) | posedge clk |
| CSR 寄存器组 | csr | 8 个 CSR + mcycle | posedge clk |
| halt 寄存器 | halt | halt_reg/halt_pc/halt_ret | posedge clk |

其余所有模块均为纯组合逻辑。

#### 2.11.10 数据通路逐指令追踪示例

**lw t0, 8(sp)**:

| 周期阶段 | 数据流 |
|---------|--------|
| IF | `pc`→imem读出→`instr`→id_stage |
| ID | `instr`切出opcode=0000011,funct3=010→control产mem_read=1,reg_write=1,alu_op=ADD; `rs1_addr=sp(2)`→regfile; `rd_addr=t0(5)`→regfile写口 |
| ID→EX | regfile→`rs1_rdata(sp值)`→ex_stage; imm_gen→`imm_out(8)`→ex_stage |
| EX | src1=sp值, src2=8, ALU→`alu_result=sp+8` |
| MEM | `mem_addr=alu_result`, mem[]组合读→`mem_rdata_raw`, lsu(lw)→`mem_rdata` |
| WB | `reg_wdata_src=01`→选`mem_rdata`→`reg_wdata`→regfile写口 |
| 下一拍 | posedge clk: regfile[t0]≤mem_rdata; PC≤PC+4 |

**beq t0, t1, target**:

| 周期阶段 | 数据流 |
|---------|--------|
| IF | `pc`→imem→`instr`→id_stage |
| ID | opcode=1100011, funct3=000→control产pc_sel=11,br_type=000(BEQ) |
| ID→EX | regfile→`rs1_rdata(t0)`, `rs2_rdata(t1)`→ex_stage(br_cond) |
| EX | br_cond: t0==t1?→`br_taken`→if_stage |
| IF(反馈) | `pc_sel=11`, br_taken=1→`pc_next=pc+imm_B` |
| 下一拍 | posedge clk: PC≤target地址（若成立） |

**jal ra, func**:

| 周期阶段 | 数据流 |
|---------|--------|
| IF | `pc`→imem→`instr`→id_stage |
| ID | opcode=1101111→control产reg_write=1, reg_wdata_src=10, pc_sel=01 |
| ID→IF | imm_gen→`imm_out(imm_J)`→if_stage(`imm_jal`) |
| IF | `pc_sel=01`→`pc_next=pc+imm_J`; `pc_plus4=pc+4`→wb_stage |
| WB | `reg_wdata_src=10`→选`pc_plus4`→regfile写口 |
| 下一拍 | posedge clk: regfile[ra]≤pc+4; PC≤func地址 |

**csrrw t0, mstatus, zero**（CSR 读-写指令）:

| 周期阶段 | 数据流 |
|---------|--------|
| IF | `pc`→DPI-C取指→`instr`→id_stage |
| ID | opcode=1110011, funct3=001→control产: csr_op=00, csr_read=1, csr_write=1, csr_imm=1, reg_write=1, reg_wdata_src=11 |
| ID→CSR | `csr_addr=0x300(mstatus)`→csr; `rs1_addr=zero(0)`→regfile |
| ID→EX | regfile→`rs1_rdata(zero=0)`→ex_stage (csr_src); control→`csr_op=00, csr_imm=1`→ex_stage |
| CSR→EX | csr(组合读)→`csr_rdata(mstatus当前值)`→ex_stage |
| EX | `csr_wdata = csr_src = rs1_rdata = 0` (csr_op=00, 直接写); `csr_rdata` 直通到 wb_stage |
| WB | `reg_wdata_src=11`→选`csr_rdata`→`reg_wdata`=mstatus旧值→regfile写口 |
| 下一拍 | posedge clk: regfile[t0]≤mstatus旧值; csr[mstatus]≤0 (写入新值) |

**ecall**（环境调用 → trap）:

| 周期阶段 | 数据流 |
|---------|--------|
| IF | `pc`→DPI-C取指→`instr`→id_stage |
| ID | opcode=1110011, funct3=000, funct12=0→control产: `trap_enter=1, trap_code=11` |
| core_top | trap_enter_eff=1, trap_code_eff=11→csr |
| CSR | trap_enter=1 → `mepc≤pc`(保存返回地址), `mcause≤11`, `mstatus`更新(关中断), `trap_target=mtvec` |
| IF | `pc_next = trap_target = mtvec` (最高优先级) |
| 下一拍 | posedge clk: PC≤mtvec，开始执行异常处理程序 |

**mret**（异常返回）:

| 周期阶段 | 数据流 |
|---------|--------|
| IF | `pc`→DPI-C取指→`instr`→id_stage |
| ID | funct12=0x302→control产: `mret=1` |
| core_top | mret_eff=1→csr |
| CSR | mret=1 → `mstatus`恢复(MIE←MPIE, MPP←0), `trap_target=mepc` |
| IF | `pc_next = trap_target = mepc` (最高优先级) |
| 下一拍 | posedge clk: PC≤mepc，回到 trap 前的指令继续执行 |

**中断响应**（定时器中断 MTIP）:

| 周期阶段 | 数据流 |
|---------|--------|
| C++ | timer_tick()→每10万周期 set mip.MTIP; isa_query_intr()→mstatus.MIE=1且mip.MTIP&mie.MTIE→返回0x80000007 |
| C++→RTL | `top->interrupt_valid=1, top->interrupt_cause=0x80000007` |
| core_top仲裁 | interrupt_valid=1→所有*_eff信号kill当前指令副作用; trap_enter_eff=1, trap_code_eff=0x80000007 |
| CSR | trap_enter=1→mepc≤pc(保存当前PC), mcause≤0x80000007, mstatus更新(关中断), mip.MTIP清零, trap_target=mtvec |
| IF | `pc_next = trap_target = mtvec`→跳转到中断向量 |
| 下一拍 | PC≤mtvec, 中断处理程序开始执行 |

---

## 三、阶段 2：扩展指令（RV32I 完整指令集 ✅ 已实现）

> **状态**：已完成。
> **内容**：RV32I 全部 37 条指令（含 ecall/ebreak/mret + CSR 指令），覆盖 R/I/S/B/U/J 六种格式。
> 对比阶段 1 新增：sll, slt, sltu, xor, or, and, slli, srli, srai, slti, sltiu, xori, ori, andi, lb, lh, lbu, lhu, sb, sh, blt, bge, bltu, bgeu, srl, sra, auipc。

### 3.1 RV32I 指令全表（37 条）

> 按 opcode 分组，★ 标记已在阶段 1 实现，其余为阶段 2 扩展。

#### 3.1.1 R 型指令（opcode = `0110011`）—— 10 条

| 指令 | funct3 | funct7 | alu_op | 说明 |
|------|:------:|:------:|:------:|------|
| add ★ | 000 | 0000000 | 0_0001 | 加法 |
| sub ★ | 000 | 0100000 | 0_0000 | 减法（funct7[5]=1） |
| sll | 001 | 0000000 | 0_0010 | 逻辑左移 |
| slt | 010 | 0000000 | 0_0011 | 有符号小于置位 |
| sltu | 011 | 0000000 | 0_0100 | 无符号小于置位 |
| xor | 100 | 0000000 | 0_0101 | 按位异或 |
| srl | 101 | 0000000 | 0_1000 | 逻辑右移 |
| sra | 101 | 0100000 | 0_1001 | 算术右移（funct7[5]=1） |
| or | 110 | 0000000 | 0_0110 | 按位或 |
| and | 111 | 0000000 | 0_0111 | 按位与 |

> **控制信号**：`reg_write=1`, `alu_src_a=0`, `alu_src_b=00`，`alu_op` 由 funct3+funct7 查表。

#### 3.1.2 I 型算术指令（opcode = `0010011`）—— 9 条

| 指令 | funct3 | funct7 | alu_op | 说明 |
|------|:------:|:------:|:------:|------|
| addi ★ | 000 | — | 0_0001 | 加立即数 |
| slli | 001 | 0000000 | 0_0010 | 逻辑左移立即数 |
| slti | 010 | — | 0_0011 | 有符号小于立即数置位 |
| sltiu | 011 | — | 0_0100 | 无符号小于立即数置位 |
| xori | 100 | — | 0_0101 | 异或立即数 |
| srli | 101 | 0000000 | 0_1000 | 逻辑右移立即数 |
| srai | 101 | 0100000 | 0_1001 | 算术右移立即数（funct7[5]=1） |
| ori | 110 | — | 0_0110 | 或立即数 |
| andi | 111 | — | 0_0111 | 与立即数 |

> **控制信号**：`reg_write=1`, `alu_src_a=0`, `alu_src_b=01`（选 imm），`alu_op` 由 funct3+funct7 查表。
> **slli/srli/srai 的移位量**：取 imm 的低 5 位（`instr[24:20]` 即 shamt 字段）。

#### 3.1.3 I 型 Load 指令（opcode = `0000011`）—— 5 条

| 指令 | funct3 | lsu_type | Load 行为 |
|------|:------:|:--------:|------|
| lw ★ | 010 | 000 | 32位字，直接透传 |
| lh | 001 | 001 | 16位半字 → 有符号扩展到 32 位 |
| lb | 000 | 010 | 8位字节 → 有符号扩展到 32 位 |
| lhu | 101 | 011 | 16位半字 → 零扩展到 32 位 |
| lbu | 100 | 100 | 8位字节 → 零扩展到 32 位 |

> **控制信号**：`reg_write=1`, `mem_read=1`, `alu_src_b=01`, `reg_wdata_src=01`（选 MEM）, `alu_op=ADD`。
> **数据路径**：`rs1 + imm → ALU → mem_addr → LSU → mem_rdata → WB → regfile`

#### 3.1.4 S 型 Store 指令（opcode = `0100011`）—— 3 条

| 指令 | funct3 | lsu_type | wmask |
|------|:------:|:--------:|------|
| sw ★ | 010 | 101 | 1111（全字） |
| sh | 001 | 110 | 0011/1100（半字，addr[1]选择） |
| sb | 000 | 111 | 0001 << addr[1:0]（单字节） |

> **控制信号**：`mem_write=1`, `alu_src_b=01`, `alu_op=ADD`。`reg_write=0`（不写寄存器）。

#### 3.1.5 B 型分支指令（opcode = `1100011`）—— 6 条

| 指令 | funct3 | br_type | 条件 |
|------|:------:|:------:|------|
| beq ★ | 000 | 000 | rs1 == rs2 |
| bne ★ | 001 | 001 | rs1 != rs2 |
| blt | 100 | 010 | signed rs1 < rs2 |
| bge | 101 | 011 | signed rs1 >= rs2 |
| bltu | 110 | 100 | unsigned rs1 < rs2 |
| bgeu | 111 | 101 | unsigned rs1 >= rs2 |

> **控制信号**：`pc_sel=11`（分支路径），`br_type` 由 funct3 查表。
> **注意 funct3 到 br_type 的映射**：B 型指令的 funct3 ≠ br_type，control.v 做翻译（例如 blt 的 funct3=100 → br_type=010）。

#### 3.1.6 U 型指令（opcode = `0110111`/`0010111`）—— 2 条

| 指令 | opcode | alu_op | 行为 |
|------|:------:|:------:|------|
| lui ★ | 0110111 | 0_1010 (LUI) | `rd = imm_U`，ALU 透传 src2（imm_U 已在 imm_gen 左移 12 位） |
| auipc | 0010111 | 0_0001 (ADD) | `rd = pc + imm_U` |

> **lui 控制信号**：`reg_write=1`, `alu_src_b=01`, `alu_op=LUI`（透传 imm）。
> **auipc 控制信号**：`reg_write=1`, `alu_src_a=1`（选 PC）, `alu_src_b=01`, `alu_op=ADD`。

#### 3.1.7 J 型指令（opcode = `1101111`）—— 1 条

| 指令 | pc_sel | reg_wdata_src | 行为 |
|------|:------:|:------------:|------|
| jal ★ | 01 | 10 | `rd = pc+4`, `pc = pc + imm_J` |

> **jal 不需 ALU**：`pc_sel=01` 走 if_stage 内部 `pc+imm_J` 路径，返回地址 `pc+4` 直接送 wb_stage 写回。

#### 3.1.8 I 型 jalr 指令（opcode = `1100111`）—— 1 条

| 指令 | pc_sel | reg_wdata_src | 行为 |
|------|:------:|:------------:|------|
| jalr ★ | 10 | 10 | `rd = pc+4`, `pc = (rs1 + imm_I) & ~1` |

> **jalr 必须走 ALU**：跳转目标 = `rs1 + imm_I`（含寄存器操作数），ALU 算出后送 if_stage 做 `&~1` 对齐。返回地址 `pc+4` 直通 WB。

### 3.2 指令与立即数格式对照

| opcode[6:0] | 格式 | 指令类别 | 指令数 |
|:----------:|:----:|---------|:----:|
| `0110011` | R型 | 整数寄存器-寄存器 | 10 |
| `0010011` | I型 | 整数立即数算术 | 9 |
| `0000011` | I型 | Load | 5 |
| `1100111` | I型 | jalr | 1 |
| `1110011` | I型 | 系统（ecall/ebreak/CSR*/mret） | 9 |
| `0100011` | S型 | Store | 3 |
| `1100011` | B型 | 分支 | 6 |
| `0110111` | U型 | lui | 1 |
| `0010111` | U型 | auipc | 1 |
| `1101111` | J型 | jal | 1 |

### 3.3 阶段 2 设计要点

**Q1: slli/srli/srai 的移位量从哪里取？**
- 取自 `instr[24:20]`（即 shamt 字段），imm_gen 把它符号扩展到 32 位
- ALU 用 `src2[4:0]` 截取低 5 位

**Q2: srl 和 sra 都是 funct3=101，怎么区分？**
- funct7[5] 区分：`funct7[5]=0` → srl（逻辑右移），`funct7[5]=1` → sra（算术右移）
- 与 add/sub 的区分方式完全一致

**Q3: B 型指令的 funct3 为什么和 br_type 不一致？**
- B 型 funct3 编码：beq=000, bne=001, blt=100, bge=101, bltu=110, bgeu=111
- br_type 编码：BEQ=000, BNE=001, BLT=010, BGE=011, BLTU=100, BGEU=101
- 差异是设计选择：br_type 将条件按"等于/不等/有符号小于/有符号大于等于/无符号小于/无符号大于等于"连续排列

**Q4: auipc 为什么 alu_src_a=1？**
- auipc 的 ALU 需要做 `pc + imm_U`，src1 取 pc 而非 rs1

---

## 四、阶段 3：特权与异常（CSR + trap + 中断 ✅ 已实现）

> **状态**：已完成。
> **内容**：ecall/ebreak/mret + 6 条 CSR 指令（csrrw/csrrs/csrrc/csrrwi/csrrsi/csrrci）+ csr.v 硬件模块 + 中断（定时器 + 外部中断）。

### 4.1 csr — 控制状态寄存器模块

| 属性 | 值 |
|------|-----|
| 文件 | `npc/vsrc/csr.v` |
| 类型 | 时序逻辑（CSR 寄存器组）+ 组合读 |
| 顶层位置 | core_top 独立模块 |

#### 4.1.1 基本信息 & 端口表

| 方向 | 信号 | 位宽 | 说明 |
|:--:|------|:--:|------|
| I | `clk` | 1 | 时钟 |
| I | `rst` | 1 | 复位（初始化 CSR 为默认值） |
| I | `csr_addr` | 12 | CSR 地址（来自 instr[31:20]） |
| I | `csr_write` | 1 | CSR 写使能 |
| I | `csr_wdata` | 32 | CSR 写数据（来自 ex_stage） |
| I | `csr_read` | 1 | CSR 读使能 |
| I | `mret` | 1 | mret 指令触发 |
| I | `trap_pc` | 32 | 陷阱入口时的 PC（用于保存到 mepc） |
| I | `trap_code` | 32 | 陷阱原因编码（11=ecall, 3=ebreak, 0x80000007=定时器中断, 0x8000000b=外部中断） |
| I | `trap_enter` | 1 | 陷阱进入信号 |
| O | `csr_rdata` | 32 | CSR 读数据（→ ex_stage → WB MUX → regfile） |
| O | `trap_target` | 32 | 陷阱目标地址（trap→mtvec, mret→mepc, 其他→0） |

#### 4.1.2 CSR 寄存器清单

| CSR | 地址 | 位宽 | 复位值 | 说明 |
|-----|:----:|:----:|--------|------|
| `mstatus` | 0x300 | 32 | `0x00001800` | 机器状态（MIE/MPIE/MPP） |
| `mtvec` | 0x305 | 32 | `0x80000000` | 陷阱向量基址 |
| `mepc` | 0x341 | 32 | 0 | 陷阱返回地址（保存 trap 时的 PC） |
| `mcause` | 0x342 | 32 | 0 | 陷阱原因（11=ecall, 3=ebreak, 高位=中断标志） |
| `mip` | 0x344 | 32 | 0 | 中断挂起（bit7=MTIP, bit11=MEIP） |
| `mie` | 0x304 | 32 | 0 | 中断使能（bit7=MTIE, bit11=MEIE） |
| `mtval` | 0x343 | 32 | 0 | 陷阱值（异常附加信息） |
| `mscratch` | 0x340 | 32 | 0 | 机器暂存寄存器 |
| `mcycle` | 0xB00 | 64 | 0 | 周期计数器（低32位 0xB00，高32位 0xB80） |

#### 4.1.3 CSR 关键位域

```
mstatus[31:0]:
  [12:11] = MPP  (Machine Previous Privilege, 2bit)
  [7]     = MPIE (Machine Previous Interrupt Enable)
  [3]     = MIE  (Machine Interrupt Enable)

mcause[31:0]:
  [31]    = Interrupt (1=中断, 0=异常)
  [30:0]  = Exception Code (3=ebreak, 11=ecall_M)

mip[31:0]:
  [11]    = MEIP (Machine External Interrupt Pending)
  [7]     = MTIP (Machine Timer Interrupt Pending)
```

#### 4.1.4 Trap 进入流程（硬件自动）

```
trap_enter 触发时（下一拍 posedge clk）：
  1. mcause  ← trap_code      （记录原因）
  2. mepc    ← trap_pc        （保存 trap 时的 PC）
  3. mstatus.MPP  ← 3         （进入 M-mode）
  4. mstatus.MPIE ← mstatus.MIE
  5. mstatus.MIE  ← 0         （关中断）
  6. 中断响应时：清除 mip 中对应 pending 位
```

#### 4.1.5 MRET 返回流程（硬件自动）

```
mret 触发时（下一拍 posedge clk）：
  1. mstatus.MPP  ← 0          （回到低特权级）
  2. mstatus.MPIE ← 1
  3. mstatus.MIE  ← mstatus.MPIE （恢复中断使能）
  4. PC ← mepc                 （跳回 trap 前的地址）
```

#### 4.1.6 trap_target 输出逻辑

```verilog
assign trap_target = trap_enter ? csr_mtvec :   // 异常/中断 → mtvec
                     mret       ? csr_mepc  :   // mret 返回 → mepc
                     32'b0;                      // 其他
```

### 4.2 CSR 指令集（6 条）

#### 4.2.1 控制信号

> control.v 新增输出信号，在 `opcode=1110011` 下按 funct3 分发。

| 新增信号 | 位宽 | 说明 |
|---------|:--:|------|
| `csr_op` | 2 | CSR 操作类型：00=csrrw/wi（直接写）, 01=csrrs/si（置位）, 10=csrrc/ci（清除） |
| `csr_read` | 1 | 读 CSR 使能 |
| `csr_write` | 1 | 写 CSR 使能 |
| `csr_imm` | 1 | 源操作数来源：1=rs1值, 0=zimm(rs1_addr 零扩展) |
| `csr_addr` | 12 | CSR 地址 = instr[31:20] |
| `csr_zimm` | 32 | 零扩展立即数 = {27'b0, instr[19:15]} |

#### 4.2.2 指令表

| 指令 | funct3 | csr_op | csr_imm | csr_write 特殊条件 |
|------|:------:|:------:|:-------:|------------------|
| csrrw | 001 | 00（写） | 1（rs1） | 无条件=1 |
| csrrs | 010 | 01（置位） | 1（rs1） | rs1≠0 时才写 |
| csrrc | 011 | 10（清除） | 1（rs1） | rs1≠0 时才写 |
| csrrwi | 101 | 00（写） | 0（zimm） | 无条件=1 |
| csrrsi | 110 | 01（置位） | 0（zimm） | zimm≠0 时才写 |
| csrrci | 111 | 10（清除） | 0（zimm） | zimm≠0 时才写 |

> **关键特性**：CSR 指令同时写 CSR 和寄存器：`csr_read=1`（读旧 CSR 值到 rd），`csr_write=1`（新值写入 CSR），`reg_write=1`，`reg_wdata_src=11`（选择 CSR 读数据）。

#### 4.2.3 ex_stage 中的 CSR 写数据生成

```verilog
// ex_stage.v 新增逻辑
wire [31:0] csr_src = csr_imm ? rs1_rdata : csr_zimm;
always @(*) begin
    case (csr_op)
        2'b00 : csr_wdata = csr_src;               // csrrw / csrrwi: 直接写
        2'b01 : csr_wdata = csr_rdata |  csr_src;  // csrrs / csrrsi: 置位
        2'b10 : csr_wdata = csr_rdata & ~csr_src;  // csrrc / csrrci: 清除
        default: csr_wdata = 32'b0;
    endcase
end
```

### 4.3 ecall / ebreak / mret

#### 4.3.1 control.v 中的系统指令分发（opcode=`1110011`, funct3=`000`）

| funct12 | 指令 | 触发信号 | 行为 |
|--------|------|---------|------|
| `0x000` | ecall | `trap_enter=1`, `trap_code=11` | 环境调用：触发异常进入 M-mode |
| `0x001` | ebreak | `is_ebreak=1` | 断点：触发 halt 停止仿真 |
| `0x302` | mret | `mret=1` | 异常返回：从 mepc 恢复 PC |

#### 4.3.2 ebreak 的 halt 处理

```
ebreak 执行流程：
  id_stage → is_ebreak=1
  core_top → is_ebreak_eff（中断 kill 后）
  halt.v   → 下一拍 posedge: halt_reg≤1, halt_pc≤pc, halt_ret≤rs_a0 (x10=a0)
  main.cpp → halt_check() 检测 top->halt→NPC_END
```

### 4.4 halt — 仿真停止模块

| 属性 | 值 |
|------|-----|
| 文件 | `npc/vsrc/halt.v` |
| 类型 | 时序逻辑 |

#### 4.4.1 端口表

| 方向 | 信号 | 位宽 | 说明 |
|:--:|------|:--:|------|
| I | `clk` | 1 | 时钟 |
| I | `rst` | 1 | 复位 |
| I | `is_ebreak` | 1 | ebreak 指令标志（经 interrupt kill） |
| I | `pc` | 32 | 当前 PC |
| I | `rs_a0` | 32 | 寄存器 a0 (x10) 值，作为退出码 |
| O | `halt_reg` | 1 | halt 标志（→ C++ 检测） |
| O | `halt_pc` | 32 | halt 时的 PC |
| O | `halt_ret` | 32 | halt 退出码 = a0 值 |

> **退出码约定**：`halt_ret=0` 表示测试通过，`≠0` 表示测试失败。

### 4.5 中断系统

#### 4.5.1 中断类型

| 中断 | 编号 (mcause) | mip 位 | mie 位 | 触发条件 |
|------|:-----------:|:------:|:------:|------|
| 定时器中断 | `0x80000007` | bit7 (MTIP) | bit7 (MTIE) | 每 100000 周期定时器 tick |
| 外部中断 | `0x8000000b` | bit11 (MEIP) | bit11 (MEIE) | 按需设置 mip.MEIP |

> **全局中断使能**：`mstatus.MIE` (bit3) = 1 时才响应任何中断。

#### 4.5.2 C++ 侧中断处理流程（interrupt.cpp）

```
interrupt_check() 每周期调用：
  1. timer_tick()：每 100000 周期 set mip.MTIP
  2. isa_query_intr()：
     - 检查 mstatus.MIE 是否使能
     - 检查 mip.MTIP && mie.MTIE → 返回 0x80000007
     - 检查 mip.MEIP && mie.MEIE → 返回 0x8000000b
  3. 有中断 → top->interrupt_valid=1, top->interrupt_cause=trap_cause
```

#### 4.5.3 RTL 侧中断注入（core_top）

```verilog
// 中断到来时：kill 当前指令的所有副作用
assign mem_read_eff     = interrupt_valid ? 1'b0 : mem_read;     // 不读内存
assign mem_write_eff    = interrupt_valid ? 1'b0 : mem_write;    // 不写内存
assign reg_write_eff    = interrupt_valid ? 1'b0 : reg_write;    // 不写寄存器
assign csr_write_eff    = interrupt_valid ? 1'b0 : csr_write;    // 不写 CSR
assign mret_eff         = interrupt_valid ? 1'b0 : mret;         // 不执行 mret
assign trap_enter_eff   = trap_enter      | interrupt_valid;     // 合并 trap + 中断
assign trap_code_eff    = interrupt_valid ? interrupt_cause : trap_code;
```

> **阀门放大模型**：中断是最高优先级的阀门——任何当前指令的所有写操作都被 kill，强制跳转到 mtvec。

#### 4.5.4 中断处理流程图

```
C++ interrupt_check()
        │
        ▼
  有中断？── 否 ──→ 正常执行
        │
       是 ▼
  top->interrupt_valid=1
  top->interrupt_cause=trap_code
        │
        ▼
  core_top 层 kill 当前指令
  trap_enter_eff=1, trap_code_eff=中断号
        │
        ▼
  if_stage: pc_next = trap_target (=mtvec)
  csr: 保存 mepc/mcause, 更新 mstatus
        │
        ▼
  下一拍：PC 跳转到 mtvec 执行中断处理程序
  中断处理程序结束后执行 mret 返回
```

### 4.6 core_top 模块更新 —— 中断支持

> 相比阶段 1，core_top 新增了 interrupt 相关的输入端口和 *_eff 有效控制信号命名。

#### 4.6.1 新增输入端口

| 方向 | 信号 | 位宽 | 说明 |
|:--:|------|:--:|------|
| I | `interrupt_valid` | 1 | C++ 侧注入的中断有效标志 |
| I | `interrupt_cause` | 32 | 中断原因编码 |

#### 4.6.2 新增输出端口

| 方向 | 信号 | 位宽 | 说明 |
|:--:|------|:--:|------|
| O | `halt` | 1 | ebreak 触发停止 |
| O | `halt_pc` | 32 | 停止时 PC |
| O | `halt_ret` | 32 | 停止时返回值（a0） |

#### 4.6.3 新增子模块例化

| 模块 | 例化名 | 功能 |
|------|--------|------|
| `csr.v` | `u_csr` | CSR 寄存器 + trap 控制 |
| `halt.v` | `u_halt` | ebreak 检测 + 停止 |

### 4.7 wb_stage 更新 —— 四选一 MUX

> 阶段 3 新增 `csr_rdata` 输入，写回 MUX 从 3 选 1 升级为 4 选 1。

| reg_wdata_src | 选择 | 说明 |
|:-----------:|------|------|
| `00` | `alu_result` | ALU 结果（R/I/U 型指令） |
| `01` | `mem_rdata` | 内存加载数据（Load 指令） |
| `10` | `pc_plus4` | 返回地址（jal/jalr） |
| `11` | `csr_rdata` | CSR 读数据（CSR 指令） |

### 4.8 regfile 更新 —— rs_a0 输出

> 为支持 halt 退出码，regfile 新增 `rs_a0` 输出端口。

| 方向 | 信号 | 位宽 | 说明 |
|:--:|------|:--:|------|
| O | `rs_a0` | 32 | 始终输出 `rf[10]`（x10/a0 寄存器的值） |

### 4.9 control.v 新增输出信号汇总

| 信号 | 位宽 | 默认值 | 去向 | 说明 |
|------|:--:|:----:|------|------|
| `csr_op` | 2 | 00 | ex_stage | CSR 操作类型 |
| `csr_read` | 1 | 0 | csr | 读 CSR 使能 |
| `csr_write` | 1 | 0 | csr | 写 CSR 使能 |
| `csr_imm` | 1 | 0 | ex_stage | CSR 源操作数选择 |
| `csr_addr` | 12 | — | csr | CSR 地址（wire assign） |
| `csr_zimm` | 32 | — | ex_stage | 零扩展立即数（wire assign） |
| `trap_enter` | 1 | 0 | csr+if_stage | 异常/中断进入 |
| `trap_code` | 32 | 0 | csr | 异常原因 |
| `is_ebreak` | 1 | 0 | halt | ebreak 标志 |
| `mret` | 1 | 0 | csr+if_stage | mret 标志 |

### 4.10 阶段 3 审查记录

| 检查项 | 结果 |
|--------|:--:|
| csr.v 8 个 CSR 寄存器 | ✅ |
| trap_enter 硬件自动保存 | ✅ |
| mret 硬件自动恢复 | ✅ |
| 6 条 CSR 指令控制信号 | ✅ |
| csrrsi/csrrs csr_write 门控（源=0 不写） | ✅ |
| ebreak→halt 链路 | ✅ |
| 中断 kill 当前指令副作用 | ✅ |
| 中断响应清除 mip 对应位 | ✅ |
| mcycle 自增 | ✅ |
| wb_stage 升级为 4 选 1 | ✅ |
| regfile 新增 rs_a0 端口 | ✅ |
| core_top 中断端口 | ✅ |

---

## 五、C++ 仿真环境（csrc 全景）

> RTL 是 CPU 核心，C++ 侧提供仿真环境：统一内存、外设、difftest、中断、itrace。

### 5.1 文件与模块映射

| 文件 | 模块 | 功能 |
|------|------|------|
| `csrc/main.cpp` | NPC 主循环 | `single_cycle()` 驱动 clk，集成中断/difftest/itrace/halt |
| `csrc/memory/memory.cpp` | 统一内存 + MMIO | DPI-C 读写 PMEM (128MB)，UART/RTC 外设 |
| `csrc/monitor/monitor.cpp` | 监控层 | 参数解析，初始化，halt 检测，状态打印 |
| `csrc/monitor/interrupt.cpp` | 中断控制器 | 定时器 tick，中断查询，中断注入 |
| `csrc/difftest/dut.cpp` | 差分测试 | NEMU 参考模型对比，PC/GPR/CSR 全量检查 |
| `csrc/device/serial.cpp` | UART 串口 | `npc_serial_putc()` 输出到 stderr |
| `csrc/monitor/sdb/*.cpp` | 简易调试器 | 表达式求值，watchpoint，寄存器/内存查看 |
| `csrc/monitor/trace/*.cpp` | 指令追踪 | itrace 记录每条指令，反汇编输出 |

### 5.2 DPI-C 接口

```verilog
// dpi_imports.vh
import "DPI-C" function int  dpi_mem_read(input int addr, input int is_load);
import "DPI-C" function void dpi_mem_write(input int addr, input int data, input int wmask);
```

| 函数 | 调用者 | 说明 |
|------|--------|------|
| `dpi_mem_read(addr, 0)` | if_stage | 取指：`is_load=0`，不触发 MMIO skip |
| `dpi_mem_read(addr, mem_read)` | mem_stage | 数据读：`is_load=1` 时触发 MMIO |
| `dpi_mem_write(addr, data, wmask)` | mem_stage | 数据写：按字节掩码写入 |

### 5.3 内存映射

| 地址范围 | 大小 | 用途 |
|---------|:----:|------|
| `0x80000000` ~ `0x87FFFFFF` | 128MB | 物理内存（PMEM），仿真时加载 .bin |
| `0xA00003F8` | 1B | UART 串口（只写，输出到 stderr） |
| `0xA0000048` | 4B | RTC 低 32 位（读主机时间 us） |
| `0xA000004C` | 4B | RTC 高 32 位 |

### 5.4 single_cycle 执行流程

```
single_cycle() 每周期：
  1. interrupt_check()       — 检查并注入中断
  2. 保存 npc_cur_pc, this_inst
  3. top->clk=1, eval()      — 上升沿：PC 更新，regfile 写，mem 写
  4. top->interrupt_valid=0   — 清除中断脉冲
  5. halt_check()             — 检测 ebreak
  6. difftest_step()          — 与 NEMU 对比（非 MMIO 周期）
  7. itrace_log()             — 记录指令
  8. top->clk=0, eval()      — 下降沿：组合逻辑稳定
```

### 5.5 Verilator RTL 访问器（npc.h）

| 访问器 | 读取信号 |
|--------|---------|
| `npc_pc(top, 0, READ)` | `core_top__DOT__pc` |
| `npc_gpr(top, i, 0, READ)` | `core_top__DOT__u_regfile__DOT__rf[i]` |
| `npc_csr(top, CSR_*, 0, READ)` | `core_top__DOT__u_csr__DOT__csr_*` |
| `npc_npc(top, 0, READ)` | `core_top__DOT__u_if_stage__DOT__pc_next` |

---

## 附录 C：全部模块文件一览

| 文件 | 模块 | 类型 | 代码行 |
|------|------|:--:|:----:|
| `vsrc/core_top.v` | core_top | 顶层连线 | ~130 |
| `vsrc/if_stage.v` | if_stage | 时序(PC)+组合 | ~50 |
| `vsrc/id_stage.v` | id_stage | 纯组合 | ~65 |
| `vsrc/ex_stage.v` | ex_stage | 纯组合 | ~55 |
| `vsrc/mem_stage.v` | mem_stage | 时序(写)+组合 | ~40 |
| `vsrc/wb_stage.v` | wb_stage | 纯组合 | ~15 |
| `vsrc/control.v` | control | 纯组合 | ~250 |
| `vsrc/alu.v` | alu | 纯组合 | ~35 |
| `vsrc/br_cond.v` | br_cond | 纯组合 | ~20 |
| `vsrc/imm_gen.v` | imm_gen | 纯组合 | ~35 |
| `vsrc/lsu.v` | lsu | 纯组合 | ~70 |
| `vsrc/regfile.v` | regfile | 时序(写)+组合(读) | ~25 |
| `vsrc/csr.v` | csr | 时序 | ~95 |
| `vsrc/halt.v` | halt | 时序 | ~20 |
| `vsrc/dpi_imports.vh` | — | DPI-C 声明 | ~10 |
| `csrc/main.cpp` | 主循环 | C++/Verilator | ~80 |
| `csrc/memory/memory.cpp` | 统一内存+MMIO | C++/DPI-C | ~110 |
| `csrc/monitor/monitor.cpp` | 监控层 | C++ | ~80 |
| `csrc/monitor/interrupt.cpp` | 中断控制器 | C++ | ~60 |
| `csrc/difftest/dut.cpp` | 差分测试 | C++ | ~110 |
| `csrc/device/serial.cpp` | UART | C++ | ~5 |

---

## 附录 D：信号编码速查总表

| 信号 | 位宽 | 编码 | 去向 |
|------|:--:|------|------|
| `alu_op` | 5 | 0_0000=SUB, 0_0001=ADD, 0_0010=SLL, 0_0011=SLT, 0_0100=SLTU, 0_0101=XOR, 0_0110=OR, 0_0111=AND, 0_1000=SRL, 0_1001=SRA, 0_1010=LUI | EX |
| `alu_src_a` | 1 | 0=rs1, 1=pc | EX |
| `alu_src_b` | 2 | 00=rs2, 01=imm, 10=4 | EX |
| `br_type` | 3 | 000=BEQ, 001=BNE, 010=BLT, 011=BGE, 100=BLTU, 101=BGEU | EX |
| `lsu_type` | 3 | 000=LW, 001=LH, 010=LB, 011=LHU, 100=LBU, 101=SW, 110=SH, 111=SB | MEM |
| `pc_sel` | 2 | 00=+4, 01=jal, 10=jalr, 11=branch | IF |
| `reg_wdata_src` | 2 | 00=ALU, 01=MEM, 10=PC+4, 11=CSR | WB |
| `csr_op` | 2 | 00=WRITE, 01=SET, 10=CLEAR | EX |
| `csr_imm` | 1 | 0=zimm, 1=rs1 | EX |
| `trap_code` | 32 | 11=ecall, 3=ebreak, 0x80000007=timer, 0x8000000b=external | CSR |

| 资料 | 路径 |
|------|------|
| RISC-V 指令集笔记 | `nemu/study/study-notes/riscv.md` |
| Verilog 学习笔记 | `nemu/study/study-notes/rtl_notes.md` |
| NEMU RISC-V 译码源码 | `nemu/src/isa/riscv32/inst.c` |
| NPC 架构图 (Mermaid) | `npc/notes/stage.mmd` |
| NPC 数据通路图 (Mermaid) | `npc/notes/data.mmd` |
| NPC 设计指南 | `npc/notes/npc.md` |
| 控制信号手册 | `npc/notes/stage.md` |
