#include <cstdio>
#include <sys/time.h>
#include <dpi_export.h>
extern "C" {

// ========== MMIO 读路由 ==========
int dpi_mmio_read(int addr) {
    switch (addr) {
        case 0x10000000: return 0;      // UART 读（只写设备，返回 0）
        // case 0xa0000048: return ...; // RTC（后续扩展）
        default:         return 0;
    }
}

// ========== 预留：定时器中断 ==========
// int dpi_timer_irq() { ... }

}