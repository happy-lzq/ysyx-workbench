# ELF 学习笔记：`man 5 elf`

> 本文汇总前两问内容：
> 1) `ELF header (Ehdr)` 成员功能与作用
> 2) 查看整个 ELF 文件的常用命令参数

## 目录

- [一、ELF Header（Ehdr）成员详解](#q1)
  - [1.1 ELF Header 的整体作用](#q1-1)
  - [1.2 结构体成员逐项说明](#q1-2)
  - [1.3 字段关系与一致性检查](#q1-3)
  - [1.4 阅读与排错要点](#q1-4)
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
## 一、ELF Header（Ehdr）成员详解

<a id="q1-1"></a>
### 1.1 ELF Header 的整体作用

`ELF Header` 是整个 ELF 文件的入口元数据，作用是告诉加载器/链接器/分析工具：

- 这是不是一个 ELF 文件（魔数）
- 是 32 位还是 64 位
- 大小端格式是什么
- 文件类型、目标架构是什么
- 程序头表（Program Header Table）和节头表（Section Header Table）在文件中的位置与规模

简单理解：`Ehdr` 像“总目录”，后续要读什么都先看它。

<a id="q1-2"></a>
### 1.2 结构体成员逐项说明

以 `ElfN_Ehdr`（`N=32/64`）为例：

- `unsigned char e_ident[EI_NIDENT]`
  - 16 字节身份信息，最关键。
  - 常见子字段：
    - `EI_MAG0..EI_MAG3`：魔数，必须是 `0x7f 'E' 'L' 'F'`
    - `EI_CLASS`：位宽（`ELFCLASS32`/`ELFCLASS64`）
    - `EI_DATA`：大小端（`ELFDATA2LSB`/`ELFDATA2MSB`）
    - `EI_VERSION`：ELF 版本
    - `EI_OSABI`、`EI_ABIVERSION`：ABI 相关信息

- `uint16_t e_type`
  - 文件类型。
  - 常见值：`ET_REL`（可重定位）、`ET_EXEC`（可执行）、`ET_DYN`（共享库/PIE）、`ET_CORE`（core 文件）。

- `uint16_t e_machine`
  - 目标架构（如 `EM_X86_64`、`EM_RISCV`、`EM_MIPS` 等）。

- `uint32_t e_version`
  - ELF 格式版本，一般为 `EV_CURRENT(1)`。

- `ElfN_Addr e_entry`
  - 程序入口虚拟地址。加载并准备完成后，通常从这里开始执行。

- `ElfN_Off e_phoff`
  - 程序头表（`Program Header Table`）在文件中的起始偏移（字节）。

- `ElfN_Off e_shoff`
  - 节头表（`Section Header Table`）在文件中的起始偏移（字节）。

- `uint32_t e_flags`
  - 架构相关标志位（不同 ISA 含义不同，属于“架构私有扩展信息”）。

- `uint16_t e_ehsize`
  - ELF Header 自身大小（字节）。

- `uint16_t e_phentsize`
  - 单个程序头表项（`ElfN_Phdr`）大小。

- `uint16_t e_phnum`
  - 程序头表项数量。

- `uint16_t e_shentsize`
  - 单个节头表项（`ElfN_Shdr`）大小。

- `uint16_t e_shnum`
  - 节头表项数量。

- `uint16_t e_shstrndx`
  - “节名字符串表”所在节在节头表中的索引。
  - 工具通过它把 `sh_name` 解析成 `.text`、`.data`、`.bss` 等节名。

<a id="q1-3"></a>
### 1.3 字段关系与一致性检查

常用关系：

- 程序头表文件区间：
  - `e_phoff ~ e_phoff + e_phentsize * e_phnum`
- 节头表文件区间：
  - `e_shoff ~ e_shoff + e_shentsize * e_shnum`

一致性检查思路：

- `e_ident`（位宽/大小端）必须与解析方式一致
- `e_ehsize`、`e_phentsize`、`e_shentsize` 应与对应结构大小匹配
- 偏移与大小计算后不能越界文件范围

<a id="q1-4"></a>
### 1.4 阅读与排错要点

- 先看 `e_ident`：魔数、位宽、大小端是否正确
- 再看 `e_machine`：是否与目标架构一致
- 再看 `e_type`：文件类型是否符合预期
- 如果 `e_shoff=0` 或 `e_shnum=0`：可能是被 `strip` 后仍可运行，不一定损坏
- 可执行文件里若 `e_entry` 异常（如明显不在映射范围）通常值得重点排查

---

<a id="q2"></a>
## 二、查看整个 ELF 文件的命令参数

<a id="q2-1"></a>
### 2.1 `readelf` 常用参数（首选）

`readelf` 专注 ELF 结构解析，最适合学习与定位格式问题。

```bash
readelf -a <file>      # all：尽可能完整展示 ELF 信息
readelf -h <file>      # ELF Header
readelf -l <file>      # Program Headers（段）
readelf -S <file>      # Section Headers（节）
readelf -s <file>      # 符号表
readelf -r <file>      # 重定位信息
readelf -d <file>      # 动态段（.dynamic）
readelf -n <file>      # note 信息
readelf -V <file>      # 版本信息
readelf -W -a <file>   # 宽输出，避免字段截断
```

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

| 字段 | 值 | 注释 |
|---|---|---|
| 文件大小 | `6516` bytes（`0x1974`） | ELF 文件总字节数 |
| 类别/端序/架构 | `ELF32`，小端，`RISC-V` | 文件格式、字节序与目标 ISA |
| `e_type` | `ET_EXEC` | 文件类型（可执行文件） |
| `e_entry` | `0x80000000` | 程序入口虚拟地址 |
| `e_ehsize` | `52`（`0x34`） | 头表大小（ELF Header） |
| `e_phoff` | `52`（`0x34`） | 程序头表偏移（Program Header Table） |
| `e_phentsize` | `32` | 单个程序头表项大小（Program Header Entry Size） |
| `e_phnum` | `3` | 程序头表项数量（Program Header Entry Count） |
| `e_shoff` | `6076`（`0x17bc`） | 节头表偏移（Section Header Table） |
| `e_shentsize` | `40` | 单个节头表项大小（Section Header Entry Size） |
| `e_shnum` | `11` | 节头表项数量（Section Header Entry Count） |
| `e_shstrndx` | `10` | 节名字符串表索引（Section Header String Table Index） |

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

| # | Name | Type | Off | Size | 说明 |
|---:|---|---|---:|---:|---|
| 1 | `.text` | PROGBITS | `0x1000` | `0x0250` | 代码段 |
| 2 | `.rodata` | PROGBITS | `0x1250` | `0x00b8` | 只读数据 |
| 3 | `.data` | PROGBITS | `0x1308` | `0x0018` | 已初始化可写数据 |
| 4 | `.sdata.str1` | PROGBITS | `0x1320` | `0x0006` | 小数据字符串 |
| 5 | `.bss` | NOBITS | `0x1326` | `0x0014` | 不占文件字节 |
| 6 | `.comment` | PROGBITS | `0x1326` | `0x001a` | 编译注释信息 |
| 7 | `.riscv.attributes` | RISCV_ATTRIBUTE | `0x1340` | `0x0033` | RISC-V 属性 |
| 8 | `.symtab` | SYMTAB | `0x1374` | `0x0300` | 符号表 |
| 9 | `.strtab` | STRTAB | `0x1674` | `0x00ec` | 字符串表 |
|10 | `.shstrtab` | STRTAB | `0x1760` | `0x005b` | 节名字符串表 |

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

