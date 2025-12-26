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
        - [Makefile 与运行时环境变量交互](#makefile-env)
        - [表达式生成器与验证（从 0 到 1）](#gen-expr)
  - [C. 监视点 (watchpoint.c)](#c-监视点-watchpointc)
  - [D. 寄存器访问 (isa.h / reg.c)](#d-寄存器访问-isah--regc)

---

# NEMU 项目设计与架构分析

*   **Monitor (监控器)**: 相当于模拟器的“外壳”或“操作系统”。它负责初始化、加载程序、提供用户交互界面（SDB）。
*   **CPU (中央处理器)**: 负责取指、译码、执行。这是模拟器的核心。
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

<a id="makefile-env"></a>
#### Makefile 与运行时环境变量交互

为方便批量生成测试样本，`tools/gen-expr/Makefile` 会在运行 `gen-expr` 可执行文件时，通过环境变量把配置信息传递给程序，程序端通过标准库函数 `getenv()` 读取这些值并据此调整行为。常见变量：

- `GEN_N`：生成条数（整数）。`gen-expr.c` 中通过 `getenv("GEN_N")` 读取字符串并用 `strtol()` 解析为整数；若不存在或解析失败则使用默认值（示例中为 `1`）。
- `GEN_OUT`：输出文件路径（字符串），程序调用 `getenv("GEN_OUT")`；若未设置则默认写入当前工作目录下的 `input` 文件。

Makefile 端示例（`tools/gen-expr/Makefile` 的 `run` 目标）：

```makefile
run: $(OUT)
	@GEN_N=$(gen_n) GEN_OUT=$(abspath $(CURDIR)/../..)/input \
	$(OUT)
```

使用说明与常见误解：

- 在 Makefile 中，覆盖变量要使用 Makefile 定义的名称（示例中为 `gen_n`），例如：`make -C tools/gen-expr run gen_n=100`。直接使用 `make run n=100` 不会生效，除非 Makefile 对 `n` 做了映射。
- 程序通常把前若干条（如前 10 条）打印到 `stderr` 作为预览，但 `GEN_OUT` 文件中会包含完整的 `GEN_N` 条目。

下面列出程序常用的环境相关 C 标准库函数及简要用法，方便理解 Makefile ↔ 程序的交互实现。

##### C 库：环境相关函数（常用）

- `char *getenv(const char *name);`
   - 功能：读取环境变量 `name` 的值，返回指向值字符串的指针；若变量不存在返回 `NULL`。
   - 注意：返回的指针指向进程环境区的内存，不应由调用者释放；该内存可被后续 `setenv`/`putenv` 修改。
   - 示例：
      ```c
      const char *v = getenv("GEN_N");
      if (v) {
         char *end = NULL;
         long n = strtol(v, &end, 10);
         if (end != v && *end == '\0' && n > 0) gen_n = (int)n;
      }
      ```

- `int setenv(const char *name, const char *value, int overwrite);`
   - 功能：在环境中设置或修改 `name` 的值为 `value`。
   - 参数：`overwrite` 为非零时允许覆盖已存在的变量；返回 `0` 成功，返回 `-1` 出错并设置 `errno`。
   - 示例：`setenv("GEN_OUT", "/path/to/input", 1);`

- `int putenv(char *string);`
   - 功能：将形如 `"NAME=VALUE"` 的字符串直接加入环境；环境保留对该字符串的指针（实现可能不复制），因此传入的字符串不能在之后被释放或修改。
   - 返回 `0` 成功，`-1` 出错。
   - 注意：`putenv` 与 `setenv` 行为不同，使用时需注意内存所有权。

- `int unsetenv(const char *name);`
   - 功能：从环境中删除变量 `name`。返回 `0` 成功，`-1` 出错并设置 `errno`。

- `long strtol(const char *nptr, char **endptr, int base);`
   - 功能：把字符串 `nptr` 按 `base`（通常为 10）解析为 `long`。`endptr` 指向首个不能转换的字符（可为 `NULL`）。用于把 `getenv` 得到的字符串解析为整数。

注意事项：

- 多线程与环境变量：对环境的修改（`setenv`/`putenv`/`unsetenv`）通常不是线程安全的；读取（`getenv`）在 POSIX 系统上通常是线程安全的，但仍建议在线程间谨慎使用环境修改。
- 程序应对 `getenv` 返回值做严格校验（空指针、非数字字符等），以免产生解析错误。

该独立小节与 `#### 表达式生成器与验证（从 0 到 1）` 并列，方便开发者既能看到生成器实现细节，也能快速理解 Makefile ↔ 程序的运行时交互契约。

<a id="gen-expr"></a>

#### 表达式生成器与验证（从 0 到 1）

##### 概要

本工具目标是随机、安全地生成单行算术表达式（包含十进制/十六进制常数与寄存器 `$name`），将每条表达式写入文件 `input`，用于批量测试和调试 `expr()` 模块。

设计原则：
- 安全写入（统一边界检查）；
- 可控随机性（权重池参数化）；
- 局部生成并验证后提交（降低全局污染与失败影响）；
- 将复杂的运行时语义校验交由表达式求值器处理，只在生成端尽量降低明显错误率（例如直接生成 `$0`、明显的文本除零等）。

##### 表达式生成示例步骤逻辑

下面按代码执行顺序、函数调用与缓冲写入时序，逐步还原一次具体生成过程。假设随机结果恰好构成表达式：

```
$s3 * 455 + $a0 + (957 * (15 + 342 * 382))
```

说明中会指出每一步调用的函数、写入到缓冲区的文本，以及 `s.ptr`（写指针）和 `s.rem`（剩余空间）如何变化（`s` 为 `struct buf_state`）。

1. 初始准备
   - 调用点：`gen_rand_expr()`。
   - 本地缓冲：`char tmp[4096];`。
   - 初始化缓冲状态：`struct buf_state s = {.ptr = tmp, .rem = (int)sizeof(tmp)};` 并调用 `gen_expr_rec(&s, cfg.max_depth)`。

2. 生成第 1 个原子 `$s3`
   - 入口：`gen_expr_rec` 决定当前层第一个原子使用 `gen_operand(s)`。
   - 内部：`gen_operand` 通过 `pool_pick(&op_pool)` 选中 `reg`，调用 `append_reg_from_white(s)`。
   - `append_reg_from_white` 随机从 `regs_name[]` 选到 `"s3"`，执行 `append_fmt(s, "$%s", r)`。
   - 写入行为：`snprintf(s.ptr, s.rem, "$s3")` 写入 3 字节；随后 `s.ptr += 3; s.rem -= 3`，写指针移到下一个可写位置。

3. 写入操作符并生成第 2 个原子 ` * 455`
   - `gen_expr_rec` 在后续循环中调用 `pool_pick(&al_pool)` 选取操作符 `*`，执行 `append_fmt(s, " %s ", op)`，写入字符串 `" * "`（含空格）。
   - 随后调用 `gen_operand` 生成右侧原子：`pool_pick(&op_pool)` 选中 `dec`，执行 `append_fmt(s, "%d", 455)` 写入 `"455"`。
   - 每次 `append_*` 写入都会读取 `n = snprintf(...)` 的返回值，若 `n < s.rem` 则更新 `s.ptr += n; s.rem -= n`。

4. 写第二个操作符并生成第 3 个原子 ` + $a0`
   - 同上：写入 `" + "`，然后 `gen_operand` 生成寄存器原子并写入 `"$a0"`（通过 `append_reg_from_white`）。

5. 写第三个操作符并决定使用子表达式：` + (`
   - 写入 `" + "` 后，`gen_expr_rec` 在此处对第 4 个原子执行 15% 的“括号子表达式”分支判断。
   - 假设随机命中：先执行 `append_str(s, "(")` 写入左括号，然后递归调用 `gen_expr_rec(s, depth-1)` 以生成括号内的子表达式 `957 * (15 + 342 * 382)`。

6. 生成子表达式（深度 = 2）：`957 * (15 + 342 * 382)`
   - 进入新的 `gen_expr_rec(s, depth=2)`，假设本层 `atoms = 2`。
   - 第一个原子：调用 `gen_operand` 写入 `"957"`（`dec`）。
   - 写入操作符 `" * "`。
   - 第二个原子触发括号分支：写入 `"("` 并递归 `gen_expr_rec(s, depth=1)` 去生成内层表达式 `15 + 342 * 382`。

7. 内层子表达式（深度 = 1）：`15 + 342 * 382`
   - 进入 `gen_expr_rec(s, depth=1)`，假设 `atoms = 3`。
   - 依次写入原子与操作符：`15`，` + `，`342`，` * `，`382`，每次写入后更新 `s.ptr` 和 `s.rem`（或在函数内部以 `s->ptr`/`s->rem` 访问）。
   - 内层完成后写入闭括号 `")"`，返回上层；上层再写入自己的闭括号 `")"`，返回顶层。

8. 结束与验证
   - 顶层 `gen_expr_rec` 返回后，`gen_rand_expr` 检查 `tmp`：若 `rem <= 0` 或 `tmp[0] == '\0'` 则视为失败并重试；否则再做括号平衡检查（遍历字符计数 `(`/`)`）。
   - 若通过验证，则 `snprintf(buf, sizeof(buf), "%s", tmp)` 把结果复制到全局 `buf` 并返回；最终 `main()` 将其写入文件 `input`。

9. 写入安全性要点
   - 所有写操作都通过 `append_str` / `append_fmt` 完成，这两个函数用 `snprintf`/`vsnprintf` 返回写入长度 `n`，并在成功时更新缓冲状态（例如 `s.ptr` / `s.rem` 或 `s->ptr` / `s->rem`），在截断/越界时把 `s.rem`（或 `s->rem`）置为 `0` 作为失败标志，阻止后续写入并触发重试。

该小节旨在把源码函数（`gen_expr_rec`、`gen_operand`、`append_*`）与缓冲区写入时序、递归调用关系一一对应，便于对生成器运行时行为的理解与调试。

##### 关键数据结构与约定

- `complexity_t cfg`：控制 `max_depth`（递归深度）、`max_atoms`（单层原子数）、`max_length`（最大字符串长度）。
- `weight_item_t` / `weight_pool_t`：表示加权项与权重池，`pool_prepare()` 计算 `total`，`pool_pick()` 基于权重抽样。
- `regs_name[]`：寄存器白名单，用于随机生成寄存器原子。

缓冲写入约定（不变式）：调用方以 `struct buf_state *s` 传递缓冲状态（包含写指针 `ptr` 与剩余字节 `rem`），所有字符串写入均通过 `append_str` / `append_fmt` 进行；这两个辅助函数在成功写入时更新 `s->ptr` 与 `s->rem`，在溢出或错误时把 `s->rem = 0`，作为失败信号向上传播。

##### 辅助函数（职责与实现要点）

- `append_str(struct buf_state *s, const char *c)`：使用 `snprintf` 写入常数字符串并更新 `s->ptr` / `s->rem`；写入在截断时把 `s->rem = 0`。
- `append_fmt(struct buf_state *s, const char *fmt, ...)`：基于 `vsnprintf` 的格式化写入封装；同样通过 `s->rem` 传播失败。
- `append_reg_from_white(struct buf_state *s)`：从 `regs_name` 随机选取寄存器名并以 `$name` 格式写入；实现中对输入数组中可能含 `$` 前缀做了统一处理以保证输出格式一致。

这些函数把缓冲边界管理集中，简化了递归生成函数的错误处理逻辑。

##### 核心生成流程（函数分解）

1. `pool_prepare(weight_pool_t *p)`：计算权重和 `p->total`，保证 `rand() % total` 有效。

2. `pool_pick(weight_pool_t *p)`：基于 `p->total` 做线性加权抽样，返回选中项下标。

3. `gen_operand(buffer_t *buf, int depth)`：生成单个原子（operand）。
   - 从 `op_pool` 抽样得到 `dec` / `hex` / `reg`；
   - `dec`：写入十进制常数；`hex`：写入 `0x` 十六进制常数；`reg`：调用 `append_reg_from_white` 输出 `$name`。

4. `gen_expr_rec(buffer_t *buf, int depth)`：递归构造表达式片段（核心构造器）。
   - 决定当前层 `atoms = 1 + rand() % cfg.max_atoms`；
   - 为第一个原子 15% 概率生成带括号的子表达式（写 `(`、递归、写 `)`），否则调用 `gen_operand`；
   - 对后续每个原子：选运算符（`al_pool`），写入 ` " %s "`，根据概率或运算符类型决定生成括号子表达式或调用 `gen_operand`；
   - 所有写入前后检查 `buf->rem`，若 `buf->rem == 0` 直接返回，交由上层重试逻辑处理。

5. `gen_rand_expr()`：顶层生成与验证。
   - 在局部 `tmp[4096]` 上尝试若干次（当前实现 5 次）：
   - 初始化缓冲状态：`struct buf_state s = {.ptr = tmp, .rem = (int)sizeof(tmp)}` 并调用 `gen_expr_rec(&s, cfg.max_depth)`；
     - 若 `rem <= 0 || tmp[0] == '\0'`，视为失败并重试；
     - 对 `tmp` 做简单语法校验（括号平衡）：遍历字符计数 `(` / `)`，遇不匹配则重试；
     - 成功则 `snprintf(buf, sizeof(buf), "%s", tmp)` 把结果复制到全局 `buf` 并返回；
   - 若所有尝试均失败，写入保底表达式 `"1+1"`。


##### 从 `main()` 出发的顺序逻辑（调用链与职责划分）

1. `main()` 初始化：
   - `srand(time(0))` 设置随机种子（可改为接受外部种子以便复现）；
   - 调用 `pool_prepare(&op_pool)` 與 `pool_prepare(&al_pool)` 计算权重和。

2. 解析参数并打开输出文件：从环境变量 `GEN_N`（通过 `getenv`）读取生成条数，使用 `strtol` 解析为正整数，若解析失败或未设置则默认 `gen_n = 1`；随后打开 `input` 用以写入。

3. 逐行生成与写入：循环 `i = 0 .. loop-1`：
   - 调用 `gen_rand_expr()`（返回结果保存在全局 `buf`）；
   - 用 `fprintf(out, "%s\n", buf)` 写入文件；并在前若干条把结果 `fprintf(stderr, ...)` 打印以便观察。

4. 结束并清理：关闭文件返回。

`main()` 只做配置、调用和消费（写入/打印），具体生成逻辑由 `gen_rand_expr` 与其下游函数承担；这种职责分离使得生成器易于单元测试与独立运行。


##### 结论

- 总体架构清晰：`main()` 负责配置与输出，`gen_rand_expr()` 管理重试与验证，`gen_expr_rec()` 做递归构造，`append_*` 负责写入安全性，`pool_*` 提供随机抽样基础。
- 通过局部生成-校验-提交的模式以及统一的写入契约，生成器在保证安全的同时便于扩展和调优。


### C. 监视点 (`watchpoint.c`)
**监视点（Watchpoint）实现笔记**

本文档目标：以可实现、可测试的方式概述 `watchpoint.c` 的设计与实现要点，包含数据结构、核心接口、执行流程、错误处理与与 CPU/表达式求值模块的集成点。

概览：
- 监视点以固定大小池（`wp_pool[NR_WP]`）预分配，使用两个链表管理：`free_list`（空闲）与 `used_list`（已启用）。
- 每个监视点保存：编号 `NO`、表达式字符串 `expr[]`、上次计算值 `prev_value`、链表指针 `next` 等。

1) 数据结构（建议）

```c
// watchpoint.h (示例)
#define NR_WP 32
typedef struct WP {
   int NO;
   char expr[128];
   word_t prev_value; // 与 cpu 相关的 word_t（uint32/64）
   struct WP *next;
} WP;

typedef struct { WP *head, *tail; int size; } wp_list_t;

extern WP wp_pool[NR_WP];
extern wp_list_t free_list, used_list;
```

2) 核心接口（必实现函数）

- `void init_wp_pool(void);`
   - 初始化 `wp_pool`，把所有节点加入 `free_list`，清空 `used_list`。

- `WP* new_wp(const char *expr, bool *success);`
   - 从 `free_list` 取节点；复制 `expr`（截断保护）；调用 `expr(expr, &ok)` 计算当前值；若 `ok==false` 或池空则回滚并返回 `NULL`（或通过 `success` 报错）；否则将节点加入 `used_list` 并返回指针。

- `void free_wp(int no);`
   - 在 `used_list` 中按 `NO` 查找并移除，清理后追加回 `free_list`。

- `bool check_watchpoints(void);`
   - 遍历 `used_list`，对每个 `wp` 调用 `expr(wp->expr, &ok)` 得到 `val`；若 `ok==false` 则记录/跳过该监视点（或按策略决定）；若 `val != wp->prev_value` 则打印变更信息并更新 `prev_value`，函数返回 `true`（表示触发，需要暂停 CPU）。否则返回 `false`。

- `void info_wp(void);`
   - 列出 `used_list` 中所有监视点 `NO`、`expr`、`prev_value`，供 `info w` 命令使用。

3) 创建监视点的详细流程（`new_wp`）

- 检查 `free_list.head` 是否等于NULL，若无空闲节点返回错误给用户。
- 取第一个空闲节点 `p = free_list.head`，调整 `free_list.head`/`tail`/`size`。
- 使用 `strncpy(p->expr, expr, sizeof p->expr - 1)` 并确保以 `\0` 终止。
- 调用 `expr(p->expr, &ok)`：
   - 若 `ok == false`：将 `p` 放回 `free_list`（恢复），并把 `*success = false` 返回；
   - 否则 `p->prev_value = val`，把 `p` 插入到 `used_list`（维护 head/tail/size），设置 `*success = true` 并返回 `p`。

4) 删除监视点（`free_wp` / `free_wp(int no)`）

