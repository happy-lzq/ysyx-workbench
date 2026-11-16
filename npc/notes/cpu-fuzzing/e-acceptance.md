# E 首版验收：单笔同步访存与五级反压

日期：2026-10-18。范围：当前纯 M、单笔全局在途、WB 始终可接收实现。此记录保存本次实验结果，不把历史 D3/D4/D5 PASS 当作新 RTL 的证据。

## 1. 问题、实现与边界

改动前快照：`npc/build/e-20261018-baseline/`。最小程序实际出现首条提交 `PC=0x80000004, inst=0x00000097`，预期 AUIPC 的 PC 是 `0x80000000`。请求沿推进 PC、返回指令与请求地址未共同绑定，以及 MEM 无完成等待/级间无完整 hold，是此次需要替换的时序假设。

本次实现：

- `mem_port` 锁定请求 owner 和载荷，独立 `dpi_mem` 执行可延迟请求/响应。IF/MEM 前端选择事件与后端握手分开。
- 四组寄存器增加 enable/flush；WB→MEM→EX→ID 反推接收容量。MEM 等结果、WB 可先排空。
- EX 首次被阻塞保存已解析计算值，最近 Load 未就绪时禁止取更老 WB 值。
- IF 预留 IF/ID 槽，busy 防重发，live 处理作废响应；PC 与返回地址正确绑定。
- D 在最老时许可，保护到退休，IRQ 延后；MMIO 读取消组合消费，保持一次性事件。
- 有效访存错误只 fail-stop，作废 IF 错误排空；完整异常留 X。

未改用户已有 `if_stage.v`、`lsu.v`、`csr.v`、`dut.cpp` 等实现；工作区相对 Git 的其他差异不自动属于本任务。相对开始快照的实际文件清单和补丁存于 `build/e-20261018-closure/changed-vs-start.json`、`changes-vs-start.diff`。

## 2. 环境和复现

所有构建/仿真在既有 **ysyx-npc** 容器，未修改宿主机或 nanhu 工具环境。Verilator 5.048，定向构建 `-j 2`。当前 DIFFTEST、TIMER 开启，逐条 DIFF_LOG、ITRACE、全程 WAVE 关闭；专项驱动独立采集有界 VCD。

从容器内运行：

```bash
source /workspace/ysyx-workbench/npc/container/env.sh
cd "$NPC_HOME"
bash tests/run_e_protocol.sh
bash tests/run_e_pipeline.sh
bash tests/run_e_device.sh
bash tests/run_e_regression.sh
bash tests/run_rtthread_delay.sh
```

CoreMark 可使用本次保留的最终生产可执行文件复现：

```bash
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
  timeout 1200s build/e-current/coremark-final/sim \
  -i ../am-kernels/benchmarks/coremark/build/coremark-riscv32I-npc.bin \
  -d ../nemu/build/riscv32-nemu-interpreter-so --log-limit 1000
```

延迟 plusargs：`+mem_req_delay=N`、`+mem_resp_delay=N`、`+mem_random=N`、`+mem_seed=N`。随机量添加在响应等待，不是随机请求 ready。默认 req=0、resp=1、random=0；延迟由仿真时钟推进，无宿主 sleep。

源码、配置、工具摘要：`build/e-20261018-closure/`。cpu-tests 输入与 REF/模型摘要另存 `build/e-regression-oV1SQF/`，CoreMark 摘要存 `build/e-current/coremark-final/inputs.sha256`。现有 guest bin 没有被回归脚本隐式重编；RT 使用独立 guest 目录重编，不替换用户 BSP 镜像。

最终生产 sim SHA256：`1fccc0107ba06c4cc45324bc6357a4550687b0def65b0f187c54291371d1efa5`。
REF SHA256：`63845c0b4a68d7475fbf8eb29e6297874bf81c117b41da9a1139e7fa99e9e116`。

## 3. 协议、流水线和控制验收

| 测试 | 实际观察 | 证据 |
| --- | --- | --- |
| e_protocol | req 等待 1/3/17、resp 延迟 1/5/31；每次 accepted=completed=3，Store=1，响应保持14拍；请求保持3/9/51拍 | `build/e-protocol-TmuEGl/` |
| e_protocol 随机 | seed=73、响应附加随机0..31；账本3/3，owner 不串，响应反压保持 | 同上 |
| e_pipeline | 固定等待下15提交/4 Store，PC+inst 序列、byte/half mask、Load/CSR结果、错路径 Store | `build/e-current/pipeline-closure.log` 首行指向实际目录 |
| e_pipeline 随机 | req=17，resp=5，附加0..31，seed=1/73/20261018；各15提交/4 Store | 同上 |
| e_stages | 7个密集槽位场景，hold=137、保存结果=137、Load等待=120、分支1、提交27 | 同上 |
| e_redirect | 作废IF响应5、IRQ6、延后IRQ23拍、Store1；mepc=0x80000024，真实MRET恢复到24且不重写Store | 同上 |
| e_fault | 活IF/D错误只报告一次，错误指令不退休；较老提交保留 | 同上 |
| e_control | CSR→ECALL/MRET，JAL/JALR link；System2、EXredirect1、IDredirect1、错路径Store0 | 同上 |
| e_dependencies | Load→地址、Load→Store数据、Load→CSR源的连续真实程序；8提交、等待69拍、Store1，结果及顺序正确 | 同上 |

`e_stages` 明确采用测试台注入合法密集流水槽并阻止额外 IF 请求，专门制造旧 WB 离开、EX 仍被老 MEM 阻塞的窗口。它验证该状态下控制/数据保持，不宣称这些槽位组合均由当前低吞吐前端自然形成。自然指令流另由 pipeline/dependencies/control、Difftest 和 RT 验证。

