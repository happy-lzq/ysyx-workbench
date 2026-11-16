# D5 固定时延专项闭环验收

日期：2026-10-11。运行标识：`d5-closure-20261011-4p5ktu4p`。容器：`ysyx-npc`；Verilator 5.048，编译并行度 2。未在宿主机编译，未修改 nanhu 环境。

## 结论与修改边界

**D5 在当前固定时延共享端口合同及下列指定场景内完成验收闭环，可进入 E。** 15 个定向场景均通过生产 Difftest，共核对 61 条预期 WB 提交。未发现需要修改 RTL 的失败。

本次没有修改或新增 `npc/vsrc`、`npc/csrc`、`npc/include`、`npc/tests` 中的源码；运行前后 111 个文件的哈希与文件集合一致。只在独立 `npc/build/d5-closure-20261011-4p5ktu4p/` 内构建测试驱动、脚本和产物，并同步笔记。核心周期行为、流水线时延和状态机没有修改。

该结论不代表全部 RTL 正确，不包括 E 的 ready/valid、跨周期等待、在途响应或 AXI/cache，也不外推为所有设备的访问宽度和副作用均正确。

## 后端与独立检查

测试链接工作区原有 `main.cpp`、`cycle_event.cpp`、`memory.cpp`、`mmio.cpp`、`keyboard.cpp`、`interrupt.cpp` 与 `dut.cpp`。编译仅将生产 `main` 重命名为 `npc_production_main`，测试入口仍调用生产 `single_cycle()`；未复制它的执行实现。

设备初始化采用生产 `init_map/init_i8042`，使用真实键盘队列、读回调 peek 和接受回调 dequeue；额外注册一个计数型 MMIO 设备检查 Store 次数与最终读值。不是 RAM-only DPI stub，也没有初始化全部 VGA/audio 设备。通过生产 `send_key()` 注入 F1～F8 的确定性按下/释放序列；预期键码及顺序由测试输入规定。

- 组合 peek、MEM 接受事件与 WB 提交分开检查；accepted Load 的 PC/数据按动态顺序跟踪到 WB，检查目标 GPR，包含 `lw x0` 消费但不改 GPR 的场景。
- 独立比较预期 WB PC 序列及原始指令、MMIO Store 后端回调计数、键盘剩余队头和读取值；同时使用现有 REF 比较架构状态。
- 链接期 wrapper 只观察生产 DPI 读写与 `mmio_read_accept` 调用。正常验收不改变返回值、调用次数或设备行为。`read_accept_calls` 表示接受事件分发次数；计数设备没有 dequeue 回调，因此其 Load 的一次分发不能称为一次键盘消费。CSV 中 `CONSUME` 也表示该分发调用；真实键盘出队另由剩余队头和已知输入序列独立检查。
- IRQ 使用确定性 `alarm_fired` 注入，经生产 `interrupt_check()` 设置 DUT/REF pending，随后按实际 `irq_take` 注入 REF。没有启用宿主周期闹钟。为了在上升沿前采集原生信号，驱动先在低电平计算一次中断并求值；生产 `single_cycle()` 内仍按原流程判断及执行。
- 初始 GPR/CSR 是明确的测试夹具，保存于各场景 `*-initial.txt`，并在初始同步给 REF；不是完整启动程序。RAM 输入保存为各场景 `.bin`。

## 场景与实际命中

所有场景都有最多 120 周期的完成检查和外层 30 秒超时；实际各运行 10～17 周期。

| 场景 | 预期 WB 数 | 观察结果与必需覆盖条件 |
| --- | ---: | --- |
| `reads` | 6 | 连续 5 次键盘 Load，包括 rd=x0；20 次 RTL peek、32 次初始重复 peek、5 次实际消费及 5 次 Load 提交；3 次相邻 MEM 授权，5 次 IF/MEM 争用 |
| `stores` | 5 | 连续 3 次 MMIO SW、3 次独立写回调；随后 Load 读到最终值；2 次相邻 MEM 授权，4 次争用 |
| `load_stall` | 4 | 2 次键盘消费；Load-use stall 与较老 MEM 访问实际同拍 1 次，旧 IF/ID valid、PC、inst 保留；转发计算结果正确 |
| `csr_stall` | 4 | CSR stall 2 拍，其中 1 拍与 MEM 访问同拍；旧 IF/ID 保留，CSR 更新值正确 |
| `branch_mem` | 4 | EX taken Branch 与较老键盘 MEM 同拍 1 次；较老 Load 接受，年轻错误路径 Load/Store 不产生设备副作用 |
| `jalr_mem` | 4 | EX JALR 与较老键盘 MEM 同拍 1 次；正确目标、link 和较老 Load；错误路径副作用为 0 |
| `jal_mem` | 5 | ID JAL 与较老键盘 MEM 同拍 1 次；正确目标及 link；错误路径副作用为 0 |
| `ecall_load` / `ecall_store` | 各 3 | ECALL 在 WB 与年轻 MEM Load/Store 各实际相撞 1 次；MEM 请求/授权/接受事件均取消，键盘消费及设备写回调为 0 |
| `mret_load` / `mret_store` | 各 3 | MRET 在 WB 与年轻 MEM Load/Store 各实际相撞 1 次；目标 PC 正确，年轻槽 kill，消费/写回调为 0 |
| `irq_load` | 5 | 普通 WB + IRQ 与年轻键盘 MEM 实际相撞；当拍不消费，保存被 kill 的最老 PC；MRET 后重执行一次、消费一次 |
| `irq_store` | 5 | 普通 WB + IRQ 取消年轻 MMIO Store；当拍 0 次写；MRET 后恢复并仅写一次 |
| `irq_mmio_wb` | 4 | MMIO WB + IRQ + 年轻键盘 MEM 实际相撞；先同步老 Load 的 WB 结果，再进入中断；年轻 Load 不提前消费，返回后仅执行一次 |
| `irq_empty` | 3 | WB 无提交时接受 IRQ；保存当前恢复 PC，进入 handler，MRET 后正常执行 |

