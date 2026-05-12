# NEMU / AM / Makefile 脚本逻辑指南

> 记录时间：2026年3月8日  
> 目标：把 `ysyx-workbench` 中和 `riscv32-nemu` 运行链路直接相关的 `Makefile` / `*.mk` 脚本关系、参数来源、include 嵌套、变量流向一次性讲清楚。  
> 阅读定位：这不是“某一个文件注释”，而是一份面向整个构建系统的脚本导航手册。

---

<a id="top"></a>

## 目录

- [零、Makefile 语法前导速查](#preface-make-syntax)
  - [自动变量总表](#preface-auto-vars-table)
  - [自动变量明细](#preface-auto-vars-detail)
  - [GNU Make 内置函数总览](#preface-functions-overview)
  - [GNU Make 内置函数明细](#preface-functions-detail)
- [零点五、术语表 / 变量速查页](#preface-glossary)
  - [核心变量总表](#preface-glossary-table)
  - [最容易混淆的变量对照](#preface-glossary-confusions)
- [一、先给出一句话总纲](#section-overview)
- [二、阅读这套脚本前必须先建立的三层认知](#section-cognition)
  - [用 ysyx 三层执行模型理解 Makefile 执行规则](#section-cognition-exec-model)
- [三、全局脚本层级总图](#section-hierarchy)
- [四、逐个脚本拆解：核心功能 + 核心变量 + 来源 + 作用](#section-script-breakdown)
  - [第一部分：工作区 / 应用选择层](#part-workspace-app)
  - [第二部分：AbstractMachine 总控层](#part-am-core)
  - [第三部分：`abstract-machine/scripts/` 细化层](#part-am-scripts)
  - [第四部分：NEMU 侧构建与运行层](#part-nemu-build)
- [五、脚本之间的逻辑关系梳理](#section-relations)
- [六、变量流向图](#section-var-flow)
- [七、include 嵌套图](#section-include-graph)
- [八、最容易混淆的几个点](#section-confusions)
- [九、最终总结](#section-summary)
- [十、实战入口链路：从 `make ARCH=riscv32-nemu ALL=string run` 到 NEMU 启动](#section-e2e-run)
  - [1. 一图看完整调用链](#section-e2e-run-chain)
  - [2. 分层调用明细（上游 -> 下游）](#section-e2e-run-layers)
  - [3. 变量生命周期总表（谁产生，谁消费）](#section-e2e-run-vars)
  - [4. 参数如何落到 monitor.c 的 `elf_file` / `img_file` / `log_file`](#section-e2e-run-parse)
  - [5. 最终命令形态与排错抓手](#section-e2e-run-debug)
- [文末返回顶部](#top)

---

<a id="preface-make-syntax"></a>

## 零、Makefile 语法前导速查

这一部分是把 `makefile.md` 里的“自动变量 + GNU Make 内置函数”浓缩进 `scripts-logic.md` 的前置索引版，目的不是替代完整统计，而是让你在阅读后面的脚本链路前，先把最常见的语法工具抓牢。

如果后面读到：

* `$@` / `$<` / `$^` / `$*`
* `$(shell ...)`
* `$(call ...)`
* `$(foreach ...)`
* `$(eval ...)`
* `$(addprefix ...)`
* `$(basename ...)`

优先回到这一节查，再继续往下看脚本逻辑，会顺畅很多。

---

<a id="preface-auto-vars-table"></a>

### 1. 自动变量总表

| 自动变量 | 变量使用格式 | 核心含义 | 当前工程常见用途 |
| :--- | :--- | :--- | :--- |
| `$@` | `$@`、`$(@D)`、`$(@:.o=.d)` | 当前规则的目标文件名 | 输出目标、派生依赖文件、取目标目录 |
| `$<` | `$<`、`$(realpath $<)` | 当前规则的第一个依赖文件 | 编译输入源文件、动态生成临时 Makefile |
| `$^` | `$^`、`$(^D)` | 当前规则的全部依赖（去重） | 归档库、链接程序、递归进入依赖目录 |
| `$*` | `$*`、`Makefile.$*` | 模式规则中 `%` 匹配到的 stem | 提取测试名、生成临时目标名 |
| `$(@D)` | `$(@D)` | 当前目标 `$@` 的目录部分 | `mkdir -p $(@D)`、进入目标目录 |
| `$(^D)` | `$(^D)` | 全部依赖 `$^` 的目录部分 | 基于依赖目录做递归 make |
| `$(@:pattern=replacement)` | `$(@:.o=.d)` 等 | 先取自动变量再做替换 | 从目标名反推 `.d`、裁剪展示路径 |

---

<a id="preface-auto-vars-detail"></a>

### 2. 自动变量明细

#### 1）`$@`
- **变量名**：`$@`
- **变量使用格式**：`$@`、`$(@D)`、`$(@:.o=.d)`
- **核心含义**：当前规则的目标文件名
- **典型功能**：作为 `-o $@` 的输出目标，或进一步派生目录和依赖文件名

#### 2）`$<`
- **变量名**：`$<`
- **变量使用格式**：`$<`、`$(realpath $<)`
- **核心含义**：当前规则的第一个依赖文件
- **典型功能**：作为编译输入文件，或写入动态生成的中间 Makefile

#### 3）`$^`
- **变量名**：`$^`
- **变量使用格式**：`$^`、`$(^D)`
- **核心含义**：当前规则的所有依赖文件列表（自动去重）
- **典型功能**：把全部对象文件打成 `.a`，或把全部依赖拿去链接

#### 4）`$*`
- **变量名**：`$*`
- **变量使用格式**：`$*`、`Makefile.$*`
- **核心含义**：模式规则中 `%` 匹配到的 stem
- **典型功能**：在 `Makefile.%` 这种规则里提取 `string`、`add` 这样的测试名

#### 5）`$(@D)`
- **变量名**：`$(@D)`
- **变量使用格式**：`$(@D)`
- **核心含义**：目标 `$@` 的目录部分
- **典型功能**：在写目标文件前执行 `mkdir -p $(@D)`

#### 6）`$(^D)`
- **变量名**：`$(^D)`
- **变量使用格式**：`$(^D)`
- **核心含义**：依赖列表 `$^` 的目录部分
- **典型功能**：基于依赖目录执行 `$(MAKE) -C $(^D)` 这类递归调用

#### 7）`$(@:pattern=replacement)` 类形式
- **变量名**：基于自动变量的替换引用
- **变量使用格式**：`$(@:.o=.d)`、`$(@:$(OBJDIR)/%=%)`
- **核心含义**：先取自动变量值，再做模式替换
- **典型功能**：将自动变量@ 所有的pattern 替换成 replacement

<a id="preface-functions-overview"></a>

### 3. GNU Make 内置函数总览

当前工作区里实际高频出现的 GNU Make 内置函数，可以先按五类记忆：

#### A. 路径处理类

* `abspath`
* `realpath`
* `dir`
* `notdir`
* `basename`

#### B. 列表处理类

* `addprefix`
* `addsuffix`
* `filter`
* `filter-out`
* `sort`
* `word`
* `words`

#### C. 字符串处理类

* `subst`
* `patsubst`
* `findstring`

#### D. 控制 / 元编程类

* `call`
* `eval`
* `foreach`
* `if`
* `flavor`

#### E. 环境交互 / 信息提示类

* `shell`
* `wildcard`
* `info`
* `warning`
* `error`

这五类要先建立直觉：

> **Makefile 的难点往往不在“命令本身”，而在“命令前面这一串路径、列表、字符串、元编程函数是怎么把变量拼出来的”。**

---

<a id="preface-functions-detail"></a>

### 4. GNU Make 内置函数明细

#### 1）`abspath`
- **函数名**：`abspath`
- **函数原型**：`$(abspath names...)`
- **参数意义**：`names...` 是一个或多个路径字符串
- **函数功能**：把路径转换成绝对路径，但不解析符号链接

#### 2）`addprefix`
- **函数名**：`addprefix`
- **函数原型**：`$(addprefix prefix,names...)`
- **参数意义**：`prefix` 是前缀，`names...` 是目标列表
- **函数功能**：给列表里的每一项统一加前缀

#### 3）`addsuffix`
- **函数名**：`addsuffix`
- **函数原型**：`$(addsuffix suffix,names...)`
- **参数意义**：`suffix` 是后缀，`names...` 是目标列表
- **函数功能**：给列表里的每一项统一加后缀

#### 4）`basename`
- **函数名**：`basename`
- **函数原型**：`$(basename names...)`
- **参数意义**：`names...` 是文件名或路径列表
- **函数功能**：去掉每个名字最后的扩展后缀(shell ①文件有路径，去掉路径留文件 ②仅有文件，去掉文件后缀)

#### 5）`call`
- **函数名**：`call`
- **函数原型**：`$(call variable,param1,param2,...)`
- **参数意义**：`variable` 是自定义宏名，后面是传入参数，对应 `$(1)`、`$(2)` ...
- **函数功能**：调用一个用户自定义宏并向其传参

#### 6）`dir`
- **函数名**：`dir`
- **函数原型**：`$(dir names...)`
- **参数意义**：`names...` 是路径列表
- **函数功能**：提取路径中的目录部分

#### 7）`error`
- **函数名**：`error`
- **函数原型**：`$(error text...)`
- **参数意义**：`text...` 是错误提示文本
- **函数功能**：立即终止 Make，并打印错误信息

#### 8）`eval`
- **函数名**：`eval`
- **函数原型**：`$(eval text)`
- **参数意义**：`text` 是需要再次当作 Makefile 语法解释的文本
- **函数功能**：把动态生成的规则或变量定义重新交给 Make 解析

#### 9）`filter`
- **函数名**：`filter`
- **函数原型**：`$(filter pattern...,text)`
- **参数意义**：`pattern...` 是保留模式，`text` 是待筛选列表
- **函数功能**：从 TEXT 中 筛选 能匹配任一 PATTERN 的单词，返回text剩余不匹配的单词。

#### 10）`filter-out`
- **函数名**：`filter-out`
- **函数原型**：`$(filter-out pattern...,text)`
- **参数意义**：`pattern...` 是排除模式，`text` 是原始列表
- **函数功能**：从 TEXT 中 剔除 能匹配任一 PATTERN 的单词，返回text剩余不匹配的单词。

#### 11）`findstring`
- **函数名**：`findstring`
- **函数原型**：`$(findstring find,in)`
- **参数意义**：`find` 是待查子串，`in` 是目标字符串
- **函数功能**：若找到子串则返回该子串，否则返回空

#### 12）`flavor`
- **函数名**：`flavor`
- **函数原型**：`$(flavor variable)`
- **参数意义**：`variable` 是变量名
- **函数功能**：返回变量类型，例如 `undefined`、`recursive`、`simple`

#### 13）`foreach`
- **函数名**：`foreach`
- **函数原型**：`$(foreach var,list,text)`
- **参数意义**：`var` 是循环变量，`list` 是待遍历列表，`text` 是每轮展开模板
- **函数功能**：把 list 中的每个元素依次赋值给 var，然后执行 text，最后把所有结果拼接起来（用空格分隔）

#### 14）`if`
- **函数名**：`if`
- **函数原型**：`$(if condition,then-part,else-part)`
- **参数意义**：`condition` 是条件，`then-part` / `else-part` 是真/假时展开内容
- **函数功能**：在变量展开阶段执行条件分支

#### 15）`info`
- **函数名**：`info`
- **函数原型**：`$(info text...)`
- **参数意义**：`text...` 是提示信息
- **函数功能**：打印提示文本但不中断执行

#### 16）`notdir`
- **函数名**：`notdir`
- **函数原型**：`$(notdir names...)`
- **参数意义**：`names...` 是路径列表
- **函数功能**：去掉目录部分，只保留文件名

#### 17）`patsubst`
- **函数名**：`patsubst`
- **函数原型**：`$(patsubst pattern,replacement,text)`
- **参数意义**：`pattern` 是模式，`replacement-在 replacement 中，% 代表匹配到的那部分` 是替换模板，`text` 是原始文本
- **函数功能**：对 text 中每个“单词”，按 pattern 进行匹配，匹配成功就替换成 replacement

#### 18）`realpath`
- **函数名**：`realpath`
- **函数原型**：`$(realpath names...)`
- **参数意义**：`names...` 是路径列表
- **函数功能**：返回真实绝对路径，并解析符号链接

#### 19）`shell`
- **函数名**：`shell`
- **函数原型**：`$(shell command)`
- **参数意义**：`command` 是一条 shell 命令
- **函数功能**：执行 shell 命令，并把标准输出作为返回值

#### 20）`sort`
- **函数名**：`sort`
- **函数原型**：`$(sort list)`
- **参数意义**：`list` 是待排序列表
- **函数功能**：排序并自动去重

#### 21）`subst`
- **函数名**：`subst`
- **函数原型**：`$(subst from,to,text)`
- **参数意义**：`from` 是原字符串，`to` 是新字符串，`text` 是目标文本
- **函数功能**：在整个 text 中，把所有 from 字符串替换为 to

#### 22）`warning`
- **函数名**：`warning`
- **函数原型**：`$(warning text...)`
- **参数意义**：`text...` 是警告文本
- **函数功能**：打印警告，但不终止执行

#### 23）`wildcard`
- **函数名**：`wildcard`
- **函数原型**：`$(wildcard pattern...)`
- **参数意义**：`pattern...` 是路径匹配模式
- **函数功能**：返回实际存在的匹配文件列表，不存在则返回空

#### 24）`word`
- **函数名**：`word`
- **函数原型**：`$(word n,text)`
- **参数意义**：`n` 是第几个词，`text` 是词列表
- **函数功能**：从空格分隔的列表中取第 `n` 项

#### 25）`words`
- **函数名**：`words`
- **函数原型**：`$(words text)`
- **参数意义**：`text` 是词列表
- **函数功能**：统计列表项个数

---

### 5. 这一节和后文脚本逻辑的联动关系

后面你在读：

* `abstract-machine/Makefile`
* `nemu/Makefile`
* `platform/nemu.mk`
* `cpu-tests/Makefile`

时，几乎反复会碰到两类模式：

#### 模式一：路径 / 列表拼接

例如：

```makefile
$(addprefix $(DST_DIR)/, $(addsuffix .o, $(basename $(SRCS))))
```

这一串本质就是：

1. 先把 `SRCS` 去后缀；
2. 再统一补 `.o`；
3. 再统一补上对象目录前缀。

#### 模式二：模板化规则生成

例如：

```makefile
$(foreach lib, $(LIBS), $(eval $(call LIB_TEMPLATE,$(lib))))
```

这一串本质就是：

1. `foreach` 遍历库列表；
2. `call` 调用用户自定义宏；
3. `eval` 把宏展开结果重新当作规则注入 Make。

所以这一节的最终目的不是记定义，而是建立这种直觉：

> **后文大多数“看起来很长的 Make 表达式”，本质都是若干路径函数、列表函数和元编程函数的组合。**

---

<a id="preface-glossary"></a>

## 零点五、术语表 / 变量速查页

这一节不再讲 GNU Make 语法，而是专门把当前这套 `ysyx-workbench` 运行链里最常出现、也最容易串线的变量集中压成一页速查。

如果你后面又看见：

* `ARCH`
* `ISA`
* `PLATFORM`
* `NAME`
* `SRCS`
* `IMAGE`
* `OBJS`
* `LINKAGE`
* `ARGS`
* `IMG`
* `BINARY`
* `NEMU_EXEC`

优先回来看这一节，能比在正文里来回翻快很多。

---

<a id="preface-glossary-table"></a>

### 1. 核心变量总表

| 变量 | 主要出现层次 | 典型来源 | 核心作用 | 当前链路中的典型值/含义 |
| :--- | :--- | :--- | :--- | :--- |
| `ARCH` | 第一层 / 第二层 | 命令行传入 | 指定“ISA + 平台”的复合目标 | `riscv32-nemu` |
| `ISA` | 第二层 / 第三层 | 由 `ARCH` 拆分，或外层递归透传 | 表示客户机指令集 | `riscv32` |
| `PLATFORM` | 第二层 | 由 `ARCH` 拆分 | 表示客户程序运行平台 | `nemu` |
| `NAME` | 第一层 / 第二层 / 第三层 | 应用入口文件定义，或 NEMU 自己派生 | 表示“当前构建对象叫什么” | 应用侧是 `string`；NEMU 侧是 `riscv32-nemu-interpreter` |
| `SRCS` | 第一层 / 第二层 / 第三层 | 应用入口给出，或 NEMU 源码汇总 | 当前要编译的源文件集合 | 例如 `tests/string.c` 或 NEMU 的 `src/**.c` 列表 |
| `WORK_DIR` | 第二层 / 第三层 build 规则 | `$(shell pwd)` | 当前这一轮 make 的工作目录 | 例如 `am-kernels/tests/cpu-tests` 或 `nemu/` |
| `DST_DIR` | 第二层 | 由 `WORK_DIR` 与 `ARCH` 派生 | 应用对象文件输出目录 | `.../build/riscv32-nemu` |
| `IMAGE` | 第二层 / 第三层前半 | 由 `NAME` 与 `ARCH` 派生 | 应用镜像的公共前缀 | `.../build/string-riscv32-nemu` |
| `OBJS` | 第二层 / 第三层 build 规则 | 从 `SRCS` 派生 | 对象文件集合 | 例如 `.../tests/string.o` |
| `LINKAGE` | 第二层 | 初始来自 `OBJS`，后续追加库 | 最终参与链接 ELF 的输入集合 | 应用 `.o` + `am/klib` 等 `.a` |
| `LDSCRIPTS` | 第二层 / 第三层前半 | 平台脚本追加 | 链接脚本列表 | 例如 `abstract-machine/scripts/linker.ld` |
| `NEMUFLAGS` | 第三层前半 | `platform/nemu.mk` 生成 | 传给 NEMU 的运行参数集合 | `-b -l .../nemu-log.txt` |
| `ARGS` | 第三层后半 | 外层透传或 `native.mk` 默认值 | NEMU 启动参数 | 本链路里等于外层传来的 `NEMUFLAGS` |
| `IMG` | 第三层前半 / 后半 | 外层透传 | 交给 NEMU 的客户程序镜像路径 | `.../string-riscv32-nemu.bin` |
| `GUEST_ISA` | 第三层后半 | `auto.conf` 的 `CONFIG_ISA` | NEMU 自己按哪种客户 ISA 被编译 | `riscv32` |
| `ENGINE` | 第三层后半 | `auto.conf` 的 `CONFIG_ENGINE` | NEMU 的执行引擎类型 | `interpreter` |
| `BINARY` | 第三层后半 | `build.mk` 派生 | NEMU 本体可执行文件路径 | `nemu/build/riscv32-nemu-interpreter` |
| `NEMU_EXEC` | 第三层后半 | `$(BINARY) $(ARGS) $(IMG)` | 最终真正交给 shell 执行的命令 | `NEMU 程序 + 参数 + 客户镜像` |

---

### 2. 按三层模型重新记这些变量

#### 第一层最关键的变量

* `ARCH`：用户指定的平台目标
* `ALL`：当前要跑哪个测试
* `MAKECMDGOALS`：当前目标，例如 `run`
* `NAME` / `SRCS`：被写入临时 `Makefile.<test>` 的最小任务描述

这一层变量的本质是：

> **先把“本轮任务是什么”说清楚。**

#### 第二层最关键的变量

* `ARCH` -> `ISA` + `PLATFORM`
* `WORK_DIR` / `DST_DIR`
* `IMAGE`
* `OBJS`
* `LINKAGE`
* `LDSCRIPTS`

这一层变量的本质是：

> **把任务转成一张可链接出 `.elf` 的构建图。**

#### 第三层最关键的变量

* `NEMUFLAGS`
* `ARGS`
* `IMG`
* `GUEST_ISA`
* `BINARY`
* `NEMU_EXEC`

这一层变量的本质是：

> **把客户程序镜像和模拟器本体一起收拢成最终运行命令。**

---

<a id="preface-glossary-confusions"></a>

### 3. 最容易混淆的变量对照

#### 1）`ARCH` vs `ISA`

* `ARCH`：复合目标，表示“指令集 + 平台”，例如 `riscv32-nemu`
* `ISA`：只表示客户机指令集，例如 `riscv32`

一句话区分：

> **`ARCH` 更像“完整运行目标”，`ISA` 更像“其中的指令集部分”。**

#### 2）`ISA` vs `GUEST_ISA`

* `ISA`：AM 侧从 `ARCH` 拆出来并继续往下透传给 NEMU 的值
* `GUEST_ISA`：NEMU 自己从 `auto.conf` 的 `CONFIG_ISA` 读出来的值

一句话区分：

> **`ISA` 是外层传给 NEMU 的期望值，`GUEST_ISA` 是 NEMU 自己内部真正采用的配置值。**

#### 3）`NAME` vs `IMAGE`

* `NAME`：只是当前程序的名字，例如 `string`
* `IMAGE`：由 `NAME + ARCH` 派生出来的镜像公共前缀，例如 `.../build/string-riscv32-nemu`

一句话区分：

> **`NAME` 是“名字”，`IMAGE` 是“产物前缀路径”。**

#### 4）`OBJS` vs `LINKAGE`

* `OBJS`：当前应用自己的对象文件集合
* `LINKAGE`：最终参与链接的更大集合，包含 `OBJS` 和递归库

一句话区分：

> **`OBJS` 只是应用零件，`LINKAGE` 是最终装配清单。**

#### 5）`NEMUFLAGS` vs `ARGS`

* `NEMUFLAGS`：`platform/nemu.mk` 在 AM 一侧准备好的运行参数
* `ARGS`：NEMU 一侧最终实际使用的参数变量

一句话区分：

> **`NEMUFLAGS` 是上游打包好的参数，`ARGS` 是下游接手后的运行参数入口。**

#### 6）`IMG` vs `BINARY`

* `IMG`：客户程序镜像，也就是被 NEMU 加载执行的 `.bin`
* `BINARY`：NEMU 本体，也就是运行在 Linux 上的宿主机程序

一句话区分：

> **`IMG` 是“被模拟执行的程序”，`BINARY` 是“执行模拟的人”。**

#### 7）`BINARY` vs `NEMU_EXEC`

* `BINARY`：只有 NEMU 可执行文件本体
* `NEMU_EXEC`：`BINARY + ARGS + IMG` 折叠后的完整命令

一句话区分：

> **`BINARY` 是程序文件，`NEMU_EXEC` 是完整启动命令。**

---

<a id="section-overview"></a>

## 一、先给出一句话总纲

这套脚本系统可以概括为一句话：

> **`am-kernels` 选择“要跑哪个客户程序”，`abstract-machine/Makefile` 负责把这个客户程序按 `ARCH=riscv32-nemu` 编译链接成镜像，`abstract-machine/scripts/` 下的脚本继续把“ISA 细节”和“平台细节”补齐，最后 `platform/nemu.mk` 再递归调用 `nemu/Makefile`，把 NEMU 模拟器本体构建出来，并把刚才生成的 `.bin` 镜像交给 NEMU 去执行。**

如果把层级写成一条最核心的调用链，就是：

```text
am-kernels/tests/cpu-tests/Makefile
-> 生成临时 Makefile.<test>
-> include abstract-machine/Makefile
-> include abstract-machine/scripts/riscv32-nemu.mk
-> include abstract-machine/scripts/isa/riscv.mk
-> include abstract-machine/scripts/platform/nemu.mk
-> 生成 .elf / .bin
-> make -C nemu ISA=riscv32 run ARGS="..." IMG=xxx.bin
-> nemu/Makefile
-> include nemu/scripts/config.mk
-> include nemu/scripts/native.mk
-> 运行 NEMU 执行客户程序
```

---

<a id="section-cognition"></a>

## 二、阅读这套脚本前必须先建立的三层认知

### 1. 第一层：这套系统里其实同时存在“两类程序”

#### 1）客户程序（Guest Program）
例如：

* `am-kernels/tests/cpu-tests/tests/string.c`
* `am-kernels/kernels/hello/hello.c`

它们会被交叉编译成 **RISC-V 机器码**，最终被 NEMU 执行。

#### 2）模拟器程序（Host Program）
例如：

* `nemu/src/...`

它本身是运行在宿主机 Linux 上的可执行文件，用来模拟一台客户机。

所以后续读脚本时，必须始终分清：

* 哪一层脚本是在构建“客户程序”；
* 哪一层脚本是在构建“NEMU 模拟器本体”。

---

### 2. 第二层：`ARCH` 与 `ISA` 不是同一个层面的变量

#### 1）`ARCH`
像：

```makefile
ARCH=riscv32-nemu
```

它是一个**复合平台描述**，同时包含：

* 指令集信息：`riscv32`
* 运行平台信息：`nemu`

#### 2）`ISA`
像：

```makefile
ISA=riscv32
```

它只表达“客户机指令集”。

所以通常是：

* AM 侧主要吃 `ARCH`
* NEMU 侧主要吃 `ISA`

---

### 3. 第三层：脚本系统本质上在完成三件事

#### 1）决定编什么
即：

* 哪个测试程序
* 哪个架构
* 哪个平台

#### 2）决定怎么编
即：

* 用哪个交叉编译器
* 用哪些 `CFLAGS` / `LDFLAGS`
* 用哪个链接脚本

#### 3）决定怎么跑
即：

* 把 `.elf` 提成 `.bin`
* 给 NEMU 传哪些参数
* 用哪份镜像启动 NEMU

---

<a id="section-cognition-exec-model"></a>

### 4. 用 ysyx 三层执行模型理解 Makefile 执行规则

这一节专门回答一个最容易反复困惑的问题：

> **Makefile 到底是怎么“执行起来”的？它为什么不是简单地从上到下跑命令？**

如果只从 GNU Make 教科书角度讲，会很抽象；但如果结合 `ysyx-workbench` 当前工程来看，这个问题其实可以直接压成三层执行模型：

1. **入口调度层**：决定“这次到底要编什么”
2. **AM 总控层**：决定“这些东西到底该怎么编”
3. **NEMU 运行层**：决定“编出来之后怎么交给模拟器去跑”

把这三层看清楚，Makefile 的执行规则就会从“零散语法点”变成一条完整链路。

#### 第一层：入口调度层 —— Make 首先要知道“本轮任务是什么”

在你当前工程里，最典型的入口是：

```bash
make ARCH=riscv32-nemu ALL=string run
```

这条命令进入 Make 后，不会立刻执行某条命令，而是先被 GNU Make 拆成三类信息：

##### 1）命令行变量

```text
ARCH = riscv32-nemu
ALL  = string
```

##### 2）目标

```text
run
```

##### 3）当前工作目录上下文

例如你是在：

```text
/home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests
```

下执行这条命令，那么这次 Make 进程的工作目录就是这个目录。

这一步非常关键，因为它说明：

> **Make 的第一件事不是执行命令，而是先建立“变量 + 目标 + 工作目录”这三个最基础的执行上下文。**

随后 `am-kernels/tests/cpu-tests/Makefile` 开始起作用。它并不负责真正编译程序，而是负责：

* 接收 `ALL=string`
* 决定这次只跑 `string`
* 动态生成一个 `Makefile.string`
* 再把任务递归转交给 AM 总控

这里一定要把“生成临时入口文件”和“进入第二层总控”拆开看，否则很容易误以为 `include $(AM_HOME)/Makefile` 本身就是一条会立即跳转的执行命令。实际上，这一层发生的是一个标准的“两段式委托”：

##### 4）先由外层规则生成一个最小临时入口

`cpu-tests/Makefile` 的关键规则是：

```makefile
Makefile.%: tests/%.c latest
  @/bin/echo -e "NAME = $*\nSRCS = $<\ninclude $${AM_HOME}/Makefile" > $@
  @if make -s -f $@ ARCH=$(ARCH) $(MAKECMDGOALS); then \
```

如果当前 `ALL=string`，那么 `%` 匹配到的 stem 就是 `string`，这一条规则会先生成：

```makefile
Makefile.string
```

它的内容本质上只有三行：

```makefile
NAME = string
SRCS = tests/string.c
include $(AM_HOME)/Makefile
```

这一步的本质不是编译，而是把“本轮任务”固化成一张最小任务卡：

* 当前测试名是谁：`NAME=string`
* 当前源文件是谁：`SRCS=tests/string.c`
* 后续构建总控由谁接管：`include $(AM_HOME)/Makefile`

所以 `Makefile.string` 的角色更准确地说不是“真正构建脚本”，而是：

> **外层调度器专门为本次测试动态生成的桥接入口。**

##### 5）再由外层 recipe 启动一个新的子 make 进程

临时文件生成完之后，真正的委托发生在第二行：

```makefile
make -s -f $@ ARCH=$(ARCH) $(MAKECMDGOALS)
```

以当前场景为例，它会展开成近似下面这条命令：

```bash
make -s -f Makefile.string ARCH=riscv32-nemu run
```

这里最关键的点是：

* `-f Makefile.string`：子 make 以这个临时文件作为入口文件；
* `ARCH=$(ARCH)`：把外层架构继续透传下去；
* `$(MAKECMDGOALS)`：把外层目标继续透传下去，例如这里还是 `run`。

也就是说，不是“当前 make 完全结束后，才去读下一份脚本”，而是：

> **当前外层 make 在执行 `Makefile.%` 这条规则时，主动启动了一个新的子 make，把后续工作递交给它。**

##### 6）`include $(AM_HOME)/Makefile` 发生在子 make 的解析阶段

这一点非常重要：

```makefile
include $(AM_HOME)/Makefile
```

不是 shell 命令，也不是运行时跳转语句，而是 GNU Make 的“文本并入/规则并入”语义。

也就是说，子 make 在读取 `Makefile.string` 时，遇到 `include $(AM_HOME)/Makefile`，会把：

```makefile
abstract-machine/Makefile
```

的内容继续读进来，并在同一个子 make 进程里完成规则解析、变量建立和目标求值。

所以这里真正的顺序应理解为：

1. 外层 `cpu-tests/Makefile` 决定当前测试任务；
2. 外层生成 `Makefile.string`；
3. 外层执行 `make -f Makefile.string ...`，启动子 make；
4. 子 make 解析 `Makefile.string`；
5. 子 make 通过 `include $(AM_HOME)/Makefile` 把 `abstract-machine/Makefile` 纳入进来；
6. 从这一刻起，真正的编译、链接、镜像生成与平台下发，才由 AM 总控层接手。

因此更精确地说：

> **`cpu-tests/Makefile` 是第一层入口调度器，`Makefile.<test>` 是第一层到第二层之间的桥接入口，而 `abstract-machine/Makefile` 才是第二层真正的总控脚本。**

所以入口调度层真正做的事情可以压缩为：

> **把“用户命令”翻译成“一个明确的构建任务”。**

#### 第二层：AM 总控层 —— Make 开始从“要做什么”转向“如何构建”

当入口层把任务下发后，控制权就进入：

```makefile
include $(AM_HOME)/Makefile
```

也就是 `abstract-machine/Makefile`。

这一层是 Make 执行规则的真正核心，因为它第一次系统性地建立出：

* 变量表
* 规则图
* 依赖关系
* 编译目标
* 输出路径

在这一层里，Make 做的事情不再是“挑测试”，而是进入标准的构建语义：

这里还要再补上一层非常重要的理解：

> **`abstract-machine/Makefile` 不是一份“单独就能解释完整运行链”的封闭脚本，它更像一份总控骨架：先接住上层传下来的 `NAME/SRCS/ARCH/MAKECMDGOALS`，再把架构脚本、平台脚本、编译规则、链接规则和最终目标拼装成一张完整构建图。**

换句话说，第二层的关键不只是“它会编译”，而是：

> **它负责把第一层传下来的最小任务描述，扩展成真正可执行的构建系统。**

##### 1）先做前置检查

例如：

* `AM_HOME` 是否有效
* `ARCH` 是否有对应脚本
* `SRCS` 是否已定义

这说明 Make 在真正执行规则前，首先会先把当前工程是否“可构建”检查一遍。

这里还要注意两个执行规则细节：

###### ① 默认目标不是永远写死的，而是由 `MAKECMDGOALS` 决定

文件一开始有：

```makefile
ifeq ($(MAKECMDGOALS),)
  MAKECMDGOALS  = image
  .DEFAULT_GOAL = image
endif
```

这意味着：

* 如果子 make 没显式指定目标，就默认构建 `image`；
* 如果上层已经透传了 `run`、`gdb`、`archive` 等目标，就按外部目标继续往下走。

所以 AM 总控层并不是“永远只负责生成镜像”，而是：

* 无目标时默认落到 `image`
* 有目标时服从上层透传的 `MAKECMDGOALS`

###### ② 不是所有目标都会做完整检查

文件中还有：

```makefile
ifeq ($(findstring $(MAKECMDGOALS),clean|clean-all|html),)
  ... checks ...
endif
```

这表示：

* `clean` / `clean-all` / `html` 这类目标，不需要完整的编译环境检查；
* 只有真正涉及构建的目标，才要求 `AM_HOME`、`ARCH`、`SRCS` 等条件都齐备。

因此 AM 总控层的第一步不是“直接开编”，而是：

> **先根据当前目标类型，判断本轮到底需不需要进入完整构建语义。**

##### 2）解析高层变量

例如：

```makefile
ARCH_SPLIT = $(subst -, ,$(ARCH))
ISA        = $(word 1,$(ARCH_SPLIT))
PLATFORM   = $(word 2,$(ARCH_SPLIT))
```

这一步不是执行命令，而是在建立更细的语义变量：

* `ARCH=riscv32-nemu`
* 被拆成 `ISA=riscv32`
* 和 `PLATFORM=nemu`

这里要注意，`ARCH` 的拆分不是为了“打印得更好看”，而是为了后面两件真实事情服务：

* `ISA` 继续向下传给 NEMU；
* `PLATFORM` 决定 AM 平台侧应该补哪些源码、链接脚本和运行目标。

##### 3）建立输出路径和中间产物路径

例如：

```makefile
WORK_DIR  = $(shell pwd)
DST_DIR   = $(WORK_DIR)/build/$(ARCH)
IMAGE_REL = build/$(NAME)-$(ARCH)
IMAGE     = $(abspath $(IMAGE_REL))
```

这里 Make 不是在“直接开始编译”，而是在先建立后续构建图里所有目标文件的落点。

同时还有一个很容易被忽略的点：

```makefile
$(shell mkdir -p $(DST_DIR))
```

这条命令会在解析阶段就执行，因此 `build/$(ARCH)` 目录并不是等某条 recipe 运行时才创建，而是 AM 总控在建立构建上下文时就先准备好了对象文件目录。

##### 4）建立规则图

例如：

```makefile
$(DST_DIR)/%.o: %.c
  @$(CC) -std=gnu11 $(CFLAGS) -c -o $@ $(realpath $<)
```

和：

```makefile
$(IMAGE).elf: $(LINKAGE) $(LDSCRIPTS)
  @$(LD) $(LDFLAGS) -o $@ --start-group $(LINKAGE) --end-group
```

这时 Make 已经把整个构建过程抽象成：

```text
.c / .S / .cc
-> .o
-> .a（可选递归库）
-> .elf
```

所以这一层的本质是：

> **Make 从“理解目标”进入“构建依赖图”。**

但这张图在 `abstract-machine/Makefile` 里并不是一次性写完的，而是分两段补齐：

###### 第一段：总控自己先声明通用骨架

例如：

* `OBJS` 如何从 `SRCS` 派生；
* `CC/LD/AR/OBJDUMP/OBJCOPY` 如何从 `CROSS_COMPILE` 派生；
* `.c/.cc/.cpp/.S -> .o` 的通用编译规则；
* `$(IMAGE).elf` 的通用链接规则；
* `image` / `archive` / `clean` 这些高层目标的基本框架。

###### 第二段：再由 `ARCH` 脚本把缺失部分补齐

最关键的一句是：

```makefile
-include $(AM_HOME)/scripts/$(ARCH).mk
```

以当前 `ARCH=riscv32-nemu` 为例，实际会继续纳入：

```makefile
abstract-machine/scripts/riscv32-nemu.mk
```

而这份脚本又继续纳入：

* `scripts/isa/riscv.mk`
* `scripts/platform/nemu.mk`

这样 AM 总控层才真正拿到了完整运行链所需要的补充内容，例如：

* `CROSS_COMPILE`
* `COMMON_CFLAGS`
* `LDSCRIPTS`
* `AM_SRCS`
* `run` / `gdb` 的平台目标实现

所以从执行规则角度看，`abstract-machine/Makefile` 并不是“自己一个文件把所有事做完”，而是：

> **先声明通用骨架，再通过 `ARCH` 脚本把具体平台的缺口补齐。**

##### 5）规则执行顺序不是按文件从上到下，而是按依赖关系决定

这也是 Makefile 最重要的执行规则之一。

比如在 `abstract-machine/Makefile` 中，虽然编译规则、归档规则、链接规则是按文本顺序写出来的，但真正执行时一定是：

1. 先编源文件得到 `.o`
2. 再递归生成 `am` / `klib` 的 `.a`
3. 最后链接生成 `.elf`

这不是因为“写在前面的先执行”，而是因为：

> **最终目标 `$(IMAGE).elf` 依赖 `$(LINKAGE)`，而 `$(LINKAGE)` 又依赖对象文件和库，所以依赖图强制了执行顺序。**

因此在 AM 总控层里，你必须始终用这种视角读 Makefile：

> **它是在声明一张构建图，而不是手写一个顺序脚本。**

##### 6）单独拆开看：当前程序到底是怎么变成 `.elf` 的？

你这里最容易卡住的点在于：`.elf` 不是某一条命令“突然生成”的，它是整张依赖图被满足之后，由链接规则最终产出的结果。

如果还是看你当前最典型的场景：

```bash
make -f Makefile.string ARCH=riscv32-nemu run
```

那么 `.elf` 的形成链可以先压成一行：

```text
SRCS
-> OBJS
-> LINKAGE
-> $(IMAGE).elf
```

把它展开后其实就是：

```text
tests/string.c
-> build/riscv32-nemu/tests/string.o
-> string.o + am/klib 等库
-> string-riscv32-nemu.elf
```

这里最关键的三个变量是：

```makefile
OBJS    = $(addprefix $(DST_DIR)/, $(addsuffix .o, $(basename $(SRCS))))
LINKAGE = $(OBJS)
IMAGE   = $(abspath build/$(NAME)-$(ARCH))
```

如果当前：

* `NAME = string`
* `SRCS = tests/string.c`
* `ARCH = riscv32-nemu`

那么它们大致会变成：

```text
OBJS    = .../am-kernels/tests/cpu-tests/build/riscv32-nemu/tests/string.o
LINKAGE = string.o + am-riscv32-nemu.a + klib-riscv32-nemu.a + ...
IMAGE   = .../am-kernels/tests/cpu-tests/build/string-riscv32-nemu
```

于是最终的 ELF 目标就是：

```text
$(IMAGE).elf
= .../am-kernels/tests/cpu-tests/build/string-riscv32-nemu.elf
```

下面按执行顺序拆成两步看会最清楚。

###### 第一步：先把源文件编译成对象文件 `.o`

规则是：

```makefile
$(DST_DIR)/%.o: %.c
	@$(CC) -std=gnu11 $(CFLAGS) -c -o $@ $(realpath $<)
```

对 `tests/string.c` 而言，它的含义就是：

```text
把 string.c 用交叉编译器编译成 string.o
```

这一步生成的是对象文件 `.o`。你可以把它理解成“已经被编译好的零件”：

* 里面已经是 RISC-V 目标代码；
* 但它还不是完整程序；
* 它还没有把运行时库、链接地址、入口符号组织成最终镜像。

所以 `.o` 的本质是：

> **单个源文件对应的编译结果，但还不是完整可运行镜像。**

###### 第二步：再把 `.o` 和库链接成 `.elf`

真正生成 `.elf` 的规则是：

```makefile
$(IMAGE).elf: $(LINKAGE) $(LDSCRIPTS)
	@$(LD) $(LDFLAGS) -o $@ --start-group $(LINKAGE) --end-group
```

这条规则要表达的意思是：

* 目标：生成 `$(IMAGE).elf`
* 输入：
  * `$(LINKAGE)`：应用自己的 `.o`，再加上递归生成的 `am` / `klib` 等静态库
  * `$(LDSCRIPTS)`：平台脚本提供的链接脚本，比如 `linker.ld`
* 动作：用 `ld` 把这些输入正式链接成一个完整 ELF 文件

这一刻做的事情，本质上就是：

1. 把应用自己的对象文件放进来；
2. 把 AM 运行时和 `klib` 这些库也放进来；
3. 按 `linker.ld` 规定的地址布局组织 `.text/.data/.bss`；
4. 确定程序入口，例如 `_start`；
5. 最终输出一个带完整地址语义的 `ELF` 镜像。

所以 `.elf` 可以直接理解成：

> **把很多 `.o/.a` 零件，按链接脚本和入口地址组装完成之后得到的“完整客户程序”。**

###### 第三步：为什么后面还要再变成 `.bin`？

因为 `.bin` 不是直接从 `string.c` 编出来的，而是平台脚本后面再从：

```makefile
$(IMAGE).elf
```

里用 `objcopy` 提取出来的裸二进制。

所以顺序一定是：

```text
.c/.S
-> .o
-> .a
-> .elf
-> .bin
```

如果只记一句话，就记这个：

> **`.elf` 是“链接完成后的完整程序镜像”，`.bin` 是“从这个完整镜像里剥出来的纯机器码内容”。**

如果把你当前最常见的调用：

```bash
make -f Makefile.string ARCH=riscv32-nemu run
```

映射到这一层，那么第二层总控的执行链可以压成下面几步：

1. 子 make 解析 `Makefile.string`，拿到 `NAME` 与 `SRCS`；
2. `include $(AM_HOME)/Makefile` 把总控骨架读进来；
3. 总控解析 `ARCH`、`ISA`、`PLATFORM`、`WORK_DIR`、`IMAGE`、`OBJS` 等变量；
4. 总控再通过 `-include $(AM_HOME)/scripts/$(ARCH).mk` 把 `riscv32-nemu` 的 ISA/平台脚本并入；
5. 平台脚本补上 `LDSCRIPTS`、`AM_SRCS`、`image` 后处理规则以及 `run` 目标；
6. 因为当前目标是 `run`，Make 不会停在 `$(IMAGE).elf`，而是继续沿着依赖先做 `insert-arg`、`image`、`image-dep`；
7. `image-dep` 又要求先生成 `$(IMAGE).elf`；
8. `$(IMAGE).elf` 反过来要求先准备 `$(LINKAGE)`，也就是应用对象文件和递归库；
9. 所有依赖满足后，才会链接出 `.elf`，再转 `.bin`，最后把控制权交给第三层 NEMU 运行层。

这里最值得你记住的一点是：

> **第二层总控真正做的不是“直接执行 run”，而是先把 `run` 背后整张依赖图补全，再让 Make 顺着依赖自动推导到最终动作。**

#### 第三层：NEMU 运行层 —— Make 从“构建程序”切换到“驱动模拟器”

当 AM 总控层把 `.elf` 做出来之后，控制权继续往下流到：

* `abstract-machine/scripts/platform/nemu.mk`
* `nemu/Makefile`
* `nemu/scripts/native.mk`

这时 Make 的角色发生了一次重要切换：

> **从“客户程序构建系统”切换成“模拟器驱动系统”。**

##### 1）平台脚本先把 `.elf` 变成 `.bin`

例如：

```makefile
image: image-dep
  @$(OBJDUMP) -d $(IMAGE).elf > $(IMAGE).txt
  @$(OBJCOPY) -S --set-section-flags .bss=alloc,contents -O binary $(IMAGE).elf $(IMAGE).bin
```

此时 Make 已经不再只是“编译源码”，而是在做镜像后处理。

如果你把 `abstract-machine/scripts/platform/nemu.mk` 单独拎出来看，它在这一层其实承担的是一个非常明确的桥接职责：

> **接住第二层已经生成好的 `$(IMAGE).elf`，把它加工成 NEMU 真正要吃的 `$(IMAGE).bin`，再把运行参数和镜像路径一起打包下发给 NEMU。**

这个桥接过程可以拆成四步。

###### 第一步：先补齐 NEMU 平台自己的运行时与链接条件

文件开头先做的不是运行，而是补平台语义：

```makefile
AM_SRCS := platform/nemu/trm.c \
           platform/nemu/ioe/ioe.c \
           ...
LDSCRIPTS += $(AM_HOME)/scripts/linker.ld
LDFLAGS   += --defsym=_pmem_start=0x80000000 --defsym=_entry_offset=0x0
LDFLAGS   += --gc-sections -e _start
```

这一步的含义是：

* 让 AM 知道当前平台是 `nemu`，所以应该编入 `trm/ioe/timer/gpu/...` 这些平台实现；
* 让链接器知道当前程序要按 `linker.ld` 的规则布局；
* 让程序的装载/入口地址锚定到 `0x80000000`；
* 让 `_start` 成为程序入口。

也就是说，`platform/nemu.mk` 并不是等 `.elf` 出来后才起作用，而是在 `.elf` 生成之前就已经参与定义：

> **这个 ELF 应该按什么平台规则被链接出来。**

###### 第二步：`image` 目标把完整 ELF 提炼成可运行的裸镜像

规则是：

```makefile
image: image-dep
  @$(OBJDUMP) -d $(IMAGE).elf > $(IMAGE).txt
  @$(OBJCOPY) -S --set-section-flags .bss=alloc,contents -O binary $(IMAGE).elf $(IMAGE).bin
```

这里要看清两个输出：

* `$(IMAGE).txt`：给人看的反汇编文本；
* `$(IMAGE).bin`：给 NEMU 吃的裸二进制镜像。

这一步本质上不是“再次编译”，而是：

* 用 `objdump` 把 ELF 反汇编出来，方便阅读；
* 用 `objcopy` 把 ELF 里的真正机器码内容抽出来，生成 `.bin`。

所以这一层里：

* `.elf` 仍然是“完整程序镜像”；
* `.bin` 是“去掉 ELF 封装后留下的纯机器码内容”。

###### 第三步：`insert-arg` 再对 `.bin` 做一次运行前补丁

在 `platform/nemu.mk` 中还有：

```makefile
insert-arg: image
  @python $(AM_HOME)/tools/insert-arg.py $(IMAGE).bin $(MAINARGS_MAX_LEN) $(MAINARGS_PLACEHOLDER) "$(mainargs)"
```

这说明 `run` 之前，Make 不会直接拿刚生成的 `.bin` 去跑，而是还会先经过一次：

```text
image
-> insert-arg
```

这一步的作用是：

* 在镜像里找到预留的参数占位区；
* 把 `mainargs` 写进去；
* 让客户程序启动后能读到它的参数。

所以更精确地说，第三层拿到手的并不是“刚从 ELF 转出来的原始 `.bin`”，而是：

> **已经经过参数注入处理的最终可运行 `.bin`。**

###### 第四步：`run` 目标把镜像和参数一起下发给 NEMU

真正跨到 NEMU 世界的规则是：

```makefile
run: insert-arg
  $(MAKE) -C $(NEMU_HOME) ISA=$(ISA) run ARGS="$(NEMUFLAGS)" IMG=$(IMAGE).bin
```

它的真实意义是：

* 先保证 `insert-arg` 已完成；
* 然后启动一个新的 make 进程进入 `$(NEMU_HOME)`；
* 把 `ISA`、`ARGS`、`IMG` 三类关键信息一起透传下去。

这里最关键的是后两项：

* `ARGS="$(NEMUFLAGS)"`：把 `-b -l .../nemu-log.txt` 这类运行参数传给 NEMU；
* `IMG=$(IMAGE).bin`：把刚才生成的裸镜像路径传给 NEMU。

也就是说，这一层真正做的是：

> **把“客户程序镜像”和“模拟器启动参数”一起打包，递交给下一层 NEMU 构建/运行系统。**

如果把 `platform/nemu.mk` 的执行顺序压成一条最短链，就是：

```text
第二层链接得到 $(IMAGE).elf
-> image 生成 $(IMAGE).txt 和 $(IMAGE).bin
-> insert-arg 给 $(IMAGE).bin 注入 mainargs
-> run: make -C $(NEMU_HOME) ... IMG=$(IMAGE).bin
```

所以第三层开头最应该记住的一句话是：

> **`platform/nemu.mk` 不是在“重新编程序”，而是在“把已经链接好的程序镜像加工成 NEMU 可运行格式，并正式移交给 NEMU”。**

##### 2）平台脚本把变量封装后递归交给 NEMU

例如：

```makefile
$(MAKE) -C $(NEMU_HOME) ISA=$(ISA) run ARGS="$(NEMUFLAGS)" IMG=$(IMAGE).bin
```

这一步有三个必须看清的动作：

* `-C $(NEMU_HOME)`：切换工作目录，启动一个新的 Make 进程
* `ISA=$(ISA)`：把 AM 侧拆出的 ISA 继续往下透传
* `IMG=$(IMAGE).bin`：把刚生成的客户程序镜像交给 NEMU

所以这一层已经不是简单 `include`，而是：

> **递归 make + 参数透传 + 新工作目录上下文**

##### 3）NEMU 侧再次建立自己的变量表和规则图

进入 `nemu/Makefile` 后，会重新开始一轮 Make 的标准流程：

* 读取 `auto.conf`
* 解析 `CONFIG_ISA`
* 生成 `GUEST_ISA`
* 收集 `src/**/filelist.mk`
* 拼接 `CFLAGS`
* include `scripts/native.mk`

这说明：

> **递归 make 不是“接着上一次状态往下跑”，而是“在新目录里重新开启一轮 Make 生命周期”。**

如果把这一轮新的生命周期继续往下拆，`nemu/Makefile` 实际上又做了四件事。

###### 第一步：先确认“这里是不是一个合法的 NEMU 工程”

文件最开始就有：

```makefile
ifeq ($(wildcard $(NEMU_HOME)/src/nemu-main.c),)
  $(error NEMU_HOME=$(NEMU_HOME) is not a NEMU repo)
endif
```

这说明 NEMU 顶层 Makefile 的第一件事也不是“直接编译”，而是先检查：

* `NEMU_HOME` 指向的目录对不对；
* 当前工作目录下是不是一份真正的 NEMU 源码树。

所以第三层在进入 NEMU 世界之后，仍然先做一轮自己的环境校验。

###### 第二步：把 `menuconfig` 结果翻译成 NEMU 自己的编译语义

最关键的几行是：

```makefile
-include $(NEMU_HOME)/include/config/auto.conf
GUEST_ISA ?= $(call remove_quote,$(CONFIG_ISA))
ENGINE ?= $(call remove_quote,$(CONFIG_ENGINE))
NAME    = $(GUEST_ISA)-nemu-$(ENGINE)
```

这一步的作用是：

* 从 `auto.conf` 里读取当前配置；
* 提取出 `CONFIG_ISA`、`CONFIG_ENGINE`；
* 再把它们折叠成 NEMU 自己的程序名。

例如在你当前配置下，它大致会得到：

```text
GUEST_ISA = riscv32
ENGINE    = interpreter
NAME      = riscv32-nemu-interpreter
```

这一刻就已经确定了：

> **第三层里真正要被构建并运行的宿主机程序，是 `build/riscv32-nemu-interpreter`。**

###### 第三步：收集 NEMU 自己的源码和编译参数

接下来是：

```makefile
FILELIST_MK = $(shell find -L ./src -name "filelist.mk")
include $(FILELIST_MK)
SRCS-y += $(shell find -L $(DIRS-y) -name "*.c")
SRCS = $(filter-out $(SRCS-BLACKLIST-y),$(SRCS-y))
```

以及：

```makefile
CFLAGS_BUILD += $(call remove_quote,$(CONFIG_CC_OPT))
CFLAGS_TRACE += -DITRACE_COND=...
CFLAGS_TRACE += -DMTRACE_COND=...
CFLAGS  += $(CFLAGS_BUILD) $(CFLAGS_TRACE) -D__GUEST_ISA__=$(GUEST_ISA)
```

这部分的意义是：

* 收集 NEMU 自己要编译的全部源码；
* 结合配置系统生成最终的 `CFLAGS/LDFLAGS`；
* 把 `__GUEST_ISA__=riscv32`、`ITRACE_COND`、`MTRACE_COND` 这类宏真正传给编译器。

也就是说，`platform/nemu.mk` 传进来的：

* `ISA=$(ISA)`
* `ARGS=...`
* `IMG=...`

只是告诉 NEMU “这次怎么跑”，而 `nemu/Makefile` 自己还必须决定：

> **“NEMU 本体自己该按什么源码集合和配置参数被编出来。”**

###### 第四步：把真正的编译/运行规则继续下发给 `native.mk`

在当前常见路径下，会走到：

```makefile
include $(NEMU_HOME)/scripts/native.mk
```

这一句的语义和前面 AM 那边一样，也不是“运行时跳转”，而是：

> **把 NEMU 侧真正负责 build/run/gdb 的规则继续并入当前这一轮 make 解析过程。**

##### 4）最后由 `native.mk` 折叠成真实运行命令

例如：

```makefile
NEMU_EXEC := $(BINARY) $(ARGS) $(IMG)
```

最终真正落到 shell 的形态是：

```text
build/riscv32-nemu-interpreter -b -l ... /path/to/string-riscv32-nemu.bin
```

所以 NEMU 运行层的最终职责非常清楚：

> **把前面所有构建结果和运行参数收拢成一条可执行命令。**

但如果再往下读一层，你会发现 `native.mk` 做的其实不是一句简单赋值，而是“三步收口”。

###### 第一步：先把 NEMU 自己编出来

`native.mk` 开头先 include：

```makefile
include $(NEMU_HOME)/scripts/build.mk
include $(NEMU_HOME)/tools/difftest.mk
```

这表示：

* `build.mk` 负责把 NEMU 源码真正编译成 `$(BINARY)`；
* `difftest.mk` 负责差分测试相关的动态库与参数。

后面还有：

```makefile
run-env: $(BINARY) $(DIFF_REF_SO)
```

这句话非常关键，它说明 `run` 之前不会直接启动，而是必须先保证：

* `$(BINARY)`：NEMU 本体已经编好；
* `$(DIFF_REF_SO)`：若开启 difftest，参考模型也已经准备好。

所以在第三层最后半段，Make 仍然在遵守老规则：

> **先满足依赖，再执行最终命令。**

###### 第二步：再把外层透传参数和 NEMU 自己的默认参数折叠起来

`native.mk` 中最关键的变量是：

```makefile
override ARGS ?= --log=$(BUILD_DIR)/nemu-log.txt
override ARGS += $(ARGS_DIFF)
IMG ?=
NEMU_EXEC := $(BINARY) $(ARGS) $(IMG)
```

这里要分开看：

* `ARGS ?=`：如果外层没传，就用 NEMU 默认日志路径；
* `override ARGS += $(ARGS_DIFF)`：再把 difftest 相关参数拼上去；
* `IMG ?=`：镜像路径默认可以为空；
* `NEMU_EXEC := $(BINARY) $(ARGS) $(IMG)`：最终折叠成一条 shell 命令。

而你当前这条链里，由于 `platform/nemu.mk` 已经显式传入：

```text
ARGS="-b -l .../nemu-log.txt"
IMG=.../string-riscv32-nemu.bin
```

所以这时 `NEMU_EXEC` 的真实含义就是：

```text
build/riscv32-nemu-interpreter + 运行参数 + 客户镜像路径
```

###### 第三步：`run` 目标最后才真正把 NEMU 作为宿主机程序启动

规则是：

```makefile
run: run-env
	$(NEMU_EXEC)
```

也就是说，前面三层所有 Makefile 兜了这么大一圈，最后真正落到 shell 的动作，其实就是：

```text
执行一条宿主机命令，把 NEMU 程序本体启动起来，并把 `.bin` 镜像作为参数交给它
```

如果把当前最常见场景代进去，这条命令就近似于：

```bash
/home/l/ysyx/ysyx-workbench/nemu/build/riscv32-nemu-interpreter \
  -b \
  -l /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/nemu-log.txt \
  /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/string-riscv32-nemu.bin
```

到这一步，Make 的职责就结束了，后面才正式切换到：

* Linux 启动 NEMU 进程；
* `nemu-main.c` 进入 `main(argc, argv)`；
* `monitor.c` 解析 `-l` 与 `IMG`；
* NEMU 装载 `.bin` 并开始执行客户程序。

所以最后这一层最值得记住的一句话是：

> **`nemu/Makefile` 负责“把 NEMU 本体编好并准备运行语义”，`native.mk` 负责“把这一切收拢成真正启动 NEMU 的那条命令”。**

#### 三层执行模型的总收束

现在再回头看 Makefile 的执行规则，就会清楚很多。

在 `ysyx-workbench` 里，Makefile 的执行逻辑不是“一份文件从上往下跑完”，而是三层协同：

##### 第一层：入口调度层

负责回答：

* 这次跑哪个测试？
* 当前目标是什么？
* 命令行变量是什么？

典型文件：

* `am-kernels/tests/cpu-tests/Makefile`

##### 第二层：AM 总控层

负责回答：

* 当前 `ARCH` 该怎么拆？
* 当前应用该怎么编成 `.elf`？
* 应该递归编哪些库？
* 目标文件、镜像文件放在哪？

典型文件：

* `abstract-machine/Makefile`

##### 第三层：NEMU 运行层

负责回答：

* 当前程序如何先链接成 `.elf`，再提取成 `.bin`？
* `.elf` 怎么变成 `.bin`？
* 参数如何传给 NEMU？
* NEMU 自己怎么编？
* 最后如何启动模拟器？

典型文件：

* `abstract-machine/scripts/platform/nemu.mk`
* `nemu/Makefile`
* `nemu/scripts/native.mk`

所以最终可以把 `ysyx` 这套 Make 执行模型压缩成一句话：

> **第一层决定“本轮任务是什么”，第二层决定“这个任务怎样被构建成镜像”，第三层决定“镜像怎样被交给模拟器运行”；Make 的执行顺序由依赖图决定，而变量和规则则在每一层被重新组织和下发。**

#### 阅读 Makefile 时的四个检查问题

以后你每次读当前工程里的 Makefile，都建议强制问自己四个问题：

##### 1）当前文件属于哪一层？

* 入口调度层？
* AM 总控层？
* NEMU 运行层？

##### 2）当前文件是在“声明规则”，还是在“递归下发任务”？

例如：

* `include ...` 更偏向规则拼接
* `$(MAKE) -C ...` 更偏向递归下发

##### 3）当前变量是从哪来的？

可能来源于：

* 命令行
* 当前 Makefile
* include 文件
* 配置文件
* 上一层递归透传

##### 4）当前命令为什么会在这里执行？

是因为：

* 当前目标被用户点名要求构建
* 或者它是最终目标的依赖
* 或者它在变量展开 / `$(shell ...)` 里就提前执行了

只要沿着这四个问题去读，Makefile 就不再像“黑魔法”，而会变成结构非常清晰的构建系统描述语言。

---

<a id="section-hierarchy"></a>

## 三、全局脚本层级总图

### 1. 构建与运行总层级图

```text
┌──────────────────────────────────────────────┐
│ am-kernels/tests/cpu-tests/Makefile         │
│ 作用：挑选测试、为每个测试动态生成 Makefile  │
└──────────────────────────────────────────────┘
                      │
                      ▼
┌──────────────────────────────────────────────┐
│ 临时 Makefile.<test>                        │
│ 内容：NAME=... SRCS=... include AM/Makefile │
└──────────────────────────────────────────────┘
                      │
                      ▼
┌──────────────────────────────────────────────┐
│ abstract-machine/Makefile                   │
│ 作用：通用交叉编译骨架                       │
└──────────────────────────────────────────────┘
                      │
                      ▼
┌──────────────────────────────────────────────┐
│ abstract-machine/scripts/riscv32-nemu.mk    │
│ 作用：把 ISA=riscv32 与 PLATFORM=nemu 绑定  │
└──────────────────────────────────────────────┘
               │                       │
               ▼                       ▼
┌───────────────────────┐   ┌─────────────────────────┐
│ isa/riscv.mk          │   │ platform/nemu.mk        │
│ 作用：RISC-V 工具链    │   │ 作用：NEMU 平台运行规则 │
└───────────────────────┘   └─────────────────────────┘
                                       │
                                       ▼
                          ┌─────────────────────────┐
                          │ 生成 .elf / .bin        │
                          │ run: make -C nemu ...   │
                          └─────────────────────────┘
                                       │
                                       ▼
                          ┌─────────────────────────┐
                          │ nemu/Makefile           │
                          │ 作用：构建 NEMU 本体     │
                          └─────────────────────────┘
                                       │
                         ┌─────────────┴─────────────┐
                         ▼                           ▼
             ┌───────────────────────┐   ┌───────────────────────┐
             │ nemu/scripts/config.mk │   │ nemu/scripts/native.mk│
             │ 作用：配置系统         │   │ 作用：编译/运行 NEMU  │
             └───────────────────────┘   └───────────────────────┘
```

---

<a id="section-script-breakdown"></a>

## 四、逐个脚本拆解：核心功能 + 核心变量 + 来源 + 作用

下面按“从外到内”的顺序整理。

---

<a id="part-workspace-app"></a>

# 第一部分：工作区 / 应用选择层

## 1. `/home/l/ysyx/ysyx-workbench/Makefile`

### 核心功能
这个根 `Makefile` **不是整个工作区统一构建入口**，它主要承担两类辅助职能：

1. 记录学生信息与 tracer 提交信息；
2. 提供 Git 辅助宏给子项目复用。

默认目标 `_default` 只会提示：

```text
Please run 'make' under subprojects.
```

也就是说：

> **真正的构建入口不在根目录，而是在各子项目目录中。**

### 核心变量与来源

| 变量 | 来源 | 作用 |
| :--- | :--- | :--- |
| `STUID` | 直接在文件中定义 | 学号信息，用于 tracer commit |
| `STUNAME` | 直接在文件中定义 | 姓名信息，用于 tracer commit |
| `TRACER` | 文件内定义 | tracer 分支名，默认 `tracer-ysyx` |
| `GITFLAGS` | 由 `TRACER` 拼出 | 控制自动 git commit 参数 |
| `YSYX_HOME` | `$(NEMU_HOME)/..` | 指向工作区根路径 |
| `WORK_BRANCH` | `git rev-parse` 命令结果 | 当前工作分支 |
| `WORK_INDEX` | 派生变量 | 备份 index 的路径 |
| `LOCK_DIR` | 派生变量 | Git 锁目录 |

### 核心宏与作用

#### `git_soft_checkout`
```makefile
define git_soft_checkout
	git checkout --detach -q && git reset --soft $(1) -q -- && git checkout $(1) -q --
endef
```
作用：在 tracer 分支与工作分支间“软切换”。

#### `git_commit`
```makefile
define git_commit
	-@flock $(LOCK_DIR) $(MAKE) -C $(YSYX_HOME) .git_commit MSG='$(1)'
	-@sync $(LOCK_DIR)
endef
```
作用：提供给子项目调用的统一提交入口。

### 与其他脚本的逻辑关系

这个文件本身不会主动驱动 `NEMU` 或 `AM` 编译；但它会被：

* `nemu/scripts/native.mk`

通过：

```makefile
-include $(NEMU_HOME)/../Makefile
```

静默包含进来，以便复用 `git_commit` 宏。

### 一句话总结

> **根 `Makefile` 不是构建骨架，而是 repo 级 Git / tracer 辅助脚本。**

---

## 2. `am-kernels/tests/cpu-tests/Makefile`

### 核心功能
这个脚本是 **CPU 测试入口层 / 调度层**，它解决的不是“编译器怎么把 `.c` 变成 `.o/.elf/.bin`”，而是更靠上的一个问题：

> **当前到底要跑哪些测试、每个测试如何被包装成一个统一入口、以及这些测试的成功/失败如何被集中汇总。**

它的设计思路非常典型：

1. 先自动扫描 `tests/` 目录，得到所有测试名；
2. 再为每个测试动态生成一个极小的 `Makefile.<test>`；
3. 这个临时 `Makefile` 只声明两件事：`NAME` 和 `SRCS`；
4. 真正复杂的交叉编译、链接、平台适配、镜像生成、NEMU 运行，全部继续委托给 `abstract-machine/Makefile`；
5. 当前脚本只负责做“批量调度 + 结果汇总 + 临时文件清理”。

换句话说：

> **它是“测试调度器”，不是“底层构建器”。**

### 核心变量与来源

| 变量 | 来源 | 作用 |
| :--- | :--- | :--- |
| `ALL` | `find tests/. -name "*.c"` 再取 basename | 测试名集合 |
| `RESULT` | 文件内定义 `.result` | 记录测试通过/失败 |
| `COLOR_RED/GREEN/NONE` | 文件内定义 | 彩色输出 |
| `ARCH` | 命令行传入，如 `ARCH=riscv32-nemu` | 指定架构与平台 |
| `MAKECMDGOALS` | make 自带 | 确定 `run` / `gdb` / `clean` |

### 逐组设计逻辑分析

#### 1）`.PHONY`：把所有测试目标都当作“动作”而不是“文件”

```makefile
.PHONY: all run gdb clean latest $(ALL)
```

这一行的设计意图有两层：

* `all`、`run`、`gdb`、`clean`、`latest` 本来就是动作型目标，理应声明为伪目标；
* `$(ALL)` 里的每个测试名（如 `string`、`add`、`dummy`）也被声明成伪目标，避免目录里如果恰好出现一个同名文件时，`make` 误以为该目标“已经是最新，不需要执行”。

因此这一步本质上是在告诉 `make`：

> **这里的测试名不是要生成的文件，而是可执行的调度入口。**

#### 2）`RESULT` 与 `$(shell > $(RESULT))`：在解析阶段就先清空结果文件

```makefile
RESULT = .result
$(shell > $(RESULT))
```

这里要特别注意时机。

* `RESULT = .result`：定义结果文件名；
* `$(shell > $(RESULT))`：不是在 `run` 目标执行时才运行，而是在 **Makefile 被解析时就执行**。

也就是说，只要当前这个 Makefile 被 `make` 读入，`.result` 就会先被清空一次。

设计意义是：

> **保证本轮测试结果不会和上一轮残留内容混在一起。**

#### 3）颜色变量：让汇总输出更易读

```makefile
COLOR_RED   = \033[1;31m
COLOR_GREEN = \033[1;32m
COLOR_NONE  = \033[0m
```

这三项不参与构建流程本身，只用于最后 `printf` 结果时着色：

* PASS 用绿色；
* FAIL 用红色；
* 结束后恢复终端默认颜色。

它们体现的不是构建逻辑，而是“测试调度器的用户交互友好性”。

#### 4）`ALL`：把 `tests/*.c` 自动提升成“测试名集合”

```makefile
ALL = $(basename $(notdir $(shell find tests/. -name "*.c")))
```

这个表达式要从里往外看：

1. `$(shell find tests/. -name "*.c")`
   * 找出 `tests/` 目录下的全部 C 测试文件；
2. `$(notdir ...)`
   * 去掉目录，只保留文件名，例如 `tests/string.c -> string.c`；
3. `$(basename ...)`
   * 再去掉扩展名，例如 `string.c -> string`。

所以最后 `ALL` 不是路径列表，而是一个纯测试名列表。它后续会被用于：

* 构造 `Makefile.string`、`Makefile.add` 这样的临时目标；
* 构造 `string`、`add` 这样的伪目标入口；
* 输出最终测试列表；
* 统计测试个数。

这一层设计说明：

> **当前脚本不是手写维护测试清单，而是通过目录扫描自动发现测试。**

### 最关键的规则

#### 规则 1：`all` 目标不是直接编译，而是先触发每个 `Makefile.<test>`

```makefile
all: $(addprefix Makefile., $(ALL))
  @echo "test list [$(words $(ALL)) item(s)]:" $(ALL)
```

这里的依赖项：

```makefile
$(addprefix Makefile., $(ALL))
```

会把测试名列表变成：

```text
Makefile.string Makefile.add Makefile.mul ...
```

所以 `all` 的真实含义并不是“直接把所有测试源码编掉”，而是：

> **依次触发“每个测试对应的临时 Makefile 生成 + 执行”流程。**

第二行：

```makefile
@echo "test list [$(words $(ALL)) item(s)]:" $(ALL)
```

只负责在全部测试完成后，打印测试列表和数量，属于汇总展示逻辑。

#### 规则 2：测试名目标只是一个别名跳板

```makefile
$(ALL): %: Makefile.%
```

这是一条静态模式规则。以 `string` 为例，它等价于：

```makefile
string: Makefile.string
```

所以如果用户执行：

```bash
make string ARCH=riscv32-nemu
```

`make` 不会去找一个叫 `string` 的真实文件，而是会转而执行 `Makefile.string` 这条规则。

这意味着：

> **测试名本身只是入口别名，真正的工作都挂在 `Makefile.%` 这条模式规则上。**

#### 规则 3：`Makefile.%` 是这个文件的核心枢纽

```makefile
Makefile.%: tests/%.c latest
	@/bin/echo -e "NAME = $*\nSRCS = $<\ninclude $${AM_HOME}/Makefile" > $@
```

这条规则才是真正把“测试调度”和“AM 构建体系”接起来的关键点。

它的依赖是：

* `tests/%.c`：当前测试的源文件；
* `latest`：一个当前为空的钩子目标，用于未来扩展。

这一行命令里几个自动变量必须记住：

* `$@`：当前目标名，例如 `Makefile.string`
* `$<`：第一个依赖，例如 `tests/string.c`
* `$*`：模式 `%` 匹配到的 stem，例如 `string`

命令会生成类似这样的临时文件：

```makefile
NAME = string
SRCS = tests/string.c
include $(AM_HOME)/Makefile
```

这三行的设计非常精炼：

* `NAME`：定义当前应用名；
* `SRCS`：定义当前应用源文件；
* `include $(AM_HOME)/Makefile`：把后续所有真正的构建逻辑交给 AM 总控。

也就是说，`cpu-tests/Makefile` 自己并不掌握“如何交叉编译、如何链接、如何转 `.bin`、如何调用 NEMU”的细节，它只会生成一张“当前测试的身份卡”，然后把这张卡递交给 `abstract-machine/Makefile`。

#### 规则 4：递归调用当前生成的临时 Makefile

```makefile
@if make -s -f $@ ARCH=$(ARCH) $(MAKECMDGOALS); then ...
```

这一步是“委托发生”的真正时刻。

它会执行：

* `-f $@`：使用刚刚生成的临时 `Makefile.string`
* `ARCH=$(ARCH)`：把外层架构继续透传下去
* `$(MAKECMDGOALS)`：把外层目标继续透传下去，比如 `run`、`gdb`

这意味着：

* 如果外层执行的是 `make ARCH=riscv32-nemu run`
* 那么内层就会执行类似：

```bash
make -s -f Makefile.string ARCH=riscv32-nemu run
```

因此当前脚本起到的是“包装并转发”的作用，而不是“亲自完成底层构建”的作用。

#### 规则 5：在当前层汇总 PASS/FAIL

递归调用成功时：

```makefile
printf "[%14s] $(COLOR_GREEN)PASS$(COLOR_NONE)\n" $* >> $(RESULT)
```

失败时：

```makefile
printf "[%14s] $(COLOR_RED)***FAIL***$(COLOR_NONE)\n" $* >> $(RESULT)
```

这说明 PASS/FAIL 统计是当前脚本的职责，而不是 AM 或 NEMU 的职责。

它像一个测试调度器那样，只关心：

* 子任务有没有成功；
* 成功了怎么记；
* 失败了怎么记。

#### 规则 6：清理临时 `Makefile.<test>`

```makefile
-@rm -f Makefile.$*
```

这一步表示：

* 每个测试执行完后，就把刚才生成的临时构建入口删掉；
* `-` 表示即使删除失败也不让整个流程报错中止；
* `@` 表示不把命令本身打印出来。

因此当前目录不会长期堆积很多 `Makefile.string`、`Makefile.add` 一类中间文件。

#### 规则 7：`run` 是结果汇总出口

```makefile
run: all
  @cat $(RESULT)
  @rm $(RESULT)
```

其设计含义是：

1. 先执行完整测试集；
2. 再统一打印 `.result`；
3. 打印后立即删除结果文件。

所以 `.result` 是一个典型的“中间汇总文件”，不是长期保存的正式产物。

#### 规则 8：`gdb` 与 `clean` 的定位

```makefile
gdb: all
```

表示 `gdb` 目标本身没有额外逻辑，只负责沿用同一套调度机制，把 `gdb` 这个目标透传给下层临时 Makefile。

```makefile
clean:
  rm -rf Makefile.* build/
```

表示清理当前目录下的：

* 动态生成的 `Makefile.*`
* 当前测试目录本地产生的 `build/`

它清的不是整个工作区，只是当前测试层的中间产物。

#### 规则 9：`latest` 是一个预留钩子

```makefile
latest:
```

当前它没有命令，也没有依赖，因此什么都不做。但它被写进：

```makefile
Makefile.%: tests/%.c latest
```

这说明它是一个占位式设计点。后续如果要在“每个单测正式执行前”加入某个统一动作，比如同步文件、更新时间戳、做额外预处理，就可以直接挂到 `latest` 上，而不需要改动主规则结构。

### 执行链闭环总结

如果执行：

```bash
make ARCH=riscv32-nemu ALL=string run
```

那么当前 `Makefile` 这一层的执行闭环可以还原成：

1. 解析文件时先清空 `.result`；
2. 计算 `ALL`，得到测试名集合；
3. 进入 `run: all`；
4. `all` 触发 `Makefile.string`；
5. `Makefile.string` 依赖 `tests/string.c`；
6. 动态生成一个最小临时入口文件；
7. 递归执行 `make -f Makefile.string ARCH=riscv32-nemu run`；
8. 临时 Makefile 再继续 `include $(AM_HOME)/Makefile`，把工作下发到 AM；
9. 成功/失败被写入 `.result`；
10. 删除临时 `Makefile.string`；
11. `run` 最后打印 `.result` 并删除它。

因此当前脚本最核心的设计关联逻辑可以压成一句话：

> **它先把“测试源码”抽象成“测试名 + 最小入口 Makefile”，再通过递归 `make` 把控制权转交给 AM，总结结果后在当前层统一收口。**

### 与其他脚本的逻辑关系

它在整套系统里位于最上层的“测试选择层”，向下把控制权交给：

* `abstract-machine/Makefile`

更准确地说，它和下层之间的关系不是“直接 include 后全盘接管”，而是：

1. 先动态生成一个临时 `Makefile.<test>`；
2. 再由这个临时文件去 `include $(AM_HOME)/Makefile`；
3. 所以它本身是“调度器”，临时 `Makefile.<test>` 是“桥接器”，`abstract-machine/Makefile` 才是“真正构建器”。

这个三段式关系非常值得记住。

### 一句话总结

> **`cpu-tests/Makefile` 的本质不是编译规则文件，而是测试调度器：它负责发现测试、为每个测试临时生成最小入口 Makefile、递归转交给 `abstract-machine/Makefile`、并在当前层统一汇总测试结果。**

---

## 3. `am-kernels/kernels/hello/Makefile`

### 核心功能
这是最简单的单程序入口示例。

内容非常短：

```makefile
NAME = hello
SRCS = hello.c
include $(AM_HOME)/Makefile
```

### 它的价值
这个文件非常适合理解整套系统的最小模型，因为它告诉你：

> **对于 AM 应用来说，应用目录下的 `Makefile` 往往只需要声明“我是谁”和“我的源码是什么”，真正的复杂逻辑全部由 `abstract-machine/Makefile` 统一接管。**

### 一句话总结

> **应用层 `Makefile` 只声明应用元信息，不负责底层交叉编译细节。**

---

<a id="part-am-core"></a>

# 第二部分：AbstractMachine 总控层

## 4. `abstract-machine/Makefile`

### 核心功能
这是 AM 系统的 **通用构建总控骨架**。

它承担的事情很多，但本质可以分成六块：

1. 检查构建环境；
2. 解析 `ARCH`；
3. 建立输出目录与目标文件路径；
4. 设定通用交叉编译工具链变量；
5. include 对应架构脚本；
6. 定义编译、归档、链接、清理规则。

它是 AM 这边最核心的一层。

---

### 核心变量与来源总表

| 变量 | 来源 | 作用 |
| :--- | :--- | :--- |
| `MAKECMDGOALS` | make 自带，如为空则默认设为 `image` | 控制默认构建目标 |
| `AM_HOME` | 环境变量 | 指向 `abstract-machine` 根目录 |
| `ARCH` | 命令行传入，如 `riscv32-nemu` | 决定采用哪份脚本 |
| `ARCH_SPLIT` | `$(subst -, ,$(ARCH))` | 把 `riscv32-nemu` 切成两个词 |
| `ISA` | `$(word 1,$(ARCH_SPLIT))` | 取出 `riscv32` |
| `PLATFORM` | `$(word 2,$(ARCH_SPLIT))` | 取出 `nemu` |
| `WORK_DIR` | `$(shell pwd)` | 当前应用目录 |
| `DST_DIR` | 派生变量 | 对象文件输出目录 |
| `IMAGE_REL` / `IMAGE` | 派生变量 | 目标镜像前缀 |
| `ARCHIVE` | 派生变量 | 静态库产物路径 |
| `OBJS` | 由 `SRCS` 派生 | 应用源码编译出的 `.o` 列表 |
| `LIBS` | 默认加上 `am klib` | 要递归构建的库 |
| `LINKAGE` | 初始为 `$(OBJS)` | 最终参与链接的对象和库 |
| `CROSS_COMPILE` | 默认空，后续由架构脚本补充 | 工具链前缀 |
| `CC/AS/LD/...` | 由 `CROSS_COMPILE` 派生 | 编译器、汇编器、链接器 |
| `INC_PATH` | 当前工程和库头路径 | 形成 `-I` 搜索路径 |
| `CFLAGS/CXXFLAGS/ASFLAGS/LDFLAGS` | 总控中先给通用部分，再由架构脚本补充 | 编译/链接参数 |
| `LDSCRIPTS` | 由平台脚本补充 | 链接脚本 |
| `AM_SRCS` | 由架构/平台脚本补充 | 运行时源码 |

---

### 它如何解析 `ARCH`

最关键的一段：

```makefile
ARCH_SPLIT = $(subst -, ,$(ARCH))
ISA        = $(word 1,$(ARCH_SPLIT))
PLATFORM   = $(word 2,$(ARCH_SPLIT))
```

对：

```makefile
ARCH=riscv32-nemu
```

会得到：

```text
ISA = riscv32
PLATFORM = nemu
```

这一点非常重要，因为它说明：

> **在 AM 体系里，`ARCH` 是最高层入口参数，而 `ISA` / `PLATFORM` 是从 `ARCH` 中切分出来的派生结果。**

---

### 它如何接入具体架构脚本

关键语句：

```makefile
-include $(AM_HOME)/scripts/$(ARCH).mk
```

如果：

```makefile
ARCH=riscv32-nemu
```

那么实际包含的就是：

```text
abstract-machine/scripts/riscv32-nemu.mk
```

这一步是整个 AM 脚本体系的“分发开关”。

---

### 它如何统一设定工具链变量

关键语句：

```makefile
AS        = $(CROSS_COMPILE)gcc
CC        = $(CROSS_COMPILE)gcc
CXX       = $(CROSS_COMPILE)g++
LD        = $(CROSS_COMPILE)ld
AR        = $(CROSS_COMPILE)ar
OBJDUMP   = $(CROSS_COMPILE)objdump
OBJCOPY   = $(CROSS_COMPILE)objcopy
READELF   = $(CROSS_COMPILE)readelf
```

这里的逻辑是：

1. `abstract-machine/Makefile` 先写出工具变量的模板；
2. 真正的前缀 `CROSS_COMPILE` 再由 `isa/riscv.mk` 这类脚本提供。

因此：

> **AM 总控负责“占坑”，ISA 脚本负责“填具体工具链前缀”。**

---

### 它如何递归构建 `am` 和 `klib`

关键宏：

```makefile
define LIB_TEMPLATE =
$$(AM_HOME)/$(1)/build/$(1)-$$(ARCH).a: force
	@$$(MAKE) -s -C $$(AM_HOME)/$(1) archive
LINKAGE += $$(AM_HOME)/$(1)/build/$(1)-$$(ARCH).a
endef
```

配合：

```makefile
$(foreach lib, $(LIBS), $(eval $(call LIB_TEMPLATE,$(lib))))
```

作用是：

* 自动给 `am` / `klib` 生成递归构建规则；
* 递归执行：
  * `make -C $(AM_HOME)/am archive`
  * `make -C $(AM_HOME)/klib archive`
* 再把生成的静态库加入 `LINKAGE`。

因此：

> **`abstract-machine/Makefile` 不只编当前应用，还会递归地把 `am` 和 `klib` 一并编好，再一起链接。**

---

### 它如何生成 `.elf`

关键规则：

```makefile
$(IMAGE).elf: $(LINKAGE) $(LDSCRIPTS)
	@$(LD) $(LDFLAGS) -o $@ --start-group $(LINKAGE) --end-group
```

这一步完成的是：

* 将应用 `.o`
* 加上 `am` / `klib` 静态库
* 再配合平台脚本提供的 `LDSCRIPTS`
* 链接成最终 `ELF`

因此：

> **`.elf` 的生成由 AM 总控负责，但它依赖于 ISA 脚本提供的工具链参数，以及平台脚本提供的链接脚本与平台运行时。**

---

### 一句话总结

> **`abstract-machine/Makefile` 是 AM 世界的总控调度器：它负责解析 `ARCH`、建立工具链框架、汇总应用/库/平台源文件，并把它们最终链接成可运行的客户程序镜像。**

---

<a id="part-am-scripts"></a>

# 第三部分：`abstract-machine/scripts/` 细化层

## 5. `abstract-machine/scripts/riscv32-nemu.mk`

### 核心功能
这个脚本是一个 **组合器 / 胶水脚本**。

它自己不承担全部细节，而是把两份更底层的脚本组合起来：

```makefile
include $(AM_HOME)/scripts/isa/riscv.mk
include $(AM_HOME)/scripts/platform/nemu.mk
```

即：

* `isa/riscv.mk`：解决“RISC-V 应该怎么编”；
* `platform/nemu.mk`：解决“给 NEMU 跑的程序应该怎么链接、怎么转镜像、怎么运行”。

---

### 核心变量与来源

| 变量 | 来源 | 作用 |
| :--- | :--- | :--- |
| `CFLAGS += -DISA_H=...` | 本脚本定义 | 指定 ISA 头实现 |
| `COMMON_CFLAGS += -march=rv32im_zicsr -mabi=ilp32` | 本脚本追加 | 把 RISC-V 收窄为 32 位 ABI |
| `LDFLAGS += -melf32lriscv` | 本脚本追加 | 让链接器按 32 位 RISC-V ELF 处理 |
| `AM_SRCS += ...` | 本脚本追加 | 增加 riscv32 + nemu 平台下的启动与异常处理源码 |

### `AM_SRCS` 的意义

它引入了：

* `riscv/nemu/start.S`
* `riscv/nemu/cte.c`
* `riscv/nemu/trap.S`
* `riscv/nemu/vme.c`

这说明这个脚本不只是“加编译参数”，还进一步声明了：

> **在 `riscv32-nemu` 组合下，AM 运行时必须额外编进哪些架构相关源码。**

---

### 为什么它必须存在

因为单独的：

* `isa/riscv.mk`

只知道“这是 RISC-V”；

单独的：

* `platform/nemu.mk`

只知道“这是 NEMU 平台”；

只有 `riscv32-nemu.mk` 才能把两者准确配对成：

> **“运行在 NEMU 上的 32 位 RISC-V 客户程序”**

---

### 一句话总结

> **`riscv32-nemu.mk` 是把“ISA 规则”和“平台规则”拼装成一个具体目标平台的胶水层。**

---

## 6. `abstract-machine/scripts/isa/riscv.mk`

### 核心功能
这个脚本负责提供 **通用 RISC-V ISA 侧的编译工具链与基础参数**。

---

### 核心变量与来源

| 变量 | 来源 | 作用 |
| :--- | :--- | :--- |
| `CROSS_COMPILE := riscv64-linux-gnu-` | 本脚本定义 | 定义交叉编译工具链前缀 |
| `COMMON_CFLAGS := ...` | 本脚本定义 | RISC-V 通用编译选项 |
| `CFLAGS += $(COMMON_CFLAGS) -static` | 本脚本追加 | 给 C 编译器追加静态链接与 RISC-V 公共参数 |
| `ASFLAGS += $(COMMON_CFLAGS) -O0` | 本脚本追加 | 给汇编阶段追加参数 |
| `LDFLAGS += -melf64lriscv` | 本脚本追加 | 默认用 RISC-V ELF 链接模式 |
| `ARCH_H := arch/riscv.h` | 本脚本覆盖 | 统一 AM 内部的架构头 |

---

### 最关键的点：为什么这里写的是 `riscv64-linux-gnu-`

虽然你的目标是 `riscv32-nemu`，这里仍然使用：

```makefile
CROSS_COMPILE := riscv64-linux-gnu-
```

原因是：

> **很多 RISC-V 工具链二进制名就叫 `riscv64-linux-gnu-gcc`，但它依然可以通过 `-march=rv32... -mabi=ilp32` 编译出 32 位目标代码。**

也就是说：

* 工具链前缀名是 `riscv64-linux-gnu-`
* 目标机器码仍然可以是 `rv32`

这正是为什么 `riscv32-nemu.mk` 要继续补：

* `-march=rv32im_zicsr`
* `-mabi=ilp32`
* `-melf32lriscv`

---

### 它和 `riscv32-nemu.mk` 的关系

可以理解成：

* `isa/riscv.mk` 提供“RISC-V 大类”的默认配置；
* `riscv32-nemu.mk` 再把它进一步压到“32 位 + nemu 平台”的具体落点。

---

### 一句话总结

> **`isa/riscv.mk` 定的是“RISC-V 系列的公共工具链底色”，不是最终的 `rv32-nemu` 完整落地配置。**

---

## 7. `abstract-machine/scripts/platform/nemu.mk`

### 核心功能
这个脚本是 **AM 世界与 NEMU 世界的桥梁脚本**。

它解决的问题是：

1. 给 NEMU 平台补上运行时源码；
2. 指定链接脚本与链接地址；
3. 把 `.elf` 转成 `.bin`；
4. 递归调用 `nemu/Makefile` 去真正运行模拟器。

这个脚本非常关键，因为“程序怎么跑到 NEMU 里”这一跳，就是它负责的。

---

### 核心变量与来源总表

| 变量 | 来源 | 作用 |
| :--- | :--- | :--- |
| `AM_SRCS` | 本脚本定义 | NEMU 平台运行时源码 |
| `CFLAGS += -fdata-sections -ffunction-sections` | 本脚本追加 | 便于链接器裁剪无用段 |
| `CFLAGS += -I$(AM_HOME)/am/src/platform/nemu/include` | 本脚本追加 | NEMU 平台头文件搜索路径 |
| `LDSCRIPTS += $(AM_HOME)/scripts/linker.ld` | 本脚本追加 | 指定链接脚本 |
| `LDFLAGS += --defsym=_pmem_start=0x80000000` | 本脚本追加 | 设定程序逻辑装载起点 |
| `LDFLAGS += --defsym=_entry_offset=0x0` | 本脚本追加 | 设定入口偏移 |
| `LDFLAGS += --gc-sections -e _start` | 本脚本追加 | 删除无用段并指定入口符号 |
| `NEMUFLAGS += -b -l .../nemu-log.txt` | 本脚本追加 | 给 NEMU 运行时传批处理和日志参数 |
| `MAINARGS_MAX_LEN` | 本脚本定义 | mainargs 插入长度上限 |
| `MAINARGS_PLACEHOLDER` | 本脚本定义 | 预留参数占位符 |

---

### 它引入了哪些平台运行时源码

```makefile
AM_SRCS := platform/nemu/trm.c \
           platform/nemu/ioe/ioe.c \
           platform/nemu/ioe/timer.c \
           platform/nemu/ioe/input.c \
           platform/nemu/ioe/gpu.c \
           platform/nemu/ioe/audio.c \
           platform/nemu/ioe/disk.c \
           platform/nemu/mpe.c
```

这部分本质上在说：

> **如果客户程序要跑在 `nemu` 平台上，那么 TRM、IOE、定时器、输入、图形、音频、磁盘、多处理扩展等平台实现都要换成 NEMU 对应版本。**

---

### 它如何定义“程序地址从哪开始”

关键语句：

```makefile
LDSCRIPTS += $(AM_HOME)/scripts/linker.ld
LDFLAGS   += --defsym=_pmem_start=0x80000000 --defsym=_entry_offset=0x0
```

这意味着：

* 链接使用 `linker.ld`
* `linker.ld` 中的：
  * `_pmem_start`
  * `_entry_offset`

会被这里具体赋值

于是程序会被布置到：

```text
0x80000000 + 0x0
```

即：

```text
0x80000000
```

因此：

> **在 `nemu` 平台脚本里，程序链接地址、后续 NEMU 装载地址、CPU 起始执行地址，被统一锚定到了 `0x80000000`。**

---

### 它如何生成 `.bin`

关键规则：

```makefile
image: image-dep
	@$(OBJDUMP) -d $(IMAGE).elf > $(IMAGE).txt
	@$(OBJCOPY) -S --set-section-flags .bss=alloc,contents -O binary $(IMAGE).elf $(IMAGE).bin
```

这一步会生成三类文件：

* `$(IMAGE).elf`：带地址语义的最终程序
* `$(IMAGE).txt`：反汇编结果，便于阅读
* `$(IMAGE).bin`：去掉 ELF 元信息后的纯机器码裸镜像

---

### 它如何把 `mainargs` 塞进镜像

关键规则：

```makefile
insert-arg: image
	@python $(AM_HOME)/tools/insert-arg.py $(IMAGE).bin $(MAINARGS_MAX_LEN) $(MAINARGS_PLACEHOLDER) "$(mainargs)"
```

作用：

* 先保证 `.bin` 已生成；
* 再把运行参数字符串填进程序预留区。

这说明：

> **客户程序 `main()` 的参数并不是由操作系统传入，而是由 AM 平台脚本后处理地写进镜像中的固定占位区域。**

---

### 它如何真正启动 NEMU

关键规则：

```makefile
run: insert-arg
	$(MAKE) -C $(NEMU_HOME) ISA=$(ISA) run ARGS="$(NEMUFLAGS)" IMG=$(IMAGE).bin
```

这是整条链里最关键的“跨世界调用”。

它把 AM 侧已经生成好的镜像交给 NEMU：

* `ISA=$(ISA)`：把当前 ISA 告诉 NEMU
* `ARGS="$(NEMUFLAGS)"`：把批处理和日志参数传给 NEMU
* `IMG=$(IMAGE).bin`：把最终客户镜像路径传给 NEMU

也就是说：

> **`platform/nemu.mk` 既负责生成镜像，也负责把镜像正式送进 NEMU。**

---

### 一句话总结

> **`platform/nemu.mk` 是 AM 与 NEMU 之间的中继桥：它决定客户程序的链接地址、镜像生成方式，以及把 `.bin` 镜像如何传给 NEMU 去运行。**

---

## 8. `abstract-machine/scripts/linker.ld`

### 核心功能
这不是 `Makefile`，但它是平台脚本逻辑不可分割的一部分。

它决定的是：

* `.text`
* `.data`
* `.rodata`
* `.bss`
* 栈
* 堆

在客户机地址空间中的布局。

### 与上层脚本的关系

它不是主动运行的，而是：

* 被 `platform/nemu.mk` 加入 `LDSCRIPTS`
* 再由 `abstract-machine/Makefile` 的链接规则传给 `ld`

所以它的地位是：

> **地址布局的最终裁判由 `linker.ld` 负责，但“是否使用它、给它传什么符号值”由 `platform/nemu.mk` 决定。**

---

<a id="part-nemu-build"></a>

# 第四部分：NEMU 侧构建与运行层

## 9. `nemu/Makefile`

### 核心功能
这是 NEMU 本体的总构建入口。

它的核心任务是：

1. 读取 `menuconfig` 生成的配置；
2. 根据配置决定 `GUEST_ISA`、`ENGINE`、编译器与编译选项；
3. 收集源码文件列表；
4. 决定链接方式；
5. 进入 `scripts/native.mk` 或 AM 目标模式。

---

### 核心变量与来源总表

| 变量 | 来源 | 作用 |
| :--- | :--- | :--- |
| `NEMU_HOME` | 环境变量/当前目录上下文 | NEMU 根目录 |
| `CONFIG_*` | `include/config/auto.conf` | menuconfig 结果 |
| `GUEST_ISA` | `$(CONFIG_ISA)` 去引号 | 当前客户机 ISA |
| `ENGINE` | `$(CONFIG_ENGINE)` 去引号 | 执行引擎类型 |
| `NAME` | `$(GUEST_ISA)-nemu-$(ENGINE)` | NEMU 二进制名 |
| `FILELIST_MK` | `find -L ./src -name filelist.mk` | 收集子目录源码表 |
| `DIRS-y / SRCS-y / SRCS-BLACKLIST-y` | 各 `filelist.mk` 和配置汇总 | 计算最终源码集合 |
| `CC` | `CONFIG_CC` 或默认值 | 实际编译器 |
| `CFLAGS_BUILD` | `CONFIG_CC_OPT` / debug / asan / lto | 构建优化与调试参数 |
| `CFLAGS_TRACE` | `CONFIG_ITRACE_COND` 派生 | 生成 `ITRACE_COND` 宏 |
| `CFLAGS` | 上述参数汇总 | 最终 C 编译参数 |
| `LDFLAGS` | 继承 `CFLAGS_BUILD` | 链接阶段构建参数 |

---

### 它的第一件大事：读取配置系统结果

关键语句：

```makefile
-include $(NEMU_HOME)/include/config/auto.conf
-include $(NEMU_HOME)/include/config/auto.conf.cmd
```

说明：

* `menuconfig` / `defconfig` 的结果不是直接写进 `Makefile`；
* 而是先生成 `auto.conf`；
* 再由 `nemu/Makefile` 把这些配置变量读进来。

例如当前配置里：

```makefile
CONFIG_ISA="riscv32"
CONFIG_ENGINE="interpreter"
CONFIG_CC="gcc"
CONFIG_ITRACE_COND="true"
```

---

### 它如何得到 `GUEST_ISA`

关键语句：

```makefile
GUEST_ISA ?= $(call remove_quote,$(CONFIG_ISA))
```

根据你当前的 `auto.conf`，最终得到：

```text
GUEST_ISA = riscv32
```

因此：

> **NEMU 侧真正采用哪种 ISA，本质上由 `menuconfig/defconfig` 生成的 `CONFIG_ISA` 决定。**

这一点要特别注意：

* AM 侧传过来的 `ISA=riscv32` 是外部调用参数；
* NEMU 自己内部真正用于编译源码的 `GUEST_ISA` 是由自己的配置文件解出的。

这两者最好保持一致。

---

### 它如何拼出二进制名

```makefile
NAME = $(GUEST_ISA)-nemu-$(ENGINE)
```

如果：

* `GUEST_ISA = riscv32`
* `ENGINE = interpreter`

那么最终 NEMU 程序名就是：

```text
riscv32-nemu-interpreter
```

---

### 它如何收集源码

关键流程：

```makefile
FILELIST_MK = $(shell find -L ./src -name "filelist.mk")
include $(FILELIST_MK)
```

作用：

* 遍历 `src/` 下所有子模块的 `filelist.mk`
* 让每个子模块自己声明：
  * 哪些目录属于 `DIRS-y`
  * 哪些目录黑名单属于 `DIRS-BLACKLIST-y`
  * 哪些源文件要编译或屏蔽

再统一汇总成最终：

```makefile
SRCS
```

因此：

> **NEMU 的源码组织采用“分模块各自报目录，总控层统一汇总”的策略。**

---

### 它如何生成关键宏

关键语句：

```makefile
CFLAGS_TRACE += -DITRACE_COND=$(if $(CONFIG_ITRACE_COND),$(call remove_quote,$(CONFIG_ITRACE_COND)),true)
CFLAGS  += $(CFLAGS_BUILD) $(CFLAGS_TRACE) -D__GUEST_ISA__=$(GUEST_ISA)
```

这说明：

* `ITRACE_COND` 不是在源码头文件里硬编码的；
* 它来自配置系统后再拼成 `-D` 参数；
* `__GUEST_ISA__` 也是由 `GUEST_ISA` 再拼成 `-D__GUEST_ISA__=riscv32`。

这些宏后续会直接影响：

* `isa.h`
* `cpu-exec.c`
* 以及很多 `IFDEF/MUXDEF` 逻辑。

---

### 它如何决定后续走哪条构建路径

关键分支：

```makefile
ifdef CONFIG_TARGET_AM
include $(AM_HOME)/Makefile
LINKAGE += $(ARCHIVES)
else
include $(NEMU_HOME)/scripts/native.mk
endif
```

当前一般不会走 `CONFIG_TARGET_AM`，而是走：

```makefile
include $(NEMU_HOME)/scripts/native.mk
```

也就是说，在你当前常见场景下：

> **`nemu/Makefile` 最后会把“真实编译与运行规则”交给 `scripts/native.mk`。**

---

### 一句话总结

> **`nemu/Makefile` 负责把 `menuconfig` 生成的配置翻译成具体的源码集合、编译参数和宏定义，再把实际构建/运行工作交给 `scripts/native.mk`。**

---

## 10. `nemu/scripts/config.mk`

### 核心功能
这个脚本是 NEMU 的 **配置系统驱动层**。

它负责：

* 构建 Kconfig 工具；
* 执行 `menuconfig`；
* 加载 `defconfig`；
* 同步生成：
  * `.config`
  * `include/config/auto.conf`
  * `include/generated/autoconf.h`

---

### 核心变量与来源

| 变量 | 来源 | 作用 |
| :--- | :--- | :--- |
| `KCONFIG_PATH` | 文件内定义 | `tools/kconfig` 路径 |
| `FIXDEP_PATH` | 文件内定义 | `tools/fixdep` 路径 |
| `Kconfig` | 文件内定义 | 顶层 `Kconfig` 文件 |
| `CONF` | 派生变量 | 文本配置程序 |
| `MCONF` | 派生变量 | 图形菜单配置程序 |
| `FIXDEP` | 派生变量 | 依赖修复工具 |
| `rm-distclean` | 文件内定义 | `distclean` 需要删除的配置产物 |

---

### 关键目标与意义

#### `menuconfig`
```makefile
menuconfig: $(MCONF) $(CONF) $(FIXDEP)
	$(Q)$(MCONF) $(Kconfig)
	$(Q)$(CONF) $(silent) --syncconfig $(Kconfig)
```
作用：

* 打开交互配置界面；
* 把结果同步到各种配置输出文件。

#### `%defconfig`
```makefile
%defconfig: $(CONF) $(FIXDEP)
	$(Q)$< $(silent) --defconfig=configs/$@ $(Kconfig)
	$(Q)$< $(silent) --syncconfig $(Kconfig)
```

如果你执行：

```bash
make riscv32-am_defconfig
```

它会读取：

```text
configs/riscv32-am_defconfig
```

再生成当前 `.config` 与对应自动配置头。

---

### 它与 `nemu/Makefile` 的关系

逻辑顺序是：

1. `config.mk` 先负责“怎么生成配置”；
2. `nemu/Makefile` 再负责“怎么读取配置并构建 NEMU”。

因此：

> **`config.mk` 解决的是“配置从哪来”，`nemu/Makefile` 解决的是“拿着这些配置怎么编”。**

---

### 一句话总结

> **`config.mk` 是 NEMU 的 Kconfig 配置入口，不负责编译源码本身，但它决定了后续编译会看到哪些 `CONFIG_*` 配置。**

---

## 11. `nemu/scripts/build.mk`

### 核心功能
这个脚本是 NEMU 的 **通用编译规则层**。

它不关心 NEMU 跑什么程序，而是统一定义：

* 对象文件输出目录；
* 二进制输出路径；
* `.c/.cc` 的编译规则；
* 最终链接规则；
* 依赖文件 `.d` 的包含规则；
* `clean` 清理规则。

---

### 核心变量与来源

| 变量 | 来源 | 作用 |
| :--- | :--- | :--- |
| `WORK_DIR` | `pwd` | 当前 NEMU 目录 |
| `BUILD_DIR` | 派生变量 | 构建输出目录 |
| `INC_PATH` | 当前目录 include 加外部路径 | 头文件目录集合 |
| `OBJ_DIR` | 由 `NAME` 派生 | 对象文件目录 |
| `BINARY` | 由 `NAME` 派生 | 最终二进制路径 |
| `INCLUDES` | `INC_PATH` 转成 `-I` 形式 | 头文件搜索参数 |
| `OBJS` | 由 `SRCS/CXXSRC` 派生 | 要编译的对象文件列表 |

---

### 它如何组织目标文件与二进制文件

#### 对象文件目录
```makefile
OBJ_DIR  = $(BUILD_DIR)/obj-$(NAME)$(SO)
```

#### 二进制路径
```makefile
BINARY   = $(BUILD_DIR)/$(NAME)$(SO)
```

这意味着：

* 中间 `.o` 和 `.d` 会进对象目录；
* 最终程序会直接放在 `build/` 下。

---

### 它如何定义编译规则

例如：

```makefile
$(OBJ_DIR)/%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<
	$(call call_fixdep, $(@:.o=.d), $@)
```

说明：

* `nemu/Makefile` 负责把 `CC/CFLAGS/SRCS` 准备好；
* `build.mk` 负责把这些变量真正执行成编译动作。

所以：

> **`build.mk` 更像 NEMU 侧的“通用施工队”，而不是策略制定者。**

---

### 一句话总结

> **`build.mk` 负责把上层汇总好的源码和参数真正转成 `.o` 和最终 `BINARY`。**

---

## 12. `nemu/scripts/native.mk`

### 核心功能
这是当前最常用的 NEMU **本地可执行构建与运行入口**。

它承担三件关键事：

1. 引入根目录 `Makefile` 的 git 辅助宏；
2. 引入 `build.mk` 和 `difftest.mk`；
3. 定义 `run` / `gdb` / `run-env` 等最终目标。

---

### 核心变量与来源

| 变量 | 来源 | 作用 |
| :--- | :--- | :--- |
| `ARGS` | 外部传入或默认值 | NEMU 启动参数 |
| `ARGS_DIFF` | `tools/difftest.mk` | difftest 相关追加参数 |
| `IMG` | 外部传入 | 客户镜像路径 |
| `NEMU_EXEC` | `$(BINARY) $(ARGS) $(IMG)` | 最终执行命令 |
| `DIFF_REF_SO` | `difftest.mk` | 参考模型动态库 |

---

### 它如何吸收根 `Makefile`

关键语句：

```makefile
-include $(NEMU_HOME)/../Makefile
```

作用：

* 即使根目录 `Makefile` 不存在也不报错；
* 若存在，则把 `git_commit` 等宏吸进来。

于是它可以调用：

```makefile
$(call git_commit, "compile NEMU")
$(call git_commit, "run NEMU")
```

---

### 它如何定义最终运行命令

关键语句：

```makefile
IMG ?=
NEMU_EXEC := $(BINARY) $(ARGS) $(IMG)
```

这条语句极其重要，因为它说明：

> **NEMU 最终执行命令的组成只有三块：模拟器本体 + 运行参数 + 客户镜像。**

举例来说，如果上游 AM 传来：

* `ARGS="-b -l /path/nemu-log.txt"`
* `IMG=/path/string-riscv32-nemu.bin`

那么最终命令就近似于：

```text
build/riscv32-nemu-interpreter -b -l /path/nemu-log.txt /path/string-riscv32-nemu.bin
```

---

### 它如何运行 NEMU

关键规则：

```makefile
run: run-env
	$(call git_commit, "run NEMU")
	$(NEMU_EXEC)
```

说明：

* `run-env` 先保证 `$(BINARY)`、`$(DIFF_REF_SO)` 已就绪；
* 然后真正执行 `$(NEMU_EXEC)`。

因此：

> **NEMU 的最终启动落点就在 `native.mk` 里，不在 `nemu/Makefile` 顶层。**

---

### 一句话总结

> **`native.mk` 是 NEMU 侧真正把“编译完成的 NEMU”跑起来的最后一跳。**

---

<a id="section-relations"></a>

# 第五部分：脚本之间的逻辑关系梳理

## 13. 按职责划分的脚本关系

### A. 选择应用层
负责回答：**“这次要编哪个客户程序？”**

相关脚本：

* `am-kernels/tests/cpu-tests/Makefile`
* `am-kernels/kernels/hello/Makefile`
* 其他应用目录下的简短 `Makefile`

它们一般只声明：

* `NAME`
* `SRCS`

然后把控制权交给 `abstract-machine/Makefile`。

---

### B. AM 总控层
负责回答：**“这个客户程序在当前 `ARCH` 下该怎么编？”**

相关脚本：

* `abstract-machine/Makefile`

它负责：

* 解析 `ARCH`
* 准备工具链变量
* 编应用
* 编 `am` / `klib`
* 链成 `.elf`

---

### C. ISA / 平台细化层
负责回答：**“这个 `ARCH` 的具体 ISA 和平台细节是什么？”**

相关脚本：

* `abstract-machine/scripts/riscv32-nemu.mk`
* `abstract-machine/scripts/isa/riscv.mk`
* `abstract-machine/scripts/platform/nemu.mk`
* `abstract-machine/scripts/linker.ld`

这层脚本决定：

* 用什么交叉工具链
* 用什么编译参数
* 链接到哪个地址
* 如何生成 `.bin`
* 如何把 `.bin` 交给 NEMU

---

### D. NEMU 配置层
负责回答：**“NEMU 自己该按什么配置编译？”**

相关脚本：

* `nemu/scripts/config.mk`
* `nemu/Kconfig`
* `nemu/include/config/auto.conf`
* `nemu/include/generated/autoconf.h`

这层系统决定：

* `CONFIG_ISA`
* `CONFIG_ENGINE`
* `CONFIG_TRACE`
* `CONFIG_DEVICE`
* `CONFIG_MBASE`
* `CONFIG_ITRACE_COND`

等一系列配置变量。

---

### E. NEMU 构建运行层
负责回答：**“NEMU 本体怎么编、怎么跑？”**

相关脚本：

* `nemu/Makefile`
* `nemu/scripts/build.mk`
* `nemu/scripts/native.mk`

它们负责：

* 汇总 NEMU 源码
* 拼接 NEMU 侧 `CFLAGS`
* 构建 `build/riscv32-nemu-interpreter`
* 执行 `BINARY + ARGS + IMG`

---

## 14. 这套系统的控制权是如何层层下放的

可以把它理解成“多级委托模型”：

### 第 1 级委托：应用选择层 → AM 总控层
应用层只说：

* 我叫 `string`
* 我的源码是 `tests/string.c`

然后委托给 AM 总控。

### 第 2 级委托：AM 总控层 → ISA/平台脚本层
AM 总控只说：

* 当前 `ARCH=riscv32-nemu`
* 请对应脚本告诉我该怎么编

于是委托给：

* `riscv32-nemu.mk`
* `isa/riscv.mk`
* `platform/nemu.mk`

### 第 3 级委托：平台脚本层 → NEMU 构建层
平台脚本只说：

* 我的 `.bin` 已经准备好
* 请 NEMU 按指定参数运行它

于是委托给：

* `nemu/Makefile`
* `nemu/scripts/native.mk`

### 第 4 级委托：NEMU 构建层 → NEMU 可执行程序本体
最终 `native.mk` 把所有变量折叠成：

```text
$(BINARY) $(ARGS) $(IMG)
```

然后交给 shell 执行。

---

<a id="section-var-flow"></a>

# 第六部分：变量流向图

## 15. 变量流向图（一）：从命令行到 AM 再到 NEMU

```text
用户命令行
make ARCH=riscv32-nemu ALL=string run
│
├─> am-kernels/tests/cpu-tests/Makefile
│    │
│    ├─ 读取 ALL=string
│    ├─ 生成 Makefile.string
│    └─ 递归执行 make -f Makefile.string ARCH=riscv32-nemu run
│
├─> Makefile.string
│    │
│    ├─ NAME=string
│    ├─ SRCS=tests/string.c
│    └─ include $(AM_HOME)/Makefile
│
├─> abstract-machine/Makefile
│    │
│    ├─ ARCH = riscv32-nemu
│    ├─ ARCH_SPLIT = riscv32 nemu
│    ├─ ISA = riscv32
│    ├─ PLATFORM = nemu
│    ├─ IMAGE = .../build/string-riscv32-nemu
│    └─ include scripts/riscv32-nemu.mk
│
├─> scripts/riscv32-nemu.mk
│    │
│    ├─ include isa/riscv.mk
│    └─ include platform/nemu.mk
│
├─> scripts/isa/riscv.mk
│    │
│    ├─ CROSS_COMPILE = riscv64-linux-gnu-
│    └─ 提供 RISC-V 通用编译参数
│
├─> scripts/platform/nemu.mk
│    │
│    ├─ NEMUFLAGS += -b -l .../nemu-log.txt
│    ├─ LDFLAGS += --defsym=_pmem_start=0x80000000
│    ├─ 生成 IMAGE.elf / IMAGE.bin
│    └─ make -C $(NEMU_HOME) ISA=$(ISA) run ARGS="$(NEMUFLAGS)" IMG=$(IMAGE).bin
│
└─> nemu/Makefile
     │
     ├─ 读取 auto.conf 得到 CONFIG_ISA="riscv32"
     ├─ GUEST_ISA = riscv32
     ├─ NAME = riscv32-nemu-interpreter
     ├─ CFLAGS += -D__GUEST_ISA__=riscv32
     └─ include scripts/native.mk
          │
          └─ NEMU_EXEC = $(BINARY) $(ARGS) $(IMG)
               = build/riscv32-nemu-interpreter + "-b -l ..." + string-riscv32-nemu.bin
```

---

## 16. 变量流向图（二）：几个最重要变量的单独追踪

### 变量 1：`ARCH`

```text
命令行传入: ARCH=riscv32-nemu
-> abstract-machine/Makefile 读取
-> 拆成 ISA=riscv32, PLATFORM=nemu
-> include scripts/riscv32-nemu.mk
```

### 变量 2：`ISA`

```text
在 AM 中由 ARCH 拆出: ISA=riscv32
-> platform/nemu.mk 用于 make -C $(NEMU_HOME) ISA=$(ISA) run
-> 传给 NEMU 外层调用
```

### 变量 3：`GUEST_ISA`

```text
NEMU 配置系统生成: CONFIG_ISA="riscv32"
-> nemu/Makefile: GUEST_ISA = remove_quote(CONFIG_ISA)
-> CFLAGS 中生成 -D__GUEST_ISA__=riscv32
-> isa.h 等源码据此选择 riscv32 实现
```

### 变量 4：`IMAGE`

```text
abstract-machine/Makefile 根据 NAME 和 ARCH 派生
-> IMAGE = .../build/string-riscv32-nemu
-> platform/nemu.mk 生成 IMAGE.elf / IMAGE.bin / IMAGE.txt
-> platform/nemu.mk 把 IMG=$(IMAGE).bin 传给 NEMU
```

### 变量 5：`ARGS` / `NEMUFLAGS`

```text
platform/nemu.mk 生成 NEMUFLAGS
-> 典型值: -b -l .../nemu-log.txt
-> make -C $(NEMU_HOME) run ARGS="$(NEMUFLAGS)"
-> native.mk 中 NEMU_EXEC = $(BINARY) $(ARGS) $(IMG)
```

---

<a id="section-include-graph"></a>

# 第七部分：include 嵌套图

## 17. include 嵌套图（一）：AM 侧

```text
应用目录 Makefile / cpu-tests 动态生成的 Makefile.<test>
│
└─ include $(AM_HOME)/Makefile
   │
   └─ -include $(AM_HOME)/scripts/$(ARCH).mk
      │
      └─ include $(AM_HOME)/scripts/riscv32-nemu.mk
         │
         ├─ include $(AM_HOME)/scripts/isa/riscv.mk
         │
         └─ include $(AM_HOME)/scripts/platform/nemu.mk
                │
                └─ 使用 $(AM_HOME)/scripts/linker.ld 作为链接脚本
```

这张图说明：

* AM 应用层只 include 一次 `$(AM_HOME)/Makefile`；
* 剩下的脚本分发和嵌套都由 AM 总控继续完成。

---

## 18. include 嵌套图（二）：NEMU 侧

```text
nemu/Makefile
│
├─ -include include/config/auto.conf
├─ -include include/config/auto.conf.cmd
├─ include src/**/filelist.mk
├─ include scripts/config.mk
└─ include scripts/native.mk
   │
   ├─ -include ../Makefile
   ├─ include scripts/build.mk
   └─ include tools/difftest.mk
```

这张图说明：

* NEMU 侧是“配置先行”；
* 然后读取源码模块列表；
* 最后把具体构建/运行规则交给 `native.mk`。

---

## 19. include 嵌套图（三）：全局拼接图

```text
用户应用入口 Makefile
│
├─ include abstract-machine/Makefile
│  │
│  └─ include abstract-machine/scripts/riscv32-nemu.mk
│     ├─ include abstract-machine/scripts/isa/riscv.mk
│     └─ include abstract-machine/scripts/platform/nemu.mk
│        └─ run: make -C nemu ...
│
└──────────────────────────────────────────────► nemu/Makefile
                                                │
                                                ├─ include scripts/config.mk
                                                └─ include scripts/native.mk
                                                   ├─ include scripts/build.mk
                                                   └─ include ../Makefile
```

这就是整个“AM 侧脚本树”和“NEMU 侧脚本树”相接的地方。

---

<a id="section-confusions"></a>

# 第八部分：最容易混淆的几个点

## 20. 为什么 `ARCH=riscv32-nemu`，但工具链前缀却是 `riscv64-linux-gnu-`？

因为工具链二进制名字叫 `riscv64-linux-gnu-gcc`，并不意味着只能生成 64 位目标。

真正决定目标位宽的是后续参数：

* `-march=rv32im_zicsr`
* `-mabi=ilp32`
* `-melf32lriscv`

所以记住：

> **工具链程序名不等于最终目标位宽。**

---

## 21. 为什么 `platform/nemu.mk` 要传 `ISA=$(ISA)` 给 NEMU，但 `nemu/Makefile` 又自己从 `CONFIG_ISA` 读 `GUEST_ISA`？

因为这是两层系统：

* AM 侧通过 `ISA=$(ISA)` 告诉 NEMU：“我这次希望你按这个 ISA 跑”；
* NEMU 自己内部仍然依赖配置系统生成的 `CONFIG_ISA` 来决定真正编译哪些源码。

因此实践中最重要的是：

> **AM 传给 NEMU 的 `ISA`，必须和 NEMU 当前 `auto.conf` 中的 `CONFIG_ISA` 保持一致。**

否则就会出现“AM 生成的是 riscv32 镜像，但 NEMU 本体却不是按 riscv32 编译”的错位。

---

## 22. 为什么应用层 `Makefile` 那么短，却能完成这么复杂的构建？

因为应用层只负责声明元数据：

* `NAME`
* `SRCS`

而真正复杂的交叉编译、链接、递归库构建、平台适配、NEMU 调用，全部被“统一沉到”后面的通用脚本里了。

这是很典型的：

> **上层极简、底层统一托管**

的脚本架构设计。

---

## 23. 为什么说 `platform/nemu.mk` 是 AM 与 NEMU 的桥？

因为它是第一层真正同时知道两边信息的脚本：

* 它知道 AM 这边的 `IMAGE.elf/.bin`；
* 它也知道 NEMU 这边要用 `make -C $(NEMU_HOME) ... IMG=...` 来运行。

所以它既属于 AM 世界，又负责把构建结果递送给 NEMU 世界。

---

<a id="section-summary"></a>

# 第九部分：最终总结

## 24. 一页纸总复盘

如果你之后只想用最短时间把整套 `make ARCH=riscv32-nemu ALL=string run` 的执行链重新捡起来，可以直接回看这一节。

### 1. 先记总链，不要先陷进语法细节

整条最核心的执行链可以直接压成：

```text
make ARCH=riscv32-nemu ALL=string run
-> cpu-tests/Makefile 选定测试并生成 Makefile.string
-> Makefile.string include abstract-machine/Makefile
-> abstract-machine/Makefile 把客户程序编成 string-riscv32-nemu.elf
-> platform/nemu.mk 把 .elf 提成 .bin，并把参数写进镜像
-> make -C nemu ISA=riscv32 run ARGS="..." IMG=...bin
-> nemu/Makefile 按当前配置编出 riscv32-nemu-interpreter
-> native.mk 折叠成 NEMU_EXEC
-> Linux 启动 NEMU 进程
-> NEMU 解析参数并执行客户程序
```

只要这条主链没丢，细节再复杂也不容易迷路。

### 2. 三层执行模型一定要牢

#### 第一层：入口调度层

典型文件：

* `am-kernels/tests/cpu-tests/Makefile`

这一层只做三件事：

1. 接收命令行变量和目标；
2. 确定“这次到底跑哪个测试”；
3. 生成最小桥接入口 `Makefile.<test>`。

这一层不负责编译，不负责链接，也不负责运行 NEMU。

它最核心的职责是：

> **把用户命令翻译成一个明确、可下发的构建任务。**

#### 第二层：AM 总控层

典型文件：

* `abstract-machine/Makefile`

这一层负责：

1. 检查 `AM_HOME`、`ARCH`、`SRCS` 等构建条件；
2. 从 `ARCH=riscv32-nemu` 拆出 `ISA=riscv32` 与 `PLATFORM=nemu`；
3. 建立 `WORK_DIR`、`DST_DIR`、`IMAGE`、`OBJS`、`LINKAGE`；
4. 通过 `-include $(AM_HOME)/scripts/$(ARCH).mk` 补齐 ISA/平台细节；
5. 把客户程序链接成最终的 `$(IMAGE).elf`。

这一层最核心的职责是：

> **把“最小任务描述”扩展成一张完整构建图，并产出完整 ELF 程序镜像。**

#### 第三层：NEMU 运行层

典型文件：

* `abstract-machine/scripts/platform/nemu.mk`
* `nemu/Makefile`
* `nemu/scripts/native.mk`

这一层负责：

1. 把 `.elf` 提炼成 `.bin`；
2. 把参数注入 `.bin`；
3. 把 `ARGS` 与 `IMG` 透传给 NEMU；
4. 把 NEMU 本体按当前配置真正编出来；
5. 折叠成一条最终的宿主机运行命令。

这一层最核心的职责是：

> **把客户程序镜像交给模拟器，并把模拟器本体真正启动起来。**

### 3. `.elf` 和 `.bin` 一定不要混

这套系统里，文件形成顺序一定是：

```text
.c/.S
-> .o
-> .a
-> .elf
-> .bin
```

对应理解如下：

* `.o`：单个源文件编译出来的对象文件，是零件；
* `.a`：多个对象文件打包后的静态库，是零件包；
* `.elf`：把应用对象文件、运行时库、链接脚本、入口地址全部装配完成后的完整程序；
* `.bin`：从 ELF 里再抽出来的纯机器码裸镜像。

一句话区分：

> **`.elf` 是“完整程序镜像”，`.bin` 是“给 NEMU 直接加载的纯机器码内容”。**

### 4. 最终真正执行的命令是什么

前面这么多 Makefile 最终收敛成的，不是一种抽象概念，而是一条宿主机命令。

形态大致是：

```bash
/home/l/ysyx/ysyx-workbench/nemu/build/riscv32-nemu-interpreter \
  -b \
  -l /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/nemu-log.txt \
  /home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/string-riscv32-nemu.bin
```

所以最后一定要看清三件事：

* `BINARY`：NEMU 自己的宿主机程序；
* `ARGS`：传给 NEMU 的启动参数；
* `IMG`：传给 NEMU 的客户程序镜像。

也就是说，`native.mk` 的：

```makefile
NEMU_EXEC := $(BINARY) $(ARGS) $(IMG)
```

就是整套脚本系统的最后落点。

### 5. 阅读这套脚本时永远问自己的五个问题

以后你再回读任意一个相关 Makefile，都建议先强制问自己下面五个问题：

1. 当前文件属于哪一层：入口调度层、AM 总控层，还是 NEMU 运行层？
2. 当前文件是在声明规则、补变量，还是在递归下发任务？
3. 当前目标是 `run`、`image`、`archive`，还是别的什么？
4. 当前变量来自命令行、当前 Makefile、include 文件，还是上层递归透传？
5. 当前动作为什么会执行：是因为被用户点名，还是因为它是最终目标的依赖？

只要这五个问题不丢，你基本就不会再把整套执行链看乱。

### 6. 整套链路的最终一句话

> **第一层负责“决定本轮任务是什么”，第二层负责“把任务构造成完整 ELF 程序”，第三层负责“把 ELF 变成可运行镜像并真正启动 NEMU”；Make 的执行顺序始终由依赖图决定，而不是由文件书写顺序决定。**

---

## 25. 整套脚本系统的核心逻辑收束

如果把这整套脚本逻辑压缩成四句话：

1. **应用层脚本只负责声明“我是谁、我要编哪些源文件”。**
2. **`abstract-machine/Makefile` 负责按 `ARCH` 把客户程序真正编译链接成镜像。**
3. **`abstract-machine/scripts/` 负责补齐 ISA 与平台细节，并把镜像转交给 NEMU。**
4. **`nemu/Makefile + scripts/native.mk` 负责构建模拟器本体，并以 `BINARY + ARGS + IMG` 的形式真正执行客户程序。**

---

## 26. 一句话终极总结

> **这套脚本系统本质上是一条“应用声明 -> AM 总控 -> ISA/平台细化 -> 镜像生成 -> NEMU 构建 -> NEMU 执行”的多级委托链；其中 `abstract-machine/Makefile` 负责把客户程序编出来，`platform/nemu.mk` 负责把客户镜像递给 NEMU，`nemu/Makefile` 负责把模拟器自己编出来，而 `native.mk` 则负责让模拟器带着镜像真正跑起来。**

---

<a id="section-e2e-run"></a>

# 第十部分：实战入口链路：从 `make ARCH=riscv32-nemu ALL=string run` 到 NEMU 启动

这一节只做一件事：以你最常用的入口命令为基准，把“谁先执行、谁后执行、变量在哪层生成、在哪层消费”完整串成一条可回放链路。

入口命令：

```bash
make ARCH=riscv32-nemu ALL=string run
```

---

<a id="section-e2e-run-chain"></a>

## 1. 一图看完整调用链

```text
am-kernels/tests/cpu-tests/Makefile
  (生成 Makefile.string 并递归)
-> make -f Makefile.string ARCH=riscv32-nemu run
-> include abstract-machine/Makefile
-> include abstract-machine/scripts/riscv32-nemu.mk
-> include abstract-machine/scripts/platform/nemu.mk
  (生成 IMAGE.elf / IMAGE.bin，并执行 insert-arg)
-> make -C $(NEMU_HOME) ISA=$(ISA) run ARGS="$(NEMUFLAGS) --elf=$(IMAGE).elf" IMG=$(IMAGE).bin
-> nemu/Makefile
-> include nemu/scripts/native.mk
-> NEMU_EXEC := $(BINARY) $(ARGS) $(IMG)
-> 启动 NEMU 进程
-> monitor.c::parse_args() 解析 -e/-l/位置参数
-> init_monitor() 初始化 log/mtrace/ftrace + 加载 img
```

这个链条里，AM 负责“产出客户程序镜像”，NEMU 负责“构建并启动模拟器本体”。

---

<a id="section-e2e-run-layers"></a>

## 2. 分层调用明细（上游 -> 下游）

### 第一层：测试调度层（上游入口）

文件：`am-kernels/tests/cpu-tests/Makefile`

职责：

1. 接收命令行变量：`ARCH=riscv32-nemu`、`ALL=string`、目标 `run`。
2. 生成桥接文件 `Makefile.string`（只包含 `NAME/SRCS/include $(AM_HOME)/Makefile`）。
3. 递归执行：`make -f Makefile.string ARCH=riscv32-nemu run`。

说明：这一层不做交叉编译细节，只做“选测试 + 下发任务”。

### 第二层：AM 总控 + 平台细化层（中游）

文件：`abstract-machine/Makefile`、`abstract-machine/scripts/riscv32-nemu.mk`、`abstract-machine/scripts/platform/nemu.mk`

职责：

1. `abstract-machine/Makefile` 拆分 `ARCH` 得到 `ISA=riscv32`、`PLATFORM=nemu`。
2. 统一建立 `IMAGE` 前缀，产出 `$(IMAGE).elf`。
3. 在 `platform/nemu.mk` 中执行：
  - `objcopy` 抽取 `$(IMAGE).bin`
  - `insert-arg` 注入 `mainargs`
  - 递归调用 NEMU：

```makefile
$(MAKE) -C $(NEMU_HOME) ISA=$(ISA) run ARGS="$(NEMUFLAGS) --elf=$(IMAGE).elf" IMG=$(IMAGE).bin
```

说明：这一层是“桥”，把 AM 产物和运行参数交给 NEMU。

### 第三层：NEMU 构建与运行层（下游落地）

文件：`nemu/Makefile`、`nemu/scripts/native.mk`、`nemu/src/monitor/monitor.c`

职责：

1. `nemu/Makefile` 组织源码和配置（`auto.conf` 等）。
2. `nemu/scripts/native.mk` 折叠最终命令：

```makefile
NEMU_EXEC := $(BINARY) $(ARGS) $(IMG)
```

3. `run` 目标执行 `$(NEMU_EXEC)`。
4. 运行期 `monitor.c` 解析参数并初始化各模块。

---

<a id="section-e2e-run-vars"></a>

## 3. 变量生命周期总表（谁产生，谁消费）

| 变量 | 产生层 | 首次赋值/来源 | 主要消费层 | 作用总结 |
| :--- | :--- | :--- | :--- | :--- |
| `ARCH` | 第一层（命令行） | `ARCH=riscv32-nemu` | AM 总控层 | 决定脚本分支与目标平台 |
| `ALL` | 第一层（命令行） | `ALL=string` | cpu-tests 调度层 | 选择本轮测试集合 |
| `MAKECMDGOALS` | 第一层（命令行） | `run` | 各层 Makefile | 决定执行目标 |
| `ISA` | 第二层（AM） | `ARCH` 拆分得到 `riscv32` | `platform/nemu.mk` | 递归传给 NEMU：`ISA=$(ISA)` |
| `IMAGE` | 第二层（AM） | `build/$(NAME)-$(ARCH)` | `platform/nemu.mk` | 派生 `.elf/.bin/.txt` |
| `NEMUFLAGS` | 第二层（平台脚本） | `-b -l .../nemu-log.txt` | `platform/nemu.mk` | 上游准备的 NEMU 参数包 |
| `ARGS` | 第三层（NEMU run） | 外层透传或默认值 | `nemu/scripts/native.mk` | NEMU 启动参数入口 |
| `IMG` | 第二层传入第三层 | `IMG=$(IMAGE).bin` | `nemu/scripts/native.mk` + monitor | 客户程序裸镜像路径 |
| `BINARY` | 第三层（NEMU 构建） | NEMU 编译产物路径 | `nemu/scripts/native.mk` | 模拟器本体可执行文件 |
| `NEMU_EXEC` | 第三层（NEMU） | `$(BINARY) $(ARGS) $(IMG)` | `run` 目标 | 最终真正执行的宿主机命令 |

记忆要点：

* 上游打包参数：`NEMUFLAGS`
* 下游接手参数：`ARGS`
* 客户程序镜像：`IMG`
* 最终落地命令：`NEMU_EXEC`

---

<a id="section-e2e-run-parse"></a>

## 4. 参数如何落到 monitor.c 的 `elf_file` / `img_file` / `log_file`

从命令参数到运行变量的映射关系如下：

```text
ARGS 中的 -e $(IMAGE).elf                                  -> parse_args() case 'e' -> elf_file
ARGS 中的 -l $(shell dirname $(IMAGE).elf)/nemu-log.txt    -> parse_args() case 'l' -> log_file
ARGS 中的 -i $(IMAGE).bin                                  -> parse_args() case ‘i‘ -> img_file
```

随后初始化阶段的关键消费点：

1. `init_log(log_file)`：初始化主日志。
2. `init_ftrace(elf_file)`：读取 ELF 符号，建立函数映射（需开启 FTRACE 且 elf_file 非空）。
3. `load_img()`：把 `img_file` 对应的 `.bin` 加载到 NEMU 模拟内存。

因此：

* `.elf` 用于“符号元信息”（给 ftrace）
* `.bin` 用于“实际执行字节流”（给 CPU 执行）

两者互补，不能互相替代。

---

<a id="section-e2e-run-debug"></a>

## 5. 最终命令形态与排错抓手

最终命令形态可抽象为：

```bash
<nemu-binary> -b -l <nemu-log-path> --elf=<image.elf> <image.bin>
```

排错时按这 5 个点逆向检查，最快：

1. `ARCH` 是否真的是 `riscv32-nemu`（防止走错脚本分支）。
2. `$(IMAGE).elf`、`$(IMAGE).bin` 是否都存在。
3. 递归调用里是否真的带了 `--elf=$(IMAGE).elf`。
4. NEMU 实际启动命令里是否保留了 `ARGS`
5. `monitor.c` 里 `elf_file` 与 `img_file` 是否都被成功解析。

如果你只记一句工程化结论：

> **这条链的本质是“上游 AM 产镜像并打包参数，下游 NEMU 接收参数并启动执行”；任何问题都可以按“变量在哪一层产生、在哪一层消费”来定位。**

---

[返回顶部](#top)