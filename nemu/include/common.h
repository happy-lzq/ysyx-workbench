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

#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include <generated/autoconf.h>
#include <macro.h>

#ifdef CONFIG_TARGET_AM
#include <klib.h>
#else
#include <assert.h>
#include <stdlib.h>
#endif

#if CONFIG_MBASE + CONFIG_MSIZE > 0x100000000ul   
//（物理内存基址 + 大小）超过 32-bit 上限（0x1_0000_0000），就需要用 64-bit 类型来表示物理地址 -> 设置 PMEM64 为真
#define PMEM64 1
#endif
/*
1、MUXDEF的构建逻辑：MUXDEF(cond, a, b)
   cond ? a : b  但这是编译期宏展开

2、
    情况 A：CONFIG_ISA64=1, PMEM64=0
        word_t -> uint64_t
        sword_t -> int64_t
        FMT_WORD -> 0x%016PRIx64
        vaddr_t -> uint64_t
        paddr_t -> uint32_t
        FMT_PADDR -> 0x%08PRIx32
    情况 B：CONFIG_ISA64=0, PMEM64=0
        全部偏 32 位（除了你手动定义的其他类型）
*/
typedef MUXDEF(CONFIG_ISA64, uint64_t, uint32_t) word_t;
typedef MUXDEF(CONFIG_ISA64, int64_t, int32_t)  sword_t;
#define FMT_WORD MUXDEF(CONFIG_ISA64, "0x%016" PRIx64, "0x%08" PRIx32)

typedef word_t vaddr_t;
typedef MUXDEF(PMEM64, uint64_t, uint32_t) paddr_t;
#define FMT_PADDR MUXDEF(PMEM64, "0x%016" PRIx64, "0x%08" PRIx32)
typedef uint16_t ioaddr_t;

#include <debug.h>

#endif
