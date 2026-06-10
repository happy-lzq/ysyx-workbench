# 控制信号分发速查表

> 默认值 = 最安全状态：PC+4、不写寄存器、不访存、不分支。
> 每条指令只需覆写自己关心的信号，其余靠默认值。

## 一、control 模块输出信号一览

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

## 二、ALU 源选择（alu_src_a / alu_src_b）

```
                 ┌─────────┐
    rdata1  ───→│  MUX_A  │──→ ALU.src1
    pc      ───→│         │
                │sel=alu_ │
                │ src_a   │
                └─────────┘

                 ┌─────────┐
    rdata2  ───→│  MUX_B  │──→ ALU.src2
    imm     ───→│         │
    4       ───→│sel=alu_ │
                │ src_b   │
                └─────────┘
```

| 信号 | 位宽 | 编码 |
|------|:--:|------|
| `alu_src_a` | 1 | 0 = rdata1（rs1 寄存器值）, 1 = pc |
| `alu_src_b` | 2 | 00 = rdata2（rs2 寄存器值）, 01 = imm（立即数）, 10 = 4（常数，用于 jal 返回地址） |

> 为什么 `alu_src_b` 位宽为 2？因为 ALU.src2 有 3 种来源（rdata2 / imm / 4），至少需要 2 位编码。

## 三、按指令类型分发

| 指令类型 | 需显式覆写的信号 | 覆写值 | 个数 |
|---------|-----------------|--------|:--:|
| **R 型** | `alu_op` | 由 funct3/funct7 查表决定 | 2 |
|  | `reg_write` | 1 | |
| **I 算术** | `alu_op` | 由 funct3/funct7 查表决定 | 3 |
|  | `alu_src_b` | 01（选 imm） | |
|  | `reg_write` | 1 | |
| **I load** | `alu_src_b` | 01（选 imm） | 6 |
|  | `mem_read` | 1 | |
|  | `lsu_type` | funct3 查表（lb/lh/lw/lbu/lhu） | |
|  | `reg_write` | 1 | |
|  | `reg_wdata_src` | 01（选 MEM 数据） | |
|  | `alu_op` | ADD（地址计算 = rs1+imm） | |
| **S store** | `alu_src_b` | 01（选 imm） | 4 |
|  | `mem_write` | 1 | |
|  | `lsu_type` | funct3 + sw/sh/sb 编码 | |
|  | `alu_op` | ADD（地址计算） | |
| **B branch** | `br_type` | 由 funct3 查表决定 | 2 |
|  | `pc_sel` | 11（分支指令） | |
| **jal** | `alu_src_a` | 1（选 PC） | 6 |
|  | `alu_src_b` | 10（选常数 4） | |
|  | `alu_op` | ADD（PC + 4 = 返回地址） | |
|  | `reg_write` | 1 | |
|  | `reg_wdata_src` | 10（选 PC+4 作返回地址） | |
|  | `pc_sel` | 01（jal 跳转） | |
| **jalr** | `alu_src_b` | 01（选 imm_I） | 5 |
|  | `alu_op` | ADD（rs1 + imm） | |
|  | `reg_write` | 1 | |
|  | `reg_wdata_src` | 10（选 PC+4 作返回地址） | |
|  | `pc_sel` | 10（jalr 跳转） | |
| **lui** | `alu_op` | LUI（透传 src2） | 3 |
|  | `alu_src_b` | 01（选 imm_U） | |
|  | `reg_write` | 1 | |
| **auipc** | `alu_src_a` | 1（选 PC） | 4 |
|  | `alu_src_b` | 01（选 imm_U） | |
|  | `alu_op` | ADD（PC + imm） | |
|  | `reg_write` | 1 | |

## 四、各信号完整编码速查

| 信号 | 位宽 | 编码值 |
|------|:--:|------|
| `alu_src_a` | 1 | 0 = rdata1, 1 = pc |
| `alu_src_b` | 2 | 00 = rdata2, 01 = imm, 10 = 4 |
| `reg_wdata_src` | 2 | 00 = ALU 结果, 01 = MEM 数据, 10 = PC+4 |
| `pc_sel` | 2 | 00 = PC+4, 01 = jal, 10 = jalr, 11 = branch |
| `br_type` | 3 | 000 = NOP, 001 = BEQ, 010 = BNE, 011 = BLT, 100 = BGE, 101 = BLTU, 110 = BGEU |
| `lsu_type` | 3 | 000 = LW, 001 = LH, 010 = LB, 011 = LHU, 100 = LBU, 101 = SW, 110 = SH, 111 = SB |

## 五、判断依据的本质：如何确定每个控制信号的取值？

### 5.1 核心方法

```
ISA 指令语义（伪代码）
        ↓
  数据通路操作（这条指令要让数据怎么流动？）
        ↓
  流动路径上每个 MUX 该选哪一路？
        ↓
  确定每个控制信号的值
```

> **先有数据通路，再有控制信号。**
> 控制信号本质上就是"数据通路中所有多路选择器的选择端 + 各模块的使能端"。

