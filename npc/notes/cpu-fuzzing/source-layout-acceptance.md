# NPC 源码目录迁移验收

## 1. 运行与范围

- 日期：2026-10-22。
- 运行标识：`source-layout-zna4p7ei`。
- 环境：现有 `ysyx-npc` 容器，Verilator 5.048；生产构建使用 4 个编译任务，专项脚本沿用 2 个任务。
- 配置：工作区当前 `.config`，RV32、Difftest 开启，Difftest 文件日志/ITRACE/WAVE 关闭；配置快照和哈希保存在运行目录。
- 证据目录：[npc/build/source-layout-zna4p7ei](../../build/source-layout-zna4p7ei/)。
- 长期目录约定和命令见 [源码目录与构建入口](../design-process/source-layout.md)。

本次迁移在已有用户改动上进行，未创建提交。编译和仿真均在项目容器内执行，未安装工具或修改宿主机及其他容器环境。

## 2. 路径映射与内容保持

| 原路径 | 新路径 | 数量 |
| --- | --- | ---: |
| `npc/csrc/**/*.cpp` | `npc/src/csrc/src/**/*.cpp` | 18 |
| `npc/vsrc/*.v` | `npc/src/vsrc/src/*.v` | 20 |
| `npc/vsrc/*.vh` | `npc/src/vsrc/include/*.vh` | 4 |
| `npc/include/*.h`，仅根目录仿真头文件 | `npc/src/csrc/include/*.h` | 10 |

总计 52 个源文件/头文件，迁移前后逐文件 SHA-256 一致。41 个受保护配置文件的哈希保持一致，包括 `.config`、`Kconfig`、配置预设、`include/config/` 和 `include/generated/`。`migration-manifest.json` 保存每个文件的新旧路径与原哈希，`before.tar.gz` 保存迁移前源码及相关脚本备份。

RTL/C++ 生产实现正文未改变，未调整流水级时延、状态机、组合路径、握手、reset、redirect/kill、异常载荷或 DPI 函数签名。`__FILE__` 展开的日志路径随源文件位置变化。

同步更新了根 Makefile、容器构建/入口/lint 包装器、测试入口及测试相对 include、VS Code 配置和当前目录说明。新增 `scripts/source_paths.sh` 为 Shell 入口递归收集有序源码列表，修正空目录 `inlcude` 的拼写。历史验收文件保留原路径记录。

## 3. 验证命令和实际结果

以下命令除包装器检查外，均在容器内 `npc/` 目录执行。日志由外层保存到本次运行目录，测试自身仍创建独立产物目录。

| 检查/命令 | 实际结果 | 日志或产物 |
| --- | --- | --- |
| 迁移前 lint、`NPC_BUILD_JOBS=4 bash container/build.sh`、add Difftest | 通过；GOOD TRAP `0x80000120`，REF 执行 840 条指令 | `baseline-*.log`、`baseline-sim` |
| 更新后所有 Shell 入口 `bash -n`，IDE JSON 解析、活动旧路径扫描 | 通过 | `static-checks.log` |
| `source scripts/source_paths.sh` | 递归收集 20 个 RTL、4 个 RTL 头文件、18 个 C++ 源文件 | `source-counts.log` |
| 新路径下 `verilator --lint-only --top-module core_top` | 通过 | `migrated-lint.log` |
| `NPC_BUILD_JOBS=4 bash container/build.sh`，归档旧模型后重新构建 | 通过 | `migrated-fresh-build.log` |
| `make sim git_commit=` | 通过，自动提交钩子禁用 | `make-sim.log` |
| 新仿真器运行同一个 add 镜像及 REF | 通过；GOOD TRAP `0x80000120`，REF 执行 840 条指令 | `migrated-add.log` |
| `bash tests/run_x_payload.sh` | 两个测试及 lint 通过；四个寄存器的 reset/hold/flush 等检查，顶层 6 次提交、43 次保持、1 次 redirect | `x-payload-suite.log`、`build/x-payload-CxuUqF/` |
| `bash tests/run_e_pipeline.sh` | 六个定向测试、三组随机 seed、三个故障注入反例通过 | `e-pipeline-suite.log`、`build/e-pipeline-NYwYp8/` |
| `bash tests/run_e_protocol.sh` | 四组时延/随机运行及四个协议反例通过 | `e-protocol-suite.log`、`build/e-protocol-byzAPU/` |
| `NPC_BUILD_JOBS=4 bash tests/run_e_regression.sh` | 35/35 cpu-tests Difftest 通过；有效路径访问错误的 fail-stop 反例通过 | `e-regression-suite.log` |
| `bash tests/run_e_device.sh` | 15 次提交、3 次键盘消费、1 次设备写入、24 次请求与响应匹配；原生 VCD/DPI 对照 461 行、WB=15、MMIO=4；损坏身份字段反例被拒绝 | `e-device-suite.log`，具体目录见日志首行 |
| `bash tests/run_diff_log.sh`，修正测试驱动后 | 开/关文件日志均能检测日志限额后的 x31 损坏；启用时保留 2 条记录，关闭时无日志文件；生产入口 GOOD TRAP | `diff-log-final.log`、`build/diff-log-test-icibd5/` |
| 宿主机加载 `container/npc.sh` 后运行 `npc pwd` | 返回容器中的 `/workspace/ysyx-workbench/npc` | `container-entry.log` |
| `container/verilator --lint-only` 检查新路径 core_top 与 x_payload_core_tb | 两个检查通过，实际 lint 在项目镜像内执行 | `vscode-rtl-lint.log`、`vscode-testbench-lint.log` |

