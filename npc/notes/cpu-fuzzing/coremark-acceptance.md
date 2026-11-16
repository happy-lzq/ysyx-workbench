# CoreMark 五级流水线功能验收

## 运行方法

在项目指定的 `ysyx-npc` 容器中运行：

```bash
cd /workspace/ysyx-workbench/npc
bash tests/run_coremark.sh
```

入口依次构建当前配置的 NEMU REF、生产 NPC 仿真程序和 `am-kernels/benchmarks/coremark` 的 `riscv32I-npc` 镜像，再复制输入与 REF 到新的 `npc/build/coremark-acceptance-*` 目录。所有构建使用 `git_commit=`，不触发构建脚本中的自动 Git 提交；不修改 RTL、Kconfig 或容器软件。实际编译 ISA 由 `abstract-machine/scripts/riscv32I-npc.mk` 决定，目前是 RV32IM + Zicsr，架构名称中的 I 不能作为无 M 扩展的证明。

需要通过 `make menuconfig` 开启 `DIFFTEST`。`DIFF_LOG_ENABLE` 独立控制 Difftest 文件日志，默认关闭：关闭时不创建 diff-log 文件，REF 执行、PC/GPR/CSR 对比和终端失配报告仍有效。开启日志后，默认 `COREMARK_LOG_LIMIT=10000`，`COREMARK_TIMEOUT=1800` 秒，可以通过同名环境变量调整。`--log-limit N` 只限制常规 ITRACE/Difftest 文件记录数量；已开启 Difftest 日志时，GPR/CSR 失配仍输出完整错误记录。`N=0` 表示无限记录，不表示关闭日志。ITRACE 是另一个配置开关；开启时最近指令环形缓冲继续更新。运行入口要求进程退出码 0、CoreMark PASS 和 GOOD TRAP 同时成立，并检查编译配置确实启用了 DIFFTEST。

RTC 仍来自宿主时间，timer 源按当前配置启动；headless SDL 只改变宿主显示/音频后端。输出的 Marks 或耗时不能解释成硬件 CoreMark/MHz。本入口属于功能验收，不能替代独立的 IRQ、MMIO 消费和微架构验证。

## 2026-10-04 验收记录

输入：仓库原始 CoreMark 镜像，2000 字节总数据规模、1000 次迭代，未修改算法、种子或迭代次数。镜像大小 16196 字节，GCC 13.2.0；容器内 Verilator 5.048。当前工作区有用户未提交改动，本次结果绑定运行目录中的源码/输入/模型哈希，不代表单独的 Git 提交。

### 首次运行与 C++ 修改

原来的 ITRACE 和 Difftest 每条事件都写文件。首次试跑在约 54836 个事件后达到本次设置的 64 MiB 文件上限，退出码 153；这是日志资源限制，不是观察到的 RTL 失配。证据保存在 `npc/build/coremark-20261004-CLRFfn`。

本次只修改 C++ 日志控制：

- `include/npc.h`、`csrc/monitor/monitor.cpp`：增加 `npc_log_limit` 和 `--log-limit N`，默认 0 保持旧行为；非法参数返回 1。
- `csrc/monitor/trace/itrace.cpp`：文件日志达到限制后停止常规写入，最近指令环形缓冲继续更新。
- `csrc/difftest/dut.cpp`：每次事件先比较，再按日志限制写常规记录；ABORT 状态始终写错误记录。
- 新增 `tests/run_coremark.sh` 和 `tests/log_limit_test.cpp`。没有修改流水线时序、valid/kill、DPI 采样或设备消费逻辑。

### 1000 次迭代实际运行

在 `npc` 目录，以重新构建的生产 `build/sim` 运行以下等价命令；实际设置了 `ulimit -c 0` 和每个文件 64 MiB 的上限：

```bash
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy \
timeout -k 2s 900s ./build/sim --log-limit 10000 \
  -i build/coremark-full-20261004-gr77Ra/coremark.bin \
  -e build/coremark-full-20261004-gr77Ra/coremark.elf \
  -d /workspace/ysyx-workbench/nemu/build/riscv32-nemu-interpreter-so
```

| 项目 | 实际观察 |
|---|---|
| Iterations | 1000 |
| seedcrc | `0xe9f5` |
| crclist | `0xe714` |
| crcmatrix | `0x1fd7` |
| crcstate | `0x8e3a` |
| crcfinal | `0xd340`，迭代次数相关 |
| CoreMark 自检 | `CoreMark PASS` |
| NPC / NEMU 退出 | 两者均 GOOD TRAP，PC `0x80002058` |
| 进程退出码 | 0 |
| Difftest | 持续开启，全程未报告失配 |
| NEMU guest instructions | 313360041；MMIO 指令通过同步处理，不计入此 REF 执行计数 |
| 程序报告的宿主计时 | 299325 ms；不是 RTL 周期数 |

