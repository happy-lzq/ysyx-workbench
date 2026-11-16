# 非法指令同步异常：原生 RTL、AM 软件与回归验收

日期：2026-10-26（Asia/Shanghai）。本轮只新增测试和记录，没有修改生产 RTL、生产 C++、AM 公共实现、RT-Thread BSP 或 REF。用户修改的当前 RTL 在测试开始前记录摘要，结束后再次核对。

后续 [2026-10-27 ISA 逐项复核](x-illegal-isa-review-20261027.md) 增加了全零编码对照和独立 REF 单步，并确认 M-only 的 MRET 后 MPP=0 是尚未处理的规范差异。本记录中的恢复 PASS 仅覆盖其列明的 PC/程序行为，不能解释为完整 MRET 特权状态符合。`0x040002b3` 的 trap 属于本项目对未支持编码的策略，不能推广为所有保留编码都由基础 ISA 强制 trap。

再后续：[NEMU 非法指令 Difftest 补齐验收](nemu-illegal-difftest-acceptance.md) 已在用户自行修正 MPP 后完成指定场景 REF 闭环和回归；本记录保留 2026-10-26 的历史未通过状态，不作为最新 REF 能力结论。

## 结论和范围

**当前非法指令机制在下述指定原生/AM 场景通过验收；35 项 CPU Difftest、RT-Thread 启动/timer/抢占复验通过。非法指令本身的 REF Difftest 尚未通过。**

范围限定：RV32 M-only、对齐 Direct `mtvec`、正常内存响应、单笔在途端口，非法指令使用 `.word 0x040002b3`（R 型未支持的 funct7，rd=x5）。不是所有非法编码、全部特权/异常行为或 AXI 错误合同的验收。

证据索引：

| 内容 | 目录/文件 |
| --- | --- |
| 本轮总目录、配置、原始 RTL 摘要、lint 和最终摘要检查 | `npc/build/x-illegal-ZpDVN0/` |
| 最终原生测试、VCD、CSV、负对照、测试源码摘要 | `npc/build/x-illegal-ZpDVN0/native-reviewed/` |
| 最终真实 AM 客户程序及独立 ELF/bin/反汇编 | `npc/build/x-illegal-ZpDVN0/am-final/` |
| 35 项 CPU Difftest、镜像/模型/REF 摘要 | `npc/build/e-regression-tlSBZL/` |
| RT 延时/抢占、波形资格、负对照 | `npc/build/rtthread-delay-gjIs9D/` |
| 原 BSP 启动、shell、timer 复验 | `npc/build/rtthread-delay-gjIs9D/boot-wdnjAd/` |
| 非法指令 REF 能力检查 | `npc/build/x-illegal-ZpDVN0/ref-illegal-capability.log`、`.exit` |

工具为 Verilator 5.048、bare-metal GCC 13.2.0、容器 clang++；构建并行度 2。全部编译和仿真均在现有 `ysyx-npc` 容器、`/workspace/ysyx-workbench/npc` 下执行。`DIFFTEST/HAS_TIMER` 开启，`DIFF_LOG_ENABLE/ITRACE/WAVE` 关闭；波形由专项测试单独捕获，没有启用全程 Difftest 文件日志。

## 本轮新增的测试

- `tests/x_illegal_tb.sv`：真实 core + 既有 RAM-only DPI 适配器；检查普通退休序列及故障动态身份、CSR、RF、Store 次数、redirect/kill、IRQ/MRET。
- `tests/check_x_illegal.py`：核对原生 VCD 与测试台 CSV 的 pre/post-edge 字段，并检查选定异常状态语义。
- `tests/run_x_illegal.sh`：构建、9 组正例/配置、负对照及摘要核对。
- `tests/illegal_am_app.c`、`tests/illegal_am_probe.S`：客户程序注册 handler，执行已知非法编码，验证真实 AM 接口与显式恢复。
- `tests/run_x_illegal_am.sh`：在独立目录构建停止、恢复、重复陷入、现有默认错误 handler 四个镜像，使用生产 `build/sim`。

## 原生异常合同与实际结果

程序在非法指令前写 `mtvec`、设置旧 GPR 值、打开 MIE，并完成一条老 Store/Load。故障指令位于 `0x80000024`，handler 位于 `0x80000200`，年轻 Store 的目标预置为 `0xdeadbeef`。

检查：

1. `wb_trap_fire=1` 与 `wb_retire_fire=0`，禁止普通 GPR/CSR 写入；x5 保留 77。
2. `mepc=0x80000024`、`mcause=2`、`mtval=0x040002b3`；MIE 清零、MPIE 保存旧 MIE、MPP=M，其他选定状态位保持。
3. 同一个上升沿 IF PC 更新到 handler，年轻槽位清除，年轻 Store/GPR 没有提前生效，老 Store/Load 结果保留。
4. 所有正常退休按程序预期 PC/inst 序列检查；故障指令不混入普通退休序列。
5. handler 读取真实 CSR。恢复用例由软件显式写 `mepc+=4` 后 MRET；年轻 Store 被重新取回后恰好执行一次。