正常请求争用时检查授权互斥及 MEM 优先；无授权、无 redirect 时 PC 保持；ID 已消费但本拍无新指令时 IF/ID 置空。redirect 本拍的 `if_req=0` 是预期行为，没有将不可能的 `redirect && if_req` 设为覆盖目标。

WB system/IRQ kill 场景在取消请求前检查年轻 EX/MEM 槽确实含有效 Load/Store，避免“本来没有请求”造成假通过。此处 WB system 指 ECALL/MRET；完整异常与其他不可撤销副作用仍按后续路线处理。

## 波形、事件与验收器资格

- 使用未修改的原生 RTL 信号 VCD，对照 C++ 周期快照和既有 DPI 事件；15 个场景共 181 周期、5068 个信号字段比较一致。
- 快照采样在上升沿前、clk=0；DPI 事件在紧随的上升沿产生，按同一 cycle 和 PC/inst 绑定。离线检查 WB、MEM 接受、Store 写入与消费分发记录对应到原生信号，无遗漏或额外事件。
- VCD 中没有变化的时间点可能省略；解释器保持上一状态直到下次变化，不能要求每个采样时刻都有显式时间标记。
- 关闭 wrapper 观察计数后重跑同样 15 个场景，原生完整 VCD 与周期快照均逐字节一致。关闭的是观察，不是设备执行、事件缓存或 Difftest。
- 故障反例只在测试驱动内模拟：peek 时错误 dequeue、重复 Store、时钟不推进，三者均退出码 1，被验收器拒绝。离线重复一条消费记录也被拒绝；生产源码未改动。

这属于本次观察工具在上述配置/字段/场景下的资格证据。原生信号与同源观察一致不是独立正确性证明；预期键码、提交路径、访问计数以及负例提供了另外的语义检查。

## 复现与产物

运行目录：`npc/build/d5-closure-20261011-4p5ktu4p/`。保存驱动、构建/运行/负例/资格脚本、输入镜像、初始状态、REF 动态库、配置、Verilator 模型、各场景日志/CSV/VCD，以及 `production-sources.sha256`、`artifacts.sha256`、`final-check.json`。

容器内命令：

```bash
cd /workspace/ysyx-workbench/npc
bash build/d5-closure-20261011-4p5ktu4p/build.sh
bash build/d5-closure-20261011-4p5ktu4p/run.sh
bash build/d5-closure-20261011-4p5ktu4p/negative.sh
python3 build/d5-closure-20261011-4p5ktu4p/qualify.py
bash build/d5-closure-20261011-4p5ktu4p/twins.sh
sha256sum -c build/d5-closure-20261011-4p5ktu4p/production-sources.sha256
```

最终上述各入口退出码为 0；负例子进程的预期失败由脚本检查。输入是确定性的，没有随机 seed。为保留本次证据，复现时先复制驱动/脚本到新的运行目录再构建；不要覆盖原目录的日志。`make clean` 会删除 build 产物，清理前需保留该验收目录。

初次测试驱动缺少 `init_map()` 的本地声明导致编译失败，已只在驱动中补齐；首次离线解析错误地要求 VCD 显式包含每个不变采样时刻，已修正状态保持解释。二者都是验收工具问题，不能写成 RTL bug。最终结论只引用上述最终正例、负例与波形比对运行。

未重跑 CoreMark、RT-Thread、完整 cpu-tests 或 MicroBench；本次没有改变生产逻辑，因此采用上述定向 Difftest、设备计数和波形检查作为 D5 剩余项的验收。
