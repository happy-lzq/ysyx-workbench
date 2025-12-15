# NEMU 项目设计与架构分析

## 目录

- [1. 整体架构概览](#1-整体架构概览)
- [2. 代码目录结构与功能](#2-代码目录结构与功能)
- [3. 运行流程分析 (Execution Flow)](#3-运行流程分析-execution-flow)
- [4. 简易调试器 (SDB) 开发准备](#4-简易调试器-sdb-开发准备)
  - [A. 命令解析框架 (sdb.c)](#a-命令解析框架-sdbc)
   - [B. 表达式求值 (expr.c)](#b-表达式求值-exprc)
      - [词法分析与递归求值详细处理过程](#词法分析与递归求值详细处理过程)
      - [表达式求值与 SDB 集成中的 bool success 设计总结](#表达式求值与-sdb-集成中的-bool-success-设计总结)
      - [表达式生成器与验证（从 0 到 1）](#gen-expr)
  - [C. 监视点 (watchpoint.c)](#c-监视点-watchpointc)
  - [D. 寄存器访问 (isa.h / reg.c)](#d-寄存器访问-isah--regc)

---

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

### A. 命令解析框架 (`sdb.c`)

**实现思路：**

1. **命令表驱动设计**  
   - 所有命令在 `cmd_table[]` 注册，包含命令名、描述、处理函数指针。
   - 主循环读取用户输入，分割为命令和参数，遍历 `cmd_table` 查找并调用对应处理函数。

2. **命令处理函数实现**  
   - 每个命令对应 `cmd_xxx(char *args)` 实现，负责参数解析和功能调用。
   - 例如：
     - `si`：解析步数参数，调用 `cpu_exec(n)`。
     - `info`：判断参数 `r` 或 `w`，分别调用寄存器显示或监视点显示。
     - `x`：解析显示数量和起始地址，循环调用内存读接口并格式化输出。
     - `p`：解析表达式字符串，调用表达式求值模块，输出结果。

3. **扩展性**  
   - 新命令只需在 `cmd_table` 注册并实现对应处理函数，易于维护和扩展。

---

### B. 表达式求值 (`expr.c`)

**词法分析与递归求值详细处理过程：**

1. **词法分析阶段**  
   - 输入表达式字符串，经过词法分析，所有匹配的 token 按顺序写入 `tokens` 数组。
   - 有效 token 的数量由 `nr_token` 记录，分析区间为 `l ~ r`（通常初始为 `0 ~ nr_token-1`）。
   - 每个 token 记录类型和内容，特殊类型（如空格）会被跳过，不记录。

2. **递归求值阶段：区间检查**  
   - 首先检查区间关系：
     - `l > r`：区间非法，表达式错误。
     - `l == r`：仅有一个 token，直接输出结果（如数字、寄存器）。
     - `l < r`：正常关系，进入下一步。

3. **递归求值阶段：括号处理**  
   - 检查 `tokens[l]` 和 `tokens[r]` 是否为一对括号，并且整个区间内括号配对正确。
   - 如果是，递归处理括号内部 `eval(l+1, r-1)`。
   - 如果括号不合法，表达式错误。

4. **递归求值阶段：主运算符查找与拆分**  
   - 在 `l ~ r` 区间内，找到优先级最低且未被括号包裹的主运算符（如加减）。
   - 记下主运算符的位置 `op`。
   - 用主运算符将表达式划分为左右两个子表达式区间。

5. **递归处理与结果合并**  
   - 递归计算左区间 `eval(l, op-1)` 和右区间 `eval(op+1, r)`。
   - 用主运算符将左右结果合并，得到当前区间的值。
   - 递归会不断拆分表达式，直到每个子表达式只剩一个 token，最终合并所有结果，得到整个表达式的值。

---

#### 表达式求值与 SDB 集成中的 `bool success` 设计总结：

1. **success 标志的作用**  
   - 在表达式求值过程中，`bool *success` 用于递归传递和记录表达式分析与计算是否成功。
   - 词法分析阶段（`make_token`）遇到非法字符或无法匹配时，直接返回 `false`，并在 `expr` 中设置 `*success = false`。
   - 递归求值阶段（`eval`）遇到语法错误、括号不匹配、未知 token 时，也会设置 `*success = false` 并返回 0。

2. **与 SDB 命令集成**  
   - 在 SDB 的 `cmd_p` 命令处理函数中，调用 `expr(args, &success)`，通过 `success` 判断表达式求值是否有效。
   - 只有 `success == true` 时，才输出表达式的计算结果；否则输出 "Bad expression"。
   - 这样保证了表达式求值的健壮性和用户体验，错误信息能及时反馈给用户。

3. **递归传递机制**  
   - `success` 指针在 `eval` 递归过程中始终传递，任何一层递归出错都能影响最终结果。
   - 调用如 `isa_reg_str2val(tokens[l].str, success)` 时，能正确传递和设置寄存器解析的成功状态。

4. **整体流程**  
   - 用户输入表达式，SDB 调用 `expr`，先词法分析，再递归求值，整个过程中 `success` 标志始终传递和维护。
   - 只有所有阶段都成功，`success` 才为 `true`，最终输出正确结果；否则任何阶段出错都能及时终止并反馈错误。

**总结：**  
`bool success` 是表达式求值模块与 SDB 命令行集成的关键标志，保证了表达式分析、计算和错误处理的完整性与一致性。  
通过递归传递和集中判断，实现了高效、健壮的表达式求值与用户交互逻辑。

<a id="gen-expr"></a>
#### 表达式生成器与验证（从 0 到 1）

**目标与概述**
- 目标：实现一个能够随机生成合法算术表达式的独立工具，输出格式为每行一个表达式，可直接作为 NEMU 根目录下 `input` 文件用于批量测试 `expr()`。
- 总体流程：定义复杂度与权重 → 安全字符串追加工具 → 生成原子（十进制/十六进制/寄存器）与运算符 → 递归构造表达式 → 简单语法校验（括号平衡）→ 写文件输出 → （可选）对表达式进行 sanitize 以用于 C 嵌入验证。

**设计与数据结构**
- `complexity_t`：控制生成表达式的规模与复杂度，字段通常包括：
   - `max_depth`：递归深度上限，防止无限递归或过深嵌套。
   - `max_atoms`：单层操作数（原子）最大个数，用于控制横向复杂度。
   - `max_length`：表达式字符串最大长度，避免缓冲区溢出。

- `weight_item_t` / `weight_pool_t`：用于参数化概率选择（例如选择十进制/十六进制/寄存器、选择运算符 + - * /）。
   - `weight_item_t`：{ const char *name; int weight; } 表示单项与其相对权重。
   - `weight_pool_t`：{ weight_item_t *items; int n; int total; }，`total` 为权重和，`pool_pick` 使用 `rand() % total` 抽样。

- `regs_name[]`：寄存器白名单数组，生成寄存器原子时从中挑选。

**辅助函数（安全与字符串构造）**
- `append_str(char **pp, int *rem, const char *s)`：安全追加字符串。检查剩余空间 `*rem`，使用 `snprintf` 写入并更新指针与剩余长度；若写入会越界，则把 `*rem=0` 表示失败/截断。
- `append_fmt(char **pp, int *rem, const char *fmt, ...)`：格式化追加（`vsnprintf`），与 `append_str` 同样处理边界与返回值。
- `append_reg_from_white(char **pp, int *rem)`：从 `regs_name` 随机挑选合法寄存器并以 `$name` 格式追加。验证名字字符合法性（首字符为字母或数字且后续为字母数字）。

这些辅助函数的目的：统一缓冲区边界检查，避免手工 strcat 引发缓冲溢出，并在生成流程中可简单中断或回退。

**核心生成函数（逐个详述）**
- `pool_prepare(weight_pool_t *p)`：遍历 `p->items` 累加权重并写入 `p->total`。保证 `p->total >= 1`，避免 `rand() % 0`。在程序初始化时调用一次。

- `pool_pick(weight_pool_t *p)`：基于 `p->total` 随机抽样获得下标。实现：`r = rand() % p->total; acc=0; for i: acc += weight[i]; if (r < acc) return i;`。时间复杂度 O(n)，n 小则开销微小。

- `gen_operand(char **pp, int *rem, int depth)`：生成一个原子（operand）。
   - 流程：`idx = pool_pick(&op_pool); kind = op_pool.items[idx].name;` 根据 `kind`:
      - `dec`：`append_fmt(pp, rem, "%d", rand() % 1000);`
      - `hex`：`append_fmt(pp, rem, "0x%X", rand() % 0x10000);`
      - `reg`：`append_reg_from_white(pp, rem);`
   - `depth` 参数保留以便未来根据深度调整分布。

- `gen_operand_nonzero(char **pp, int *rem, int depth)`：生成保证非零的原子（用于 `/` 的 RHS），尝试多次（如 10 次）选择 `dec/hex/reg` 并确保：
   - 十进制/十六进制产生 1..N 的非零常数；
   - 寄存器则避免选择 `$0`；
   - 若尝试失败则退化为 `1`。注意：不能完全避免子表达式在运行时结果为 0，但能显著降低明显的 `/ 0` 文本或 `$0` 的生成。

- `gen_expr_rec(char **pp, int *rem, int depth)`：递归构造表达式片段。
   - 决定 `atoms = 1 + rand() % cfg.max_atoms;`（每层原子数）。
   - 生成第一个原子：若 `depth > 0` 且按概率产生子表达式，则写 `(`、递归 `gen_expr_rec(depth-1)`、写 `)`；否则调用 `gen_operand`。
   - 对后续每个原子：从 `al_pool` 中 pick 运算符并追加 `" %s "`，若运算符是 `/` 则调用 `gen_operand_nonzero`，否则按概率生成子表达式或简单原子。
   - 每次操作前检查 `*rem`；当剩余空间不足时提前返回。

- `gen_rand_expr()`：顶层生成器。
   - 在局部 `tmp[]` 缓冲上尝试若干次（例如 5 次）：
      - 清空 `tmp`，`p=tmp`，`rem=sizeof(tmp)`；调用 `gen_expr_rec(&p,&rem,cfg.max_depth)`。
      - 若 `rem <= 0` 或 `tmp` 为空则视为失败继续尝试。
      - 做简单括号平衡与合法性检查（遍历字符计数 `(` / `)`），若不合法继续尝试。
      - 合法则 `snprintf(buf, sizeof(buf), "%s", tmp); return;`。
   - 若重试结束仍失败则 `snprintf(buf, sizeof(buf), "1+1");` 作为退路。

- `sanitize_for_c(const char *src, char *dst, int dstsz)`：把 `$name` token 替换为安全字面量（如 `1`），用于把表达式嵌入到临时 C 源做编译检查。
   - 遍历 `src`，遇到 `$` 就跳过后续字母数字序列并在 `dst` 写入 `1`；其他字符原样复制，确保 `dst` 不越界。

以上函数协同工作，提供了可控、参数化、并尽量安全的表达式文本生成能力。

**入口与文件输出**
- `main(argc, argv)` 行为：
   - seed 随机数 (`srand(time(0))`)；调用 `pool_prepare`。
   - 读取命令行参数 `argv[1]` 作为生成数量 `loop`。
   - 打开 `input` 文件并循环调用 `gen_rand_expr()`：写原始含 `$reg` 的表达式到文件（`fprintf(out, "%s\n", buf);`）。
   - 打印前 10 条示例用于预览，但注意应输出到 `stderr`（已修复），以免用户将程序的 `stdout` 重定向到 `input` 时把示例也写入文件，导致行数偏大。

**编译与运行示例**
```bash
cd tools/gen-expr
make -j4
./build/gen-expr 10000   # 生成 10000 条并写入 tools/gen-expr/input
wc -l tools/gen-expr/input  # 验证行数
```

注意：不要用 `./build/gen-expr 10000 > input` 这类把 stdout 重定向到文件的方式（已把示例打印改到 stderr，但仍然推荐直接使用工具写入的文件）。

**与 `expr.c` 的验证与协同**
- 词法/求值模块（`src/monitor/sdb/expr.c`）负责把文本 token 化并计算值。生成器应尽量产生语法正确且不易触发运行时错误的表达式，但运行时仍需健壮：
   - `expr.c` 要对 `tokens` 容量做边界检查，防止超长表达式导致写越界（已扩容到 256 并加检查）。
   - `eval` 在除法处应检测除零并通过 `*success=false` 返回错误而不是崩溃（已添加保护）。
   - 生成器在 `/` 的右侧尽量生成非零原子以减少明显的 `/ 0`。

**测试策略与健壮性提升建议**
- 渐进式验证：先生成小样本（100/1000），用 `make_token` 检查词法，然后在 NEMU 中跑前 200 条，观察 `Bad expression` 或 `division by zero` 比例。
- 如果运行时报错率高：降低 `cfg.max_depth` 和 `cfg.max_atoms`，或提高 `gen_operand_nonzero` 的力度（例如对小子树也进行静态估算）。
- 可选增强：在生成器内部做一次轻量“静态评估”或调用内嵌的 eval（使用 `sanitize_for_c` 之后的 C 编译/运行）来过滤掉语义上问题较多的表达式，但代价是性能与实现复杂度上升。

**小结（设计要点回顾）**
- 分层与局部缓冲：在局部 `tmp` 上生成并校验后才拷贝到全局 `buf`，降低对全局状态破坏的风险。
- 参数化权重：通过 `weight_pool` 可轻松调整生成风格（更多寄存器、更多常数、更多除法等）。
- 安全优先：统一使用 `append_fmt`/`append_str` 做缓冲边界管理；在 `expr.c` 中也做解析时的边界检查与除零保护。
- 可扩展性：代码模块化、易于增加新原子类型（函数调用、位运算等）或改进抽样算法。

以上为从 0 到 1 构建表达式生成器与验证流程的完整、分层、可执行的设计与实现要点（与此前讨论内容一致并作技术细节扩展）。

### C. 监视点 (`watchpoint.c`)

**实现过程设计思路：**

1. **监视点池管理**
   - 预分配一组监视点结构体（如数组），通过链表维护空闲和已用监视点。
   - 提供 `new_wp(expr)` 分配新监视点，保存表达式字符串和当前值。
   - 提供 `free_wp(wp)` 释放监视点，回收到空闲链表。

2. **监视点添加与删除**
   - `w expr` 命令解析表达式，调用 `new_wp(expr)` 创建监视点，并初始化当前值。
   - `d N` 命令根据编号查找监视点，调用 `free_wp(wp)` 删除。

3. **监视点检测机制**
   - 在 `cpu_exec` 每次指令执行后，遍历所有已用监视点。
   - 对每个监视点，调用表达式求值模块，获取新值。
   - 若新值与旧值不同，则输出变化信息，暂停 CPU 执行。

4. **信息展示接口**
   - `info w` 命令遍历所有已用监视点，输出编号、表达式和当前值。

5. **与表达式求值模块结合**
   - 监视点的表达式求值依赖 `expr.c`，每次检测都调用表达式求值接口。


---

### D. 寄存器访问 (`isa.h` / `reg.c`)

**实现思路：**

1. **寄存器结构体定义**  
   - `CPU_state` 结构体保存所有寄存器的值。
   - 不同 ISA 可能有不同寄存器布局，统一接口定义在 `isa.h`。

2. **寄存器显示与查询**  
   - `isa_reg_display()`：遍历所有寄存器，格式化输出名称和值。
   - `isa_reg_str2val(name, &success)`：根据寄存器名查找并返回值，支持表达式求值模块调用。

3. **寄存器访问相关接口与说明**

   - 寄存器类型定义：`word_t`，在 `include/common.h` 中定义，实际为 `uint32_t`（RV32）或 `uint64_t`（RV64）。
   - 访问所有寄存器：`isa_reg_display()`，打印所有通用寄存器和 PC 的值，适用于 `info r` 命令。
   - 按名字访问寄存器：`isa_reg_str2val(const char *s, bool *success)`，根据字符串（如 `$ra`、`$sp`）返回对应寄存器的值，适用于表达式求值（如 `p $sp + 4`）。
   - 直接访问结构体成员：`cpu.pc`（程序计数器），`cpu.gpr[i]`（第 i 个通用寄存器），一般用于底层实现。

   > 调试器开发建议：`info r` 直接调用 `isa_reg_display()`，表达式求值时调用 `isa_reg_str2val()`。

4. **与命令结合**  
   - `info r` 命令调用寄存器显示接口。
   - 表达式求值支持 `$eax` 等寄存器名解析。

5. **内存访问与加载相关接口与说明**

    - 虚拟地址类型：`vaddr_t`，在 `include/common.h` 中定义，实际为 `uint32_t`（RV32）或 `uint64_t`（RV64）。
    - 读取虚拟内存：`vaddr_read(vaddr_t addr, int len)`，从虚拟地址 `addr` 读取 `len` 字节数据，返回 `word_t` 类型。适用于调试器 `x` 命令。
    - 写入虚拟内存：`vaddr_write(vaddr_t addr, int len, word_t data)`，向虚拟地址写入数据。
    - 读取物理内存：`paddr_read(paddr_t addr, int len)`，底层硬件模拟用。
    - 指令预取：`vaddr_ifetch(vaddr_t addr, int len)`，用于 CPU 取指阶段。
    
    > 调试器开发建议：SDB 中优先使用 `vaddr_read` 进行内存扫描，地址参数可通过表达式求值获得。
    
---

**总结：**  
各模块分工明确，命令解析负责交互入口，表达式求值和监视点为调试核心功能，寄存器访问和内存访问为底层支持。整体架构易于扩展和维护，便于实现更多调试功能。
