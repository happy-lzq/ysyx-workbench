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

#include "local-include/reg.h"
#include <cpu/cpu.h>
#include <cpu/ifetch.h>
#include <cpu/decode.h>

#define R(i) gpr(i)     // 宏定义利用R(i) 替换gpr(i)
#define Mr vaddr_read   // Mr 内存读
#define Mw vaddr_write  // Mw 内存写
// 此处枚举int数字类型用于后续decode_operand传入指令类型的整数参数
enum {
  TYPE_I, TYPE_U, TYPE_S, TYPE_R, TYPE_J, TYPE_B,
  TYPE_N, // None 无类型指令
};
/*  BITS(i, hi, lo)：这个宏的作用是从 32 位整数 i 中，把从第 hi 位到第 lo 位的这段比特流抠出来,并且进行右对齐，再进行扩展
    SEXT(val, len)：Sign EXTension（符号扩展）。
    作用是把一个 len 位长度的有符号数，强行拉长到 32 位（如果最高位是 1，前面全补 1；如果是 0，前面全补 0）。
*/
/*================================== IUSRJB 立即数处理 ==================================================*/
#define src1R() do { *src1 = R(rs1); } while (0)
#define src2R() do { *src2 = R(rs2); } while (0)
#define immI() do { *imm = SEXT(BITS(i, 31, 20), 12); } while(0)   
#define immU() do { *imm = SEXT(BITS(i, 31, 12), 20) << 12; } while(0)
#define immS() do { *imm = (SEXT(BITS(i, 31, 25), 7) << 5) | BITS(i, 11, 7); } while(0)
// R类型无立即数操作
#define immJ() do { \
  *imm = ((SEXT(BITS(i, 31, 31),  1)  << 20 ) |  \
          (SEXT(BITS(i, 19, 12),  8)  << 12 ) |  \
          (SEXT(BITS(i, 20, 20),  1)  << 11 ) |  \
          (SEXT(BITS(i, 30, 21), 10)  << 1  )); \
} while(0)
#define immB() do { \
  *imm = ((SEXT(BITS(i, 31, 31),  1)  << 12 ) | \
          (SEXT(BITS(i,  7,  7),  1)  << 11 ) | \
          (SEXT(BITS(i, 30, 25),  6)  <<  5 ) | \
          (SEXT(BITS(i, 11,  8),  4)  <<  1 )); \
} while(0)

static void decode_operand(Decode *s, int *rd, word_t *src1, word_t *src2, word_t *imm, int type) {
  uint32_t i = s->isa.inst;
  int rs1 = BITS(i, 19, 15);
  int rs2 = BITS(i, 24, 20);
  *rd     = BITS(i, 11, 7);
  switch (type) {
    case TYPE_I: src1R();          immI(); break;
    case TYPE_U:                   immU(); break;   // U无源寄存器类型
    case TYPE_S: src1R(); src2R(); immS(); break;
    case TYPE_R: src1R(); src2R();         break;   // R无立即数 类型
    case TYPE_J:                   immJ(); break;   // J无源寄存器类型
    case TYPE_B: src1R(); src2R(); immB(); break;
    case TYPE_N: break;
    default: panic("unsupported type = %d", type);
  }
}

static int decode_exec(Decode *s) {
  s->dnpc = s->snpc;

#define INSTPAT_INST(s) ((s)->isa.inst)       // 宏定义获取指令码
#define INSTPAT_MATCH(s, name, type, ... /* execute body */ ) { \
  int rd = 0; \
  word_t src1 = 0, src2 = 0, imm = 0; \
  decode_operand(s, &rd, &src1, &src2, &imm, concat(TYPE_, type)); \
  __VA_ARGS__ ; \
}
// INSTPAT条目核心：opcode functs3 functs7 ；源寄存器，目标寄存器，以及立即数利用 ? 表示 
  INSTPAT_START();
  INSTPAT("??????? ????? ????? ??? ????? 00101 11", auipc  , U, R(rd) = s->pc + imm);
  INSTPAT("??????? ????? ????? 100 ????? 00000 11", lbu    , I, R(rd) = Mr(src1 + imm, 1));
  INSTPAT("??????? ????? ????? 000 ????? 00100 11", addi   , I, R(rd) = src1 + imm);

  INSTPAT("??????? ????? ????? 000 ????? 01000 11", sb     , S, Mw(src1 + imm, 1, src2));
  INSTPAT("??????? ????? ????? 010 ????? 01000 11", sw     , S, Mw(src1 + imm, 4, src2));

  INSTPAT("??????? ????? ????? ??? ????? 11011 11", jal    , J, R(rd) = s->snpc; s->dnpc = s->pc + imm);
  
  INSTPAT("0000000 00001 00000 000 00000 11100 11", ebreak , N, NEMUTRAP(s->pc, R(10))); // R(10) is $a0
  INSTPAT("??????? ????? ????? ??? ????? ????? ??", inv    , N, INV(s->pc));
  INSTPAT_END();

  R(0) = 0; // reset $zero to 0

  return 0;
}

int isa_exec_once(Decode *s) {
  s->isa.inst = inst_fetch(&s->snpc, 4);
  return decode_exec(s);
}