| 场景 | 周期计数 | 正常退休 | Trap | Store | 特别检查 |
| --- | ---: | ---: | ---: | ---: | --- |
| 0：非法指令进入 handler 后停止 | 236 | 14 | 1 | 1 | 老结果保留，年轻副作用取消 |
| 1：handler 显式修改 mepc 并 MRET | 327 | 19 | 1 | 2 | MRET=1，恢复后重新取指执行 |
| 2：真实 branch 跳过非法编码 | 184 | 12 | 0 | 1 | Branch=1，无错误路径 trap |
| 3：WB 异常与 timer 输入同拍 | 236 | 14 | 1 | 1 | 请求=1、irq_take=0，cause/PC 属于 WB 异常 |
| 4：WB 异常与年轻 MEM/EX 冲突 | 236 | 14 | 1 | 1 | 测试台注入年轻 Store、不同异常载荷、pending taken-branch 请求；均被取消 |
| 5：老 EX branch 与已译码 ID 非法指令 | 184 | 12 | 0 | 1 | 测试台注入 ID 槽；实际 id_exc_valid=1，但不进入后级 trap |
| 6：WB 异常与可执行的年轻 EX branch | 236 | 14 | 1 | 1 | 测试台注入，MEM 为非访存且 mem_allowin=1，EX 被 WB flush 抑制 |
| 1：随机等待，seed=1 | 466 | 19 | 1 | 2 | 恢复链路保持正确 |
| 1：随机等待，seed=73 | 458 | 19 | 1 | 2 | 恢复链路保持正确 |

基本配置：请求等待 3、响应延迟 7。随机配置：请求等待 5、响应延迟 3、随机上限 15。周期为测试台采样循环计数，不能当作硬件性能结论。

场景 4/5/6 是明确标注的测试台寄存器注入，用来覆盖阻塞取指模型较难自然产生的控制碰撞，不能称为自然程序流覆盖。场景 4 有 MEM Store 反压，不能据此单独声称已验证两个可执行 redirect 的年龄仲裁；场景 6 排除这一反压条件。场景 2 只断言 branch 跳过编码且无 trap；已在 ID 检出的异常被取消，由场景 5 单独覆盖。

### 波形资格与负对照

最终 9 组正例共核对 **2572 行** CSV，每行绑定上升沿前后两个原生 VCD 时间点。字段包括 WB PC/inst/异常有效、普通退休/异常事件、IRQ 请求/接受、普通写许可、trap PC/cause/tval、redirect/target、前后 CSR 与年轻槽 valid。检查低时钟/高时钟采样边界，异常元数据与同一故障指令身份绑定。

- 强制放开故障指令 GPR 写许可：被 `faulting instruction normal write/retirement` 拒绝。
- 强制关闭 trap redirect：被 `missing trap redirect` 拒绝。
- 仅损坏副本中故障指令的 CSV `wb_inst`：被原生 VCD/日志身份核对拒绝。

同源信号一致只证明本次采样合同的一致性，不是独立 CPU 正确性证明。程序预期身份、CSR 数值、独立 RAM 结果/次数、软件断言和负例共同构成指定场景证据。本轮没有改变生产 `dpi_wb_event` ABI；这不是同步异常 DPI 合同已完成的声明。

## 真实 AM 软件闭环

使用未修改的 `AM trap.S → __am_irq_handle`，`cause=2` 经现有 default 分发为 `EVENT_ERROR`，由 `cte_init` 注册的具体 handler 处理。客户上下文没有 `mtval` 字段，验收 handler 在入口通过 CSR 指令读取 `mtval`。

| 客户模式 | 预期与结果 |
| --- | --- |
| stop | 检查 cause、mepc、mtval、旧 x5 和年轻内存哨兵，`halt(0)`，GOOD TRAP |
| resume | 检查现场后修改 Context.mepc，真实 trap.S 恢复并 MRET；年轻 Store 随重新取指执行，GOOD TRAP |
| retry（负例） | 故意不修改 mepc；第二次进入同一非法指令 trap，被 `count=2` 检查拒绝，BAD TRAP、退出 1 |
| default（既有错误策略） | 直接注册 `am-tests/src/tests/intr.c` 的真实 `simple_trap`；`EVENT_ERROR → default → panic → halt(1)`，BAD TRAP、退出 1 |

最终恢复用例实际记录：`cause=2 epc=0x80000204 tval=0x040002b3 x5=77 young=0xdeadbeef`，恢复后 `count=1 young=77`。地址取决于独立镜像布局，不能与原生测试固定 PC 混用。

软件 BAD TRAP 的最终 PC 是 `halt()` 中 EBREAK 的地址；原始故障位置是 Context.mepc。负例按明确诊断和非零退出验收，不能只看到 BAD TRAP 就认为 RTL 出错。

## CPU 与 RT-Thread 回归

### CPU

