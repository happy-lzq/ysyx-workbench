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
  { 0xB00, CSR_IDX_mcyclel },
  { 0xB80, CSR_IDX_mcycleh}
};

// 查找函数：根据 csr_num 返回 csr_idx，失败返回 -1
word_t* csr_idx_addr(uint32_t csr_num) {
  int len = sizeof(csr_map) / sizeof(csr_map[0]);
  for (int i = 0; i < len; i++) {
    if (csr_map[i].csr_num == csr_num) {
      return &cpu.csr[csr_map[i].csr_idx];
    }
  }
  // 未找到，返回NULL进入地址异常中断处理
  // panic("Invalid CSR number: 0x%x", csr_num);
  return NULL;
}

// 当前 M-only 软件写合同；硬件 trap 现场更新由 system/intr.c 处理。
// 字段约束不等同于 CSR 地址非法，不能因此产生新的非法指令异常。
void csr_write(uint32_t csr_num, word_t value) {
  word_t *target = csr_idx_addr(csr_num);
  assert(target != NULL);  // 调用者先完成地址合法性检查
  switch (csr_num) {
    case 0x300:  // 仅 MIE/MPIE 可写，MPP 固定为 M；无 S/U/F/V 等字段
      value = (value & 0x88u) | 0x1800u;
      break;
    case 0x304:  // MEIE/MTIE
      value &= 0x880u;
      break;
    case 0x305:  // Direct-only mtvec
    case 0x341:  // 固定 IALIGN=32 mepc
      value &= ~3u;
      break;
    case 0x344:  // MTIP/MEIP 为设备状态，CSR 软件写保持；其他字段未实现
      value = *target & 0x880u;
      break;
    default:
      // mscratch/mcause/mtval 和计数器被寻址半部均保留完整 32 位。
      // REF 无 DUT 周期概念，此处不把一条解释执行指令当成一个核心时钟。
      break;
  }
  *target = value;
}
