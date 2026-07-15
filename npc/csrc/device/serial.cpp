#include <device.h>
#include <mmio.h>

extern "C" void npc_serial_putc(unsigned char ch) {
  fputc(ch, stderr);
  // fflush(stderr);
}

void serial_write_handler(MMIODevice *dev, uint32_t offset,
                    uint32_t data, uint8_t wmask){
    switch (offset){
    case 0 : 
      if (wmask & 0x1){
        npc_serial_putc(dev->space[0]);
      }
      break;
    case 5 : // LSR — 线路状态寄存器
     break;
    default:
      break;
    }
}
