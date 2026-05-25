# NPC 单周期 RV32I — RISC-V 指令集查询表

> 基于 RISC-V 官方手册 + NEMU 源码 (`nemu/src/isa/riscv32/inst.c`) 整理。
> 标注：⭐ 第一批 | 🔶 第二批 | 🔷 第三批 (CSR/异常/特权)
>
> **配套文档**：架构图 `stage.mmd` / `data.mmd` / `control.mmd` | 设计笔记 `design.md` | 控制信号手册 `stage.md`

---

## 目录

- [一、RV32I 完整指令集分类表](#一rv32i-完整指令集分类表)
  - [1.1 R 型（寄存器-寄存器）](#11-r-型寄存器-寄存器)
  - [1.2 I 型 — 算术立即数](#12-i-型--算术立即数)
  - [1.3 I 型 — 加载](#13-i-型--加载)
  - [1.4 I 型 — jalr](#14-i-型--jalr)
  - [1.5 S 型（存储）](#15-s-型存储)
  - [1.6 B 型（分支）](#16-b-型分支)
  - [1.7 U 型（上立即数）](#17-u-型上立即数)
  - [1.8 J 型（跳转）](#18-j-型跳转)
  - [1.9 系统/CSR 指令](#19-系统csr-指令)
- [二、32 位指令编码总览](#二32-位指令编码总览)
- [三、opcode 速查表](#三opcode-速查表)
- [四、NPC 自定义编码参考](#四npc-自定义编码参考)
  - [4.1 ALU 操作码 (alu_op[4:0])](#41-alu-操作码-alu_op40)
  - [4.2 分支类型 (br_type[2:0])](#42-分支类型-br_type20)
  - [4.3 访存类型 (lsu_type[2:0])](#43-访存类型-lsu_type20)
  - [4.4 PC 选择 (pc_sel[1:0])](#44-pc-选择-pc_sel10)
  - [4.5 写回来源 (reg_wdata_src[1:0])](#45-写回来源-reg_wdata_src10)
- [五、funct3 全值速查](#五funct3-全值速查)
- [六、伪指令速查 (Pseudo-instructions)](#六伪指令速查-pseudo-instructions)

---

## 一、RV32I 完整指令集分类表

### 1.1 R 型（寄存器-寄存器）

> **编码**: `funct7[31:25]` | `rs2[24:20]` | `rs1[19:15]` | `funct3[14:12]` | `rd[11:7]` | `opcode[6:0]`
>
> **opcode** = `0110011`  (0x33)

| 优先级 | 指令 | funct7 | funct3 | ISA 伪代码 | ALU | 说明 |
|:--:|------|:------:|:------:|------|:--:|------|
| ⭐ | `add rd, rs1, rs2` | `0000000` | `000` | `x[rd] = x[rs1] + x[rs2]` | ADD | 加法 |
| ⭐ | `sub rd, rs1, rs2` | `0100000` | `000` | `x[rd] = x[rs1] - x[rs2]` | SUB | 减法 (funct7[5]=1) |
| ⭐ | `sll rd, rs1, rs2` | `0000000` | `001` | `x[rd] = x[rs1] << (x[rs2] & 0x1F)` | SLL | 逻辑左移 |
| ⭐ | `slt rd, rs1, rs2` | `0000000` | `010` | `x[rd] = (int32_t)x[rs1] < (int32_t)x[rs2] ? 1 : 0` | SLT | 有符号小于 |
| ⭐ | `sltu rd, rs1, rs2` | `0000000` | `011` | `x[rd] = x[rs1] < x[rs2] ? 1 : 0` | SLTU | 无符号小于 |
| ⭐ | `xor rd, rs1, rs2` | `0000000` | `100` | `x[rd] = x[rs1] ^ x[rs2]` | XOR | 按位异或 |
| 🔶 | `srl rd, rs1, rs2` | `0000000` | `101` | `x[rd] = x[rs1] >> (x[rs2] & 0x1F)` (逻辑) | SRL | 逻辑右移 |
| 🔶 | `sra rd, rs1, rs2` | `0100000` | `101` | `x[rd] = (int32_t)x[rs1] >>> (x[rs2] & 0x1F)` (算术) | SRA | 算术右移 (funct7[5]=1) |
| ⭐ | `or rd, rs1, rs2` | `0000000` | `110` | `x[rd] = x[rs1] \| x[rs2]` | OR | 按位或 |
| ⭐ | `and rd, rs1, rs2` | `0000000` | `111` | `x[rd] = x[rs1] & x[rs2]` | AND | 按位与 |

> **funct7[5] 规则**：`0` = 标准操作 (add/sll/slt/xor/srl/or/and)，`1` = 变体操作 (sub/—/—/—/—/sra/—/—)

---

### 1.2 I 型 — 算术立即数

> **编码**: `imm[11:0][31:20]` | `rs1[19:15]` | `funct3[14:12]` | `rd[11:7]` | `opcode[6:0]`
>
> **opcode** = `0010011`  (0x13)

| 优先级 | 指令 | funct3 | ISA 伪代码 | ALU | 说明 |
|:--:|------|:------:|------|:--:|------|
| ⭐ | `addi rd, rs1, imm` | `000` | `x[rd] = x[rs1] + sext(imm[11:0])` | ADD | 立即数加法 (无 subi) |
| ⭐ | `slli rd, rs1, shamt` | `001` | `x[rd] = x[rs1] << shamt[4:0]` | SLL | 立即数逻辑左移；RV32I: shamt[5]=0 |
| ⭐ | `slti rd, rs1, imm` | `010` | `x[rd] = (int32_t)x[rs1] < sext(imm) ? 1 : 0` | SLT | 有符号小于立即数 |
| ⭐ | `sltiu rd, rs1, imm` | `011` | `x[rd] = x[rs1] < sext(imm) ? 1 : 0` | SLTU | 无符号小于立即数 |
| ⭐ | `xori rd, rs1, imm` | `100` | `x[rd] = x[rs1] ^ sext(imm[11:0])` | XOR | 立即数异或 |
| 🔶 | `srli rd, rs1, shamt` | `101` | `x[rd] = x[rs1] >> shamt[4:0]` (逻辑) | SRL | 立即数逻辑右移；funct7=0000000 |
| 🔶 | `srai rd, rs1, shamt` | `101` | `x[rd] = (int32_t)x[rs1] >>> shamt[4:0]` (算术) | SRA | 立即数算术右移；funct7=0100000 |
| ⭐ | `ori rd, rs1, imm` | `110` | `x[rd] = x[rs1] \| sext(imm[11:0])` | OR | 立即数或 |
| ⭐ | `andi rd, rs1, imm` | `111` | `x[rd] = x[rs1] & sext(imm[11:0])` | AND | 立即数与 |

> **RV32I 约束**：`slli`/`srli`/`srai` 中 `instr[25]`(shamt[5]) 必须为 0。ALU 通过 `src2[4:0]` 只取低 5 位。
> **srli/srai 区分**：两者 funct3 相同(=101)，靠 funct7[5] 区分（0=srli, 1=srai）。

---

### 1.3 I 型 — 加载

> **opcode** = `0000011`  (0x03)

| 优先级 | 指令 | funct3 | ISA 伪代码 | LSU |
|:--:|------|:------:|------|:--:|
| 🔶 | `lb rd, offset(rs1)` | `000` | `x[rd] = sext(M[x[rs1] + sext(offset)][7:0])` | LB |
| 🔶 | `lh rd, offset(rs1)` | `001` | `x[rd] = sext(M[x[rs1] + sext(offset)][15:0])` | LH |
| ⭐ | `lw rd, offset(rs1)` | `010` | `x[rd] = M[x[rs1] + sext(offset)][31:0]` | LW |
| 🔶 | `lbu rd, offset(rs1)` | `100` | `x[rd] = zero_ext(M[x[rs1] + sext(offset)][7:0])` | LBU |
| 🔶 | `lhu rd, offset(rs1)` | `101` | `x[rd] = zero_ext(M[x[rs1] + sext(offset)][15:0])` | LHU |

---

### 1.4 I 型 — jalr

> **opcode** = `1100111`  (0x67)；**funct3** = `000`

| 优先级 | 指令 | ISA 伪代码 | pc_sel | 说明 |
|:--:|------|------|:--:|------|
| ⭐ | `jalr rd, rs1, offset` | `t = pc+4; pc = (x[rs1] + sext(offset)) & ~1; x[rd] = t` | `10` (jalr) | 函数返回/间接跳转；`rd=x0, rs1=ra, off=0` → `ret` |

> `jalr` 的 funct3 官方规定为 `000`（部分第三方手册误标为 `010`，以 RISC-V 官方 spec 和 NEMU 源码为准）。

---

### 1.5 S 型（存储）

> **编码**: `imm[11:5][31:25]` | `rs2[24:20]` | `rs1[19:15]` | `funct3[14:12]` | `imm[4:0][11:7]` | `opcode[6:0]`
>
> **opcode** = `0100011`  (0x23)

| 优先级 | 指令 | funct3 | ISA 伪代码 |
|:--:|------|:------:|------|
| 🔶 | `sb rs2, offset(rs1)` | `000` | `M[x[rs1] + sext(offset)][7:0] = x[rs2][7:0]` |
| 🔶 | `sh rs2, offset(rs1)` | `001` | `M[x[rs1] + sext(offset)][15:0] = x[rs2][15:0]` |
| ⭐ | `sw rs2, offset(rs1)` | `010` | `M[x[rs1] + sext(offset)][31:0] = x[rs2][31:0]` |

---

### 1.6 B 型（分支）

> **编码**: `imm[12\|10:5][31:25]` | `rs2[24:20]` | `rs1[19:15]` | `funct3[14:12]` | `imm[4:1\|11][11:7]` | `opcode[6:0]`
>
> **opcode** = `1100011`  (0x63)；**跳转范围**: ±4 KiB

| 优先级 | 指令 | funct3 | ISA 伪代码 | br_type | 条件 |
|:--:|------|:------:|------|:--:|------|
| ⭐ | `beq rs1, rs2, label` | `000` | `if (x[rs1] == x[rs2]) pc += sext(offset)` | `000` | 相等 |
| ⭐ | `bne rs1, rs2, label` | `001` | `if (x[rs1] != x[rs2]) pc += sext(offset)` | `001` | 不等 |
| 🔶 | `blt rs1, rs2, label` | `100` | `if ((int32_t)x[rs1] < (int32_t)x[rs2]) pc += sext(offset)` | `010` | 有符号小于 |
| 🔶 | `bge rs1, rs2, label` | `101` | `if ((int32_t)x[rs1] >= (int32_t)x[rs2]) pc += sext(offset)` | `011` | 有符号大于等于 |
| 🔶 | `bltu rs1, rs2, label` | `110` | `if (x[rs1] < x[rs2]) pc += sext(offset)` | `100` | 无符号小于 |
| 🔶 | `bgeu rs1, rs2, label` | `111` | `if (x[rs1] >= x[rs2]) pc += sext(offset)` | `101` | 无符号大于等于 |

---

### 1.7 U 型（上立即数）

> **编码**: `imm[31:12][31:12]` | `rd[11:7]` | `opcode[6:0]`

| 优先级 | 指令 | opcode | ISA 伪代码 | ALU | 说明 |
|:--:|------|:------:|------|:--:|------|
| ⭐ | `lui rd, imm` | `0110111` | `x[rd] = sext(imm[31:12] << 12)` | LUI | 加载高20位，低12位填0 |
| ⭐ | `auipc rd, imm` | `0010111` | `x[rd] = pc + sext(imm[31:12] << 12)` | ADD | PC加高20位立即数 |

> `lui` + `addi` 组合加载任意 32 位立即数；`auipc` + `jalr`/`lw` 实现位置无关代码。

---

### 1.8 J 型（跳转）

> **编码**: `imm[20\|10:1\|11\|19:12][31:12]` | `rd[11:7]` | `opcode[6:0]`
>
> **opcode** = `1101111`  (0x6f)；**跳转范围**: ±1 MiB

| 优先级 | 指令 | ISA 伪代码 | pc_sel | 说明 |
|:--:|------|------|:--:|------|
| ⭐ | `jal rd, label` | `x[rd] = pc+4; pc += sext(offset[20:1])` | `01` (jal) | 函数调用；`rd=x0` → 无条件跳转 `j` |

---

### 1.9 系统/CSR 指令

| 优先级 | 指令 | opcode | funct3 | 说明 |
|:--:|------|:------:|:------:|------|
| 🔷 | `ecall` | `1110011` | `000` | 环境调用：触发异常，转入机器模式 |
| 🔷 | `ebreak` | `1110011` | `000` | 断点：触发调试异常 |
| 🔷 | `mret` | `1110011` | `000` | 从机器模式异常返回：pc = mepc |
| 🔷 | `fence` | `0001111` | `000` | 内存/I/O顺序保证（NPC 可作 NOP） |
| 🔷 | `fence.i` | `0001111` | `000` | 指令缓存同步（NPC 可作 NOP） |
| 🔷 | `csrrw rd, csr, rs1` | `1110011` | `001` | 读后写 CSR: `rd=CSR[csr]; CSR[csr]=rs1` |
| 🔷 | `csrrs rd, csr, rs1` | `1110011` | `010` | 读后置位 CSR: `rd=CSR[csr]; CSR[csr]\|=rs1` |
| 🔷 | `csrrc rd, csr, rs1` | `1110011` | `011` | 读后清零 CSR: `rd=CSR[csr]; CSR[csr]&=~rs1` |
| 🔷 | `csrrwi rd, csr, uimm` | `1110011` | `101` | 读后写立即数 CSR |
| 🔷 | `csrrsi rd, csr, uimm` | `1110011` | `110` | 读后置位立即数 CSR |
| 🔷 | `csrrci rd, csr, uimm` | `1110011` | `111` | 读后清零立即数 CSR |

> CSR 寄存器（阶段 3 实现）：`mstatus(0x300)`, `mtvec(0x305)`, `mepc(0x341)`, `mcause(0x342)`, `mie(0x304)`, `mip(0x344)` 等。

---

## 二、32 位指令编码总览

```
        31          25 24     20 19     15 14  12 11      7 6        0
       ┌──────────────┬─────────┬─────────┬───────┬─────────┬──────────┐
R-type │ funct7       │ rs2     │ rs1     │funct3 │ rd      │ opcode   │
I-type │ imm[11:0]              │ rs1     │funct3 │ rd      │ opcode   │
S-type │ imm[11:5]    │ rs2     │ rs1     │funct3 │ imm[4:0]│ opcode   │
B-type │ imm[12|10:5] │ rs2     │ rs1     │funct3 │ imm[4:1|11] │ opcode │
U-type │ imm[31:12]                                    │ rd      │ opcode  │
J-type │ imm[20|10:1|11|19:12]                        │ rd      │ opcode │
       └──────────────┴─────────┴─────────┴───────┴─────────┴──────────┘
```

---

## 三、opcode 速查表

| opcode[6:0] | hex | 类别 | 代表指令 | 指令数 |
|:----------:|:---:|------|---------|:--:|
| `0110011` | 0x33 | R 型 | add, sub, sll, slt, sltu, xor, srl, sra, or, and | 10 |
| `0010011` | 0x13 | I 算术 | addi, slli, slti, sltiu, xori, srli, srai, ori, andi | 9 |
| `0000011` | 0x03 | I 加载 | lb, lh, lw, lbu, lhu | 5 |
| `1100111` | 0x67 | I jalr | jalr | 1 |
| `1110011` | 0x73 | I 系统 | ecall, ebreak, mret, csr* | 10 |
| `0100011` | 0x23 | S 型 | sb, sh, sw | 3 |
| `1100011` | 0x63 | B 型 | beq, bne, blt, bge, bltu, bgeu | 6 |
| `0110111` | 0x37 | U lui | lui | 1 |
| `0010111` | 0x17 | U auipc | auipc | 1 |
| `1101111` | 0x6f | J 型 | jal | 1 |

---

## 四、NPC 自定义编码参考

> 这些编码不属于 RISC-V 规范，是 NPC 内部自定义中间编码，用于 control.v → 各子模块的通信。

### 4.1 ALU 操作码 (alu_op[4:0])

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
| `0_1011~0_1111` | 预留 | — | — |
| `1_0000~1_0111` | M 扩展 | mul, mulh, mulhsu, mulhu, div, divu, rem, remu | 4 |

> **设计决策**：5 位编码（32 种），最高位=0→RV32I(11种)，最高位=1→M扩展(8种)，预留 13 种。

### 4.2 分支类型 (br_type[2:0])

| br_type[2:0] | 指令 | 条件 |
|:----------:|------|------|
| `000` | beq | rs1 == rs2 |
| `001` | bne | rs1 != rs2 |
| `010` | blt | signed rs1 < rs2 |
| `011` | bge | signed rs1 >= rs2 |
| `100` | bltu | unsigned rs1 < rs2 |
| `101` | bgeu | unsigned rs1 >= rs2 |
| `110`/`111` | — | default→0 |

### 4.3 访存类型 (lsu_type[2:0])

| lsu_type[2:0] | 指令 | Load/Store | 操作 |
|:-----------:|------|:--:|------|
| `000` | lw | Load | 整字透传 |
| `001` | lh | Load | 半字 → 有符号扩展 |
| `010` | lb | Load | 字节 → 有符号扩展 |
| `011` | lhu | Load | 半字 → 零扩展 |
| `100` | lbu | Load | 字节 → 零扩展 |
| `101` | sw | Store | 存整字 wmask=1111 |
| `110` | sh | Store | 存半字 wmask=0011/1100 |
| `111` | sb | Store | 存字节 wmask=0001<<addr[1:0] |

### 4.4 PC 选择 (pc_sel[1:0])

| pc_sel[1:0] | PC_next | 对应指令 | opcode |
|:---------:|------|------|:------:|
| `00` | `pc + 4` | 默认顺序 | 其余所有 |
| `01` | `pc + imm_J` | jal | `1101111` |
| `10` | `{alu_result[31:1], 1'b0}` | jalr | `1100111` |
| `11` | `br_taken ? pc+imm_B : pc+4` | 分支 | `1100011` |

### 4.5 写回来源 (reg_wdata_src[1:0])

| reg_wdata_src[1:0] | 写回数据 = | 哪些指令用 |
|:----------------:|--------|---------|
| `00` | alu_result | add, addi, sub, lui, auipc... |
| `01` | mem_rdata | lw, lh, lb, lhu, lbu |
| `10` | pc_plus4 | jal, jalr（函数返回地址） |

---

## 五、funct3 全值速查

| funct3 | R型 (0x33) | I算术 (0x13) | I加载 (0x03) | S型 (0x23) | B型 (0x63) |
|:------:|------|------|-------|------|------|
| `000` | add/sub | addi | lb | sb | beq |
| `001` | sll | slli | lh | sh | bne |
| `010` | slt | slti | lw | sw | — |
| `011` | sltu | sltiu | — | — | — |
| `100` | xor | xori | lbu | — | blt |
| `101` | srl/sra | srli/srai | lhu | — | bge |
| `110` | or | ori | — | — | bltu |
| `111` | and | andi | — | — | bgeu |

> **funct3=000 时** R型靠 `funct7[5]` 区分 add(0) 和 sub(1)
> **funct3=101 时** R型和I型都靠 `funct7[5]` 区分 srl/srli(0) 和 sra/srai(1)

---

## 六、伪指令速查 (Pseudo-instructions)

| 伪指令 | 展开为 | 功能 |
|--------|--------|------|
| `nop` | `addi x0, x0, 0` | 空操作 |
| `mv rd, rs` | `addi rd, rs, 0` | 寄存器拷贝 |
| `not rd, rs` | `xori rd, rs, -1` | 按位取反 |
| `neg rd, rs` | `sub rd, x0, rs` | 取负 |
| `j label` | `jal x0, label` | 无条件跳转 |
| `jr rs` | `jalr x0, rs, 0` | 跳转寄存器 |
| `ret` | `jalr x0, ra, 0` | 函数返回 |
| `call label` | `auipc ra, %hi(label); jalr ra, %lo(label)(ra)` | 远调用 |
| `li rd, imm` | `lui` + `addi` (复杂时多条) | 加载立即数 |
| `la rd, sym` | `auipc rd, %pcrel_hi(sym); addi rd, rd, %pcrel_lo(sym)` | 加载地址 |
| `beqz rs, label` | `beq rs, x0, label` | rs==0 跳转 |
| `bnez rs, label` | `bne rs, x0, label` | rs!=0 跳转 |
| `csrr rd, csr` | `csrrs rd, csr, x0` | 读 CSR |
| `csrw csr, rs` | `csrrw x0, csr, rs` | 写 CSR |
