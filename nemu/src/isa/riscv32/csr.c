#include <isa.h>
#include "local-include/csr.h"

typedef struct {
  uint32_t csr_num;   // CSR 编号（如 0x300）
  int      csr_idx;   // 枚举值
} CSR_MAP;

// 映射表（静态初始化）
static const CSR_MAP csr_map[] = {
  { 0x300, CSR_IDX_mstatus },
  { 0x344, CSR_IDX_mip },
  { 0x304, CSR_IDX_mie },
  { 0x342, CSR_IDX_mcause },
  { 0x305, CSR_IDX_mtvec },
  { 0x343, CSR_IDX_mtval },
  { 0x341, CSR_IDX_mepc },
  { 0x340, CSR_IDX_mscratch },
};

// 查找函数：根据 csr_num 返回 csr_idx，失败返回 -1
word_t* csr_idx_addr(uint32_t csr_num) {
  int len = sizeof(csr_map) / sizeof(csr_map[0]);
  for (int i = 0; i < len; i++) {
    if (csr_map[i].csr_num == csr_num) {
      return &cpu.csr[csr_map[i].csr_idx];
    }
  }
  // 未找到，处理错误
  panic("Invalid CSR number: 0x%x", csr_num);
  return NULL;
}