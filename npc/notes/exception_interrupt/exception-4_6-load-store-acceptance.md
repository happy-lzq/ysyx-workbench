# MEM 精确异常定向验收：cause=4/5/6/7

日期：2026-11-14。运行标识：`mem-exception-20261114-b_8rny16`。

> 后续更新（2026-11-16）：原 REF 未对齐缺口已修复，指定完整异常序列 Difftest 与 CPU35 已通过，见 [REF 修复与 ISA 对照](nemu-mem-exception-fix-acceptance.md)。下文保留 11 月 14 日的失败证据与当时结论。

**结论：当前 DUT 的本地对齐/范围错误、指定设备后端错误及 WB trap 路径通过本次定向验收；完整异常 Difftest 尚未闭环。** 当前 REF 继续执行未对齐的 Load/Store，没有采用本项目选择的 RAM 对齐异常策略。该差异已由独立 REF 单步和完整 Difftest 的首次失配共同确认。本次没有修改生产 RTL、生产 C++、NEMU 源码或默认 REF 文件。

## 1. 源码范围与本次实际改动

本次新增独立测试程序、C++ 验收驱动、构建/检查脚本和本记录，并同步进展笔记。代码与证据位于：

- [运行目录](../../build/mem-exception-20261114-b_8rny16/README.txt)。
- [输入与期望](../../build/mem-exception-20261114-b_8rny16/mem_cases.json)、[自检汇编](../../build/mem-exception-20261114-b_8rny16/mem_cases.S)。
- [验收驱动](../../build/mem-exception-20261114-b_8rny16/audit_driver.cpp)、[构建结果](../../build/mem-exception-20261114-b_8rny16/build-results.json)。
- [生产源码完整性](../../build/mem-exception-20261114-b_8rny16/source-integrity.json)：58 个已绑定文件前后 SHA-256 相同，包含 RTL/C++/头文件、NPC 配置、相关 REF 源码和默认 REF 文件。完整列表见同目录 `source-before.json`、`source-after.json`。

验收未改变流水级延迟、组合路径、状态机、复位和 ready/valid 逻辑。独立 Verilator 构建启用 `--trace --public-flat-rw`，以读取原生信号；驱动通过包含生产 `main.cpp` 复用实际 `single_cycle()`，没有重新实现 MMIO 接受或 Difftest 时序。`--wrap=dpi_mem_access` 调用真实后端后记录地址、类型、宽度、返回码和数据，不替换设备行为。

生产源码事实：

1. [mem_stage.v](../../src/vsrc/src/mem_stage.v:45)：依 LSU 类型确定 1/2/4 字节，33 位计算末字节，区分 RAM 对齐、Device 本地检查及完整区间检查。
2. [core_top.v](../../src/vsrc/src/core_top.v:400)：保留已有异常优先级，将本地错误和匹配 MEM owner 的错误响应合并为 `mem_exc_valid/cause/tval`。
3. [请求许可](../../src/vsrc/src/core_top.v:254)：`!mem_exc_valid` 阻止本地故障请求；[完成条件](../../src/vsrc/src/core_top.v:152)允许故障槽或错误响应结束等待。
4. 合并后的载荷进入 MEM/WB，`wb_trap_fire` 抑制普通 GPR/CSR 写，保存现场并 redirect；MEM 错误不再走 `faulted` 停机。IF 活路径错误仍走旧停机路径。

## 2. 本次测试采用的项目策略

以下是当前实现与本次夹具的合同，不是所有 RISC-V 执行环境必须采用的同一未对齐策略：

| 条件 | 期望处理 | 原始 D 请求次数 |
| --- | --- | --- |
| RAM 完整范围内，字节访问/自然对齐半字或字 | 正常访问 | 1 |
| RAM 完整范围内，半字或字未对齐 | Load=4，Store=6 | 0 |
| 访问跨 RAM/Device 结束边界、落在两窗口之外、末字节发生 32 位溢出 | Load=5，Store=7 | 0 |
| Device 地址未对齐 | Load=5，Store=7 | 0 |
| Device 窗口内未注册地址，或当前回调设备不支持的窄读 | 后端返回错误，Load=5，Store=7 | 1 |

窗口采用结束地址不包含的区间：RAM `[0x80000000,0x88000000)`，Device `[0xa0000000,0xa2000000)`。Device 大窗口不表示每个地址都已注册；真实设备映射与回调限制仍由 C++ 后端检查。

`ram_fault` 包含 RAM 跨界候选，而跨界同时触发 `local_range_fault`。当前顶层优先选择访问错误，因此跨界报告 5/7；不能只看到 `ram_fault` 就断言报告 4/6。本次包含该重叠条件的实际仿真。

## 3. 输入与独立期望

73 个场景包括：

