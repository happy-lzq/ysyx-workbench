#ifndef ARCH_H__
#define ARCH_H__

#ifdef __riscv_e
#define NR_REGS 16
#else
#define NR_REGS 32
#endif

// 上下文异常、外部中断发生瞬间cpu的快照结构体
// trap进入内核处理时，栈上临时生成的现场副本
/*
1、对应trap.S的核心功能。
  1、保存上下文
    把所有通用寄存器（x1, x3..x31）、mcause、mstatus、mepc 压入栈中，形成 struct Context 的完整镜像
  2、调用 C 处理函数
    把栈上的 Context 指针传给 __am_irq_handle.函数根据事件类型（自陷/中断）进行处理，并可能返回一个新的 Context（用于上下文切换）
  3、恢复上下文并返回
    根据 __am_irq_handle 返回的 Context 指针，把栈上的值恢复到对应的寄存器和 CSR 中，然后执行 mret，使 CPU 跳回 mepc 指向的地址继续执行
  4、
*/
struct Context {
  uintptr_t gpr[NR_REGS],mcause,mstatus,mepc;
  void *pdir;
};

#ifdef __riscv_e
#define GPR1 gpr[15] // a5
#else
#define GPR1 gpr[17] // a7
#endif

#define GPR2 gpr[0]
#define GPR3 gpr[0]
#define GPR4 gpr[0]
#define GPRx gpr[0]

#endif
