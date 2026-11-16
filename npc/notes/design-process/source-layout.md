# NPC 源码目录与构建入口

## 1. 目录职责

```text
npc/
├── src/
│   ├── csrc/
│   │   ├── include/          # C++ 仿真环境接口，包括 npc.h、DPI、Difftest
│   │   └── src/              # main.cpp、device、difftest、memory、monitor
│   └── vsrc/
│       ├── include/          # ctrl_defs.vh、mem_if_defs.vh、dpi_imports.vh、exception.vh
│       └── src/              # RTL 模块
├── include/
│   ├── config/               # Kconfig 生成文件
│   └── generated/            # autoconf.h
├── scripts/                  # 配置工具入口、源码路径收集
├── container/                # 容器入口、构建与 VS Code lint 包装器
├── tests/                    # 测试台、驱动、检查脚本
├── configs/                  # 配置预设
├── build/                    # 仿真程序和独立验收产物
└── obj_dir/                  # 当前生产 Verilator 模型
```

C++ 仿真环境的公共接口也属于 `src/csrc/include/`。例如 `npc.h` 依赖 Verilator 模型及仿真状态；`include/config/`、`include/generated/` 保持配置系统原有输出位置。新增真正独立的项目公共定义时，可另行放入根 `include/`。

## 2. 路径与递归收集

| 内容 | 路径 | Makefile 变量 | Shell 变量 |
| --- | --- | --- | --- |
| RTL 实现 | `src/vsrc/src/` | `RTL_SRC_DIR` | `npc_rtl_dir` |
| RTL 头文件 | `src/vsrc/include/` | `RTL_INC_DIR` | `npc_rtl_include_dir` |
| C++ 实现 | `src/csrc/src/` | `CPP_SRC_DIR` | `npc_cpp_dir` |
| C++ 仿真头文件 | `src/csrc/include/` | `CPP_INC_DIR` | `npc_cpp_include_dir` |

Makefile 通过 `find` 递归收集实现和头文件。Bash 构建、测试与 lint 包装器共用 [source_paths.sh](../../scripts/source_paths.sh)，提供按路径排序的 `npc_rtl_sources`、`npc_rtl_headers`、`npc_cpp_sources` 数组，避免仅使用根目录 `*.v` 漏掉子目录模块。

头文件当前集中在单层 `include/`。以后若进一步分目录，应按相对于该 include 根目录的路径引用，例如 `decode/defs.vh`，并检查重名文件。

原有名称引用继续有效：

```cpp
#include <npc.h>
#include <dpi_export.h>
#include <generated/autoconf.h>
```

```verilog
`include "ctrl_defs.vh"
`include "exception.vh"
```

C++ 同时搜索 `src/csrc/include/` 和根 `include/`；生成模型头文件仍从 `obj_dir/` 搜索。

## 3. 构建和验证命令

以下命令在 `ysyx-npc` 容器内、工作目录 `npc/` 执行：

```bash
source container/env.sh
source scripts/source_paths.sh

# RTL lint
verilator --lint-only --top-module core_top \
  -I"$npc_rtl_include_dir" "${npc_rtl_sources[@]}"

# 完整仿真器，默认两个编译任务；不启动 guest
NPC_BUILD_JOBS=2 bash container/build.sh

# Makefile 入口；验收时禁用上层自动提交钩子
make sim git_commit=

# 原有专项与 Difftest 入口
bash tests/run_x_payload.sh
bash tests/run_e_pipeline.sh
bash tests/run_e_protocol.sh
bash tests/run_e_device.sh
bash tests/run_e_regression.sh
```

可执行文件继续为 `build/sim`，AM 调用的 NPC 根 Makefile 位置保持不变。项目的 `npc` 容器函数继续从根 `npc/` 启动。

VS Code Remote-SSH 的 C++ 与 Verilog include 路径已对应新目录。迁移后可执行 `Developer: Reload Window` 刷新已打开的旧文件与诊断。

工作区同时包含宿主仿真 C++ 和 AM 客户程序。默认 `NPC` IntelliSense 配置除仿真路径外，还需要保留 `abstract-machine/am/include`、`abstract-machine/am/src`、`abstract-machine/klib/include`，分别解析 `am.h`、`riscv/riscv.h`、`klib.h`；`am.h` 通过已有的 `ARCH_H=\"arch/riscv.h\"` 宏选择 RISC-V 上下文定义。只配置 `npc/src/csrc/include` 无法覆盖 AM 源文件，会导致 `Context`、`Event` 等类型连带报错。这些编辑器路径不修改 AM Makefile 的编译参数；客户程序的实际构建仍在 `ysyx-npc` 容器中使用 RISC-V 工具链。

## 4. 迁移及证据维护

目录迁移仅改变源文件位置、构建路径、测试中的路径引用与 IDE 配置。RTL/C++ 实现正文通过迁移前后 SHA-256 对照检查；流水线、握手、DPI ABI、异常检测逻辑均不在本次修改范围内。

旧 `obj_dir/*.d` 可能仍引用原源码路径，应归档后重新生成模型。历史验收目录、日志和哈希清单保留原样；旧清单中的路径对应当时版本，不能直接作为新目录的校验命令。

迁移前后编译器展开的 `__FILE__` 会显示不同源码路径，日志中的文件定位文字因此可能变化。这与架构状态差异应分开判断。

本次迁移的运行标识、命令、通过结果和验证边界见 [源码目录迁移验收记录](../cpu-fuzzing/source-layout-acceptance.md)。
