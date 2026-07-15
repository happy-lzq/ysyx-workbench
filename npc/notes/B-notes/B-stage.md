# B-Stage: 控制信号分组（集中控制 → 分布式控制第一步）

> 状态：🚧 进行中（已完成：`ctrl_defs.vh`, `control.v`, `id_stage.v`, `wb_stage.v`）  
> 日期：2026-07-15  
> 关联文件：`ctrl_defs.vh`, `control.v`, `id_stage.v`, `core_top.v`, `ex_stage.v`, `mem_stage.v`, `wb_stage.v`, `if_stage.v`, `csr.v`

---

## 0. 分布式控制核心总架构

```
                    core_top（接线员）
                ┌──────────────────────────────────────┐
control.v       │                                      │
┌──────┐        │  ex_ctrl[10:0]  ────────────────────→ ex_stage
│ 译码 │        │     拆：alu_op, alu_src_a/b, br_type   │
│ 打包 │──bus──→│                                      │
└──────┘        │  mem_ctrl[4:0]  ────────────────────→ mem_stage
                │     拆：mem_read, mem_write, lsu_type  │
                │     内部 interrupt 门控                │
                │                                      │
                │  wb_ctrl[7:0]  ──┬──→ wb_stage        │
                │     拆：reg_wdata_src                  │
                │                  └──→ regfile（写端口） │
                │     core_top代拆：reg_write, rd_addr   │
                │                                      │
                │  csr_ctrl[48:0] ──┬──→ ex_stage       │
                │     拆：csr_op, csr_imm, csr_zimm      │
                │     生成 csr_wdata                     │
                │                   └──→ csr             │
                │     拆：csr_read, csr_write, csr_addr  │
                │                                      │
                │  sys_ctrl[34:0] ──┬──→ csr             │
                │                   ├──→ if_stage        │
                │                   └──→ halt            │
                │     core_top代拆：trap_enter, mret,    │
                │                  is_ebreak, trap_code  │
                │     内部 interrupt 门控                │
                │                                      │
                │  pc_sel[1:0]  ──────────────────────→ if_stage
                │     （独立前馈，不走流水线）             │
                │                                      │
                │  数据流（不打包，照旧直连）：            │
                │  pc, pc_plus4, rs1_rdata, rs2_rdata,  │
                │  imm_out, alu_result, mem_rdata,      │
                │  csr_rdata, rd_wdata ...              │
                └──────────────────────────────────────┘
```

### 核心设计原则

| 原则 | 说明 |
|------|------|
| **数据与控制分离** | 数据通路（pc, rs_data, alu_result...）不走总线，照旧直连 |
| **控制按功能域打包** | 译码产生的控制信号按目标阶段/功能域分组，每组一根总线 |
| **一根总线多消费者** | `csr_ctrl` 同时服务 ex_stage（生成 csr_wdata）和 csr 模块（读写 CSR）；`wb_ctrl` 同时服务 wb_stage（选回写源）和 regfile（写使能）——各取所需字段 |
| **谁用谁拆包** | 复杂模块（ex/mem/wb/if/csr）收到总线后自行拆包；简单模块（regfile/halt）由 core_top 代拆所需信号 |
| **门控下放** | `interrupt_valid` 传入需要的模块（mem_stage, csr, if_stage），内部自行门控 |
| **core_top 只接线** | 顶层不译码，只负责：声明总线、连接生产者和消费者、为简单模块代拆少量信号 |

---

## 1. 设计动机

### 改前（单周期 / 集中控制）

```
control.v 输出 20+ 根散线 → id_stage.v 透传 → core_top.v 扇出到所有阶段
```

- 控制信号从 ID 阶段"广播"到 EX/MEM/WB，所有阶段实时可见
- 每新增/修改一个控制信号，需要在 3+ 个文件里改端口声明和连线
- 引入流水线后，ID/EX 寄存器需要逐根存储，代码膨胀严重

### 改后（分布式控制第一步：分组打包）

