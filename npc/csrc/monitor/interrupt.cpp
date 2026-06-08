#include <interrupt.h>


word_t isa_query_intr(){
    // 1. 全局中断使能未开，直接返回空
    if ((npc_csr(top, CSR_MSTATUS, 0, READ) & 0x8) == 0)
        return INTR_EMPTY;

    // 2. 定时器中断：mip.MTIP 和 mie.MTIE 同时为 1
    if ((npc_csr(top, CSR_MIP, 0, READ) & M_TIME_MASK) &&
        (npc_csr(top, CSR_MIE, 0, READ) & M_TIME_MASK)) {
        return 0x80000007;  // M-mode 时钟中断号
    }

    // 3. 外部中断：mip.MEIP 和 mie.MEIE 同时为 1
    if ((npc_csr(top, CSR_MIP, 0, READ) & M_MEIP_MASK) &&
        (npc_csr(top, CSR_MIE, 0, READ) & M_MEIP_MASK)) {
        return 0x8000000b;  // Machine External Interrupt
    }

    // 4. 其他中断可以继续添加...

    return INTR_EMPTY;
}

bool timer_sync_needed = false;  // difftest 同步标志：定时器刚触发但未进中断

static bool timer_tick() {
    const int timer_interval_cycles = 100000;
    if (cycle > 0 && cycle % timer_interval_cycles == 0) {
        word_t mip = npc_csr(top, CSR_MIP, 0, READ);
        npc_csr(top, CSR_MIP, mip | M_TIME_MASK, WRITE);
        return true;   // 定时器触发，需通知 difftest 同步
    }
    return false;
}

word_t isa_raise_intr(word_t trap_cause,vaddr_t npc_pc){
    switch (trap_cause){
        case IRQ_M_TIMER:{
            word_t mip_wdata = npc_csr(top, CSR_MIP, 0, READ) & ~M_TIME_MASK;   // clear MTIP
            npc_csr(top,CSR_MIP,mip_wdata,WRITE);
            break;
        }
        case IRQ_M_EXT: {
            word_t mip_wdata = npc_csr(top, CSR_MIP, 0, READ) & ~M_MEIP_MASK;   // clear MEIP
            npc_csr(top,CSR_MIP,mip_wdata,WRITE);
            break;
        }
        default:
          break;
        }
    npc_csr(top,CSR_MEPC,npc_pc,WRITE);
    npc_csr(top,CSR_MCAUSE,trap_cause,WRITE);
    word_t mstatus = npc_csr(top,CSR_MSTATUS,0,READ);
    mstatus = (mstatus & ~(1 << 7)) | (((mstatus >> 3) & 1) << 7); 
    mstatus &= ~(1 << 3) ;                                         
    mstatus = (mstatus & ~(3 << 11)) | (3 << 11);    
    npc_csr(top,CSR_MSTATUS,mstatus,WRITE);
    return npc_csr(top,CSR_MTVEC,0,READ);
}

void interrupt_check(){
    top->interrupt_valid = 0;
    top->interrupt_cause = 0;
    timer_sync_needed = false;

    if (npc_sim_state.state != NPC_RUNNING) return;

    bool timer_fired = timer_tick();

    word_t trap_cause = isa_query_intr();
    if (trap_cause != INTR_EMPTY){
        top->interrupt_valid = 1;
        top->interrupt_cause = trap_cause;
    }

    // 定时器刚触发但中断未使能：标记 difftest 需要同步（防止 mip 不匹配）
    if (timer_fired && !top->interrupt_valid) {
        timer_sync_needed = true;
    }
}