- LB/LBU/LH/LHU/LW、SB/SH/SW 在 RAM 四个字节偏移处的正常与异常访问；符号扩展、零扩展、Store mask 和相邻字保持。
- RAM/Device 边界、窗口外地址、`0xffffffff` 末字节溢出、Device 未对齐及未注册地址。
- 真实键盘和 RTC 回调拒绝窄读；有效键盘读取、Audio buffer 字节/半字/字访问和串口字节写。
- 故障 Load 的 `rd=x0`、负立即数有效地址、异常前已完成结果、紧随故障的年轻 Store。
- 交替设置 MIE/MPIE 初值，检查 trap 后的 mstatus；由自定义 handler 校验 cause/mepc/mtval，修改 mepc 为本例恢复标签后 MRET，最终有界结束。

正常数据、故障原因、有效地址、指令 PC、预期请求次数来自生成器输入和已声明策略，不以 DUT 的异常输出倒推。汇编自检与 C++ 检查共同使用这些输入；二者不是两份独立 ISA 实现，因此还单独保留真实 NEMU 的能力检查。

每条故障指令检查 WB 仅出现一次、不正常退休、所有 GPR 在 trap 边沿不变、普通 CSR 写许可关闭，mepc 为故障指令 PC、mtval 为原有效地址、mcause 符合夹具，redirect 指向 mtvec。观察请求接受、后端握手、真实 DPI 执行、响应接受和设备读消费次数，防止只比较最终寄存器而漏掉重复副作用。

## 4. 当前 DUT 实测结果

| 配置 | 请求等待 | 基础响应等待 | 随机额外等待 / seed | 场景 | 预期异常 | 实际周期 | 结果 |
| --- | ---: | ---: | --- | ---: | ---: | ---: | --- |
| native-default | 0 | 1 | 0 / 1 | 73 | 43 | 20703 | GOOD TRAP，0 检查失败 |
| native-delayed | 3 | 5 | 11 / 17 | 73 | 43 | 85073 | GOOD TRAP，0 检查失败 |
| native-seed1 | 0 | 2 | 23 / 1 | 73 | 43 | 85367 | GOOD TRAP，0 检查失败 |
| native-seed2026 | 5 | 3 | 31 / 2026 | 73 | 43 | 137066 | GOOD TRAP，0 检查失败 |

每组场景构成为正常 30、cause=4 共 9、cause=6 共 5、cause=5 共 17、cause=7 共 12。每组都实际出现 43 次 WB trap；36 个本地故障没有 D 请求，7 个后端故障各有一次 D 请求、一次真实后端执行、一次错误响应、一次 WB trap，无重发。见 [逐例结果](../../build/mem-exception-20261114-b_8rny16/native-default-results.csv)及同目录各配置 `*-summary.json`。

设备消费：先注入 ESCAPE/F1 两个真实键码，错误窄读后，三次有效键盘 LW 依次得到 `0x8001`、`0x8002`、`0`；第二次使用 `rd=x0`，仍消费该键。错误访问没有触发成功 MMIO 读接受事件。该结果限定于本次键盘队列路径，不覆盖所有设备的读写属性。

年轻 Store：自检 canary 保持，故障后的 Store 请求次数为 0；其中 7 个后端错误在 WB trap 时确实观察到较年轻的有效 MEM 槽，且请求许可为 0。36 个本地错误不据此宣称同样的流水级碰撞覆盖。动态身份和周期见 [trap-events.json](../../build/mem-exception-20261114-b_8rny16/trap-events.json)。

## 5. 原生波形与日志资格

本次 `native-default` **实际采集**原生 `native-default.vcd`（约 51 MiB），未给 RTL 添加 probe 或修改被测逻辑。

- 使用生产复位与时钟循环，首个低相位采样时间为 15，此后每周期加 10；下一上升沿后的状态在加 5 处核对。VCD 标示时间单位为 1ps，这里的时间是仿真采样编号，不是频率/性能结论。
- 对齐 20703 行周期记录、5174 次真实 DPI 执行以及对应请求/响应 owner、地址、类型、错误和数据。
- [资格检查](../../build/mem-exception-20261114-b_8rny16/qualification.json)：967853 次字段比较，无不一致；现有 DPI WB valid/PC 与原生提交槽同周期匹配。
- [负对照](../../build/mem-exception-20261114-b_8rny16/finish-checks.json)：仅将一条夹具的预期 cause 从 4 改成 6，检查器在同一故障指令处拒绝；RTL 和实际执行输入保持相同。

原生信号与日志一致只验证本次观测方法的字段、身份和采样合同，不能单独证明 CPU 正确。语义判断另依赖输入夹具、软件自检和明确的正常/故障对照。其余延迟配置复用同一驱动和依赖语义，仅采日志，**不宣称这些运行也各自采集并通过了 VCD**。

## 6. Difftest 结果与最早差异

