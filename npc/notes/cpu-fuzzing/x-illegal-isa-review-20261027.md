# 非法指令与 M-only trap 返回：ISA 逐项复核

日期：2026-10-27（Asia/Shanghai）。本轮重建和仿真全部在现有 `ysyx-npc` 容器内进行，构建并行度 2；没有修改生产 RTL、C++、NEMU 或公共 AM 实现。仅在独立 build 目录生成附加测试输入、驱动与检查器，并补充本记录。

后续状态：用户已自行修正 DUT/NEMU 的 MRET MPP，新任务补齐 REF 非法指令入口并完成指定场景 Difftest。最新证据见 [NEMU 补齐验收](nemu-illegal-difftest-acceptance.md)。下文失败结果对应本次较早的 ISA 复核版本，保留为问题发现记录。

## 结论

**指定非法指令的检测、异常进入和软件处理检查通过；完整 trap 进入/返回机制不能标记为 ISA 全部符合。** 当前还有两个不同问题：

1. 当前 NEMU REF 未把不匹配编码转换成同步异常。独立 REF 单步和真实 AM 镜像 Difftest 均复现；全零编码也未产生 cause=2。
2. 按当前核仅支持 M 模式的定义，RTL 与 REF 的 MRET 都把 MPP 清成 0，不符合 M-only 的 MPP 规则。实际 DUT 波形在 6 个返回实例中均观察到该差异。本轮没有修改它。

这里的“符合”仅指列明的场景和检查项，不是完整 ISA 合规认证。CPU35、CoreMark、MicroBench、RT-Thread 本轮没有重跑。

## 官方规范与实现选择

| 项目 | 规范/本项目合同 | 本轮判断 |
| --- | --- | --- |
| `0x040002b3` | opcode=0x33、funct7=2、funct3=0、rd=x5；不属于当前 RV32IM/Zicsr 支持集。项目选择为这种未支持编码产生 cause=2 | 可用于本项目异常策略验收；不能声称所有保留编码在全部 RISC-V 实现上都必须 trap |
| 保留编码 | RV32I 2.1 §1.1.2 将 reserved instruction 行为定义为 UNSPECIFIED，可由平台进一步约束 | 修正把“未支持”直接等同于“基础规范一律强制 trap”的表述 |
| 全零编码 | 规范明确保证自然对齐的全零 32 位字可用作非法指令；当前无 16 位指令扩展，按 32 位处理 | 增加该输入作标准保证非法的对照 |
| `mcause` | 非法指令异常编号 2，Interrupt 位=0 | DUT 两种输入均符合；REF 两种输入均未更新为 2 |
| `mepc` | 保存遇到异常的指令地址 | DUT 原生/AM 场景符合 |
| `mtval` | 非法指令时允许为 0，或按规范返回故障指令位 | DUT 选择返回指令位；这是允许的选择，不能把 REF 返回 0 单独判为 ISA 违反 |
| trap 状态栈 | 从 M 进入 M：MPP=M、MPIE=原 MIE、MIE=0 | 本轮选定状态符合 |
| Direct `mtvec` | 同步异常跳到 BASE | 当前对齐 Direct 配置符合；未测试 Vectored/WARL |
| 普通副作用 | 故障指令不正常退休/写回，较老指令结果保留，年轻指令取消 | 原生预期序列、GPR/Store 检查通过；不代表已验证硬件 `minstret`，当前未实现完整计数体系 |
| 软件恢复 | 软件显式修改 mepc；MRET 使用保存的 mepc | PC 恢复链路通过，但不代表 MRET 的所有状态位符合规范 |
| M-only MRET 的 MPP | xRET 后 xPP 设为实现支持的最低特权级；只有 M 时应为 3。MPP 也不应表示未实现的 U 模式 | DUT/REF 当前都得到 0，未通过 |

规范来源：

