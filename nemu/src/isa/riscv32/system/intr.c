/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#define IRQ_M_TIMER  0x80000007
#define IRQ_M_EXT    0x8000000b
#define M_TIME_MASK (1 << 7)
#define M_MEIP_MASK (1 << 11)

// 同步异常传 s->pc，外部中断传 s.dnpc 
word_t isa_raise_intr(word_t NO, vaddr_t epc) {

  switch (NO) {
    case IRQ_M_TIMER:
      cpu.csr[CSR_IDX_mip] &= ~(1 << 7);   // clear MTIP
      break;
    case IRQ_M_EXT:
      cpu.csr[CSR_IDX_mip] &= ~(1 << 11);  // clear MEIP
      break;
    default:
      break;
  }

  cpu.csr[CSR_IDX_mepc]   = epc;
  cpu.csr[CSR_IDX_mcause] = NO;
  word_t mstatus = cpu.csr[CSR_IDX_mstatus];
  mstatus = (mstatus & ~(1 << 7)) | (((mstatus >> 3) & 1) << 7);  // MPIE = MIE 保存进入trap前的MIE
  mstatus &= ~(1 << 3) ;                                          // MIE=0 关闭中断，防止被打扰
  mstatus = (mstatus & ~(3 << 11)) | (3 << 11);                   // 修改当前特权级=M(3)
  cpu.csr[CSR_IDX_mstatus] = mstatus;                             // 完善mstatus寄存器 
  return cpu.csr[CSR_IDX_mtvec];       // 返回异常处理入口地址  ；cte_init()提前将trap.S写入mtvec

}

word_t isa_query_intr() {
    // 1. 全局中断使能未开，直接返回空
    if ((cpu.csr[CSR_IDX_mstatus] & 0x8) == 0)
        return INTR_EMPTY;

    // 2. 定时器中断：mip.MTIP 和 mie.MTIE 同时为 1
    if ((cpu.csr[CSR_IDX_mip] & (M_TIME_MASK)) &&               // mip寄存器中 MTIP(bit=7) 中断挂起位
        (cpu.csr[CSR_IDX_mie] & (M_TIME_MASK))) {               // mie寄存器中 MTIE(bit=7) 中断是能位
         cpu.csr[CSR_IDX_mip] &= ~M_TIME_MASK;
        return 0x80000007;                                      // M-mode 时钟中断号 7  最高位 = 1 表示中断，低 30 位 = 7 表示定时器
    }

    // 3. 外部中断：mip.MEIP 和 mie.MEIE 同时为 1                    
    if ((cpu.csr[CSR_IDX_mip] & (M_MEIP_MASK)) &&                     
        (cpu.csr[CSR_IDX_mie] & (M_MEIP_MASK))) {                     
        return 0x8000000b;  // Machine External Interrupt
    }

    // 4. 其他中断可以继续添加...

    return INTR_EMPTY;
}
