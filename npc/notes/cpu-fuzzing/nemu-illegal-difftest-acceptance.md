# NEMU 非法指令同步异常：Difftest 补齐验收

日期：2026-10-27（Asia/Shanghai）。本任务在用户已自行修改 DUT/NEMU MRET 的 MPP=M 后开始。**本任务没有修改 CPU RTL、生产 C++、公共 AM 或 RT-Thread BSP**；保留用户已有修改，完成 REF 软件和测试的补齐。

## 结论与范围

指定非法指令场景现已通过真实 Difftest：原编码 `0x040002b3` 和规范明确非法的全零编码均能由 REF 独立产生 cause=2、保存自身故障 PC、返回指令位到 mtval，再经真实 AM handler 停止或恢复。没有从 DUT 复制异常计算结果给 REF。

同轮独立 REF 29/29、CPU35/35、当前原生异常 9 组、RT-Thread 启动/timer/睡眠唤醒/抢占回归通过。旧 REF 对同一恢复镜像仍发生原始 `mcause` 失配，负对照有效。

只覆盖 RV32IM/Zicsr 已有支持集、M-only、对齐 Direct mtvec、正常取指/数据响应下的列明场景。不是全部非法编码穷举、完整特权 ISA 或整个 X 阶段完成的声明。

## Root cause 与生产修改

### `nemu/src/isa/riscv32/inst.c`

原活动译码没有合法匹配后的异常兜底；旧 INV 条目在注释块内，无匹配编码沿默认 `s->dnpc=s->snpc` 顺序前进。新增活动兜底：

```c
INSTPAT("??????? ????? ????? ??? ????? ????? ??", illegal, N,
    s->dnpc = isa_raise_intr(2, s->pc);
    cpu.csr[CSR_IDX_mtval] = s->isa.inst;
  );
```

合法 INSTPAT 匹配会跳到 INSTPAT_END，所以不会再进入兜底。只有没有匹配任何已实现指令时才进入此路径。传给 trap 的 EPC 来自 REF 自己的本条 PC，mtval 来自 REF 自己取出的编码；没有使用 DUT 异常字段。

同时为 opcode=0x0f/funct3=000 添加 FENCE 合法条目。此前 FENCE 也通过“无匹配但顺序前进”表现为空操作；新增统一兜底后必须明确将它识别为合法。当前顺序解释器没有等待排空的内存操作，FENCE 无额外状态更新。FENCE.I 尚不属于当前 DUT 支持集，不将它误称为已实现。

### `nemu/src/isa/riscv32/system/intr.c`

公共 trap 路径增加：

```c
cpu.csr[CSR_IDX_mtval] = 0;
```

ECALL/当前 IRQ 没有该类 trap value，应避免留下先前非法指令或软件写入的 mtval。非法指令调用公共入口后，再填写本条编码，因此这两次软件赋值的顺序有意义。已有公共 mepc/mcause/mstatus 更新和用户修改的 MRET 分支保留。

当前只有上述两个生产 C 文件发生本任务修改。没有更改 DPI ABI、REF API、流水线时延、组合路径、ready/valid、reset、flush 或仲裁逻辑。重建了 REF 动态库和当前生产仿真器。

## 独立 REF 红绿验证

新增 `npc/tests/nemu_illegal_ref.py`，直接加载真实 REF，使用公开 Difftest API，不加载 DUT。核对 RV32 状态/内存复制 ABI 后，对每个用例检查 32 个 GPR、PC、8 个现有 CSR 和内存哨兵。

- 合法正对照：ADD/SUB/MUL、移位、FENCE、忽略 rs1/rd 的 FENCE、JALR/BEQ、CSR 读、ECALL、当前 M-only MRET。
- 非法场景：R/移位 funct7、Load/Store/Branch/JALR funct3、错误 SYSTEM 编码、未支持 opcode/FENCE.I、全零，以及关闭中断时仍应进入同步异常。
- 旧值清理：预置非零 mtval，检查 ECALL、timer IRQ、external IRQ 清零。

修复前从**当前用户源码**构建独立基线，得到 **12/29 通过，17 项实际失败，退出 1**。因此该基线已包含用户 MPP 修正，不拿更早的旧二进制冒充当前源代码。修改后同一组用例 **29/29 通过，退出 0**。FENCE 正例防止新兜底误伤合法编码，GPR/内存哨兵检查非法指令没有写入副作用。

## AM 软件与 Difftest 闭环

测试辅助文件 `illegal_am_app.c`、`illegal_am_probe.S`、`run_x_illegal_am.sh` 支持 `ILLEGAL_AM_INST` 和可选 `NPC_X_ILLEGAL_REF_SO`。启用 REF 时检查 DIFFTEST 配置并保存动态库快照；C 与 `.S` 分别通过 CFLAGS、ASFLAGS 接收同一个输入编码。

两种编码分别执行：

| 模式 | 验收结果 |
| --- | --- |
| stop | REF/DUT 异常现场比较通过，真实 AM handler 检查现场后 GOOD TRAP |
| resume | 软件显式修改 mepc，trap.S 恢复并 MRET；年轻 Store 随重新取指执行一次，REF/DUT 最终 GOOD TRAP |
| retry | 故意保留原 mepc，第二次进入 handler 按 `count=2` 拒绝，双方 BAD TRAP、退出 1，预期负例通过 |
| default | 真实 simple_trap 接到 EVENT_ERROR 后 panic/halt(1)，双方 BAD TRAP、退出 1，预期错误策略通过 |

