# PA2 学习笔记

<a id="sec-pa2-toc"></a>
## 目录

- [Linux 软链接知识点总结](#sec-softlink)
  - [1. 什么是软链接？](#sec-softlink-01)
  - [2. 核心命令格式](#sec-softlink-02)
  - [3. 与 C 语言指针的类比](#sec-softlink-03)
  - [4. 为什么使用软链接解决编译器前缀问题？](#sec-softlink-04)
  - [5. 软链接查看与修改实操（含 python/python3 案例）](#sec-softlink-05)
- [2026年2月25日 - 2月26日 Debug 记录与总结](#sec-debug-log)
  - [一、基础设施与环境配置问题](#sec-debug-01)
  - [二、指令实现 Bug 与修复记录](#sec-debug-02)
  - [三、核心 Debug 思路与方法论总结](#sec-debug-03)
- [PA2 专题：指令修复、UB行为与运行时生命周期全景梳理](#sec-pa2-fix-lifecycle)
  - [1. 核心架构解析：NEMU 的双重视角内存映射模型](#sec-fixlife-01)
  - [2. AM (Abstract Machine) 解析：TRM 与堆栈初始化](#sec-fixlife-02)
  - [3. 深度总结：从 C 源码到模拟器执行的完整生命周期](#sec-fixlife-03)
- [PA2 专题：Makefile 核心机制深度解析与终端控制流控制反转](#sec-pa2-makefile)
- [PA2 专题：VS Code `c_cpp_properties.json` 配置核心逻辑总结](#sec-pa2-vscode)
  - [1. 问题背景：为什么源码能编过，但编辑器仍然满屏飘红？](#sec-vscode-01)
  - [2. 总体逻辑：`c_cpp_properties.json` 的本质职责是什么？](#sec-vscode-02)
  - [3. 为什么旧版配置会显得臃肿？其设定逻辑的问题在哪里？](#sec-vscode-03)
  - [4. 精简后的新逻辑：为什么现在只剩几项也能正确工作？](#sec-vscode-04)
  - [5. 最终抽象：以后遇到新的红线，应该怎样判断是否需要修改 `json`？](#sec-vscode-05)
  - [6. 这次配置总结背后的方法论](#sec-vscode-06)
  - [7. 本次最终结论（一句话压缩）](#sec-vscode-07)
  - [8. 路径该怎么归类：`includePath`、`browse.path` 与 `forcedInclude` 的完整判断法](#sec-vscode-08)
- [PA2 专题：NEMU 执行客户程序的完整生命周期总总结](#sec-pa2-program-lifecycle)
  - [一、总目标：NEMU 到底在完成什么任务？](#sec-proglife-01)
  - [二、第一层：客户程序从何而来？——源码到镜像的构建链](#sec-proglife-02)
  - [三、第二层：NEMU 怎么拿到这个外部镜像？——命令行与 `parse_args()`](#sec-proglife-03)
  - [四、第三层：系统初始化顺序——`init_monitor()` 为什么这么排](#sec-proglife-04)
  - [五、第四层：`init_isa()`、`restart()`、`load_img()` 之间的关系](#sec-proglife-05)
  - [六、第五层：`RESET_VECTOR` 的语义——为什么它是全局锚点](#sec-proglife-06)
  - [七、第六层：客户机地址如何落到宿主机数组里——`paddr.c` 的核心桥梁作用](#sec-proglife-07)
  - [八、第七层：`load_img()` 如何借助地址映射把镜像落地](#sec-proglife-08)
  - [九、第八层：为什么 `.bin` 本身没有地址，但程序仍然能从 `0x80000000` 运行](#sec-proglife-09)
  - [十、第九层：为什么 `includePath` 与 IntelliSense 配置问题值得纳入生命周期总结](#sec-proglife-10)
  - [十一、第十层：CPU 真正开始执行时发生了什么](#sec-proglife-11)
  - [十二、全流程中的关键函数关系图](#sec-proglife-12)
  - [十三、这两天分析中最关键的“为什么”与“如何解决”](#sec-proglife-13)
  - [十四、最终总收束：NEMU 执行客户程序的完整生命周期一句话版本](#sec-proglife-14)
- [PA2 专题：`mtrace` 的完整设计过程总结（从提问到落地）](#sec-pa2-mtrace)
  - [1. 问题是怎么被提出的？](#sec-mtrace-01)
  - [2. 为什么需要单独设计 `mtrace`，而不是复用现有 `log`？](#sec-mtrace-02)
  - [3. 设计目标是什么？](#sec-mtrace-03)
  - [4. 整体思路：把 `mtrace` 拆成三条链来看](#sec-mtrace-04)
  - [5. 第一步：先解决“要不要编译 `mtrace`”的问题](#sec-mtrace-05)
  - [6. 第二步：为什么 `CONFIG_MTRACE_COND` 还不够，必须再有 `MTRACE_COND`？](#sec-mtrace-06)
  - [7. 第三步：为什么插桩点必须放在 `paddr_read()` / `paddr_write()`？](#sec-mtrace-07)
  - [8. 第四步：输出层怎么设计？](#sec-mtrace-08)
  - [9. 第五步：路径为什么成了整个 `mtrace` 设计里最关键的部分？](#sec-mtrace-09)
  - [10. 第六步：真正应该依赖什么来推导 `mtrace` 的路径？](#sec-mtrace-10)
  - [11. 第七步：路径最终是怎么实现的？](#sec-mtrace-11)
  - [12. 第八步：设计过程中踩到的几个关键坑](#sec-mtrace-12)
  - [13. 第九步：最终验证链是怎样闭环的？](#sec-mtrace-13)
  - [14. 这次 `mtrace` 设计里最核心的“为什么”总结](#sec-mtrace-14)
  - [15. 最终方案一句话收束](#sec-mtrace-15)
  - [16. `mtrace` 路径专题：从 `make ARCH=riscv32-nemu ALL=string run` 到 `mtrace-log.txt` 的完整路径形成链](#sec-mtrace-16)
- [PA2 专题：Ftrace 从 0 到 1 全链路设计笔记](#sec-pa2-ftrace-fullchain)
  - [1. 全局目标与最小闭环](#sec-ftrace-01)
  - [2. 构建与启动参数链路](#sec-ftrace-02)
  - [3. monitor 初始化阶段的 ftrace 生命周期](#sec-ftrace-03)
  - [4. ELF 静态解析主流程](#sec-ftrace-04)
  - [5. 关键结构体关系图](#sec-ftrace-05)
  - [6. 运行期 Hook：为什么是 jal/jalr](#sec-ftrace-06)
  - [7. ftrace_call 与 ftrace_ret 的协作机制](#sec-ftrace-07)
  - [8. 静态 ELF 与软件内存的关系](#sec-ftrace-08)
  - [9. 设计分层与每层职责](#sec-ftrace-09)
  - [10. 端到端时序总结](#sec-ftrace-10)
  - [11. 常见坑与工程性检查点](#sec-ftrace-11)
  - [12. ftrace 出栈/入栈执行逻辑、问题来源与解决全总结](#sec-ftrace-stack)
  - [13. 目标函数匹配问题的引出与逻辑总结](#sec-ftrace-symbol-match)
- [PA2 专题：klib 测试全链路设计、Debug 与闭环验证](#sec-pa2-klib-tests)
  - [1. 为什么需要单独设计 `klib-tests`？](#sec-klibtest-01)
  - [2. 直接测试对象、间接运行平台与最终验证目标](#sec-klibtest-02)
  - [3. 测试分类为什么最终定成 `write/read/format/stdlib`？](#sec-klibtest-03)
  - [4. 项目结构的演化：为什么最后采用 `cpu-tests` 风格？](#sec-klibtest-04)
  - [5. `Makefile` 的核心逻辑：谁负责筛选测试，谁负责真正编译？](#sec-klibtest-05)
  - [6. 测试执行闭环：从 `make ... run` 到 `PASS/FAIL` 的完整路径](#sec-klibtest-06)
  - [7. `ARCH=native` 与 `ARCH=riscv32-nemu` 的本质区别](#sec-klibtest-07)
  - [8. 断言系统的设计：从普通 `check()` 到可定位报错](#sec-klibtest-08)
  - [9. 设计与 Debug 过程中踩到的关键问题、根因与解决](#sec-klibtest-09)
  - [10. 最终结果、验证结论与方法论总结](#sec-klibtest-10)
- [PA2 专题补充](#sec-pa2-supplement)
  - [cpu-tests native 链接报错 `__isoc23_strtol` 排查笔记](#sec-native-build-mismatch)
- [PA2 专题：从传参到串口输出的全链路追踪（`make run mainargs=ysyx`）](#sec-pa2-io-trace)
  - [1. 开发环境追踪记录（插曲问题）](#sec-io-01)
  - [2. 参数注入机制：`mainargs` 怎么传给客户程序？](#sec-io-02)
  - [3. 外设 MMIO 的建立与页对齐机制](#sec-io-03)
  - [4. 路由分发链与 I/O 截获：从 `outb` 到终端打印](#sec-io-04)
- [PA2 专题：时钟计时器与软硬协同全栈生命周期梳理](#sec-pa2-rtc-timer)
  - [1、全局视角：时钟测试在验证什么？](#sec-rtc-01)
  - [2、第一阶段：NEMU（硬件）的启动与外设准备](#sec-rtc-02)
  - [3、第二阶段：AM-tests（软件）的编译、加载与启动](#sec-rtc-03)
  - [4、第三阶段：软硬协同的巅峰——一次读取时间的完整物理全貌](#sec-rtc-04)
  - [5、第四阶段：成果的展示——第二次协同 (串口输出)](#sec-rtc-05)
  - [6、代码层面关键解耦机制总结 (LUT与IOE)](#sec-rtc-06)
  - [7、方法论与全局总结](#sec-rtc-07)


- [PA2 专题：`dtrace` 的完整设计过程总结与踩坑记录](#sec-pa2-dtrace)
  - [1. `dtrace` 设备记录追踪也可以像 `mtrace` 这样设计吗？](#sec-dtrace-01)
  - [2. 为什么 `mtrace`, `itrace`, `ftrace` 都添加了 `depends on TRACE && ...` 是必要条件吗？](#sec-dtrace-02)
  - [3. 为什么一开始给出的 `dtrace` 设计里没有加上这些依赖？](#sec-dtrace-03)
  - [4. `dtrace` 的路径生成逻辑与 `sizeof(ftrace_log_file)` 的踩坑修复](#sec-dtrace-04)
  - [5. 为什么不直接在 `map_read` / `map_write` 里做 `fopen/fclose`，还要搞个全局指针 `dtrace_fp` 与 `init_monitor`？](#sec-dtrace-05)
  - [6. 为什么编译器会报错 `DTRACE_COND` 未定义？("ture" 拼写错误踩坑)](#sec-dtrace-06)
  - [7. 为什么 `dtrace_fp` 一直开着不用 `fclose()` 关闭？](#sec-dtrace-07)
  - [8. Serial 通信里的一个疑问：RTC 测试输出了串口，但在 RTL 里没有？](#sec-dtrace-08)
- [PA2 专题：NEMU 运行 NEMU (NEMU on NEMU) 避坑与总结](#sec-pa2-nemu-on-nemu)
- [PA2 专题：pa2-keyboard 键盘外设、AM 抽象接口与软硬件协同全链路梳理](#sec-pa2-keyboard)
  - [1. 先纠正一个最容易误解的点：AM 不是“再造一套硬件寄存器”](#sec-kbd-01)
  - [2. 所谓“抽象寄存器”的意义，到底该怎么准确理解？](#sec-kbd-02)
  - [3. `io_read()` / `io_write()` 到底是什么？](#sec-kbd-03)
  - [4. 键盘专题的三层模型：软件层、AM 层、硬件层](#sec-kbd-04)
  - [5. 键盘外设启动阶段：设备是怎样被挂到总线上的？](#sec-kbd-05)
  - [6. 宿主机真实按键发生后，事件是如何进入 NEMU 的？](#sec-kbd-06)
  - [7. NEMU 内部：SDL 扫描码怎样变成客户机可理解的键值？](#sec-kbd-07)
  - [8. 应用发起读取：`io_read(AM_INPUT_KEYBRD)` 这一句背后到底发生了什么？](#sec-kbd-08)
  - [9. 真正的硬件交互核心：读 `KBD_ADDR` 时，NEMU 底层发生了什么？](#sec-kbd-09)
  - [10. 最终返回应用：为什么应用看到的是 `keydown/keycode`，而不是一个整数？](#sec-kbd-10)
  - [11. 用一张分层时序图，把键盘这条链路整体串起来](#sec-kbd-11)
  - [12. 最终总结：键盘案例里，软硬件协同到底是什么？](#sec-kbd-12)
- [PA2 专题：pa2-声卡设计，音频流控与并发同步的软硬件双向全栈总结](#sec-pa2-audio)
  - [1. 核心与前提：SDL 音频子系统数据结构与交互规范](#sec-audio-sdl)
  - [2. 架构第一层：应用层 (Application / `am-tests`) —— “宏观水泵”](#sec-audio-layer1)
  - [3. 架构第二层：AM 驱动层 (`__am_audio_play`) —— “前线调度流水线与环形队列”](#sec-audio-layer2)
  - [4. 架构第三层：抽象通信协议层 (MMIO) —— “设备仪表盘”](#sec-audio-layer3)
  - [5. 架构第四层：NEMU 硬件模拟层 (`audio.c`) —— “保护锁与底层水槽”](#sec-audio-layer4)
  - [6. 架构第五层：SDL 宿主消费者 —— “匀速出水口”](#sec-audio-layer5)
  - [7. 设计全景总结回顾](#sec-audio-layer6)
<a id="sec-softlink"></a>
## Linux 软链接 (Symbolic Link) 知识点总结

在解决交叉编译器前缀不匹配的问题时，我们使用了一种非常巧妙的方法：**建立软链接**。这不仅避免了修改项目源码（如 Makefile），还保持了环境的整洁。

<a id="sec-softlink-01"></a>
### 1. 什么是软链接？

软链接（Symbolic Link，也称符号链接）是 Linux/Unix 系统中的一种特殊文件类型。它本身不包含实际的数据，而是包含了一个指向另一个文件或目录的**路径引用**。

<a id="sec-softlink-02"></a>
### 2. 核心命令格式

创建软链接的命令是 `ln` (link)，配合 `-s` (symbolic) 参数：

```bash
ln -s [目标文件/真实文件] [链接文件/快捷方式名]
```

**常用参数组合：**
*   `-s`：创建软链接（如果不加 `-s`，则创建的是硬链接）。
*   `-f`：强制执行 (force)。如果目标链接名已经存在，则覆盖它。

**示例解析：**
```bash
sudo ln -sf "riscv64-unknown-linux-gnu-gcc" "riscv64-linux-gnu-gcc"
```
*   **目标文件**：`riscv64-unknown-linux-gnu-gcc` (真实存在的编译器程序)
*   **链接文件**：`riscv64-linux-gnu-gcc` (我们创建的“快捷方式”)
*   **结果**：当系统或 Makefile 尝试执行 `riscv64-linux-gnu-gcc` 时，实际上会被重定向去执行 `riscv64-unknown-linux-gnu-gcc`。

<a id="sec-softlink-03"></a>
### 3. 与 C 语言指针的类比

理解软链接最直观的方式就是将其类比为 C 语言中的**指针**。

| 概念 | Linux 软链接 | C 语言指针 |
| :--- | :--- | :--- |
| **本质** | 一个特殊的文件，存储的是另一个文件的**路径字符串**。 | 一个变量，存储的是另一个变量的**内存地址**。 |
| **目标** | 真实存在的文件或目录（靶点）。 | 内存中实际存储数据的变量。 |
| **访问机制** | 访问链接文件时，操作系统会自动解析路径，最终操作的是**目标文件**。 | 解引用指针（`*p`）时，CPU 会根据地址访问**目标变量**的内存。 |
| **独立性** | 链接文件有自己的 inode 和权限，但大小很小（仅路径长度）。 | 指针变量有自己的内存地址，大小固定（如 32位系统 4 字节，64位系统 8 字节）。 |
| **失效情况** | 如果删除了目标文件，软链接依然存在，但会变成**死链接 (Dangling Link)**，访问会报错（如 "No such file or directory"）。 | 如果释放了指针指向的内存（如 `free(p)`），指针依然存在，但变成了**悬空指针 (Dangling Pointer)**，访问会导致未定义行为（如段错误）。 |

**代码类比：**

```c
// 1. 真实存在的数据 (目标文件)
int real_compiler = 100; 

// 2. 创建指针指向真实数据 (创建软链接)
// 相当于: ln -s real_compiler shortcut_compiler
int *shortcut_compiler = &real_compiler; 

// 3. 通过指针操作数据 (通过软链接执行程序)
// 相当于执行: ./shortcut_compiler
printf("%d\n", *shortcut_compiler); // 输出 100
```

<a id="sec-softlink-04"></a>
### 4. 为什么使用软链接解决编译器前缀问题？

*   **非侵入性**：不需要修改 Abstract-Machine 的底层 Makefile (`riscv.mk`)。修改底层构建脚本可能会在未来拉取更新时产生冲突。
*   **全局生效**：在 `/opt/riscv64/bin` (或 `/usr/local/bin` 等 PATH 目录) 下建立软链接后，整个系统任何地方调用 `riscv64-linux-gnu-gcc` 都能正确映射，一劳永逸。
*   **批量处理**：结合 Shell 脚本的字符串替换功能（如 `${file/-unknown/}`），可以极高效率地为整个工具链（gcc, g++, objdump, objcopy 等）批量创建别名。

<a id="sec-softlink-05"></a>
### 5. 软链接查看与修改实操（含 python/python3 案例）

这一小节聚焦两个高频动作：

1. **我现在调用到的到底是谁？**
2. **我该怎么安全地改掉它？**

#### 5.1 先看命令解析结果（PATH 层）

```bash
which python
which python3
```

作用：先确认 shell 最终命中的是哪个可执行路径。

你给的案例中：

```text
/usr/bin/python
/usr/bin/python3
```

说明 `python` 和 `python3` 这两个命令名都能命中 `/usr/bin` 下的条目。

#### 5.2 再看它是不是软链接（文件层）

```bash
ls -l /usr/bin/python
```

典型输出：

```text
lrwxrwxrwx 1 root root 16  3月 14 22:58 /usr/bin/python -> /usr/bin/python3
```

解读要点：

* 行首是 `l`：表示这是符号链接（link）。
* `->` 右侧是当前链接目标：这里是 `/usr/bin/python3`。

#### 5.3 查看最终真实落点（链路终点）

```bash
readlink -f /usr/bin/python
```

典型输出：

```text
/usr/bin/python3.10
```

这一步很关键：

* `ls -l` 看到的是“下一跳”目标；
* `readlink -f` 给的是“最终真实文件”（会把多级链接一路展开）。

所以你的案例链路是：

```text
python (命令)
-> /usr/bin/python (软链接)
-> /usr/bin/python3 (可能仍是链接)
-> /usr/bin/python3.10 (最终真实可执行文件)
```

#### 5.4 修改软链接的推荐方式

如果需要把 `python` 改指向另一个目标，建议使用：

```bash
sudo ln -sfn /usr/bin/python3 /usr/bin/python
```

参数说明：

* `-s`：创建符号链接。
* `-f`：目标已存在时强制替换。
* `-n`：把已存在的链接当作普通文件处理，避免把链接当目录进入。

修改后建议立刻复核：

```bash
which python
ls -l /usr/bin/python
readlink -f /usr/bin/python
```

#### 5.5 工程实践中的稳妥原则

1. **先看再改**：先 `which` + `ls -l` + `readlink -f`，确认现状。
2. **尽量改“别名端”**：优先改命令别名，不改工具链真实二进制。
3. **改后立刻验证**：至少验证命令解析路径和最终落点是否一致。
4. **避免误改系统关键链接**：涉及 `/usr/bin` 时必须明确影响范围。

---

<a id="sec-debug-log"></a>
## 2026年2月25日 - 2月26日 Debug 记录与总结

<a id="sec-debug-01"></a>
### 一、基础设施与环境配置问题

#### 1. 指令执行日志 (Log) 提前停止打印
*   **现象**：在使用 `si` 单步调试时，发现执行超过 10 步后，NEMU 不再打印指令的执行日志（Log）。
*   **原因**：NEMU 源码中限制了最大打印指令数，以防止长程序产生过大的日志文件。
*   **解决方案**：修改 `nemu/src/cpu/cpu-exec.c` 中的 `MAX_INST_TO_PRINT` 宏，将其值调大（例如 `1000000`），以便在调试期间能够持续观察指令流。

#### 2. 运行 `hello-str` 触发 `out_of_bound` 内存越界
*   **现象**：运行 `hello-str` 测试用例时，NEMU 报错物理内存越界，访问了地址 `0xa00003f8`。
*   **分析思路**：
    1. 查阅 NEMU 内存映射文档，发现 `0xa00003f8` 属于串口 (Serial Port) 的 MMIO (Memory-Mapped I/O) 地址空间，而不是普通的物理内存 (PMEM)。
    2. 为什么会当成普通内存访问？说明设备的 MMIO 映射没有被启用。
*   **解决方案**：在 `nemu` 目录下运行 `make menuconfig`，进入 `Device` 选项，开启 `Enable hardware devices` (`CONFIG_DEVICE`)。
*   **后续**：开启设备后，程序成功向串口输出，但随后在 `klib/src/stdio.c` 触发了 `Not implemented` 的 panic。这指明了下一步的开发任务：实现 `klib` 中的 `printf` / `vsprintf` 等格式化输出函数。

<a id="sec-debug-02"></a>
### 二、指令实现 Bug 与修复记录

#### 1. 立即数提取 (B/J/S 型) 逻辑错误
*   **现象**：分支跳转或内存访问地址计算错误。
*   **原因**：在编写 `immB`, `immJ`, `immS` 宏时，对指令的各个比特位段**先进行了符号扩展 (`SEXT`)，然后再进行位拼接 (`|`)**。这导致高位被符号位污染，拼接后的结果完全错误。
*   **解决方案**：严格遵循“**先拼接，后扩展**”的原则。先用 `uint32_t` 变量将各个离散的比特位移位并按位或 (`|`) 拼接成一个完整的数值，最后再对这个整体进行一次 `SEXT` 符号扩展。

#### 2. Load/Store 指令的符号扩展失效 (`lb`, `lh`)
*   **现象**：`load-store.c` 测试失败，期望读出的值是 `0xffff8000`，但实际读出的是 `0x00008000`。
*   **原因**：`Mr` (内存读) 宏返回的是无符号数。对于 `lb` (Load Byte) 和 `lh` (Load Halfword)，RISC-V 要求进行符号扩展。如果没有显式地告诉 C 语言这是一个有符号数，C 语言会默认进行零扩展。
*   **解决方案**：利用 C 语言的类型转换机制触发符号扩展。在赋值给寄存器前，先将 `Mr` 的结果强制转换为对应的有符号类型：`(int8_t)Mr(...)` 和 `(int16_t)Mr(...)`。

#### 3. 64位乘法指令高位截断 (`mulh`, `mulhu`, `mulhsu`)
*   **现象**：乘法高 32 位结果不正确。
*   **原因**：在 32 位系统中，两个 32 位数相乘，结果默认还是 32 位，高 32 位会直接溢出丢失。然后再右移 32 位，结果必然是 0 或符号位。
*   **解决方案**：在乘法运算发生前，必须先将操作数强制转换为 64 位类型 (`int64_t` 或 `uint64_t`)，这样 C 语言才会生成 64 位乘法指令。乘法完成后，再 `>> 32` 提取高 32 位。

#### 4. 移位指令的未定义行为 (UB) 与宏匹配覆盖
*   **现象**：`movsx.c` 测试失败，`check(C[1] == 0xffffff80)` 不通过。
*   **Debug 思路 (反向推导法)**：
    1. **确认现象**：`C[1] = 0x80` (即 -128)，提升为 `int` 时应为 `0xffffff80`，但实际不是。说明符号扩展失败。
    2. **查看汇编**：通过 `objdump` 查看 `movsx.c` 的汇编，发现编译器并没有使用 `lb`，而是使用了 `lbu` 加载，然后通过 `slli` (左移 24 位) 和 `srai` (算术右移 24 位) 来手动实现符号扩展。
    3. **锁定嫌疑人**：问题必定出在 `slli` 或 `srai` 的实现上。
    4. **审查代码**：
        *   **Bug A (逻辑覆盖)**：在 `inst.c` 中，R 型指令 (`sll`, `srl`, `sra` 等) 被定义了两次。由于 `INSTPAT` 是从上到下顺序匹配，第一次错误的定义“截胡”了匹配过程，导致后面正确的代码变成了死代码。
        *   **Bug B (未定义行为)**：第一次错误的定义中，移位量直接使用了 `src2`。如果 `src2` 的值大于等于 32，在 C 语言中会导致未定义行为 (UB)。RISC-V 规范要求对移位量取低 5 位 (`src2 & 0x1F`)。
        *   **Bug C (类型规范)**：算术右移需要高位补符号位，依赖于 C 语言的有符号右移。之前使用了 `(signed)`，为了跨平台安全和严谨性，应统一使用 `<stdint.h>` 中的 `(int32_t)`。
*   **解决方案**：删除重复的指令定义，保留带有 `(src2 & 0x1F)` 掩码的版本，并将所有有符号操作的强制类型转换统一规范为 `(int32_t)`。

<a id="sec-debug-03"></a>
### 三、核心 Debug 思路与方法论总结

1. **反向推导法 (Reverse Engineering)**
   * 当高级语言 (C语言) 的测试用例失败时，不要盲目猜测。
   * **第一步**：找到失败的具体 C 代码行。
   * **第二步**：使用 `riscv64-linux-gnu-objdump -d` 查看对应的机器码/汇编指令。
   * **第三步**：分析编译器是如何用 RISC-V 指令实现这段 C 代码逻辑的。
   * **第四步**：锁定具体的嫌疑指令，去 NEMU 源码中针对性地检查该指令的实现。

2. **警惕 C 语言的未定义行为 (Undefined Behavior, UB)**
   * 模拟器是用 C 语言写的，必须深刻理解 C 语言的底层行为。
   * **移位操作**：移位位数不能大于等于类型的位宽，否则结果不可预测。必须加掩码 (如 `& 0x1F`)。
   * **类型转换与扩展**：深刻理解无符号数 (`uint32_t`) 和有符号数 (`int32_t`, `int8_t`) 在右移 (`>>`) 和赋值时的不同表现（逻辑右移 vs 算术右移，零扩展 vs 符号扩展）。

3. **理解宏展开与匹配逻辑**
   * NEMU 的 `INSTPAT` 宏本质上是顺序执行的 `if-else` 链。
   * 编写指令模式时，要注意模式字符串的精确性（特别是 `funct7` 等关键位），避免通配符 `?` 滥用导致指令冲突或被提前误匹配。
   * 保持代码整洁，避免同一条指令在不同区域重复定义。

<a id="sec-pa2-fix-lifecycle"></a>
## PA2 专题：指令修复、UB行为与运行时生命周期全景梳理
> 记录时间：2026年3月3日 | 核心主题：指令行为 Debug，AM 运行时机制解析，程序生命周期总结

<a id="sec-fixlife-01"></a>
### 1. 核心架构解析：NEMU 的双重视角内存映射模型 (Memory Emulation)
* **抛出问题**：看到测试程序反汇编，想进一步深入理解 NEMU 的内存管理。
* **分析过程与逻辑梳理**：
  * NEMU 采用**宿主机（Host）与客体机（Guest）相互映射对偶**的方式构建。
  * **宿主视角**：Linux/NEMU 进程通过 `malloc` 创建了一个巨大的 C 语言数组 `pmem`。
  * **客体视角**：RISC-V 虚拟 CPU 认为它的内存条是从 `0x80000000` 这个物理地址起始的（对应宏 `CONFIG_MBASE`）。
  * **桥梁**：通过核心函数 `guest_to_host()` 和 `host_to_guest()`，对内存地址做线性平移映射。当客户机 CPU 想访问 `0x80000004` 时，NEMU 会拦截该地址，将其减去 `0x80000000` 得到 `0x00000004`，从而转化为真正的C数组下标 `pmem[4]` 去提取出数据。这种抽象是虚拟化最底层的根基所在。

<a id="sec-fixlife-02"></a>
### 2. AM (Abstract Machine) 解析：TRM 与堆栈初始化
* **抛出问题**：C 语言程序在没有操作系统的裸机环境（TRM）上是如何运行起来的？特别是堆、栈环境是如何建立的？
* **逻辑梳理**：
  AM（Abstract Machine）屏蔽了底层的硬件差异。为了支撑一个 C 程序，它需要三步协同走位来构建 TRM（Turing Machine）：
  1. **建空间（链接器 `linker.ld`）**：链接器脚本充当规划局，负责安放 `.text`、`.data`、`.bss` 后，利用内存位置宏强行划分出 `32KB` 作为**栈 (Stack)**，栈顶地址记录在 `_stack_pointer` 中。余下的物理内存直至尽头，其起始点记为 `_heap_start` 准备作**堆 (Heap)**。
  2. **设指针（汇编入口 `start.S`）**：C 程序的函数跳转依赖栈指针寄存器 `sp`。CPU 加载 `start.S` 后的第一件事，就是 `la sp, _stack_pointer`，这确立了 C 环境可以进行参数传递和函数跳转的立足点。接着 `call _trm_init` 跃迁进 C代码。
  3. **定环境（C 入口封装 `trm.c`）**：在这个文件不仅暴露了字符设备打印 (`putch`) 和程序终止机制 (`halt`)，更利用链接器的 `_heap_start` 变量结合物理内存边界 `PMEM_END`，封装为 `Area heap` 结构体对外暴露动态内存分配池。在这里最后一次 wrapper 转发 `int ret = main(mainargs)` 从而踏入你的测试用例。

<a id="sec-fixlife-03"></a>
### 3. 深度总结：从 C 源码到模拟器执行的完整生命周期
结合前三部分的知识盲点，这里构建完整的生命周期认知：
* **阶段一：跨界锻造（交叉编译与链接布局）**
  * 宿主机的交叉编译器将 `add.c`、硬件汇编 `start.S` 及其 `trm.c` 编译链接生成 `.elf`。（链接器在这个阶段确立堆栈地址及内存分布）。
  * 通过 `objcopy -O binary` 提取剥离 ELF 信息，得到最纯粹的机器码裸数据 `.bin` 镜像。
* **阶段二：宇宙大爆炸（NEMU 启动与镜像加载）**
  * NEMU 在宿主机中开辟物理映射大数组 `pmem`。
  * `load_img()` 将 `.bin` 被原封不动填装到 `pmem` 的起始位置。
  * NEMU 重置 CPU `$pc` 为起始运行基地址（RISCV 为 `0x80000000`）。
* **阶段三：命运齿轮转动（CPU 取指-译码-执行大循环 F-D-E）**
  * **取指**：根据 `pc` 地址映射至 `pmem` 并抓取取出连续的 32 bit 指令码。
  * **译码**：根据位操作截取 Opcode，提取出源操作数寄存器索、目标寄存器以及立即数常数。
  * **执行**：根据操作码进行 C 语言层的计算模拟，例如加减、移位（期间注意 `mask & 0x1F` 等 UB 限制）、访存。
  * **写回**：将结果覆写回目标的虚拟 CPU 寄存器 `cpu.gpr[rd]` 中。
* **阶段四：终局归寂（TRM Halt 捕获）**
  * 程序通过层层 `return` 退出 `main()` 回到 `trm.c`，并触发 `halt()` 函数。
  * `halt()` 后端藏着一条定制陷阱伪指令（例如 `ebreak`）。
  * 译码器识别出特殊的 NEMU_TRAP 指令，从而终止 F-D-E 死循环，打印统计结果如 `HIT GOOD TRAP` 后关闭 NEMU 进程释放资源。

<a id="sec-pa2-makefile"></a>
## PA2 专题：Makefile 核心机制深度解析与终端控制流控制反转
> 记录时间：2026年3月4日 | 核心主题：AM Makefile 宏与函数详解，SDB `-b` 批处理模式原理分析





#### 1. 问题的逻辑来源
NEMU 在本质上不仅是一个指令执行机器，更是一个自带 SDB（Simple Debugger 简易调试器）的分析工具。为了方便开发者一上来就能使用 `si`, `x`, `info r` 等指令探测程序的初始状态，NEMU 选择将默认的控制流“挂起”（阻塞），强行把第一阶段的话语权通过一个循环界面交给用户，而不是直接让客体代码狂奔。

#### 2. 解决问题的思维逻辑
如果不需要调试，只想“跑得出结果就拉倒”，我们需要一种方式来绕过这个“挂起等待”的循环。
通过追踪 NEMU 启动机制 `engine_start()` 发现，它最终陷入的是 `sdb_mainloop()`。只要我们能向这个 mainloop 传递一个信号（标志位），让它内部识别到这个信号后，能**系统自动代替用户发出一次 `c` (Continue) 指令**，就能直接突破交互屏障，让 CPU 一路狂飙到底。

#### 3. 如何根据解决思维处理代码
通过外部环境变量打通内部开关：
* **第一步：传参破壁**
  在 AM 中管理向 NEMU 传参的 Makefile (`abstract-machine/scripts/platform/nemu.mk`) 中，找到 `NEMUFLAGS` 配置项。
  修改代码为其追加批处理启动参数指令：`NEMUFLAGS += -b`
* **第二步：参数透传运行**
  在此脚本下方的 `run:` 伪目标内，该标志在 `make run` 时被拼接在了执行脚本面：
  `$(MAKE) -C $(NEMU_HOME) ... run ARGS="$(NEMUFLAGS)" ...` 这样 NEMU 的主控就能接收到。

#### 4. 处理原则：代码背后的控制反转原理
* **为什么有效（处理逻辑）**：因为在 `nemu/src/monitor/monitor.c` 中内置了系统对命令行 `args` 的解析捕捉。一旦截获参数 `-b`，便会触发调用 `sdb_set_batch_mode()` 函数。
* **修改间的逻辑关联**：该函数会将当前 SDB 内的静态全局标志位开关 `is_batch_mode = true`（置为1）。
当 `sdb_mainloop` （位于 `sdb.c` 当中）被唤醒时，第一句话的判断便会发生奇效：
```c
void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL); // <--- 代码自己执行了 Continue！
    return;      // <--- 然后直接 return，绕过了下方的 for 用户交互接收大循环
  }
}
```

#### 5. 全局逻辑链归纳
这个过程是标准的**“命令行传参 (CLI) -> 状态机拨片 -> 控制反转”**模型总结：
1. **指令发出**：外部使用 Makefile `nemu.mk` 注入 `-b` 启动参数。
2. **侦听标记**：NEMU 的初始化引导 `parse_args` 捕捉到字符 `b`。
3. **拨动开关**：修改运行态全局唯一标志变量 `is_batch_mode` 设为 `1`。
4. **截流反转**：SDB 控制大循环 `sdb_mainloop()` 开启时的前置哨兵（If 守卫）察觉开关已开启。
5. **系统代管**：内部模拟玩家行为触发 `cmd_c(NULL)`，此函数直接跨层连通 `cpu_exec(-1)` 核心，使其毫无留恋地陷入 F-D-E 虚拟生命执行死循环，直到碰见 `ebreak` 陷阱程序优雅退出。

<a id="sec-pa2-vscode"></a>
## PA2 专题：VS Code `c_cpp_properties.json` 配置核心逻辑总结
> 记录时间：2026年3月6日 | 核心主题：IntelliSense 与真实编译环境对齐，两份 `json` 配置的职责拆解与收敛

<a id="sec-vscode-01"></a>
### 1. 问题背景：为什么源码能编过，但编辑器仍然满屏飘红？
* **现象**：`nemu/src/monitor/monitor.c` 与 `nemu/src/cpu/cpu-exec.c` 中出现 `CONFIG_TRACE`、`CONFIG_DEVICE`、`__GUEST_ISA__`、`ITRACE_COND` 等“未定义标识符”提示。
* **根因**：这不是 NEMU 源码本身错误，而是 **VS Code 的 IntelliSense 没有完整复刻 `make` 的真实编译上下文**。
* **本质矛盾**：终端中的 `make` 知道如何传入 `-I` 头文件搜索路径、`-D` 宏定义以及 `menuconfig` 生成的配置头；但编辑器静态分析器若没有获得同样的信息，就会对源码作出错误判断。

<a id="sec-vscode-02"></a>
### 2. 总体逻辑：`c_cpp_properties.json` 的本质职责是什么？
这一配置文件的核心逻辑可以概括为一句话：

> **让 VS Code 的 IntelliSense 尽量模拟真实的 NEMU 编译现场。**

为了达成这一点，它主要要补齐四类信息：

#### 1）. `includePath`：告诉编辑器“头文件去哪里找”
* **作用**：等价于编译命令里的 `-I...`。
* **在 NEMU 里的典型需求**：
  * `include`：放着 `common.h`、`isa.h`、`utils.h` 等主头文件。
  * `src/isa/riscv32/include`：放着 `isa-def.h`，而 `isa.h` 会直接 `#include <isa-def.h>`。
* **如果缺失**：编辑器会以为头文件不存在，导致包含链在最前面就断裂。

#### 2）. `defines`：告诉编辑器“编译期额外有哪些宏(非头文件可查询的，一般为makefile脚本中设定的)”
* **作用**：等价于编译命令里的 `-D...`。
* **在当前 NEMU 场景中的关键宏**：
  * `__GUEST_ISA__=riscv32`
  * `ITRACE_COND=true`
* **为什么必须手补**：因为这类宏不是普通头文件里天然写死的，而是 `Makefile` 在真实编译时从外部注入给编译器。

#### 3）. `forcedInclude`：告诉编辑器“无论源文件写没写，都先自动包含这个头”
* **作用**：让 IntelliSense 在分析任何 `.c/.h` 文件前，都先看到某个关键配置头。
* **当前最关键的文件**：`include/generated/autoconf.h`
* **意义**：这个头文件承载了几乎全部 `menuconfig` 生成的 `CONFIG_*` 宏，例如：
  * `CONFIG_TRACE`
  * `CONFIG_DEVICE`
  * `CONFIG_MBASE`
  * `CONFIG_ITRACE`
* **如果不用 `forcedInclude`**：就得把一长串 `CONFIG_*` 手动抄到 `defines` 里，配置会又长又脆弱。

#### 4）. `compilerPath` / `intelliSenseMode`：告诉编辑器“按谁的编译器规则来理解代码”
* **作用**：让 IntelliSense 选择和真实编译器更接近的系统头路径、内建宏和语法行为。
* **当前工程的合理选择**：`/usr/bin/gcc` 配合 `linux-gcc-x64`
* **为什么要和真实环境一致**：因为 NEMU 实际构建使用的是 `gcc`，若编辑器按 `clang` 风格推断，虽然不一定立刻报错，但分析结果可能和真实编译存在偏差。

<a id="sec-vscode-03"></a>
### 3. 为什么旧版配置会显得臃肿？其设定逻辑的问题在哪里？
早期那份较长的 `nemu/.vscode/c_cpp_properties.json`，逻辑上采用的是一种“**手工复制编译结果**”的策略：
* 在 `defines` 中显式写入大批量 `CONFIG_*`，例如 `CONFIG_TRACE`、`CONFIG_MBASE`、`CONFIG_ITRACE` 等。
* 通过 `${workspaceFolder}/**` 广泛递归搜索几乎所有目录，试图用“大范围兜底”避免缺头文件。
* 同时手动加入 `/usr/include`、`/usr/local/include` 和 `clang` 编译器信息，希望让系统头别出问题。

这种写法虽然短期能压住红线，但从工程逻辑上说存在三类问题：

#### 1. 重复维护问题
* `CONFIG_*` 的真实来源本来就是 `include/generated/autoconf.h`。
* 一旦 `menuconfig` 变化，手写的 `defines` 与自动生成头文件就可能失去同步。
* 这会造成“真实编译正确，但 IntelliSense 还在看旧配置”的错位问题。

#### 2. 作用域过大问题
* `${workspaceFolder}/**` 会让编辑器在整个目录树里做超大范围扫描。
* 它虽然省事，但会降低语义分析效率，也容易把本不需要的目录统统卷入索引。

#### 3. 逻辑来源混乱问题
* 有的宏来自 `autoconf.h`。
* 有的宏来自 `Makefile -D...`。
* 有的搜索路径本来由 `compilerPath` 就能自动推断。
* 如果把这些信息全部一股脑塞在一个 `json` 里，后续很难分辨“哪个字段是在弥补什么问题”。

<a id="sec-vscode-04"></a>
### 4. 精简后的新逻辑：为什么现在只剩几项也能正确工作？
精简后的配置不是“功能变少了”，而是**信息来源被重新分层**了：

#### 第一层：交给 `autoconf.h` 的事，就不要自己抄写
* 凡是 `menuconfig` 生成的 `CONFIG_*`，统一交给：
  * `forcedInclude: include/generated/autoconf.h`
* 这样做的好处是：
  * 配置源唯一；
  * 跟随 `menuconfig` 自动变化；
  * 不需要手工维护几十个宏定义。

#### 第二层：只有 `Makefile` 外部注入的宏才留在 `defines`
当前仅保留两类真正需要手补的宏：
* **`__GUEST_ISA__=riscv32`**
  * 对应 `Makefile` 里的：`-D__GUEST_ISA__=$(GUEST_ISA)`
  * 用于让 `isa.h` 内的 `concat(__GUEST_ISA__, ...)` 正确展开。
* **`ITRACE_COND=true`**
  * 对应 `Makefile` 里的：`-DITRACE_COND=...`
  * 用于让 `cpu-exec.c` 中的 `if (ITRACE_COND)` 被 IntelliSense 识别。

#### 第三层：只保留真正必要的头文件目录
* `include`
* `src/isa/riscv32/include`

这两个目录已经足以覆盖本次报错链条中所需的关键头文件，继续扩大目录范围收益不高，反而可能影响分析效率。

<a id="sec-vscode-05"></a>
### 5. 最终抽象：以后遇到新的红线，应该怎样判断是否需要修改 `json`？
可以用一条简单的判断链处理：

#### 情况一：报错的是 `CONFIG_XXX`
* **优先判断**：它是否本来应该由 `menuconfig` 生成？
* **处理逻辑**：通常不改 `defines`，先确认 `autoconf.h` 是否存在、是否最新，以及 `forcedInclude` 路径是否正确。

#### 情况二：报错的是非 `CONFIG_` 宏
* 例如：`__GUEST_ISA__`、`ITRACE_COND`
* **优先判断**：它是否来自 `Makefile` 中的 `CFLAGS += -D...`
* **处理逻辑**：若确实属于外部注入宏，则应补进 `defines`。

#### 情况三：报错是某个头文件找不到
* **优先判断**：这个头到底位于哪个目录。
* **处理逻辑**：把真正包含它的目录加入 `includePath`，而不是盲目扩大到整个工程树。

<a id="sec-vscode-06"></a>
### 6. 这次配置总结背后的方法论
这一轮处理 VS Code 红线问题，本质上不是“会不会写 `json`”的问题，而是一次典型的**构建系统认知反推**：

1. **先分清真实编译通过与否**：用 `make` 确认真正的代码正确性。
2. **再分清 IntelliSense 缺的是什么信息**：是缺头文件、缺宏，还是缺配置头。
3. **按信息来源拆分职责**：
   * 头文件路径 → `includePath`
   * `Makefile -D` 注入宏 → `defines`
   * `menuconfig` 自动生成配置 → `forcedInclude autoconf.h`
   * 编译器语义 → `compilerPath` 与 `intelliSenseMode`
4. **遵循“最小必要集”原则**：只保留必需项，不用大而全兜底。

<a id="sec-vscode-07"></a>
### 7. 本次最终结论（一句话压缩）
> **`c_cpp_properties.json` 的核心逻辑，不是胡乱堆配置项，而是把 NEMU 真正的编译环境拆成“头文件路径 + 外部注入宏 + 自动生成配置头 + 编译器语义”四层，再最小化地同步给 VS Code。**


<a id="sec-vscode-08"></a>
### 8. 路径该怎么归类：`includePath`、`browse.path` 与 `forcedInclude` 的完整判断法
> **补充时间：2026年3月6日 | 目标：解决“一个目录到底该放哪一类功能区”这个最容易混淆的问题**

在实际修改 `json` 时，最容易混淆的不是语法，而是路径归类：某个目录到底该放进 `includePath`，还是 `browse.path`？为什么有的头文件目录必须进 `includePath`，有的源码目录却更适合放 `browse.path`？为什么 `autoconf.h` 不只是“能找到”就够了，而要单独放进 `forcedInclude`？

#### 1）. `includePath`：负责“当前文件能不能被正确分析”
* **核心职责**：解决当前翻译单元的头文件查找问题。
* **判断方式**：如果少了这个目录，当前文件的 `#include` 会直接找不到头文件，那么这个目录就应该归到 `includePath`。
* **等价理解**：它本质上对应真实编译命令中的 `-I...`。
* **当前工程中的典型归类**：
  * `nemu/include`：放着 `common.h`、`isa.h`、`utils.h`、`debug.h` 等主头文件。
  * `nemu/src/isa/riscv32/include`：放着 `isa-def.h`，会被 `isa.h` 直接包含。
  * `abstract-machine/am/include`、`abstract-machine/klib/include`：如果工作区根配置也希望兼顾 `AM`/`klib` 语义分析，这些目录也应归到 `includePath`。

> **凡是“会参与 `#include` 解析”的目录，优先归 `includePath`。**

#### 2）. `browse.path`：负责“整个工程能不能被顺畅跳转和导航”
* **核心职责**：服务全局索引、符号搜索、`F12` 跳转定义等导航能力。
* **判断方式**：如果某个目录不一定参与 `#include`，但你希望 VS Code 扫描它里面的 `.c/.h` 文件并建立符号索引，那么它更适合归到 `browse.path`。
* **当前工程中的典型归类**：
  * `nemu/src`：`engine_start()`、`is_exit_status_bad()`、`expr()` 这些定义都在 `.c` 实现文件中，为了从声明跳到定义，它非常适合加入 `browse.path`。
  * `nemu/include`、`nemu/src/isa/riscv32/include`：这两类目录通常既适合放 `includePath`，也适合放 `browse.path`，因为它们既参与头文件查找，也值得进入全局索引。

> **凡是“希望被全工程索引、用于跳转和导航”的目录，优先归 `browse.path`。**

#### 3）. `forcedInclude`：负责“关键配置头是否在分析前自动生效”
* **核心职责**：让某个关键头文件在分析任何源码前，都被默认预加载。
* **判断方式**：如果一个头文件必须让所有翻译单元都先看到，而不是依赖源码自己写 `#include`，那么它更适合归到 `forcedInclude`。
* **当前工程中的关键案例**：
  * `nemu/include/generated/autoconf.h`：它承载了 `menuconfig` 生成的 `CONFIG_*` 宏，必须在分析前自动生效；仅把它所在目录放到 `includePath` 还不够，因为那只能保证“找得到”，不能保证“默认先包含”。

> **凡是“需要预装到每个翻译单元里”的关键配置头，归到 `forcedInclude`。**

#### 4）. 当前两份 `json` 的路径归类落点
* **工作区根目录 `/.vscode/c_cpp_properties.json`**
  * `includePath`：`${workspaceFolder}/nemu/include`、`${workspaceFolder}/nemu/src/isa/riscv32/include`、`${workspaceFolder}/abstract-machine/am/include`、`${workspaceFolder}/abstract-machine/klib/include`
  * `browse.path`：`${workspaceFolder}/nemu/include`、`${workspaceFolder}/nemu/src`、`${workspaceFolder}/nemu/src/isa/riscv32/include`
  * `forcedInclude`：`${workspaceFolder}/nemu/include/generated/autoconf.h`
* **`nemu/.vscode/c_cpp_properties.json`**
  * `includePath`：`${workspaceFolder}/include`、`${workspaceFolder}/src/isa/riscv32/include`
  * `browse.path`：若要恢复 `.c` 定义跳转，建议加入 `${workspaceFolder}/include`、`${workspaceFolder}/src`、`${workspaceFolder}/src/isa/riscv32/include`
  * `forcedInclude`：`${workspaceFolder}/include/generated/autoconf.h`

#### 5）. 最实用的归类三问法
1. **这个目录里的文件会不会被源码通过 `#include` 直接包含？**
   * 会 → 归 `includePath`
2. **这个目录里的文件是不是主要用于函数实现、变量定义、符号跳转？**
   * 是 → 归 `browse.path`
3. **这个是不是必须在所有文件分析前自动生效的关键配置头？**
   * 是 → 归 `forcedInclude`

#### 6）. 一句话总收束
> **`includePath` 管“头文件能不能找到”，`browse.path` 管“定义能不能跳到”，`forcedInclude` 管“关键配置头会不会自动预加载”。路径该归哪一类，不看名字像不像头文件目录，而看它在 IntelliSense 里承担的职责是什么。**


<a id="sec-pa2-program-lifecycle"></a>
## PA2 专题：NEMU 执行客户程序的完整生命周期总总结
> 记录时间：2026年3月8日 | 核心主题：从源码构建、镜像生成、参数解析、镜像装载、地址映射到 CPU 取指执行的全流程闭环总结

<a id="sec-proglife-01"></a>
### 一、总目标：NEMU 到底在完成什么任务？
在这两天的分析中，最核心的问题其实只有一个：

> **NEMU 如何让一个在 `am-kernels`/`tests` 中编写的客户程序，最终在宿主机 Linux 进程里“像运行在一台真实机器上”那样被执行起来？**

要回答这个问题，必须把整个过程拆成五层：

1. **程序构建层**：客户程序源码如何变成机器码镜像；
2. **参数传递层**：镜像路径和运行选项如何传入 NEMU；
3. **系统初始化层**：NEMU 如何建立 CPU、内存和调试环境；
4. **镜像装载与地址映射层**：客户机地址如何落到宿主机 `pmem` 数组；
5. **CPU 执行层**：程序如何从 `RESET_VECTOR` 开始进入取指、译码、执行、写回循环。

如果把它压成一条总链，可以写成：

```text
客户程序源码
-> 交叉编译 + 链接得到 ELF
-> objcopy 提取为 BIN
-> make run 将 BIN 作为 IMAGE 传给 NEMU
-> parse_args() 解析 IMAGE 和选项
-> init_mem() / init_isa() 建立初始机器状态
-> load_img() 将 BIN 覆盖装入 RESET_VECTOR
-> guest_to_host() 映射到宿主机 pmem
-> cpu.pc = RESET_VECTOR
-> 进入 CPU 取指、译码、执行、写回
```

<a id="sec-proglife-02"></a>
### 二、第一层：客户程序从何而来？——源码到镜像的构建链
#### 1. 客户程序的真正来源
NEMU 自己并不“生成程序”，它只是**执行程序**。真正的客户程序来源于：

* `am-kernels/kernels/*`
* `am-kernels/tests/*`
* 以及和 `AbstractMachine` 配套的 `trm.c`、`start.S`、`klib` 等运行时支持代码

例如：

* `am-kernels/kernels/hello/hello.c`
* `am-kernels/tests/cpu-tests/*.c`
* `am-kernels/tests/am-tests/*.c`

这些源文件的目标不是宿主机 Linux，而是 **客户机 ISA（例如 `riscv32`）**。

#### 2. 为什么必须交叉编译
因为宿主机当前是 Linux/x86_64，而客户机目标是 `riscv32-nemu`。所以必须使用交叉编译器，把：

* C 语言源码
* 汇编入口 `start.S`
* 运行时支持代码 `trm.c`

转成 **RISC-V 机器码目标文件**。

所以“交叉编译”的本质不是单纯换个编译器，而是：

> **让生成出的机器码属于客户机架构，而不是宿主机架构。**

#### 3. 为什么先得到 `.elf`
在 `abstract-machine/Makefile` 中，源码先被编译为 `.o`，再链接为：

* `build/xxx-riscv32-nemu.elf`

这里的 `.elf` 是完整的可执行格式，包含：

* `.text` 代码段
* `.rodata` 只读数据
* `.data` 已初始化数据
* `.bss` 未初始化数据
* 入口符号 `_start`
* 各符号和各段的**地址布局**

也就是说：

> **ELF 是“带地址语义”的程序镜像。**

#### 4. 链接脚本 `linker.ld` 在做什么
`abstract-machine/scripts/linker.ld` 中最重要的一句是：

```ld
. = _pmem_start + _entry_offset;
```

而 `abstract-machine/scripts/platform/nemu.mk` 又通过：

```makefile
LDFLAGS += --defsym=_pmem_start=0x80000000 --defsym=_entry_offset=0x0
```

把这两个符号具体化。

于是当前平台下：

```text
程序链接起始地址 = 0x80000000
```

这说明程序在**客户机视角**下被安排成从 `0x80000000` 开始布局，而不是说明宿主机 Linux 真正把它装在了这个地址上。

这个区别极其重要：

* **链接地址**：客户机程序的逻辑装载/运行地址；
* **宿主机真实地址**：Linux 进程中 `pmem` 数组的实际指针值；
* 二者最终通过 `guest_to_host()` 做映射。

#### 5. 为什么还要从 `.elf` 提取成 `.bin`
在 `nemu.mk` 中：

```makefile
$(OBJCOPY) -S --set-section-flags .bss=alloc,contents -O binary $(IMAGE).elf $(IMAGE).bin
```

这一步把 ELF 变成 BIN。这里要记住两个结论：

* **`.elf` 有地址和段信息**；
* **`.bin` 只有纯裸字节流，没有地址元信息**。

所以：

> **`.bin` 本身并不会告诉你“我应该从 `0x80000000` 开始”。**

真正让我们认定它从 `0x80000000` 开始的是三者统一：

1. 链接脚本把 ELF 安排到 `0x80000000`；
2. NEMU 在 `RESET_VECTOR` 处装载 BIN；
3. CPU 的 `cpu.pc` 也从 `RESET_VECTOR` 开始。

<a id="sec-proglife-03"></a>
### 三、第二层：NEMU 怎么拿到这个外部镜像？——命令行与 `parse_args()`
#### 1. 镜像如何从 Makefile 传给 NEMU
在 `abstract-machine/scripts/platform/nemu.mk` 中：

```makefile
run: insert-arg
$(MAKE) -C $(NEMU_HOME) ISA=$(ISA) run ARGS="$(NEMUFLAGS)" IMG=$(IMAGE).bin
```

这里完成了一个关键动作：

> **把构建出来的 `.bin` 文件，通过 `IMG=$(IMAGE).bin` 传给 NEMU 的运行命令。**

在 `nemu/scripts/native.mk` 中又进一步拼成：

```makefile
NEMU_EXEC := $(BINARY) $(ARGS) $(IMG)
```

于是最终 NEMU 的执行命令大致就是：

```text
build/riscv32-nemu-interpreter [OPTION...] /path/to/xxx.bin
```

这里最后那个普通位置参数，就是客户程序镜像路径。

#### 2. `parse_args()` 的整体职责
`init_monitor()` 一开始就调用：

```c
parse_args(argc, argv);
```

它的职责不是执行程序，而是：

> **把命令行中的运行配置解析成 NEMU 的内部状态变量。**

它解析出的主要变量包括：

* `log_file`：日志文件路径，对应 `-l`
* `diff_so_file`：DiffTest 参考实现库路径，对应 `-d`
* `difftest_port`：DiffTest 端口，对应 `-p`
* `img_file`：客户机镜像路径，对应普通参数 `IMAGE`
* `batch mode`：SDB 是否以批处理模式运行，对应 `-b`

#### 3. `getopt_long()` 机制与 `optarg`
在 `parse_args()` 中：

```c
while ((o = getopt_long(argc, argv, "-bhl:d:p:", table, NULL)) != -1) {
```

这里有两个极其关键的点：

##### （1）`optarg` 的语义
`optarg` 不是用户自定义变量，而是 `getopt_long()` 提供的全局变量，表示：

> **当前解析到的这个选项所对应的参数字符串。**

例如：

* `-l nemu-log.txt` -> `optarg = "nemu-log.txt"`
* `-p 1234` -> `optarg = "1234"`
* `-d ref.so` -> `optarg = "ref.so"`

##### （2）为什么普通参数 `IMAGE` 会返回 `1`
这里 `shortopts` 字符串最前面有一个前导 `-`：

```c
"-bhl:d:p:"
```

GNU `getopt` 的规则是：

> **当短选项规则串以 `-` 开头时，遇到普通非选项参数时，会返回值 `1`，并把该普通参数放入 `optarg`。**

所以：

```c
case 1: img_file = optarg; return 0;
```

就是在处理客户机镜像路径。

这并不是“程序员自己规定普通参数等于 1”，而是 `getopt_long()` 的约定行为。

#### 4. 为什么只接收第一个普通参数
`parse_args()` 中处理镜像文件时写的是：

```c
case 1: img_file = optarg; return 0;
```

这里不是 `break`，而是 `return 0`。这表示：

> **一旦碰到第一个普通位置参数，就把它当作 IMAGE，并结束参数解析。**

这体现了当前 NEMU 的命令行模型：

```text
NEMU [OPTION...] IMAGE
```

也就是说，它设计上只关心一个客户机镜像文件。

<a id="sec-proglife-04"></a>
### 四、第三层：系统初始化顺序——`init_monitor()` 为什么这么排
`init_monitor()` 的顺序是：

```c
parse_args(argc, argv);
init_rand();
init_log(log_file);
init_mem();
IFDEF(CONFIG_DEVICE, init_device());
init_isa();
long img_size = load_img();
init_difftest(diff_so_file, img_size, difftest_port);
init_sdb();
IFDEF(CONFIG_ITRACE, init_disasm());
welcome();
```

这个顺序不是随便写的，而是有严格依赖关系：

#### 1. 先解析参数
因为后面所有初始化模块都依赖这些配置：

* `init_log()` 要知道 `log_file`
* `load_img()` 要知道 `img_file`
* `init_difftest()` 要知道 `diff_so_file` 和 `difftest_port`
* SDB 要知道是否 batch mode

#### 2. 再初始化日志和内存
因为后续很多模块需要打印信息，也需要可用的物理内存空间。

#### 3. 再执行 `init_isa()`
因为 CPU 起始状态、内置镜像和复位入口要先建立起来。

#### 4. 再执行 `load_img()`
因为外部镜像如果存在，就要覆盖 `init_isa()` 放进去的内置镜像。

#### 5. 最后初始化 `difftest` / `sdb` / `disasm`
因为这几个模块依赖于：

* 镜像大小
* CPU 初始状态
* 已经装载好的程序内容

所以这个顺序实际上体现了一个非常清晰的设计：

> **先解析配置，再准备环境，再建立最小可运行 CPU 状态，再装载真正的客户程序，再挂接调试与辅助模块。**

<a id="sec-proglife-05"></a>
### 五、第四层：`init_isa()`、`restart()`、`load_img()` 之间的关系
这是整个启动流程中最容易混淆、但也是最关键的一条小链。

#### 1. `init_isa()` 不是负责加载外部镜像
在 `nemu/src/isa/riscv32/init.c` 中：

```c
static const uint32_t img[] = { ... };

void init_isa() {
  memcpy(guest_to_host(RESET_VECTOR), img, sizeof(img));
  restart();
}
```

这里复制的是**内置镜像** `img[]`，不是外部 `.bin` 文件。

所以：

> **`init_isa()` 的职责是：先放一个 built-in image，并初始化 CPU 起始状态。**

#### 2. `restart()` 负责 CPU 启动状态
```c
static void restart() {
  cpu.pc = RESET_VECTOR;
  cpu.gpr[0] = 0;
}
```

它做了两件关键事：

* 把 CPU 的起始 `pc` 设为 `RESET_VECTOR`
* 把 RISC-V 的零寄存器 `x0` 置 0

也就是说：

> **`restart()` 决定 CPU 从哪里开始执行。**

#### 3. `load_img()` 才负责加载外部 `.bin`
在 `monitor.c` 中：

```c
fread(guest_to_host(RESET_VECTOR), size, 1, fp);
```

这表示：

> **如果存在外部镜像，它会被直接写到 `RESET_VECTOR` 对应的内存位置，从而覆盖掉 `init_isa()` 先前写入的 built-in image。**

#### 4. 为什么先放内置镜像再覆盖依然合理
因为 NEMU 需要支持两条启动路径：

* **无外部镜像**：用 built-in image 启动，保证系统最小可运行；
* **有外部镜像**：built-in image 只做占位，随后被真实客户程序覆盖。

所以这条小链应记成：

```text
init_isa()
-> 在 RESET_VECTOR 放 built-in image
-> restart() 将 cpu.pc 设为 RESET_VECTOR
-> load_img() 若有外部 .bin，则覆盖同一位置内容
```

这说明：

* **CPU 从哪里开始执行**：由 `restart()` 决定；
* **CPU 实际执行什么内容**：由 `RESET_VECTOR` 处最终存的字节决定。

<a id="sec-proglife-06"></a>
### 六、第五层：`RESET_VECTOR` 的语义——为什么它是全局锚点
在 `paddr.h` 中：

```c
#define PMEM_LEFT  ((paddr_t)CONFIG_MBASE)
#define PMEM_RIGHT ((paddr_t)CONFIG_MBASE + CONFIG_MSIZE - 1)
#define RESET_VECTOR (PMEM_LEFT + CONFIG_PC_RESET_OFFSET)
```

当前常见配置下：

* `CONFIG_MBASE = 0x80000000`
* `CONFIG_PC_RESET_OFFSET = 0x0`

所以：

```text
RESET_VECTOR = 0x80000000
```

#### 1. 它不是“宿主机真实地址”
`RESET_VECTOR` 表示的是：

> **客户机物理地址空间中的复位入口地址**

而不是 Linux 进程里真实的 C 数组指针。

#### 2. 它为什么如此重要
因为它同时被三方使用：

* **链接器**：把程序逻辑布局安排到这里；
* **装载器**：把镜像内容写到这里；
* **CPU**：把 `pc` 设到这里开始执行。

所以它是整个执行生命周期中的统一锚点。

#### 3. 为什么保留 `CONFIG_PC_RESET_OFFSET`
虽然当前偏移量通常是 `0`，但这个设计允许：

* 程序入口不一定等于物理内存起始地址；
* 不同平台可以通过 offset 调整复位入口；
* 更贴近真实硬件中“复位向量 = 基址 + 偏移”的设计模型。

<a id="sec-proglife-07"></a>
### 七、第六层：客户机地址如何落到宿主机数组里——`paddr.c` 的核心桥梁作用
这是整个系统里最底层、最本质的一步：

> **客户机物理地址如何变成宿主机 Linux 进程中 `pmem` 数组的真实指针？**

#### 1. `pmem` 是谁
在 `paddr.c` 中：

```c
static uint8_t pmem[CONFIG_MSIZE] PG_ALIGN = {};
```

这就是 NEMU 用来模拟客户机物理内存的真实宿主机数组。

也就是说：

* 客户机眼中：有一块从 `0x80000000` 开始的物理内存；
* 宿主机眼中：其实只是一个 `uint8_t pmem[]`。

#### 2. `guest_to_host()` 的本质
```c
uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }
```

这一步做的是：

> **把客户机物理地址，平移成 `pmem` 数组的下标。**

例如：

* `paddr = 0x80000000` -> `pmem + 0`
* `paddr = 0x80000004` -> `pmem + 4`

所以：

```c
guest_to_host(RESET_VECTOR)
```

在当前配置下本质上就是：

```c
pmem + 0
```

#### 3. `host_to_guest()` 的反向作用
```c
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }
```

它把宿主机指针重新翻译回客户机物理地址。

#### 4. `paddr_read()` / `paddr_write()` 的职责
它们统一管理对物理地址的访问：

* 如果地址在 `pmem` 范围内 -> 读写 `pmem`
* 如果不是普通内存但启用了设备 -> 转去 `mmio_read/mmio_write`
* 再不是 -> 越界报错

这说明 `paddr.c` 的设计不是“裸数组访问”，而是：

> **建立一个统一的物理地址访问分发层。**

<a id="sec-proglife-08"></a>
### 八、第七层：`load_img()` 如何借助地址映射把镜像落地
现在把前面几层合并起来，`load_img()` 的关键一句就彻底清楚了：

```c
fread(guest_to_host(RESET_VECTOR), size, 1, fp);
```

它的真实语义是：

1. `RESET_VECTOR` 给出客户机物理地址入口；
2. `guest_to_host(RESET_VECTOR)` 把它翻译成宿主机 `pmem` 指针；
3. `fread()` 把 `.bin` 中的裸字节顺序写到这个 `pmem` 区域；
4. 于是从客户机视角看，就等价于：
   * 镜像被装到了物理内存 `0x80000000` 开始的位置。

所以：

> **镜像不是“装进一个神秘的客户机内存”，而是被装进宿主机数组 `pmem`，只是 NEMU 把它解释成客户机物理内存。**

<a id="sec-proglife-09"></a>
### 九、第八层：为什么 `.bin` 本身没有地址，但程序仍然能从 `0x80000000` 运行
这是这两天里最容易混淆的问题之一。

#### 1. `.bin` 只有字节，没有地址元信息
它只是：

```text
00 11 22 33 ...
```

这种字节流。

#### 2. 真正的地址语义来自 ELF 链接布局
链接时，`.elf` 已经被安排到了：

```text
0x80000000
```

#### 3. 装载时，NEMU 再把 `.bin` 第 0 个字节放到 `RESET_VECTOR`
也就是：

```text
0x80000000
```

#### 4. CPU 也从 `RESET_VECTOR` 开始执行
于是形成闭环：

```text
链接地址 = 装载地址 = CPU 起始 PC = 0x80000000
```

所以我们说“这个 `.bin` 对应的程序从 `0x80000000` 开始”，并不是因为 `.bin` 自己写了这个地址，而是因为：

> **整个系统从链接、装载到执行三处都共同遵守了这套地址约定。**

<a id="sec-proglife-10"></a>
### 十、第九层：为什么 `includePath` 与 IntelliSense 配置问题值得纳入生命周期总结
虽然 VS Code 配置不是 NEMU 运行时逻辑的一部分，但在调试和阅读源码时，它直接影响我们是否能正确理解代码，因此也应该纳入总结。

#### 1. 核心认知
`c_cpp_properties.json` 的目的不是乱堆路径，而是：

> **让 IntelliSense 尽量模拟真实编译环境。**

#### 2. 三种功能区分工
* **`includePath`**：负责当前文件头文件能否找到；
* **`browse.path`**：负责全局跳转与索引；
* **`forcedInclude`**：负责自动预加载关键配置头，例如 `autoconf.h`；
* **`defines`**：负责 `Makefile -D` 注入但头文件里没有的宏，例如 `__GUEST_ISA__`、`ITRACE_COND`。

#### 3. 两个典型误区
* **误区一：`includePath` 会递归搜索子目录**
  * 实际上不会自动深搜，但如果 `#include <subdir/file.h>`，只加父目录就够。
* **误区二：所有文件都适用同一份 guest-target 配置**
  * `nemu/tools/kconfig/conf.c` 这类 host tool 文件使用当前 `NEMU-riscv32` 配置时，可能产生如 `PATH_MAX` 的 IntelliSense 误报。

这一部分的意义在于提醒后续阅读代码时要始终区分：

> **“真实编译问题”** 和 **“编辑器语义分析误报”**

<a id="sec-proglife-11"></a>
### 十一、第十层：CPU 真正开始执行时发生了什么
当：

* 内存已经初始化完成
* `pc` 已经设置为 `RESET_VECTOR`
* 镜像已经落到 `pmem` 对应位置

之后，NEMU 就进入经典 CPU 主循环：

```text
取指 (Fetch)
-> 译码 (Decode)
-> 执行 (Execute)
-> 写回 (Write-back)
-> 更新 pc
-> 下一条指令
```

这里的第一步取指，本质就是去访问：

```c
paddr_read(cpu.pc, ...)
```

最终经过：

* `in_pmem()`
* `pmem_read()`
* `guest_to_host()`
* `host_read()`

把宿主机数组里的字节重新解释成客户机指令。

这说明：

> **程序能执行，不是因为 NEMU 真的拥有客户机物理内存，而是因为它把宿主机数组中的字节流解释成了客户机内存内容，再按客户机 ISA 规则解释成指令。**

<a id="sec-proglife-12"></a>
### 十二、全流程中的关键函数关系图
为了后续复习，可以把这几组函数关系单独记住：

#### 1. 构建链
```text
am-kernels/tests 源码
-> abstract-machine/Makefile
-> .elf
-> objcopy
-> .bin
```

#### 2. 参数链
```text
make run
-> IMG=$(IMAGE).bin
-> NEMU_EXEC
-> argv[]
-> parse_args()
-> img_file / log_file / diff_so_file / difftest_port
```

#### 3. 初始化链
```text
init_monitor()
-> parse_args()
-> init_log()
-> init_mem()
-> init_isa()
-> load_img()
-> init_difftest()
-> init_sdb()
-> welcome()
```

#### 4. 装载链
```text
load_img()
-> fopen(img_file)
-> fseek/ftell 获取镜像大小
-> fread(guest_to_host(RESET_VECTOR), ...)
-> 外部镜像覆盖 built-in image
```

#### 5. 地址映射链
```text
客户机物理地址 paddr
-> guest_to_host(paddr)
-> 宿主机 pmem 指针
-> host_read/host_write
```

#### 6. 执行链
```text
cpu.pc = RESET_VECTOR
-> paddr_read(cpu.pc, len)
-> 取到指令字节
-> ISA 译码与执行
-> 更新 cpu.pc
-> 下一条指令
```

<a id="sec-proglife-13"></a>
### 十三、这两天分析中最关键的“为什么”与“如何解决”
#### 1. 为什么 `.bin` 没有地址还能跑起来？
* **原因**：地址语义来自 ELF 链接和 NEMU 的装载策略，而不是来自 `.bin` 本身。
* **解决思路**：分清 `.elf` 负责“地址布局”，`.bin` 负责“裸字节内容”。

#### 2. 为什么 `RESET_VECTOR` 如此关键？
* **原因**：它同时被链接、装载、CPU 启动使用，是统一锚点。
* **解决思路**：始终把它看成“程序装载地址 = CPU 起跑线”。

#### 3. 为什么 `init_isa()` 和 `load_img()` 看起来都在装程序？
* **原因**：前者装 built-in image，后者装外部 image。
* **解决思路**：明确 `init_isa()` 是兜底初始化，`load_img()` 是真实客户程序覆盖装载。

#### 4. 为什么 IntelliSense 报错但编译通过？
* **原因**：编辑器配置没有完整模拟真实编译上下文，或 host tool 使用了不匹配的 target 配置。
* **解决思路**：区分 `includePath`、`browse.path`、`forcedInclude`、`defines` 的职责，并优先相信真实编译结果。

#### 5. 为什么 `parse_args()` 要用 `case 1` 接镜像路径？
* **原因**：`getopt_long()` 在前导 `-` 模式下会把普通参数作为返回值 `1` 交出。
* **解决思路**：理解 GNU `getopt` 约定，而不是把它误当成程序员随手写的魔法数字。

<a id="sec-proglife-14"></a>
### 十四、最终总收束：NEMU 执行客户程序的完整生命周期一句话版本
> **客户程序源码先经交叉编译和链接生成带地址语义的 ELF，再提取为裸二进制 BIN；运行时该 BIN 通过命令行传入 NEMU，被 `parse_args()` 解析为 `img_file`，随后 NEMU 在 `init_mem()` 建立模拟物理内存、在 `init_isa()` 初始化 CPU 并写入内置镜像、在 `load_img()` 中把外部镜像加载到 `RESET_VECTOR` 对应的 `pmem` 区域覆盖内置镜像，同时 CPU 的 `pc` 也从 `RESET_VECTOR` 起跑，最终在宿主机进程的数组内存上完成对客户机程序的取指、译码、执行和写回，从而实现完整的软件模拟执行。**


<a id="sec-pa2-mtrace"></a>
## PA2 专题：`mtrace` 的完整设计过程总结（从提问到落地）
> 记录时间：2026年3月9日 | 核心主题：`mtrace` 的设计目标、配置链、插桩点选择、路径处理与最终验证

<a id="sec-mtrace-01"></a>
### 1. 问题是怎么被提出的？

在已经理解 `itrace`、`watchpoint`、`iringbuf` 等调试机制之后，自然会继续追问一个更底层的问题：

> **CPU 执行过程中，访存究竟访问了哪些物理地址？读了什么？写了什么？这些行为能不能像 `itrace` 一样被记录下来？**

这就是 `mtrace`（memory trace）的出发点。

它和 `itrace` 的关注点不同：

* `itrace` 记录“执行了哪条指令”；
* `mtrace` 记录“这条指令在执行期间做了哪些内存读写”。

因此，`mtrace` 的本质不是“另一种日志文件”，而是对 **CPU 执行副作用** 的观测工具。

---

<a id="sec-mtrace-02"></a>
### 2. 为什么需要单独设计 `mtrace`，而不是复用现有 `log`？

#### 2.1 直接复用普通 `log` 不够清晰
普通 `log` 主要承载的是框架初始化信息、运行时提示、统计输出等内容，例如：

* 物理内存范围；
* 设备映射；
* 加载镜像大小；
* `HIT GOOD TRAP`；
* 指令 trace 等。

如果把内存读写也混在里面，会有两个问题：

1. **语义混杂**：初始化日志和内存行为日志不在同一层次；
2. **数据量爆炸**：访存条目通常远大于普通调试输出，很容易把普通日志淹没。

因此更合理的方式是：

* 普通日志继续写 `mtrace-log.txt`；
* 专门给 `mtrace` 准备独立文件 `mtrace-log.txt`。

#### 2.2 为什么还要支持“条件表达式”？
如果每一次访存都打印，日志体量会非常大。于是就引出了第二层需求：

> **能不能只在满足某个条件时才记录访存？**

这和 `itrace` 里的 `ITRACE_COND` 思路一致，所以 `mtrace` 也需要一个类似的过滤表达式：

* `CONFIG_MTRACE`：决定是否编译进 `mtrace` 代码；
* `MTRACE_COND`：决定运行时这一次访存要不要真正记下来。

---

<a id="sec-mtrace-03"></a>
### 3. 设计目标是什么？

围绕上面的需求，最终把 `mtrace` 的目标拆成了四层：

#### 3.1 配置层目标
通过 `menuconfig` 控制 `mtrace` 开关，而不是手写宏硬编码。

#### 3.2 执行层目标
在真正发生物理内存读写的地方插桩，而不是在上层猜测。

#### 3.3 输出层目标
将日志写入独立文件，避免污染普通日志。

#### 3.4 路径层目标
`mtrace-log.txt` 必须跟随 `make ARCH=riscv32-nemu ALL=xxx run` 的实际运行目录生成，不能只对 NEMU 内置最小镜像有效，更不能简单写死成相对路径 `build/mtrace-log.txt`。

---

<a id="sec-mtrace-04"></a>
### 4. 整体思路：把 `mtrace` 拆成三条链来看

从最后成功运行的结果回看，`mtrace` 实际上是三条链协同工作：

#### 4.1 配置链
```text
Kconfig
-> menuconfig
-> .config
-> include/generated/autoconf.h
-> Makefile -D 宏注入
-> C 代码中的 CONFIG_MTRACE / MTRACE_COND
```

#### 4.2 运行链
```text
cpu_exec()
-> 指令执行
-> paddr_read()/paddr_write()
-> if (MTRACE_COND)
-> mtrace_write(...)
-> mtrace-log.txt
```

#### 4.3 路径链
```text
am-kernels/tests/cpu-tests/Makefile
-> abstract-machine/scripts/platform/nemu.mk
-> NEMUFLAGS += -b -l .../build/nemu-log.txt
-> NEMU 启动 parse_args()
-> log_file / img_file
-> monitor.c 推导 mtrace-log.txt 目录
-> init_mtrace_log(...)
```

只要这三条链里任何一条断掉，`mtrace` 最终都不会正确工作。

---

<a id="sec-mtrace-05"></a>
### 5. 第一步：先解决“要不要编译 `mtrace`”的问题

#### 5.1 为什么要放进 `Kconfig`？
因为 NEMU 已经有一套成熟的配置体系：

* `CONFIG_TRACE`
* `CONFIG_ITRACE`
* `CONFIG_ITRACE_COND`
* `CONFIG_WATCHPOINT`

`mtrace` 最自然的做法，就是按同样的风格融入进去，而不是自创一套配置入口。

#### 5.2 最终设计
在 `nemu/Kconfig` 里加入：

* `config MTRACE`
* `config MTRACE_COND`

它们的职责区分非常重要：

* **`CONFIG_MTRACE`**：总开关。没开就连相关代码都不编译；
* **`CONFIG_MTRACE_COND`**：配置项字符串，例如 `"true"`；
* **`MTRACE_COND`**：真正供 C 表达式直接使用的宏，例如 `true`、`cpu.pc >= 0x80000000`。

这里最容易混淆的是后两者。

---

<a id="sec-mtrace-06"></a>
### 6. 第二步：为什么 `CONFIG_MTRACE_COND` 还不够，必须再有 `MTRACE_COND`？

这是整个设计过程中最关键的“为什么”之一。

#### 6.1 `CONFIG_MTRACE_COND` 的本质
`menuconfig` 生成到 `autoconf.h` 里的通常是：

```c
#define CONFIG_MTRACE_COND "true"
```

注意这里是一个**字符串字面量**，不是直接可执行的 C 条件表达式。

#### 6.2 代码里真正想写的是什么？
在 `paddr_read()` / `paddr_write()` 里更希望写的是：

```c
if (MTRACE_COND) {
  ...
}
```

这要求 `MTRACE_COND` 展开后直接变成一段合法 C 表达式，例如：

```c
true
cpu.pc >= 0x80000100
addr >= 0x80000000 && addr < 0x80001000
```

#### 6.3 所以需要 Makefile 再做一层“解字符串”
在 `nemu/Makefile` 中用：

```makefile
CFLAGS_TRACE += -DMTRACE_COND=$(if $(CONFIG_MTRACE_COND),$(call remove_quote,$(CONFIG_MTRACE_COND)),true)
```

它的作用是把：

* `CONFIG_MTRACE_COND="true"`

变成：

* 编译命令行里的 `-DMTRACE_COND=true`

这样 C 代码里才能直接写 `if (MTRACE_COND)`。

#### 6.4 为什么 VS Code 里还要单独加 `defines`？
因为 IntelliSense 不会真正执行 Makefile。

也就是说：

* **真实编译器** 能看到 `-DMTRACE_COND=true`；
* **编辑器语法分析器** 看不到，除非你在 `.vscode/c_cpp_properties.json` 里显式补上。

所以：

* `autoconf.h` 解决的是 **配置生成**；
* `Makefile -D` 解决的是 **真实编译表达式**；
* VS Code `defines` 解决的是 **编辑器静态分析**。

三者不是重复，而是各司其职。

---

<a id="sec-mtrace-07"></a>
### 7. 第三步：为什么插桩点必须放在 `paddr_read()` / `paddr_write()`？

#### 7.1 不能放在更上层吗？
如果把 `mtrace` 放在 ISA 执行层，每条指令都要自己判断“这次是不是访存、访问了哪里、长度是多少”，会立刻出现两个问题：

1. 代码重复：每条 load/store 指令都要分别写日志逻辑；
2. 不完整：设备访问、未来扩展的访存路径不一定都能被统一覆盖。

#### 7.2 `paddr_read()` / `paddr_write()` 是什么角色？
这两个函数是 **物理内存访问的统一入口**。

也就是说，无论上层是哪条指令，最终只要真的发生了物理读写，都会来到这里。

因此把 `mtrace` 插在这里，等价于把摄像头装在了“总闸门”上：

* 不用关心是哪条具体指令；
* 只关心发生了什么访存行为；
* 可以统一拿到 `addr`、`len`、`data`。

#### 7.3 最终实现方式
在 `nemu/src/memory/paddr.c` 中：

```c
#ifdef CONFIG_MTRACE
  if (MTRACE_COND) {
    mtrace_write('R', addr, len, pr_data);
  }
#endif
```

以及写路径：

```c
#ifdef CONFIG_MTRACE
  if (MTRACE_COND) {
    mtrace_write('W', addr, len, data);
  }
#endif
```

这里还踩过一个非常典型的 C 语言坑：

* 错误写法：`mtrace_write("R", ...)`
* 正确写法：`mtrace_write('R', ...)`

原因是：

* `"R"` 是字符串，类型接近 `char *`；
* `'R'` 才是单个字符，类型是 `char`/`int` 兼容常量。

---

<a id="sec-mtrace-08"></a>
### 8. 第四步：输出层怎么设计？

#### 8.1 为什么单独放到 `log.c`？
因为它本身就是“日志输出后端”的一部分。

既然已有：

* `FILE *log_fp`
* `init_log()`

那么最自然的拓展方式就是新增：

* `FILE *mtrace_fp`
* `init_mtrace_log(const char *mtrace_log_file)`
* `mtrace_write(char type, paddr_t addr, int len, word_t data)`

#### 8.2 `mtrace_write()` 写什么内容？
最终选取的字段是：

* 当前 `pc`
* 读/写类型 `R` 或 `W`
* 物理地址 `addr`
* 长度 `len`
* 数据 `data`

所以每条日志长成这样：

```text
pc=0x80000138 W addr=0x80008ffc len=4 data=0x80000010
```

这条信息的价值很高：

* 能知道是哪条指令引发的访存；
* 能知道访问了哪段地址；
* 能知道读写宽度；
* 能知道具体数据内容。

#### 8.3 为什么在每次写后 `fflush()`？
因为 `mtrace` 的主要用途是调试，而不是离线高性能批处理。

调试时更重要的是：

* 程序一旦中途崩溃，日志尽量不要丢；
* 每一条访存尽快落盘，便于边跑边看。

所以这里选择了更偏调试友好的策略。

---

<a id="sec-mtrace-09"></a>
### 9. 第五步：路径为什么成了整个 `mtrace` 设计里最关键的部分？

因为日志“写不写得出来”和“写到哪里”，是两件不同的事。

前面的配置、插桩、输出逻辑即使都正确，如果文件路径选错了，`mtrace` 在真实项目里依然是不完整的。

#### 9.1 最开始的错误思路：写死相对路径
最初的实现是：

```c
fopen("build/mtrace-log.txt", "w");
```

这看似简单，但本质上只对一种特殊场景可靠：

* 当前工作目录恰好就是你想要的工程目录；
* 并且这个目录下恰好有 `build/`。

这对 NEMU 自己的 built-in image 可能凑巧能用，但对 `am-kernels/tests/*.c` 的真实运行链路就不稳了。

#### 9.2 为什么这会出问题？
因为 `fopen("build/mtrace-log.txt", "w")` 里的 `build/` 是相对于 **进程当前工作目录** 的，而不是相对于：

* `img_file`
* 测试程序源码目录
* 目标镜像目录
* Makefile 所在目录

换句话说，`fopen()` 根本不知道你心里想的是哪一个 `build/`，它只认当前 `cwd`。

这就导致一个严重问题：

> **代码逻辑看起来写得没错，但路径语义完全依赖“你从哪个目录启动 NEMU”。**

这种实现对教学实验来说太脆弱了。

---

<a id="sec-mtrace-10"></a>
### 10. 第六步：真正应该依赖什么来推导 `mtrace` 的路径？

答案不是“当前目录”，而是：

> **依赖 `make ... run` 已经构建好的那条参数传递链。**

#### 10.1 现有运行脚本已经做了什么？
在 `abstract-machine/scripts/platform/nemu.mk` 中：

```makefile
NEMUFLAGS += -b -l $(shell dirname $(IMAGE).elf)/nemu-log.txt
...
run: insert-arg
	$(MAKE) -C $(NEMU_HOME) ISA=$(ISA) run ARGS="$(NEMUFLAGS)" IMG=$(IMAGE).bin
```

这说明两件事：

1. `run` 脚本已经知道测试产物在哪个 `build/` 目录；
2. 它已经把这个目录封装进 `-l .../nemu-log.txt` 传给 NEMU 了。

也就是说：

* `nemu-log.txt` 的路径不是随便猜的；
* 它是 **构建系统已经算好的结果**。

#### 10.2 所以最合理的 `mtrace` 路径策略是什么？
直接复用这条已经正确存在的路径链。

也就是：

* 优先跟随 `log_file` 的目录；
* 如果没有 `log_file`，再退回到 `img_file` 所在目录；
* 如果两者都没有，才回退到默认相对路径。

这比简单写死 `build/mtrace-log.txt` 强得多，因为它依赖的是“运行脚本明确告诉 NEMU 的路径信息”，而不是模糊的当前目录。

---

<a id="sec-mtrace-11"></a>
### 11. 第七步：路径最终是怎么实现的？

在 `monitor.c` 中增加：

```c
static char mtrace_log_file[PATH_MAX] = {};

static const char *get_mtrace_log_file() {
  const char *path = log_file != NULL ? log_file : img_file;
  if (path == NULL) {
    return "build/mtrace-log.txt";
  }

  const char *slash = strrchr(path, '/');
  if (slash == NULL) {
    return "mtrace-log.txt";
  }

  size_t dir_len = slash - path + 1;
  int ret = snprintf(mtrace_log_file, sizeof(mtrace_log_file),
                     "%.*smtrace-log.txt", (int)dir_len, path);
  Assert(ret > 0 && ret < sizeof(mtrace_log_file),
         "mtrace log path is too long: %s", path);
  return mtrace_log_file;
}
```

然后在初始化阶段：

```c
init_log(log_file);
IFDEF(CONFIG_MTRACE, init_mtrace_log(get_mtrace_log_file()));
```

#### 11.1 这段代码在做什么？
它不是自己生造目录，而是做下面这件事：

* 若 `log_file=/.../build/nemu-log.txt`
* 就提取目录 `/.../build/`
* 再拼成 `/.../build/mtrace-log.txt`

也就是说，`mtrace` 和 `log` 共享同一个“由构建系统算好的输出目录”，只是文件名不同。

#### 11.2 为什么优先取 `log_file`，而不是 `img_file`？
因为 `log_file` 更直接体现“这次运行希望把日志写到哪里”。

* `img_file` 只是镜像位置；
* `log_file` 是运行脚本明确指定的日志输出位置。

如果已有 `-l`，那它就是比镜像路径更强的语义来源。

#### 11.3 为什么还保留 `img_file` 作为后备？
因为某些运行场景下可能没有显式传 `-l`，但仍然传了镜像路径。这时至少还能把 `mtrace-log.txt` 放在镜像同目录下，而不是完全失去落点。

#### 11.4 为什么还要有最后的默认值？
为了兼容 built-in image 或特殊启动方式：

* 没传 `-l`
* 也没传外部镜像

那么只能退回到相对路径默认值，这是兜底策略，而不是主路径策略。

---

<a id="sec-mtrace-12"></a>
### 12. 第八步：设计过程中踩到的几个关键坑

#### 12.1 `MTRACE_COND` 未定义
**现象**：编译 `paddr.c` 时提示 `MTRACE_COND` 未声明。

**根因**：虽然 `CONFIG_MTRACE_COND` 已经出现在 `autoconf.h` 中，但 Makefile 没把它转换成 `-DMTRACE_COND=...` 注入给编译器。

**解决思路**：检查实际编译命令，而不是只看 `autoconf.h`。最终在 `nemu/Makefile` 中补齐 `CFLAGS_TRACE += -DMTRACE_COND=...`。

#### 12.2 `mtrace_write("R", ...)` 类型错误
**现象**：编译器提示参数类型不匹配。

**根因**：`"R"` 是字符串，不是单字符。

**解决思路**：改成 `'R'` / `'W'`。

#### 12.3 `utils.h` 声明放在 include guard 外面
**现象**：结构上不规范，可能带来重复声明风险。

**解决思路**：把 `mtrace` 相关声明放回 `#ifndef __UTILS_H__` 与 `#endif` 之间。

#### 12.4 把“编辑器报错”和“真实构建错误”混为一谈
**现象**：VS Code 里提示宏未定义，但有时真实构建未必错；有时真实构建也确实错。

**解决思路**：必须分三层看：

1. `autoconf.h` 是否生成；
2. Makefile 是否真的把 `-D` 宏带进编译命令；
3. IntelliSense 是否手动补了 `defines`。

---

<a id="sec-mtrace-13"></a>
### 13. 第九步：最终验证链是怎样闭环的？

最后用真实命令验证：

```bash
cd /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests
make ARCH=riscv32-nemu ALL=string run
```

观察到运行输出中明确出现：

```text
Log is written to /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/nemu-log.txt
Mtrace log is written to /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/mtrace-log.txt
```

并且测试通过：

```text
[        string] PASS
```

再读取文件内容，确实存在大批量访存记录，例如：

```text
pc=0x80000138 W addr=0x80008ffc len=4 data=0x80000010
```

这说明三条链都通了：

* 配置链通了；
* 插桩链通了；
* 路径链也通了。

---

<a id="sec-mtrace-14"></a>
### 14. 这次 `mtrace` 设计里最核心的“为什么”总结

#### 14.1 为什么不能只做“能打印”就算完成？
因为调试工具最重要的不只是功能存在，而是它在真实工作流里是否可靠可用。

#### 14.2 为什么路径处理反而成了关键？
因为 `mtrace` 是“实验运行链路的一部分”，不是单独跑的小 demo。它必须跟着：

* `am-kernels/tests/*.c`
* `abstract-machine`
* `NEMU run`

整条流水线一起工作。

只要路径依赖当前工作目录而不是依赖真实脚本参数，功能就是脆弱的。

#### 14.3 为什么路径要“跟随构建系统”，而不是“在代码里猜目录”？
因为构建系统最清楚最终产物在哪。

代码层最稳妥的策略不是重新发明目录规则，而是复用已有的、已经被 `run` 脚本算好的路径信息。

#### 14.4 为什么优先复用 `-l` 这条链？
因为 `-l` 本质上已经是“日志输出目录的官方答案”。

既然 `nemu-log.txt` 能被稳定放到正确的 `build/` 里，那么 `mtrace-log.txt` 跟随它，就是最低风险、最一致的做法。

---

<a id="sec-mtrace-15"></a>
### 15. 最终方案一句话收束

> **`mtrace` 的正确实现，不只是“在 `paddr_read()` / `paddr_write()` 里打印一下”，而是要把“配置开关、条件过滤、统一插桩、独立日志文件、以及跟随 `make ... run` 真实产物目录的路径推导”整个闭环全部接通；其中路径处理的核心原则是：优先复用构建系统已经算好的 `log_file` 目录，其次退回 `img_file` 目录，最后才使用默认相对路径兜底。**


---

<a id="sec-mtrace-16"></a>
### 16. mtrace路径专题：从 `make ARCH=riscv32-nemu ALL=string run` 到 `mtrace-log.txt` 的完整路径形成链

前面已经解释了 `mtrace` 的路径策略为什么要“优先复用 `log_file` 目录”，这里再把**整条真实调用链**按时间顺序完整串起来。

这一节要回答的核心问题是：

> `monitor.c` 里的 `log_file` 到底从哪里来？`mtrace-log.txt` 又是怎么从 Makefile 一路传到 `fopen()` 的？

#### 16.1 先看一眼总链路

把完整流程压缩成一行，就是：

```text
make ARCH=riscv32-nemu ALL=string run
-> am-kernels/tests/cpu-tests/Makefile
-> 临时 Makefile.string
-> abstract-machine/Makefile
-> abstract-machine/scripts/platform/nemu.mk
-> make -C $(NEMU_HOME) run ARGS="-b -l .../nemu-log.txt" IMG=...bin
-> nemu/scripts/native.mk
-> NEMU 可执行文件 argv[]
-> monitor.c: parse_args()
-> log_file = optarg
-> get_mtrace_log_file()
-> init_mtrace_log(...)
-> fopen(".../mtrace-log.txt", "w")
```

也就是说，`mtrace-log.txt` 的路径不是在 `log.c` 里凭空创建的，而是沿着 **构建系统先算出 `nemu-log.txt` 路径，再在 `monitor.c` 中派生同目录下的 `mtrace-log.txt`** 这条链形成的。

---

#### 16.2 第一站：`cpu-tests/Makefile` 先把单个测试委托给 `abstract-machine/Makefile`

假设你在目录：

```bash
cd /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests
make ARCH=riscv32-nemu ALL=string run
```

先进入的是：

* `am-kernels/tests/cpu-tests/Makefile`

其中关键规则是：

```makefile
Makefile.%: tests/%.c latest
@/bin/echo -e "NAME = $*\nSRCS = $<\ninclude $${AM_HOME}/Makefile" > $@
@if make -s -f $@ ARCH=$(ARCH) $(MAKECMDGOALS); then \
```

当目标是 `string` 时，这里会：

1. 生成临时文件 `Makefile.string`；
2. 内容大致是：

```makefile
NAME = string
SRCS = tests/string.c
include ${AM_HOME}/Makefile
```

3. 再执行：

```bash
make -s -f Makefile.string ARCH=riscv32-nemu run
```

因此，这一层的作用是：

* 先把“本次要跑的测试是谁”固定下来；
* 再把真正的构建/运行逻辑转交给 `abstract-machine/Makefile`。

它自己并不直接生成 `mtrace-log.txt`，而是把场景信息继续往下传。

---

#### 16.3 第二站：`abstract-machine/Makefile` 先算出当前测试产物的根路径

进入 `abstract-machine/Makefile` 后，首先会计算：

```makefile
WORK_DIR  = $(shell pwd)
DST_DIR   = $(WORK_DIR)/build/$(ARCH)
IMAGE_REL = build/$(NAME)-$(ARCH)
IMAGE     = $(abspath $(IMAGE_REL))
```

如果当前目录就是 `am-kernels/tests/cpu-tests`，且：

* `NAME = string`
* `ARCH = riscv32-nemu`

那么这些变量就会变成：

```text
WORK_DIR  = /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests
DST_DIR   = /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/riscv32-nemu
IMAGE_REL = build/string-riscv32-nemu
IMAGE     = /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/string-riscv32-nemu
```

这里的 `IMAGE` 很重要，因为后面：

* `$(IMAGE).elf` 是 ELF 路径；
* `$(IMAGE).bin` 是交给 NEMU 运行的镜像路径；
* `nemu-log.txt` 的目录就是围绕这个 `IMAGE` 推出来的。

也就是说，从这一步开始，构建系统已经知道“本次测试对应哪个 `build/` 目录”。

---

#### 16.4 第三站：`platform/nemu.mk` 第一次显式拼出 `nemu-log.txt` 路径

随后会进入：

* `abstract-machine/scripts/platform/nemu.mk`

这里最关键的一行是：

```makefile
NEMUFLAGS += -b -l $(shell dirname $(IMAGE).elf)/nemu-log.txt
```

这一行必须逐段拆开理解。

##### 16.4.1 `$(IMAGE).elf` 是什么？

它就是上一步得到的 ELF 路径，例如：

```text
/home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/string-riscv32-nemu.elf
```

##### 16.4.2 `$(shell dirname $(IMAGE).elf)` 做了什么？

它提取这个 ELF 所在目录，得到：

```text
/home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build
```

##### 16.4.3 再拼上 `/nemu-log.txt` 后变成什么？

得到：

```text
/home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/nemu-log.txt
```

##### 16.4.4 所以 `NEMUFLAGS` 最终是什么？

类似于：

```text
-b -l /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/nemu-log.txt
```

这一步的意义非常大：

* `nemu-log.txt` 的位置不是运行时现猜的；
* 它是 **构建系统在知道镜像产物目录之后，显式拼出来的**。

---

#### 16.5 第四站：`platform/nemu.mk` 把 `ARGS` 和 `IMG` 一起传给 NEMU

同一个文件还有：

```makefile
run: insert-arg
$(MAKE) -C $(NEMU_HOME) ISA=$(ISA) run ARGS="$(NEMUFLAGS)" IMG=$(IMAGE).bin
```

它展开后，本质上类似于执行：

```bash
make -C /home/l/ysyx/ysyx-workbench/nemu \
  ISA=riscv32 \
  run \
  ARGS="-b -l /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/nemu-log.txt" \
  IMG=/home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/string-riscv32-nemu.bin
```

这里同时传下去两类路径信息：

* `ARGS`：给 NEMU 的命令行选项，其中包含 `-l .../nemu-log.txt`；
* `IMG`：给 NEMU 的镜像路径，对应 `.../string-riscv32-nemu.bin`。

后面在 `monitor.c` 中，这两者会分别落到：

* `log_file`
* `img_file`

这也正好对应 `get_mtrace_log_file()` 的两个候选来源。

---

#### 16.6 第五站：`nemu/scripts/native.mk` 把 Make 变量组装成真正的执行命令

进入 NEMU 顶层 Makefile 后，会继续包含：

* `nemu/scripts/native.mk`

其中关键代码是：

```makefile
override ARGS ?= --log=$(BUILD_DIR)/nemu-log.txt
override ARGS += $(ARGS_DIFF)

IMG ?=
NEMU_EXEC := $(BINARY) $(ARGS) $(IMG)

run: run-env
$(NEMU_EXEC)
```

这里要特别注意：

##### 16.6.1 为什么默认 `--log=$(BUILD_DIR)/nemu-log.txt` 这次没有成为最终答案？

因为外层 `platform/nemu.mk` 已经显式传入了：

```text
ARGS="-b -l /.../cpu-tests/build/nemu-log.txt"
```

而 `?=` 的含义是“只有没有外部赋值时才使用默认值”。

所以本次真正生效的日志路径，不是 `native.mk` 的默认值，而是**来自测试镜像目录那条外层传入路径**。

##### 16.6.2 最终运行命令长什么样？

本质上相当于：

```bash
/home/l/ysyx/ysyx-workbench/nemu/build/riscv32-nemu-interpreter \
  -b \
  -l /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/nemu-log.txt \
  /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/string-riscv32-nemu.bin
```

从这里开始，Makefile 中的变量就已经变成 NEMU 进程实际收到的 `argv[]` 内容了。

---

#### 16.7 第六站：`nemu-main.c` 把 `argv[]` 交给 `init_monitor()`

NEMU 程序入口在：

* `nemu/src/nemu-main.c`

核心代码是：

```c
int main(int argc, char *argv[]) {
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif

  engine_start();
  return is_exit_status_bad();
}
```

在你当前这种运行路径下，会走：

```c
init_monitor(argc, argv);
```

这意味着：

* `-b`
* `-l /.../nemu-log.txt`
* `.../string-riscv32-nemu.bin`

这些参数，全部被原样带进 `monitor.c`。

---

#### 16.8 第七站：`parse_args()` 把参数拆成 `log_file` 和 `img_file`

在 `nemu/src/monitor/monitor.c` 中，先有这些静态变量：

```c
static char *log_file = NULL;
static char *diff_so_file = NULL;
static char *img_file = NULL;
static int difftest_port = 1234;
static char mtrace_log_file[260] = {};
```

然后在 `init_monitor()` 一开始调用：

```c
parse_args(argc, argv);
```

而 `parse_args()` 的关键部分是：

```c
while ((o = getopt_long(argc, argv, "-bhl:d:p:", table, NULL)) != -1) {
  switch (o) {
    case 'b': sdb_set_batch_mode(); break;
    case 'p': sscanf(optarg, "%d", &difftest_port); break;
    case 'l': log_file = optarg; break;
    case 'd': diff_so_file = optarg; break;
    case 1: img_file = optarg; return 0;
  }
}
```


```c
case 'l': log_file = optarg; break;
case 1: img_file = optarg; return 0;
```

这表示：

* `-l` 后面的那个路径字符串，会直接写进 `log_file`；
* 最后那个普通参数 `.bin` 镜像路径，会写进 `img_file`。

因此，当前场景下它们会变成：

```text
log_file = /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/nemu-log.txt
img_file = /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/string-riscv32-nemu.bin
```

到这里，`monitor.c` 已经拿到了后续推导 `mtrace-log.txt` 所需的全部路径信息。

---

#### 16.9 第八站：`init_log(log_file)` 先直接打开普通日志

在 `init_monitor()` 中，接着会调用：

```c
init_log(log_file);
IFDEF(CONFIG_MTRACE, init_mtrace_log(get_mtrace_log_file()));
```

先看第一句。

在 `nemu/src/utils/log.c` 中：

```c
void init_log(const char *log_file) {
  log_fp = stdout;
  if (log_file != NULL) {
    FILE *fp = fopen(log_file, "w");
    Assert(fp, "Can not open '%s'", log_file);
    log_fp = fp;
  }
  Log("Log is written to %s", log_file ? log_file : "stdout");
}
```

这条链很直接：

* `parse_args()` 解析出的 `log_file`
* 作为参数传给 `init_log()`
* 最终被 `fopen(log_file, "w")` 打开

所以 `nemu-log.txt` 的来源到这里就完全闭环了。

---

#### 16.10 第九站：`get_mtrace_log_file()` 基于 `log_file` 目录派生 `mtrace-log.txt`

真正和 `mtrace` 相关的关键是下面这段：

```c
static const char *get_mtrace_log_file() {
  const char *path = log_file != NULL ? log_file : img_file;
  if (path == NULL) {
    return "build/mtrace-log.txt";
  }

  const char *slash = strrchr(path, '/');
  if (slash == NULL) {
    return "mtrace-log.txt";
  }

  size_t dir_len = slash - path + 1;
  int ret = snprintf(mtrace_log_file, sizeof(mtrace_log_file),
                     "%.*smtrace-log.txt", (int)dir_len, path);
  Assert(ret > 0 && ret < sizeof(mtrace_log_file),
         "mtrace log path is too long: %s", path);
  return mtrace_log_file;
}
```

这段逻辑可以拆成四步。

##### 16.10.1 第一步：优先选择 `log_file`

```c
const char *path = log_file != NULL ? log_file : img_file;
```

这意味着：

* 只要命令行传了 `-l`，就优先用 `log_file`；
* 只有 `log_file` 不存在时，才退回 `img_file`。

所以在 `make ... run` 的正常链路里，`path` 实际上就是：

```text
/home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/nemu-log.txt
```

##### 16.10.2 第二步：找到最后一个 `/`

```c
const char *slash = strrchr(path, '/');
```

它定位到路径中最后一个 `/`，也就是目录和文件名之间的分隔点。

##### 16.10.3 第三步：截取目录部分

```c
size_t dir_len = slash - path + 1;
```

这样得到的就是：

```text
/home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/
```

这里故意保留最后的 `/`，是为了后面直接拼接文件名。

##### 16.10.4 第四步：改名拼成 `mtrace-log.txt`

```c
snprintf(mtrace_log_file, sizeof(mtrace_log_file),
         "%.*smtrace-log.txt", (int)dir_len, path);
```

于是最终得到：

```text
/home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/mtrace-log.txt
```

这就是 `mtrace-log.txt` 路径真正被构造出来的时刻。

换句话说：

* Makefile 直接传进来的只有 `nemu-log.txt`；
* `mtrace-log.txt` 是在 `monitor.c` 中，以 `log_file` 所在目录为基准派生出来的。

---

#### 16.11 第十站：`init_mtrace_log()` 最终把这个派生路径交给 `fopen()`

回到 `init_monitor()`：

```c
IFDEF(CONFIG_MTRACE, init_mtrace_log(get_mtrace_log_file()));
```

在 `nemu/src/utils/log.c` 中：

```c
void init_mtrace_log(const char *mtrace_log_file) {
  mtrace_fp = fopen(mtrace_log_file, "w");
  Assert(mtrace_fp, "Can not open '%s'", mtrace_log_file);
  Log("Mtrace log is written to %s", mtrace_log_file);
}
```

所以最后一跳就是：

```text
get_mtrace_log_file()
-> 返回 /.../cpu-tests/build/mtrace-log.txt
-> init_mtrace_log(...)
-> fopen("/.../cpu-tests/build/mtrace-log.txt", "w")
```

到这里，整条路径链才真正闭环。

---

#### 16.12 为什么这条链比 `fopen("build/mtrace-log.txt", "w")` 稳得多？

因为后者依赖的是“当前工作目录”，而前者依赖的是“构建系统显式传下来的产物路径”。

如果直接写：

```c
fopen("build/mtrace-log.txt", "w");
```

那么这个 `build/` 指向哪里，取决于：

* 你是从 `nemu/` 目录直接运行；
* 还是从 `am-kernels/tests/cpu-tests/` 间接触发；
* 当前 shell 的 `cwd` 恰好是什么。

这就会把路径语义绑定到一个非常脆弱的隐式条件上。

而现在这条链依赖的是：

* `abstract-machine/Makefile` 算出的 `IMAGE`
* `platform/nemu.mk` 算出的 `-l .../nemu-log.txt`
* `monitor.c` 中解析得到的 `log_file`

这些都属于显式、可追溯、可解释的参数传递结果。

因此它更稳，也更适合实验框架里的长期使用。

---

#### 16.13 最后再压缩成一句话

`mtrace-log.txt` 的路径不是在 `monitor.c` 里“凭感觉猜出来”的，而是按下面这条逻辑生成的：

1. `cpu-tests/Makefile` 指定测试名并委托给 `abstract-machine/Makefile`；
2. `abstract-machine/Makefile` 算出当前测试的 `IMAGE` 路径；
3. `platform/nemu.mk` 用 `dirname $(IMAGE).elf` 构造 `-l .../nemu-log.txt`；
4. 这条 `-l` 通过 `ARGS` 传给 NEMU；
5. `monitor.c::parse_args()` 把它写进 `log_file`；
6. `get_mtrace_log_file()` 取出 `log_file` 所在目录并改名为 `mtrace-log.txt`；
7. `init_mtrace_log()` 最后调用 `fopen()` 真正创建文件。

所以这条链最核心的原则就是：

> **先让构建系统决定“日志该写到哪个产物目录”，再让 `monitor.c` 复用这条已经正确的路径链派生 `mtrace-log.txt`，而不是在 C 代码里重新猜一个 `build/`。**


---

<a id="sec-pa2-ftrace-fullchain"></a>
## PA2 专题：Ftrace 从 0 到 1 全链路设计笔记


本文基于当前工程中的 ftrace 相关实现，按真实执行路径整理一条完整链路：
构建系统传参 -> monitor 初始化 -> ELF 静态解析 -> 运行期指令 Hook -> 调用栈日志输出。

目标是回答 5 个问题：
1. ftrace 的输入从哪里来，何时初始化。
2. ELF 三类关键表如何协作得到函数地址与函数名。
3. 为什么在 jal/jalr 译码点触发，而不是其他地方。
4. call/ret 记录如何保证层级与函数名对应。
5. ELF 文件里的静态字节，如何映射到 NEMU 进程中的软件内存结构。

---

<a id="sec-ftrace-01"></a>
## 1. 全局目标与最小闭环

ftrace 的最小闭环是：

1. 在启动前拿到 ELF 路径。
2. 启动时解析 ELF，构建 函数地址区间 -> 函数名 的查询表。
3. 运行时在 call/ret 指令处触发记录。
4. 输出带缩进层级的日志。

这四步缺一不可：
- 没有 ELF，运行时只有地址没有函数名。
- 没有运行期 Hook，只有静态符号表不会产生调用序列。
- 没有调用栈，ret 无法和之前 call 对齐。

---

<a id="sec-ftrace-02"></a>
## 2. 构建与启动参数链路

### 2.1 构建侧注入 ELF 参数

在平台脚本 run 规则里，通过 ARGS 注入：
- --elf=$(IMAGE).elf

作用：把客户程序 ELF 的绝对路径传递给 NEMU 主程序，而不仅仅传 bin 镜像。

### 2.2 monitor 参数解析

monitor 的 getopt_long 选项表包含：
- 长选项 elf
- 短选项 e

解析后把 optarg 写入全局 elf_file。后续 init_monitor 中再决定是否初始化 ftrace。

结论：
- bin 用于执行装载。
- elf 用于符号信息解析。
- 两者职责不同，互补而非替代。

---

<a id="sec-ftrace-03"></a>
## 3. monitor 初始化阶段的 ftrace 生命周期

在 init_monitor 中，ftrace 相关生命周期是：

1. parse_args 完成后，得到 elf_file。
2. 若启用 CONFIG_FTRACE 且 elf_file 非空：
   - init_ftrace_log: 打开日志文件。
   - init_ftrace: 解析 ELF，构建函数表。
3. 若启用 CONFIG_FTRACE 但缺失 elf：
   - 打印提示并跳过初始化。

这是一种“可用则启用，不可用则降级”的容错策略。

---

<a id="sec-ftrace-04"></a>
## 4. ELF 静态解析主流程

核心目标：把 ELF 中的符号信息，转成运行期可快速查询的函数数组。

### 4.1 读取并校验 Elf32_Ehdr

校验点：
- 魔数 EI_MAG0..3
- EI_CLASS = ELFCLASS32
- EI_DATA = ELFDATA2LSB

作用：确认当前解析器处理的是目标格式，避免后续偏移解释错误。

### 4.2 读取 Section Header Table 到 shdrs 数组

依据：
- e_shoff: 节头表偏移
- e_shnum: 节头数量
- e_shentsize: 单项大小

读取结果：
- shdrs 是软件内存中的节描述数组，每个元素对应 ELF 的一个节。

### 4.3 用 e_shstrndx 定位 .shstrtab

说明：
- e_shstrndx 只告诉“节名字符串表”的索引。
- 它不直接告诉 .symtab/.strtab 的索引。

因此流程是：
1. 先用 e_shstrndx 读到 .shstrtab 的原始字符串池。
2. 再遍历 shdrs，通过 sh_name 索引解析出每个节名。
3. 在遍历中定位 .symtab 与 .strtab。

### 4.4 读取 .strtab 内容

- .strtab 存储符号名字符串池。
- sym.st_name 是该池中的字节偏移。
- 真实名字 = strtab_buf + sym.st_name。

### 4.5 遍历 .symtab 构建 func_symbols

计算条目数：
- sym_count = symtab.sh_size / symtab.sh_entsize

逐项读取 Elf32_Sym 并做卫语句过滤：
- 类型必须是 STT_FUNC。
- 不能是 SHN_UNDEF。
- 大小不能为 0。
- 名字偏移必须在 .strtab 边界内。
- func_symbols 不能越界。

通过后落表：
- start = st_value
- end = st_value + st_size
- name = .strtab 中的字符串

最终得到运行期查询表：
- func_symbols[i] = {start, end, name}

---

<a id="sec-ftrace-05"></a>
## 5. 关键结构体关系图

### 5.1 文件级结构体

- Elf32_Ehdr：总索引入口，给出节头表位置与规模。
- Elf32_Shdr：描述每个节的元信息（偏移、大小、名字索引等）。
- Elf32_Sym：.symtab 的单个条目（符号值、大小、类型、名字偏移）。

### 5.2 软件内存结构体

- FuncSymbol：ftrace 自定义函数条目，服务运行时快速匹配。
- CallFrame：ftrace 自定义调用栈帧，维护 ret 对应关系。


### 5.3 结构体映射关系

1. Elf32_Ehdr -> 告诉如何读 Elf32_Shdr[]。
2. Elf32_Shdr[] + .shstrtab -> 找到 .symtab/.strtab。
3. .symtab 中 Elf32_Sym.st_name -> 到 .strtab 取字符串。
4. Elf32_Sym -> 转译为 FuncSymbol。
5. 运行时 FuncSymbol + CallFrame -> 输出 call/ret 树形日志。

---

<a id="sec-ftrace-06"></a>
## 6. 运行期 Hook：为什么是 jal/jalr

RISC-V 中函数调用与返回没有独立新指令，本质都落在 jal/jalr 的组合语义上。

### 6.1 call 判定

当 rd == 1（x1/ra）时，表示指令把返回地址写入 ra，这是函数调用约定的关键特征。

因此在 jal/jalr 的执行体中：
- if rd == 1 -> 触发 ftrace_call。

### 6.2 ret 判定

标准 ret 伪指令展开为：
- jalr x0, 0(x1)

对应判定条件：
- rd == 0
- rs1_idx == 1
- imm == 0

满足时触发 ftrace_ret。

说明：
- 这里判断的是寄存器编号，不是寄存器内容值。

---

<a id="sec-ftrace-07"></a>
## 7. ftrace_call 与 ftrace_ret 的协作机制

### 7.1 ftrace_call

步骤：
1. 前置保护：日志文件存在且条件宏允许。
2. 通过目标地址查 FuncSymbol。
3. 按当前 call_depth 打印缩进与 call 行。
4. 把 ret_addr 与 func 指针压入 call_stack。
5. call_depth++。

### 7.2 ftrace_ret

步骤：
1. 前置保护同上。
2. 若 call_depth > 0，则先 depth--。
3. 读取对应栈帧函数名。
4. 打印 ret 行与返回目标地址。

### 7.3 为什么要软件调用栈

仅靠当前 PC 无法稳定恢复“从哪个函数返回”。
必须在 call 时保存上下文，在 ret 时回放上下文，才能得到正确层级与函数名。

---

<a id="sec-ftrace-08"></a>
## 8. 静态 ELF 与软件内存的关系

这个问题可以分三层理解：

### 8.1 ELF 文件是只读数据源

- 在 init_ftrace 阶段，fopen/fread/fseek 从磁盘读取 ELF 字节。
- 这些字节被解释为 Ehdr、Shdr、Sym 等标准结构。

### 8.2 NEMU 进程内临时解析内存

- shdrs、shstrtab、strtab_buf 都是解析时动态申请的临时缓冲。
- 这些缓冲在函数结束前被 free。

### 8.3 NEMU 进程内持久运行数据

- func_symbols 与 call_stack 是 ftrace 的持久状态。
- 解析阶段从 ELF 抽取“最小必要信息”后，只保留这两个运行期需要的容器。

### 8.4 与客户程序执行内存的关系

- 客户程序 bin 会被加载到 pmem（guest memory）执行。
- ftrace 不修改 pmem 指令，只在译码执行路径旁路观察 PC/dnpc。
- 本质是“旁路观测 + 元信息映射”，非侵入执行。

---

<a id="sec-ftrace-09"></a>
## 9. 设计分层与每层职责

### 层 A：构建与参数层

职责：把 ELF 路径与 bin 路径正确传给 NEMU。

### 层 B：启动初始化层

职责：解析参数，决定是否初始化 ftrace，创建日志文件。

### 层 C：静态解析层

职责：把 ELF 的符号体系转换为运行期可查询结构。

### 层 D：运行期 Hook 层

职责：在 jal/jalr 指令点识别 call/ret 事件。

### 层 E：日志与可视化层

职责：按调用深度输出可读日志，反映真实调用树。

分层价值：
- 易调试：每层可单独验证。
- 易扩展：可增加过滤策略或符号来源而不破坏其他层。
- 易降级：缺失 ELF 时可仅关闭 ftrace，不影响主执行。

---

<a id="sec-ftrace-10"></a>
## 10. 端到端时序总结

1. Make 侧 run 注入 --elf=xxx.elf 与 IMG=xxx.bin。
2. monitor parse_args 收到 elf_file/img_file。
3. init_monitor 在 CONFIG_FTRACE 下执行 init_ftrace_log + init_ftrace。
4. init_ftrace 解析 ELF：
   - Ehdr -> Shdr[] -> .shstrtab -> 找到 .symtab/.strtab -> 读取并过滤 Sym -> 构建 func_symbols。
5. 客户程序开始执行，译码器进入 jal/jalr 条目：
   - rd==1 触发 ftrace_call。
   - rd==0 && rs1==1 && imm==0 触发 ftrace_ret。
6. ftrace_call/ftrace_ret 维护 call_stack 与 call_depth，并持续写日志。

这就是 ftrace 从“静态元数据”走到“动态行为追踪”的完整闭环。

---

<a id="sec-ftrace-11"></a>
## 11. 常见坑与工程性检查点

1. 只传 bin 不传 elf：
   - 现象：可运行但无函数名追踪。
   - 处理：补 --elf 参数。

2. 把 e_shstrndx 误当成 .symtab 索引：
   - 错因：e_shstrndx 只指向节名字符串表。
   - 正解：遍历 shdrs 按节名匹配 .symtab/.strtab。

3. 未做边界检查：
   - 风险：st_name 越界导致非法访问。
   - 正解：保留卫语句过滤。

4. 错把寄存器索引判定当寄存器值判定：
   - 正解：rd/rs1_idx 是寄存器编号，不是寄存器内容。

5. 忽略调用栈深度上限：
   - 风险：深递归场景下数组越界。
   - 正解：MAX_CALL_DEPTH 上限保护。

<a id="sec-ftrace-stack"></a>
## 12. ftrace 出栈/入栈执行逻辑、问题来源与解决全总结

#### 12.1 问题来源与现象
在实现 ftrace 过程中，最常见的疑问是：
- **如何保证 call/ret 日志的缩进层级与真实函数调用树一致？**
- **为什么要在 call 时保存 FuncSymbol 指针，而不是只保存返回地址？**

#### 12.2 逻辑梳理与原理
1. **call 指令（jal/jalr rd==1）时：**
   - 通过目标地址查找对应的 FuncSymbol（即函数名、起止区间）。
   - 打印带缩进的 call 日志。
   - 将 {ret_addr, func} 作为 CallFrame 压入 call_stack[]。
   - call_depth++。
2. **ret 指令（jalr rd==0, rs1==1, imm==0）时：**
   - 先判断 call_depth>0，防止栈下溢。
   - call_depth--，弹出栈顶 CallFrame。
   - 直接用 frame->func->name 打印 ret 日志，保证与 call 时函数名一一对应。

#### 12.3 为什么不能只保存返回地址？
因为同一个返回地址可能属于不同函数（如递归/多分支），只有保存 call 时查到的 FuncSymbol 指针，才能保证 ret 时准确还原“从哪个函数返回”。

#### 12.4 典型问题与解决
- **问题：只保存返回地址，ret 时如何还原函数名？**
  - 解决：call 时就查好 FuncSymbol 指针，ret 时直接用，无需再查。
- **问题：递归/多分支下如何保证层级？**
  - 解决：call_stack[] + call_depth 形成 LIFO 结构，天然支持嵌套与递归。
- **问题：栈溢出/下溢？**
  - 解决：MAX_CALL_DEPTH 上限保护，ret 前判断 call_depth>0。

#### 12.5 总结一句话
> ftrace 的 call/ret 日志层级完全依赖于“每次 call 时保存的 {ret_addr, func}”，只有这样才能保证任意复杂调用树下的日志与真实执行路径一一对应。

---

<a id="sec-ftrace-symbol-match"></a>
## 13. 目标函数匹配问题的引出与逻辑总结

#### 13.1 问题引出
ftrace 运行时如何通过目标地址（tar_addr）找到对应的函数名？为什么只要满足 start <= addr < end 就能唯一确定？

#### 13.2 ELF 符号表的本质
- 每个函数在 ELF .symtab 里有唯一的 st_value（起始地址）和 st_size（长度）。
- 编译/链接时，所有函数的地址区间不会重叠。
- ftrace 启动时将所有 STT_FUNC 类型的符号转为 {start, end, name} 数组。

#### 13.3 匹配原理
1. 运行时遇到 call 指令，得到目标地址 tar_addr。
2. 遍历 func_symbols[]，查找满足 start <= tar_addr < end 的条目。
3. 若找到，说明 tar_addr 落在该函数区间内，直接返回 name。
4. 若找不到，说明该地址不属于任何已知函数（如裸跳转/异常）。

#### 13.4 为什么区间匹配一定成立？
- 链接器保证每个函数的 [start, end) 区间互不重叠。
- tar_addr 必然落在某个函数体内（只要是合法 call）。
- 只要区间查找实现无误，匹配结果唯一。

#### 13.5 典型问题与解决
- **问题：如果 st_size=0 怎么办？**
  - 解决：解析时过滤掉 size=0 的符号。
- **问题：目标地址正好等于 end？**
  - 解决：区间采用左闭右开 [start, end)，end 不属于该函数。
- **问题：有重叠区间怎么办？**
  - 解决：链接器不会生成重叠函数区间，若有则为 ELF 错误。

#### 13.6 总结一句话
> ftrace 通过 ELF 符号表的唯一性，将目标地址映射为函数名，区间匹配 [start, end) 是链接器保证的唯一分区，保证了运行时任意 call/ret 都能准确还原函数名。

---

<a id="sec-pa2-klib-tests"></a>
## PA2 专题：klib 测试全链路设计、Debug 与闭环验证

[返回目录](#sec-pa2-toc)

<a id="sec-klibtest-01"></a>
### 1. 为什么需要单独设计 `klib-tests`？

在 `PA2` 里，`string` 等已有客户程序虽然会间接调用 `klib`，但它们并不适合作为系统性的 `klib` 专项测试，原因主要有三点：

1. **覆盖面太窄**  
   现成测试往往只顺手调用了少量函数，难以覆盖 `memset/strcpy/strncpy/strcat/memmove/strlen/strcmp/snprintf/atoi/rand` 这些不同语义的接口。

2. **oracle 不独立**  
   如果用一个尚未证明正确的库函数去验证另一个库函数，就会出现“错误互相掩盖”的风险。例如用 `memcmp()` 去验证 `memset()`，如果 `memcmp()` 自己也错了，测试就会失真。

3. **不能精准归因**  
   把多个函数串在一条语句里测试（如 `strcat(strcpy(...), ...)`）会让失败点模糊，不利于快速定位到底是哪一个库函数出了问题。

因此，`klib-tests` 的核心目标不是“随便跑一个客户程序看看能不能过”，而是：

> **为 `klib` 的不同类别函数建立独立、清晰、可复用的测试用例与判断标准，并分别在 `native` 和 `riscv32-nemu` 两种环境中验证它们。**

---

<a id="sec-klibtest-02"></a>
### 2. 直接测试对象、间接运行平台与最终验证目标

`klib-tests` 的关系链必须先分清四层：

1. **测试程序**
   - 位于 `am-kernels/tests/klib-tests/src/*.c`
   - 这些文件本身不是 `klib` 实现，而是“客户程序形式的测试代码”
   - 它们负责：
     - 调用 `klib` 函数
     - 构造输入
     - 用 `check()` 断言输出或返回值

2. **被测对象**
   - 位于 `abstract-machine/klib/src/*.c`
   - 真正被测的是这里的实现：
     - `string.c`
     - `stdio.c`
     - `stdlib.c`

3. **装配者**
   - `abstract-machine/Makefile`
   - 它负责把：
     - 测试程序源码
     - `am` 运行时
     - `klib`
     一起编译链接成可执行目标

4. **执行者**
   - `ARCH=native` 时：宿主机环境直接执行
   - `ARCH=riscv32-nemu` 时：NEMU 逐条执行生成的客户程序指令

因此，`klib-tests` 的准确定位是：

> **写一个 AM 客户程序去测试 `klib`；`native` 用来隔离验证测试逻辑与基础语义，`riscv32-nemu` 用来验证 `klib + AM + NEMU` 这一整条链。**

---

<a id="sec-klibtest-03"></a>
### 3. 测试分类为什么最终定成 `write/read/format/stdlib`？

最初容易想到的分法是按头文件来分，例如：

- `string`
- `stdio`
- `stdlib`

但从测试设计角度看，更合理的主结构其实是按**测试方法**和**oracle 构造方式**来分。

最终形成的四类是：

1. **`write`**
   - 代表函数：
     - `memset`
     - `strcpy`
     - `strncpy`
     - `strcat`
     - `memmove`
   - 关注点：
     - 返回值是否为原始目标指针
     - 目标缓冲区最终布局是否正确
     - 未覆盖区域是否保持原值
     - 是否正确处理边界与重叠

2. **`read`**
   - 代表函数：
     - `strlen`
     - `strcmp`
     - `strncmp`
     - `memcmp`
   - 关注点：
     - 返回值是否正确
     - 比较关系是否正确（`<0` / `==0` / `>0`）
     - `n=0`、中间 `'\0'`、前缀相同但后续不同等边界语义

3. **`format`**
   - 代表函数：
     - `sprintf`
     - `snprintf`
   - 关注点：
     - 输出字符串内容
     - 返回值
     - 截断行为
     - `'\0'` 终止

4. **`stdlib`**
   - 代表函数：
     - `abs`
     - `atoi`
     - `srand`
     - `rand`
   - 关注点：
     - 数学语义
     - 字符串解析规则
     - 随机序列的确定性

这种划分的优点是：

- 每类函数的测试方法更统一；
- 每个 `.c` 文件职责更单纯；
- 失败时更容易从“类别 -> 函数 -> case”逐级定位。

---

<a id="sec-klibtest-04"></a>
### 4. 项目结构的演化：为什么最后采用 `cpu-tests` 风格？

`klib-tests` 的设计经历了一个关键演化过程。

#### 4.1 初始思路：一个总入口 + 多个分类模块

最开始容易想到的结构是：

- 一个总 `main()`
- `write/read/format/stdlib` 各自提供一个入口函数
- 运行时通过参数选择执行哪个分类

这种方案的优点是直观，但有两个明显问题：

1. **与 `cpu-tests` 的组织方式不一致**
2. **所有分类 `.c` 会一起编进同一个程序**
   - 公共头文件里的普通函数定义更容易引发 `multiple definition`
   - Makefile 上的“单测/全测”选择不够干净

#### 4.2 最终思路：每个分类 `.c` 都是一个独立测试程序

最终结构改成：

- `src/write.c`
- `src/read.c`
- `src/format.c`
- `src/stdlib.c`

每个文件都：

- 自己包含测试函数
- 自己有 `main()`
- 成功时 `return 0`
- 失败时由 `check()` -> `assert_fail()` -> `halt(1)` 终止

这样做的好处是：

1. 和 `cpu-tests` 的“每个测试项对应一个独立程序”完全同构
2. Makefile 只需要在外层决定“这次编哪一个 `.c`”
3. 不再需要中央分发 `mainargs`
4. 每个测试项天然可以单独执行：
   - `make ARCH=native ALL=write run`
   - `make ARCH=riscv32-nemu ALL=format run`

---

<a id="sec-klibtest-05"></a>
### 5. `Makefile` 的核心逻辑：谁负责筛选测试，谁负责真正编译？

`klib-tests` 最终采用的是和 `cpu-tests/Makefile` 同源的设计思想：

> **测试筛选发生在外层 `klib-tests/Makefile`，真正的编译链接执行仍然交给 `abstract-machine/Makefile`。**

#### 5.1 `abstract-machine/Makefile` 不负责筛选测试项

这是整个理解里最容易混淆的一点。

`abstract-machine/Makefile` 的职责是：

- 编译 `SRCS`
- 链接 `am` 与 `klib`
- 生成镜像
- 执行 `run/gdb/clean`

它不会理解：

- `ALL=write`
- `ALL=format`
- 默认跑全部还是单个测试

它只会“老老实实编译传给它的 `SRCS`”。

#### 5.2 真正的测试筛选发生在外层 `Makefile`

`klib-tests/Makefile` 的关键思路是：

```make
ALL = $(basename $(notdir $(shell find src/. -name "*.c")))
all: $(addprefix Makefile., $(ALL))

Makefile.%: src/%.c latest
	@/bin/echo -e "NAME = $*\nSRCS = $<\nINC_PATH += .../include\ninclude $${AM_HOME}/Makefile" > $@
	@if make -s -f $@ ARCH=$(ARCH) $(MAKECMDGOALS); then ...; fi
```

它做了三件核心事情：

1. **收集测试项名**
   - 从 `src/*.c` 推导出：
     - `write`
     - `read`
     - `format`
     - `stdlib`

2. **为每个测试项生成一个临时 wrapper Makefile**
   - 例如 `Makefile.write`
   - 内容类似：
     ```make
     NAME = write
     SRCS = src/write.c
     INC_PATH += /.../include
     include ${AM_HOME}/Makefile
     ```

3. **递归调用 wrapper**
   - 由 wrapper 再去调用 `abstract-machine/Makefile`
   - 这时 `SRCS` 已经被缩小成单个测试源文件

因此，“筛选测试项”的本质不是 AM 在做，而是：

> **外层 Makefile 先把 `SRCS` 收缩成单个 `.c`，再把这个单文件程序交给 AM 去真正构建。**

#### 5.3 为什么 `cpu-tests` 里要写 `$${AM_HOME}`？

在生成 wrapper 时，必须区分当前层与下一层 Makefile。

- 当前层 Makefile 里如果写 `$${AM_HOME}`
- 第一层 `make` 会把 `$$` 变成字面量 `$`
- 最终写进临时文件的是：
  ```make
  include ${AM_HOME}/Makefile
  ```
- 这才会在下一层 wrapper 里被正确展开

因此：

- `$(AM_HOME)`：是“当前层 make 立即展开”
- `$${AM_HOME}`：是“保留给下一层 Makefile 再展开”
- `$$(AM_HOME)`：则是错误写法，因为 shell 会把 `$(...)` 当成命令替换

#### 5.4 PASS/FAIL 是怎么打印出来的？

Makefile 里引入了：

- `RESULT = .result`
- 颜色变量
- `if make ...; then printf PASS; else printf FAIL; fi`

也就是说：

- 每个单项测试先由 wrapper 执行
- wrapper 根据退出码把结果追加到 `.result`
- 最后 `run: all` 统一 `cat $(RESULT)` 打印汇总

这也是为什么最终输出会像：

```text
test list [1 item(s)]: format
[        format] PASS
```

---

<a id="sec-klibtest-06"></a>
### 6. 测试执行闭环：从 `make ... run` 到 `PASS/FAIL` 的完整路径

这一条闭环是整个 `klib-tests` 设计中最关键的主线。

以：

```bash
make ARCH=riscv32-nemu ALL=write run
```

为例，完整链路是：

1. **外层 `klib-tests/Makefile`**
   - 解析 `ALL=write`
   - 生成 `Makefile.write`

2. **临时 wrapper**
   - 指定：
     - `NAME = write`
     - `SRCS = src/write.c`
     - `INC_PATH += include`
   - 再 `include ${AM_HOME}/Makefile`

3. **`abstract-machine/Makefile`**
   - 编译 `src/write.c`
   - 编译 `am`
   - 编译 `klib`
   - 链接成目标程序

4. **运行时**
   - `write.c` 的 `main()` 顺序调用各个测试函数
   - 测试函数内部调用对应库函数
   - 再用 `check(...)` 验证返回值或内存布局

5. **断言通过/失败**
   - 若全部 `check()` 通过：`main()` 返回 `0`
   - 若任一 `check()` 失败：`assert_fail()` 打印定位信息并 `halt(1)`

6. **wrapper 回收退出码**
   - 退出码为 `0`：追加 `PASS`
   - 非 `0`：追加 `***FAIL***`

7. **最终 `run` 目标输出汇总结果**

因此，`klib-tests` 的成功标准并不是“程序看起来跑了”，而是：

> **每一个测试 case 的断言都成立，程序最终以 0 正常退出，由 Makefile 汇总打印为 PASS。**

---

<a id="sec-klibtest-07"></a>
### 7. `ARCH=native` 与 `ARCH=riscv32-nemu` 的本质区别

这是整个 `klib-tests` 分析里最关键的认知点之一。

#### 7.1 `ARCH=native` 默认不等于“正在测试自己的 klib”

虽然 `abstract-machine/Makefile` 总会链接 `klib`，但 `klib/src/string.c`、`stdio.c`、`stdlib.c` 里都带有：

```c
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
```

而 `ARCH=native` 时：

- `__ISA_NATIVE__` 会被定义
- `__NATIVE_USE_KLIB__` 默认没有开启

所以默认 native 下：

- 这些 `klib` 实现并不会真正作为当前生效实现参与
- 字符串/stdio/stdlib 行为更偏向宿主机 `libc/glibc`

因此：

> **`ARCH=native` 默认主要用于验证测试代码本身和通用语义，不等于已经验证了你自己的 `klib` 实现。**

#### 7.2 `ARCH=riscv32-nemu` 才真正落到你自己的 `klib`

当 `ARCH=riscv32-nemu` 时：

- `__ISA_NATIVE__` 不成立
- `klib/src/string.c`、`stdio.c`、`stdlib.c` 会真正参与
- 客户程序中的 `strlen/strcpy/snprintf/atoi/...` 会落到你自己的 `klib` 实现
- 最后再由 NEMU 逐条解释执行

所以可以把两者压缩成一句：

- `ARCH=native`：默认更像宿主 glibc 路径
- `ARCH=riscv32-nemu`：真正测试 `klib + AM + NEMU`

<a id="sec-klibtest-07-3"></a>
**7.3 ISA_NATIVE 和 NATIVE_USE_KLIB 的来源与选择逻辑**

这一点是理解整条 native/klib 选择链的关键。

在 `klib/src/string.c`、`stdio.c`、`stdlib.c` 里，经常能看到：

```c
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
```

这两个宏的来源并不相同。

<a id="sec-klibtest-07-3-1"></a>
**7.3.1 __ISA_NATIVE__ 的来源：由构建系统通过 -D 自动注入**

它不是在源码里手写 `#define` 的，而是由 [abstract-machine/Makefile](/home/luo/ysyx/ysyx-workbench/abstract-machine/Makefile#L34) 到 [abstract-machine/Makefile](/home/luo/ysyx/ysyx-workbench/abstract-machine/Makefile#L36) 先解析 `ARCH`：

```make
ARCH_SPLIT = $(subst -, ,$(ARCH))
ISA        = $(word 1,$(ARCH_SPLIT))
PLATFORM   = $(word 2,$(ARCH_SPLIT))
```

然后在 [abstract-machine/Makefile](/home/luo/ysyx/ysyx-workbench/abstract-machine/Makefile#L83) 里注入编译宏：

```make
-D__ISA__=\"$(ISA)\" -D__ISA_$(shell echo $(ISA) | tr a-z A-Z)__
```

因此：

- 当 `ARCH=native`
  - `ISA = native`
  - 编译器会收到：
    ```c
    -D__ISA_NATIVE__
    ```

- 当 `ARCH=riscv32-nemu`
  - `ISA = riscv32`
  - 编译器会收到：
    ```c
    -D__ISA_RISCV32__
    ```

也就是说：

> **`__ISA_NATIVE__` 不是源码里固定写死的宏，而是构建系统根据 `ARCH=native` 自动注入的编译期开关。**

<a id="sec-klibtest-07-3-2"></a>
**7.3.2 __NATIVE_USE_KLIB__ 的来源：klib 自己预留的显式开关**

这个宏可以直接在 [klib.h](/home/luo/ysyx/ysyx-workbench/abstract-machine/klib/include/klib.h#L12) 看到：

```c
//#define __NATIVE_USE_KLIB__
```

它当前默认是注释掉的，因此默认情况下没有定义。

它的设计意图是：

- native 默认优先使用宿主 libc/glibc；
- 如果用户想在 native 下也强制走自己实现的 `klib`，就显式打开这个宏。

因此：

> **`__NATIVE_USE_KLIB__` 是 `klib` 侧手动预留的开关，默认关闭，只有显式启用时 native 才会改走 `klib`。**

<a id="sec-klibtest-07-3-3"></a>
**7.3.3 这条 #if 的选择逻辑到底是什么？**

把条件拆开看：

```c
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
```

它的含义是：

1. **如果当前不是 native**
   - 即 `!defined(__ISA_NATIVE__)` 为真
   - 那就直接启用这份 `klib` 实现

2. **如果当前是 native**
   - 那么第一项为假
   - 这时只有在 `defined(__NATIVE_USE_KLIB__)` 为真时，才启用 `klib`

于是四种典型情况可以整理成下面这张表：

| 当前构建场景 | `__ISA_NATIVE__` | `__NATIVE_USE_KLIB__` | 条件结果 | 最终行为 |
| :--- | :--- | :--- | :--- | :--- |
| `ARCH=native`，默认状态 | 已定义 | 未定义 | `false || false` | 不启用 `klib`，更偏宿主 libc/glibc |
| `ARCH=native`，显式开启 `__NATIVE_USE_KLIB__` | 已定义 | 已定义 | `false || true` | native 下也启用 `klib` |
| `ARCH=riscv32-nemu` | 未定义 | 无论是否开启都不重要 | `true || ...` | 启用 `klib` |
| 其他非 native 架构 | 未定义 | 无论是否开启都不重要 | `true || ...` | 启用 `klib` |

所以这条 `#if` 的本质可以压缩成一句话：

> **非 native 默认启用 `klib`；native 默认关闭 `klib`，除非显式打开 `__NATIVE_USE_KLIB__`。**

<a id="sec-klibtest-07-3-4"></a>
**7.3.4 为什么要这样设计？**

这是一个很典型的“按平台区分默认行为”的设计：

- **native 默认路径**
  - 尽量复用宿主机成熟的 libc/glibc
  - 更适合先验证测试代码本身是否合理

- **非 native 默认路径**
  - 没有宿主 libc 可以依赖
  - 必须走自己实现的 `klib`

- **保留额外开关**
  - 允许开发者在 native 下也强制测试自实现 `klib`

因此，`ARCH=native` 与 `ARCH=riscv32-nemu` 的行为差异，并不是“运行时动态切换”，而是：

> **在编译期通过 `__ISA_NATIVE__` 与 `__NATIVE_USE_KLIB__` 共同决定，某份 `klib` 源文件里的实现到底会不会被编进最终程序。**

<a id="sec-klibtest-07-4"></a>
**7.4 这对 Debug 的意义**

它直接决定了排障优先级：

1. **如果 `ARCH=native` 都失败**
   - 优先怀疑：
     - 测试代码写错
     - 断言对象写错
     - case 设计不独立

2. **如果 `ARCH=native` 过、`ARCH=riscv32-nemu` 失败**
   - 优先怀疑：
     - `klib` 实现不完整
     - NEMU 执行链问题
     - 串口/设备/外层构建环境问题

---

<a id="sec-klibtest-08"></a>
### 8. 断言系统的设计：从普通 `check()` 到可定位报错

#### 8.1 最初需求

最开始的 `check()` 只有：

- 条件成立：继续执行
- 条件失败：`halt(1)`

这样虽然能让测试程序返回非零，但问题是：

- 只能看到 `Exit code = 01h`
- 不知道哪一条断言失败
- 不知道是哪个测试函数挂了

因此必须把它升级成：

> **既能终止，也能输出精确定位信息的断言系统。**

#### 8.2 为什么不能只用普通函数？

因为要打印完整定位信息，需要这四样东西：

- `__FILE__`：文件名
- `__LINE__`：行号
- `__func__`：函数名
- `#cond`：断言表达式原文

其中：

- `__FILE__`、`__LINE__`、`__func__` 来自编译器/预处理器上下文
- `#cond` 来自宏参数字符串化

普通函数只能拿到 `cond` 的真假，拿不到表达式原文，因此：

> **`check` 的最外层必须写成宏。**

#### 8.3 最终断言分层

最终结构分成两层：

1. **宏层**
   - `check(cond)`
   - 负责：
     - 判断条件
     - 自动抓取调用点上下文

2. **底层失败处理函数**
   - `assert_fail(file, line, func, expr)`
   - 负责：
     - 打印：
       ```text
       check failed at write.c:145 in test_xxx: buf[3] == '\0'
       ```
     - `halt(1)`

#### 8.4 为什么要写成 `do { ... } while (0)`？

多语句宏如果直接裸展开，会在 `if/else` 场景下出现语法绑定问题。

把宏写成：

```c
do {
  if (!(cond)) {
    assert_fail(...);
  }
} while (0)
```

可以让 `check(...)` 在语法上始终表现得像“一条普通语句”，这是标准安全写法。

#### 8.5 颜色输出的补充

后续又给 `assert_fail()` 的 `printf` 增加了 ANSI 颜色码，使断言失败时能以红色输出，更利于快速观察。

---

<a id="sec-klibtest-09"></a>
### 9. 设计与 Debug 过程中踩到的关键问题、根因与解决

这一部分按“问题 -> 根因 -> 解决”统一梳理。

#### 9.1 头文件里普通函数定义导致 `multiple definition of check`

**现象：**

链接时报：

```text
multiple definition of `check'
```

**根因：**

- 当时还是“多源文件编进同一个程序”的结构
- `klib-test.h` 里直接放了普通全局 `check()` 定义
- 每个 `.c` 都包含一次头文件，就各自产生一个外部符号

**解决：**

- 要么把定义移到单独 `.c`
- 要么在头文件里改成 `static inline`

最终在断言系统升级后，采用了：

- `static inline assert_fail(...)`
- `#define check(cond) ...`

#### 9.2 `cpu-tests` 风格 Makefile 迁移时，外层 wrapper 生成失败

**典型问题：**

- `ALL` 推导和文件名模式不匹配
- `src/%-test.c` 与重命名后的 `src/%.c` 不一致
- `INC_PATH += ...` 被 shell 当成命令执行，而不是写入临时 wrapper
- 错把 `$$(AM_HOME)` 当成了正确写法，导致 shell 试图执行 `AM_HOME`

**根因：**

- 没把“当前层 Makefile”和“下一层 wrapper Makefile”分开理解
- 没意识到 `abstract-machine/Makefile` 不负责筛选测试项

**解决：**

- 统一文件名为：
  - `write.c`
  - `read.c`
  - `format.c`
  - `stdlib.c`
- 让 `ALL` 与 `Makefile.%: src/%.c` 保持一致
- 把 `INC_PATH` 一并写进临时 wrapper
- 用 `$${AM_HOME}` 正确保留下一层再展开

#### 9.3 `native` 过、`riscv32-nemu` 下断言输出却没有出现

**现象：**

- 故意制造 `check()` 失败后，在 `riscv32-nemu` 下没有看到自定义报错信息
- 反而先触发了：
  - `address = 0xa00003f8 is out of bound of pmem`

**根因：**

- `assert_fail()` 里调用了 `printf`
- 在 `riscv32-nemu` 下，`printf -> klib stdio -> putch -> serial MMIO`
- 如果当前实际运行的 NEMU 没有正确带上设备配置，串口 MMIO 访问会先炸掉

**解决：**

- 开启：
  - `CONFIG_DEVICE=y`
  - `CONFIG_HAS_SERIAL=y`
  - `CONFIG_SERIAL_MMIO=0xa00003f8`
- 并确保当前运行的 NEMU 二进制确实已经按这份配置重新构建

这说明：

> **断言输出本身也依赖底层设备链路是否正常。**

#### 9.4 `format` 在 native 下通过，在 `riscv32-nemu` 下失败

**现象：**

`format.c` 中关于：

- `%x`
- `%c`
- `%%`

的 `sprintf` 测试在 `native` 下通过，但在 `riscv32-nemu` 下失败。

**根因：**

- `native` 默认走宿主 glibc 的 `sprintf`
- `riscv32-nemu` 走的是自己的 `klib sprintf`
- 进一步排查发现：
  - `sprintf()` 调的是 `vsprintf()`
  - 当前 `vsprintf()` 只真正实现了 `%s` 和 `%d`
  - 但 `vsnprintf()` 已经支持 `%s/%d/%x/%p/%c/%%`

也就是说：

> **不是测试错了，而是 `klib/src/stdio.c` 的实现覆盖面本身不一致。**

**解决：**

把 `vsprintf()` 直接收敛到 `vsnprintf()` 的逻辑上，使：

- `sprintf/vsprintf`
- `snprintf/vsnprintf`

支持同一套格式语义。

**结果：**

- `make ARCH=native ALL=format run` 通过
- `make ARCH=riscv32-nemu ALL=format run` 通过

#### 9.5 测试用例本身的典型错误模式

在设计测试过程中，还多次暴露出“不是库错，而是测试写错”的问题，典型包括：

1. **检查了错误的缓冲区**
   - 例如 `strncpy(buf_1, ...)` 之后却去检查 `buf[...]`

2. **把 `NULL` 当成空字符串**
   - 例如 `strcat(dst, NULL)` 不是“空串测试”，而是未定义行为
   - 正确空串应写成 `""`

3. **把字符串字面量当作可写目标缓冲区**
   - 例如把 `"lzqzmm"` 当成 `dst`
   - 这会落入只读存储区，属于未定义行为

4. **测试 case 之间共用状态导致耦合**
   - 一个 case 的结果被下一个 case 直接当输入
   - 这样会降低独立性，失败时不易定位

5. **`memcmp` 长度超过数组容量**
   - 这是读类测试里必须避免的 UB

这些问题的共同教训是：

> **先保证测试代码自身语义正确，再谈用它去验证 `klib`。**

---

<a id="sec-klibtest-10"></a>
### 10. 最终结果、验证结论与方法论总结

#### 10.1 最终达成的结构

`klib-tests` 最终形成了如下稳定结构：

- `src/write.c`
- `src/read.c`
- `src/format.c`
- `src/stdlib.c`
- `include/klib-test.h`
- 外层 `Makefile` 采用 `cpu-tests` 风格的 wrapper + `.result` 汇总机制

#### 10.2 当前已经打通的闭环

1. **测试结构闭环**
   - 每个分类测试都有独立 `main()`
   - 每个 `.c` 可单独编译成测试程序

2. **Makefile 选择闭环**
   - 默认可全量跑
   - `ALL=<name>` 可单独跑某一类

3. **断言定位闭环**
   - 失败时可输出：
     - 文件
     - 行号
     - 函数名
     - 断言表达式

4. **运行环境认知闭环**
   - 已明确：
     - `native` 默认偏宿主 glibc
     - `riscv32-nemu` 才真正测试 `klib + NEMU`

5. **`stdio` 实现差异闭环**
   - 已定位并修复 `vsprintf`/`vsnprintf` 能力不一致问题
   - `format` 测试在 NEMU 下也已通过

#### 10.3 这次 `klib-tests` 设计中最重要的方法论

可以压缩成五句话：

1. **先分层**：测试程序、被测对象、构建系统、执行平台必须分开理解。
2. **先分阶段**：先用 `native` 验证测试本身，再用 `riscv32-nemu` 验证 `klib + NEMU`。
3. **先分类型**：按 `write/read/format/stdlib` 分类，比按头文件表面分更适合测试设计。
4. **先分职责**：外层 Makefile 负责“选谁测”，AM Makefile 负责“怎么编怎么跑”。
5. **先让报错会说话**：没有精确断言定位，后续一切 Debug 成本都会急剧上升。

#### 10.4 一句话总收束

> `klib-tests` 的真正价值，不只是“给 klib 补了一组 case”，而是建立了一整套从测试分类、Makefile 选择、构建装配、运行平台区分，到断言定位与差异 Debug 的完整方法论闭环。

---

<a id="sec-pa2-supplement"></a>
# PA2 专题补充

<a id="sec-native-build-mismatch"></a>
## cpu-tests native 链接报错 `__isoc23_strtol` 排查笔记

[返回目录](#sec-pa2-toc)

### 1. 问题来源
在执行 `make ALL=string ARCH=native run` 时，链接阶段报错：

```text
/usr/bin/ld: ...platform.o: undefined reference to `__isoc23_strtol'
```

表面上看，错误发生在 `abstract-machine/am/src/native/platform.c` 中两处 `atoi()` 相关逻辑；但深入排查后确认，问题本质并不是源码逻辑错误，而是 native 构建产物与当前宿主机工具链、运行库不匹配。

### 2. 现象与关键线索
排查时得到两组关键信息：

1. 当前宿主机环境为：
   - `gcc 11.4.0`
   - `glibc 2.35`
2. 参与链接的归档文件 `abstract-machine/am/build/am-native.a` 内部记录的编译器却是：
   - `GCC 13.3.0 (Ubuntu 24.04)`

这说明当前构建过程中复用了另一台机器生成的旧 archive，而不是由当前机器重新编译得到。

### 3. 处理逻辑梳理
整个分析过程遵循了“先分离源码问题，再定位环境问题”的思路：

1. 先看报错位置，确认 `platform.c` 里的代码只是正常的 `atoi()` 调用，本身没有明显业务错误。
2. 再检查 `am-native.a` 的符号，确认 archive 中确实残留了对 `__isoc23_strtol` 的依赖。
3. 然后对比当前机器的 `gcc/glibc` 版本，发现当前 libc 中并没有这个符号。
4. 最后查看 archive 的编译器标记，确认它来自另一台 Ubuntu 24.04 / GCC 13.3.0 环境。

由此可以推出完整逻辑链：

- `ARCH=native` 的构建依赖宿主机本地 libc。
- `make` 会按时间戳复用已有 `build/` 产物。
- 从另一台机器 `scp` 过来的工作区把旧的 `build/` 目录也带过来了。
- 于是出现“旧机器编出来的 archive + 当前机器的 libc”混合链接。
- 最终触发 `__isoc23_strtol` 符号不兼容。

### 4. 根因本质
这次问题的根因不是 NEMU 配置改错，也不是 `string` 测试写错，而是 native 目标对宿主环境强依赖，但构建缓存没有和宿主 ABI 绑定。

换句话说，构建系统默认假设：

> 只要 `build/` 目录里的文件时间戳没问题，这些 archive/object 就可以继续复用。

这个假设在同一台机器上通常成立，但在“跨机器拷贝工作区”场景下就会失效，因为：

- 编译器版本可能不同。
- glibc 版本可能不同。
- 头文件与运行库导出的符号集合可能不同。

### 5. 解决方案
解决思路不是修改源码，而是清理旧机器遗留的 native 构建缓存，并在当前机器上完整重编。

实际处理步骤：

```bash
make -C "$AM_HOME/am" ARCH=native clean
make -C "$AM_HOME/klib" ARCH=native clean
make clean
make ALL=string ARCH=native run
```

清理后，`abstract-machine/am/build/am-native.a` 会由当前机器重新生成，不再继续复用旧环境产物。

### 6. 处理结果
重建之后：

- `string` 测试结果变为 `PASS`
- 新生成的 `am-native.a` 显示的编译器版本已经变成：
  - `GCC 11.4.0 (Ubuntu 22.04)`

这说明参与链接的 archive 已经和当前宿主环境一致，`__isoc23_strtol` 的符号错配问题被彻底消除。

### 7. 一句话总结
> 这次 `__isoc23_strtol` 链接错误的本质，是从另一台机器拷贝过来的 native 构建缓存污染了当前环境；清理旧 `build/` 产物并在本机重新编译后，问题即可恢复正常。

### 8. 链接期与装载期典型报错、常见原因与 Debug 逻辑对照表

这一节的目标不是记住所有命令，而是先建立一个判断框架：

- 如果错误发生在 `make` 过程中，通常优先归入链接期问题。
- 如果程序已经生成成功，但运行时启动失败，通常优先归入装载期问题。
- 真正的根因不一定在链接器或装载器本身，而往往在它们的输入出了问题。

#### 8.1 链接期问题：典型报错、常见原因、Debug 逻辑

| 典型报错 | 常见原因 | 推荐 Debug 逻辑 |
| :--- | :--- | :--- |
| `undefined reference to xxx` | 只有声明没有定义；实现文件未参与链接；缺少静态库/动态库；库版本或 ABI 不匹配；旧产物污染 | 先看完整链接命令；再用 `nm -A` 查谁在引用 `xxx`、谁真正定义了 `xxx`；最后确认定义来源是否真的参与了本次链接 |
| `multiple definition of xxx` | 同一个全局符号被多个 `.o` 或 `.a` 重复定义；头文件里直接写了非 `static` / 非 `inline` 的定义 | 用 `nm -A` 或报错中的对象名定位重复定义来自哪些文件，再回查是否把变量/函数定义写进了头文件或重复编进多个目标 |
| `cannot find -lxxx` | 链接器搜索路径里没有对应库；库名写错；库未安装 | 先检查链接命令里是否有 `-L`；再确认磁盘上是否存在 `libxxx.so`/`libxxx.a`；最后确认库名和平台是否匹配 |
| `skipping incompatible xxx` | 架构不匹配，例如 32 位/64 位混用，或主机架构与目标架构混用 | 用 `file`、`readelf -h` 检查参与链接的目标文件和库，确认架构、位数、ABI 是否一致 |
| `file format not recognized` | 输入文件损坏；把源码、文本文件或错误架构文件当作目标文件/库传给了链接器 | 先用 `file` 看该文件到底是不是 ELF/归档；再检查构建脚本是否把错误输入喂给了链接器 |
| `DSO missing from command line` | 需要的共享库没有显式出现在链接命令里，或者库顺序不正确 | 检查最终链接命令，补齐缺失库，并把依赖者放前面、被依赖库放后面 |
| `relocation ... can not be used` / `recompile with -fPIC` | 位置无关代码、PIE、静态/动态链接方式不匹配 | 检查目标文件是否带 `-fPIC`/`-fpie`，以及最终链接目标是否要求 PIE/PIC |

#### 8.2 装载期问题：典型报错、常见原因、Debug 逻辑

| 典型报错 | 常见原因 | 推荐 Debug 逻辑 |
| :--- | :--- | :--- |
| `error while loading shared libraries: libxxx.so: cannot open shared object file` | 运行时找不到共享库；`LD_LIBRARY_PATH`、`RPATH`、`RUNPATH` 不正确 | 用 `ldd` 看程序依赖的共享库；再用 `readelf -d` 看 `NEEDED/RPATH/RUNPATH`；最后检查环境变量和库实际路径 |
| `symbol lookup error: undefined symbol: xxx` | 程序运行时加载到的共享库里没有该符号；加载到了错误版本的库 | 先用 `ldd` 看实际加载了哪份库；再用 `nm -D` 或 `readelf -Ws` 看该库是否导出 `xxx` |
| `version 'GLIBC_x.y' not found` | 程序或库依赖了更高版本的 glibc / libstdc++，当前机器版本过低 | 对比程序和共享库所需的版本信息，确认是不是跨系统复制产物导致的版本错配 |
| `wrong ELF class: ELFCLASS32/ELFCLASS64` | 程序与共享库位数不一致 | 用 `file` 检查程序和 `.so` 的架构与位数是否一致 |
| 程序文件存在，但启动时仍报 `No such file or directory` | 解释器路径不存在，例如 ELF 头里记录的动态装载器路径无效 | 用 `readelf -l` 查看 `interpreter` 字段，确认动态装载器是否在当前系统存在 |

#### 8.3 统一 Debug 顺序：先判断阶段，再判断符号归属

以后遇到类似问题，建议固定按下面顺序排：

1. 先判断错误发生在链接期还是装载期。
2. 如果是链接期，先看最终链接命令，再看符号由谁引用、由谁定义。
3. 如果是装载期，先看程序实际加载了哪些共享库，再确认符号是否存在于那份实际被加载的库里。
4. 再回到源码与构建系统，确认是不是声明/定义缺失、条件编译、库顺序、架构不匹配、ABI 不兼容或旧产物污染。

#### 8.4 这次 `__isoc23_strtol` 案例在这张表里属于哪一类

本次错误属于：

- 阶段：链接期
- 表现：`undefined reference to __isoc23_strtol`
- 表面现象：`platform.o` 引用了一个当前系统 libc 中不存在的符号
- 根因：旧机器生成的 `am-native.a` 与当前机器 `glibc 2.35` 不兼容

所以它符合上表中的这一类：

> `undefined reference to xxx` 不一定意味着“源码没写实现”，也可能是“对象文件来自另一套工具链/运行库环境，导致链接时找不到兼容符号定义”。

#### 8.5 一句话方法论

> 看到“未定义符号”时，不要立刻认定源码写错；先判断它本该来自哪里，再判断它为什么没有在当前这次链接或装载过程中被正确解析出来。

<a id="sec-pa2-io-trace"></a>
## PA2 专题：从传参到串口输出的全链路追踪（`make run mainargs=ysyx`）

本专题详细梳理在执行 `make ARCH=riscv32-nemu run mainargs=ysyx` 时，系统是如何将参数注入到客户程序（Guest），并通过客户程序的 `outb` 指令，经过 NEMU 模拟器的层层路由，最终触发宿主机（Host）终端的字符打印。同时涵盖了开发过程中遇到的 Kconfig 与 VSCode 配置问题。

---

<a id="sec-io-01"></a>
### 1. 开发环境追踪记录（插曲问题）

#### 1.1 为什么 menuconfig 中 `Enable watchpoint check` 会出错出现在 `Devices` 菜单下？
* **是什么**：在配置 `make menuconfig` 时，外设 Device 选项中多出了本不该属于外设的 watchpoint 配置。
* **为什么**：通过搜索排查发现，Kconfig 文件中存在重复定义的 `config WATCHPOINT`。一处位于 `nemu/Kconfig`（Testing and Debugging），另一处被错误复制或写入了 `nemu/src/device/Kconfig`（Devices 菜单内）。Kconfig 解析器把第二个定义并入了当前的 `menu "Devices"` 命名空间下。
* **怎么解决**：删除了 `nemu/src/device/Kconfig` 中重复和越界的 `config WATCHPOINT` 块，确保 Kconfig 结构与目录逻辑统一。
* **结果是**：`menuconfig` 恢复正常，调试功能回到对应的测试调试目录。

#### 1.2 为什么 VSCode 无法跳转或识别 `init_monitor` 等函数定义？
* **是什么**：LSP 服务器（C/C++ Intellisense 或 clangd）由于找不到正确的头文件或没解析到条件编译宏，将正确存在的函数（位于 `nemu/src/monitor/monitor.c`）标红或无法跳转。
* **为什么**：这类框架使用了严重依赖 C 语言预处理器的条件编译（`#ifdef CONFIG_xxx`）。VSCode 默认没加载 `nemu/include/generated/autoconf.h`，导致所有 `IFDEF` 相关宏统统失效，被预处理器切断了代码解析。
* **怎么解决**：修改了 `.vscode/c_cpp_properties.json` 中的 `forcedInclude`，强制全局引入 `autoconf.h`；更标准的解法是生成 `compile_commands.json` 提供给 LSP 引擎。
* **结果是**：VSCode 恢复了对条件编译包围的函数的语义高定和跳转。

---

<a id="sec-io-02"></a>
### 2. 参数注入机制：`mainargs` 怎么传给客户程序？

执行命令：`make ARCH=riscv32-nemu run mainargs=ysyx`。

#### 2.1 为什么不能直接像执行普通 C 语言那样用 `argv` 传给 `main()`?
* **是什么**：普通的宿主机 C 程序由操作系统（OS）加载器并把命令行参数放入栈首；但我们跑在 NEMU 上的是个“裸机（Bare-metal）”程序（经 Abstract Machine 打包的 `.bin`），里面没有 OS 帮你传参数。
* **为什么/怎么解决（机制）**：AM 框架采用了一套**编译期占位 + 链接后二进制替换 (Binary Patching)** 的精妙机制来解决传参。
  1. **Make 提取与宏注入**：我们在终端传入了 `mainargs=ysyx`，Makefile (`abstract-machine/scripts/platform/nemu.mk`) 将其捕获。如果没有传，默认通过 `MAINARGS_PLACEHOLDER` 生成一组用来占位的字符。
  2. **C 代码中的占位符**：`trm.c` 中定义了静态字符数组 `static const char mainargs[MAINARGS_MAX_LEN] = TOSTRING(MAINARGS_PLACEHOLDER);`。编译时，这块内存被嵌入到了生成的 ELF 和 `.bin` 二进制中。
  3. **Python 脚本暴力替换**：Makefile 的 build 链里调用了 `insert-arg.py` 脚本。脚本的本质是：打开编译好的 `.bin` 文件，在二进制流中搜索占位符字节序列，然后用我们传入的 `"ysyx"` 字符串暴力覆盖这部分文件流，最后不足长度用 `\0` 填充。

* **结果是**：NEMU 加载这块 `.bin` 到物理内存起跑时，`mainargs` 数组里的内容已经被替换成了 `ysyx`。`_trm_init()` 启动时直接执行 `main(mainargs)`，程序就像获得普通环境 `argv` 一样获得了字符串。

---

<a id="sec-io-03"></a>
### 3. 外设 MMIO 的建立与页对齐机制

外设怎么在 NEMU 里安家落户的？我们围绕 `IOMap` 结构体进行剖析。

#### 3.1 `IOMap` 结构体是什么？
```c
typedef struct {
  const char *name;    // 外设名称
  paddr_t low;         // 物理起始地址
  paddr_t high;        // 物理结束地址
  void *space;         // 主机上为这个外设分配的"模拟寄存器/状态"内存区
  io_callback_t callback; // 函数指针：发生读写时触发的副作用回调
} IOMap;
```
* **为什么这样设计**：实现内存状态存储（`space`）和副作用行为（`callback`）的解耦。写入时只管写 `space`，然后无脑调 `callback` 执行实际功能。这是一种高度面向对象的 C 语言实现多态的机制。

#### 3.2 为什么 `new_space` 要进行页面对齐？它怎么实现的？
```c
size = (size + (PAGE_SIZE - 1)) & ~PAGE_MASK;
```
* **是什么**：不管设备只需要多小的内存（如串口只需要 8 字节），这段分配代码总是按 `4096`（一页）的整数倍划拨内存给自己。
* **为什么**：
  1. **权限隔离**：硬件体系结构管理内存权限（读/写/不可缓存等）最小粒度就是 4KB 一页。不同的外设绝对不能揉进同一个页，否则对其设置页表权限会相互踩踏波及。
  2. **寻址防溢出与简单性**：页面对齐后低 12 位总是零，基址干净利落。线性分配只需要推动全局指针（Bump Allocator）。
* **怎么做算的**：`(size + 4095)` 将大小强行推入下一个页面的范围，然后通过对掩码取反做按位与（`& ~0xFFF`）暴力抹除了低 12 位的“零头”。其本质并不是在找当前页，而是在“加了零头的情况下寻找下一页的基地址”。这也解释了为什么申请大于 4096 字节的设备，能完美连跨两页获得 8192 字节。
* **结果是**：每个传入的外设，通过 `new_space` 都拿到了一块纯净的、整以页对齐的宿主机大块堆内存指针，这个指针就存入了 `map->space`。

---

<a id="sec-io-04"></a>
### 4. 路由分发链与 I/O 截获：从 `outb` 到终端打印

这是重中之重：客户程序怎么能仅仅操作一个内存地址，就让宿主机打出一个字符的？

#### 4.1. 客户程序发起 I/O（`outb`）
* **发生什么**：在 AM 库的实现中，向串口输出是执行 `outb(SERIAL_PORT, ch)`。这在底层实际上会被翻译成往串口物理地址（通常是宏定义的一个定值）抛入一个写操作。
* **本质**：CPU 执行了 `store` 指令（对应 RISC-V 里的 `sb` / `sw` 等）。

#### 4.2. 指令级拦截：`vaddr` -> `paddr`
* **虚拟到物理**：NEMU 的指令执行流进入到 `vaddr_write`，接着直接透传到物理内存访问 `paddr_write(addr, len, data)`。
* **分流点（为什么能被 NEMU 抓住）**：`paddr_write` 中具有关键判断逻辑：
  ```c
  if (in_pmem(addr)) { ...pmen_write... }
  else { mmio_write(addr, len, data); return; }
  ```
  因为串口的地址宏设置的数值**远超出了正常的客户物理内存（pmem）范围**，所以被精准分流到了 `mmio_write`。

#### 4.3. MMIO 映射表查询与写入 (`mmio_write` -> `map_write`)
* **怎么解决寻址**：`mmio_write(addr, len, data)` 调用了 `fetch_mmio_map(addr)`。它遍历全局的 `maps[NR_MAP]` 数组，通过 `low <= addr <= high` 的匹配，找到了事先注册好的 `serial` 的 `IOMap`。
* **状态模拟**：接着调用 `map_write(addr, len, pos, map)`，通过 `offset = addr - map->low` 算出相对偏移。
* **写入 backing buffer**：调用 `host_write(map->space + offset, len, data)`，先把数据写进事先分好的 `space` 状态区。这就相当于在物理电路上完成了电平寄存器的翻转。

#### 4.3.1 核心揭秘：这句 `offset = addr - map->low` 到底有多重要？
* **是什么**：这是整个外设模拟中**“地址转换”**的最核心公式。它把 CPU 视角下的“全局绝对物理地址”转换成了纯净的、设备自身视角下的“局部相对偏移地址”。
* **为什么需要它（跨越鸿沟）**：
  1. **Guest（客户 CPU）的世界**：它是在用指令访存一整片巨大的全局物理地址空间。比如它要写串口第 0 个寄存器，它发出的 `addr` 就是 `0xa00003f8`。
  2. **Host（宿主机 NEMU）的世界**：NEMU 的外设缓冲区只是宿主机操作系统分配的一小块堆内存指针（比如 `0x7fff...`）。如果 NEMU 的 `host_write` 胆敢直接向 `0xa00003f8` 写入，你的 Linux 宿主机操作系统会立刻抛出 `Segfault`（段错误），由于它是非法内存！
* **怎么转换与使用（极度贴合真实硬件的解耦机制）**：
  计算出的 `offset = addr - map->low;` 完美映射了真实物理主板上的寻址逻辑。这可以总结为：
  - **`map->low` 匹配**：相当于主板上的**“片选信号（Chip Select）”**。译码器发现全局物理地址落在串口范围，就下发片选电平唤醒串口芯片（对应 `fetch_mmio_map` 找到设备）。
  - **`offset` 的得出**：相当于由于片选完成，串口芯片已经被唤醒，接下来它只看地址总线的低几位**“寄存器选择引脚”**。也就是说，**`offset` 完全等价于外设内部的各个设备寄存器的偏移地址**。比如它此刻算出 `offset = 0`，代表的就是当前访问的地址其实是设备的“读写缓冲寄存器”。

* **结果是（两层精妙的运用）**：
  1. **给数据安全安家**：计算出 `offset` 后，用纯净合法的宿主机指针加上局部偏移（`map->space + offset`），将 CPU 写入的虚假数据精准、安全地存进了宿主机的实际内存空间里。这才真正写入了外设分配的内存中。
  2. **极简的外设驱动开发（最重要的一层）**：当框架调用 `invoke_callback(..., offset, ...)` 把这个简单干净的 `offset`（而不是巨大的 `addr`）丢给开发串口外设的程序员时，写 `serial_io_handler` 的程序员**完全不需要去死记硬背恶心的系统全局基址了**！它直接通过一个极其简单干脆的 `switch (offset)` 寄存器偏移判断就行了：`case 0` 就意味着操作缓冲寄存器发送字符；`case 4` 意味着操作线路控制寄存器调波特率。这使得**特定体系结构相关的物理地址空间**与**特定外设模块内部的行为逻辑**实现了最完美的彻底切割与解耦！

#### 4.4. 回调触发与终端打印 (`invoke_callback`)
* **为什么能打字**：`map_write` 写完之后立刻进行 `invoke_callback(map->callback, offset, len, true)`。
* **函数指针显威**：由于 `init_serial()` 时，`map->callback` 被绑定成了 `serial_io_handler`。此时其实执行的就是：
  ```c
  serial_io_handler(offset, len, is_write=true)
  ```
* **设备副作用真正产生**：串口回调函数发现 `is_write` 是真，且偏移量（`offset`）是收发寄存器偏移（`CH_OFFSET`，0），于是代码跑向了 `serial_putc(serial_base[0])`。由于 `space`（即 `serial_base`）在上一秒刚被 `host_write` 塞进了我们要打印的字符 `y`，它把它拿出来丢给了宿主机的库函数 `putc(ch, stderr)`。

#### 4.5. 总结：全通了
* **结果是**：客户机里面虚假的写内存操作（`outb`），成功“欺骗”了 NEMU。NEMU 判断这不是真正的内存，从而走设备总线；查到设备是串口后，把该字符暂存到虚拟的串口寄存器（`space`），并通过多态绑定的函数指针引发了真实操作系统的 `putc`，最终字符 `y` 从你的电脑终端黑框里冒了出来。


---

<a id="sec-pa2-rtc-timer"></a>
## PA2 专题：时钟计时器与软硬协同全栈生命周期梳理

当你敲下 `make ARCH=riscv32-nemu run mainargs=t` 并按下回车那一刻，看似只是终端里弹出了当前的时间：`1900-0-0 01:05:09 GMT`，但其背后贯穿了**编译构建、软件运行时、指令集架构定义、微架构仿真以及内存映射 I/O (MMIO)** 整个计算机底层体系。

本节我们将以时钟测试程序 (`rtc_test`) 为例，把 NEMU（模拟硬件）和 AM-tests（运行其上的软件）如何从启动到完成软硬件协同的过程进行全面深度的分层架构总结。

<a id="sec-rtc-01"></a>
### 1、全局视角：时钟测试在验证什么？

由于 NEMU 本质是个 C 语言写成的单线程死循环模拟器（状态机），它里面并没有真实的“晶振”或时钟模块去给虚拟机发时钟中断。
验证“时钟”其实是在验证**软硬通信链路**：
*   **软件（AM）**能不能通过特定的“内存指针操作”正确发送时间查询请求。
*   **硬件（NEMU）**能不能在这特定的物理地址读写操作上实施拦截（MMIO 回调），并通过宿主系统（Linux）的 API 代为获取真实的挂钟时间返回给软件。

<a id="sec-rtc-02"></a>
### 2、第一阶段：NEMU（硬件）的启动与外设准备

在执行前，模拟器必须搭好“舞台”。
1.  **命令行解析与子系统初始化**：
    启动进入 NEMU `main()`，如果未定义 `CONFIG_TRACET_AM`，则走到 `init_monitor()`。
    按序执行 `parse_args()` 获取命令行参数、`init_mem()` 初始化物理内存边界等。
2.  **外设注册地图 (MMIO 初始化)**：
    重点在于 `init_device()` -> 进而在各硬件（比如 `timer.c` 初始化）调用 `add_mmio_map()`。
    **检查机制**：此时会在 NEMU 架构内生成一块地址为 `0xa0000048` 的特殊内存区。`add_mmio_map` 函数中利用严格的“非相交”原理 `left <= maps[i].high && right >= maps[i].low` 做碰撞检测，确保它不与常规内存区 (`PMEM`) 或其他外设地址冲突。
3.  **硬件准备就绪**：
    该区域不属于真正的 RAM 数组，而是被绑定了 `rtc_io_handler`（时钟回调函数）。此时，NEMU 这台“虚拟机主板”组装完毕。

<a id="sec-rtc-03"></a>
### 3、第二阶段：AM-tests（软件）的编译、加载与启动

舞台搭好后，软件这个“演员”开始上场。
1.  **编译与封装**：
    `am-tests` 被编译为标准的 ELF 文件，`objcopy` 取出机器码生成 `.bin` 镜像。
2.  **传参注入**：
    外部命令 `mainargs=t` 使得字符 `'t'` 被放在了镜像的约定传参位置。
3.  **上板加载与启动**：
    NEMU 的 `load_img()` 将镜像拷贝到 `0x80000000` 开始的物理内存数组中，然后 CPU 指令计数器 PC 指向 `0x80000000` 并启动 Fetch-Decode-Execute (取指-译码-执行) 循环。
4.  **软件环境 (TRM) 构建**：
    程序进入 AM 的 `_start`，配置好栈后跳转至软件主函数 `main(args)`。在这个 `main` 函数里，读取入参分支走到 `CASE('t', rtc_test, IOE)`。
5.  **HAL (硬件抽象层) 挂载**：
    调用 `ioe_init()` 函数！这个函数通过一系列底层的宏展开和函数调用（如 `__am_timer_init`），会将针对各种不同设备的响应函数，分别注册存入一张大表（Look-Up Table, `lut`）。在这个表里，时间读取的需求 `AM_TIMER_UPTIME` 指向了函数 `__am_timer_uptime`。此外，`__am_timer_init` 在这一阶段还会利用直接的 MMIO 读函数记录下 NEMU 启动时的**绝对时刻**存入 `boot_time`。

<a id="sec-rtc-04"></a>
### 4、第三阶段：软硬协同的巅峰——一次读取时间的完整物理全貌

接下来进入时钟测试 `rtc_test()`，核心是循环读取时间差：`while(io_read(AM_TIMER_UPTIME).us / 1000000 < sec);`。这个宏最终展开成了一次底层内存交互，其贯穿软件到硬件的**“纵向 6 层栈”**全过程如下：

1.  **APP 层 (am-tests)**：调用 `io_read(AM_TIMER_UPTIME)` 索要上电运行微秒数。
2.  **HAL 层 (AM ioe)**：去查 `lut` 分发表，命中 `__am_timer_uptime` 函数。
3.  **BSP 层 (Board Support Package)**：`__am_timer_uptime` 内部去读取特定的宏指针 `inl(RTC_ADDR)`，本质是对 `volatile uint32_t *` 类型的 `0xa0000048` 进行解引用。
4.  **编译/指令层 (GCC)**：由于 `volatile` 存在，编译器忠实地将其翻译为一条向地址 `0xa0000048` 取数据的架构汇编指令（比如 `lw a5, 0(a4)`）。
5.  **仿真底层 (NEMU Decode)**：NEMU 此时运行到这条 `lw`。经过之前高度优化的 `inst.c` 内的 `switch(opcode)` 中高频访存指引路线，NEMU 立刻知道要去 `0xa0000048` 这个地址“读4个字节”。
6.  **硬件响应层 (MMIO)**：NEMU 的总线机制发现 `0xa0000048` 落在了注册的外设映射空间里。因此，中断普通内存读取，转而调用绑定在它身上的 `rtc_io_handler` 函数。在该回调里，NEMU 越界向宿主机(Linux系统)请求获取物理时钟当前的时间并返回给虚拟机寄存器。整个 `lw` 执行结束！

这一个简单的“读取时间”调用，其实打破了虚拟机结界，是一次极度深刻的系统穿透。

<a id="sec-rtc-05"></a>
### 5、第四阶段：成果的展示——第二次协同 (串口输出)

测试程序经过计算差值达成 `sec` 秒级别循环跳出条件后，继续获取年月日格式进行打印展示。
1.  **二次读操作**：程序再次调用 `io_read(AM_TIMER_RTC)` 读取年、月、日等 RTC 成员变量。
2.  **格式化规整**：使用定制过的 `klib` 中的 `vsprintf` 标准库，解析类似 `%02d` 这种前补零格式。
3.  **串口通信闭环**：`printf` 的底层 `putch` 再次以同样的套路（对串口的指定内存地址执行 `sb` 储存指令）。此时 NEMU 截获写串口的 MMIO，调用 `serial_io_handler` 将字符转发到 Linux 的原终端屏幕上。

<a id="sec-rtc-06"></a>
### 6、代码层面关键解耦机制总结 (LUT与IOE)

在本次 AM 对设备驱动封装的设计中，展现了极其精妙的**硬件抽象与解耦**设计思路：
*   **应用程序（APP层）完全无状态屏蔽**：它只看到 `rtc = io_read(AM_TIMER_RTC)`，不需要知道具体的物理地址是什么。它将这种架构迁移适配性交给底层的硬件抽象库实现。
*   **高效驱动匹配**：采用大表（Look-Up Table, `lut`）充当“设备路由器”。这种静态或动态分表的策略是底层操作系统驱动子系统（如 Linux VFS，字符设备 file_operations）的超级缩影版，方便新增驱动模块进行统一化调度。

<a id="sec-rtc-07"></a>
### 7、方法论与全局总结

通过实现一个“看似微小的时钟计时器输出”，成功在纵向上**打穿**了计算机底层的各个环节。
> “外设读写，本质上就是一场由软件精心布局的‘针对特定地址’的存取游戏，而硬件在这个过程中则是一名默默配合执行‘重定向’与‘数据篡改’以返回宿主世界状态的守门员。”

[回到顶部目录](#sec-pa2-toc)

<a id="sec-pa2-dtrace"></a>
## PA2 专题：`dtrace` 的完整设计过程总结与踩坑记录

### 1. `dtrace` 设备记录追踪也可以像 `mtrace` 这样设计吗？
<a id="sec-dtrace-01"></a>
**可以。** `dtrace` 与 `mtrace` 的设计本质一致，都是需要在 Kconfig 中增加配置，在 Makefile 里处理编译条件与文件路径，并在 C 代码中实现追踪逻辑。
具体在 `dtrace` 的场景下，核心的插桩点位于设备 I/O 访存的核心：`map_read` 和 `map_write` (在 `src/device/io/map.c` 中)。只要通过 Kconfig 开启宏，就能复用类似的配置、编译条件和全局路径生成的机制来记录对指定设备的读写类型、时间/PC、设备名及读写数值。

### 2. 为什么 `mtrace`, `itrace`, `ftrace` 都添加了 `depends on TRACE && TARGET_NATIVE_ELF && ENGINE_INTERPRETER` 是必要条件吗？
<a id="sec-dtrace-02"></a>
这几个依赖条件是**必要的前提**，是为了保证追踪机制能在正确且合理的环境下编译和运行，各个条件的含义极具工程意义：
*   **`TRACE`**：作为追踪功能的“总开关”。只有打开大总管才能打开个别专项监控，体现模块化管理的层次感。
*   **`TARGET_NATIVE_ELF`**：追踪生成的 Log 文件最终是要落地到宿主机的操作系统的文件系统里的。`TARGET_NATIVE_ELF` 意味着当前 NEMU 工作在宿主机的原生 ELF 平台上，可以随便调用标准库 `fopen`, `fprintf` 去写本地文件。如果在不支持本地文件系统的环境（比如直接裸金属或者部分特殊的交叉编译目标），这些标准库调用必然导致链接错误或者运行崩溃。
*   **`ENGINE_INTERPRETER`**：NEMU 的执行引擎有很多种可能，有些机制可能高度定制。依赖解释器环境能确保这些 Trace 插桩的执行上下文和模拟器的主体执行逻辑是最契合的，避免不兼容执行器带来的副作用。

### 3. 那给出的 `dtrace` 为啥没有加上？
<a id="sec-dtrace-03"></a>
因为初步设计时**仅聚焦于功能实现本身**（只关心怎么在 `map.c` 里拦截读写），而忽略了工程视角的严谨约束。
实际上，为了保持与整个 NEMU Kconfig 体系的安全性和一致性，`dtrace` 同样应该补齐这些 `depends on` 条件，防止当宿主环境发生变化时不正确的代码被强行编译进去。

### 4. 检查当前设计逻辑合理吗关于 `dtrace` 已经完善 config 以及路径处理...
<a id="sec-dtrace-04"></a>
在仿照实现时，出现了一个经典的 Copy-Paste Bug：使用了错误的全局变量尺寸作安全检查或分配。
*   **错误现象**：在处理 `dtrace_log_file` 路径生成的容错检查或缓冲区拷贝时，可能错写成了 `sizeof(ftrace_log_file)`。
*   **深层影响**：如果只看编译器其实不会报错（因为变量存在且类型合法），但在 C 语言运行时（Runtime），这会导致缓冲区溢出检查失效，或者获取到错误的路径尺寸。
*   **修正结论**：关于 `dtrace` 已经完善了 `init_monitor` 里关于路径接收和处理的逻辑（通过参数传递路径或赋予默认路径），必须确保每一个涉及缓冲区的 `sizeof()` 或长度计算使用的都是**本模块专属**的变量。

### 5. 我的 log 写入是在 `map_read` / `map_write` 内嵌入的... 还需要去初始化？
<a id="sec-dtrace-05"></a>
如果在每次读写设备时单独 `fopen()` 然后马上 `fclose()`，虽然避免了管理文件指针的存活生命周期，但会引发极大性能灾难：
*   **I/O 操作频繁**：因为设备的存取是非常高频触发的（比如每隔几个周期就会读取一次串口的 STATUS 标志，或者时钟定时器）。
*   **系统调用开销**：`fopen` 和 `fclose` 每次都需要切入内核态（Syscall），频繁的上下文切换会让模拟器慢如蜗首，不仅逻辑冗长且严重拖累执行性能。
*   **正确做法**：将 `dtrace_fp` (文件指针) 定义为全局变量。在程序的最一开始 (`init_monitor`) 时执行一次打开 (`fopen`)，随后在全局周期内一直使用 `fprintf` 缓冲写。这种**生命周期分离**的方式是典型的性能敏感型系统常用设计。

### 6. 当前编译器表示 `DTRACE_COND` 未定义
<a id="sec-dtrace-06"></a>
这是一个非常低级但极具杀伤力的 Typo 踩坑记录：
*   **问题场景**：`Kconfig` 中可能这样配置了 `default "ture"`。
*   **后果**：Menuconfig 在生成自动引用的代码头文件时，将这个拼写错误的 `"ture"` 转换为宏定义。而在 C 代码中，当我们使用 `if (CONFIG_DTRACE_COND)` 或者类似的判断语句时，宏被展开等价于 `if (ture)`。
*   **报错本质**：C 语言里只有 `true`（如果有 `<stdbool.h>`）或者非 0 值。编译器不认识 `ture` 这个未声明的标识符，直接报出 `DTRACE_COND` 或 `ture` 未定义。
*   **修复**：在 Kconfig 里老老实实写正确的布尔类型默认值 `default y` 或者 `default "true"`，必须绝对注意拼写检查。

### 7. 那 `dtrace_fp` 指针一直开着？不用关闭吗？
<a id="sec-dtrace-07"></a>
程序退出时，操作系统的进程资源回收机制会自动清理所有该进程依然打开着的文件描述符，并将残留的 Page Cache 数据落盘。
*   **免去手动关闭**：对于生命周期等同于全局进程的工具日志，可以依赖 OS 级别的进程资源回收，进程结束时自动释放。
*   **配合保障流 (`fflush`)**：但是如果遇到了进程异常（比如由于 Bug 导致 Segfault 时），C 标准用户态缓冲里的部分最后日志可能会丢失。因此，可以常开文件指针并在关键的或者每次写入后，追加使用 `fflush(dtrace_fp)`。这样既保证了写盘不丢日志，也能免去到处放置 `fclose` 或繁杂的管理问题。

### 8. Serial 通信里的一个疑问：RTC 测试输出了串口，但在 RTL 里没有？
<a id="sec-dtrace-08"></a>
*   **现象描述**：在进行 RTC 实时时钟测试 (`am-tests`) 时，输出被成功打到了串口上，但是在验证 RTL 时没看到？
*   **原理解析**：在目前的软件框架实现中，AM 的 `putch` 本质是向特定内存地址的写入（如 `outb()` 操作）。NEMU 截获写存后，并没有经过真实的物理串行线，而是直接转到原生操作系统的标准输出打印了。这是一种纯软件环境下的**捷径旁路**。如果不单独在 RTL 仿真中针对特定的串口引脚搭建接收和转换监控并输出的机制，底层 RTL 是不知道所谓“控制台输出”的，它仅仅把这一切视为普通的信号翻转。虚拟环境下的串口输出是因为存在 `serial_io_handler` 这样的转发代理。

[回到顶部目录](#sec-pa2-toc)

<a id="sec-pa2-nemu-on-nemu"></a>
## PA2 专题：NEMU 运行 NEMU (NEMU on NEMU) 避坑与总结

将 NEMU 作为一个普通程序，运行在另一个 NEMU 之上（即客体 NEMU 运行在宿主机 NEMU 上），在这个过程中会遇到很多特殊的底层机制与配置问题。

### 1. `CONFIG_TARGET_AM` 的核心作用与解析
- **原生模式 vs AM 模式**：
  普通的 NEMU 编译为 Linux 原生程序（执行 `init_monitor(argc, argv)`），直接利用宿主机 Linux 的系统库，生成 `riscv32-nemu-interpreter` ELF 可执行文件。
  但在 NEMU 上跑 NEMU 时，目标“客户机 NEMU”被打包成了 AbstractMachine (AM) 的裸机应用（会被编译为后缀是 `.bin` 的内存镜像），此时为了剔除原生 OS 的依赖，编译器会开启 `CONFIG_TARGET_AM` 宏。
- **入口分化机制**：
  在 `nemu-main.c` 的宏控路由处：
  ```c
  #ifdef CONFIG_TARGET_AM
    am_init_monitor();  // 裸机环境：没有 OS 提供 argc/argv，直接读取内存中存入的主参数解析
  #else
    init_monitor(argc, argv); // 原生环境
  #endif
  ```
- **警惕本地 `.config` 污染**：
  在 `am-kernels/kernels/nemu` 内 `make` 时，会自动为你挂载 `riscv32-am_defconfig`。**千万注意：宿主机本地真实的 `nemu/menuconfig` 里绝对不要误勾选 `TARGET_AM=y`**。如果宿主机环境被污染为 AM 模式，会导致外层 Makefile 在 `ifeq ($(CONFIG_TARGET_AM), y)` 时错误地抛弃了 `native.mk` 而去 `include am.mk`，从而陷入无限构建循环或导致最外层失去原本的 `make run` 模拟能力。

### 2. 标准输出（`printf`）去哪儿了？为什么卡住没有反应？
- **通过外部串口重定向输出**：
  带有 `CONFIG_TARGET_AM` 的客户机 NEMU 并没有 Linux 环境，它发出的 `printf` 调用的不是原生 glibc，而是 `klib` 中的简易库实现。底层是通过 AM 层的 `putch()` 函数将字符写入特殊物理内存地址：**串口 MMIO (Serial Port MMIO)**。
  - 启动外部环境后，终端通常会停留在宿主机的 `(nemu)` 提示符处。**此时客户机并没有开始执行！**
  - **切记**：只有在宿主机的 `(nemu)` 中键入 `c` 回车，宿主机开始执行模拟指令，并监听截获到“客户机往该地址写入串口内容”后，才会在你的 Linux 终端上代为输出对应的字符。

### 3. 灵活修改参数解析 (`parse_args`) 带来的 `-i` 镜像空跑陷阱
- **问题机制**：原版 NEMU 往往默认直接把没选项的最后一个参数当作 `.bin` 镜像提取。而我们在 `monitor.c` 中把 `parse_args` 规范成了必须用 `-i xxx.bin` 来接收。然而在 NEMU on NEMU 场景中，`am-kernels/kernels/nemu/Makefile` 最后的一句 `make run IMG=xxx.bin` 并不知道这个改变，仍然只传了 `xxx.bin` 字面量。
- **后果**：导致外层宿主 NEMU 检测不到 `-i` 标识，以为自己什么镜像都没挂载，直接加载自带的一串微小空闲指令迅速停机。
- **修复**：修改项目对应的触发级 Makefile (`am-kernels/kernels/nemu/Makefile`) 引号包裹强制挂上 `-i`：
  ```makefile
  $(MAKE) -C $(NEMU_HOME) run IMG="-i $(NEMU_HOME)/build/$(ISA)-nemu-interpreter-$(ARCH).bin"
  ```
  注意：一定要带上引号包裹 `"-i xxx.bin"`，保证 Make 不会被内部的空格割裂成分散的选项。

### 4. klib 中缺失 `malloc` 导致的启动崩溃 (Not implemented)
- **问题现象**：执行成功后会报 `AM Panic: Not implemented @ stdlib.c` 并在 `__am_panic` 宕机。这是因为 Nemu (作为纯 C 程序工程) 在初始化自身数据结构（例如为访存模型分配 `pmem`）时极其依赖动态分配申请堆内存，而它跑在 AM 上所连入的 `klib` 出厂默认并未实现 `malloc()`。
- **解决方案**：手写一个最简单的线性内存分配器（Bump Pointer allocator）。
  - 利用 AM 已经完成的全局堆区边界（`heap.start` ~ `heap.end`）。
  - 定用静态指针 `static char *addr`，每次将起始地址加上需要的 `size` 后向后移。
  - 需要做 4/8 字节强制对齐避免访问指令的严格对齐陷入，并时刻 `assert(addr <= heap.end)` 查验溢出。

[回到顶部目录](#sec-pa2-toc)

---

<a id="sec-pa2-keyboard"></a>
## PA2 专题：pa2-keyboard 键盘外设、AM 抽象接口与软硬件协同全链路梳理

本专题专门把我们在实践中最容易混淆的几件事彻底分开讲清楚：
*   软件层到底在读什么？
*   AM 层到底抽象了什么？
*   NEMU 键盘外设到底在模拟什么？
*   所谓“抽象寄存器”到底该如何准确理解？

如果只看表面，键盘输入仿佛只是程序里一次简单的 `io_read(AM_INPUT_KEYBRD)`；但如果深入到底层，就会发现这背后实际上贯穿了：
*   宿主机 SDL 事件系统；
*   NEMU 键盘设备模型；
*   设备寄存器 / MMIO 总线分发；
*   AM 统一设备接口；
*   应用层对结构化键盘事件的消费。

这一整条路径，正是 PA2 中“软硬件协同”最典型、也最适合建立系统观的案例。

<a id="sec-kbd-01"></a>
### 1. 先纠正一个最容易误解的点：AM 不是“再造一套硬件寄存器”

我们一开始很容易把这套架构想成三层：
1.  最底层硬件寄存器；
2.  中间层 AM 构建“抽象寄存器”；
3.  上层应用通过“抽象寄存器”取数据。

这个理解**抓住了一部分事实**，但如果要做到和代码实现完全一致，还需要更精确地修正。

更准确的说法应该是：
*   **底层并不是“真实芯片寄存器”**，而是 **NEMU 用 C 代码模拟出来的设备寄存器/MMIO 状态**；
*   **AM 并不是创造一套新的硬件寄存器**，而是提供一层**统一设备访问接口**；
*   **上层应用看到的也不是“寄存器位图”**，而是 AM 已经整理好的 **抽象设备数据结构**。

所以更准确的三层关系是：
1.  **设备寄存器层 / NEMU 硬件模拟层**：NEMU 模拟出键盘设备寄存器、地址空间、回调行为；
2.  **AM 驱动/平台适配层**：AM 负责访问底层设备寄存器，并把寄存器原始值转换成统一的数据结构；
3.  **应用层**：应用通过 `io_read()` / `io_write()` 这套统一接口使用设备，不再直接关心具体地址和位布局。

一句话压缩就是：

> 上层软件不是通过“抽象寄存器”直接取数据，而是通过 AM 提供的抽象设备接口取数据；AM 再去访问底层设备寄存器，并把寄存器原始值转换成统一的数据结构返回给上层。

<a id="sec-kbd-02"></a>
### 2. 所谓“抽象寄存器”的意义，到底该怎么准确理解？

在 AM 里，确实存在一种很像“抽象寄存器”的对象，例如：
```c
AM_DEVREG( 8, INPUT_KEYBRD, RD, bool keydown; int keycode);
```
它会展开成两样东西：
1.  一个接口编号：`AM_INPUT_KEYBRD`
2.  一个结构体类型：`AM_INPUT_KEYBRD_T`

这段定义位于 `abstract-machine/am/include/amdev.h`。

它的真正意义不是“重新发明一块寄存器”，而是：
*   给上层软件一个**平台无关的设备访问编号**；
*   给上层软件一个**统一的数据返回格式**。

所以更准确地说：
*   **`AM_INPUT_KEYBRD`** 是“抽象设备接口编号”；
*   **`AM_INPUT_KEYBRD_T`** 是“抽象后的设备数据结构”；
*   它们共同构成了软件层与设备层之间的**统一协议**。

这里最关键的一层认识是：
*   **底层设备寄存器仍然存在**；
*   **AM 只是把“寄存器原始读写”包装成“结构化设备语义”**。

因此，“抽象寄存器”这个词如果继续使用，最好理解为：

> 它不是一块真实存在的新硬件寄存器，而是 AM 为上层软件定义出来的“抽象设备接口 + 结构化数据表示”。

<a id="sec-kbd-03"></a>
### 3. `io_read()` / `io_write()` 到底是什么？

从软件开发者视角看，`io_read()` / `io_write()` 可以把它理解成：
*   一层**设备访问库接口**；
*   一层**平台抽象接口**；
*   一层**软硬件交互中间层**。

上层 `.c` 代码不直接操心：
*   MMIO 地址是多少；
*   寄存器位布局是什么；
*   当前平台是 NEMU、NPC 还是别的后端；
*   设备内部到底是怎样模拟出来的。

它只需要写：
```c
AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
```

`io_read` 的宏定义本质如下：
```c
#define io_read(reg) \
  ({ reg##_T __io_param; \
    ioe_read(reg, &__io_param); \
    __io_param; })
```

这说明：
1.  你传进去的不是“结构体实例”，而是一个**接口名**：`AM_INPUT_KEYBRD`
2.  宏内部自动创建一个对应的结构体：`AM_INPUT_KEYBRD_T`
3.  再调用 `ioe_read(AM_INPUT_KEYBRD, &__io_param)`
4.  最后把填好的结构体返回给你

因此更准确的语言应该是：

> `io_read()` / `io_write()` 可以看成一个平台抽象层提供的库式接口。  
> 它们内部已经约定好了“该调用哪个设备驱动、该怎么访问底层寄存器、该怎么把结果组织成统一结构体”，从而把上层软件和底层硬件细节解耦。

也可以进一步简写成：

> 不是“软件直接读硬件寄存器”，而是“软件调用 AM 的设备接口，AM 再代它去访问硬件寄存器并整理结果”。

<a id="sec-kbd-04"></a>
### 4. 键盘专题的三层模型：软件层、AM 层、硬件层

#### 4.1 软件层（应用 / 测试程序）

应用层只关心：
*   当前有没有键盘事件；
*   是按下还是释放；
*   哪个键被触发。

应用并不关心：
*   `KBD_ADDR` 是多少；
*   这是 MMIO 还是 PIO；
*   SDL 扫描码是什么；
*   NEMU 内部是否维护了事件队列。

对应用来说，它看到的是：
```c
AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
```

应用所消费的是：
*   `ev.keydown`
*   `ev.keycode`

这一层里，设备已经不再表现为“寄存器位”，而是表现为“结构化设备语义”。

#### 4.2 AM 层（驱动 / 平台适配层）

AM 层做两件事：
1.  **按平台规则去访问底层设备寄存器**
2.  **把底层原始值整理成统一数据结构**

以键盘为例，`__am_input_keybrd()` 的实现非常直接：
```c
uint32_t scancode = inl(KBD_ADDR);
kbd->keydown = (scancode & KEYDOWN_MASK) ? 1 : 0;
kbd->keycode = scancode & ~KEYDOWN_MASK;
```

这说明 AM 层并没有“重新发明硬件”，它只是：
*   从底层键盘寄存器读到一个 32 位原始值；
*   再把这个原始值拆解成：
    *   `keydown`
    *   `keycode`

因此，AM 的角色更接近：
*   驱动层；
*   BSP（Board Support Package）；
*   平台适配层；
*   硬件抽象接口层。

#### 4.3 硬件层（NEMU 设备寄存器 / MMIO 模拟层）

这一层是 NEMU 的职责范围：
*   注册键盘设备地址；
*   维护键盘输入队列；
*   在客户机读取键盘地址时，把下一条键盘事件送入设备寄存器；
*   通过 MMIO 机制把这个值返回给客户机 CPU。

所以这层里的“硬件”不是物理芯片，而是：

> NEMU 用内存空间、设备回调、地址映射和事件队列模拟出来的设备寄存器行为。

<a id="sec-kbd-05"></a>
### 5. 键盘外设启动阶段：设备是怎样被挂到总线上的？

在客户机程序真正开始读取键盘前，NEMU 必须先完成键盘设备注册。这个过程发生在 `init_device()` -> `init_i8042()` 里。

其核心动作如下：

1.  **申请一块设备空间**
    ```c
    i8042_data_port_base = (uint32_t *)new_space(4);
    ```
    这 4 字节空间就是“键盘数据寄存器”的模拟存储区。

2.  **将初值设置为无按键事件**
    ```c
    i8042_data_port_base[0] = NEMU_KEY_NONE;
    ```

3.  **把这块地址注册进设备映射表**
    ```c
    add_mmio_map("keyboard", CONFIG_I8042_DATA_MMIO, i8042_data_port_base, 4, i8042_data_io_handler);
    ```
    这意味着：
    *   某个地址范围现在属于“键盘设备”；
    *   当客户机 CPU 读到这里时，不再走普通 RAM；
    *   而是会分发给键盘设备回调函数来处理。

从此以后，客户机访问这个地址，就相当于访问“键盘控制器的数据寄存器”。

<a id="sec-kbd-06"></a>
### 6. 宿主机真实按键发生后，事件是如何进入 NEMU 的？

这一步是从“现实世界”进入“模拟世界”的入口。

NEMU 执行过程中，每隔一段模拟时间会调用一次 `device_update()`。在这个函数里，NEMU 会不断轮询 SDL 事件队列：
*   `SDL_KEYDOWN`
*   `SDL_KEYUP`

流程如下：

1.  宿主机用户按下某个键；
2.  SDL 在宿主机侧捕获到该键盘事件；
3.  NEMU 在 `device_update()` 中调用 `SDL_PollEvent()` 取出它；
4.  提取出：
    *   `scancode`
    *   `is_keydown`
5.  然后调用：
    ```c
    send_key(k, is_keydown);
    ```

这一阶段的本质是：

> 宿主机发生的真实按键事件，被 NEMU 收集并送入虚拟机内部的键盘设备模型。

注意，此时事件**还没有直接交给客户机应用**，而只是进入了 NEMU 键盘模型内部。

<a id="sec-kbd-07"></a>
### 7. NEMU 内部：SDL 扫描码怎样变成客户机可理解的键值？

在 `keyboard.c` 中，NEMU 使用了一套 X-macro 机制维护键名表和键值映射表。

其核心逻辑是：
1.  定义统一键列表：`NEMU_KEYS(...)`
2.  生成内部枚举：`NEMU_KEY_ESCAPE`, `NEMU_KEY_A`, ...
3.  初始化 `keymap[]`，将：
    *   `SDL_SCANCODE_XXX`
    *   映射成
    *   `NEMU_KEY_XXX`

之后 `send_key()` 做两件关键事：

1.  **状态编码**
    ```c
    uint32_t am_scancode = keymap[scancode] | (is_keydown ? KEYDOWN_MASK : 0);
    ```
    它把：
    *   低位编码成键码；
    *   高位 `KEYDOWN_MASK` 编码成“按下/释放”状态。

2.  **入队**
    ```c
    key_enqueue(am_scancode);
    ```
    把这个键盘事件压入环形队列 `key_queue`。

这一步说明一个非常重要的事实：

> 键盘事件不是一发生就“立刻塞给应用”，而是先进入 NEMU 的设备队列，等待客户机软件主动读取。

这正是当前键盘模型的“轮询式设备”本质。

<a id="sec-kbd-08"></a>
### 8. 应用发起读取：`io_read(AM_INPUT_KEYBRD)` 这一句背后到底发生了什么？

从应用角度看，只写了：
```c
AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
```

但它背后实际上是如下链路：

1.  `io_read(AM_INPUT_KEYBRD)` 展开成：
    *   创建 `AM_INPUT_KEYBRD_T`
    *   调用 `ioe_read(AM_INPUT_KEYBRD, &buf)`
    *   返回结构体

2.  `ioe_read()` 查 `lut` 分发表，找到：
    ```c
    [AM_INPUT_KEYBRD] = __am_input_keybrd
    ```

3.  进入 `__am_input_keybrd(AM_INPUT_KEYBRD_T *kbd)`

4.  AM 驱动执行：
    ```c
    uint32_t scancode = inl(KBD_ADDR);
    ```
    也就是去读底层键盘寄存器。

5.  然后把返回的原始值拆成：
    *   `kbd->keydown`
    *   `kbd->keycode`

所以对应用来说，它拿到的是：
*   “一个键盘事件结构体”

而不是：
*   “一个设备寄存器原始整数”

<a id="sec-kbd-09"></a>
### 9. 真正的硬件交互核心：读 `KBD_ADDR` 时，NEMU 底层发生了什么？

这一节是理解软硬件协同的关键。

当 AM 驱动执行：
```c
uint32_t scancode = inl(KBD_ADDR);
```
本质上就是让客户机 CPU 发起一次“对键盘设备地址的读操作”。

在 NEMU 里，这次读大致会沿着下面这条路径走：

1.  **客户机指令层**
    编译器把 `inl(KBD_ADDR)` 翻译为一次对固定地址的 load 指令。

2.  **访存入口**
    NEMU CPU 执行到这条 load 指令后，会发起一次地址读取。

3.  **物理读接口**
    进入：
    ```c
    paddr_read(addr, len)
    ```

4.  **判断地址类型**
    如果地址不在普通 `pmem` 范围内，就转到：
    ```c
    mmio_read(addr, len)
    ```

5.  **MMIO 设备分发**
    `mmio_read()` 会根据地址从 MMIO 表中找到：
    *   这段地址属于哪个设备；
    *   这个设备的寄存器空间是什么；
    *   绑定的回调函数是谁。

6.  **设备回调触发**
    最关键的一步在 `map_read()`：
    ```c
    invoke_callback(map->callback, offset, len, false);
    ```
    也就是说：
    **在真正读取设备寄存器存储区之前，先让设备自己更新状态。**

7.  **键盘设备响应**
    键盘回调 `i8042_data_io_handler()` 被调用，其中执行：
    ```c
    i8042_data_port_base[0] = key_dequeue();
    ```
    含义是：
    *   从 `key_queue` 取出一个等待中的键盘事件；
    *   把它写入键盘数据寄存器。

8.  **返回读值**
    回调结束后，`map_read()` 再从 `i8042_data_port_base[0]` 里把数据读出来，返回给客户机 CPU。

于是这一整套动作的真实语义就是：

> 客户机 CPU 读取键盘寄存器  
> -> NEMU 先让键盘设备回调刷新这次读应该看到的值  
> -> 键盘设备把队列中的下一条事件写入寄存器  
> -> CPU 最终读到这个寄存器值

这个设计也解释了为什么当前键盘模型是“轮询式”的：
*   设备不会主动把值推给软件；
*   软件必须主动来读；
*   读的时候设备才返回当前事件。

<a id="sec-kbd-10"></a>
### 10. 最终返回应用：为什么应用看到的是 `keydown/keycode`，而不是一个整数？

因为 AM 驱动已经做了最后一层“结构化翻译”。

底层返回给 AM 的是一个编码后的 32 位值：
*   高位：是否按下 (`KEYDOWN_MASK`)
*   低位：键码值

AM 层把它拆成：
```c
kbd->keydown = (scancode & KEYDOWN_MASK) ? 1 : 0;
kbd->keycode = scancode & ~KEYDOWN_MASK;
```

因此到了应用层，你看到的就变成：
*   `ev.keydown`
*   `ev.keycode`

而不是去自己解析位掩码。

这正是 AM 存在的一个重要意义：

> 它把“寄存器位语义”提升成“结构化设备语义”。

<a id="sec-kbd-11"></a>
### 11. 用一张分层时序图，把键盘这条链路整体串起来

#### 11.1 三层总图

```text
[第 1 层] 设备寄存器层 / NEMU 硬件模拟层
------------------------------------------------
宿主机按键
  ->
SDL 事件
  ->
send_key(scancode, is_keydown)
  ->
key_queue 里缓存键盘事件
  ->
客户机读取 KBD_ADDR 时
i8042_data_io_handler()
  ->
把一个键盘事件写入 i8042_data_port_base[0]
  ->
CPU 通过 MMIO 读到这个寄存器值

这一层里，客户机看到的是“设备寄存器”
本质上是 NEMU 模拟出来的寄存器状态
```

```text
[第 2 层] AM 驱动/平台适配层
------------------------------------------------
AM 的键盘驱动执行：

inl(KBD_ADDR)
  ->
拿到一个 32 位原始值 scancode
  ->
拆解成：
keydown = 高位标志
keycode = 低位键码
  ->
填入 AM_INPUT_KEYBRD_T 结构体

这一层做的是：
“访问底层寄存器” + “把原始值整理成统一接口”
```

```text
[第 3 层] 上层应用层
------------------------------------------------
应用调用：

io_read(AM_INPUT_KEYBRD)
  ->
得到 AM_INPUT_KEYBRD_T
  ->
只关心：
这个键是否按下
键码是多少

应用不需要关心：
- KBD_ADDR 是多少
- 这是 MMIO 还是 PIO
- NEMU 内部有没有 key_queue
- SDL 扫描码如何转换
```

#### 11.2 全链路箭头图

```text
宿主机键盘事件
  ->
[NEMU] key_queue / i8042 寄存器
  ->
[AM驱动] 读 KBD_ADDR，解码成 AM_INPUT_KEYBRD_T
  ->
[应用] 读取 keydown/keycode
```

#### 11.3 最终的详细时序图

```text
宿主机按键
-> SDL 产生事件
-> NEMU send_key()
-> 写入 key_queue
-> 应用调用 io_read(AM_INPUT_KEYBRD)
-> AM 驱动读 KBD_ADDR
-> NEMU 触发键盘设备回调
-> key_dequeue() 取出一个事件写入设备寄存器
-> MMIO 读返回该值
-> AM 解码成 keydown/keycode
-> 应用拿到键盘事件结构体
```

<a id="sec-kbd-12"></a>
### 12. 最终总结：键盘案例里，软硬件协同到底是什么？

通过键盘这个例子，可以把“软硬件协同”总结成下面几句话：

1.  **软件不是直接读宿主机键盘**  
    它只是去读客户机视角下的“键盘设备寄存器”或调用 AM 设备接口。

2.  **AM 不是创造一套新的硬件寄存器**  
    它提供的是统一设备接口和统一数据结构，把底层寄存器访问包装成平台无关的调用方式。

3.  **NEMU 才是底层设备语义的实现者**  
    它负责把宿主机 SDL 事件，转化成客户机可读的键盘寄存器值。

4.  **所谓“抽象寄存器”的真正意义**  
    更准确地说，应理解为：
    *   抽象设备接口编号；
    *   抽象设备数据结构；
    *   以及 AM 对底层寄存器值的统一语义包装。

5.  **键盘模型的本质是轮询式协同**  
    不是设备主动把值推给应用，而是：
    *   NEMU 先缓存事件；
    *   AM/软件主动发起读取；
    *   设备在读路径上返回当前事件。

因此，键盘这条链路最本质的一句话可以写成：

> 宿主机事件先进入 NEMU 设备模型，再通过客户机设备寄存器，被 AM 读取并翻译给应用。

也可以进一步压缩成：

> 上层软件看到的是统一设备接口，中层 AM 负责寄存器访问与结构化翻译，底层 NEMU 负责把外部现实事件变成客户机可读的设备寄存器状态。

[回到顶部目录](#sec-pa2-toc)

---

<br>
<a id="sec-pa2-audio"></a>
## PA2 专题：pa2-声卡设计，音频流控与并发同步的软硬件双向全栈总结

在开发 PA2 声卡功能时，我们需要处理软硬件的相互配合、MMIO 通信协议、以及上层生产者与底层独立线程消费者之间的流控同步问题。

本专题将依据“宏观水库 -> 中转调度站 -> 硬件底层水槽 -> 真实扬声器出口”的多级流控设计模型，对声卡的全技术栈进行结构化梳理。理解该流程的核心在于掌握**两级切块分发设计**、**状态机保护（pending_count 的分离）**，以及 **SDL 抽象的回调与时钟机制**。

<a id="sec-audio-sdl"></a>
### 1. 核心与前提：SDL 音频子系统数据结构与交互规范

在这个声卡架构的最底端出口是宿主机的真实扬声器，而控制宿主机扬声器播放的是 **SDL (Simple DirectMedia Layer)** 的媒体子模块。首先总结这里用到的 SDL 结构与函数：

#### 1.1 核心数据结构 `SDL_AudioSpec`
`SDL_AudioSpec` 可以看作是软件和物理硬件定下的“音频播放契约”，涵盖了消费者应该怎么解析这些数据：
- **`freq` (int)**: 采样频率（如 8000 Hz）。即一秒钟向喇叭送多少次数据。
- **`format` (SDL_AudioFormat)**: 采样格式。这里用 `AUDIO_S16SYS`，表示有符号 16 位整数，并按照系统字节序。这就决定了音量幅度的刻度。
- **`channels` (Uint8)**: 声道数。1为单声道，2为立体声。
- **`samples` (Uint16)**: 一次拉取回调需要多少个采样点，比如 1024。注意它并非字节数，实际字节数 = `samples * channels * 2`(16位是2字节)。
- **`callback` (SDL_AudioCallback)**: 这是一个函数指针，指向**独立的音频抓取函数**。它是整个拉取（Pull）模型的核心。
- **`userdata` (void*)**: 传给 callback 的自定义数据，本例不需要，设为 `NULL`。

#### 1.2 核心拉取回调函数 `sdl_audio_callback`

```c
void sdl_audio_callback(void *userdata, Uint8 *stream, int len);
```
- 这是整个流控出口的水龙头，**由 SDL 音频独立线程基于内置时钟周期性调用**。
- `stream`: 缓冲区指针，指向宿主机真实声卡播放队列的内存地址，我们需要将解出来的数据填入它里面。
- `len`: 宿主机本次向你“索要”的字节长度（与 `samples` 直接挂钩）。
- **强制规则**：你的回调函数**必须**往 `stream` 里面填满 `len` 个字节！如果缓冲不够，填不齐，绝不能抛开不管，不然声卡就会播放杂乱内存发出巨大的“爆音”。这也是为什么当缓冲不足时需要使用 `memset` 填入 `0`（静音）的原因。

#### 1.3 操作核心函数
- `SDL_InitSubSystem(SDL_INIT_AUDIO)`: 初始化音频子模块。
- `SDL_OpenAudioDevice(NULL, 0, &want, &have, 0)`: 根据前面的 `want` 参数契约向系统申请打开宿主的默认音频外设，并返回设备句柄 `audio_dev`。
- `SDL_PauseAudioDevice(audio_dev, 0)`: 激活该外设通道，使得 `callback` 开始被周期性回调。

---

<br>
<a id="sec-audio-layer1"></a>
### 2. 架构第一层：应用层 (Application / `am-tests`) —— “宏观水泵”

应用层并不关心硬件声卡具体大小和配置，它负责从抽象意义上加载音乐并发起播放指令。

**应用层核心流程**：
1. **核对身份**：读取 `AM_AUDIO_CONFIG` 获知设备是否在总线上。
2. **制定合同**：写入 `AM_AUDIO_CTRL`，传入比如 `freq=8000, channels=1, samples=1024`。这三条指令其实也就是为了喂给底层 SDL 配置库。
3. **定量分发任务（宏观切块）**：
   完整的测试音频可能超过上百 KB 或几 MB，如果应用层一次性把这些全丢给驱动，势必会导致驱动里原地死等造成程序系统假死挂起。所以上层设立了一个阈值：每次最多传递 **4096 字节**。
   ```c
   int len = (audio_len - nplay > 4096 ? 4096 : audio_len - nplay);
   sbuf.end = sbuf.start + len;
   io_write(AM_AUDIO_PLAY, sbuf); // 发包给 AM 层
   ```
   **作用**：保证业务层与驱动层解耦。一首完整歌曲被切分为很多块，业务逻辑只需通过循环去推送任务，间隙可以输出 `printf` 通知日志或者执行物理模拟。

---

<br>
<a id="sec-audio-layer2"></a>
### 3. 架构第二层：AM 驱动层 (`__am_audio_play`) —— “前线调度流水线与环形队列”

驱动层收到上层应用最高 `4096` 字节的数据后，负责流控和具体的写入搬运。AM 层负责向硬件发请求。

**核心运作与流控逻辑**：
1. **读表盘 (Flow Control 基础)**：向底层的硬件索要其当前的水位状态 `sbuf_size - inl(AUDIO_COUNT_ADDR)`，得出当前硬件外设环形缓冲区还能装多少数据（`free_space`）。
2. **死等阻塞机制 (Spin-Wait)**：
   如果 `free_space == 0`，代表被底层 SDL 消费太慢，产生拥堵，此时进入 `continue` 开启轮询等效于死等，从而通过时钟悬停挂起主程序，保证了这批数据不可能被覆盖。
3. **微观切块 (Micro-chunking)**：
   有可能当前剩下了 `1000` 字节空隙，但业务传进来了 `4096`。这时候 AM 需要自己执行二次分切：
   `size_t able_copy = len < free_space ? len : free_space;` 
   仅仅拷入空闲部分的额度数据，并将剩余数据留存在外面的循环。
4. **环形缓冲截断拷贝管理**：
   要填的数据被定为 `able_copy`，那应该放在 `sbuf` 的哪个位置？
   驱动自己维护了静态指针 `write_pos`，表示当前该从什么偏移开始拼接数据。
   如果放进去后，越过了 `sbuf_size`，就代表**跨过了缓冲区的末尾物理界限，需要将数据截断分两批拷贝**。
   第一批（`first_part`）贴在缓冲区的尾部，第二批（`second_part`）从缓冲池的 `[0]` 起点重新开始贴，形成环形的绕回效果。最后取模更新自己的 `write_pos`。
5. **硬件反馈更新**：
   使用 `outl(AUDIO_COUNT_ADDR, able_copy)` 机制主动联络底层 NEMU 硬件，告知本次新加塞（增量）的数据尺寸，使硬件更新记录。

---

<br>
<a id="sec-audio-layer3"></a>
### 4. 架构第三层：抽象通信协议层 (MMIO) —— “设备仪表盘”

位于软件层与 Nemu 模拟器最严格的中间带就是 `amdev.h` 抽象的**六个虚拟控制寄存器和一条物理流数据专线**。

- **数据通路 (Data Plane)**：物理区域 `sbuf` —— 这是完全交给 AM 去作为“水槽”内存拷贝操作的区域。直接针对大块宿主内存 `memcpy`。
- **控制通路 (Control Plane)**：以 `AUDIO_ADDR` 为基址的各项参数指令，当发出 `inl/outl` 时，Nemu 的 `audio_io_handler` 就是这些抽象寄存器的拦截看守。

---

<br>
<a id="sec-audio-layer4"></a>
### 5. 架构第四层：NEMU 硬件模拟层 (`audio.c`) —— “保护锁与底层水槽”

NEMU 肩负着“虚构出物理元件存活以及管理状态机”的重担。

**运作流程与关键设计：**
1. **初始化物理元件**：
   在 `init_audio()` 时，开辟 `sbuf_size(64KB)` 的虚拟物理环境以及注册那 6 个可接受读写的寄存器指针位置。并启动了 SDL_InitSubSystem。此时设备通了电但没有任何音频播放工作。
2. **屏障同步点 `reg_init`**：
   当上层 AM 写完频率与采样格式等设定，并最终 `outl(AUDIO_INIT_ADDR, 1)` 后，这就成了激活系统的号角声。这时候 NEMU 清理所有计数并启动 `SDL_OpenAudioDevice`。丢弃废数据，彻底应用新格式的开始点！
3. **最巧妙的多线程安全锁 —— 硬件 `pending_count`**:
   NEMU 和 SDL 本身运行在不同的时空中，怎么保障记录数量不会竞争死锁？
   由于 **`reg_count` (AM可读写) 与 `pending_count`(只有 NEMU 底层知晓)** 是分离的两个概念。
   上层通过 `outl(AUDIO_COUNT_ADDR, able_copy)` 发增量数据。
   Nemu 拿着上层的 `delta`，累加进入真正的物理水位表 `pending_count += delta`，同时加上防御性屏障（如果超出物理容量，强行截断 `delta` 防止溢出）。当底层消费走一些数据后，再把 `pending_count` 回印给 `reg_count`，保障了上层探查的信息精准性。

---

<br>
<a id="sec-audio-layer5"></a>
### 6. 架构第五层：SDL 宿主消费者 —— “匀速出水口”

处在另外一条时空线的 SDL 子模块，拥有它独立的生命运转法则：

**核心 `sdl_audio_callback` 运作流**：
1. **基于拉取的时钟工作**：
   系统独立周期性地索求 `len` 个字节。不论你的 AM 端在不在写入状态，只要在运行它就不会停下来。
2. **防干转的静音填补机制**：
   取数据的时候查看 NEMU 的 `pending_count`，如果水位低于要求的 `len` (比如发生了卡顿断流，甚至根本没来得及给水)，则仅将真实的那些剩余拉走。但对于声卡而言必须吸够水否则会发出“音爆噪点（干转）”的特性，果断往宿主机 `stream` 中填充代表静空（`0`）的 `memset` 数据保驾护航。
3. **自我维护读取位移**：
   消费完之后独立推进自己的环形指针偏移量 `read_pos = (read_pos + able_copy) % sbuf_size;` 并且削减 `pending_count -= able_copy`，最终同步印还给 `reg_count` 寄存器。

---

<br>
<a id="sec-audio-layer6"></a>
### 7. 设计全景总结回顾

**整个声卡系统就是一个配合高度默契的多级水处理厂**：

应用层向 AM 驱动进行**定量倒水决策**；AM 驱动负责看着 NEMU 的控制面板寄存器进行**流控降压与缓冲水池（sbuf）的首尾导流环形拼接**；抽象层利用 MMIO **切分了数据层与控制层（规避控制干涉效率）**；NEMU 硬件从底层维护着真实物理状态面板 **防止上层干扰物理真实水位量** 防止异常；SDL 架构利用子线程模型作为一台永不疲倦的**抽水泵（CallBack）匀速拉取向真实扬声器送水**并添加静音液防止抽头干转爆音。

[回到顶部目录](#sec-pa2-toc)
