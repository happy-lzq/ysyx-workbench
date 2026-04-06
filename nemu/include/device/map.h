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

#ifndef __DEVICE_MAP_H__
#define __DEVICE_MAP_H__

#include <cpu/difftest.h>

uint8_t* new_space(int size);
typedef void(*io_callback_t)(uint32_t, int, bool);
/* 回调函数的理解
1、cpu与外设通信的回调函数定义规范（协议）
  uint32_t offset 相对该设备起始地址的偏移量
  int len         访问字节大小 通常按 1、2、4、8四种读取字节类别进行读写
  bool            读写标志位    ture代表cpu写设备  false代表cpu读设备 
2、串口设备回调函数定义：
  void serial_io_handler(uint32_t offset, int len, bool is_write)
*/



// 内存映射MMIO设备地图：描述和管理所有通过内存映射I/O（MMIO）方式访问的外部设备。 
typedef struct {
  const char *name;
  // we treat ioaddr_t as paddr_t here
  paddr_t low;
  paddr_t high;
  void *space;    //nemu模拟设备内部状态的内存区域   它将写入的数据复制到设备内部状态区的对应偏移处
  io_callback_t callback;
} IOMap;

static inline bool map_inside(IOMap *map, paddr_t addr) {
  return (addr >= map->low && addr <= map->high);
}

static inline int find_mapid_by_addr(IOMap *maps, int size, paddr_t addr) {
  int i;
  for (i = 0; i < size; i ++) {
    if (map_inside(maps + i, addr)) {
      difftest_skip_ref();
      return i;
    }
  }
  return -1;
}

void add_pio_map(const char *name, ioaddr_t addr,
        void *space, uint32_t len, io_callback_t callback);
void add_mmio_map(const char *name, paddr_t addr,
        void *space, uint32_t len, io_callback_t callback);

word_t map_read(paddr_t addr, int len, IOMap *map);
void map_write(paddr_t addr, int len, word_t data, IOMap *map);

#endif