### E-01～E-15 对照

| ID | 本版覆盖 / 裁定 |
| --- | --- |
| 01/02/03 | 请求稳定、延迟响应、协议端响应反压；WB 背压不适用 |
| 04 | Load→ALU（pipeline/stages）及地址、Store、CSR（dependencies） |
| 05/06 | 最近 Load 遮挡更老 WB；ALU/CSR/Store/branch 结果跨 hold 保存（stages） |
| 07 | 同时需求 MEM 优先，已锁 IF 不被抢占（protocol） |
| 08/09/10 | OFFER 等ready时redirect、WAIT连续redirect、响应同拍redirect/旧错误（redirect） |
| 11 | 真键盘含rd=x0、重复eval、窄设备访问、次数与最终状态（device） |
| 12 | 作废IF错误不生效；有效IF/D错误只fail-stop（redirect/fault/regression） |
| 13 | WB system取消未许可年轻Store；D许可后IRQ等待，恢复PC/MRET与副作用一次（redirect/control/RT） |
| 14 | 固定seed随机响应等待，协议账本、生产Difftest与真实设备独立计数 |
| 15 | 本版WB不hold，原“WB被阻塞的CSR写”窗口不适用；同址CSR等待真实提交及新旧GPR转发另验 |

所有 D 请求都采用最老许可，不存在合法“已发年轻 D 被更老 WB 取消”的窗口；不把 IF 排空验收扩大为投机数据访问支持。

### 负例

已按预期检查器拒绝：请求载荷变化、响应数据变化、丢响应（测试台内部 watchdog）、重复 Store、重复 WB、取消 EX 快照、忽略 IF live、篡改 DPI 的 WB PC。脚本拒绝 timeout=124 充当正常负例通过；具体诊断保留在各运行目录。

## 4. 真实设备与波形资格

`build/e-device-XSBYK3/` 使用生产 `single_cycle()`、生产 MMIO/键盘队列和 REF：

- 15 条提交，键盘消费3次（包含 rd=x0），设备写1次。
- 结束后排空：请求24、响应24；不能把响应数直接等同于退休数。
- VCD / 边沿事件CSV检查461行，WB15、MMIO接受4；篡改回调 PC 后校验拒绝。
- 34位高地址、设备取指、未对齐、越界、错误写mask及键盘窄读均拒绝。

本次发现并修正两项设备适配问题：窄写 callback 未截断高字节；现有 read callback 不携带宽度，窄读会变成整字设备读，键盘偏移读还会触发宿主断言。先复现失败（`e-device-m60u3T`、`e-device-oEl0Rz`），再对窄写截断，对不支持的 callback 窄读返回错误。后者不是宣称所有外设支持任意读宽度。

资格检查确认观测信号与采样沿/动态身份对应；独立预期值、计数和反例提供语义约束。同源波形一致本身不是独立架构正确性证明。

## 5. 程序与 RT 回归

### cpu-tests / CoreMark

- `build/e-regression-oV1SQF/`：35/35 cpu-tests，生产Difftest，req=3、resp=5、随机响应0..7、seed=73；有效错误程序明确退出1并报告PC/地址/kind。
- `build/e-current/coremark-final/`：最终生产版本1000迭代，CRC `e714/1fd7/8e3a/d340`，CoreMark PASS；REF与DUT均在 `0x80002058` GOOD TRAP，313360041条guest指令。总任务退出0保存在 `build/e-current/final-suite-exit.txt`。
- 本次CoreMark显示359675ms，8 Marks；这是当前仿真和计时环境结果，不是RTL综合频率或CoreMark/MHz。随机延迟覆盖以专门测试配置为证据。

### RT-Thread

`build/rtthread-delay-owSdfs/acceptance.txt` 及 `build/e-current/rtthread-final.log`，完整脚本退出0：

| 模式 | 周期 / Difftest事件 | 结果 |
| --- | --- | --- |
| 无timer负对照 | 6000000 / 985033 | IRQ=0、tick=0，预期未完成，返回5，不能GOOD TRAP |
| controlled tick | 878089 / 147666 | IRQ49、tick49、ISR切换19、MRET74；A唤醒8/B唤醒4，忙循环抢占2 |
| host timer | 2056963 / 365678 | IRQ42、tick42、ISR切换25、MRET79；忙循环抢占5 |

诊断、tick、host 有界原生VCD资格分别30002/20804/9912行；另检查保存上下文与MRET恢复。普通BSP启动的off/restore/host各200万拍检查main/shell、help/ps和IRQ（0/1/25），属于有界OS运行，不要求OS自然退出。篡改WB身份、pre_a0、上下文目标及结束关系的反例均拒绝。

## 6. 结论与后续

E 首版在上述范围形成实现—定向/负例—程序回归—波形资格闭环。旧 D3/D4/D5 固定时延脚本没有被直接当作新版本通过证据；本次用 E 用例保留关键语义检查。

仍未完成：精确非法/访存错误异常、未对齐策略更新、通用可阻塞WB、投机D访问、多在途、独立复位隔离、AXI4、可综合SoC、Cache/MMU/Linux。这些不由本次Difftest PASS自动得到保证。

下一步先按 [逐拍导读](../design-process/e-pipeline-walkthrough.md) 理解当前实现，再进行 X 的异常合同和 F 的bridge设计；不要直接将 DPI 替换成 AXI 而略过错误与复位边界。
