# M 模式五级流水线：精确同步异常进展路线

更新日期：2026-11-16（Asia/Shanghai）。本文件是当前同步异常的专项进度入口，承接 [CPU 总进展](../cpu-fuzzing/cpu.md) 和 [整体路线第 5 节](cpu-design-roadmap-e-to-linux-dualissue.md)。

2026-10-28 的初始版本仅整理源码；2026-11-01 已实际修改十个既有 CSR 的软件写行为和 NEMU 并重新验收。以下“已完成”均限定于对应记录列明的版本和场景；测试数字、配置和原始产物以验收记录为准。

2026-11-03：用户完成 cause=0 的 JAL、JALR/Branch RTL 接线，本轮补齐 NEMU 六类 Branch 的未对齐异常，并通过真实 REF 检查、生产 RTL 定向 Difftest 和既有 CPU-test 回归；与较老 trap/IRQ 的控制碰撞专项仍待验收。见 [cause=0 验收记录](../exception_interrupt/exception-00-acceptance.md)。

**当前定位：异常载荷、非法编码、固定十个 CSR 的软件写字段，以及 cause=0 的基础闭环已有指定场景验收；完整 M-only 精确异常尚未完成。** 字段合同见 [设计](m-only-csr-fields.md)，CSR 结果见 [验收](../cpu-fuzzing/m-only-csr-acceptance.md)，跳转对齐结果见 [cause=0 验收](../exception_interrupt/exception-00-acceptance.md)。

2026-11-14：当前 MEM 本地对齐/范围检查和后端错误已接入精确 WB trap；指定 DUT 场景在默认与随机等待配置下验收通过。正常访问 Difftest 和既有 cause=0 短回归通过；完整异常 Difftest 首次在未对齐 LH 失配，独立单步确认当前 REF 尚未采用本项目的 RAM 对齐异常策略。本轮未改生产 RTL/C++/REF，详情见 [MEM 异常验收](../exception_interrupt/exception-4_6-load-store-acceptance.md)。

2026-11-16：NEMU 已补齐当前 RAM 完整区间/未对齐策略及无设备 REF 的未映射访问错误，指定异常 Difftest、独立 REF 和 CPU35 通过；RTL 未修改。MMIO 仍采用既有结果同步，没有由此建立独立设备属性模型，见 [REF 修复与 ISA 对照](../exception_interrupt/nemu-mem-exception-fix-acceptance.md)。

## 1. 本阶段范围与完成目标

当前基线为五级顺序单发射、已有 RV32IM/Zicsr 指令及 CSR 子集、M-only、无 C、无 MMU。先在对齐 Direct mtvec 的范围完善共同机制，再为 AXI/SoC 留下错误响应和副作用合同。

需要收敛的同步异常编号为 **0、1、2、3、4、5、6、7、11**。这是一版本项目的目标范围，不表示所有保留编码或未对齐访问在所有 RISC-V 执行环境中都必须采取相同策略。

每种异常都应回答以下问题：

1. 什么条件触发，在哪里检测？
2. 故障属于哪条动态指令，PC、cause、tval 分别是什么？
3. 停顿、redirect 和错误路径取消时，异常信息如何保持或失效？
4. 如何禁止故障指令的普通写入，以及年轻指令的不可撤销副作用？
5. 如何在 WB 保存现场并进入软件，软件选择停止、重试还是恢复？
6. REF 如何独立得到相应行为，测试如何发现错误而不是覆盖错误？

精确性的共同目标：较老指令保留合法已完成的结果；故障指令不正常退休；年轻指令不越过允许的副作用边界；mepc 与故障指令身份一致。

## 2. 当前状态总表

状态含义：“指定场景已验收”不等于穷举或完整 ISA 合规；“部分已有”表示存在可复用逻辑，但不能按该工作包的目标宣称闭环。

本文件使用 M0～M8 标记工作包，原整体路线中的 X-01～X-14 是验收场景编号，两者不混用。

