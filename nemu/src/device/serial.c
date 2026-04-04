/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <utils.h>
#include <device/map.h>

/* http://en.wikibooks.org/wiki/Serial_Programming/8250_UART_Programming */
// NOTE: this is compatible to 16550

#define CH_OFFSET 0

static uint8_t *serial_base = NULL;   // IOMap maps[NR_MAP]->void *space  设备状态区


static void serial_putc(char ch) {
  MUXDEF(CONFIG_TARGET_AM, putch(ch), putc(ch, stderr));
}

static void serial_io_handler(uint32_t offset, int len, bool is_write) {
  assert(len == 1);       // 串口只支持单字节访问
  switch (offset) {
    /* We bind the serial port with the host stderr in NEMU. */
    case CH_OFFSET:       // 偏移量位0 是收发缓冲寄存器
      if (is_write) serial_putc(serial_base[0]);
      else panic("do not support read");
      break;
    default: panic("do not support offset = %d", offset);
  }
}
// ============================= 初始化nemu模拟外部设备I/O状态 ==========================
void init_serial() {
  serial_base = new_space(8);      // 
/*  
  采用8250 UART串口 总计8个8位寄存器(8byte总空间)，偏移量位0 是收发缓冲寄存器
  new_space(int size) 就是为设备向nemu申请模拟的设备寄存器/状态存储内存空间，按页申请。
 */ 
#ifdef CONFIG_HAS_PORT_IO
  add_pio_map ("serial", CONFIG_SERIAL_PORT, serial_base, 8, serial_io_handler);
#else
  add_mmio_map("serial", CONFIG_SERIAL_MMIO, serial_base, 8, serial_io_handler);
#endif

}

/* 回调函数与IOmap 结构体数组成员：io_callback_t callback 构成回调关联
1、init_serial()函数中利用定义add_mmio_map()将IOmap 结构体成员进行绑定，包括回调函数
2、void add_mmio_map(const char *name, paddr_t addr, void *space, uint32_t len, io_callback_t callback)
*/