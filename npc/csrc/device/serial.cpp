#include <device.h>

extern "C" void npc_serial_putc(unsigned char ch) {
  fputc(ch, stderr);
  fflush(stderr);
}
