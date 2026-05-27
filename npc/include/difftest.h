#ifndef __NPC_DIFFTEST_DEF_H__
#define __NPC_DIFFTEST_DEF_H__

#include <npc.h>

// ==================== difftest 方向 ====================
enum { DIFFTEST_TO_DUT, DIFFTEST_TO_REF };

// ==================== 参考模型函数指针 ====================
extern void (*ref_difftest_memcpy)(paddr_t, void *, size_t, bool);
extern void (*ref_difftest_regcpy)(void *, bool);
extern void (*ref_difftest_exec)(uint64_t);
extern void (*ref_difftest_raise_intr)(uint64_t);
extern void (*ref_difftest_init)(int);

// ==================== difftest API ====================
void init_difftest(const char *so_path, long img_size);
void difftest_step(Vcore_top *top,int idx);
void diff_so_path();
#endif