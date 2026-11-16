# NPC Verilator 容器

该镜像继承本机已有的 `xiangshan-nanhu:local`，复用其中的 Verilator、G++、SDL2 和 readline。容器 `ysyx-npc` 挂载整个 `ysyx-workbench` 到 `/workspace/ysyx-workbench`，进入目录为 `npc`，运行 UID/GID 与宿主用户一致。容器 HOME 保存在 `~/.cache/ysyx-npc-home`。

首次构建镜像：

```bash
docker build -t ysyx-npc:local -f ~/ysyx-workbench/npc/container/Dockerfile ~/ysyx-workbench/npc/container
```

新终端输入 `npc` 进入持久容器；入口会加载 `container/env.sh`，使用与 `nanhu-dfz` 一致的绿色用户/主机名、蓝色路径提示符。输入 `npc verilator --version` 可直接运行一条容器内命令。当前终端可执行 `source ~/.bashrc` 加载函数。

在容器内，从 `npc` 目录执行 RTL lint：

```bash
source scripts/source_paths.sh
verilator --lint-only --top-module core_top -I"$npc_rtl_include_dir" "${npc_rtl_sources[@]}"
```

`npc/Makefile` 的 `sim` 目标会调用上级 Makefile 的 `git_commit`，因此验证构建不要直接运行 `make sim`。容器内用下面的入口构建完整 RTL＋C++ 仿真程序；生成头文件位于 `npc/obj_dir`，可执行文件为 `npc/build/sim`：

```bash
bash container/build.sh
```

也可在宿主终端运行 `npc bash container/build.sh`。该脚本默认使用两个编译任务，可用 `NPC_BUILD_JOBS` 调整。它只构建，不启动仿真。

VS Code 若连接到 `ysyx-npc` 容器，HDL 插件需要在该容器环境中启用 `verilog.linting.linter: verilator`。

## VS Code Remote-SSH 中的语法检查

SSH 窗口中的扩展运行在宿主机，而 Verilator 和 SDL2、readline 头文件安装在容器里。工作区 `.vscode/settings.json` 将 Verilog-HDL 的 Verilator 路径指向 `npc/container/verilator`，脚本把 lint 命令转交给 `ysyx-npc:local`。重新加载 VS Code 窗口后，打开或保存 `npc/src/vsrc/src/` 下的 `.v` 文件可触发 lint；也可运行命令 `Verilog: Rerun lint tool`。

包装器跳过 `npc/build/` 中的验收快照和 `npc/obj_dir/` 中的生成文件；这些文件不是当前 RTL 工程的检查入口。否则归档测试台可能经工作区 include 路径加载当前 RTL，产生批量 `TIMESCALEMOD` 或旧接口诊断。当前生产 RTL 与 `npc/tests/` 下的测试台仍正常检查。修正入口后，已有诊断可通过重新检查原文件或 `Developer: Reload Window` 刷新。

C/C++ IntelliSense 在 SSH 宿主机读取头文件。构建完仿真程序生成 `npc/obj_dir/Vcore_top.h` 后，执行：

```bash
bash npc/container/sync-vscode-headers.sh
```

此命令把容器中的 Verilator、SDL2 和 readline 头文件复制到忽略版本控制的 `npc/.vscode-includes`。在 VS Code 执行 `C/C++: Select a Configuration` 并选择 `NPC`，然后执行 `Developer: Reload Window`；修改镜像后重新运行同步脚本。真正的编译和仿真仍在 `ysyx-npc` 容器内执行。
