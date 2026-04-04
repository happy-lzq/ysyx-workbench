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

#ifndef __MEMORY_PADDR_H__
#define __MEMORY_PADDR_H__

#include <common.h>

#define PMEM_LEFT  ((paddr_t)CONFIG_MBASE)                      // nemu物理内存起始地址
#define PMEM_RIGHT ((paddr_t)CONFIG_MBASE + CONFIG_MSIZE - 1)   // nemu物理内存终止地址
#define RESET_VECTOR (PMEM_LEFT + CONFIG_PC_RESET_OFFSET)       // 对应宿主机nemu的物理内存起始地址pmem
/*
  1、客户机.c程序链接确定elf文件的链接地址：0x800000000 （nmeu.mk）
  2、宿主机nmeu将.bin文件加载到起始地址：RESET_VECTOR=0x80000000 (init.c)
  3、宿主机cpu.pc 起始地址：RESET_VECTOR=0x80000000  (init.c)
  4、总上三者约定好数据对应的起始地址，进而在进行cpu处理时地址与数据一一对应
*/
/* convert the guest physical address in the guest program to host virtual address in NEMU */
uint8_t* guest_to_host(paddr_t paddr);
/* convert the host virtual address in NEMU to guest physical address in the guest program */
paddr_t host_to_guest(uint8_t *haddr);

static inline bool in_pmem(paddr_t addr) {
  return addr - CONFIG_MBASE < CONFIG_MSIZE;
}

word_t paddr_read(paddr_t addr, int len);
void paddr_write(paddr_t addr, int len, word_t data);

#endif