| 运行 | 结果 |
| --- | --- |
| aligned-difftest | 正常 30 例通过，1701 条 REF 指令，7924 个 DUT 周期 |
| aligned-delayed-difftest | 同一正常输入通过，1701 条 REF 指令，32526 个 DUT 周期 |
| faults-difftest | 第 10 例、首个未对齐 LH 发生 GPR 失配；未通过 |
| cause0-regression | 复用既有自检镜像，48 场景/19 trap，当前 RTL + 当前 REF 通过，8874 周期 |
| cause0-delayed-regression | 同一输入在延迟配置下 Difftest 通过，36414 周期 |

本次 REF 从当前 NEMU 源码重新构建到独立 `ref-build/`，没有覆盖默认 REF。

首次可观测 Difftest 差异：

```text
目标：LH t2,0(t0)，有效地址 0x80010001
故障指令 PC：0x800009c8
WB trap 上升沿：cycle=2775；生产循环结束后的计数为 2776
DUT x7：0x5a123456（保留故障前值）
REF x7：0xffffff7f（实际执行了未对齐读）
DUT mcause=4，mepc=0x800009c8，mtval=0x80010001
```

[完整失配日志](../../build/mem-exception-20261114-b_8rny16/faults-difftest.log)。末尾汇总中的其他未完成场景计数是首个失配终止后的连带缺项，不是额外发现了几十个 RTL bug。

独立 [REF 单步结果](../../build/mem-exception-20261114-b_8rny16/ref-probe.json)：未对齐 LH/LHU/LW/SH/SW 五例均未采用预期异常策略；SH/SW 还实际修改了未对齐目标数据。三个正常访问对照通过。对应 [inst.c](../../../nemu/src/isa/riscv32/inst.c:125) 仍直接使用 `Mr/Mw`，没有按本项目策略检测这类故障。

因此本轮最早差异指向 DUT/REF 的执行环境策略不一致。不能将其直接判为硬件核错误，也不能通过将 DUT 的 cause/CSR 复制到 REF 来宣称异常 Difftest 通过。

## 7. 环境与复现

仅在 `ysyx-npc` 容器构建/仿真，用户 `1006:1006`，工作区 `/workspace/ysyx-workbench`；Verilator 5.048、RISC-V bare-metal GCC 13.2.0、Clang 19.1.1、Python 3.12.3。构建并行度为 4。

采用当前 NPC 配置，关闭大容量 Difftest/ITRACE 文件日志。SDL 使用 dummy video/audio，以初始化真实设备回调；本次没有显示图像。**不启动宿主 timer alarm**，mie=0，隔离同步异常；生产 `interrupt_check()` 仍每拍执行，因此本次不包含 IRQ 碰撞验收。

在容器的工作区根目录执行，`RUN` 为本次证据目录：

```bash
RUN=npc/build/mem-exception-20261114-b_8rny16
python3 "$RUN/launch_build.py"
python3 "$RUN/run.py" native-default mem_cases -
python3 "$RUN/qualify.py"
python3 "$RUN/run.py" native-delayed mem_cases - \
  +mem_req_delay=3 +mem_resp_delay=5 +mem_random=11 +mem_seed=17
python3 "$RUN/run.py" aligned-difftest aligned_cases \
  "$PWD/$RUN/ref-build/riscv32-nemu-interpreter-so"
python3 "$RUN/finish_checks.py"
```

确认 REF 缺口的两条命令在当前版本下**预期返回非零**，产物中记录实际差异：

```bash
python3 "$RUN/ref_probe.py"
python3 "$RUN/run.py" faults-difftest mem_cases \
  "$PWD/$RUN/ref-build/riscv32-nemu-interpreter-so"
```

每次仿真最多 300000 周期/40 秒。原复现脚本会写相同名字的结果；若要保留本轮证据，应先复制整个运行目录并使用新的目录名，再执行。不要覆盖既有证据或默认生产仿真器。

## 8. 未完成项与下一步

1. REF 按相同地址图、未对齐策略和错误条件独立产生 4/5/6/7，禁止故障指令 GPR/Store 副作用，再重跑完整异常 Difftest。本次没有修改 REF。
2. IF 活路径错误仍是 `faulted` 停机，cause=1 精确 WB trap 尚未接入。
3. 本次没有 IRQ、较老 WB trap、复位中断事务等完整碰撞专项，没有跑 CPU35、CoreMark 或 RT-Thread；旧通过结果不当作当前版本实测。
4. 本次未覆盖所有设备的读写权限/宽度、后端可能产生部分外部效果后再报错的硬件情形、AXI/SoC 的错误源，以及独立 WB 反压。现有后端拒绝发生在设备动作之前，不能外推到任意外部设备。
5. 当前检查器绑定这版单笔在途/生产 DPI 事件时序。修改 owner、响应时序、WB 事件合同或相关实现后，应重做受影响的观测资格验证。

M3/M5 可标记“指定 DUT 场景已验收，REF 未闭环”，不能标记整个精确异常阶段完成。
