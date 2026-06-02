#ifndef __DPI_EXPORT_H__
#define __DPI_EXPORT_H__
#include <npc.h>

#ifdef __cplusplus
extern "C" {
#endif

int  dpi_mmio_read(int addr);
// int  dpi_timer_irq();
// void dpi_serial_putc(int ch);

#ifdef __cplusplus
}
#endif

#endif