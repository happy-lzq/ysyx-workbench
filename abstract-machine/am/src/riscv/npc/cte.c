#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

#define MSTATUS 0x1888

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    ev.cause = c->mcause;
    switch (c->mcause) {
      case 0xb:   // M-mode environment call
        if (c->gpr[17] == (uintptr_t)-1) {   // a7 = -1
          ev.event = EVENT_YIELD;
        } else {
          ev.event = EVENT_SYSCALL;
          ev.ref = c->gpr[17];
        }
        c->mepc += 4;   // mepc 里保存的是触发 trap 的那条 ecall 指令地址，返回后要跳过到下一条指令。
        break;

      case 0x80000007:  
        ev.event = EVENT_IRQ_TIMER;
        break;

      case 0x8000000b:  
        ev.event = EVENT_IRQ_IODEV;
        break;

      default:
        ev.event = EVENT_ERROR;
        break;
    }

    c = user_handler(ev, c);  // 核心：返回的Context *是切换进程/当前进程！！！！！！
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));
  asm volatile("csrs mie, %0" : : "r"(1<<7));   // 打开mie.MTIE中断使能
  // register event handler
  user_handler = handler;
  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  uintptr_t sp = (uintptr_t)kstack.end;
  sp = (sp - sizeof(Context)) & ~0x7;

  Context *ctx = (Context *)sp;
  *ctx = (Context){0};
  ctx->mepc = (uintptr_t)entry;
  ctx->mstatus = MSTATUS;
  ctx->gpr[2] = (uintptr_t)kstack.end;
  ctx->gpr[10] = (uintptr_t)arg;
  return ctx;
}

void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, -1; ecall");
#endif
}

// “从 CPU 当前状态里，读出 machine interrupt 总开关状态”
bool ienabled() {
  uint32_t mstatus_val;
  asm volatile("csrr %0, mstatus" : "=r"(mstatus_val));
  bool mie = (mstatus_val & (1<<3)) != 0; 
  return mie;
}



// “修改 CPU 当前状态里的 machine interrupt 总开关” mstatus 中的mie
void iset(bool enable) {
  if (enable){
    asm volatile("csrs mstatus,%0" : : "r"(1 << 3));
  }else {
    asm volatile("csrc mstatus,%0" : : "r"(1 << 3)); 
  }
}
