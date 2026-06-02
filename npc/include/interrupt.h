#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <npc.h>


#define IRQ_M_TIMER  0x80000007
#define IRQ_M_EXT    0x8000000b
#define M_TIME_MASK (1 << 7)
#define M_MEIP_MASK (1 << 11)

word_t isa_query_intr();
word_t isa_raise_intr(word_t trap_cause,vaddr_t npc_pc);
void interrupt_check();
#endif