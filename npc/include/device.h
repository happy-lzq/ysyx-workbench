#ifndef __NPC_DEVICE_H__
#define __NPC_DEVICE_H__
#include <npc.h>
#include <mmio.h>
#include <csignal>
// ================ 外部设备初始化 ========================

extern volatile sig_atomic_t alarm_fired;
extern uint64_t host_time_us() ;
void init_vga();
uint8_t* new_space(int size);
void init_i8042();
void send_key(uint8_t scancode, bool is_keydown);
void sdl_clear_event_queue();

// ================= 外部设计的回调函数声明 =================
void serial_write_handler(MMIODevice *dev, uint32_t offset,
                    uint32_t data, uint8_t wmask);
uint32_t rtc_read_handler(MMIODevice *dev, uint32_t offset);
void vga_update_screen();

#ifdef __cplusplus
extern "C" {
#endif

void npc_serial_putc(unsigned char ch);

#ifdef __cplusplus
}
#endif

#endif


