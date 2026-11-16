#ifndef __RISCV32_CSR_H__
#define __RISCV32_CSR_H__

#include <common.h>
#include <isa.h>

word_t *csr_idx_addr(uint32_t csr_num);
void csr_write(uint32_t csr_num, word_t value);

#endif
