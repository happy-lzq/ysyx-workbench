# ELF 学习笔记：`man 5 elf`

> 本文汇总核心内容：
> 1) ELF 文件 4 大核心数据结构（`Ehdr`、`Phdr`、`Shdr`、`Nhdr`）成员功能、作用与相互关系
> 2) 查看整个 ELF 文件的常用命令参数
> 3) 真实 RISC-V ELF 文件可视化布局案例解析

## 目录

- [一、ELF 文件四大核心结构体详解](#q1)
  - [1.1 ELF Header（Ehdr）：文件总蓝图](#q1-1)
  - [1.2 Program Header（Phdr）：运行视图（装载与段）](#q1-2)
  - [1.3 Section Header（Shdr）：链接视图（逻辑与节）](#q1-3)
  - [1.4 Symbol Entry（Elf32_Sym）：符号表条目](#q1-4)
  - [1.5 Note Header（Nhdr）：附加元数据](#q1-5)
  - [1.6 总结：结构体之间的宏观关系（大楼模型）](#q1-6)
- [二、查看整个 ELF 文件的命令参数](#q2)
  - [2.1 `readelf` 常用参数（首选）](#q2-1)
  - [2.2 `objdump` 常用参数（补充）](#q2-2)
  - [2.3 原始字节查看（十六进制）](#q2-3)
  - [2.4 推荐排查顺序](#q2-4)
- [三、项目真实 ELF 可视化布局（`string-riscv32-nemu.elf`）](#q3)
  - [3.1 样本关键信息](#q3-1)
  - [3.2 从上到下文件布局（按偏移）](#q3-2)
  - [3.3 Section 偏移表（真实数据）](#q3-3)
  - [3.4 五个关键字段如何落在文件中](#q3-4)

---

<a id="q1"></a>
## 一、ELF 文件四大核心结构体详解

除了以下 4 个构成了 ELF 骨架的核心结构体，ELF 文件中还存在符号表（`ElfN_Sym`）、重定位表（`ElfN_Rel` / `ElfN_Rela`）、动态链接信息（`ElfN_Dyn`）等结构体。但掌握以下 4 个，就掌握了理解 ELF 的钥匙。

<a id="q1-1"></a>
### 1.1 ELF Header（Ehdr）：文件总蓝图

`ELF Header` 位于文件的最开头（偏移量为 0），作用是告诉操作系统和链接器这个文件是什么、适用于什么架构、以及**去哪里找其他的表（程序头表和节头表）**。简单理解：`Ehdr` 像“总目录”，后续要读什么都先看它。

以 `Elf32_Ehdr` 为例：

| 成员 | 类型 | 描述与作用 |
| :--- | :--- | :--- |
| `e_ident` | `unsigned char[16]` | **身份信息**。包含魔数(`0x7f 'E' 'L' 'F'`)、位宽(32/64)、端序(大/小端)、ELF版本及 ABI 信息。 |
| `e_type` | `Elf32_Half` | **文件类型**。`ET_REL`(可重定位)、`ET_EXEC`(可执行)、`ET_DYN`(共享库/PIE)、`ET_CORE`。 |
| `e_machine` | `Elf32_Half` | **目标架构**。如 x86, ARM, RISC-V (`EM_RISCV`) 等。 |
| `e_version` | `Elf32_Word` | **ELF 格式版本**。一般为 `EV_CURRENT(1)`。 |
| `e_entry` | `Elf32_Addr` | **程序入口虚拟地址**。加载完成后，通常从这里开始执行（执行起点）。 |
| `e_phoff` | `Elf32_Off` | **程序头表 (Phdr Table) 偏移量**。操作系统通过它找 Segment。 |
| `e_shoff` | `Elf32_Off` | **节头表 (Shdr Table) 偏移量**。链接器/工具通过它找 Section。 |
| `e_flags` | `Elf32_Word` | **架构私有标志位**。处理器特定的扩展信息。 |
| `e_ehsize` | `Elf32_Half` | **ELF Header 自身大小**。 |
| `e_phentsize`| `Elf32_Half` | **单个程序头表项大小**（步长）。 |
| `e_phnum` | `Elf32_Half` | **程序头表项数量**（Segment 数量）。 |
| `e_shentsize`| `Elf32_Half` | **单个节头表项大小**（步长）。 |
| `e_shnum` | `Elf32_Half` | **节头表项数量**（Section 数量）。 |
| `e_shstrndx` | `Elf32_Half` | **节头表名称字符串表在节头表数组列表中的索引**。对应: .shstrtab（极其重要，解析器靠它找 `.text` 等名字）。 |

**排错与关系要点：**
* **程序头表文件区间：** `e_phoff ~ e_phoff + e_phentsize * e_phnum`
* **节头表文件区间：** `e_shoff ~ e_shoff + e_shentsize * e_shnum`
* 如果 `e_shoff=0` 或 `e_shnum=0`：说明文件可能被 `strip`（剥离了调试和节信息），但如果它是一个可执行文件，依然可以只依赖 Phdr 运行。

<a id="q1-2"></a>
### 1.2 Program Header（Phdr）：运行视图（装载与段）

`Program Header` 为**操作系统加载程序**服务（Execution View）。它描述了“段”（Segment）。它告诉操作系统：把文件中的哪部分，映射到内存的哪个虚拟地址，并赋予什么权限。一个 Segment 通常包含一个或多个 Section。

以 `Elf32_Phdr` 为例：

| 成员 | 类型 | 描述与作用 |
| :--- | :--- | :--- |
| `p_type` | `Elf32_Word` | **段的类型**。`PT_LOAD`(可加载段，最重要)、`PT_DYNAMIC`(动态链接信息)、`PT_INTERP`(解释器路径) 等。 |
| `p_offset` | `Elf32_Off` | **文件偏移**。这个段在 ELF 文件中的绝对起始位置。 |
| `p_vaddr` | `Elf32_Addr` | **虚拟地址**。这个段期望被加载到内存中的位置。 |
| `p_paddr` | `Elf32_Addr` | **物理地址**。在现代带 MMU 的操作系统中通常被忽略或与 `vaddr` 相同。 |
| `p_filesz` | `Elf32_Word` | **文件大小**。这个段在 ELF 文件里占用的字节数。 |
| `p_memsz` | `Elf32_Word` | **内存大小**。这个段在内存里占用的字节数。 |
| `p_flags` | `Elf32_Word` | **内存权限标志**。读(`PF_R`)、写(`PF_W`)、执行(`PF_X`)。 |
| `p_align` | `Elf32_Word` | **内存对齐要求**。通常是操作系统的页大小（如 0x1000 / 4096）。 |

**核心关系分析（.bss 节的原理）：**
为什么会有 `filesz` 和 `memsz` 两个大小？通常 `p_memsz >= p_filesz`。如果 `memsz` 大于 `filesz`，多出来的部分在内存中会被操作系统初始化为 0。这就是 **`.bss` 节（未初始化全局变量）** 的实现原理：它在文件中不占空间（`filesz` 小），但在内存中需要空间（`memsz` 大）。

<a id="q1-3"></a>
### 1.3 Section Header（Shdr）：链接视图（逻辑与节）

`Section Header` 为**编译器和链接器**服务（Linking View）。它描述了“节”（Section）。它把文件按逻辑功能划分（如代码放 `.text`，已初始化数据放 `.data`，符号表放 `.symtab`）。

以 `Elf32_Shdr` 为例：

| 成员 | 类型 | 描述与作用 |
| :--- | :--- | :--- |
| `sh_name` | `Elf32_Word` | **节的名称索引**。（注意：这是一个整数！是 `.shstrtab` 字符串表中的索引）。 |
| `sh_type` | `Elf32_Word` | **节的类型**。`SHT_PROGBITS`(程序数据如代码/变量)、`SHT_SYMTAB`(符号表)、`SHT_STRTAB`(字符串表) 等。 |
| `sh_flags` | `Elf32_Word` | **节的标志**。`SHF_ALLOC`(运行时需分配内存)、`SHF_EXECINSTR`(可执行代码) 等。 |
| `sh_addr` | `Elf32_Addr` | **虚拟地址**。期望被加载到的内存地址（对于未链接的目标文件通常是 0）。 |
| `sh_offset`| `Elf32_Off` | **文件偏移**。这个节的实际数据在 ELF 文件中的起始位置。 |
| `sh_size` | `Elf32_Word` | **节的大小**。在文件中占用的大小（字节）。 |
| `sh_link` | `Elf32_Word` | **关联节索引**。链接到其他节的索引（表示节与节之间的依赖关系，极重要）。 |
| `sh_info` | `Elf32_Word` | **附加信息**。取决于节的类型。 |
| `sh_addralign`|`Elf32_Word` | **地址对齐要求**。如 4 字节或 8 字节对齐。 |
| `sh_entsize`| `Elf32_Word` | **条目大小**。如果节包含固定大小的数组（如符号表），这里是单个条目的大小。 |

**核心网状关系分析：**
1. **字符串解析 (`sh_name`)**：它本身不存字符串。解析过程：读取 `Ehdr.e_shstrndx` 找到字符串表节 -> 加上当前 `sh_name` 作为偏移量 -> 读出以 `\0` 结尾的真实名字（如 `".text"`）。
2. **节间依赖 (`sh_link` & `sh_info`)**：
   - 如果当前节是**重定位表** (`.rel.text`)：`sh_link` 会指向其对应的**符号表**节索引；`sh_info` 会指向它**要修改的目标节**（`.text`）索引。
   - 如果当前节是**符号表** (`.symtab`)：`sh_link` 会指向其依赖的**字符串表** (`.strtab`)节索引。

<a id="q1-4"></a>
### 1.4 Symbol Entry（Elf32_Sym）：符号表条目

`Symbol Entry` 是**符号表（`.symtab` 节）**中的一个条目。它用来记录程序中各种符号（函数、全局变量、对象文件中的重定位标记等）的属性信息。每个 `Elf32_Sym` 结构体实例代表一个符号在编译链接过程中或运行时的**身份与位置信息**。

以 `Elf32_Sym` 为例：

| 成员 | 类型 | 描述与作用 |
| :--- | :--- | :--- |
| `st_name` | `Elf32_Word` | **符号名称索引**。（注意：是整数索引！是 `.strtab` 字符串表中的偏移量。通过 `strtab_buf + st_name` 得到真实的 C 字符串）。 |
| `st_value` | `Elf32_Addr` | **符号的值**。通常是符号的**虚拟地址**（在可执行文件中）或相对地址（在可重定位文件中）。对于函数，就是函数入口点的地址；对于变量，就是变量的内存地址。 |
| `st_size` | `Elf32_Word` | **符号的大小**。（字节数）。对于函数，通常是函数代码的字节长度；对于变量，是占用的字节数。 |
| `st_info` | `unsigned char` | **符号类型和绑定属性**。将两个 4 位字段**打包**在一个字节内：高 4 位 = Bind（绑定），低 4 位 = Type（类别）。 |
| `st_other` | `unsigned char` | **符号可见性**。通常被忽略。`STV_DEFAULT`, `STV_INTERNAL`, `STV_HIDDEN`, `STV_PROTECTED` 等。 |
| `st_shndx` | `Elf32_Half` | **符号定义所在的节序号**。是节头表数组的索引，指向这个符号定义在哪个节里（如 `.text`, `.data` 等）。特殊值 `SHN_UNDEF` (0) 表示未定义的外部符号；`SHN_ABS` 表示绝对值符号。 |

#### st_info 深度拆解（极其重要）

`st_info` 虽然只有 8 个字节，但它脆弱地打包了两份关键信息：

```
st_info 字节布局 (byte):
┌──────────────┬──────────────┐
│ Bit 7-4      │ Bit 3-0      │
│ Bind         │ Type         │
├──────────────┼──────────────┤
│ Binding      │ Symbol Type  │
└──────────────┴──────────────┘
```

**Type（低 4 位）**：用宏 `ELF32_ST_TYPE(st_info)` 提取。定义了符号的类别：

| 值 | 宏常量 | 含义 |
| :--- | :--- | :--- |
| 0 | `STT_NOTYPE` | 未知类型（一般用于占位或未初始化符号）。 |
| 1 | `STT_OBJECT` | 数据对象（全局变量、静态变量、数组等）。 |
| 2 | `STT_FUNC` | **函数或可执行代码**（这是 `ftrace` 重点关注的类型！）。 |
| 3 | `STT_SECTION` | 代表一个节（Section）本身（通常是链接器内部使用）。 |
| 4 | `STT_FILE` | 代表源代码文件名（一般记录编译该目标文件的源文件路径）。 |
| 13-15 | `STT_LOPROC ... STT_HIPROC` | 处理器特定的扩展类型。 |

**Bind（高 4 位）**：用宏 `ELF32_ST_BIND(st_info)` 提取。定义了符号的**作用域与可见性**：

| 值 | 宏常量 | 含义 |
| :--- | :--- | :--- |
| 0 | `STB_LOCAL` | **局部符号**。只在其定义所在的目标文件内部可见（如函数的静态变量）。 |
| 1 | `STB_GLOBAL` | **全局符号**。在多个目标文件间可见，链接器可以合并重复定义。 |
| 2 | `STB_WEAK` | **弱符号**。全局可见，但优先级低于强符号。链接时若有强符号存在就用强的，没有才用弱的。 |
| 3-13 | `STB_LOPROC ... STB_HIPROC` | 处理器或操作系统特定的扩展。 |

#### 符号表与字符串表的协作

`Elf32_Sym` 中的 `st_name` 不是直接存储 UTF-8 字符串。它是一个**32 位整数**，代表该符号名字在 `.strtab` 字符串表中的**字节偏移量**。

真实的解析流程：

```c
// 假设已读取到 strtab_buf 和 sym
const char *symbol_name = strtab_buf + sym.st_name; // 用偏移量当指针加法
puts(symbol_name);  // 真实的 C 字符串（以 \0 结尾）
```

**为什么这样设计？**避免在每条符号表记录中都硬编码一个可变长度的字符串，节省空间。所有符号的名字都集中在一个大字符池（`.strtab`）里，通过索引引用，就像**数据库的外键关系**。

#### 关键公式

* **符号表中符号的总数**：`sym_count = symtab_shdr.sh_size / symtab_shdr.sh_entsize`
  * `sh_size`：`.symtab` 节的总字节数。
  * `sh_entsize`：单个 `Elf32_Sym` 的字节大小（通常 16 字节）。
  * 整除的结果就是符号条目总数。

* **地址区间判定（ftrace 用法）**：对于 `STT_FUNC` 类型的符号，程序运行时的地址范围是：`[st_value, st_value + st_size)`
  * 如果 CPU 当前 PC（程序计数器）落在这个范围内，说明正在执行这个函数。

<a id="q1-5"></a>
### 1.5 Note Header（Nhdr）：附加元数据

`Note Header` 提供由操作系统或工具链添加的“元数据”（Metadata）。比如记录编译器的版本、构建 ID（Build ID），或者是程序崩溃时产生的 Core Dump 文件中的寄存器状态。

以 `Elf32_Nhdr` 为例：

| 成员 | 类型 | 描述与作用 |
| :--- | :--- | :--- |
| `n_namesz` | `Elf32_Word` | **名称长度**。Note 所有者名称的长度（如 "GNU\0" 长度为 4）。 |
| `n_descsz` | `Elf32_Word` | **描述数据长度**。实际元数据有效载荷的长度。 |
| `n_type` | `Elf32_Word` | **Note 类型**。如 `NT_GNU_BUILD_ID`, `NT_PRSTATUS` 等。 |

**内存布局分析：**
`Elf32_Nhdr` 只是一个**头部**，它后面会紧跟着实际的数据。它在文件中的实际连续布局通常是：
`[ Elf32_Nhdr (12字节) ]` -> `[ name 字符串数据 (n_namesz) ]` -> `[ 4字节对齐 Padding ]` -> `[ descriptor 描述数据 (n_descsz) ]` -> `[ 4字节对齐 Padding ]`。

<a id="q1-6"></a>
### 1.6 总结：结构体之间的宏观关系（大楼模型）

如果你把一个 ELF 文件想象成一座大楼：

1. **`Elf32_Ehdr` (大楼向导牌)**：站在大楼门口，它告诉你大楼的主人是谁（架构/系统），大厅在哪里（Entry），以及“按功能划分的平面图”（Shdr 表）在几层，“消防/电力等物业管理图”（Phdr 表）在几层。
2. **`Elf32_Shdr` (房间平面图 / 链接视图)**：给**装修工人（链接器）**看的。它把文件细分为代码房间(`.text`)、数据房间(`.data`)、工具间(`.symtab` 符号表)。房间之间通过电话线（`sh_link` / `sh_info`）相互依赖。
3. **`Elf32_Sym` (住户档案卡 / 符号条目)**：存放在 `.symtab` 房间里的单条记录。每张档案卡写着住户是谁（`st_name`，需要去 `.strtab` 名字池换真实姓名）、住在哪里（`st_value` 地址）、占多大空间（`st_size`），以及身份类别（`st_info`，如 `STT_FUNC` / `STT_OBJECT`）。
4. **`Elf32_Phdr` (物业管理图 / 运行视图)**：给**大楼管理员（操作系统内核）**看的。管理员不关心哪个房间存什么代码，他只关心：这一层楼（Segment）是只读的还是可写的？因此，`Phdr` 的机制会把属性相同相邻的 `Shdr` 房间打包成一整块（Segment），统一映射到内存中。
5. **`Elf32_Nhdr` (大楼铭牌)**：嵌在大楼墙上的铜牌，记录了大楼是哪年建的，承建商是谁（构建版本信息）。

---

<a id="q2"></a>
## 二、查看整个 ELF 文件的命令参数

<a id="q2-1"></a>
### 2.1 `readelf` 常用参数（首选）

`readelf` 专注 ELF 结构解析，最适合学习与定位格式问题。

```bash
readelf -a <file>      # all：尽可能完整展示 ELF 信息
readelf -h <file>      # 解析 ELF Header (Ehdr)
readelf -l <file>      # 解析 Program Headers (Phdr)
readelf -S <file>      # 解析 Section Headers (Shdr)
readelf -s <file>      # 符号表 (Symbol table)
readelf -r <file>      # 重定位信息 (Relocations)
readelf -d <file>      # 动态段（.dynamic）
readelf -n <file>      # 解析 Note 信息 (Nhdr)
readelf -V <file>      # 版本信息
readelf -W -a <file>   # 宽输出，避免终端折行时字段截断
<a id="q2-2"></a>
### 2.2 `objdump` 常用参数（补充）

`objdump` 在反汇编和二进制内容查看上更强。

```bash
objdump -x <file>      # 综合信息（头、段、符号等）
objdump -h <file>      # 节头摘要
objdump -d <file>      # 反汇编可执行代码节
objdump -D <file>      # 反汇编所有可反汇编节
objdump -s <file>      # 节内容（十六进制）
objdump -t <file>      # 符号表
objdump -r <file>      # 重定位
objdump -p <file>      # 私有头信息（动态链接相关）
```

<a id="q2-3"></a>
### 2.3 原始字节查看（十六进制）

如果要看“文件原始字节”，可用：

```bash
xxd <file> | less
hexdump -C <file> | less
```

<a id="q2-4"></a>
### 2.4 推荐排查顺序

建议按下面顺序看，效率更高：

1. `readelf -h <file>`：先确认文件身份/架构/位宽/端序
2. `readelf -l <file>`：确认装载视角（段）
3. `readelf -S <file>`：确认链接视角（节）
4. `readelf -s -r <file>`：看符号和重定位
5. `objdump -d <file>`：结合反汇编看实际执行代码

---

以上内容可作为 `man 5 elf` 的入门索引笔记，后续可继续补充 `Program Header` 与 `Section Header` 的字段级详细说明。

---

<a id="q3"></a>
## 三、项目真实 ELF 可视化布局（`string-riscv32-nemu.elf`）

样本文件：

- `/home/l/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/string-riscv32-nemu.elf`

<a id="q3-1"></a>
### 3.1 样本关键信息

根据 `readelf -h`：

根据 `readelf -h`：

| 字段           | 值                       | 注释                                                  |
| :------------- | :----------------------- | :---------------------------------------------------- |
| 文件大小       | `6516` bytes（`0x1974`） | ELF 文件总字节数                                      |
| 类别/端序/架构 | `ELF32`，小端，`RISC-V`  | 文件格式、字节序与目标 ISA                            |
| `e_type`       | `ET_EXEC`                | 文件类型（可执行文件）                                |
| `e_entry`      | `0x80000000`             | 程序入口虚拟地址                                      |
| `e_ehsize`     | `52`（`0x34`）           | 头表大小（ELF Header 自身大小）                       |
| `e_phoff`      | `52`（`0x34`）           | 程序头表偏移（Program Header Table）                  |
| `e_phentsize`  | `32`                     | 单个程序头表项大小（Program Header Entry Size）       |
| `e_phnum`      | `3`                      | 程序头表项数量（Program Header Entry Count）          |
| `e_shoff`      | `6076`（`0x17bc`）       | 节头表偏移（Section Header Table）                    |
| `e_shentsize`  | `40`                     | 单个节头表项大小（Section Header Entry Size）         |
| `e_shnum`      | `11`                     | 节头表项数量（Section Header Entry Count）            |
| `e_shstrndx`   | `10`                     | 节名字符串表索引（Section Header String Table Index） |

快速校验：

- Program Header Table 总大小：`32 * 3 = 96`（`0x60`）
- PHT 区间：`0x34 ~ 0x94`
- Section Header Table 总大小：`40 * 11 = 440`（`0x1b8`）
- SHT 区间：`0x17bc ~ 0x1974`（刚好到 EOF）

<a id="q3-2"></a>
### 3.2 从上到下文件布局（按偏移）

> 这是文件字节布局，不是运行时虚拟地址布局。

```text
0x0000 ┌────────────────────────────────────────────┐
  │ ELF Header (Ehdr)                          │
  │ size = 0x34 (52 bytes)                     │
0x0034 ├────────────────────────────────────────────┤
  │ Program Header Table (3 * 0x20 = 0x60)     │
0x0094 ├────────────────────────────────────────────┤
  │ (padding / 对齐空洞)                        │
0x1000 ├────────────────────────────────────────────┤
  │ .text      (0x250)                          │
0x1250 │ .rodata    (0x0b8)                          │
0x1308 │ .data      (0x018)                          │
0x1320 │ .sdata.str1(0x006)                          │
0x1326 │ .bss       (NOBITS，不占文件字节)           │
0x1326 │ .comment   (0x01a)                          │
0x1340 │ .riscv.attributes (0x033)                   │
0x1374 │ .symtab    (0x300)                          │
0x1674 │ .strtab    (0x0ec)                          │
0x1760 │ .shstrtab  (0x05b)                          │
0x17bc ├────────────────────────────────────────────┤
  │ Section Header Table (11 * 0x28 = 0x1b8)   │
0x1974 └────────────────────────────────────────────┘
  EOF
```

<a id="q3-3"></a>
### 3.3 Section 偏移表（真实数据）

|  # | Name                | Type            |      Off |     Size | 说明             |
| -: | :------------------ | :-------------- | -------: | -------: | :--------------- |
|  1 | `.text`             | PROGBITS        | `0x1000` | `0x0250` | 代码段           |
|  2 | `.rodata`           | PROGBITS        | `0x1250` | `0x00b8` | 只读数据         |
|  3 | `.data`             | PROGBITS        | `0x1308` | `0x0018` | 已初始化可写数据 |
|  4 | `.sdata.str1`       | PROGBITS        | `0x1320` | `0x0006` | 小数据字符串     |
|  5 | `.bss`              | NOBITS          | `0x1326` | `0x0014` | 不占文件字节     |
|  6 | `.comment`          | PROGBITS        | `0x1326` | `0x001a` | 编译注释信息     |
|  7 | `.riscv.attributes` | RISCV_ATTRIBUTE | `0x1340` | `0x0033` | RISC-V 属性      |
|  8 | `.symtab`           | SYMTAB          | `0x1374` | `0x0300` | 符号表           |
|  9 | `.strtab`           | STRTAB          | `0x1674` | `0x00ec` | 字符串表         |
| 10 | `.shstrtab`         | STRTAB          | `0x1760` | `0x005b` | 节名字符串表     |

<a id="q3-4"></a>
### 3.4 五个关键字段如何落在文件中

- `e_ehsize = 0x34`
  - 表示文件最开头 `0x0000 ~ 0x0033` 是 ELF Header 本体。

- `e_phentsize = 0x20`，`e_phnum = 3`
  - Program Header Table 总大小 `= 0x20 * 3 = 0x60`。
  - 结合 `e_phoff = 0x34`，得出 PHT 区间 `0x0034 ~ 0x0093`。

- `e_shentsize = 0x28`，`e_shnum = 11`
  - Section Header Table 总大小 `= 0x28 * 11 = 0x1b8`。
  - 结合 `e_shoff = 0x17bc`，得出 SHT 区间 `0x17bc ~ 0x1973`。

- SHT 末尾是 `0x1974`，恰好等于文件大小，说明这个 ELF 的节头表在文件尾部。

