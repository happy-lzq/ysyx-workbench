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

static void timer_tick() {
    const int timer_interval_cycles = 100000;
    if (cycle > 0 && cycle % timer_interval_cycles == 0) {
        word_t mip = npc_csr(top, CSR_MIP, 0, READ);
        npc_csr(top, CSR_MIP, mip | M_TIME_MASK, WRITE);
    }
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
    if (npc_sim_state.state != NPC_RUNNING) return;

    timer_tick();

    word_t trap_cause = isa_query_intr();
    if (trap_cause != INTR_EMPTY){
        word_t mepc = npc_pc(top,0,READ);                  // 当前周期已提交后的下一条指令
        word_t trap_pc = isa_raise_intr(trap_cause,mepc);  // 保存CSR，返回mtvec
        npc_pc(top,trap_pc,WRITE);                         // 重定向: 下周期跳转中断处理
    }
}

