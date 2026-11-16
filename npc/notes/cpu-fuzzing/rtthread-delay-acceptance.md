# RT-Thread-AM：延时唤醒与 timer 抢占验收

## 目的与验收合同

本测试在当前 RV32I NPC 五级流水线上运行真实 RT-Thread 调度器，复用生产 `single_cycle()`、DPI 事件缓存、MMIO 接受与提交级 Difftest。测试应用单独链接到证据目录的 BSP 镜像；不替换正常 RT 镜像或 `npc/build/sim`。

功能要求：

- A：优先级 8，每轮睡眠 3 tick，共 8 轮。
- B：优先级 9，每轮睡眠 5 tick，共 4 轮；唤醒后忙循环 4 tick，期间不调用 yield、delay 或 schedule。
- A 必须在 B 忙循环期间由 timer 到期唤醒并抢占 B，不能把 A 主动睡眠后返回 B 当成 B 被抢占。
- 每次阻塞返回校验 s0～s11 的不同线程/轮次模式、16 个栈内 canary、延时下界及返回 SP；两个线程都完成后才发出 PASS 并 `halt(0)`。
- 所有有效 WB/接受 IRQ 事件执行生产 Difftest；普通指令保持逐条架构比较。
- 没有 timer 的负对照应停留在睡眠状态，不能唤醒或进入 GOOD TRAP。

## 本轮定位到的 BSP 软件错误

原 `rt_hw_context_switch_interrupt(from,to,...)` 每次都覆盖 `rt_save_ptr` 和已取出的 `rt_next_ctx`。一次 timer ISR 可以使多个线程到期，每次 `_thread_timeout()` 都可能调用 `rt_schedule()`；调度器会立即更新 `rt_current_thread`，实际 CPU 仍在原线程的 IRQ handler 中。

因此，ISR 内 `shell → B → A` 是两个调度决定，B 尚未真正执行。handler 的参数 `c` 属于 shell，只能保存到首次 from 的 Context 槽；不能保存到第二次 from 的 B 槽。

### 修正前证据

目录：`npc/build/rtthread-delay-k7vzBU`。首次受控运行达到 4000 万拍仍未完成：A 唤醒 8 次，B 没有 WAKE；RT 报告 B 栈溢出，Difftest 未出现失配。两台机器执行同一个错误的 BSP，所以 ISA 状态一致不足以判断调度功能通过。

后续有界诊断运行在 `diagnostic.*`：826274 拍、74 次 IRQ、仍未完成。原生波形与 CSV 核对 26341 个周期，覆盖 6 次 MRET、3 次 IRQ；`diagnostic.stores.csv` 从原生 MEM 写信号提取，而非根据最终 shell 输出推测。

| 周期（测试循环零起点） | 实际观察 |
| --- | --- |
| 144700 | timer IRQ 被接受，原线程为 shell，保存 Context 应为 0x82036d58 |
| 145511 | 第一次 ISR 切换请求：shell → B，from 槽 0x82035e74 |
| 145512 | MEM 写 `rt_save_ptr=0x82035e74` |
| 146075 | 第二次请求：B → A；B 仅为中间调度目标 |
| 146076 | MEM 写把 `rt_save_ptr` 改为 B 的槽 0x8002a908；这是本记录最早可观测的保存来源合同违例 |
| 146677 | handler 把 shell Context 0x82036d58 写到 B 的槽 |
| 149800～149836 | A 再睡眠后切到 B，却用 shell 的 Context 恢复并 MRET 到 shell PC |

离线来源检查拒绝该记录：`diagnostic-contract.log` 报告 `ISR original save destination changed`。历史目录中 `model/Vcore_top` 曾为诊断重新链接，最初 `inputs.sha256` 对该模型的摘要属于首次运行；诊断使用原镜像/REF和保留的 guest 源码，不能把模型摘要混作同一次运行。最终交付目录的摘要另行完整核对。

### 修正逻辑

生产软件修改仅在 `am-kernels/rt-thread-am/bsp/abstract-machine/src/context.c`：

1. 增加 `rt_next_ptr` 保存最终目标线程的 Context 槽地址。
2. ISR 第一次请求设置 `rt_save_ptr`；后续请求保留首次来源，只更新 `rt_next_ptr`。
3. timer handler 先执行 `*rt_save_ptr=c`，再从最终目标槽读取 Context，清空 ISR 请求状态并返回。

先保存再取目标，源码上也允许最终调度目标回到原线程时读取刚保存的 Context；这类回到原线程的特定序列本轮没有另作独立定向验收。普通 YIELD 路径仍使用 `rt_next_ctx`。

生产 RTL、DPI 接口、trap.S 和流水级数未修改。软件增加了请求选择和槽读取指令，因此 handler 执行周期会变化；硬件 ready/valid、reset、kill 和流水级延迟没有变更。此前修正的初始 Context `mstatus=0x1880` 继续生效。

## 动态闭环与采样边界