- 在线性遍历 `used_list` 查找 `NO == no`（记录前驱节点以便移除）。
- 移除时注意处理删除头节点、尾节点以及唯一节点的边界情况。
- 释放后把节点追加到 `free_list`（清空 `expr` 与 `prev_value` 可选）。

5) 检测流程与集成点（`check_watchpoints`）

- 集成点：建议在 `cpu_exec` 的主循环中每条指令后或每 N 条指令后调用 `check_watchpoints()`；当其返回 `true` 时暂停执行、切换回 SDB 主循环。
- 检测实现注意：表达式求值函数 `expr()` 的语义是无副作用的（仅读取 CPU 状态/内存），并以 `bool success` 报告计算是否成功。
- 实现细节：
   - 对每个 `wp`：调用 `val = expr(wp->expr, &ok)`；若 `ok==false` 则打印 `Bad expression` 或在调试输出中标记该 watchpoint（按策略）；
   - 若 `val != wp->prev_value`：打印触发信息，例如：

```
Watchpoint %d triggered: %s
   old value = 0x%lx
   new value = 0x%lx
```

   - 更新 `wp->prev_value = val`；记录至少一个触发则返回 `true`。

6) 错误处理与边界条件

- 池耗尽：用户应收到清晰错误信息（例如："No free watchpoint"）。
- 表达式求值失败：创建时拒绝并告知用户；检测时打印警告并跳过该监视点的触发判断（或按配置决定是否删除）。
- 链表维护：在增删时须同时更新 `head`/`tail`/`size`，避免悬挂指针。

