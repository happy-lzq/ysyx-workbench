#ifndef __DPI_EXPORT_H__
#define __DPI_EXPORT_H__

#ifdef __cplusplus
extern "C" {
#endif

// 统一内存读写（DPI-C，由 memory.cpp 实现）
int  dpi_mem_read(int addr, int is_load);
void dpi_mem_write(int addr, int data, int wmask);

// 预留
// int  dpi_timer_irq();

#ifdef __cplusplus
}
#endif

#endif