### 5.2 从 ISA 伪代码到控制信号：逐条推演

| ISA 定义（伪代码） | 翻译为数据通路操作 | 对应的控制信号 |
|-------------------|-------------------|-------------|
| `x[rd] = x[rs1] + x[rs2]` | ALU 做 rs1+rs2，结果写 rf | `alu_src_a=0` `alu_src_b=00` `reg_write=1` `reg_wdata_src=00` |
| `x[rd] = x[rs1] + sext(imm)` | ALU 做 rs1+imm，结果写 rf | `alu_src_a=0` `alu_src_b=01` `reg_write=1` |
| `x[rd] = M[x[rs1] + sext(imm)]` | ALU 算地址，读内存，写 rf | `alu_src_a=0` `alu_src_b=01` `mem_read=1` `reg_write=1` `reg_wdata_src=01` |
| `M[x[rs1] + sext(imm)] = x[rs2]` | ALU 算地址，写内存 | `alu_src_b=01` `mem_write=1` `reg_write=0` |
| `if (x[rs1] == x[rs2]) pc += sext(imm)` | ALU 做减法比较，条件成立跳转 | `alu_src_a=0` `alu_src_b=00` `br_type=BEQ` `pc_sel=11` |

### 5.3 通用判断流程（以 alu_src_a / alu_src_b 为例）

**问题链**：

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
| `jal rd,label` | pc + 4（返回地址） | **pc** | **1** | **4** | **10** |
| `sw rs2,off(rs1)` | rs1 + off（地址） | rdata1 | 0 | imm | 01 |

### 5.4 如何判断"哪些端口受影响、哪些不受影响"？

**原则**：对照"默认安全状态"，看指令语义是否改变了默认行为。

| 默认状态 | 含义 | 什么指令会改变它？ |
|---------|------|-------------------|
| `alu_src_a = 0` | ALU.src1 取自 rs1 | auipc、jal（需要用 PC 做源操作数） |
| `alu_src_b = 00` | ALU.src2 取自 rs2 | 几乎所有非 R 型（需要用 imm 或常数） |
| `reg_write = 0` | 不写寄存器 | 所有需要产生结果的指令 |
| `reg_wdata_src = 00` | 写回数据来自 ALU | load（来自内存）、jal/jalr（来自 PC+4） |
| `mem_read = 0` | 不读内存 | load 指令 |
| `mem_write = 0` | 不写内存 | store 指令 |
| `pc_sel = 00` | 下一条 = PC+4 | 跳转/分支指令 |
| `br_type = 000` | 不分支 | B 型指令 |

**一句话总结**：控制信号 = 数据通路中所有 MUX 选择端 + 模块使能端，对照 ISA 伪代码逐条判断即可。

---

## 六、PC 下一拍来源（if_stage 内部选择）

> PC 的下一拍共有 4 种来源。`pc_sel` 决定走哪条路，`br_taken` 在分支路径内部做二级判断。
> `pc_sel` 由 control.v 的 opcode 决定；`br_taken` 由 br_cond.v 在 EX 阶段算出后反馈给 IF。

### 6.1 PC 来源总表

| pc_sel | 来源 | PC_next = | 对应指令 | 触发条件 |
|:------:|------|------|------|------|
| `00` | 顺序 | `pc + 4` | 大部分指令（addi, lw, sw, lui...） | 默认 |
| `01` | jal | `pc + imm_J` | jal | opcode = `1101111` |
| `10` | jalr | `(rs1 + imm_I) & ~1` | jalr | opcode = `1100111` |
| `11` | 分支 | `br_taken ? (pc + imm_B) : (pc + 4)` | beq, bne, blt, bge, bltu, bgeu | opcode = `1100011` 且 br_taken=1 |

> **注意**：`pc_sel = 11` 时不是无条件跳转，还要看 `br_taken`——br_taken=0 时回退到 `pc + 4`。

### 6.2 PC 选择逻辑（if_stage 内部）

```
                ┌── pc + 4         (pc_sel == 00)
                ├── pc + imm_J     (pc_sel == 01)
    pc_next  =  ├── jump_target    (pc_sel == 10)  ← ALU 算出 (rs1+imm_I)&~1
                └── br_taken ? (pc + imm_B) : (pc + 4)
                                   (pc_sel == 11)
```

### 6.3 互斥性说明

4 种来源互斥——`pc_sel` 的一个值只对应一种指令类型，因为一条指令只有一个 opcode。

| pc_sel | opcode | 指令类 |
|:------:|:------:|------|
| `00` | 其余所有 | 默认顺序 |
| `01` | `1101111` | J 型 (jal) |
| `10` | `1100111` | I 型 (jalr) |
| `11` | `1100011` | B 型 (branch) |

### 6.4 扩展预留（阶段 3）

| 来源 | 触发条件 | PC_next = |
|------|------|------|
| 异常入口 | ecall 等 | `mtvec` |
| 异常返回 | mret | `mepc` |

> 阶段 3（CSR/异常）时需扩展 `pc_sel` 位宽或增加独立信号。

---

## 七、PC 相关计算：走 ALU 还是走 if_stage？

