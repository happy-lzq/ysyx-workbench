#include <common.h>
#include <isa.h>
#include <elf.h>
#include <stdio.h>

#ifndef FTRACE_COND
#define FTRACE_COND true
#endif
// 读取elf数据存好的fucn函数结构体
typedef struct {
    vaddr_t start;
    vaddr_t end;
    char name[128];
} FuncSymbol;
// 调用帧结构体 
typedef struct {
    vaddr_t ret_addr;       // 函数执行完后的返回地址
    const FuncSymbol *func; // 指向被调用的那个函数信息的指针
} CallFrame;

#define MAX_FUNC_SYMBOLS 1024
#define MAX_CALL_DEPTH 128

static FuncSymbol func_symbols[MAX_FUNC_SYMBOLS];
static int func_symbol_count = 0;

static CallFrame call_stack[MAX_CALL_DEPTH];   // 调用栈
static int call_depth = 0;                     // 调用深度
static FILE *ftrace_fp = NULL;                 // ftrace-log.txt 文件写入指针

// elf文件中选择出来的symtab目标条目的结构体数组中基于目标地址寻找对应的条目
static const FuncSymbol *find_func_by_addr(vaddr_t addr) {
    for (int i = 0; i < func_symbol_count; i++) {
        if (addr >= func_symbols[i].start && addr < func_symbols[i].end) {
            return &func_symbols[i];
        }
    }
    return NULL;
}

static void print_indent(FILE *fp, int depth) {
    if (fp == NULL) {
        return;
    }
    for (int i = 0; i < depth; i++) {
        fprintf(fp, "  ");
    }
}
// ftrace_log_file 文件生成路径
void init_ftrace_log(const char *ftrace_log_file) {
    Assert(ftrace_log_file != NULL, "ftrace log file path is null");
    ftrace_fp = fopen(ftrace_log_file, "w");
    Assert(ftrace_fp != NULL, "Can not open '%s'", ftrace_log_file);
    Log("Ftrace log is written to %s", ftrace_log_file);
}


