# X 异常载荷承载验收

> 历史记录边界：下文对应只承载异常字段、尚未启用生产 trap 消费的版本。2026-10-26 已有真实非法指令 producer 与 WB trap，当前状态见 [非法指令验收](x-illegal-acceptance.md)。旧 `x_payload_core_tb` 会把所有动态指令强制标为异常并期待它们继续普通执行，不能原样作为当前 trap 版本的通过标准；独立寄存器承载检查仍有参考价值。

## 范围

本记录只验收三字段 `exc_valid`（1 位）、`exc_cause`（32 位）、`exc_tval`（32 位）随流水线槽保存与传递。PC 复用原有各级 PC，没有新增流水级。

- 用户新增的 IF/ID 字段按 reset、flush、enable 顺序保存，hold 时保持；本次保留其实现。
- 顶层原先声明的 `if_id_exc_*_in` 没有驱动，本次明确接零。
- ID/EX、EX/MEM、MEM/WB 增加同样的三字段输入、输出和时序保存，顶层按相邻流水级连接。
- reset 清空三字段；flush 清除槽 valid 和 exc_valid，cause/tval 可保留；enable 捕获；否则保持。
- 生产路径目前没有异常生产者，也没有使用新字段改变普通写回、访存、CSR 或 redirect。IF/D 错误仍按 E 的 fail-stop 路径处理。

本步不是完整的精确异常支持。5.3 的检测、错误响应转换、普通退休与 trap 分离、异常副作用许可、REF 同步异常适配均未完成；非零异常字段尚不能作为生产输入。

## 命令与环境

在现有 `ysyx-npc` 容器内、`/workspace/ysyx-workbench/npc` 下运行：

```bash
bash tests/run_x_payload.sh
bash tests/run_e_pipeline.sh
```

工具：Verilator 5.048；每次编译使用 `-j 2`；单项仿真有 20 秒 timeout。
新承载脚本记录源码 SHA-256 和工具版本。E 定向测试使用请求等待 3、响应延迟 17；随机回归使用请求等待 17、响应延迟 5、随机上限 31，seed 为 1、73、20261018。

## 本次结果

### 新增承载专项

证据目录：`npc/build/x-payload-uAoVBs/`。

- `x_payload_regs_tb`：四个真实寄存器、13 个上升沿，检查 reset 优先级、异常捕获、输入变化时 hold、enable=0 时 flush、flush 优先于捕获、正常槽覆盖异常槽、bubble、满位宽数据和 cause=0 的异常载荷。
- `x_payload_core_tb`：在测试中替换 IF/ID 异常输入，按原取指响应 PC 构造不同 cause/tval 标记；沿真实四级寄存器检查身份对齐。
- 顶层专项实际观察：6 条 WB 提交、43 拍 MEM hold、1 次 EX redirect；四级有效槽检查次数为 29、6、49、6；错误路径 Store 次数为零。
- 顶层 lint 通过。

顶层测试的非零标记只用于承载验证，不是真实异常检测。它仍使用当前普通执行路径，因此六条提交不能解释为“故障指令可以退休”或“trap 已验收”。

实现前运行同一专项，在 `npc/build/x-payload-bViPPu/` 观察到三个后级寄存器缺少 18 个新增端口的预期 elaboration 失败。这是接口缺失检查，不是实现前完成过功能仿真。

### 既有 E 回归与测试结束修正

首次回归目录 `npc/build/e-pipeline-SaW9up/` 中，`e_dependencies_tb` 在达到 PASS 条件后调用 `$finish`，随后同一执行流程又落入末尾的 `$fatal("dependency watchdog")`。本工具运行中进程仍返回零，旧脚本仅依赖退出码，可能把含 Fatal 的正例计为成功。

本次只修正该 testbench 的结束控制：成功后跳出循环，watchdog 仅在未 halt 时触发；`run_e_pipeline.sh` 对正例与随机正例同时要求 PASS 标记、无 `%Fatal/%Error`。负例仍要求非零退出并匹配预期诊断。

修正后证据目录：`npc/build/e-pipeline-u7qWkq/`。最终结果：六个 E 专项与三个随机 seed 均通过，正例日志无 Fatal/Error；三个 mutation 均按预期被拒绝。`e_dependencies` 观察到 8 条提交、69 拍依赖等待、1 次 Store，成功结束后未再触发 watchdog。

## 观测边界

- 本步未修改生产 DPI 事件合同，未新增生产 probe；新测试读取原生 RTL 状态。未采集或复核本次 VCD，不声称波形资格验证通过。
- 未重跑 CoreMark、35 项 cpu-tests 或 RT-Thread/Difftest 长回归；历史结果不作为本次版本的通过证据。
- 后续接入非零生产异常时，必须补齐故障指令不普通写回/退休、年轻请求许可及 WB trap，不能只打开 exc_valid 输入。
