# RISC-V 异常与中断 Cause 编号参考

整理日期：2026-10-22。

依据《The RISC-V Instruction Set Manual, Volume II: Privileged Architecture》：Machine-Level ISA 1.13，以及 H 扩展 1.0。本文记录规范编号与使用边界，不表示当前 NPC RTL 已实现或通过验收。

## 1. Exception、Interrupt 与 mcause

| 术语 | 含义 |
| --- | --- |
| Exception（异常） | 与当前指令相关的同步事件，例如非法指令、访存失败、ECALL |
| Interrupt（中断） | 异步事件，例如定时器中断、外部中断 |
| Trap（陷入） | 异常或中断引起的控制转移 |
| Cause | 标识陷入原因的架构编号 |

对于 RV32，`mcause[31]` 为中断标记，`mcause[30:0]` 为原因编号：

```text
mcause[31] = 0：同步异常
mcause[31] = 1：中断

非法指令异常：mcause = 0x00000002
M-mode 定时器中断：mcause = 0x80000007
```

RV64 的中断标记位为 `mcause[63]`。低位 cause 编号与完整 `mcause` 值需要区分。[规范：Machine Cause Register](https://docs.riscv.org/reference/isa/v20260120/priv/machine.html)

## 2. 同步异常编号：基础特权规范

以下各项的 `mcause.Interrupt=0`，编号以十进制表示。AMO 表示原子内存操作；是否支持某类异常取决于实现的特权模式、扩展和平台要求。

| Cause | 异常名称 | 适用说明 |
| ---: | --- | --- |
| 0 | 指令地址未对齐 | 不满足指令地址对齐要求 |
| 1 | 指令访问错误 | 取指访问失败或被拒绝 |
| 2 | 非法指令 | 非法编码或按相应规则禁止执行的指令/CSR 访问 |
| 3 | 断点异常 | EBREAK 或相关断点机制 |
| 4 | Load 地址未对齐 | 数据读取地址未对齐 |
| 5 | Load 访问错误 | 数据读取失败或被拒绝 |
| 6 | Store/AMO 地址未对齐 | 数据写入或原子访问地址未对齐 |
| 7 | Store/AMO 访问错误 | 数据写入或原子访问失败、被拒绝 |
| 8 | U-mode 环境调用 | U 模式执行 ECALL |
| 9 | S-mode 环境调用 | S 模式执行 ECALL |
| 10 | 保留 | H 扩展另有定义，见第 3 节 |
| 11 | M-mode 环境调用 | M 模式执行 ECALL |
| 12 | 指令缺页异常 | 指令地址转换发生页故障 |
| 13 | Load 缺页异常 | 数据读取的地址转换发生页故障 |
| 14 | 保留 | — |
| 15 | Store/AMO 缺页异常 | 数据写入或原子访问的地址转换发生页故障 |
| 16 | 双重 trap 异常 | 相关双重 trap 扩展；不能据此推断任意嵌套 trap 都报告 16 |
| 17 | 保留 | — |
| 18 | 软件检查异常 | 相关软件检查扩展 |
| 19 | 硬件错误异常 | 相关硬件错误机制 |
| 20～23 | 保留 | H 扩展另有定义，见第 3 节 |
| 24～31 | 自定义用途 | 由实现定义 |
| 32～47 | 保留 | — |
| 48～63 | 自定义用途 | 由实现定义 |
| ≥64 | 保留 | — |

编号来自 Machine-Level ISA 的 Table 6；同一条指令存在多个候选异常时，选择顺序参见 Table 7，不能根据 cause 数值大小决定优先级。[规范：Machine-Level ISA](https://docs.riscv.org/reference/isa/v20260120/priv/machine.html)

## 3. H 虚拟化扩展补充的同步异常

启用 H 扩展时，以下编号具有对应含义。Guest-page fault 与虚拟化的 G-stage 地址转换有关。

| Cause | 异常名称 |
| ---: | --- |
| 10 | VS-mode 执行 ECALL |
| 20 | 指令 Guest-page fault |
| 21 | Load Guest-page fault |
| 22 | 虚拟指令异常 |
| 23 | Store/AMO Guest-page fault |

H 扩展下，编号 8 对应 U/VU-mode ECALL，编号 9 对应 S/HS-mode ECALL，编号 11 仍为 M-mode ECALL。参见 H 扩展的 Table 7，异常优先级参见 Table 8。[规范：H 扩展 Trap Cause Codes](https://docs.riscv.org/reference/isa/v20260120/priv/hypervisor.html)

## 4. 中断编号

### 4.1 基础特权规范

以下各项的 `mcause.Interrupt=1`。完整值列针对 RV32。

| Cause | 中断类型 | RV32 完整 mcause |
| ---: | --- | --- |
| 1 | S-mode 软件中断 | `0x80000001` |
| 3 | M-mode 软件中断 | `0x80000003` |
| 5 | S-mode 定时器中断 | `0x80000005` |
| 7 | M-mode 定时器中断 | `0x80000007` |
| 9 | S-mode 外部中断 | `0x80000009` |
| 11 | M-mode 外部中断 | `0x8000000b` |
| 13 | 计数器溢出中断 | `0x8000000d` |
| ≥16 | 平台用途 | 由平台分配 |

基础表中，编号 0、2、4、6、8、10、12、14、15 为保留编号；H 扩展补充分配其中部分编号。编号 13 与相关计数器溢出扩展有关。[规范：Machine-Level ISA Table 6](https://docs.riscv.org/reference/isa/v20260120/priv/machine.html)

### 4.2 H 扩展补充的中断

| Cause | 中断类型 | RV32 完整 mcause |
| ---: | --- | --- |
| 2 | VS-mode 软件中断 | `0x80000002` |
| 6 | VS-mode 定时器中断 | `0x80000006` |
| 10 | VS-mode 外部中断 | `0x8000000a` |
| 12 | S-mode Guest 外部中断 | `0x8000000c` |

此表列出 M/HS 级观察的 H 扩展编号；委托到 VS 模式时，部分编号会转换。外部中断 cause 也不等同于中断控制器分配给各设备的源 ID。[规范：H 扩展](https://docs.riscv.org/reference/isa/v20260120/priv/hypervisor.html)

## 5. 对照 NPC 的异常载荷设计

以下是流水线设计约定，不是 ISA 规定必须存在的物理信号。

| 字段 | RV32 设计含义 |
| --- | --- |
| `stage_valid` | 当前流水线槽位是否有效 |
| `exc_valid` | 当前有效指令是否携带异常 |
| `exc_cause` | 当前指令选定的同步异常原因编号 |
| `exc_tval` | 随异常传递的信息，最终用于更新 mtval |
| 指令 PC | 随指令传递，异常发生时用于更新 mepc |

注意以下边界：

- **cause=0 是合法异常编号。** 是否存在异常必须看 `exc_valid`，不能用 `exc_cause != 0` 判断。
- 本级合并的是同一条指令携带的上游异常与本级新增异常；`cause/tval` 应成组选择，并遵守异常优先级。
- 异常指令需要保留有效槽位到 WB 处理；若被更老指令 kill，指令及其异常信息一起取消。正常 GPR/CSR 写入、访存与设备副作用需要另外门控。
- 对非法指令，可选择把 32 位指令编码记录到 `exc_tval`；规范也允许相应实现将 mtval 写零。tval 不总是地址，其含义取决于异常类型和平台规定。
- ECALL 是合法指令，会主动引发环境调用异常；MRET 是返回指令。正常 MMIO 访问本身不代表发生异常或外部中断。
- 未对齐访问需要明确处理策略，不能仅凭地址未对齐就断言所有实现都必须产生异常。

mtval 的具体报告规则参见 [Machine Trap Value Register](https://docs.riscv.org/reference/isa/v20260120/priv/machine.html)。当前 RTL 实现进度与验收结果另见 [CPU 执行路线](../cpu-fuzzing/cpu.md)。