| 工作包 | 对象 | 当前状态 | 当前依据/缺口 |
| --- | --- | --- | --- |
| M0 | 异常运输、WB trap、非法编码 cause=2 | 指定场景已验收 | 原生 RTL、AM handler、独立 REF、Difftest 已有证据；不涵盖完整 CSR 合法性 |
| **M1** | **CSR 访问合法性，仍为 cause=2** | **固定地址表和软件写字段已做指定验收** | DUT/REF 对不支持地址进入非法路径；十地址/六操作及字段已有定向检查。没有 S/U 切换、独立只读 CSR 或完整特权 ISA 合规 |
| M2 | 实际跳转目标未对齐，cause=0 | 基础定向闭环已验收 | JAL ID 检测、Branch/JALR EX final 检测及独立 REF 已接入；现场、恢复及延迟配置通过，较老 trap/IRQ 碰撞专项待验收 |
| M3 | Load/Store 未对齐，cause=4/6；Device 访问属性 | RAM 基础闭环含 REF/Difftest 已验收；Device 有指定场景 | REF 已独立检查 RAM，故障不读写；设备条件仍须靠独立夹具/消费计数，完整属性模型未建立 |
| M4 | 取指访问错误，cause=1 | 待接入 | IF/ID 异常输入目前为 0；有效错误仍进入 faulted 停机 |
| M5 | Load/Store 访问错误，cause=5/7 | 指定输入 Difftest 已验收；完整环境合同待完善 | REF 对 RAM 边界和其未映射地址独立报错；指定后端错误通过，不代表 REF 已独立模拟 NPC 设备，IRQ/总线碰撞仍待补 |
| M6 | M 模式 ECALL，cause=11 | 部分已有，待统一 | 已有 sys_ctrl/WB 系统 trap，但尚未统一为不正常退休的同步异常事件 |
| M7 | 架构 EBREAK，cause=3 | 待迁移测试出口后实现 | 当前 EBREAK 是仿真 HALT 兼容入口，不能直接替换后破坏全部程序退出 |
| M8 | 事件、CSR 字段约束、年龄与不可撤销副作用 | 部分已有，贯穿全程 | 有 WB trap/IRQ 仲裁和保守 D 请求许可；完整事件 ABI、错误事务收尾和通用合同仍待补齐 |

相关源码入口：

- [exception.vh](../../src/vsrc/include/exception.vh)：编号和范围说明；定义不等于实现。
- [control.v](../../src/vsrc/src/control.v)、[id_stage.v](../../src/vsrc/src/id_stage.v)：译码及当前非法编码检测。
- [core_top.v](../../src/vsrc/src/core_top.v)：载荷合并、各级 fire、WB 分流、redirect、错误停机及请求许可。
- [csr.v](../../src/vsrc/src/csr.v)：架构现场、CSR 地址/字段行为、MRET。
- [mem_port.v](../../src/vsrc/src/mem_port.v)、[dpi_mem.v](../../src/vsrc/src/dpi_mem.v)、[memory.cpp](../../src/csrc/src/memory/memory.cpp)：事务身份、错误响应和当前仿真停机。
- [NEMU inst.c](../../../nemu/src/isa/riscv32/inst.c)、[CSR 映射](../../../nemu/src/isa/riscv32/csr.c)、[trap 处理](../../../nemu/src/isa/riscv32/system/intr.c)：独立 REF 行为。

## 3. 共用的处理路径

```text
检测本条指令的异常
    -> 有效指令 PC/inst + exc_valid/cause/tval
    -> 随所属流水槽保持、推进或被取消
    -> 最老故障到达 WB
    -> 禁止普通 GPR/CSR 写入及正常退休
    -> CSR 保存现场，redirect，取消年轻指令
    -> 软件 handler 决定停止、修复、重试或恢复
    -> REF 独立处理并与 DUT 比较
```

