#ifndef __NPC_DEVICE_H__
#define __NPC_DEVICE_H__

#include <stdint.h>

extern "C" void npc_serial_putc(unsigned char ch);
void mmio_write_handler(uint32_t addr, uint32_t wdata, uint8_t wmask);

#endif