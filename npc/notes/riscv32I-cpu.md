# NPC 单周期 RV32I 设计笔记

> 按开发阶段组织的完整设计记录。
> 打开方式：`Ctrl+Shift+V` 预览，点击目录链接跳转。

---

## 目录

- [一、阶段 0：基础设施（独立模块）](#一阶段-0基础设施独立模块)
  - [1.1 总览](#11-总览)
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
  - [2.1 架构总览：5 Stage + regfile](#21-架构总览5-stage--regfile)
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
- [三、阶段 2：扩展指令](#三阶段-2扩展指令rv32i-剩余) 🔜
- [四、阶段 3：特权与异常](#四阶段-3特权与异常csr) 🔜
- [附录：参考资料](#附录参考资料)

---

## 一、阶段 0：基础设施（独立模块）

### 1.1 总览

五个独立模块的数据流关系：

```
instr[31:0] ──┬──→ imm_gen ──→ imm_out 
              ├──→ regfile (读口) ──→ rdata1/rdata2
              │       ↑
              │    (写口来自 WB)
              └──→ control ──→ 10组控制信号
                              │
         ┌────────────────────┘
         ↓
    ┌─────────┐    ┌──────────┐
    │ alu.v   │    │ br_cond  │
    └────┬────┘    └────┬─────┘
    alu_result      br_taken
         │
    ┌────┴────┐
    │ lsu.v   │
    └────┬────┘
     mem_rdata
```

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
| 2 | 时序逻辑用 `always @(posedge clk)` + `<=` | regfile(写口), if_stage(PC) |
| 3 | 不需要的端口不加 (clk/en/rst) | alu, imm_gen, br_cond, lsu, control |
| 4 | `instr[a:b]` 是 wire 切片，零成本 | id_stage 整体 |
| 5 | RISC-V 编码只在 control.v 内接触 | 其他模块用自定义编码 |
| 6 | `case` 和并行 `assign`+MUX 综合结果等价 | 风格自由 |
| 7 | `default` 设 0，禁止 X 态传播 | 所有 case |
| 8 | 控制信号默认值 = 安全空操作 | reg_write=0, mem_write=0, pc_sel=00 |

---

## 二、阶段 1：单周期核心（stage 集成 + core_top）

### 2.1 架构总览：5 Stage + regfile

```
              ┌───────────────────────┐
              │    regfile.v (独立)    │
              │  读口(组合) 写口(时序)  │
              └──┬──────────┬─────────┘
         读地址  │  rdata   │  reg_wdata
                 ▼          │  + reg_write
  ┌──────┐  ┌──────┐  ┌──────┐  ┌──────┐  ┌──────┐
  │  IF  │─→│  ID  │─→│  EX  │─→│ MEM  │─→│  WB  │
  └──┬───┘  └──────┘  └──┬───┘  └──────┘  └──┬───┘
     │        ↑           │                    │
     │        └─── br_taken / alu_result ──────┘
     │                     └─── alu_result ────→(MEM)
     └─── pc_plus4 ─────────────────────────────→(WB)
```

| 模块 | 文件 | 内部子模块 | 类型 |
|------|------|---------|------|
| IF | `if_stage.v` | PC寄存器 + PC选择MUX + IMEM[64K] | 时序(PC) + 组合 |
| ID | `id_stage.v` | 字段切片 + imm_gen + control | 纯组合 |
| EX | `ex_stage.v` | ALU源MUX + alu + br_cond | 纯组合 |
| MEM | `mem_stage.v` | 统一内存 MEM[64K] + lsu | 时序(写) + 组合 |
| WB | `wb_stage.v` | 写回MUX 3选1 | 纯组合 |
| RF | `regfile.v` | 32×32 寄存器文件 | 时序(写) + 组合(读) |

> **注意**：regfile 从 id_stage 内部移至 core_top 层独立模块，ID 接读口、WB 接写口。
> 单周期中唯一两处时序元件：if_stage 的 PC 寄存器和 mem_stage 的 mem[] 写口 + regfile 写口。

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
                   └──────────┘
```

| 方向 | 信号 | 位宽 | 说明 |
|:--:|------|:--:|------|
| I | `opcode` | 7 | instr[6:0] |
| I | `funct3` | 3 | instr[14:12] |
| I | `funct7` | 7 | instr[31:25] |
| O | `alu_op` | 5 | ALU 操作码 |
| O | `alu_src_a` | 1 | ALU 源A选择：0=rs1, 1=pc |
| O | `alu_src_b` | 2 | ALU 源B选择：00=rs2, 01=imm, 10=4 |
| O | `br_type` | 3 | 分支类型：000=BEQ, 001=BNE, 010=BLT, 011=BGE, 100=BLTU, 101=BGEU |
| O | `mem_read` | 1 | 读内存使能 |
| O | `mem_write` | 1 | 写内存使能 |
| O | `lsu_type` | 3 | 访存类型：000=LW, 001=LH, 010=LB, 011=LHU, 100=LBU, 101=SW, 110=SH, 111=SB |
| O | `reg_write` | 1 | 写寄存器使能 |
| O | `reg_wdata_src` | 2 | 写回来源：00=ALU, 01=MEM, 10=PC+4 |
| O | `pc_sel` | 2 | PC选择：00=+4, 01=jal, 10=jalr, 11=branch |

#### 2.2.2 默认值设计

```verilog
alu_op        = ADD      // 无害计算，结果被 reg_write=0 丢弃
alu_src_a     = 0        // rs1
alu_src_b     = 00       // rs2
br_type       = 000      // BEQ, 但 pc_sel=00 忽略
mem_read      = 0        // 不读内存
mem_write     = 0        // 不写内存（关键阀门）
lsu_type      = 000      // LW, 但 mem_read=0 门控
reg_write     = 0        // 不写寄存器（关键阀门）
reg_wdata_src = 00       // ALU
pc_sel        = 00       // PC+4（关键阀门）
```

> 三道阀门：`reg_write=0`、`mem_write=0`、`pc_sel=00`。它们保证了未知指令不会修改CPU状态。

#### 2.2.3 指令分发总表（按指令类型逐信号列出）

> 每条指令只需覆写与默认值不同的信号，其余靠默认值。
> 首批 ⭐ 10条：addi, lui, add, sub, beq, bne, jal, jalr, lw, sw

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

> **注意**：jal 不需要 `alu_op`/`alu_src_a`/`alu_src_b`，`reg_wdata_src=10` 直接选 pc_plus4 线。if_stage 内部算 `pc+imm_J`，不消耗 ALU。jalr 必须走 ALU（op1 是寄存器值 rs1）。

#### 2.2.4 审查记录

| 检查项 | 结果 |
|--------|:--:|
| 10组信号默认值 | ✅ |
| R/I算术/I Load/S Store/B Branch 覆写 | ✅ |
| jal 不需ALU（pc_plus4直通） | ✅ |
| jalr 需ALU（rs1+imm） | ✅ |
| auipc alu_src_a=1（选PC） | ✅ |
| srli/srai 用 funct7[5] 区分 | ✅ |

#### 2.2.5 各信号完整编码速查

| 信号 | 位宽 | 编码值 |
|------|:--:|------|
| `alu_src_a` | 1 | 0 = rdata1（rs1 寄存器值）, 1 = pc |
| `alu_src_b` | 2 | 00 = rdata2（rs2 寄存器值）, 01 = imm（立即数）, 10 = 4（常数） |
| `reg_wdata_src` | 2 | 00 = ALU 结果, 01 = MEM 数据, 10 = PC+4 |
| `pc_sel` | 2 | 00 = PC+4, 01 = jal(pc+imm_J), 10 = jalr(ALU结果), 11 = branch |
| `br_type` | 3 | 000 = BEQ, 001 = BNE, 010 = BLT, 011 = BGE, 100 = BLTU, 101 = BGEU |
| `lsu_type` | 3 | 000 = LW, 001 = LH, 010 = LB, 011 = LHU, 100 = LBU, 101 = SW, 110 = SH, 111 = SB |

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
| O | `pc` | 32 | ID+EX | 当前PC |
| O | `pc_plus4` | 32 | WB(wb_stage) | 返回地址 |
| O | `instr` | 32 | ID | 取出的指令 |

#### 2.3.2 PC 选择逻辑

```verilog
// 4路MUX + 分支二级判断
assign pc_next = (pc_sel == 2'b00) ? pc + 32'd4              :  // 默认
                 (pc_sel == 2'b01) ? pc + imm_jal            :  // jal
                 (pc_sel == 2'b10) ? {jump_jalr[31:1], 1'b0} :  // jalr &~1
                 (pc_sel == 2'b11) ? (br_taken ? pc + imm_br : pc + 32'd4) : pc + 32'd4;
                 // 分支：br_taken=1才跳，否则回退+4
```

| pc_sel | 来源 | PC_next | 对应指令 |
|:------:|------|------|------|
| `00` | 顺序 | `pc + 4` | 默认 |
| `01` | jal | `pc + imm_J` | jal |
| `10` | jalr | `(alu_result) & ~1` | jalr |
| `11` | 分支 | `br_taken ? pc + imm_B : pc + 4` | beq/bne/... |

#### 2.3.3 指令存储器映射

- 统一内存 `imem[0:65535]`（256KB，与 mem_stage 分离版）
- 索引：`pc[17:2]`——丢弃低2位（指令对齐），取16位 = 65536个32位字
- 复位向量：`pc <= 32'h8000_0000`（AM _pmem_start一致）
- 仿真时 `$readmemh` 把 .bin 加载到 imem

#### 2.3.4 审查记录

| 检查项 | 结果 |
|--------|:--:|
| PC_next 4路MUX | ✅ |
| jalr `& ~1` 对齐 | ✅ |
| 分支 br_taken 二级判断 | ✅ |
| 复位向量 0x80000000 | ✅ |
| pc_plus4 输出到 WB | ✅ |
| `pc[17:2]` 索引 | ✅ |
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
| O | `alu_result` | 32 | → MEM + WB + IF(jalr) |
| O | `br_taken` | 1 | → IF |

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

#### 2.6.2 统一内存 + LSU 流程

```
alu_result(=mem_addr) → mem_idx = mem_addr[17:2]
                              ↓
                       mem[mem_idx] → mem_rdata_raw (组合读)
                              ↓
rs2_rdata(=mem_wdata_raw) → lsu.v → mem_wdata + mem_wmask
                              ↓
                       mem[mem_idx] ← mem_wdata (时序写, posedge clk, 按 wmask 字节写)
                              ↓
                       mem_rdata_raw → lsu.v → mem_rdata (Load: 扩展 → WB)
```

**lsu.v 是必经之路**：读时做扩展（lw/lh/lb/lhu/lbu），写时做对齐+掩码（sw/sh/sb）。Store 不是 rdata2 直接写入 mem[]，必须经过 LSU 格式化成 wdata + wmask。

#### 2.6.3 审查记录

| 检查项 | 结果 |
|--------|:--:|
| mem[] 组合读 | ✅ |
| mem[] 时序写（按 wmask 字节写） | ✅ |
| lsu 例化（含 mem_read/mem_write 门控） | ✅ |
| mem_idx = alu_result[17:2] | ✅ |

### 2.7 wb_stage — 写回阶段

#### 2.7.1 基本信息 & 端口表

| 方向 | 信号 | 位宽 | 来源/去向 |
|:--:|------|:--:|------|
| I | `reg_wdata_src` | 2 | id_stage(control) |
| I | `alu_result` | 32 | ex_stage |
| I | `mem_rdata` | 32 | mem_stage |
| I | `pc_plus4` | 32 | if_stage |
| O | `reg_wdata` | 32 | → regfile.rd_wdata |

纯组合MUX：`00→alu_result, 01→mem_rdata, 10→pc_plus4`。

**wb_stage = 1个3选1MUX = NPC 所有 stage 中最简单的模块。**

#### 2.7.2 审查记录

| 检查项 | 结果 |
|--------|:--:|
| 3选1 MUX | ✅ |
| 无时序 | ✅ |
| reg_wdata 输出 | ✅ |

### 2.8 core_top — 顶层连线

#### 2.8.1 基本信息

| 属性 | 值 |
|------|-----|
| 文件 | `npc/vsrc/core_top.v` |
| 输入 | `clk`, `rst` |
| 输出 | 无（CPU 核是封闭黑盒） |
| 内部模块 | 5 stage + regfile = 6个 |

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

> 前面跑得多热闹不重要。两扇门关着，数据进不来。

| 阀门 | 控制信号 | 默认值 | 关了=效果 |
|------|---------|:--:|------|
| 阀门1(访存) | `mem_read`/`mem_write` | 0 | 不读写内存 |
| 阀门2(写回) | `reg_write` | 0 | 不修改任何寄存器 |

其余信号（alu_op、br_type、lsu_type）的默认值虽然等于真实指令，但被阀门挡住无副作用：
- ALU 算出垃圾 → `reg_write=0` 不写回
- br_cond 判断成立 → `pc_sel=00` 不跳转
- LSU 扩展出垃圾 → `mem_read=0` 门控输出0

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
| O | `pc` | — | id_stage, ex_stage | `pc[31:0]` |
| O | `pc_plus4` | — | wb_stage | `pc_plus4[31:0]` |
| O | `instr` | — | id_stage | `instr[31:0]` |

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
| I | `mem_read` | id_stage(control) | — | `mem_read` |
| I | `mem_write` | id_stage(control) | — | `mem_write` |
| O | `mem_rdata` | — | wb_stage | `mem_rdata[31:0]` |

> mem_stage 内部例化：lsu.v（6入3出纯组合） + mem[64K] 数组。`mem_rdata_raw` 在 mem_stage 内部 wire，不从 core_top 传入。

#### 2.11.5 wb_stage（写回）→ 模块 I/O 表

| 方向 | 端口 | 来源模块 | 去向模块 | core_top wire |
|:--:|------|---------|---------|:--:|
| I | `reg_wdata_src` | id_stage(control) | — | `reg_wdata_src[1:0]` |
| I | `alu_result` | ex_stage(alu) | — | `alu_result[31:0]` |
| I | `mem_rdata` | mem_stage(lsu) | — | `mem_rdata[31:0]` |
| I | `pc_plus4` | if_stage | — | `pc_plus4[31:0]` |
| O | `reg_wdata` | — | regfile 写口 | `rd_wdata[31:0]` |

> wb_stage = 纯组合3选1MUX。1行assign。`reg_wdata = rd_wdata`（同一根wire，端口名不同）。

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
| **时钟复位** | `clk` / `rst` | 1 | 外部 | if_stage, mem_stage, regfile |
| **指令流** | `instr` | 32 | if_stage | id_stage |
| **PC相关** | `pc` | 32 | if_stage | id_stage, ex_stage |
|  | `pc_plus4` | 32 | if_stage | wb_stage |
|  | `pc_sel` | 2 | id_stage(control) | if_stage |
|  | `jump_jalr` | 32 | (=alu_result) | if_stage |
|  | `imm_jal` | 32 | (=imm_out) | if_stage |
|  | `imm_br` | 32 | (=imm_out) | if_stage |
|  | `br_taken` | 1 | ex_stage | if_stage |
| **寄存器地址** | `rs1_addr` / `rs2_addr` / `rd_addr` | 5 | id_stage | regfile |
| **寄存器数据** | `rs1_rdata` / `rs2_rdata` | 32 | regfile | id_stage, ex_stage |
|  | `rd_wdata` | 32 | wb_stage(=reg_wdata) | regfile |
| **执行控制** | `alu_op` | 5 | id_stage(control) | ex_stage |
|  | `alu_src_a` | 1 | id_stage(control) | ex_stage |
|  | `alu_src_b` | 2 | id_stage(control) | ex_stage |
|  | `br_type` | 3 | id_stage(control) | ex_stage |
|  | `imm_out` | 32 | id_stage(imm_gen) | ex_stage, if_stage |
| **执行结果** | `alu_result` | 32 | ex_stage | mem_stage, wb_stage, if_stage |
| **访存控制** | `mem_read` / `mem_write` | 1 | id_stage(control) | mem_stage |
|  | `lsu_type` | 3 | id_stage(control) | mem_stage |
|  | `mem_addr` | 32 | (=alu_result) | mem_stage |
|  | `mem_wdata_raw` | 32 | (=rs2_rdata) | mem_stage |
| **访存结果** | `mem_rdata` | 32 | mem_stage | wb_stage |
| **写回控制** | `reg_write` | 1 | id_stage(control) | regfile |
|  | `reg_wdata_src` | 2 | id_stage(control) | wb_stage |

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

单周期 CPU 中，**状态存储只存在于 3 处**：

| 位置 | 模块 | 描述 | 时钟沿 |
|------|------|------|:--:|
| PC 寄存器 | if_stage | `pc <= pc_next` | posedge clk |
| 统一内存 | mem_stage | `mem[addr][byte] <= wdata[byte]` (受 wmask) | posedge clk |
| 寄存器文件 | regfile | `rf[rd] <= rd_wdata` (受 reg_write) | posedge clk |

其余所有模块均为纯组合逻辑，不包含任何 `always @(posedge clk)`。

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

---

## 三、阶段 2：扩展指令（RV32I 剩余） 🔜

> **状态**：待阶段 1 验证通过。
> **内容**：sll, slt, sltu, xor, or, and, slli, srli, srai, slti, sltiu, xori, ori, andi, lb, lh, lbu, lhu, sb, sh, blt, bge, bltu, bgeu, srl, sra

---

## 四、阶段 3：特权与异常（CSR） 🔜

> **状态**：待阶段 2 完成。
> **内容**：ecall, ebreak, mret, csrrw/csrrs/csrrc/csrrwi/csrrsi/csrrci + csr_reg.v + 异常入口(mtvec) + 异常返回(mepc)

---

## 附录：参考资料

| 资料 | 路径 |
|------|------|
| RISC-V 指令集笔记 | `nemu/study/study-notes/riscv.md` |
| Verilog 学习笔记 | `nemu/study/study-notes/rtl_notes.md` |
| NEMU RISC-V 译码源码 | `nemu/src/isa/riscv32/inst.c` |
| NPC 架构图 (Mermaid) | `npc/notes/stage.mmd` |
| NPC 数据通路图 (Mermaid) | `npc/notes/data.mmd` |
| NPC 设计指南 | `npc/notes/npc.md` |
| 控制信号手册 | `npc/notes/stage.md` |
