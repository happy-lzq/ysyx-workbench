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
extern void (*ref_difftest_set_intr_pending)(uint64_t);
extern void (*ref_difftest_init)(int);
// ==================== difftest API ====================
void init_difftest(const char *so_path, long img_size);
void difftest_set_intr_pending(uint64_t cause);
void difftest_step(
    Vcore_top *top, 
    int idx, 
    uint32_t npc_exec_pc,
    bool wb_valid, 
    bool irq_taken, 
    uint32_t interrupt_cause,
    bool wb_is_mmio,
    uint32_t wb_inst
);
void difftest_init();
#ifdef CONFIG_DIFF_LOG_ENABLE
void init_diff_log(const char *img_path);
void diff_log_write(NPC_state *npc, NPC_state *ref, int cycle);
#endif
void npc_state_data(Vcore_top* top);
void difftest_compare();
#endif
