#ifndef __NPC_DEVICE_H__
#define __NPC_DEVICE_H__
#include <npc.h>
#include <csignal>

extern volatile sig_atomic_t alarm_fired;
extern uint64_t host_time_us() ;
#ifdef __cplusplus
extern "C" {
#endif

void npc_serial_putc(unsigned char ch);

#ifdef __cplusplus
}
#endif

#endif