- 指令 valid=1 与 exc_valid=1 可以同时成立：它是需要处理的故障指令，不是 bubble。
- cause=0 是有效编号，不能用 cause 是否为零判断有没有异常。
- 多条指令的异常按年龄处理，不能把各级异常 OR 成一个与指令身份无关的请求。
- 同一指令存在多个候选异常时，要按规范及本项目选定的优先级表选择；不能无条件认为“最早流水级检测的原因永远优先”。
- GPR/普通 CSR 写入禁止，不代表禁止 trap 自身对 mepc/mcause/mtval/mstatus 的更新。
- Store/MMIO 副作用可能早于 WB，必须在首次呈现请求前满足许可，不能事后只靠清空 valid 撤销。

### 已有的 M0 证据

异常载荷、非法编码、WB trap/写许可、现场、redirect/kill、部分年龄碰撞、真实 AM 停止/恢复、REF 独立处理已做指定场景验收。

- [当前非法指令 REF/Difftest 验收](../cpu-fuzzing/nemu-illegal-difftest-acceptance.md)：当前主要证据。
- [初始非法指令验收](../cpu-fuzzing/x-illegal-acceptance.md)、[ISA 复核](../cpu-fuzzing/x-illegal-isa-review-20261027.md)：保留问题发现及较早未通过状态。
- [异常载荷承载验收](../cpu-fuzzing/x-payload-acceptance.md)：较早的运输层记录，不代替当前 trap 验收。

用户已修正当前 M-only MRET 后的 MPP=M，已有指定返回实例检查；十个既有 CSR 的普通软件写字段现按 M-only 合同验收，计数器读的周期 Difftest 对齐仍未完成。RAM-only 原生测试和人工注入碰撞，也不能替代真实设备副作用的全部验收。

## 4. 推进顺序与每步验收

```text
M0 已有基础
 -> M1 CSR 合法性
 -> M2 跳转对齐
 -> M3 数据对齐/Device 属性
 -> M4 IF 访问错误
 -> M5 D 访问错误
 -> M6 ECALL 统一
 -> M7 EBREAK 与测试出口迁移
 -> M8 汇总验收与冻结边界
```

**M8 的身份、一次性事件和副作用要求从每一步开始就适用，不是等最后再补。** 上述顺序用于逐项学习和实现，不按 cause 数字大小排序。

### M1：CSR 访问合法性——固定十地址已有专项验收

**目标：** CSR 指令编码合法，但访问行为不允许时，通过既有 cause=2 路径进入 trap。

- [x] 建立当前十个 CSR 的 M-only 支持及字段表，见 [字段合同](m-only-csr-fields.md)；不代表整个特权 CSR 集合已实现。
- [x] 当前 DUT/REF 都支持十地址，包括 mcycle/mcycleh；REF 软件写行为已对齐，周期读值同步仍待专门处理。
- [x] 六类 CSR 的真实读写意图和 rd/rs1/zimm 抑制已有独立 REF 及生产字段程序检查。
- [ ] 本项目选择将未支持 CSR 访问报告为非法；权限不足和对整个只读 CSR 的写入按对应规则处理。
- [x] 当前固定表中字段只读/WARL 的软件写入按合同忽略或合法化，不新增 cause=2。独立只读 CSR 的支持与访问检查另行验收。
- [ ] DUT 产生本条指令的 cause=2、tval=编码，并禁止其普通 GPR/CSR 副作用。
- [ ] REF 将对应访问转为架构 trap，而不是宿主 panic，也不复制 DUT 结果。

**验收：** 合法访问不误报；非法访问 cause/epc/tval 正确；故障 rd 和 CSR 保持；合法只读访问与禁止写入分开；后随年轻 Store/MMIO 不提前生效；关闭 MIE 仍能处理同步异常。先建立表和定向测试，再接线。

### M2：跳转目标未对齐，cause=0

