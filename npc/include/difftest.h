#ifndef __NPC_DIFFTEST_DEF_H__
#define __NPC_DIFFTEST_DEF_H__

#include <npc.h>
#include <dlfcn.h>
#include <verilated.h>
#include <Vcore_top.h>
#include <Vcore_top___024root.h>

enum { DIFFTEST_TO_DUT, DIFFTEST_TO_REF };
enum { READ, WRITE };
extern Vcore_top* top;

static inline uint32_t npc_gpr(Vcore_top *top, int idx, uint32_t val, int r_w) {
    if (r_w == WRITE){
        return top->rootp->core_top__DOT__u_regfile__DOT__rf[idx] = val;
    } else
        return top->rootp->core_top__DOT__u_regfile__DOT__rf[idx];
}

static inline uint32_t npc_pc(Vcore_top *top, uint32_t val, int r_w){
    if ( r_w == WRITE){
        return top->rootp->core_top__DOT__pc = val;
    } else
        return top->rootp->core_top__DOT__pc;
}

static inline uint32_t npc_imem(Vcore_top *top, int idx, uint32_t val, int r_w) {
    if (r_w == WRITE){
        return top->rootp->core_top__DOT__u_if_stage__DOT__imem[idx] = val;
    } else
       return top->rootp->core_top__DOT__u_if_stage__DOT__imem[idx];
}

extern void (*ref_difftest_memcpy)(paddr_t, void*, size_t, bool);
extern void (*ref_difftest_regcpy)(void*, bool);
extern void (*ref_difftest_exec)(uint64_t);
extern void (*ref_difftest_raise_intr)(uint64_t);
extern void (*ref_difftest_init)(int);

void init_difftest(const char* so_path,long img_size);
void difftest_step(Vcore_top* top) ;

#endif