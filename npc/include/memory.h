#ifndef __NPC_MEMORY_H__
#define __NPC_MEMORY_H__

#include <npc.h>

#define PMEM_BASE 0x80000000U
#define PMEM_SIZE (128U * 1024 * 1024)   // 128MB
#define PMEM_END  (PMEM_BASE + PMEM_SIZE)

#ifdef __cplusplus
extern "C" {
#endif

// DPI-C: Verilog 侧调用的统一内存读写
int  dpi_mem_read(int addr);
void dpi_mem_write(int addr, int data, int wmask);

#ifdef __cplusplus
}

// C++ 侧辅助函数和全局变量
extern uint8_t npc_pmem[PMEM_SIZE];
extern long npc_img_size;

void pmem_init();
void pmem_load_bin(const char *path);

uint32_t pmem_read(uint32_t addr, int len);
void     pmem_write(uint32_t addr, int len, uint32_t data);

#endif

#endif
