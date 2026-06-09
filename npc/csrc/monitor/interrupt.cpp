#include <interrupt.h>

bool difftest_sync_needed = false;  // difftest 统一同步标志


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
// ============ 宿主闹钟：信号处理器只设标志 ============
static volatile sig_atomic_t alarm_fired = 0;

static void alarm_handler(int signum) {
    alarm_fired = 1;  // 仅此一行！不做任何其他事
}

void init_timer_alarm() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = alarm_handler;
    sigaction(SIGVTALRM, &sa, NULL);

    struct itimerval it = {};
    it.it_value.tv_sec     = 0;
    it.it_value.tv_usec    = 1000000 / 60;   // TIMER_HZ=60, ≈16.7ms
    it.it_interval         = it.it_value;    // 周期性触发
    setitimer(ITIMER_VIRTUAL, &it, NULL);
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
    difftest_sync_needed  = false;

    if (npc_sim_state.state != NPC_RUNNING) return;
    #ifdef CONFIG_HAS_TIMER
    if (alarm_fired) {
        alarm_fired = 0;
        word_t mip = npc_csr(top, CSR_MIP, 0, READ);
        npc_csr(top, CSR_MIP, mip | M_TIME_MASK, WRITE);
        difftest_sync_needed = true;   // ① 定时器置 MTIP → NEMU 无法复现 → 需同步
    }
    #endif
    word_t trap_cause = isa_query_intr();
    if (trap_cause != INTR_EMPTY){
        top->interrupt_valid = 1;
        top->interrupt_cause = trap_cause;
        difftest_sync_needed = true;   // ② 中断响应 → NEMU 无法复现 → 需同步
    }
}