- [x] JAL 在 ID 检测；Branch/JALR 在顶层采用 EX 实时/保存选择后的 final 请求与目标检测。
- [x] 无 C 时按 IALIGN=32 检查；条件分支只有 taken 才检查被采用目标。
- [x] JALR 先按指令语义清 bit0，再检查对齐；不能漏掉 bit1。
- [x] mepc 保存跳转指令自身 PC，tval 保存错误目标地址，已有软件 handler 检查及 MRET 恢复实例。
- [x] 正常 redirect 加入异常门控；故障 JAL/JALR 不写链接寄存器，指定年轻 Store canary 检查通过。
- [x] REF 独立实现相同判断；六类 Branch 已补齐 taken 目标检查，未 taken 保持 snpc。
- [ ] 显式命中与较老 WB trap/实际接受 IRQ 的年龄碰撞，以及错误路径取消场景，记录动态覆盖证据。

2026-11-03 的具体数量、配置、修改前失败与修改后结果见 [验收记录](../exception_interrupt/exception-00-acceptance.md)。本次未采集 VCD，不宣称全部停顿/碰撞状态已覆盖。

**验收：** JAL/JALR、taken/not-taken、目标低位组合、rd=x0、与较老 WB trap/IRQ/停顿相撞；普通合法跳转不回退。

### M3：数据未对齐与 Device 属性，cause=4/6 或访问错误

当前：RAM 对齐/范围策略已完成指定 REF/Difftest 闭环；Device 的独立属性判断仍未完成，参见 [REF 修复与 ISA 对照](../exception_interrupt/nemu-mem-exception-fix-acceptance.md)。

- [x] 固定普通 RAM 首版策略：不透明拆分，按本项目选择报告 Load/Store 地址未对齐。
- [x] 在首次呈现请求前，根据有效地址和访问宽度检查；字节无额外对齐要求，半字检查 bit0，字检查低两位。
- [ ] Device 单独检查区域支持的宽度/对齐；不能透明拆成多次消费，按属性和已选优先级报告访问错误或相应异常。
- [x] 本地已知的对齐/范围故障不产生外部请求，tval 记录原有效地址；后端才识别的设备错误通过一次请求/错误响应结束。
- [x] 当前 RAM 策略的 REF 与定向软件预期一致，CPU35 镜像已回归；`unalign` 实际被编译为字节访问，其 PASS 不能代替原始未对齐指令测试。

**验收：** 正常对齐与错误边界、Load rd 保持、Store 内存不变、Device 请求/消费次数为零；测试证明没有截断地址后悄悄访问另一个位置。

### M4：取指访问错误，cause=1

- [ ] 将存活 IF 错误响应转换为有效异常槽，绑定原请求 PC，tval 为故障取指地址。
- [ ] 已作废 IF 请求的响应只排空；redirect 同拍及随后返回的旧错误不进入架构。
- [ ] 错误包保留更高优先的取指异常，不能被占位指令的非法译码覆盖，也不能执行占位内容产生的普通控制。
- [ ] 与 IF/ID 接收、hold、flush 协同，不丢失、不重复触发。
- [ ] 区分可报告的架构访问错误与模型/协议自身故障；不能简单删除全部 fail-stop 检查。
- [ ] REF 使用一致的可执行地址图和故障条件，正确进入异常。

**验收：** 活路径错误、错路径错误、响应/redirect 同拍、ID 反压、较老访存等待、入口恢复；故障包最终只产生一次 trap，旧错误不影响新路径。

### M5：数据访问错误，cause=5/7

当前：本地范围与指定设备错误序列已通过 Difftest；RAM/未映射地址有独立 REF 检查，完整设备模型与碰撞验收仍待完成，见 [REF 修复与 ISA 对照](../exception_interrupt/nemu-mem-exception-fix-acceptance.md)。

- [x] 响应绑定原 Load/Store 的 PC、有效地址和访问类型，不能取本拍无关的 IF PC。
- [x] 错误表示“事务已经结束但失败”，必须结束等待并传递异常；不能继续依赖 `!mem_resp_err` 将流水线永久卡住。
- [x] 故障 Load 不把返回值写 rd，不正常退休；tval 为故障有效地址。
- [ ] Store/MMIO 从首次呈现到响应和最终处理都保持合法 owner，错误时不自动重发。
- [ ] 明确后端报错是否可能已产生部分外部效果；CSR trap 不能回滚已被设备消费的动作。
- [ ] 软件异常入口和恢复策略有定义，尤其不能用统一 mepc+=4 掩盖可重试/不可重试差异。
- [ ] REF 对地址图或注入故障有独立处理；外部总线模型产生的错误，要对齐环境条件，不复制 DUT 算出的最终异常状态。