E 流水线随机 seed 为 `1`、`73`、`20261018`；协议与设备测试使用脚本固定的 seed `73`；35 个 cpu-tests 使用请求延迟 3、响应延迟 5、随机上界 7、seed 73。输入镜像与 REF 哈希由原回归脚本保存在各自产物目录，复用已有镜像，没有重编译 guest 或 REF。

## 4. 本次定位并处理的问题

### 4.1 旧模型依赖仍指向原路径

首次完整构建失败于 `obj_dir/audio.d`：旧依赖仍要求 `../csrc/device/audio.cpp`，而新 `Vcore_top.mk` 已使用新路径。完整旧 `obj_dir` 已归档为运行目录下的 `baseline-obj_dir/`，随后重新生成模型并构建成功。未删除历史验收目录。

首次失败日志保留在 `migrated-build.log`；新构建结果见 `migrated-fresh-build.log`。

### 4.2 日志测试驱动缺少初始化并假定一拍必有比较

`tests/log_limit_test.cpp` 自己创建模型并绕过生产 `main()`。首次执行因未调用 `Verilated::commandArgs(argc, argv)` 而在后端读取 plusarg 时退出；补齐初始化后，测试暴露出“损坏寄存器后只运行一拍就期待 Difftest 比较”的旧假设。当前访存等待期间可能没有 WB 事件，因此这一拍不会比较。

仅修改该测试驱动：补齐参数初始化，并在 `cycle<200` 的界限内等待下一次提交/比较事件；实际生产仿真代码未改。修正后两种日志配置均在 PC `0x80000014` 检测出预期 x31 mismatch。失败及最终结果分别保存在 `diff-log-suite.log`、`diff-log-retry.log`、`diff-log-final.log`。

## 5. 适用边界

- 本记录验收的是目录迁移、相关构建与测试入口，以及上述功能回归；不表示未覆盖的 RTL 行为已得到证明。
- 本次实际重新运行了 E 设备测试的原生 VCD/DPI 资格检查；未新增生产 probe 或修改事件合同。
- X payload 通过仅说明异常信息传递，不能代替完整的精确异常 trap 验收。
- 本轮未重跑 CoreMark 1000 迭代、MicroBench 或 RT-Thread 完整系统验收；旧通过记录保留其原版本边界。
- VS Code 包装器命令已实际验证；IDE 界面诊断需要用户重新加载窗口刷新。

最终文件哈希见运行目录中的 `sources-after.sha256`，从工作区根目录执行 `sha256sum -c npc/build/source-layout-zna4p7ei/sources-after.sha256` 可复核记录对应版本。
