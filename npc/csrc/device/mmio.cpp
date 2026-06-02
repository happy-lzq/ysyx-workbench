
#include <dpi_export.h>
#include <device.h>


static void mmio_write_handler(uint32_t addr, uint32_t wdata, uint8_t wmask) {
    // ===== NPC 外设地址路由表 =====
    switch (addr) {
        case 0x10000000:   // UART 串口
            if (wmask & 0x1)
                npc_serial_putc(wdata & 0xFF);
            break;

        // case 0xa0000048:   // RTC 低32位 (后续)
        // case 0xa000004c:   // RTC 高32位 (后续)
        // case 0x20000000:   // Timer 配置 (后续)

        default:
            // 未识别的设备地址，暂不处理
            break;
    }
}