7) 性能与策略建议

- 频率控制：若每次指令后都评估所有监视点会较慢，可提供按需检查（如仅在单步模式、断点附近或每 N 条指令检查）。
- 表达式缓存：对于复杂表达式可考虑缓存解析结果（token 列表）以减少重复词法分析费用，但需权衡内存与实现复杂度。

8) 与 SDB 命令的映射示例

- `w <expr>`：调用 `new_wp(expr, &success)` 并在成功时打印分配的 `NO`。
- `d <no>`：调用 `free_wp(no)`，并打印结果。
- `info w`：调用 `info_wp()` 列出当前监视点。

9) 简单示例（伪代码）

```c
WP *new_wp(const char *expr, bool *success) {
   if (free_list.size == 0) { *success = false; return NULL; }
   WP *p = pop_free();
   strncpy(p->expr, expr, sizeof p->expr - 1);
   p->expr[sizeof p->expr - 1] = '\0';
   bool ok = true;
   word_t v = expr_eval(p->expr, &ok);
   if (!ok) { push_free(p); *success = false; return NULL; }
   p->prev_value = v;
   push_used(p);
   *success = true;
   return p;
}
```

10) 测试建议

- 单元测试：模拟 `expr()` 返回已知值的场景，测试 `new_wp`/`free_wp`/`check_watchpoints` 的链表维护与边界条件。
- 集成测试：在解释器中设置一个监视点，运行若干条指令，验证当寄存器/内存变化时能触发并暂停。

小结：按照上文接口与流程实现 `watchpoint.c` 中的增删查改与定期检测，并在 `cpu_exec` 合适位置调用 `check_watchpoints()`，即可得到一个稳健且易于调试的监视点功能。


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