void init_ftrace(const char *elf_file) {
    Assert(elf_file != NULL, "ftrace elf file is null");
    FILE *fp = fopen(elf_file, "rb");
    Assert(fp != NULL, "can not open ftrace elf '%s'", elf_file);

    Elf32_Ehdr ehdr;                    // C程序数据结构 定义在系统头文件 <elf.h> ehdr属于该数据结构名称
    int ret = fread(&ehdr, sizeof(ehdr), 1, fp);
    Assert(ret == 1, "read elf header failed: %s", elf_file);

// ====================================== 读取并校验 ELF Header =======================================
/*

ELF header 数据结构体
    typedef struct {
        unsigned char e_ident[EI_NIDENT]; 16字节身份信息 
        uint16_t      e_type;             ET_REL（可重定位）、ET_EXEC（可执行）、ET_DYN（共享库/PIE）、ET_CORE（core 文件）。
        uint16_t      e_machine;          目标架构：EM_X86_64   EM_RISCV    EM_MIPS
        uint32_t      e_version;          ELF格式版本 一般为 EV_CURRENT(1)
        ElfN_Addr     e_entry;            程序入口的虚拟地址。加载并准备完成后，开始执行的起始地址
        ElfN_Off      e_phoff;            程序头表（Program Header Table）在文件中的起始偏移(字节)
        ElfN_Off      e_shoff;            节头表  (Section Header Table) 在文件中的起始偏移(字节)
        uint32_t      e_flags;            架构相关标志位(不同ISA含义不同)
        uint16_t      e_ehsize;           ELF Header 自身大小(字节)
        uint16_t      e_phentsize;        单个程序头表项(ElfN_Phdr)大小
        uint16_t      e_phnum;            程序头表项数目
        uint16_t      e_shentsize;        单个节头表项(ElfN_Shdr大小
        uint16_t      e_shnum;            节头表项数目
        uint16_t      e_shstrndx;         "节头字名称子符串表"所在节头表数组中的索引  存在的目的就是对应节头表.shstrtab 
    } ElfN_Ehdr;

*/
    Assert(ehdr.e_ident[EI_MAG0] == ELFMAG0 &&
           ehdr.e_ident[EI_MAG1] == ELFMAG1 &&
           ehdr.e_ident[EI_MAG2] == ELFMAG2 &&
           ehdr.e_ident[EI_MAG3] == ELFMAG3,
           "invalid elf magic: %s", elf_file);
    Assert(ehdr.e_ident[EI_CLASS] == ELFCLASS32, "only support ELF32 now: %s", elf_file);
    Assert(ehdr.e_ident[EI_DATA] == ELFDATA2LSB, "only support little-endian ELF now: %s", elf_file);

//  ===================================== 读取 Section Header Table =================================
/*
读 Header 拿偏移量 -> fseek 找位置 -> malloc 开空间 -> fread 读数组
1、确认 shdr 单个节头表项大小与编译器结构体一致，避免错读。
2、动态分配结构体数组，建立节头表项数组shdrs[i]与单个节头表项之间索引映射   
	节头：
	  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
	  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
	  [ 1] .text             PROGBITS        80000000 001000 000250 00  AX  0   0  4
	  [ 2] .rodata           PROGBITS        80000250 001250 0000b8 00   A  0   0  4
	  [ 3] .data             PROGBITS        80000308 001308 000018 00  WA  0   0  4
	  [ 4] .sdata.str1       PROGBITS        80000320 001320 000006 00  WA  0   0  4
	  [ 5] .bss              NOBITS          80000328 001326 000014 00  WA  0   0  4
	  [ 6] .comment          PROGBITS        00000000 001326 00001a 01  MS  0   0  1
	  [ 7] .riscv.attributes RISCV_ATTRIBUTE 00000000 001340 000033 00      0   0  1
	  [ 8] .symtab           SYMTAB          00000000 001374 000300 10      9  23  4  存放各类变量、函数等等条目信息
	  [ 9] .strtab           STRTAB          00000000 001674 0000ec 00      0   0  1  存放symtab节条目名称字符串
	  [10] .shstrtab         STRTAB          00000000 001760 00005b 00      0   0  1  存放 “节”名字的字符串
    
      .shstrtab (Section Header String Table)
    角色： “文件夹的名字池”。
    作用： 专职存放各个节（Section）的名字（如 ".text", ".symtab", ".data"）。
    谁去找它： 所有的节头表项 (Elf32_Shdr)。通过 sh_name 索引进去拿名字。没有它，你就不知道哪个节是什么作用。
    
    .symtab (Symbol Table)
    角色： “人事档案表”。
    作用： 存放函数、全局变量等具体的二进制条目信息。它是一个规规矩矩的表格（由 Elf32_Sym 结构体排队组成），里面记录了某函数的起始地址（st_value）、大小（st_size）等。
    缺失的拼图： 但它里面没有存真正的英文字母名字，它的 st_name 只是个数字索引。这就需要它去找兄弟部门 .strtab。
    
    .strtab (String Table)
    角色： “人名池”。
    作用： 专职存放 .symtab 里各个条目的具体名字字符串（如 "main", "printf", "my_global_var"）。
    谁去找它： .symtab 里的每一个条目 (Elf32_Sym)。通过 st_name 索引进去，把这一串连着的字母翻译成人类看得懂的函数名。
    逻辑：从shstrtab中读取.strtab .symtab 
         从symtab中筛选符合要求的类别，如stt_func
         利用对应Sym 结构体中的sh_name 获取对应stt_func在strtab中的相对偏移地址获取真实字符串
         最后snprintf 写入func_symbols[]存放。

3、Elf32_Shdr shdrs[i]: 节头表数组每个成员属于下面的结构体数据结构如下：
    typedef struct {
        uint32_t sh_name;      // 节名字在.shstrtab中的索引
        uint32_t sh_type;      // 节类型
        uint32_t sh_flags;     // 节属性标志位
        uint32_t sh_addr;      // 节加载到内存后的虚拟地址
        uint32_t sh_offset;    // 节在文件中的偏移地址
        uint32_t sh_size;      // 节大小（字节）整个节大小
        uint32_t sh_link;      // 链接信息（根据节类型含义不同）
        uint32_t sh_info;      // 额外信息（根据节类型含义不同）
        uint32_t sh_addralign; // 内存对齐要求
        uint32_t sh_entsize;   // 针对有些节的数据类型是表格条目的形式，对比：symtab
    } Elf32_Shdr;

4、Elf32_Sym 数据结构如下：设计目的就是为了存放符号表(.symtab)中的符号条目信息，符号表中的每个符号都对应一个 Elf32_Sym 结构体实例。
    typedef struct {
        uint32_t st_name;       // 符号名字在.strtab中的索引 实际上只是在字符串表里的索引偏移
        uint32_t st_value;      // 符号的值（地址）  
        uint32_t st_size;       // 符号的大小（字节）
        unsigned char st_info;  // 符号类型和绑定属性   
            STT_NOTYPE      (0)：未知类型
            STT_OBJECT      (1)：数据对象（比如全局变量、数组）
            STT_FUNC        (2)：函数或可执行代码
            STT_SECTION     (3)：表示一个节区（Section）
            STT_FILE        (4)：表示源文件名称         
        unsigned char st_other; // 符号的可见性
        uint16_t st_shndx;      // 符号定义所在的节头表索引
    } Elf32_Sym; 
5、将文件指针 fp 移动到节头表在文件中的“绝对起始偏移量”
6、从节头表数据物理起始地址开始将整个节头表一次性加载进内存数组
7、通过 ret 返回值确认读取是否完整 返回数目与写入字节大小一致
*/
    Assert(ehdr.e_shentsize == sizeof(Elf32_Shdr),
                 "section header size mismatch: e_shentsize=%u", ehdr.e_shentsize);
    Elf32_Shdr *shdrs = malloc(ehdr.e_shnum * sizeof(Elf32_Shdr));
    Assert(shdrs != NULL, "malloc section headers failed");
    fseek(fp, ehdr.e_shoff, SEEK_SET);
    ret = fread(shdrs, sizeof(Elf32_Shdr), ehdr.e_shnum, fp);
    Assert(ret == ehdr.e_shnum, "read section headers failed");

// ===================================== 读取 .shstrtab（section 名字表） =============================
/*
1、确认shstrndx < e_shnum 保证 节头表字符串索引正确，不越界。否则找不到.shstrtab 
2、从已经构建的节头表结构体数组数据中获取对应 节头表名称字符串表 存入节头表数据结构变量 shstr 中备用。
3、为 shstrtab 分配 对应的内存大小数组 并检验分配成功？
4、将文件内存指针移动至对应shstrtab表在物理内存中的绝对起始地址
5、将文件中 节头表名称字符串表 数据完整读取到内存地址shstrtab数组中，并检验读取结果
*/
    Assert(ehdr.e_shstrndx < ehdr.e_shnum, "invalid e_shstrndx: %u", ehdr.e_shstrndx);
    Elf32_Shdr shstr = shdrs[ehdr.e_shstrndx];
    char *shstrtab = malloc(shstr.sh_size);
    Assert(shstrtab != NULL, "malloc .shstrtab failed");
    fseek(fp, shstr.sh_offset, SEEK_SET);
    ret = fread(shstrtab, shstr.sh_size, 1, fp);
    Assert(ret == 1, "read .shstrtab failed");

// ===================================== 遍历 section，定位 .symtab 与 .strtab ========================
/*
1、遍历节头表数组，获取每个节头表项的名字索引 sh_name 从而获取节名字
2、通过 strcmp 比较节名字与 .symtab、.strtab 定位到对应的节头表项，存入 symtab、strtab 变量备用
3、检验找到的 .symtab 节头表项的每个表项大小与编译器结构体 Elf32_Sym 大小一致，保证后续读取正确
4、如果没有找到 .symtab 或 .strtab 则报错
*/
    Elf32_Shdr *symtab = NULL;
    Elf32_Shdr *strtab = NULL;

    for (int i = 0; i < ehdr.e_shnum; i++) {
        const char *sec_name = shstrtab + shdrs[i].sh_name;         
        if (strcmp(sec_name, ".symtab") == 0) symtab = &shdrs[i];
        if (strcmp(sec_name, ".strtab") == 0) strtab = &shdrs[i];
    }

    Assert(symtab != NULL, "can not find .symtab in %s", elf_file);
    Assert(strtab != NULL, "can not find .strtab in %s", elf_file);
    Assert(symtab->sh_entsize == sizeof(Elf32_Sym),
                 ".symtab entry size mismatch: %u", symtab->sh_entsize);
    char *strtab_buf = malloc(strtab->sh_size);
    Assert(strtab_buf != NULL, "malloc .strtab failed");
    fseek(fp, strtab->sh_offset, SEEK_SET);
    ret = fread(strtab_buf, strtab->sh_size, 1, fp);
    Assert(ret == 1, "read .strtab failed");

// ===================================== 遍历 .symtab 并构建函数表（核心） ========================
/*
Symbol table '.symtab' contains 48 entries:     下面的每一条数据大小：Elf32_Shdr shdrs[8].sh_entsize. 
   Num:    Value  Size Type    Bind   Vis      Ndx Name
     0: 00000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 80000000     0 SECTION LOCAL  DEFAULT    1 .text
     2: 80000258     0 SECTION LOCAL  DEFAULT    2 .rodata
     3: 8000030c     0 SECTION LOCAL  DEFAULT    3 .data
     4: 80000324     0 SECTION LOCAL  DEFAULT    4 .sdata.str1
     5: 8000032c     0 SECTION LOCAL  DEFAULT    5 .bss
     6: 00000000     0 SECTION LOCAL  DEFAULT    6 .comment      
     ……
1、// 提取类别的宏，相当于丢弃高4位，只保留低4位 (0xf 就是二进制的 0000 1111)  提取低位类别
   
*/

    int sym_count = symtab->sh_size / symtab->sh_entsize;   // 计算符号表中符号的数量
    func_symbol_count = 0;
    fseek(fp, symtab->sh_offset, SEEK_SET);
    for (int i = 0; i < sym_count; i++) {
        Elf32_Sym sym;   // Elf32_Sym 每个表项的数据结构结构体
        ret = fread(&sym, sizeof(sym), 1, fp);   // fread 核心：fp自动向后推移nmemb * sizeof()字节大小的地址
        Assert(ret == 1, "read .symtab entry failed at %d", i);
// 卫语句过滤
    // #define ELF32_ST_TYPE(info) ((info) & 0xf)                  低四位表示类别
        if (ELF32_ST_TYPE(sym.st_info) != STT_FUNC) continue;   // 不是函数？踢出去！(continue)
        if (sym.st_shndx == SHN_UNDEF) continue;                // 没有实体？踢出去！(continue)
        if (sym.st_size == 0) continue;                         // 占用大小为0？踢出去！(continue)
        if (sym.st_name >= strtab->sh_size) continue;           // 名字越界了？踢出去！(continue)
        if (func_symbol_count >= MAX_FUNC_SYMBOLS) break;       // 数组装不下了？关门停止！(break)
// 提取过滤出来的有效函数的“名称、起始地址、结束地址”，并将其保存在全局数组 FuncSymbol 中 
        const char *name = strtab_buf + sym.st_name;
        FuncSymbol *sel_enrty = &func_symbols[func_symbol_count++];  // 相同结构体指针指向该有效symtab条目
        sel_enrty->start = (vaddr_t)sym.st_value;                    // 有效起始地址
        sel_enrty->end = (vaddr_t)(sym.st_value + sym.st_size);      // 有效起始地址+有效大小
        snprintf(sel_enrty->name,sizeof(sel_enrty->name),"%s",name);
    }

// ==================================== 资源回收与结果日志       ==================================
    free(strtab_buf);   // 该结构体数组内容用于存放读取elf文件中的strtab数据内容，该部分存放节头表名称字符串内容
    free(shstrtab);     // 存放 ELF 文件中所有“节（Section）”的名字字符串，供节头表（Section Headers）去查阅
    free(shdrs);        // 节 结构体数组，用于存放读取lf文件中所有的节数据
    fclose(fp);         // 文件指针

    Log("ftrace elf: %s", elf_file);
    Log("ftrace loaded %d function symbols", func_symbol_count);
}