```
control.v 输出 5 根总线 + 1 根独立线 → id_stage.v 透传 → core_top.v 拆包连到各阶段

ex_ctrl[10:0]   → EX 阶段用
mem_ctrl[4:0]   → MEM 阶段用
wb_ctrl[7:0]    → WB 阶段用
csr_ctrl[48:0]  → CSR 模块用（控制 + csr_zimm 数据合入）
sys_ctrl[34:0]  → 异常/系统控制
pc_sel[1:0]     → 独立：ID→IF 前馈，不随流水线传递
```

- 控制信号按**目标阶段/功能域**分组，每组一根总线
- `csr_zimm`（32 位数据）合入 `csr_ctrl`：CSR 相关全部在一根总线里走
- `trap_enter`/`mret`/`is_ebreak`/`trap_code` 打包为 `sys_ctrl`：异常信息整体传递
- 仅 `pc_sel` 保留独立：它是 ID→IF 前馈，不走流水线
- 新增信号只需改 `ctrl_defs.vh` 和 `control.v` 内的拼接，不影响接口
- 为流水线寄存器"整存整取"做好数据类型准备

---

## 2. 控制信号分组方案

### 2.1 组定义与位宽映射（`ctrl_defs.vh`）

#### EX_CTRL（11 bits）— 执行阶段

```
assign ex_ctrl = {alu_op[4:0], alu_src_a, alu_src_b[1:0], br_type[2:0]};
//                [10:6]       [5]        [4:3]           [2:0]
```

| 信号 | 位宽 | 位段 | `define 宏 |
|------|:----:|------|------------|
| `alu_op` | 5 | `[10:6]` | `` `EX_CTRL_ALU_OP_MSB `` / `` `EX_CTRL_ALU_OP_LSB `` |
| `alu_src_a` | 1 | `[5]` | `` `EX_CTRL_ALU_SRC_A `` |
| `alu_src_b` | 2 | `[4:3]` | `` `EX_CTRL_ALU_SRC_B_MSB `` / `` `EX_CTRL_ALU_SRC_B_LSB `` |
| `br_type` | 3 | `[2:0]` | `` `EX_CTRL_BR_TYPE_MSB `` / `` `EX_CTRL_BR_TYPE_LSB `` |

#### MEM_CTRL（5 bits）— 访存阶段

```
assign mem_ctrl = {mem_read, mem_write, lsu_type[2:0]};
//                  [4]        [3]         [2:0]
```

