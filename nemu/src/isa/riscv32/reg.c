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
#include <common.h>
#include <isa.h>
#include <isa-def.h> 
#include "local-include/reg.h"

extern CPU_state cpu; 
const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

// 注意：cpu 变量通常在 src/cpu/cpu-exec.c 中定义，这里不需要再次定义，否则会报错
// CPU_state cpu = {};  <-- 删除或注释掉这一行

void isa_reg_display() {
  int length = ARRLEN(regs);
    // 单独打印 PC
  printf("pc  = 0x%08x\n", cpu.pc);
  // 打印通用寄存器 (GPR)，格式：4列 x 8行
  for (int i = 0; i < length; i++) {
    // 打印当前寄存器：名字占3字符，值占10字符(0x...)，制表符对齐
    printf("%-3s = 0x%08x\t", regs[i], cpu.gpr[i]);
    
    // 每打印 4 个寄存器换一行
    if ((i + 1) % 4 == 0) {
      printf("\n");
    }
  }
}
word_t isa_reg_str2val(const char *s, bool *success) {
  if (s == NULL) {
    if (success) *success = false;
    return 0;
  }

  const char *name = s;
  if (name[0] == '$') name = name + 1; // accept both "$t0" and "t0"

  // check pc first
  if (strcmp(name, "pc") == 0) {
    if (success) *success = true;
    return cpu.pc;
  }

  // if starts with digit, parse numeric register index
  if (name[0] >= '0' && name[0] <= '9') {
    char *endptr = NULL;
    long idx = strtol(name, &endptr, 10);
    if (endptr != NULL && *endptr == '\0' && idx >= 0 && idx < (long)ARRLEN(regs)) {
      if (success) *success = true;
      return cpu.gpr[idx];
    } else {
      if (success) *success = false;
      return 0;
    }
  }

  // search named registers (regs array) - regs[] may contain "$0" for index 0 or names without '$'
  int len = ARRLEN(regs);
  for (int i = 0; i < len; i++) {
    const char *r = regs[i];
    // compare both with and without leading '$'
    if (r[0] == '$') {
      if (strcmp(r + 1, name) == 0) {
        if (success) *success = true;
        return cpu.gpr[i];
      }
    }
    if (strcmp(r, name) == 0) {
      if (success) *success = true;
      return cpu.gpr[i];
    }
  }

  // not found
  if (success) *success = false;
  return 0;
}
