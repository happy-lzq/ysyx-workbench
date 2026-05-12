# Verilator 仿真框架 & Yosys 综合工具笔记

> 整理自 `nvboard/rtl_prj/other/notes.txt`。
> 内容涵盖 Verilator 编译参数、C++ 仿真框架模板、断言策略，以及 Yosys 命令行参数速查。

---

## 目录

- [一、Verilator 编译与使用](#一verilator-编译与使用)
  - [1.1 命令行参数](#11-命令行参数)
  - [1.2 仅检查模式（Lint）](#12-仅检查模式lint)
- [二、C++ 仿真框架（Testbench 模板）](#二c-仿真框架testbench-模板)
  - [2.1 标配初始化](#21-标配初始化)
  - [2.2 时钟步进器 tick()](#22-时钟步进器-tick)
  - [2.3 复位序列](#23-复位序列)
  - [2.4 组合逻辑驱动与观测](#24-组合逻辑驱动与观测)
  - [2.5 断言策略](#25-断言策略)
  - [2.6 结束与清理](#26-结束与清理)
  - [2.7 完整 Testbench 示例](#27-完整-testbench-示例)
- [三、设计原则速记](#三设计原则速记)
- [四、Yosys 命令行参数速查](#四yosys-命令行参数速查)
  - [4.1 操作选项](#41-操作选项)
  - [4.2 记录选项](#42-记录选项)
  - [4.3 开发者选项](#43-开发者选项)

---

## 一、Verilator 编译与使用

### 1.1 命令行参数

```bash
verilator --cc --exe --build --trace --Wall \
          --top-module $(TOP_MODULE) \
          -Mdir $(OBJ_DIR)
```

| 参数 | 说明 |
|------|------|
| `--cc` | 把 Verilog 转成 C++ 模型 |
| `--exe tb.cpp` | 把你的 testbench 一起纳入构建（你提供 `main()`） |
| `--build` | 自动调用生成的子 Makefile 完成编译/链接 |
| `--trace` | 启用波形接口（C++ 里还需 `traceEverOn` / `trace` / `dump`） |
| `--top-module <TOP>` | 明确顶层模块名 |
| `-Mdir obj_dir` | 所有中间产物放到 `obj_dir/` 目录 |

### 1.2 仅检查模式（Lint）

```bash
verilator --lint-only -Wall
```

不生成 C++ 代码，只做语法和规则检查。

---

## 二、C++ 仿真框架（Testbench 模板）

### 2.1 标配初始化

```cpp
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vdut.h"   // 按你的顶层模块名生成（如 Vcore_top.h）

static vluint64_t sim_time = 0;

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Verilated::traceEverOn(true);       // 启用波形

    Vdut* top = new Vdut;
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 1);                 // 层次深度按需
    tfp->open("wave.vcd");              // 可用 GTKWave 查看
    // ...
}
```

### 2.2 时钟步进器 `tick()`

```cpp
static void tick(Vdut* top, VerilatedVcdC* tfp) {
    // 低电平阶段
    top->clk = 0; top->eval(); tfp->dump(sim_time++);

    // 上升沿（时序逻辑在这里采样）
    top->clk = 1; top->eval(); tfp->dump(sim_time++);

    // 回到低电平（给组合逻辑一次稳定机会）
    top->clk = 0; top->eval(); tfp->dump(sim_time++);
}
```

> **为什么是三段？** ① 确保在上升沿之前信号稳定；② posedge 采样；③ 下一拍的组合逻辑传播到输出。

### 2.3 复位序列

```cpp
// 上电默认：先置复位，再跑若干拍让 reset 分支确实被采样
top->rst = 1; top->eval(); tfp->dump(sim_time++);
tick(top, tfp);
tick(top, tfp);

// 断言复位态（把预期写成可执行检查）
assert(top->some_state == EXPECTED_RESET_VALUE);

// 释放复位，再来一拍，使 DUT 进入正常运行态
top->rst = 0; top->eval(); tfp->dump(sim_time++);
tick(top, tfp);
```

**复位要点**：
- 必须先 `rst=1` + 至少 2 个 `tick()`，确保 Posedge 确实采样到了复位
- 复位态检查用 `assert`，首拍重点验
- 释放复位后再给一拍，让 DUT 从复位态转入正常运行态

### 2.4 组合逻辑驱动与观测

```cpp
// 组合块：改输入 → eval → 读输出
top->in_a = 0; top->in_b = 1;
top->eval(); tfp->dump(sim_time++);
assert(top->out_f == (top->in_a ^ top->in_b));  // 预期是 XOR
```

### 2.5 断言策略

| 类型 | 检查时机 | 示例 |
|------|---------|------|
| **组合断言** | 输入变化后 `eval()` 立刻判定 | `assert(top->out == expected)` |
| **时序断言** | `tick()` 后判定（寄存器已更新） | `assert(top->reg_q == next_val)` |
| **关键事件** | 复位后第一拍、状态翻转边界、计数回零、握手/ready-valid 采样点 | — |

> 让断言成为"可执行规格"——一旦失败，进程非 0 退出，CI 清晰可见。

### 2.6 结束与清理

```cpp
tfp->close();
top->final();
delete tfp;
delete top;
```

### 2.7 完整 Testbench 示例

```cpp
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vdut.h"

static vluint64_t sim_time = 0;

static void tick(Vdut* t, VerilatedVcdC* v) {
    t->clk = 0; t->eval(); v->dump(sim_time++);
    t->clk = 1; t->eval(); v->dump(sim_time++);
    t->clk = 0; t->eval(); v->dump(sim_time++);
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Verilated::traceEverOn(true);

    Vdut* t = new Vdut;
    VerilatedVcdC* v = new VerilatedVcdC;
    t->trace(v, 1);
    v->open("wave.vcd");

    // ── 上电 + 复位 ──
    t->rst = 1; t->eval(); v->dump(sim_time++);
    tick(t, v); tick(t, v);
    assert(t->reg_q == 0);                    // 复位态检查

    // ── 释放复位 ──
    t->rst = 0; t->eval(); v->dump(sim_time++);

    // ── 组合路径检查 ──
    t->in_a = 0; t->in_b = 1; t->eval(); v->dump(sim_time++);
    assert(t->out_f == (t->in_a ^ t->in_b));

    // ── 准备让时序路径在下个 posedge 采样 ──
    t->in_a = 1; t->in_b = 1;                // 驱动次态
    tick(t, v);                               // 经过一个上升沿
    assert(t->reg_q == EXPECTED_VALUE);       // 时序路径检查

    // ── 多拍运行 ──
    for (int i = 0; i < 10; ++i) tick(t, v);

    // ── 清理 ──
    v->close(); t->final(); delete v; delete t;
    return 0;
}
```

---

## 三、设计原则速记

| 流程 | 口诀 |
|------|------|
| **组合逻辑** | 改 → `eval` → 读/断言 → `dump` |
| **时序逻辑** | 准备输入 → `tick`（采样）→ 读/断言 → 继续 `tick` |
| **关键边沿** | 复位覆盖 posedge，首拍重点验；慢路径要能加速 |
| **全程留痕** | 三相位 dump，问题可视化；断言即规格 |

---

## 四、Yosys 命令行参数速查

### 4.1 操作选项

| 参数 | 说明 |
|------|------|
| `-b, --backend <backend>` | 使用 `<backend>` 作为输出文件的后端 |
| `-f, --frontend <frontend>` | 使用 `<frontend>` 作为输入文件的前端 |
| `-s, --scriptfile <file>` | 执行脚本文件中的命令 |
| `-c, --tcl-scriptfile <file>` | 执行 TCL 脚本文件中的命令 |
| `-C, --tcl-interactive` | 进入 TCL 交互式 shell 模式 |
| `-p, --commands <cmds>` | 执行命令（多条用 `; ` 分隔：`cmd1; cmd2`） |
| `-r, --top <top>` | 详细说明指定的 HDL `<top>` 模块 |
| `-m, --plugin <plugin>` | 加载指定的 plugin 模块 |
| `-D, --define <def>[=<val>]` | 设置 Verilog define 值 |
| `-S, --synth` | 调用 `synth` 命令的快捷方式，将 Verilog 输入转换为门级网表。例如：`yosys -o output.blif -S input.v` |
| `-H` | 打印命令列表 |
| `-h, --help [<cmd>]` | 打印帮助消息，若指定 `<cmd>` 则打印该命令帮助 |
| `-V, --version` | 打印版本信息并退出 |

### 4.2 记录选项

| 参数 | 说明 |
|------|------|
| `-Q` | 禁止打印横幅（版权、免责声明、版本） |
| `-T` | 禁止打印页脚（日志哈希、版本、计时统计） |
| `--no-version` | 抑制在除 `-V`/`--version` 之外的任何地方输出版本 |
| `-q, --quiet` | 安静运行，仅输出警告和错误。用两次也消除警告 |
| `-v, --verbose <level>` | 将日志头打印到控制台（level 级别） |
| `-t, --timestamp` | 用时间戳注释所有日志消息 |
| `-d, --detailed-timing` | 退出时打印更详细的计时统计信息 |
| `-l, --logfile <file>` | 将日志消息写入文件 |
| `-L, --line-buffered-logfile <file>` | 类似 `-l`，但以行缓冲模式打开 |
| `-o, --outfile <file>` | 退出时将设计写入文件 |
| `-P, --dump-design <header_id>[:<file>]` | 在打印指定日志头时转储设计。`"ALL"` = 每个头都转储 |
| `-W, --warning-as-warning <regex>` | 匹配 `<regex>` 的日志消息打印为警告 |
| `-w, --warning-as-message <regex>` | 匹配 `<regex>` 的警告消息打印为常规消息 |
| `-e, --warning-as-error <regex>` | 匹配 `<regex>` 的警告消息打印为错误 |
| `-E, --deps-file <file>` | 写入 Makefile 依赖文件 |

### 4.3 开发者选项

| 参数 | 说明 |
|------|------|
| `-X, --trace` | 启用核心数据结构更改跟踪（调试用） |
| `-M, --randomize-pointers` | 随机化分配的指针地址（调试用） |
| `--autoidx <idx>` | 从 `<seed>` 开始计算 autoidx |
| `--hash-seed <seed>` | 将哈希值与 `<seed>` 混合（极端优化和测试用） |
| `-A, --abort` | 脚本末尾调用 `abort()`（调试用） |
| `-x, --experimental <feature>` | 不打印实验性功能的警告 |
| `-g, --debug` | 全局启用调试日志消息 |
| `--perffile <perffile>` | 将 JSON 性能日志写入 `<perffile>` |
