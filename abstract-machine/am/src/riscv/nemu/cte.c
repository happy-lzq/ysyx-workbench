#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>


// 全局静态结构体函数指针
static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);


// cte_init() 的核心职责就是注册事件处理函数
bool cte_init(Context*(*handler)(Event, Context*)) {
  
  // 把 __am_asm_trap 的地址写入 mtvec
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  user_handler = handler;
  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}

void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, -1; ecall");
#endif
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
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
5、
*/