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
#include <cpu/cpu.h>
#include <difftest-def.h>
#include <memory/paddr.h>

__EXPORT void difftest_memcpy(paddr_t addr, void *buf, size_t n, bool direction) {
  if (direction == DIFFTEST_TO_REF){
    memcpy(guest_to_host(addr), buf, n);
  } else{
    memcpy(buf, guest_to_host(addr), n);
  }
}

__EXPORT void difftest_regcpy(void *dut, bool direction) {
  // printf("[REF regcpy] direction=%d, cpu.pc=0x%08x\n", direction, cpu.pc);
  if (direction == DIFFTEST_TO_REF){
    memcpy(&cpu,dut,DIFFTEST_REG_SIZE);
  } else{
    memcpy(dut,&cpu,DIFFTEST_REG_SIZE);
  }
}

__EXPORT void difftest_exec(uint64_t n) {
  extern bool difftest_exec_mode;
  difftest_exec_mode = true;
  cpu_exec(n);
  difftest_exec_mode = false;
}

__EXPORT void difftest_raise_intr(word_t NO) {
  cpu.pc = isa_raise_intr(NO, cpu.pc);
}

__EXPORT void difftest_set_intr_pending(word_t NO) {
  switch (NO) {
    case 0x80000007:
      cpu.csr[CSR_IDX_mip] |= (1 << 7);
      break;
    case 0x8000000b:
      cpu.csr[CSR_IDX_mip] |= (1 << 11);
      break;
    default:
      panic("Unsupported interrupt cause: " FMT_WORD, NO);
  }
}

__EXPORT void difftest_init(int port) {
  void init_mem();
  init_mem();   // 初始化分配nemu 物理内存
  init_isa();   // 初始化cpu结构体--核心GPR+PC
  nemu_state.state = NEMU_RUNNING; 
}
