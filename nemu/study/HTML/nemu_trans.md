# NEMU 项目设计与架构分析

## 1. 整体架构概览

NEMU (NJU Emulator) 的设计遵循 **"程序 = 状态机"** 的核心思想。整个模拟器主要由以下几个部分组成：

*   **Monitor (监控器)**: 相当于模拟器的“外壳”或“操作系统”。它负责初始化、加载程序、提供用户交互界面（SDB）。
*   **CPU (中央处理器)**: 负责取指、译码、执行。这是模拟器的核心。
*   **Memory (存储器)**: 模拟物理内存和虚拟内存。
*   **Device (设备)**: 模拟外设（如串口、时钟、VGA等）。

## 2. 代码目录结构与功能

关键目录及其功能如下：

*   `src/nemu-main.c`: **入口函数**。程序的起点。
*   `src/monitor/`: **监控器核心**。
    *   `monitor.c`: 负责初始化 (`init_monitor`) 和加载镜像。
    *   `sdb/`: **简易调试器 (Simple Debugger)** 的实现。
        *   `sdb.c`: 调试器的主循环 (`sdb_mainloop`) 和命令解析。
        *   `expr.c`: 表达式求值模块（用于 `p` 命令）。
        *   `watchpoint.c`: 监视点池管理（用于 `w` 命令）。
*   `src/cpu/`: **CPU 模拟**。
    *   `cpu-exec.c`: 指令执行的主循环 (`cpu_exec`)。
*   `src/engine/`: 模拟引擎（解释器模式）。
*   `include/`: 头文件，定义了各种数据结构和接口。
    *   `isa.h`: 定义了 ISA 相关的接口（如寄存器结构）。

## 3. 运行流程分析 (Execution Flow)

程序的生命周期如下：

1.  **启动 (`src/nemu-main.c`)**:
    *   调用 `init_monitor(argc, argv)` 进行初始化。
    *   调用 `engine_start()` 开始运行。

2.  **初始化 (`src/monitor/monitor.c`)**:
    *   解析参数（如 `-l` 日志, `-b` 批处理模式）。
    *   `load_img()`: 将客户程序（Guest Program）镜像加载到模拟内存中。
    *   `init_sdb()`: 初始化调试器相关结构（如正则引擎、监视点池）。

3.  **进入主循环 (`src/monitor/sdb/sdb.c` -> `engine_start`)**:
    *   如果是批处理模式，直接运行。
    *   如果是交互模式，进入 `sdb_mainloop()`。
    *   **`sdb_mainloop()`**: 这是调试器的核心。它使用 `readline` 读取用户输入，解析命令（如 `c`, `si`, `info r`），并调用对应的处理函数。

4.  **指令执行 (`src/cpu/cpu-exec.c`)**:
    *   当用户输入 `c` (continue) 或 `si` (step) 时，最终会调用 `cpu_exec(n)`。
    *   `cpu_exec` 循环调用 `execute(n)` -> `exec_once()`。
    *   `exec_once()` 完成一条指令的生命周期：**取指 -> 译码 -> 执行 -> 更新 PC**。

## 4. 简易调试器 (SDB) 开发准备

后续添加调试器功能的关键模块：

### A. 命令解析框架 (`sdb.c`)
目前 `cmd_table` 数组定义了支持的命令。
```c
static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = { ... };
```
**任务**: 在这里注册新的命令（如 `si`, `info`, `x`, `p`），并实现对应的 `cmd_xxx` 函数。

### B. 表达式求值 (`expr.c`)
调试器需要计算表达式（例如 `p $eax + 1`）。
**原理**: 使用正则表达式 (`regex.h`) 进行词法分析 (`make_token`)，然后使用递归下降算法进行语法分析和求值。

### C. 监视点 (`watchpoint.c`)
监视点用于在内存或变量发生变化时暂停程序。
**原理**: 维护一个监视点链表。在 `cpu_exec` 的每次循环末尾，扫描链表中的所有监视点，计算其表达式的值，如果发生变化则暂停 CPU。

### D. 寄存器访问 (`isa.h` / `src/isa/.../reg.c`)
调试器需要读取寄存器状态。
**原理**: `CPU_state` 结构体（定义在 `isa-def.h` 中）存储了寄存器的值。
**任务**: 实现 `isa_reg_display()` (打印所有寄存器) 和 `isa_reg_str2val()` (根据名字读取寄存器值)。