**验收：** rd 预置标记、内存/设备计数、旧 WB 同拍完成、响应/IRQ 碰撞、长延迟与有界完成；错误访问无普通退休、无无限等待或重复设备动作。

### M6：统一 M 模式 ECALL，cause=11

- [ ] 将 ECALL 归入统一同步异常合同，而不是仍按普通退休事件解释。
- [ ] mepc 指向 ECALL 自身，tval 按当前策略为零；没有普通 GPR/CSR 写入。
- [ ] 保留 AM 对 yield/syscall 的分发；由现有软件策略决定 mepc 前进，硬件不能再额外加 4。
- [ ] REF 对 ECALL 只推进一次，不能执行它自然 trap 后又重复注入同一异常。
- [ ] 合法 MRET 继续作为正常退休与系统返回，不混入故障指令事件。

**验收：** ECALL 指令身份、非退休语义、软件返回地址、MRET、RT 调度与 timer 交互回归。

### M7：EBREAK 与仿真退出分离，cause=3

- [ ] 先选定并实现测试退出合同：专用观察地址/机制或明确兼容模式，不能假设 tohost 已存在。
- [ ] 迁移 AM halt、既有测试和 REF 的对应观察逻辑，保持成功/失败退出可验证。
- [ ] 普通架构 EBREAK 进入 breakpoint trap，mepc 指向自身，不普通退休。
- [ ] 断点 trap 与测试成功退出分别建立正反例。

**验收：** CPU35/AM/RT 的结束行为不被迁移破坏；真实 EBREAK 不是“遇到就 GOOD TRAP”；REF 与 DUT 一致且不是复制状态。

### M8：共同合同与最终退出关口

- [ ] 明确普通退休、同步 trap、IRQ、合法 xRET redirect 的一次性事件，故障指令不能同时正常退休。
- [ ] 当前生产 DPI valid 来自 wb_fire，仍需完成显式事件语义；不把事件数直接称为 ISA 退休数。
- [ ] 同步异常 EPC 取故障指令，IRQ EPC 取合法恢复边界；正常后继 PC 与 IF 推测/预取 PC 区分。
- [ ] 固定与 trap 相关的 M-only CSR 字段行为：MPP、mepc 对齐、mtvec 支持模式等，未实现模式不能通过任意 CSR 写值伪造。
- [ ] 单指令多异常优先级有表，跨指令按年龄选择；所有普通架构写入与 trap 写入各受正确许可。
- [ ] 更老异常阻止年轻 Store/MMIO 首次呈现；已经取得不可撤销许可的事务在等待/响应/处理阶段不失去 owner。
- [ ] 保持已有 timer pending/irq_take 边界，检查更老 trap、CSR 写、MRET 与 IRQ 的碰撞，不在本文件顺便扩展中断控制器。
- [ ] 新增/变化的观测合同按项目要求做原生 RTL 与日志/VCD 资格核对，并有独立语义期望和负对照；已合格且语义未变的观测可复用。

**最终验收：** 每类故障本身、错误路径、年龄冲突、副作用及软件恢复均有有界正反例；完成适用的独立 REF/Difftest；按变更影响复跑 CPU/RT，必要时再运行长程序。用例通过范围与未实现项必须写入对应验收记录。

## 5. REF、软件和实验的共同要求

| 类型 | REF/软件的处理原则 |
| --- | --- |
| 非法编码、CSR 合法性、对齐检查 | REF 根据同一输入独立判断，不能从 DUT 的 cause/CSR 结果倒推期望 |
| 地址图/外部响应错误 | 对齐环境输入、访问类型和指令身份；选择一种 REF 推进合同，不能先正常执行又重复注入 |
| 外部 IRQ | 保留实际接受边界的注入方式，与同步异常检测分开 |
| AM handler | 明确是停止、重试、修复还是跳过；不能为所有异常统一 mepc+=4 |
| Store/MMIO | 用实际访问/消费次数检查外部效果，GPR/CSR 一致不能代替设备副作用验收 |

