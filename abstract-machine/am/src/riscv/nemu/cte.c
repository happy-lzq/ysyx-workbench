#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

#define MSTATUS 0x1888

// 全局静态上下文结构体函数指针
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


// cte_init() 的核心职责就是注册事件处理函数
bool cte_init(Context*(*handler)(Event, Context*)) {
  
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));        // 把 __am_asm_trap 的地址写入 mtvec
  asm volatile("csrs mie,%0" : : "r"(1<<7));                    // time 使能机器定时器中断
  user_handler = handler;
  return true;
}
// 三个参数：kstack是栈的范围，entry是内核线程的入口, arg则是内核线程的参数 构造初始化Context
Context* kcontext(Area kstack, void (*entry)(void *), void *arg) {
  Context* ctx = kstack.end - sizeof(Context);     
  *ctx = (Context){0};
  ctx->mepc = (uintptr_t)entry;
  ctx->mstatus = MSTATUS;                       // mstatus设置，MMP MPIE MIE 置位处理
  ctx->gpr[2]  = (uintptr_t)kstack.end;         // 每一个pcb模块的栈指针指向栈顶sp
  ctx->gpr[10] = (uintptr_t)arg;                // 函数参数接受寄存器从a0(gpr[10])开始
  return ctx;
}
// 0x110010001000
void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, -1; ecall");
// addi a7, x0, -1
// ecall
#endif
}


// “从 CPU 当前状态里，读出 machine interrupt 总开关状态”
bool ienabled() {
  uint32_t mstatus_val;
  asm volatile("csrr %0, mstatus" : "=r"(mstatus_val));
  bool mie = (mstatus_val & (1<<3)) != 0; 
  return mie;
}



// “修改 CPU 当前状态里的 machine interrupt 总开关”
void iset(bool enable) {
  if (enable){
    asm volatile("csrs mstatus,%0" : : "r"(1 << 3));
  }else {
    asm volatile("csrc mstatus,%0" : : "r"(1 << 3)); 
  }
}


/*
上下文异常处理逻辑链：以ecall 自陷为例
  ecall  从当前特权级发起一个环境调用异常，cpu同步跳转到机械态进行内核处理
1、触发阶段：用户程序执行到该指令，则同步申请进入内核处理请求
2、硬件自动同步响应：
  1、R[mepc] = pc  当前返回地址pc保存到mepc寄存器中
  2、R[mcause] = 异常编号
  3、PC = R[mtvec] 从mtvec寄存器中读取异常入口地址，下一条指令从该pc地址执行
  4、status 相关位被硬件修改（保存旧特权级、进入机器模式、可能关闭中断）。
3、mtvec寄存器的来源与异常入口地址的决定逻辑
  1、R[mtvec] = 0(默认)，异常入口地址是由软件层根据操作系统写入——> csrw 指令
  2、故而出现异常处理入口函数：__am_asm_trap()  汇编代码函数
  3、异常入口地址确定过程：
    1、汇编函数trap.S 决定逻辑，
    2、链接器构建elf文件，为该函数分配绝对地址——>addr
    3、cte_init()初始化，基于指令：csrw mtvec, addr 。成功将汇编处理函数地址写入 R[metvec]
4、汇编函数的核心作用 trap.S
  1、分配栈空间，手动把所有通用寄存器按顺序保存到栈上
  2、把 mepc mcause mstatus 读取出来进行保存
  3、最终在栈上构造完整的 struct Context 
  4、将指向这个Context结构体的指针作为参数，准备调用C程序事件处理函数
5、如何正确读写CSR控制寄存器  CSR指令家族
  读：C程序需要一个对应的变量来存储寄存器的值
    uintptr_t mepc_val;
    asm volatile("csrr %0,mepc" : "=r"(mepc_val));
    汇编生成：csrr rd,mepc
  写入mtevc
    asm volatile("csrw mtvec, %0" : : "w"(__am_asm_trap))
  写：pc = R[mepc]
    uintptr_t new_epc = ...;
    asm volatile("csrw mepc, %0" : : "r"(new_epc));
  操作需求	          内联汇编例子
  读 CSR 到变量	      asm volatile("csrr %0, mepc" : "=r"(val));
  写变量值到 CSR	    asm volatile("csrw mtvec, %0" : : "r"(addr));
  设置 CSR 中某位	    asm volatile("csrs mstatus, %0" : : "r"(8));
  清除 CSR 中某位	    asm volatile("csrc mstatus, %0" : : "r"(8));
  同时读和写	        asm volatile("csrrw %0, mstatus, %1" : "=r"(old) : "r"(new));
6、原子级操作指令
  csrs : set bit in csr     对csr寄存器读后执行置位操作
  csrc : clean bit in csr   对csr寄存器读后执行清零操作
  csrw : write in csr       对csr寄存器读后执行写寄存器操作
  csrr : read in csr        只读操作
*/