- [RV32I 2.1，Base Instruction Formats](https://docs.riscv.org/reference/isa/v20260120/unpriv/rv32.html)：reserved instruction 边界。
- [官方 Unprivileged Introduction](https://docs.riscv.org/reference/isa/v20240411/unpriv/intro.html)：全零编码明确非法的规则。该条引用固定官方 20240411 页面；本轮 20260120 同名页面未能读取，不据此推断规则变更。
- [Machine-Level ISA 1.13](https://docs.riscv.org/reference/isa/v20260120/priv/machine.html)：§2.1.1.6.1 状态栈、§2.1.1.7 mtvec、§2.1.1.14 mepc、§2.1.1.15 mcause、§2.1.1.16 mtval、§2.1.3.2 trap 返回。

## 本次产物与实际结果

总目录：`npc/build/x-illegal-isa-20261027-iaccDQ/`。

| 检查 | 产物 | 实际结果 |
| --- | --- | --- |
| 原编码原生测试 | 根目录 `case-*`、`random-*`、`native-summary.log` | 9 个配置及 3 个负对照通过，2572 行 VCD/CSV 核对 |
| 全零编码原生测试 | `zero/` 下同名文件 | 9 个配置及 3 个负对照通过，2572 行 VCD/CSV 核对 |
| 原编码真实 AM handler | 根目录 `guest-*`、`am-summary.log` | stop/resume 通过；retry/default 按指定失败行为验收通过 |
| 全零编码真实 AM handler | `zero/guest-*`、`zero/am-summary.log` | 同上四种模式匹配预期 |
| 原/全零客户镜像接当前 REF | 两级目录各自的 `ref-guest.log`、`.exit` | 均退出 1，分别在 PC=0x80000204 / 0x80000200 报 CSR[3] mismatch |
| 独立 REF 单步 | `ref_single_step.py`、`ref-single-step.json`、`.log`、`.exit` | ADD/SUB/MUL/ECALL 正对照通过；未支持 R、全零、M-only MRET 三项失败；检查器退出 1 |
| DUT M-only 返回规则 | `check_mret_m_only.py`、`mret-rule-check.log`、`.exit` | 两种编码各基本/seed1/seed73，共 6 个 MRET 实例不满足 MPP 规则；退出 1 |
| 生产源码保持 | `production-before.sha256`、`final-production-hashes.log` | 已记录的 RTL/头文件、C++ 和 REF 译码/trap 源文件摘要不变 |

原生正例共 18 个配置、5144 行 VCD/CSV；三种负对照各跑两组。场景 4/5/6 仍是明确标记的寄存器注入，不是自然程序覆盖。日志字段与原生波形的身份和采样边界按现有检查器本轮重新核对；同源一致性不等于独立正确性证明。

全零测试是现有测试的派生输入：只把 BAD_INST/软件期望值/探针 `.word` 改为全零；派生脚本、SV、C/汇编和检查器全部保留在 `zero/`，没有替换已有 `tests/` 文件。软件明确跳过本例已知的四字节测试输入，不推广成所有非法编码都统一加 4。

独立 REF 驱动只使用 NEMU 的公开 Difftest API；先核对状态/内存复制回环，再执行每个输入。它不读取 DUT 或同步 DUT 结果。其 `mtval=指令位` 期望用于本项目选定的一致性合同，不将另一项合法 mtval 策略直接判为 ISA 违规。首次驱动缺少 readline 全局加载的失败保存在 `*.loader-attempt1`，最终结果来自补齐驱动依赖后的日志。

## 最早可观测的 MRET 差异与定位

根目录 `case-1.csv` 的 cycle=262（其余随机配置周期不同）：

```text
WB PC          = 0x80000218
WB instruction = 0x30200073  (MRET)
before mstatus = 0x00001880
after  mstatus = 0x00000088
expected       = 0x00001888  (M-only)
after PC       = 0x80000028  (软件设定的恢复地址，正确)
```

该行 pre/post 状态已与本轮原生 VCD 核对。这是本例直接观察到的返回状态差异，不意味着已穷举所有 CSR WARL 缺陷。

生产 RTL `src/vsrc/src/csr.v` 的 MRET 分支固定拼接 `2'b00` 到 MPP；当前 NEMU `src/isa/riscv32/inst.c` 的 MRET 分支也执行 `mstatus_val & ~(3 << 11)`。因此两边同时得到 0，既有 Difftest 可以一致但不满足 M-only 规范。旧专项检查了 PC 恢复和程序结果，没有检查这个返回后 MPP 位；旧 PASS 不能扩展为完整特权符合。

后续若继续采用 M-only，最小修改方向是 MRET 的 MPP 保持 M，并约束 CSR 写不能引入未实现的特权级；未来真正实现 U/S 后再按实现支持的最低级和权限恢复。应分别用规范驱动 DUT/REF，而不是只让双方相互匹配。按用户约定，本轮只报告，不直接修改 RTL。

## 复验入口

容器内、工作目录 `npc/`，基础验证使用新的空证据目录，不能覆盖本次产物：

```bash
run=$(mktemp -d "$PWD/build/x-illegal-XXXXXX")
bash tests/run_x_illegal.sh "$run"
NPC_BUILD_JOBS=2 bash container/build.sh
bash tests/run_x_illegal_am.sh "$run"
```

本次附加输入/检查器的确切版本和原始命令留在总目录。独立 REF 检查可在 `npc/` 下调用：

```bash
python3 build/x-illegal-isa-20261027-iaccDQ/ref_single_step.py /tmp/ref-step-result.json
python3 build/x-illegal-isa-20261027-iaccDQ/check_mret_m_only.py \
  build/x-illegal-isa-20261027-iaccDQ/case-1.csv
```

当前两条检查都预期退出 1，不能作为通过结果。后者检查的是已有 CSV；修改 RTL 后必须重新构建、采集并核对新 VCD/CSV，不能只修改日志数值。生成的全零 runner 同样有目录保护；再次执行须复制派生源到新的空目录。