最终恢复日志：

```text
原编码：cause=2 epc=80000204 tval=40002b3 x5=77 young=deadbeef
全零  ：cause=2 epc=80000200 tval=0       x5=77 young=deadbeef
恢复后：ILLEGAL_AM PASS explicit-resume count=1 young=77
```

地址不同来自独立镜像布局，不能混作同一动态身份。对同一原编码恢复镜像换用基线 REF，仍在 `pc=0x80000204` 发生 CSR[3] mismatch、退出 1；说明正例没有绕过 Difftest。

首次全零运行因测试 CFLAGS 未同步到 ASFLAGS，实际探针仍含旧编码，触发软件 tval 断言；已从反汇编定位。失败目录 `zero/` 保留，只使用修正后的 `reserved-final/` 与 `zero-final/` 支持最终 AM 结论，没有为了通过测试修改 REF/RTL 的判断。

## 当前核回归与证据

| 检查 | 目录/文件 | 本轮结果 |
| --- | --- | --- |
| 总目录、基线/修复 REF、红绿 JSON、任务范围补丁 | `npc/build/nemu-illegal-fix-20261027-7Qi6Kq/` | `ref-red.*`、`ref-green.*`、`task-changes.patch` |
| 当前原生异常、负对照、VCD/CSV | `npc/build/x-illegal-ref-fix-3AcSN3/` | 9 组、2572 行资格核对、3 个负对照通过；场景 4/5/6 仍是明确注入 |
| 两种编码最终 AM Difftest | 上述目录的 `reserved-final/`、`zero-final/` | 8 种模式匹配预期；每组有 `am-options.txt` 和 REF 快照 |
| 用户当前 M-only MRET | 总目录 `user-mret-check.log` | 3 个已有本轮合格 VCD/CSV 返回实例 post mstatus=0x1888，MPP=3 |
| CPU | `npc/build/e-regression-30hAjQ/` | 35/35 Difftest；既有有效 Load 错误 fail-stop 负例符合 E 合同 |
| RT 延时/抢占 | `npc/build/rtthread-delay-sGYDE9/` | off 预期不完成；diagnostic/tick/host GOOD TRAP，timer/调度/原生波形检查及负对照通过 |
| 普通 RT 启动/shell | 上述目录 `boot-zG47XT/` | off/restore/host 各有界 2000000 拍，main/shell/help/ps/timer 检查通过 |

CPU 回归复用并记录 35 个现成二进制，没有重新构建这些 CPU C 程序。RT 构建独立 guest/model，未覆盖用户 BSP 镜像。没有运行 CoreMark/MicroBench。

测试开始前记录当前全部 RTL/头文件摘要，最后再次核对；本任务没有改动用户 CPU 核。MPP 的修正是用户在本任务开始前完成，本轮只是重建并验收。原生波形资格按本次配置重新执行；不以同源字段一致性代替完整 CPU 正确性结论。

一次独立只读评审未发现本任务新增的阻塞问题，核对了生产修改、固定期望测试、当前文件摘要、REF 库一致性和最终日志；评审者没有编辑文件或重跑仿真，其波形结论来自本轮检查器日志，不是额外独立 VCD 复验。

## 可复现命令

在 `ysyx-npc` 容器内、工作目录 `npc/`：

```bash
source container/env.sh
make -C "$NEMU_HOME" -j2 git_commit=
NPC_BUILD_JOBS=2 bash container/build.sh

run=$(mktemp -d "$PWD/build/x-illegal-ref-XXXXXX")
python3 tests/nemu_illegal_ref.py \
  --ref "$NEMU_HOME/build/riscv32-nemu-interpreter-so" --out "$run/ref.json"
bash tests/run_x_illegal.sh "$run"

mkdir "$run/reserved" "$run/zero"
NPC_X_ILLEGAL_REF_SO="$NEMU_HOME/build/riscv32-nemu-interpreter-so" \
  bash tests/run_x_illegal_am.sh "$run/reserved"
NPC_X_ILLEGAL_INST=0x00000000 \
NPC_X_ILLEGAL_REF_SO="$NEMU_HOME/build/riscv32-nemu-interpreter-so" \
  bash tests/run_x_illegal_am.sh "$run/zero"

NPC_BUILD_JOBS=2 bash tests/run_e_regression.sh
bash tests/run_rtthread_delay.sh
```

不要把已有验收目录当作可覆盖的输出。当前新增软件用例必须绑定构建后的 REF，单改源文件不会更新已加载或已复制的动态库。

## 未完成范围

- 保留编码的行为由平台进一步约束；本项目选择把未支持编码报告为非法，不能据此宣称所有保留编码都由基础 ISA 强制 trap。mtval 返回指令位是本项目选择的合法策略。
- 未补齐 CSR 访问权限、只读/不存在 CSR 的架构异常、完整 M-only WARL、未对齐、IF/D 访问错误的精确 trap、Vectored mtvec 或 S/U 模式。
- 当前生产 DPI 的 WB valid 仍来自 wb_fire，普通退休/同步异常尚未用独立 ABI 字段区分。该事件合同以及直接的 trap 后 PC 检查可继续完善；本轮没有改变它，也不将事件计数称作严格 ISA 退休数。
- NEMU 的统计指令数不能替代硬件 minstret 正确性的验证。