| 信号 | 位宽 | 位段 | `define 宏 |
|------|:----:|------|------------|
| `mem_read` | 1 | `[4]` | `` `MEM_CTRL_READ `` |
| `mem_write` | 1 | `[3]` | `` `MEM_CTRL_WRITE `` |
| `lsu_type` | 3 | `[2:0]` | `` `MEM_CTRL_LSU_MSB `` / `` `MEM_CTRL_LSU_LSB `` |

#### WB_CTRL（8 bits）— 写回阶段

```
assign wb_ctrl = {reg_write, reg_wdata_src[1:0], rd_addr[4:0]};
//                  [7]         [6:5]               [4:0]
```

| 信号 | 位宽 | 位段 | `define 宏 |
|------|:----:|------|------------|
| `reg_write` | 1 | `[7]` | `` `WB_CTRL_REG_WRITE `` |
| `reg_wdata_src` | 2 | `[6:5]` | `` `WB_CTRL_WDATA_SRC_MSB `` / `` `WB_CTRL_WDATA_SRC_LSB `` |
| `rd_addr` | 5 | `[4:0]` | `` `WB_CTRL_RD_ADDR_MSB `` / `` `WB_CTRL_RD_ADDR_LSB `` |

#### CSR_CTRL（49 bits）— CSR 控制 + 数据

```
assign csr_ctrl = {csr_read, csr_write, csr_op[1:0], csr_imm, csr_addr[11:0], csr_zimm[31:0]};
//                  [48]       [47]        [46:45]      [44]     [43:32]          [31:0]
```

| 信号 | 位宽 | 位段 | `define 宏 |
|------|:----:|------|------------|
| `csr_read` | 1 | `[48]` | `` `CSR_CTRL_READ `` |
| `csr_write` | 1 | `[47]` | `` `CSR_CTRL_WRITE `` |
| `csr_op` | 2 | `[46:45]` | `` `CSR_CTRL_OP_MSB `` / `` `CSR_CTRL_OP_LSB `` |
| `csr_imm` | 1 | `[44]` | `` `CSR_CTRL_IMM `` |
| `csr_addr` | 12 | `[43:32]` | `` `CSR_CTRL_ADDR_MSB `` / `` `CSR_CTRL_ADDR_LSB `` |
| `csr_zimm` | 32 | `[31:0]` | `` `CSR_CTRL_ZIMM_MSB `` / `` `CSR_CTRL_ZIMM_LSB `` |

> `csr_zimm` 放最低 32 位的好处：EX 阶段取用它时直接 `csr_ctrl[31:0]`，无需移位。

#### SYS_CTRL（35 bits）— 系统 / 异常控制

```
assign sys_ctrl = {trap_enter, mret, is_ebreak, trap_code[31:0]};
//                  [34]        [33]   [32]        [31:0]
```

| 信号 | 位宽 | 位段 | `define 宏 |
|------|:----:|------|------------|
| `trap_enter` | 1 | `[34]` | `` `SYS_CTRL_TRAP_ENTER `` |
| `mret` | 1 | `[33]` | `` `SYS_CTRL_MRET `` |
| `is_ebreak` | 1 | `[32]` | `` `SYS_CTRL_IS_EBREAK `` |
| `trap_code` | 32 | `[31:0]` | `` `SYS_CTRL_TRAP_CODE_MSB `` / `` `SYS_CTRL_TRAP_CODE_LSB `` |

> 流水线冲刷时只需清空 `sys_ctrl_q` 一个寄存器，不用逐信号清零。

#### PC_CTRL（37 bits）— PC 控制（预留）

| 信号 | 位宽 | 位段 | `define 宏 | 说明 |
|------|:----:|------|-----------|------|
| `pc_sel` | 2 | `[36:35]` | `` `PC_SEL_MSB `` / `` `PC_SEL_LSB `` | ID 发出 |
| `br_taken` | 1 | `[34]` | `` `BR_TAKEN `` | ⚠️ EX 阶段产生，不应在 ID 控制包中 |
| `is_jal` | 1 | `[33]` | `` `IS_JAL `` | ID 发出 |
| `is_jalr` | 1 | `[32]` | `` `IS_JALR `` | ID 发出 |
| `trap_code` | 32 | `[31:0]` | `` `TRAP_CODE_MSB `` / `` `TRAP_CODE_LSB `` | ID 发出 |

> ⚠️ `PC_CTRL` 中的 `br_taken` 是 EX 阶段**产生**的信号，与其他 ID 阶段**发出**的信号混在同一组中。流水线化后需拆分：ID 发出的 PC 控制请求（前馈）vs EX 反馈的跳转结果（反馈），走不同的路径。

#### 汇总

| 总线 | 总宽 | 拼接式子 |
|------|:----:|----------|
| `ex_ctrl` | 11 | `{alu_op(5), alu_src_a(1), alu_src_b(2), br_type(3)}` |
| `mem_ctrl` | 5 | `{mem_read(1), mem_write(1), lsu_type(3)}` |
| `wb_ctrl` | 8 | `{reg_write(1), reg_wdata_src(2), rd_addr(5)}` |
| `csr_ctrl` | 49 | `{csr_read(1), csr_write(1), csr_op(2), csr_imm(1), csr_addr(12), csr_zimm(32)}` |
| `sys_ctrl` | 35 | `{trap_enter(1), mret(1), is_ebreak(1), trap_code(32)}` |
| `pc_ctrl` | 37 | `{pc_sel(2), br_taken(1), is_jal(1), is_jalr(1), trap_code(32)}` （预留，未启用） |

### 2.2 位段宏的必要性

```verilog
// ❌ 无宏：魔法数字，改一处需改 N 处
assign alu_op = ex_ctrl[10:6];