执行既有 `run_e_regression.sh`，复用并记录 35 个现成 `*-riscv32I-npc.bin` 摘要，没有重新构建这些 C 测试镜像。**35/35 在当前 RTL 模型与现有 REF 下通过 Difftest**。请求等待 3、响应延迟 5、随机上限 7、seed=73。

既有有效 Load 错误负例仍按 E 合同 fail-stop，返回 1，并报告 `pc=0x80000004 addr=0x40000000 kind=1`；它没有被误记为精确访问异常已实现。

### RT 两线程延时与抢占

使用独立 BSP/模型，关闭集成 AM apps，保留生产 `single_cycle`、timer/pending、Difftest 和现有 BSP 调度实现。受控 timer 在指定起点之后每 10000 模拟拍产生一次；host 模式使用现有宿主虚拟 timer，结果随宿主调度变化。

| 模式 | 拍数 | Difftest 事件 | IRQ / tick | MRET | 结果 |
| --- | ---: | ---: | ---: | ---: | --- |
| off | 6000000 | 985033 | 0 | 6 | 预期未完成：两线程仍睡眠，退出 5，无 GOOD TRAP |
| diagnostic | 878089 | 147666 | 49 | 74 | A8/B4 完成，GOOD TRAP |
| tick | 878089 | 147666 | 49 | 74 | 忙循环内抢占唤醒 2 次，GOOD TRAP |
| host | 2506469 | 445489 | 41 | 77 | 忙循环内抢占唤醒 5 次，GOOD TRAP |

原生窗口资格：diagnostic 30002 行（MRET5/IRQ4）、tick 20804 行（MRET2/IRQ3）、host 9912 行（MRET1/IRQ2）。Context 来源/目标槽、寄存器/栈、延时下界检查通过；日志身份和调度合同副本损坏负例均被拒绝。Difftest 事件数不等于严格 ISA 正常退休数。

### 普通 RT 启动与 shell

复验未加入新测试应用的 BSP，三个模式各有界运行 2000000 拍，均到达 main、shell 并通过固定输入 `help/ps` 检查。off 无 IRQ；restore 接受 1 次 IRQ、tick=1；host 接受 26 次 IRQ、tick=26。每模式原生窗口核对 2810 行；完整运行每拍检查生产 DPI 的 WB 身份。测试按有界 OS 进展退出，不要求 OS 自己打印 GOOD TRAP。

## REF 与仿真事件的未完成项

能力检查将同一个 `am-attempt2` 恢复镜像加上现有 `-d ../nemu/build/riscv32-nemu-interpreter-so`，在第一条非法编码处退出 1：

```text
ABORT: CSR[3] mismatch at pc=0x80000204
```

`NPC_state.csr[3]` 对应 `mcause`。源码定位到当前 NEMU 活动译码未为该不匹配的 R 编码产生 cause=2；旧兜底 INV 在注释块中，不能按它推断当前 REF 行为。这是本轮最早报告的不一致字段，不是本轮发现并修正了 DUT RTL 错误。

当前原生断言和真实 AM handler 可独立验证 DUT 指定行为，但不等于非法指令 Difftest 通过。后续仍需：

- REF 独立识别非法编码并建立正确同步异常现场，包括本核选择的 mtval 策略。
- 生产 DPI/CycleEvent 明确普通退休、同步 trap、IRQ 的事件语义及 REF 推进合同。现有 `dpi_wb_event` 的 valid 仍取 `wb_fire`，不能将故障处理事件数称为正常退休数。
- IF/D 访问错误、未对齐与 CSR 访问权限、ECALL/EBREAK 分类统一、M-only CSR WARL/权限和完整不可撤销副作用策略继续按 X 路线处理。

本轮未运行 CoreMark/MicroBench，也没有新增 AXI、SoC 或 Linux 支持。RT-Thread 启动通过不证明上述未覆盖机制正确。

## 可复现入口

在容器 `npc/` 下新建证据目录；不要把已有通过目录当作可覆盖的运行输出：

```bash
run=$(mktemp -d build/x-illegal-XXXXXX)
bash tests/run_x_illegal.sh "$run"
NPC_BUILD_JOBS=2 bash tests/run_e_regression.sh
bash tests/run_x_illegal_am.sh "$run"
bash tests/run_rtthread_delay.sh
```

`run_x_illegal_am.sh` 使用当前生产 `build/sim`；上述 CPU 回归入口会先构建该模型。该 AM 专项故意不传 `-d`，REF 能力检查结果单独保存。最终原生/AM 正例与预期失败均需匹配具体检查，不以“进程存活”或已有可执行文件推断通过。

新测试开发中曾修正测试自身的文件句柄位宽判断、SV VCD 无 TOP 前缀的层级解析及 Klib 不支持 `%u` 的日志格式；失败尝试保留在总目录。最终只读评审指出 MEM Store 反压会限制场景 4 的 EX 仲裁覆盖，因此补入无 MEM 反压的场景 6。最终结论只使用 `native-reviewed` 和 `am-final`。生产 RTL 未为这些问题修改。