```text
timer pending
  → irq_take 被接受
  → __am_asm_trap 保存原线程 Context
  → EVENT_IRQ_TIMER
  → rt_tick_increase / rt_timer_check
  → _thread_timeout(A) / rt_schedule
  → 首次 from + 最终 to
  → 保存原 Context、返回目标 Context
  → mv sp,a0 / CSR 与 GPR 恢复 / MRET
  → A 从 rt_thread_delay 返回并校验寄存器、栈及 tick
```

这条链由实际动态事件关联，不把源码中的函数排列当成运行时顺序。`*.chains.csv` 保存每个完整 timer IRQ 的 tick、timeout、切换请求、handoff 和 MRET；`*.acceptance.json` 保存 A 在 B 忙循环内被抢占唤醒的具体轮次与周期。

- marker 是函数首条普通 NOP，只有其有效 WB 提交才记事件；a0～a3、sp 从该提交之前的架构 GPR 读取，不使用 IF 取指身份。
- START 发布 Guest 编译得到的 TCB 大小与 sp 字段偏移；ISR 的 from/to 必须等于对应线程 TCB+偏移。捕获窗口的 events 与已经 qualified 的 native CSV 按 cycle、WB PC、参数、SP 关联，避免两份日志的字段各自正确却未绑定动态身份。
- 每拍核对原生 pre-edge WB 与 DPI 缓存的 valid/PC/inst。
- `current` 是模拟周期之后的 RAM 快照，不是新的 commit 字段。特别是普通切换入口，`rt_current_thread` 已为目标；判断主动让出的线程使用提交的 from 参数。
- Context 槽 RAM 读用于辅助关联。原生 VCD 核对 WB、参数 GPR/SP、CSR/redirect、MRET、IRQ 保存 PC 和四组年轻槽 valid；捕获窗口内另检查 MEM 写的首次来源与 Context 保存地址。
- `target_context` 是 ISR 请求入口之后的目标槽 RAM 快照；每次 handoff 校验最终目标 Context。如果最终 to 等于首次 from，期望刚保存的原 Context。原生窗口额外核对 `a1 → rt_next_ptr 写入 → 保存原 Context → 最终槽 MEM load → handoff.a0`，再关联 MRET 的 PC/SP。
- MEM store 的物理写发生在 WB 前，本记录明确标为 MEM 周期。接受 IRQ 时 MEM 被 kill，离线提取不把请求更新前的快照误认作实际执行的 store。

源码审阅和同源字段一致不是形式证明；波形资格限定于实际捕获窗口，其他周期通过有效提交日志和 Difftest 检查。

## 2026-10-04 最终验收记录

统一入口退出码 **0**；目录：`npc/build/rtthread-delay-f59PA0`，最终标志为 `acceptance.txt`。工具为 Verilator 5.048、bare-metal GCC 13.2.0；ARCH=riscv32I-npc、构建并行度 2，DIFFTEST/HAS_TIMER 开启，DIFF_LOG_ENABLE/ITRACE/WAVE 关闭。测试驱动另采集有界 VCD，没有开启全程日志或全程波形。

无随机 seed。diagnostic/tick 从 START 后每 10000 模拟拍设置生产 `alarm_fired`，再沿现有 pending→IRQ 流程执行。host 保留生产 ITIMER_VIRTUAL 约 60 Hz timer，依赖宿主用户态 CPU 时间，因此周期数和 IRQ 数不固定。

| 模式 | 模拟拍 | Difftest 事件 idx | IRQ / tick | ISR 切换请求 / MRET | 结果 |
| --- | ---: | ---: | ---: | ---: | --- |
| off | 600000 | 401251 | 0 / 0 | 0 / 6 | 两线程停在首次睡眠；预期未完成，退出 5；负对照检查通过 |
| diagnostic | 457630 | 341798 | 37 / 37 | 25 / 74 | 同一 ISR 多次决定、保存首次来源；A8/B4，GOOD TRAP，退出 0 |
| tick | 457630 | 341798 | 37 / 37 | 25 / 74 | A 在 B 忙循环内被抢占唤醒 2 次；A8/B4，GOOD TRAP，退出 0 |
| host | 970914 | 697530 | 39 / 39 | 24 / 75 | 忙循环内抢占唤醒 5 次；A8/B4，GOOD TRAP，退出 0 |

idx 是 WB/IRQ 的 Difftest 调用事件数，同拍可含两者，不是严格 retired instruction 数。三组正例全程未出现 Difftest mismatch。Guest 寄存器/栈/延时检查全部通过，NPC/NEMU GOOD TRAP PC=0x80019d1c。

实际采集的原生波形资格：

| 窗口 | 核对周期 | MRET / IRQ | Context 来源检查 |
| --- | ---: | ---: | --- |
| diagnostic | 26344 | 6 / 3 | 3 个完整切换 IRQ；其中 1 个含多个调度决定 |
| tick | 25344 | 4 / 3 | 1 个完整切换 IRQ，包含 B 忙循环被 A 抢占 |
| host | 5549 | 2 / 1 | 1 个完整切换 IRQ，包含 B 忙循环被 A 抢占 |