// ✅ 有宏：单一真相源，自文档化
`define EX_CTRL_ALU_OP_MSB 10
`define EX_CTRL_ALU_OP_LSB  6
assign alu_op = ex_ctrl[`EX_CTRL_ALU_OP_MSB:`EX_CTRL_ALU_OP_LSB];
```

**三大好处**：
- **单一真相源**：位宽/位位置只在 `.vh` 定义一次
- **防错**：编译器检查宏名拼写，数字写错不报错
- **易重构**：增删信号只改 `.vh`，所有引用自动跟随

### 2.3 唯一独立保留的信号：`pc_sel`

所有控制/数据信号均已打包进 5 根总线，**仅 `pc_sel[1:0]` 保留为独立端口**。

| 信号 | 原因 |
|------|------|
| `pc_sel` | ID → IF **前馈**信号，不随指令流过 EX/MEM/WB 流水线，与控制包的生命周期不同 |

> 其他原本独立的信号归宿：
> - `csr_zimm` → 合入 `csr_ctrl[31:0]`
> - `trap_enter`, `mret`, `is_ebreak`, `trap_code` → 合入 `sys_ctrl[34:0]`

---

## 3. 核心改造技巧：端口降级 + 末尾打包

### 3.1 问题

原来的 `control.v` 中信号既是端口又是 reg：

```verilog
output reg [4:0] alu_op;   // 不能同时保留这个又新增 bus 端口
```

### 3.2 解决方案

```
原来：output reg 信号  →  直接当端口用
现在：内部 reg 信号    →  assign 拼接  →  output wire 总线端口
```

**三步操作**：

1. **端口声明改总线**：`output wire [WIDTH-1:0] ex_ctrl;`
2. **原端口降级为内部 reg**：`reg [4:0] alu_op;`（去掉 `output`）
3. **末尾打包**：`assign ex_ctrl = {alu_op, alu_src_a, alu_src_b, br_type};`

### 3.3 端口与内部 reg 命名冲突

保留独立端口的信号（如 `pc_sel`），其端口名与内部 reg 冲突。解决：内部 reg 加 `_int` 后缀。

```verilog
// 端口
output wire [1:0] pc_sel;

// 内部 reg（always 块里赋值）
reg [1:0] pc_sel_int;

// 末尾桥接
assign pc_sel = pc_sel_int;
```

---

## 4. 译码逻辑：一行不动

```
always @(*) begin
    // 默认值 + case(opcode) ... 完全不变
    // 唯一变化：_int 后缀的信号名
end
```

核心原则：**控制信号的"产生逻辑"与"传递方式"解耦**。

---

## 5. 文件修改清单

### 已完成 ✅

| 文件 | 改动内容 | 关键点 |
|------|----------|--------|
| `ctrl_defs.vh` | 扩展 `CSR_CTRL` 为 49 bits（+`csr_zimm` 位段）；新增 `SYS_CTRL` 35 bits；`WB_CTRL` 加入 `rd_addr` | 位宽算术需逐组验证 |
| `control.v` | 端口：5 总线（ex/mem/wb/csr/sys）+ 1 独立（pc_sel）；内部 reg 降级；末尾 assign 打包 | `always @(*)` 逻辑不动；`_int` 后缀一致性；`csr_zimm` 必须 `wire`+`assign` 而非 `reg` 初始化 |
| `id_stage.v` | 端口声明从散线改总线；`control` 例化对应修改；`imm_gen` 保持不动 | `rd_addr` 提取移到 `control.v` 内部；数据信号（rs1/rs2_addr, imm_out）原样保留 |
| `wb_stage.v` | 端口 `reg_wdata_src` → `wb_ctrl` 总线；内部一行拆包 | 改动最小（1 行拆包），验证总线方案可行性 |

### 待完成 🚧

| 文件 | 改动内容 | 复杂度 |
|------|----------|:------:|
| `mem_stage.v` | 端口改 `mem_ctrl` 总线 + `interrupt_valid`；内部拆包 + `_eff` 门控 | ⭐⭐ |
| `if_stage.v` | 端口加 `sys_ctrl` 总线 + `interrupt_valid`；拆包 + trap 门控 | ⭐⭐ |
| `ex_stage.v` | 端口改 `ex_ctrl` + `csr_ctrl` 双总线；拆包 7 个控制信号 | ⭐⭐⭐ |
| `csr.v` | 端口改 `csr_ctrl` + `sys_ctrl` 双总线 + `interrupt_valid`；内部拆包 + 门控 | ⭐⭐⭐ |
| `core_top.v` | 清理旧散线声明；声明 5 总线 + pc_sel；各模块例化改总线连接；保留 `reg_write_eff`/`is_ebreak_eff` 两个简单门控 | ⭐⭐ |

### 推荐修改顺序

```
wb_stage → mem_stage → if_stage → ex_stage → csr → core_top
 (最简单)   (门控)      (门控)     (最复杂)   (复杂)   (总装)
