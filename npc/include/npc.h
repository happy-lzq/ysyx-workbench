#ifndef __NPC_H__
#define __NPC_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

#define DIFFTEST_REG_SIZE (sizeof(uint32_t) * 33)  // 32 GPR + 1 PC
#define RESET_VECTOR 0x80000000 
#define PMEM_SIZE (128*1024*1024)

typedef uint32_t paddr_t;  

typedef struct {
    uint32_t gpr[32];
    uint32_t pc;
} NPC_state;

extern NPC_state npc_s,ref_s; 
extern uint8_t npc_pmem[PMEM_SIZE];
extern uint8_t* guest_to_host(paddr_t paddr);

#endif
