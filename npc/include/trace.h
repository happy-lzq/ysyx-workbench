#ifndef __NPC_TRACE_H__
#define __NPC_TRACE_H__

#include <npc.h>

// ======================== itrace =======================
extern const char *elf_file;
extern FILE *itrace_fp;
void init_disasm();
void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
void itrace_log(uint32_t pc, uint32_t inst_word);


#endif
