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

#include <memory/host.h>
#include <memory/paddr.h>
#include <device/mmio.h>
#include <isa.h>

#if   defined(CONFIG_PMEM_MALLOC)
static uint8_t *pmem = NULL;
#else // CONFIG_PMEM_GARRAY
static uint8_t pmem[CONFIG_MSIZE] PG_ALIGN = {};      
      // CONFIG_MSIZE=0X80000000   客户机的模拟的物理内存
      // PG_ALIGN ：让nemu物理内存条(pmem[]数组)按4096字节页对齐
#endif

uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }
/*
  1、paddr 客户机程序基于RISCV32交叉编译后产生的实际内存数据地址：0x80000000 ;返回对应nemu物理内存pmem中相对应的存储地址
    去掉基于真实RISCV32机器下的物理内存基址偏移，转换成模拟器nmeu宿主机数组中相对应的地址pmem数组下标
  2、haddr 宿主机nmeu内存pmem下的数组下标，进而反向转化为相对应的真实RISCV32机器下的物理内存地址
*/
static word_t pmem_read(paddr_t addr, int len) {
  word_t ret = host_read(guest_to_host(addr), len);
  return ret;
}
/*
  1、nmeu物理内存读取逻辑：paddr转化为pmem数组下标，进而根据len读取需求的字节长度
  2、nmeu物理内存写入逻辑：paddr转化为pmem数组下标，进而根据len写入要求的字节data
*/
static void pmem_write(paddr_t addr, int len, word_t data) {
  host_write(guest_to_host(addr), len, data);
}

static void out_of_bound(paddr_t addr) {
  panic("address = " FMT_PADDR " is out of bound of pmem [" FMT_PADDR ", " FMT_PADDR "] at pc = " FMT_WORD,
      addr, PMEM_LEFT, PMEM_RIGHT, cpu.pc);
}

void init_mem() {
#if   defined(CONFIG_PMEM_MALLOC)
  pmem = malloc(CONFIG_MSIZE);
  assert(pmem);
#endif
  IFDEF(CONFIG_MEM_RANDOM, memset(pmem, rand(), CONFIG_MSIZE));
  Log("physical memory area [" FMT_PADDR ", " FMT_PADDR "]", PMEM_LEFT, PMEM_RIGHT);
}

// 正式的物理内存读写功能的API函数

word_t paddr_read(paddr_t addr, int len) {
  if (likely(in_pmem(addr))) {
    word_t pr_data = pmem_read(addr,len);
    #ifdef CONFIG_WATCHPOINT

    #endif
    return pr_data;    // nmeu addr 属于物理内存内正常调用并反馈

  }
  
  IFDEF(CONFIG_DEVICE, return mmio_read(addr, len));         // nmeu addr 属于外部接口地址，则进入外部接口调用
  out_of_bound(addr);                                        // 不属于以上两个 越界
  return 0;
}

void paddr_write(paddr_t addr, int len, word_t data) {
  if (likely(in_pmem(addr))) { pmem_write(addr, len, data); return; }
  IFDEF(CONFIG_DEVICE, mmio_write(addr, len, data); return);
  out_of_bound(addr);
}
