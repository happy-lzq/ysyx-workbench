# RT-Thread 完整 AM 应用构建验收（2026-10-11）

## 问题与原因

用户在默认应用集成入口构建 RT-Thread 时，snake 缺少 `stdlib.h`、FCEUX 缺少 `inttypes.h`，最终出现 `__am_fceux_am_main` 未定义。

本次核验的 NPC 容器使用 `/usr/bin/riscv64-unknown-elf-gcc` 13.2.0；`npc/container/env.sh` 已选定该前缀。`libnewlib-dev` 4.4.0.20231231-2 已提供 `/usr/include/newlib`，但该编译器默认不搜索此目录。此前头文件配置只在 RT BSP 中生效，独立子应用构建没有获得它。移除 `-isystem /usr/include/newlib` 的独立预处理仍可复现缺头文件，加入后成功。

错误继续传播有两层原因：应用集成脚本未检查子 make/nm/objcopy 返回码；BSP 使用 `-include` 加载应用清单，清单更新失败也能继续构建。故最终链接错误是前面的应用编译失败所造成。

## 修改与长期构建约定

1. `abstract-machine/scripts/isa/riscv.mk`：仅在 `CROSS_COMPILE` 包含 `unknown-elf` 时，为 CFLAGS 添加 `-isystem $(NEWLIB_INCLUDE)`，默认 `/usr/include/newlib`。公共 CXXFLAGS 引用 CFLAGS，因此 C++ 也获得该路径。运行时仍由既有 AM/RT 链接输入提供，没有新增 Newlib 库链接。
2. `am-kernels/rt-thread-am/bsp/abstract-machine/Makefile`：去掉重复的 Newlib 路径，保留 RT 所需 POSIX 可见性；正常目标必须成功包含应用清单，单独 `clean` 不触发应用集成。
3. `integrate-am-apps.py`：显式传递选定的交叉编译前缀；子构建默认 `AM_APP_JOBS=2`；检查 make、nm、objcopy 返回码和应用对象存在性；先完成全部子构建，再复制/转换对象，全部转换成功后发布生成的 C 文件和清单。子构建失败不会覆盖旧清单及包装文件。这里不宣称 objcopy 中途失败时转换对象也能全部回滚。

`npc/container/env.sh` 本来已经选择 NPC 的 unknown-elf 工具链，本次无需修改。没有安装宿主机软件、重建容器或修改 nanhu 环境；没有更改工具链程序。公共 Makefile 的路径设置随工作区源码生效，使用该工作区和 unknown-elf 的其他构建也应提供 Newlib 头文件，必要时通过 `NEWLIB_INCLUDE` 指定路径。

## 验收输入和证据

证据目录：`npc/build/rtthread-app-build-8n07ttix`。所有实际编译、预处理和仿真均在 `ysyx-npc` 容器内进行。保留了修改前文件、当前任务补丁、故障注入夹具、构建日志、完整镜像/ELF、模拟器/REF 副本、配置和 SHA-256。

- ARCH：`riscv32I-npc`，既有脚本实际使用 RV32IM_Zicsr / ilp32。
- GCC/G++：13.2.0；Verilator：5.048；Newlib 头文件包：4.4.0.20231231-2。
- NPC 配置：DIFFTEST、HAS_TIMER 开启；DIFF_LOG_ENABLE、ITRACE、WAVE 关闭。启动保留生产宿主 timer；事件时机依赖宿主运行时间，不使用随机 seed。
- 新 RT bin：635788 字节，完整五应用清单。镜像、REF、模拟器及关键源文件散列见 `inputs.sha256`；工具信息见 `toolchain.txt`。
- 启动通过生产 `single_cycle()` / Difftest 执行；SDL dummy，20 秒有界运行。命令和检查保存在 `verify_boot.sh`。

| 检查 | 本次实际结果 |
| --- | --- |
| 独立 snake image | 编译、链接、objcopy 成功，退出 0 |
| 独立 FCEUX image | C++ 编译、链接、objcopy 成功，退出 0 |
| 完整 RT image | hello、microbench、typing-game、snake、FCEUX 全部纳入，链接成功，退出 0 |
| 包装入口与符号 | 五个 `__am_*_main` 在 ELF 中均有定义，五个 shell 导出项存在 |
| 子构建失败测试 | 修改前脚本继续并覆盖清单；修改后立即非零退出，保留旧清单及包装文件，未构建后续应用 |
| BSP 失败测试 | 修改前 `-include` 后仍执行 image；修改后停止，未执行 image，保留旧生成文件 |
| NPC 模型构建 | `make sim git_commit=` 退出 0 |
| 完整镜像 Difftest 启动 | 输出 Hello RISC-V、shell、help/ps/memcheck；help 列出五个 AM 命令；20 秒内无可见 mismatch/ABORT/BAD TRAP |
| 有界运行退出 | timeout 返回 124，符合持续运行的 OS；不是 GOOD TRAP 正常退出 |
| RTL 保持 | `npc/vsrc` 的 21 个 RTL/头文件及文件集合与修改前 SHA-256 一致 |
| Python/clean | 脚本语法检查通过；BSP `make -n ... clean` 成功，未执行删除 |

日志：`snake-build.log`、`fceux-build.log`、`rtthread-build.log`、`rtthread-final-build.log`、`sim-build.log`、`boot.log`、`boot-check.log`、`rtl-after-check.log`。`check_failed_app.py`、`check_failed_bsp.py` 和各 `fail-case-*/result.log` 保存故障复现及复验。

## 在容器内继续运行

```bash
source /workspace/ysyx-workbench/npc/container/env.sh
cd "$WORKSPACE/am-kernels/rt-thread-am/bsp/abstract-machine"
make -j2 ARCH=riscv32I-npc run git_commit=
```

默认入口包含五个应用。无需重建容器或运行 make clean；如需限定子应用并行度，设置 `AM_APP_JOBS=2`。`git_commit=` 避免构建规则自动创建 Git 提交。

复验保存的完整镜像启动：

```bash
source /workspace/ysyx-workbench/npc/container/env.sh
cd "$NPC_HOME"
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy timeout -k 2s 20s \
  build/rtthread-app-build-8n07ttix/sim \
  -i build/rtthread-app-build-8n07ttix/rtthread.bin \
  -e build/rtthread-app-build-8n07ttix/rtthread.elf \
  -d build/rtthread-app-build-8n07ttix/ref.so
```

## 结论边界

本次闭环覆盖缺头文件、失败传播、应用符号集成和完整镜像的有界 Difftest 启动。尚未逐个执行游戏、MicroBench 和 FCEUX；shell 出现命令不等于应用运行正确。保留既有 RT 链接脚本的 RWX LOAD segment 警告，本次未调整其权限布局。

没有修改 RTL，因此流水级、访存时延、状态机、握手、reset 和 redirect/kill 逻辑均未因本次修改改变。本次未新增插桩或采集 VCD；既有 RT timer/context 波形资格记录仍见 `rtthread-acceptance.md`，不能把它表述为本次新 VCD 验收。本次启动没有独立统计 IRQ 次数，不能据此宣称全部中断行为或 RTL 正确性得到覆盖。
