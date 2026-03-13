#include <common.h>
#include <isa.h>
#include <elf.h>
#include <stdio.h>

typedef struct {
    vaddr_t start;
    vaddr_t end;
    char name[128];
} FuncSymbol;

typedef struct {
    vaddr_t ret_addr;
    const FuncSymbol *func;
} CallFrame;

#define MAX_FUNC_SYMBOLS 1024
#define MAX_CALL_DEPTH 128

static FuncSymbol func_symbols[MAX_FUNC_SYMBOLS];
static int func_symbol_count = 0;

static CallFrame call_stack[MAX_CALL_DEPTH];
static int call_depth = 0;
static FILE *ftrace_fp = NULL;

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

    Elf32_Ehdr ehdr;
    int ret = fread(&ehdr, sizeof(ehdr), 1, fp);
    Assert(ret == 1, "read elf header failed: %s", elf_file);

    Assert(ehdr.e_ident[EI_MAG0] == ELFMAG0 &&
                 ehdr.e_ident[EI_MAG1] == ELFMAG1 &&
                 ehdr.e_ident[EI_MAG2] == ELFMAG2 &&
                 ehdr.e_ident[EI_MAG3] == ELFMAG3,
                 "invalid elf magic: %s", elf_file);
    Assert(ehdr.e_ident[EI_CLASS] == ELFCLASS32, "only support ELF32 now: %s", elf_file);
    Assert(ehdr.e_ident[EI_DATA] == ELFDATA2LSB, "only support little-endian ELF now: %s", elf_file);

    Assert(ehdr.e_shentsize == sizeof(Elf32_Shdr),
                 "section header size mismatch: e_shentsize=%u", ehdr.e_shentsize);

    Elf32_Shdr *shdrs = malloc(ehdr.e_shnum * sizeof(Elf32_Shdr));
    Assert(shdrs != NULL, "malloc section headers failed");

    fseek(fp, ehdr.e_shoff, SEEK_SET);
    ret = fread(shdrs, sizeof(Elf32_Shdr), ehdr.e_shnum, fp);
    Assert(ret == ehdr.e_shnum, "read section headers failed");

    Assert(ehdr.e_shstrndx < ehdr.e_shnum, "invalid e_shstrndx: %u", ehdr.e_shstrndx);
    Elf32_Shdr shstr = shdrs[ehdr.e_shstrndx];

    char *shstrtab = malloc(shstr.sh_size);
    Assert(shstrtab != NULL, "malloc .shstrtab failed");
    fseek(fp, shstr.sh_offset, SEEK_SET);
    ret = fread(shstrtab, shstr.sh_size, 1, fp);
    Assert(ret == 1, "read .shstrtab failed");

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

    int sym_count = symtab->sh_size / symtab->sh_entsize;
    func_symbol_count = 0;

    fseek(fp, symtab->sh_offset, SEEK_SET);
    for (int i = 0; i < sym_count; i++) {
        Elf32_Sym sym;
        ret = fread(&sym, sizeof(sym), 1, fp);
        Assert(ret == 1, "read .symtab entry failed at %d", i);

        if (ELF32_ST_TYPE(sym.st_info) != STT_FUNC) continue;
        if (sym.st_shndx == SHN_UNDEF) continue;
        if (sym.st_size == 0) continue;
        if (sym.st_name >= strtab->sh_size) continue;
        if (func_symbol_count >= MAX_FUNC_SYMBOLS) break;

        const char *name = strtab_buf + sym.st_name;
        FuncSymbol *slot = &func_symbols[func_symbol_count++];
        slot->start = (vaddr_t)sym.st_value;
        slot->end = (vaddr_t)(sym.st_value + sym.st_size);
        strncpy(slot->name, name, sizeof(slot->name) - 1);
        slot->name[sizeof(slot->name) - 1] = '\0';
    }

    free(strtab_buf);
    free(shstrtab);
    free(shdrs);
    fclose(fp);

    Log("ftrace elf: %s", elf_file);
    Log("ftrace loaded %d function symbols", func_symbol_count);
}

void ftrace_call(vaddr_t pc, vaddr_t target, vaddr_t ret_addr) {
    if (ftrace_fp == NULL) {
        return;
    }
    const FuncSymbol *callee = find_func_by_addr(target);

    print_indent(ftrace_fp, call_depth);
    if (callee != NULL) {
        fprintf(ftrace_fp, FMT_WORD ": call [%s@" FMT_WORD "]\n", pc, callee->name, target);
    } else {
        fprintf(ftrace_fp, FMT_WORD ": call [unknown@" FMT_WORD "]\n", pc, target);
    }
    fflush(ftrace_fp);

    if (call_depth < MAX_CALL_DEPTH) {
        call_stack[call_depth].ret_addr = ret_addr;
        call_stack[call_depth].func = callee;
        call_depth++;
    }
}

void ftrace_ret(vaddr_t pc, vaddr_t target) {
    if (ftrace_fp == NULL) {
        return;
    }
    if (call_depth > 0) {
        call_depth--;
    }

    const FuncSymbol *func = (call_depth >= 0 && call_depth < MAX_CALL_DEPTH)
        ? call_stack[call_depth].func : NULL;

    print_indent(ftrace_fp, call_depth);
    if (func != NULL) {
        fprintf(ftrace_fp, FMT_WORD ": ret  [%s] -> " FMT_WORD "\n", pc, func->name, target);
    } else {
        fprintf(ftrace_fp, FMT_WORD ": ret  [unknown] -> " FMT_WORD "\n", pc, target);
    }
    fflush(ftrace_fp);
}