改变一条 CSV 的 wb_inst 或 pre_a0 后，波形检查均退出 1；改变 events 的最终目标槽参数、目标 Context 快照或 off 的运行状态也分别被语义检查拒绝，见 `negative-*.log`。正常资格检查退出 0。关闭 timer 的功能负对照也通过，避免把 busy-loop/输出或 REF 状态同步当成完成。

独立审阅发现并补齐上述“最终 to 未完整绑定”和“off 未拒绝提前结束”两个检查合同缺口。目录 `npc/build/rtthread-delay-jKHSL1/review-red-sfjsd8hu` 的新鲜变异副本被旧检查器接受（RED），补齐检查后拒绝（GREEN）；未修改原通过记录。最终入口又重跑全部正负例及启动回归，复审未发现新的实质问题。审阅是源码和产物核验，不能代替实际仿真。

### 一段实际抢占时序

来自 tick 模式的 B 第 0 轮忙循环、A 第 2 轮唤醒（对应 JSON 与 CSV）：

| 周期 | 事件 |
| --- | --- |
| 148672 | B 开始不主动让出的忙循环 |
| 174700 | 接受 timer IRQ，打断 B |
| 174859 | 进入 rt_tick_increase |
| 175038 | A 的 _thread_timeout |
| 175511 | ISR 请求 B → A，from 槽 0x8002a908、to 槽 0x8002a85c |
| 175699 | handoff 选择 A 的 Context 0x80029690 |
| 175735 | MRET 返回 A，sp=0x80029720 |
| 176043 | A 完成返回校验，发出第 2 轮 WAKE |
| 185417 | B 恢复后结束忙循环 |

### 普通启动回归

同一统一入口继续在 `boot-g0XQpu` 构建无测试应用的普通 BSP 镜像，复用隔离的 RTL 模型；三个模式各执行 200 万拍：

- off：IRQ/tick=0/0，main、Hello、shell 运行，退出 0。
- restore：IRQ/tick=1/1，恢复边界 pending 延后接受，main/shell 运行，退出 0。
- host：IRQ/tick=58/58，main/shell 运行，退出 0。

每组额外核对 1976 个原生波形周期；三组没有 Difftest 失配。普通 OS 回归按有界观察完成，不期待 GOOD TRAP；退出码 0 来自审计驱动的里程碑判定，不表示 RT 内核退出。

最终镜像、REF、模型、选定软件输入/测试源码摘要由 `inputs-check.log` 核对；20 个生产 RTL 文件与本轮前摘要一致，见 `rtl-after-check.log`。本次没有安装工具、修改宿主机或 nanhu 环境，没有主动结束用户的仿真进程。

## 复现命令

在 ysyx-npc 容器内：

```bash
source /workspace/ysyx-workbench/npc/container/env.sh
cd "$NPC_HOME"
bash tests/run_rtthread_delay.sh
```

每次创建新的 `npc/build/rtthread-delay-*`，隔离镜像和 Verilator `--Mdir`；保存构建日志、配置、符号/反汇编、事件、波形、资格与结果。包含负对照、三组正例、资格反例和普通启动回归，不覆盖同目录旧证据，不调用自动 git 提交。

必要条件：bare-metal RISC-V 工具链、当前 NEMU REF `.so`、NPC DIFFTEST/HAS_TIMER 开启，三个全程日志/波形配置关闭。各正例上限 4000 万拍、45 秒；无进展 50 万拍提前结束；off 上限 60 万拍。文件上限 128 MiB/单文件，core dump 关闭。新的条件不满足或失配时保留目录并返回失败。

测试相关代码：

- `rtthread_delay_app.c` / `rtthread_delay_probe.S`：Guest 工作负载、marker、寄存器/栈校验。
- `rtthread_delay_test.cpp`：生产仿真复用、有效 WB 身份、受控 timer、诊断窗口与失败前 64 条提交。
- `check_rtthread_delay.py`：动态链关联、来源合同、功能负对照、抢占语义。
- `check_rtthread_context_vcd.py`：原生波形采样资格、MEM 写提取；兼容此前启动验收。
- `run_rtthread_delay.sh` / `run_rtthread_boot_isolated.sh`：统一容器构建、运行、回归入口。

## 适用范围与未完成项

已验收指定配置中的 delay/timer 到期、两个不同优先级线程的抢占、Context 保存恢复、MRET 与生产提交级 Difftest。此次功能错误定位到 BSP 保存来源；未观察到本用例的 RTL 架构失配，不能据此宣称全部 RTL 正确。

仍未验收完整 RT API、信号量/互斥锁、同优先级时间片公平性、真正嵌套中断、完整特权 ISA；设备 MEIP 来源、交互 UART/设备接收中断、RTC BSP 注册尚未接入。本测试没有解决宿主约 60 Hz timer 与 RT_TICK_PER_SECOND=100 的时间尺度差异；不提供核的硬件性能分数。普通通用 AM kcontext 的独立调度路径也不由本 RT 测试覆盖。