> 关键判断：第二操作数是常量（if_stage 能算）还是来自寄存器（必须走 ALU）？结果是否要写回寄存器？

### 7.1 三类路由规则

| 路由 | 判断条件 | 走哪里 |
|------|---------|--------|
| **if_stage 内部** | 操作数只有 PC + 常数，且结果只用于改 PC | `if_stage.v` 内部加法器 |
| **必须走 ALU** | 操作数涉及寄存器值（如 jalr 的 rs1） | EX 阶段 ALU |
| **必须走 ALU** | 结果要写回寄存器文件（如 auipc） | EX 阶段 ALU → WB |

### 7.2 逐指令对照

| 指令 | PC 计算 | 操作数 | 结果去向 | 走哪里 |
|------|------|------|------|:--:|
| **默认** | `pc + 4` | PC + 常数 4 | 仅改 PC | if_stage |
| **jal** | `pc + imm_J` | PC + 常数 | 仅改 PC | if_stage |
| **jal** | 返回地址 `pc + 4` | PC + 常数 4 | 写 rd | 直通 `pc_plus4` 线 (reg_wdata_src=10) |
| **jalr** | `(rs1 + imm_I) & ~1` | **寄存器 rs1** + 常数 | 仅改 PC | **ALU** |
| **jalr** | 返回地址 `pc + 4` | PC + 常数 4 | 写 rd | 直通 `pc_plus4` 线 |
| **branch** | `pc + imm_B` | PC + 常数 | 仅改 PC | if_stage |
| **auipc** | `pc + imm_U` | PC + 常数 | **写 rd** | **ALU**（结果走 EX→MEM→WB） |

### 7.3 简记

```
if_stage 管：pc+4, pc+imm_J, pc+imm_B（纯跳转，不改寄存器）
ALU 管：     rs1+imm（jalr）、pc+imm_U（auipc）
直通 pc_plus4：jal/jalr 返回地址，不消耗 ALU
```

---

## 八、control 输出信号含义速查

### 8.1 送到 EX 阶段（4 组）

| 信号 | 位宽 | 含义 | 接收模块 |
|------|:--:|------|:--:|
| `alu_op` | 5 | ALU 操作码：ADD/SUB/SLL/SLT/SLTU/XOR/OR/AND/SRL/SRA/LUI | `alu.v` |
| `alu_src_a` | 1 | ALU 源 A 选择：0 = rdata1（rs1）, 1 = PC | ex_stage MUX |
| `alu_src_b` | 2 | ALU 源 B 选择：00 = rdata2, 01 = imm, 10 = 4 | ex_stage MUX |
| `br_type` | 3 | 分支比较类型：000=BEQ, 001=BNE, 010=BLT, 011=BGE, 100=BLTU, 101=BGEU | `br_cond.v` |

### 8.2 送到 MEM 阶段（3 组）

| 信号 | 位宽 | 含义 | 接收模块 |
|------|:--:|------|:--:|
| `mem_read` | 1 | 读内存使能：1 = 从 data_mem 读 | `data_mem` + `lsu.v` |
| `mem_write` | 1 | 写内存使能：1 = 写 data_mem | `data_mem` + `lsu.v` |
| `lsu_type` | 3 | 访存类型：000=LW, 001=LH, 010=LB, 011=LHU, 100=LBU, 101=SW, 110=SH, 111=SB | `lsu.v` |

### 8.3 送到 WB 阶段（2 组）

| 信号 | 位宽 | 含义 | 接收模块 |
|------|:--:|------|:--:|
| `reg_write` | 1 | 写寄存器使能：1 = posedge 时 rf[rd] ← rd_wdata | `regfile` 写口 |
| `reg_wdata_src` | 2 | 写回数据来源：00 = ALU 结果, 01 = MEM 数据, 10 = PC+4 | wb_stage MUX |

### 8.4 送到 IF 阶段（1 组）

| 信号 | 位宽 | 含义 | 接收模块 |
|------|:--:|------|:--:|
| `pc_sel` | 2 | PC 下一拍选择：00 = PC+4, 01 = jal (pc+imm_J), 10 = jalr (ALU结果), 11 = branch (pc+imm_B, 看 br_taken) | `if_stage.v` |

### 8.5 默认值安全网

| 信号 | 默认值 | 安全效果 |
|------|:-----:|------|
| `pc_sel` | 00 | 顺序执行，不跳转 |
| `reg_write` | 0 | 不修改任何寄存器 |
| `mem_write` | 0 | 不写入内存 |
| `mem_read` | 0 | 不读取内存（LSU 输出被丢弃） |

> 其余信号（alu_op、br_type、lsu_type 等）的默认值虽然对应真实指令，但被 `reg_write=0`/`pc_sel=00`/`mem_write=0` 三道阀门挡住了副作用。

> 先把默认值设为"什么都不发生"的安全状态；然后对照每条指令的 ISA 伪代码，看它在数据通路中"多做了什么事"，就覆写对应的控制信号。信号取值不是背出来的，是**从数据流动路径上推导出来的**。