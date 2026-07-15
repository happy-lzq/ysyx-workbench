# B-Stage: 控制信号分组——集中控制 → 分布式控制

> 状态：✅ 已完成并通过测试  
> 日期：2026-07-15 ~ 2026-07-16  
> 分支：`b-stage`

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
| **控制按功能域打包** | 译码信号按目标阶段/功能域分组，5 组总线 + 1 独立信号 |
| **一根总线多消费者** | `csr_ctrl` → ex_stage + csr；`wb_ctrl` → wb_stage + regfile；`sys_ctrl` → csr + if_stage + halt |
| **谁用谁拆包** | 复杂模块收总线自行拆包；简单模块（regfile/halt）由 core_top 代拆 |
| **门控下放** | `interrupt_valid` 传入 mem_stage/csr/if_stage，各模块内部自行门控 |
| **core_top 只接线** | 顶层不译码：声明总线、连接生产者消费者、为简单模块代拆少量信号 |

---

## 1. 设计动机

### 1.1 改前：单周期集中控制

```
control.v ──alu_op──┬──→ id_stage ──→ core_top ──→ ex_stage
           ──mem_read─┤
           ──reg_write┘  20+ 根散线，"谁要谁拿走"
```

**痛点**：
- 新增控制信号需要改 3+ 个文件的端口声明和连线
- 引入流水线后 ID/EX 寄存器需逐信号声明，代码膨胀
- 控制信号从 ID "广播"到所有阶段，各阶段之间的依赖关系不清晰

### 1.2 改后：分布式控制

```
control.v ──ex_ctrl──→ id_stage ──→ core_top ──→ ex_stage（拆包）
           ──mem_ctrl─→           ──→           ──→ mem_stage（拆包）
           ──wb_ctrl──→           ──→           ──→ wb_stage（拆包）
           ──csr_ctrl─→           ──→           ──→ ex_stage + csr
           ──sys_ctrl─→           ──→           ──→ csr + if_stage + halt
           ──pc_sel───→           ──→           ──→ if_stage

           5 根集装箱 + 1 根快递单，"你的包裹请签收"
```

**优势**：
- 控制信号按目标阶段打包，依赖关系一目了然
- 流水线寄存器"整存整取"：`reg [EX_CTRL_WIDTH-1:0] ex_ctrl_q;` 一行搞定
- 新增信号只改 `.vh` 和 `control.v` 内部拼接，接口不变

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

## 4. interrupt 门控分发策略

### 4.1 设计决策：分布式门控

`interrupt_valid` 从 DPI-C 仿真环境传入，作用是"取消当前指令副作用 + 注入 trap"。选择**分布式门控**：各模块自行处理，而非 core_top 集中门控。

| 模块 | 收什么 | 内部门控逻辑 |
|------|--------|-------------|
| `mem_stage` | `mem_ctrl` + `interrupt_valid` | `mem_read`/`mem_write` 清零（阻止访存） |
| `csr` | `csr_ctrl` + `sys_ctrl` + `interrupt_valid` + `interrupt_cause` | `csr_write`/`mret` 清零，`trap_enter = raw \| interrupt`，`trap_code` 替换为 `interrupt_cause` |
| `if_stage` | `sys_ctrl` + `interrupt_valid` | `trap_enter = raw \| interrupt`，`mret` 清零 |
| **core_top 代劳** | `wb_ctrl` / `sys_ctrl` | `reg_write_eff`（给 regfile）、`is_ebreak_eff`（给 halt） |

### 4.2 两种门控模式

| 模式 | 含义 | 示例 |
|------|------|------|
| **清零** | 中断时取消操作 | `mem_write = interrupt_valid ? 1'b0 : raw` |
| **OR** | 中断时强制注入 | `trap_enter = raw | interrupt_valid` |

### 4.3 `csr.v` 需额外接收 `interrupt_cause`

中断时 `csr_mcause` 应写入 `interrupt_cause`（如 `0x80000007` 定时器中断），而非指令自身的 `trap_code`。因此 `csr` 需额外 `interrupt_cause` 输入端口。

---

## 5. 核心改造技巧

### 5.1 "端口降级 + 末尾打包"（`control.v`）

```
原来：output reg [4:0] alu_op;   ← 既是端口又是 reg
现在：reg [4:0] alu_op;          ← 降级为内部 reg
     assign ex_ctrl = {alu_op, ...};  ← 末尾拼接打包成总线端口
```

### 5.2 端口与内部 reg 命名冲突

`pc_sel` 等保留独立端口的信号，内部 reg 加 `_int` 后缀：

```verilog
output wire [1:0] pc_sel;       // 端口
reg [1:0] pc_sel_int;           // 内部 reg（always 块赋值）
assign pc_sel = pc_sel_int;     // 末尾桥接
```

涉及信号：`pc_sel_int`, `trap_enter_int`, `mret_int`, `is_ebreak_int`, `trap_code_int`

### 5.3 译码逻辑：一行不动

`always @(*)` 块内 case(opcode) 逻辑完全不变。核心原则：**控制信号的产生逻辑与传递方式解耦**。

### 5.4 `csr_zimm` 的陷阱

```verilog
// ❌ 错误：reg 初始化只在 t=0 执行一次，inst 变化时不更新
reg [31:0] csr_zimm = {{27{1'b0}}, inst[19:15]};

// ✅ 正确：wire + assign 持续赋值
wire [31:0] csr_zimm = {{27{1'b0}}, inst[19:15]};
```