实际日志、退出码和 SHA-256：`npc/build/coremark-full-20261004-gr77Ra`。Difftest 文件约 12 MiB，ITRACE 文件约 572 KiB。`vsrc/*` 20 个文件与运行前 SHA-256 一致。本次完成后执行 `make -C nemu SHARE=1 git_commit=`，输出 Nothing to be done，确认当前 REF 构建依赖无需更新。

### 日志限制不会关闭检查的负例

`tests/log_limit_test.cpp` 使用生产仿真循环和 NEMU，输入为 10 条 NOP、清零 a0 和 EBREAK。日志限制设为 1；处理完第 5 个事件后，在该独立测试进程中从 C++ 人为修改 DUT x31，然后继续运行一拍。

观察：Difftest 在 PC `0x80000014` 检出 GPR[31] 失配；文件有两条完整记录，分别为常规第 0 条和失配第 5 条。测试输出 `log_limit_test PASS: mismatch_detected=1 records=2 failure_record=1`，退出码 0。产物保存在 `npc/build/log-limit-test-9BFcHw`。这是 C++ 检查路径的受控负例，不是发现了 RTL bug。

复现该负例时，先在容器内 `make sim git_commit=`，将测试链接至 `obj_dir` 中除 `main.o`、`Vcore_top__ALL.o` 外的生产对象及 `Vcore_top__ALL.a`，使用与生产相同的 Verilator/SDL include 和链接库；输入镜像中的 12 个小端字依次为十个 `0x00000013`、`0x00000513`、`0x00100073`。测试调用方式为 `log_limit_test <input.bin> <ref.so>`。

### 验收边界

本次没有采集 VCD，没有改变 DPI 插桩事件合同，也没有进行新的插桩波形资格验证。新的运行脚本通过 shell 语法检查；上述完整运行使用其等价构建/执行命令，没有再重复第二轮 1000 次迭代。

本次证据支持当前配置下 CoreMark 的功能运行与提交状态对齐。该程序没有建立中断处理程序来专门验收 timer trap；不能由本次 PASS 推导所有中断边界、所有 MMIO 设备行为或整个 RTL 已无 bug。前述在途 CSR 读与异步 pending 更新的时序问题仍需独立处理。

## 2026-10-04：Difftest 检查与文件日志配置拆分

根因：原实现只通过运行参数限制记录数量，`difftest_init()` 仍无条件创建日志文件。用户新增的 `diff_log_enable` 与 `#ifdef Enable Diff_Log` 没有对齐；后者带两个 token，不能表达该 Kconfig 配置。

本轮修改 `npc/Kconfig`、`csrc/difftest/dut.cpp` 和 `include/difftest.h`，统一为 `DIFF_LOG_ENABLE` / `CONFIG_DIFF_LOG_ENABLE`，依赖 DIFFTEST，默认 n。同时控制日志函数编译、文件创建和事件写入；REF 初始化与 `difftest_compare()` 独立保留。通过 Kconfig 工具同步现有配置，最终为 `CONFIG_DIFFTEST=y`、`# CONFIG_DIFF_LOG_ENABLE is not set`，没有手写生成头或修改 RTL。`tests/run_coremark.sh` 额外保存配置，并在 DIFFTEST 未编译时拒绝将运行标为 Difftest 验收。

验证：先更新 `tests/log_limit_test.cpp`，在修改生产代码前观察到日志关闭用例失败：`mismatch_detected=1 no_file=0`，证据在 `build/diff-log-config-shYnsB`。修改后在 ysyx-npc 容器运行：

```bash
cd /workspace/ysyx-workbench/npc
bash tests/run_diff_log.sh
```

该入口构建生产仿真程序，在新的产物目录中用 Kconfig 生成日志开启/关闭两份配置，不改变工作区配置；复用生产 RTL 模型和其他 C++ 对象，分别编译受配置影响的 Difftest 模块与测试驱动。两种配置都在第 5 个事件后从 C++ 注入 x31 失配。

实际观察（`build/diff-log-test-LBWRs8`）：

- 日志关闭：失配被检出，`diff_fp=nullptr`，没有创建 diff-log 文件，测试 PASS。
- 日志开启：常规第 0 条与失配第 5 条共两条完整记录，超过记录限制仍保留失配，测试 PASS。
- 工作区配置下运行生产 `build/sim`，12 条指令输入输出 GOOD TRAP，退出码 0，无 diff-log 文件。
- 整组构建/测试命令退出码 0；生产 RTL 与修改前 SHA-256 一致。

本次验证日志开关，不重复运行 1000 次迭代 CoreMark，也没有采集 VCD。日志关闭后缺少文件中的完整历史状态，但终端失配报告及 ABORT 退出仍保留；需要完整状态取证时重新开启日志。大容量文件主要消耗磁盘并带来文件缓存和 I/O 压力，不能把文件大小直接等同于进程内存占用。