当前基础验证入口位于 `npc/tests/`，执行环境仍为现有 `ysyx-npc` 容器。以下命令从容器内 `/workspace/ysyx-workbench/npc` 执行；这些是已有入口，不表示未来工作包专项测试已经写好：

```bash
source container/env.sh
make -C "$NEMU_HOME" -j2 git_commit=
NPC_BUILD_JOBS=2 bash container/build.sh

run=$(mktemp -d "$PWD/build/x-illegal-XXXXXX")
python3 tests/nemu_illegal_ref.py \
  --ref "$NEMU_HOME/build/riscv32-nemu-interpreter-so" --out "$run/ref.json"
bash tests/run_x_illegal.sh "$run"
mkdir "$run/am"
NPC_X_ILLEGAL_REF_SO="$NEMU_HOME/build/riscv32-nemu-interpreter-so" \
  bash tests/run_x_illegal_am.sh "$run/am"
```

CPU 回归入口为 `tests/run_e_regression.sh`，RT 回归入口为 `tests/run_rtthread_delay.sh`。每次修改选择与风险相称的检查，不为纯文档变更启动硬件回归；新增异常需要自己的最小正反例，不能只靠这几个基础入口。

## 6. 本阶段暂不扩展的编号

| 编号/类别 | 后续前提 |
| --- | --- |
| ECALL_U=8、ECALL_S=9 | 真正实现 U/S 模式及相应权限/委派 |
| 页故障 12、13、15 | MMU、Sv32、TLB/PTW |
| H 类同步异常和中断 | 虚拟化扩展；不是当前普通 Linux 路线的先决条件 |
| double trap=16、software check=18、hardware error=19 | 明确采用相应扩展或错误机制 |
| IRQ_* | 属于异步中断，按实际源、pending 和 SoC 接口另行完善 |

MRET 没有单独异常编号；合法返回属于正常指令，非法使用才按相应非法指令规则处理。未实现相关扩展时，不应为了“补全编号表”制造这些异常源。

## 7. 进度更新规则

每个工作包完成后，在本文件更新状态，并新增或更新独立验收记录：

| 更新字段 | 需要记录的内容 |
| --- | --- |
| 实现范围 | 影响的模块、信号和明确未实现项 |
| 动态身份与时序 | PC/inst、请求/响应归属、异常产生和最终处理边界 |
| 验证输入 | 配置、工具、镜像、seed、运行目录与复现命令 |
| 结果 | 正例、预期失败、负对照、回归结果及采样资格范围 |
| 状态迁移 | 待实现 → 部分已有 → 指定场景已验收；未完成项不得靠已有 PASS 自动勾选 |

本文件维护顺序和完成范围；具体数字、工具版本与当前通过产物保留在验收记录。源码审阅不是形式证明，Difftest PASS 也不自动说明完整微架构或安全行为正确。

## 8. 规范与项目参考

- [RISC-V RV32I 2.1](https://docs.riscv.org/reference/isa/v20260120/unpriv/rv32.html)：目标对齐、访存对齐与保留编码边界。
- [RISC-V Machine-Level ISA 1.13](https://docs.riscv.org/reference/isa/v20260120/priv/machine.html)：cause、mepc、mtval、状态栈和异常优先级。
- [CSR 规则](https://docs.riscv.org/reference/isa/priv/priv-csrs.html)：权限、只读与保留访问行为。
- [总体 X 设计与验证矩阵](cpu-design-roadmap-e-to-linux-dualissue.md#5-阶段-x精确异常irq-与不可撤销副作用)：本文件的上层设计依据。
- [当前非法指令闭环证据](../cpu-fuzzing/nemu-illegal-difftest-acceptance.md)：已有结果的主要入口。