```

每改完一个文件就编译验证。

---

## 6. 设计思想总结

### 从"广播"到"传递"

```
单周期（广播）：
  ID 控制信号 ──┬──→ EX
               ├──→ MEM
               └──→ WB
  所有阶段同时看到控制信号

流水线（传递）：
  ID → [ID/EX] → EX → [EX/MEM] → MEM → [MEM/WB] → WB
        ex_ctrl      mem_ctrl      wb_ctrl
        存入          存入          存入
```

### 打包 = 流水线寄存器的天然数据类型

```verilog
// 有了打包，流水线寄存器声明只需：
reg [`EX_CTRL_WIDTH-1:0]  ex_ctrl_id_ex;   // 一行替代 5 个 reg
reg [`MEM_CTRL_WIDTH-1:0] mem_ctrl_id_ex;
reg [`WB_CTRL_WIDTH-1:0]  wb_ctrl_id_ex;
```

### 每阶段只看到自己需要的控制

| 阶段 | 使用的总线 | 穿过（不丢弃） | 丢弃的总线 |
|------|-----------|---------------|-----------|
| EX | `ex_ctrl`, `csr_ctrl` | `mem_ctrl`, `wb_ctrl`, `sys_ctrl` | — |
| MEM | `mem_ctrl` | `wb_ctrl`, `sys_ctrl` | `ex_ctrl`, `csr_ctrl` |
| WB | `wb_ctrl` | `sys_ctrl`（如需精确异常） | `mem_ctrl` |

> `sys_ctrl` 贯穿全流水线：异常可能在 MEM 阶段触发（如 load 访问违例），需要 `sys_ctrl` 伴随到 MEM 才能判断。

---

## 7. 下一步

### 当前进度

- [x] `ctrl_defs.vh` — 控制总线位段宏定义
- [x] `control.v` — 译码输出从散线改 5 总线 + pc_sel
- [x] `id_stage.v` — 端口透传从散线改总线
- [x] `wb_stage.v` — 接收 `wb_ctrl` 总线，内部拆包
- [ ] `mem_stage.v` — 接收 `mem_ctrl` + `interrupt_valid`，内部拆包 + 门控
- [ ] `if_stage.v` — 接收 `sys_ctrl` + `interrupt_valid`，内部拆包 + 门控
- [ ] `ex_stage.v` — 接收 `ex_ctrl` + `csr_ctrl` 双总线，内部拆包
- [ ] `csr.v` — 接收 `csr_ctrl` + `sys_ctrl` + `interrupt_valid`，内部拆包 + 门控
- [ ] `core_top.v` — 清理旧散线 + 总线直连各模块
- [ ] 引入流水线寄存器（ID/EX, EX/MEM, MEM/WB）
- [ ] 引入 valid/allowin 握手
- [ ] 处理数据冒险（forwarding / stalling）
- [ ] 处理控制冒险（branch redirect / flushing）
