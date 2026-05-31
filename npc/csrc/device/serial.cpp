#include <cstdio>

extern "C" void npc_serial_putc(unsigned char ch) {
  std::fputc(ch, stderr);
  std::fflush(stderr);
}
