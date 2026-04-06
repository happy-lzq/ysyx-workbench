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

#include <isa.h>
#include <memory/host.h>
#include <memory/vaddr.h>
#include <device/map.h>

#define IO_SPACE_MAX (32 * 1024 * 1024)
static void invoke_callback(io_callback_t c, paddr_t offset, int len, bool is_write);
static uint8_t *io_space = NULL;
static uint8_t *p_space = NULL;
static FILE *dtrace_fp = NULL;
//预分配的 IO 区域 io_space 中返回一段连续内存作为设备的 backing buffer（设备寄存器/状态存储）。大小 high - low +1
/*
1、页对齐知识点
  ①addr & ~PAGE_MASK ：寻找当前页的起始地址
  ②(size + (PAGE_SIZE - 1)) & ~PAGE_MASK ：（）任何地址+4095）& ~page_mask  下一页的起始地址
2、根据传参size按字节大小向nemu申请设备寄存器/状态内存空间，按页申请
3、返回指针p 即是开辟的设备寄存器/状态内存空间
*/
void init_dtrace_log(const char *path){
  if (path !=NULL)
  {
    dtrace_fp = fopen(path,"w");
    Assert(dtrace_fp,"can't open the dtrace log file '%s'",path);
  }
  
}
static void dtrace_read(IOMap *map, paddr_t addr,int len,word_t ret){
    if (dtrace_fp != NULL){
    fprintf(dtrace_fp,"dtrace: read from device [%-10s] at index " FMT_PADDR "(len = %d) ->> ret = " FMT_WORD "\n",map->name,addr,len,ret);
    fflush(dtrace_fp);
  }
}

static void dtrace_write(IOMap *map, paddr_t addr,int len,word_t ret){
    if (dtrace_fp != NULL){
    fprintf(dtrace_fp,"dtrace: write to device [%-10s] at index " FMT_PADDR "(len = %d) ->> ret = " FMT_WORD "\n",map->name,addr,len,ret);
    fflush(dtrace_fp);
  }
}

uint8_t* new_space(int size) {
  uint8_t *p = p_space;
  // page aligned;
  size = (size + (PAGE_SIZE - 1)) & ~PAGE_MASK;     
  p_space += size;
  assert(p_space - io_space < IO_SPACE_MAX);
  return p;
}

static void check_bound(IOMap *map, paddr_t addr) {
  if (map == NULL) {
    Assert(map != NULL, "address (" FMT_PADDR ") is out of bound at pc = " FMT_WORD, addr, cpu.pc);
  } else {
    Assert(addr <= map->high && addr >= map->low,
        "address (" FMT_PADDR ") is out of bound {%s} [" FMT_PADDR ", " FMT_PADDR "] at pc = " FMT_WORD,
        addr, map->name, map->low, map->high, cpu.pc);
  }
}

static void invoke_callback(io_callback_t c, paddr_t offset, int len, bool is_write) {
  if (c != NULL) { c(offset, len, is_write); }
}

void init_map() {
  io_space = malloc(IO_SPACE_MAX);
  assert(io_space);
  p_space = io_space;
}

word_t map_read(paddr_t addr, int len, IOMap *map) {
  assert(len >= 1 && len <= 8);
  check_bound(map, addr);
  paddr_t offset = addr - map->low;
  invoke_callback(map->callback, offset, len, false); // prepare data to read
  word_t ret = host_read(map->space + offset, len);
  #ifdef CONFIG_DTRACE 
    if (DTRACE_COND){
      dtrace_read(map,addr,len,ret);
    }
  #endif

  return ret;
}
/*
1、host_write():负责nemu模拟更新设备内部状态的内存镜像(数据存储功能)
2、invoke_callback()：负责触发设备的副作用行为（例如串口输出字符、VGA 刷新屏幕、时钟更新计数等）。
3、分离了“状态存储”和“行为触发”，使得设备模拟更清晰：回调函数可以依赖 space 中的最新数据来执行正确的操作
*/

void map_write(paddr_t addr, int len, word_t data, IOMap *map) {
  assert(len >= 1 && len <= 8);                   // 限制 MMIO 写操作的数据长度为 1 到 8 字节
  check_bound(map, addr);
  paddr_t offset = addr - map->low;               
  //  offset相当于外设内部的各个设备寄存器的偏移地址，比如当前offset=0代表地址为读写缓冲寄存器
  host_write(map->space + offset, len, data);    
  //  基于设备自身视角下的局部相对偏移地址，通过这个偏移+设备内存储存区域进行写。这才真正写入外设的内存里
  invoke_callback(map->callback, offset, len, true);
  #ifdef CONFIG_DTRACE 
  if (DTRACE_COND){
    dtrace_write(map,addr,len,data);
  }
  #endif  
}
