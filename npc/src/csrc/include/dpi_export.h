#ifndef __DPI_EXPORT_H__
#define __DPI_EXPORT_H__

#ifdef __cplusplus
extern "C" {
#endif

int dpi_mem_access(unsigned long long addr, int kind, int size,
                   int device_attr, int data, int mask, int *rdata);
void dpi_mem_fault(int pc, int addr, int kind);

// 统一内存读写（DPI-C，由 memory.cpp 实现）
int  dpi_mem_read(int addr, int is_load);
void dpi_mem_write(int addr, int data, int wmask);

// wb阶段的写回事件
void dpi_wb_event(
    int valid, 
    int pc, 
    int inst, 
    int irq_take, 
    int interrupt_cause,
    int wb_is_mmio
);

void dpi_mmio_read_event(
    int valid,
    int pc,
    int addr,
    int data
);

// mem 阶段mmio 设备访问检查
int dpi_is_mmio(int addr);

#ifdef __cplusplus
}
#endif

#endif
