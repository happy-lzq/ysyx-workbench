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

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  cpu.csr[CSR_IDX_mepc]   = epc;
  cpu.csr[CSR_IDX_mcause] = NO;
  word_t mstatus = cpu.csr[CSR_IDX_mstatus];
  mstatus = (mstatus & ~(1 << 7)) | (((mstatus >> 3) & 1) << 7);  // MPIE = MIE 保存进入trap前的MIE
  mstatus &= ~(1 << 3) ;                                          // MIE=0 关闭中断，防止被打扰
  mstatus = (mstatus & ~(3 << 11)) | (3 << 11);                     // 修改当前特权级=M(3)
  cpu.csr[CSR_IDX_mstatus] = mstatus;                             // 晚上mstatus寄存器 
  return cpu.csr[CSR_IDX_mtvec];                                  // 返回异常处理入口地址
}

word_t isa_query_intr() {
  
  return INTR_EMPTY;
}