---

## 6. 文件修改全记录

---

### 6.1 控制总线定义

| 文件 | 改动 | 复杂度 |
|------|------|:--:|
| `ctrl_defs.vh` | 扩展 `WB_CTRL` 加入 `rd_addr`；`CSR_CTRL` 扩至 49 bits（+csr_zimm）；新增 `SYS_CTRL` 35 bits | ⭐ |

### 6.2 译码 + 打包（信号生产者）

| 文件 | 改动 | 复杂度 |
|------|------|:--:|
| `control.v` | 输出从 ~20 根散线 → 5 总线 + pc_sel；内部 reg 降级；末尾打包；csr_zimm 改为 wire+assign | ⭐⭐⭐ |

### 6.3 透传层

| 文件 | 改动 | 复杂度 |
|------|------|:--:|
| `id_stage.v` | 端口从散线改总线；control 例化对应修改；数据信号不变 | ⭐ |

### 6.4 阶段模块——收总线、拆包、门控

| 顺序 | 文件 | 收什么总线 | 内部拆包 | 门控 | 复杂度 |
|:--:|------|-----------|----------|:--:|:--:|
| ① | `wb_stage.v` | `wb_ctrl` | `rd_wdata_src` | — | ⭐ |
| ② | `mem_stage.v` | `mem_ctrl` | `mem_read/write/lsu_type` | 清零 | ⭐⭐ |
| ③ | `if_stage.v` | `sys_ctrl` | `trap_enter/mret` | 清零+OR | ⭐⭐ |
| ④ | `ex_stage.v` | `ex_ctrl` + `csr_ctrl` | 7 信号（4 EX + 3 CSR） | — | ⭐⭐⭐ |
| ⑤ | `csr.v` | `csr_ctrl` + `sys_ctrl` | 6 信号（3 CSR + 3 SYS） | 清零+OR+替换 | ⭐⭐⭐ |

### 6.5 顶层总装

| 文件 | 改动 | 复杂度 |
|------|------|:--:|
| `core_top.v` | 清理所有旧散线声明；声明 5 总线 + pc_sel；各模块例化从散线改总线；补 6 条数据通路 assign；为 regfile/halt 代拆门控 | ⭐⭐ |

### 6.6 不需改动的模块

| 模块 | 原因 |
|------|------|
| `alu.v` | 收 `src1/src2/alu_op`，由 ex_stage 拆包传入 |
| `br_cond.v` | 收 `rs1/rs2/br_type`，由 ex_stage 拆包传入 |
| `halt.v` | 收 `is_ebreak`，由 core_top 代拆+门控传入 |
| `lsu.v` | 收 `lsu_type/mem_read/mem_write`，由 mem_stage 拆包传入 |
| `regfile.v` | 收 `reg_write/rd_addr`，由 core_top 代拆+门控传入 |
| `imm_gen.v` | 收 `inst`，由 id_stage 直传 |

---

## 7. 常见错误与教训

| 错误 | 原因 | 教训 |
|------|------|------|
| `` `WB_CTRL_WIDTH `` 未定义 | 忘记 `include "ctrl_defs.vh"` | 每个用到总线宏的文件都要 include |
| `csr_zimm` 声明为 `reg` | reg 初始化只在 t=0 生效 | 需持续赋值的必须 `wire` + `assign` |
| `pc_sel` 在 always 块直接赋值 | 端口改 output wire 后不能 always 赋值 | 内部 reg 加 `_int` 后缀 + 末尾 assign 桥接 |
| `rs1_addr`/`rs2_addr` 未声明 | id_stage 新增输出但 core_top 忘声明 wire | 改模块端口时同步检查顶层连线 |
| `mem_addr`/`mem_wdata_raw` 未赋值 | 清理旧散线时误删数据通路 assign | 数据通路 assign 不是控制信号，不能删 |
| `.instr(inst)` 端口名不匹配 | id_stage/if_stage 改名 `inst` 但顶层未同步 | 改端口名要全局搜索引用 |

---

## 8. 设计思想总结

## 8. 设计思想总结

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

## 9. 当前进度与测试结果

### 已完成全部文件 ✅

- [x] `ctrl_defs.vh` — 控制总线位段宏定义
- [x] `control.v` — 译码输出从散线改 5 总线 + pc_sel
- [x] `id_stage.v` — 端口透传从散线改总线
- [x] `wb_stage.v` — 接收 `wb_ctrl` 总线，内部拆包
- [x] `mem_stage.v` — 接收 `mem_ctrl` + `interrupt_valid`，内部拆包 + 门控
- [x] `if_stage.v` — 接收 `sys_ctrl` + `interrupt_valid`，内部拆包 + 门控
- [x] `ex_stage.v` — 接收 `ex_ctrl` + `csr_ctrl` 双总线，内部拆包
- [x] `csr.v` — 接收 `csr_ctrl` + `sys_ctrl` + `interrupt_cause`，内部拆包 + 门控
- [x] `core_top.v` — 清理旧散线 + 总线直连各模块

### 测试结果

✅ 编译通过，cpu-tests 全部通过。

### 下一步展望

- [ ] 引入流水线寄存器（IF/ID, ID/EX, EX/MEM, MEM/WB）
- [ ] 引入 valid/allowin 握手
- [ ] 数据冒险：forwarding / stalling
- [ ] 控制冒险：branch redirect / flushing
- [ ] 精确异常：kill younger instruction
