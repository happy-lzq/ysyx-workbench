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

#ifndef __ISA_RISCV_H__
#define __ISA_RISCV_H__

#include <common.h>
enum {
  CSR_IDX_mstatus = 0,
  CSR_IDX_mip,
  CSR_IDX_mie,
  CSR_IDX_mcause,
  CSR_IDX_mtvec,
  CSR_IDX_mtval,
  CSR_IDX_mepc,
  CSR_IDX_mscratch,
  CSR_NUM_MAX   // 用于表示数组长度
};

typedef struct {
  word_t gpr[MUXDEF(CONFIG_RVE, 16, 32)];
  word_t csr[CSR_NUM_MAX];
  vaddr_t pc;
} MUXDEF(CONFIG_RV64, riscv64_CPU_state, riscv32_CPU_state);

/*
CPU_state  根据系统选择的ISA构建对应的cpu_state 结构体
typedef struct {
  uint32_t gpr[32];
  uint32_t pc;
} riscv32_CPU_state;

*/
// decode
typedef struct {
  uint32_t inst;
} MUXDEF(CONFIG_RV64, riscv64_ISADecodeInfo, riscv32_ISADecodeInfo);

#define isa_mmu_check(vaddr, len, type) (MMU_DIRECT)

#endif
