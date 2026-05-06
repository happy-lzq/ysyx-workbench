# RISC-V 汇编指令集汇总

> 基于 RV32I 基础整数指令集 + M/A/Zicsr 扩展，适用于 NEMU / NPC 环境。

---

## 目录

- [0. 寄存器 ABI 对照表](#0-寄存器-abi-对照表)
- [1. R 型指令（寄存器-寄存器）](#1-r-型指令寄存器-寄存器)
- [2. I 型指令（立即数-寄存器）](#2-i-型指令立即数-寄存器)
- [3. S 型指令（存储）](#3-s-型指令存储)
- [4. B 型指令（分支）](#4-b-型指令分支)
- [5. U 型指令（上立即数）](#5-u-型指令上立即数)
- [6. J 型指令（跳转）](#6-j-型指令跳转)
- [7. 系统指令](#7-系统指令)
- [8. M 扩展（乘除法）](#8-m-扩展乘除法)
- [9. A 扩展（原子指令）](#9-a-扩展原子指令)
- [10. CSR 指令（Zicsr）](#10-csr-指令zicsr)
- [11. 伪指令（Pseudo-instructions）](#11-伪指令pseudo-instructions)
- [12. 指令格式速查](#12-指令格式速查)

---

## 0. 寄存器 ABI 对照表

> RISC-V 通用寄存器 x0–x31 在函数调用约定 (ABI) 中的标准用途。
> ✅ = 调用者保存 (caller-saved)，调用后可被覆盖；
> ❌ = 被调用者保存 (callee-saved)，函数返回前必须恢复原值。

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

### 寄存器分类速查

| 类别 | 寄存器 | 数量 | 说明 |
|------|--------|:--:|------|
| **参数/返回值** | `a0`–`a7` | 8 | 函数参数 x10–x17，`a0`/`a1` 兼返回值 |
| **临时变量** | `t0`–`t6` | 7 | x5–x7, x28–x31，调用者可自由覆盖 |
| **被调用者保存** | `s0`–`s11` | 12 | x8–x9, x18–x27，函数返回前必须恢复 |
| **特殊用途** | `zero`, `ra`, `sp`, `gp`, `tp` | 5 | x0–x4 |

---

## 1. R 型指令（寄存器-寄存器）

> 格式: `op rd, rs1, rs2` — 两个源寄存器，一个目的寄存器。
> 编码: `funct7[31:25] | rs2[24:20] | rs1[19:15] | funct3[14:12] | rd[11:7] | opcode[6:0]`

| 指令 | 功能 | 操作 (C 语言语义) | 示例 |
|------|------|-------------------|------|
| `add rd, rs1, rs2` | 加法 | `rd = rs1 + rs2` | `add t0, t1, t2` → `t0 = t1 + t2` |
| `sub rd, rs1, rs2` | 减法 | `rd = rs1 - rs2` | `sub t0, t1, t2` → `t0 = t1 - t2` |
| `sll rd, rs1, rs2` | 逻辑左移 (rs2 低5位) | `rd = rs1 << (rs2 & 0x1f)` | `sll t0, t1, t2` → `t0 = t1 << (t2 & 31)` |
| `srl rd, rs1, rs2` | 逻辑右移 | `rd = (uint32_t)rs1 >> (rs2 & 0x1f)` | `srl t0, t1, t2` → 无符号右移 |
| `sra rd, rs1, rs2` | 算术右移 | `rd = (int32_t)rs1 >> (rs2 & 0x1f)` | `sra t0, t1, t2` → 有符号右移（保留符号位） |
| `slt rd, rs1, rs2` | 有符号小于置1 | `rd = (int32_t)rs1 < (int32_t)rs2 ? 1 : 0` | `slt t0, t1, t2` |
| `sltu rd, rs1, rs2` | 无符号小于置1 | `rd = (uint32_t)rs1 < (uint32_t)rs2 ? 1 : 0` | `sltu t0, t1, t2` |
| `xor rd, rs1, rs2` | 按位异或 | `rd = rs1 ^ rs2` | `xor t0, t1, t2` |
| `or rd, rs1, rs2` | 按位或 | `rd = rs1 \| rs2` | `or t0, t1, t2` |
| `and rd, rs1, rs2` | 按位与 | `rd = rs1 & rs2` | `and t0, t1, t2` |

---

## 2. I 型指令（立即数-寄存器）

> 格式: `op rd, rs1, imm` — 一个寄存器 + 一个 12 位有符号立即数。
> 编码: `imm[11:0][31:20] | rs1[19:15] | funct3[14:12] | rd[11:7] | opcode[6:0]`

### 2.1 算术/逻辑立即数

| 指令 | 功能 | 操作 | 示例 |
|------|------|------|------|
| `addi rd, rs1, imm` | 立即数加法 | `rd = rs1 + sign_ext(imm)` | `addi t0, t1, 42` → `t0 = t1 + 42` |
| `slli rd, rs1, shamt` | 立即数逻辑左移 | `rd = rs1 << shamt` (shamt = imm[4:0]) | `slli t0, t1, 3` → `t0 = t1 << 3` |
| `srli rd, rs1, shamt` | 立即数逻辑右移 | `rd = (uint32_t)rs1 >> shamt` | `srli t0, t1, 4` → 无符号右移4位 |
| `srai rd, rs1, shamt` | 立即数算术右移 | `rd = (int32_t)rs1 >> shamt` | `srai t0, t1, 4` → 有符号右移4位 |
| `slti rd, rs1, imm` | 有符号小于立即数置1 | `rd = (int32_t)rs1 < sign_ext(imm) ? 1 : 0` | `slti t0, t1, 10` |
| `sltiu rd, rs1, imm` | 无符号小于立即数置1 | `rd = (uint32_t)rs1 < (uint32_t)sign_ext(imm) ? 1 : 0` | `sltiu t0, t1, 10` |
| `xori rd, rs1, imm` | 立即数异或 | `rd = rs1 ^ sign_ext(imm)` | `xori t0, t1, -1` → 按位取反 |
| `ori rd, rs1, imm` | 立即数或 | `rd = rs1 \| sign_ext(imm)` | `ori t0, t1, 0xff` |
| `andi rd, rs1, imm` | 立即数与 | `rd = rs1 & sign_ext(imm)` | `andi t0, t1, 0x0f` → 取低4位 |

### 2.2 加载指令

| 指令 | 功能 | 操作 | 示例 |
|------|------|------|------|
| `lb rd, offset(rs1)` | 加载字节（有符号扩展） | `rd = sign_ext(M[rs1 + offset][7:0])` | `lb t0, 0(sp)` → 从栈顶加载1字节 |
| `lh rd, offset(rs1)` | 加载半字（有符号扩展） | `rd = sign_ext(M[rs1 + offset][15:0])` | `lh t0, 4(a0)` |
| `lw rd, offset(rs1)` | 加载字（32位） | `rd = M[rs1 + offset][31:0]` | `lw t0, 8(sp)` → 从栈加载32位 |
| `lbu rd, offset(rs1)` | 加载字节（无符号扩展） | `rd = zero_ext(M[rs1 + offset][7:0])` | `lbu t0, 0(a0)` |
| `lhu rd, offset(rs1)` | 加载半字（无符号扩展） | `rd = zero_ext(M[rs1 + offset][15:0])` | `lhu t0, 2(a0)` |

### 2.3 特权/系统（I 型格式）

| 指令 | 功能 | 操作 | 示例 |
|------|------|------|------|
| `ecall` | 环境调用（系统调用） | 触发 Environment Call 异常 | `ecall` → 从 U 模式陷入 M 模式 |
| `ebreak` | 断点 | 触发 Breakpoint 异常 | `ebreak` → 调试用断点 |
| `mret` | 从机器模式异常返回 | `pc = mepc; mstatus.mie = mstatus.mpie; ...` | `mret` → 异常处理函数末尾使用 |
| `fence` | 内存/I/O 顺序保证 | 确保前后内存访问的顺序 | `fence iorw, iorw` |
| `fence.i` | 指令缓存同步 | 同步指令和数据内存 | `fence.i` → 自修改代码后使用 |

---

## 3. S 型指令（存储）

> 格式: `op rs2, offset(rs1)` — 将 rs2 存入内存 [rs1 + offset]。
> 编码: `imm[11:5][31:25] | rs2[24:20] | rs1[19:15] | funct3[14:12] | imm[4:0][11:7] | opcode[6:0]`

| 指令 | 功能 | 操作 | 示例 |
|------|------|------|------|
| `sb rs2, offset(rs1)` | 存储字节 | `M[rs1 + offset][7:0] = rs2[7:0]` | `sb t0, 0(sp)` → 存1字节到栈顶 |
| `sh rs2, offset(rs1)` | 存储半字 | `M[rs1 + offset][15:0] = rs2[15:0]` | `sh t0, 2(a0)` |
| `sw rs2, offset(rs1)` | 存储字 | `M[rs1 + offset][31:0] = rs2[31:0]` | `sw ra, 12(sp)` → 保存返回地址 |

---

## 4. B 型指令（分支）

> 格式: `op rs1, rs2, label` — 比较 rs1 和 rs2，满足条件跳转到 label。
> 编码: `imm[12\|10:5][31:25] | rs2[24:20] | rs1[19:15] | funct3[14:12] | imm[4:1\|11][11:7] | opcode[6:0]`
> 跳转范围: ±4 KiB (12位有符号立即数 × 2)

| 指令 | 功能 | 操作 | 示例 |
|------|------|------|------|
| `beq rs1, rs2, label` | 相等则分支 | `if (rs1 == rs2) pc += offset` | `beq t0, t1, loop` |
| `bne rs1, rs2, label` | 不等则分支 | `if (rs1 != rs2) pc += offset` | `bne t0, zero, error` |
| `blt rs1, rs2, label` | 有符号小于则分支 | `if ((int32_t)rs1 < (int32_t)rs2) pc += offset` | `blt t0, t1, exit` |
| `bge rs1, rs2, label` | 有符号大于等于则分支 | `if ((int32_t)rs1 >= (int32_t)rs2) pc += offset` | `bge t0, t1, done` |
| `bltu rs1, rs2, label` | 无符号小于则分支 | `if ((uint32_t)rs1 < (uint32_t)rs2) pc += offset` | `bltu t0, t1, loop` |
| `bgeu rs1, rs2, label` | 无符号大于等于则分支 | `if ((uint32_t)rs1 >= (uint32_t)rs2) pc += offset` | `bgeu t0, t1, done` |

---

## 5. U 型指令（上立即数）

> 格式: `op rd, imm` — 将 20 位立即数加载到 rd 的高 20 位，低 12 位清零。
> 编码: `imm[31:12][31:12] | rd[11:7] | opcode[6:0]`

| 指令 | 功能 | 操作 | 示例 |
|------|------|------|------|
| `lui rd, imm` | 加载高位立即数 | `rd = imm << 12` (低12位填0) | `lui t0, 0x12345` → `t0 = 0x12345000` |
| `auipc rd, imm` | PC 加高位立即数 | `rd = pc + (imm << 12)` | `auipc t0, 0x1` → `t0 = pc + 0x1000` |

> **常见用法**: `lui` + `addi` 组合加载任意 32 位立即数；`auipc` + `jalr`/`lw` 实现位置无关的跳转/加载。

---

## 6. J 型指令（跳转）

> 格式: `op rd, label` — `jal` 无条件跳转；`jalr` 为 I 型格式。
> 编码: `imm[20\|10:1\|11\|19:12][31:12] | rd[11:7] | opcode[6:0]`
> 跳转范围: ±1 MiB (20位有符号立即数 × 2)

| 指令 | 功能 | 操作 | 示例 |
|------|------|------|------|
| `jal rd, label` | 跳转并链接 | `rd = pc + 4; pc += offset` | `jal ra, func` → 调用函数，ra 保存返回地址 |
| `jalr rd, rs1, offset` | 跳转并链接（寄存器） | `rd = pc + 4; pc = (rs1 + offset) & ~1` | `jalr x0, ra, 0` → 函数返回（等效 `ret`） |

> `jal x0, label` → 无条件跳转（不保存返回地址），伪指令 `j label`。

---

## 7. 系统指令

| 指令 | 功能 | 操作 | 示例 |
|------|------|------|------|
| `ecall` | 环境调用 | 从 U/S 模式请求 M 模式服务 | `ecall` → 触发 syscall |
| `ebreak` | 断点调试 | 触发调试异常 | `ebreak` |
| `mret` | 机器模式异常返回 | `pc = mepc;` 恢复中断使能 | `mret` |
| `sret` | 监管者模式异常返回 | `pc = sepc;` 恢复中断使能 | `sret` |
| `uret` | 用户模式异常返回 | `pc = uepc;` (N 扩展) | `uret` |
| `wfi` | 等待中断 | 暂停 CPU 直到中断到来 | `wfi` → 低功耗等待 |

---

## 8. M 扩展（乘除法）

> RV32M 标准扩展，提供硬件乘除法指令。所有指令均为 R 型。

| 指令 | 功能 | 操作 | 示例 |
|------|------|------|------|
| `mul rd, rs1, rs2` | 乘法（低32位） | `rd = (int32_t)rs1 * (int32_t)rs2` (低32位) | `mul t0, t1, t2` |
| `mulh rd, rs1, rs2` | 有符号乘法高32位 | `rd = ((int64_t)rs1 * (int64_t)rs2) >> 32` | `mulh t0, t1, t2` |
| `mulhsu rd, rs1, rs2` | 有符号×无符号乘法高32位 | `rd = ((int64_t)(int32_t)rs1 * (uint64_t)(uint32_t)rs2) >> 32` | `mulhsu t0, t1, t2` |
| `mulhu rd, rs1, rs2` | 无符号乘法高32位 | `rd = ((uint64_t)rs1 * (uint64_t)rs2) >> 32` | `mulhu t0, t1, t2` |
| `div rd, rs1, rs2` | 有符号除法 | `rd = (int32_t)rs1 / (int32_t)rs2` | `div t0, t1, t2` |
| `divu rd, rs1, rs2` | 无符号除法 | `rd = (uint32_t)rs1 / (uint32_t)rs2` | `divu t0, t1, t2` |
| `rem rd, rs1, rs2` | 有符号取余 | `rd = (int32_t)rs1 % (int32_t)rs2` | `rem t0, t1, t2` |
| `remu rd, rs1, rs2` | 无符号取余 | `rd = (uint32_t)rs1 % (uint32_t)rs2` | `remu t0, t1, t2` |

> 除零时: `div`/`rem` 结果 = UINT32_MAX（全1）；`divu`/`remu` 结果 = UINT32_MAX。

---

## 9. A 扩展（原子指令）

> RV32A 标准扩展，提供硬件原子操作指令。用于多核/多线程同步（自旋锁、无锁数据结构等）。
> 所有原子指令均为 **R 型格式**，opcode = `0101111`。
> **关键特性**: LR/SC 配对实现 CAS（Compare-And-Swap），AMO 实现原子读-改-写。

### 9.1 LR/SC（Load-Reserved / Store-Conditional）

> LR 从内存加载并设置"预留"标记；SC 仅在预留标记仍有效时写入，rd 返回 0（成功）或非0（失败）。
> RV32A 中只有 .W 字操作；RV64A 额外有 .D 双字操作。

| 指令 | 功能 | 操作 | 示例 |
|------|------|------|------|
| `lr.w rd, (rs1)` | 加载预留字 | `rd = M[rs1];` 在地址 rs1 设置硬件预留标记 | `lr.w t0, (a0)` → 从 a0 地址加载并预留 |
| `sc.w rd, rs2, (rs1)` | 条件存储字 | `if (reserved) { M[rs1] = rs2; rd = 0; } else { rd = 1; }` | `sc.w t0, t1, (a0)` → t0=0 成功，t0≠0 失败 |

> **典型 CAS 自旋锁流程**:
> ```asm
> try_lock:
>     lr.w   t0, (a0)        # 加载锁值
>     bnez   t0, try_lock    # 锁已被持有，自旋等待
>     li     t1, 1
>     sc.w   t0, t1, (a0)    # 尝试原子写入
>     bnez   t0, try_lock    # sc 失败，重试
>     fence  r, rw           # acquire 屏障
> ```
>
> ```asm
> unlock:
>     fence  rw, w           # release 屏障
>     sw     x0, (a0)        # 释放锁
> ```

### 9.2 AMO（Atomic Memory Operations）

> 原子地执行: `rd = M[rs1]; M[rs1] = op(rd, rs2);` — 先读出旧值到 rd，再将运算结果写回内存。
> 整个读-改-写过程不可分割。

| 指令 | 功能 | 操作 | 示例 |
|------|------|------|------|
| `amoswap.w rd, rs2, (rs1)` | 原子交换 | `tmp = M[rs1]; M[rs1] = rs2; rd = tmp` | `amoswap.w t0, t1, (a0)` → 原子 swap |
| `amoadd.w rd, rs2, (rs1)` | 原子加法 | `tmp = M[rs1]; M[rs1] = tmp + rs2; rd = tmp` | `amoadd.w t0, t1, (a0)` → 原子加 |
| `amoxor.w rd, rs2, (rs1)` | 原子异或 | `tmp = M[rs1]; M[rs1] = tmp ^ rs2; rd = tmp` | `amoxor.w t0, t1, (a0)` |
| `amoand.w rd, rs2, (rs1)` | 原子与 | `tmp = M[rs1]; M[rs1] = tmp & rs2; rd = tmp` | `amoand.w t0, t1, (a0)` |
| `amoor.w rd, rs2, (rs1)` | 原子或 | `tmp = M[rs1]; M[rs1] = tmp \| rs2; rd = tmp` | `amoor.w t0, t1, (a0)` |
| `amomin.w rd, rs2, (rs1)` | 原子有符号最小值 | `tmp = M[rs1]; M[rs1] = min((int32_t)tmp, (int32_t)rs2); rd = tmp` | `amomin.w t0, t1, (a0)` |
| `amomax.w rd, rs2, (rs1)` | 原子有符号最大值 | `tmp = M[rs1]; M[rs1] = max((int32_t)tmp, (int32_t)rs2); rd = tmp` | `amomax.w t0, t1, (a0)` |
| `amominu.w rd, rs2, (rs1)` | 原子无符号最小值 | `tmp = M[rs1]; M[rs1] = (uint32_t)tmp < (uint32_t)rs2 ? tmp : rs2; rd = tmp` | `amominu.w t0, t1, (a0)` |
| `amomaxu.w rd, rs2, (rs1)` | 原子无符号最大值 | `tmp = M[rs1]; M[rs1] = (uint32_t)tmp > (uint32_t)rs2 ? tmp : rs2; rd = tmp` | `amomaxu.w t0, t1, (a0)` |

### 9.3 A 扩展 funct3 / funct7 编码

> opcode = `0101111` 对所有原子指令统一。

| funct3 | funct7[31:27] | 指令 | 说明 |
|--------|---------------|------|------|
| `010` | `00010` | `lr.w` | 加载预留 (aq=0, rl=0) |
| `010` | `00011` | `sc.w` | 条件存储 (aq=0, rl=0) |
| `010` | `00001` | `amoswap.w` | 原子交换 |
| `010` | `00000` | `amoadd.w` | 原子加 |
| `010` | `00100` | `amoxor.w` | 原子异或 |
| `010` | `01100` | `amoand.w` | 原子与 |
| `010` | `01000` | `amoor.w` | 原子或 |
| `010` | `10000` | `amomin.w` | 原子有符号 min |
| `010` | `10100` | `amomax.w` | 原子有符号 max |
| `010` | `11000` | `amominu.w` | 原子无符号 min |
| `010` | `11100` | `amomaxu.w` | 原子无符号 max |

> funct7[26] = `aq` (acquire), funct7[25] = `rl` (release)。组合使用可实现顺序一致性。

### 9.4 AMO 典型用例

**无锁原子计数器**:
```asm
# a0 = 计数器地址, t1 = 增量
amoadd.w t0, t1, (a0)   # 原子加，t0 返回旧值
```

**原子位标志设置**:
```asm
li     t1, 0x01
amoor.w t0, t1, (a0)     # 原子设置 bit0, t0=旧值
```

**自旋锁（AMO 版本）**:
```asm
acquire:
    li     t0, 1
try:
    amoswap.w t1, t0, (a0)  # 原子交换
    bnez   t1, try          # t1(旧值)≠0，锁被占用
    fence  r, rw
release:
    fence  rw, w
    amoswap.w x0, x0, (a0)  # 写 0 释放锁 (或 sw x0, (a0))
```

---

## 10. CSR 指令（Zicsr）

> 用于读写控制和状态寄存器 (CSR)。NEMU 中 CSR 用于异常/中断处理。

| 指令 | 功能 | 操作 | 示例 |
|------|------|------|------|
| `csrrw rd, csr, rs1` | 读后写 CSR | `tmp = CSR[csr]; CSR[csr] = rs1; rd = tmp` | `csrrw t0, mstatus, t1` |
| `csrrs rd, csr, rs1` | 读后置位 CSR | `tmp = CSR[csr]; CSR[csr] = tmp \| rs1; rd = tmp` | `csrrs t0, mstatus, t1` → 设置 mstatus 位 |
| `csrrc rd, csr, rs1` | 读后清零 CSR | `tmp = CSR[csr]; CSR[csr] = tmp & ~rs1; rd = tmp` | `csrrc t0, mstatus, t1` → 清除 mstatus 位 |
| `csrrwi rd, csr, uimm` | 读后写立即数 CSR | `rd = CSR[csr]; CSR[csr] = zero_ext(uimm)` | `csrrwi t0, mstatus, 0` |
| `csrrsi rd, csr, uimm` | 读后置位立即数 CSR | `rd = CSR[csr]; CSR[csr] = rd \| zero_ext(uimm)` | `csrrsi t0, mie, 0x8` |
| `csrrci rd, csr, uimm` | 读后清零立即数 CSR | `rd = CSR[csr]; CSR[csr] = rd & ~zero_ext(uimm)` | `csrrci t0, mie, 0x8` |

### 常用 CSR 寄存器

| CSR 名称 | 地址 | 功能 |
|----------|------|------|
| `mstatus` | 0x300 | 机器模式状态（全局中断使能等） |
| `mtvec` | 0x305 | 机器模式陷阱向量基址 |
| `mepc` | 0x341 | 机器模式异常程序计数器 |
| `mcause` | 0x342 | 机器模式异常原因 |
| `mtval` | 0x343 | 机器模式异常附加信息 |
| `mie` | 0x304 | 机器模式中断使能 |
| `mip` | 0x344 | 机器模式中断挂起 |
| `misa` | 0x301 | 机器模式 ISA 信息 |

---

## 11. 伪指令（Pseudo-instructions）

> 伪指令由汇编器展开为一条或多条真实指令。

| 伪指令 | 展开为 | 功能说明 |
|--------|--------|----------|
| `nop` | `addi x0, x0, 0` | 空操作 |
| `mv rd, rs` | `addi rd, rs, 0` | 寄存器拷贝 |
| `not rd, rs` | `xori rd, rs, -1` | 按位取反 |
| `neg rd, rs` | `sub rd, x0, rs` | 取负 (补码) |
| `seqz rd, rs` | `sltiu rd, rs, 1` | rs == 0 置1 |
| `snez rd, rs` | `sltu rd, x0, rs` | rs != 0 置1 |
| `sltz rd, rs` | `slt rd, rs, x0` | rs < 0 置1 |
| `sgtz rd, rs` | `slt rd, x0, rs` | rs > 0 置1 |
| `beqz rs, label` | `beq rs, x0, label` | rs == 0 则跳转 |
| `bnez rs, label` | `bne rs, x0, label` | rs != 0 则跳转 |
| `blez rs, label` | `bge x0, rs, label` | rs <= 0 则跳转 |
| `bgez rs, label` | `bge rs, x0, label` | rs >= 0 则跳转 |
| `bltz rs, label` | `blt rs, x0, label` | rs < 0 则跳转 |
| `bgtz rs, label` | `blt x0, rs, label` | rs > 0 则跳转 |
| `bgt rs1, rs2, label` | `blt rs2, rs1, label` | 有符号大于 |
| `ble rs1, rs2, label` | `bge rs2, rs1, label` | 有符号小于等于 |
| `bgtu rs1, rs2, label` | `bltu rs2, rs1, label` | 无符号大于 |
| `bleu rs1, rs2, label` | `bgeu rs2, rs1, label` | 无符号小于等于 |
| `j label` | `jal x0, label` | 无条件跳转 |
| `jr rs` | `jalr x0, rs, 0` | 跳转到寄存器地址 |
| `jal label` | `jal ra, label` | 调用函数（ra=1时展开为 `jal x1, label`） |
| `ret` | `jalr x0, ra, 0` | 函数返回 |
| `call label` | `auipc x1, %hi(label); jalr ra, %lo(label)(x1)` | 远调用 (±2 GiB) |
| `tail label` | `auipc x6, %hi(label); jalr x0, %lo(label)(x6)` | 尾调用 |
| `li rd, imm` | `lui` + `addi` (复杂立即数需多条) | 加载 32 位立即数 |
| `la rd, symbol` | `auipc rd, %pcrel_hi(symbol); addi rd, rd, %pcrel_lo(symbol)` | 加载地址 |
| `csrr rd, csr` | `csrrs rd, csr, x0` | 读 CSR |
| `csrw csr, rs` | `csrrw x0, csr, rs` | 写 CSR |
| `csrs csr, rs` | `csrrs x0, csr, rs` | 设置 CSR 位 |
| `csrc csr, rs` | `csrrc x0, csr, rs` | 清除 CSR 位 |
| `csrwi csr, uimm` | `csrrwi x0, csr, uimm` | 写立即数到 CSR |
| `csrsi csr, uimm` | `csrrsi x0, csr, uimm` | 设置 CSR 位（立即数） |
| `csrci csr, uimm` | `csrrci x0, csr, uimm` | 清除 CSR 位（立即数） |
| `frflags rd` | `csrrs rd, fflags, x0` | 读 FP 标志 |
| `fsflags rs` | `csrrw x0, fflags, rs` | 写 FP 标志 |

---

## 12. 指令格式速查

```
R-type:  funct7 | rs2 | rs1 | funct3 |  rd  | opcode
I-type:  imm[11:0]   | rs1 | funct3 |  rd  | opcode
S-type:  imm[11:5]   | rs2 | rs1 | funct3 | imm[4:0] | opcode
B-type:  imm[12|10:5]| rs2 | rs1 | funct3 | imm[4:1|11] | opcode
U-type:  imm[31:12]              |  rd  | opcode
J-type:  imm[20|10:1|11|19:12]   |  rd  | opcode
```

### opcode 映射

| opcode[6:0] | 类型 | 指令组 |
|-------------|------|--------|
| `0110011` | R | OP (add/sub/sll/slt/xor/or/and/srl/sra) |
| `0111011` | R | OP-32 (RV64 字操作) |
| `0010011` | I | OP-IMM (addi/slli/slti/xori/ori/andi/srli/srai) |
| `0000011` | I | LOAD (lb/lh/lw/lbu/lhu) |
| `1100111` | I | JALR |
| `1110011` | I | SYSTEM (ecall/ebreak/csrr*) |
| `0100011` | S | STORE (sb/sh/sw) |
| `1100011` | B | BRANCH (beq/bne/blt/bge/bltu/bgeu) |
| `0110111` | U | LUI |
| `0010111` | U | AUIPC |
| `1101111` | J | JAL |
| `0001111` | I | FENCE / FENCE.I |

### funct3 映射 (OP / OP-IMM)

| funct3 | OP (R-type) | OP-IMM (I-type) |
|--------|-------------|------------------|
| `000` | ADD / SUB* | ADDI |
| `001` | SLL | SLLI |
| `010` | SLT | SLTI |
| `011` | SLTU | SLTIU |
| `100` | XOR | XORI |
| `101` | SRL / SRA* | SRLI / SRAI* |
| `110` | OR | ORI |
| `111` | AND | ANDI |

> \* SUB 由 funct7[30]=1 区分于 ADD；SRA 由 funct7[30]=1 区分于 SRL。

---

## 参考

- https://soc.ustc.edu.cn/COD/lab1/src/RISCV_ref.pdf