// ==================================== ftrace cell/ret log ====================================
void ftrace_call(vaddr_t pc, vaddr_t tar_addr, vaddr_t ret_addr) {
    if (ftrace_fp == NULL || !FTRACE_COND) {
        return;
    }

    const FuncSymbol *callee = find_func_by_addr(tar_addr);

    fprintf(ftrace_fp, FMT_WORD " : ", pc);          // FMT_WORD  32、64位的输出模式选择
    print_indent(ftrace_fp, call_depth);
    if (callee != NULL) {
        fprintf(ftrace_fp, "call [%s@" FMT_WORD "]\n", callee->name, tar_addr);
    } else {
        fprintf(ftrace_fp, "call [unknown@" FMT_WORD "]\n", tar_addr);
    }
    fflush(ftrace_fp);

    if (call_depth < MAX_CALL_DEPTH) {
        call_stack[call_depth].ret_addr = ret_addr;
        call_stack[call_depth].func = callee;
        call_depth++;
    }
}

void ftrace_ret(vaddr_t pc, vaddr_t tar_addr) {
    if (ftrace_fp == NULL) {
        return;
    }
    if (!FTRACE_COND) {
        return;
    }
    if (call_depth > 0) {
        call_depth--;
    }

    const FuncSymbol *func = (call_depth >= 0 && call_depth < MAX_CALL_DEPTH)
        ? call_stack[call_depth].func : NULL;

    fprintf(ftrace_fp, FMT_WORD " : ", pc);
    print_indent(ftrace_fp, call_depth);
    if (func != NULL) {
        fprintf(ftrace_fp, "ret  [%s] -> " FMT_WORD "\n", func->name, tar_addr);
    } else {
        fprintf(ftrace_fp, "ret  [unknown] -> " FMT_WORD "\n", tar_addr);
    }
    fflush(ftrace_fp);
}