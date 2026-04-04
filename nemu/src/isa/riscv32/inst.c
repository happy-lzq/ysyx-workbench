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
/*================================== IUSRJB 立即数处理 先拼接再符号位扩展 ==================================================*/

#define src1R() do { *src1 = R(rs1); } while (0)
#define src2R() do { *src2 = R(rs2); } while (0)
#define immI()  do { *imm = SEXT (BITS(i, 31, 20), 12); } while(0)   
#define immU()  do { *imm = SEXT (BITS(i, 31, 12), 20) << 12; } while(0)
#define immS()  do { *imm = SEXT(((BITS(i, 31, 25) << 5) | BITS(i, 11, 7)),12); } while(0)
// R类无立即数：第0位固定为0，最高位为20位，总计立即数为21位
#define immJ() do {                             \
  uint32_t val = ((BITS(i, 31, 31))  << 20 ) |  \
                 ((BITS(i, 19, 12))  << 12 ) |  \
                 ((BITS(i, 20, 20))  << 11 ) |  \
                 ((BITS(i, 30, 21))  << 1  ) ;  \
  *imm = SEXT(val,21)                        ;  \
} while(0)
// B型立即数：第0位固定为0，最高位为12位，总计立即数为13位
#define immB() do {                              \
  uint32_t val = ((BITS(i, 31, 31))  << 12 ) |   \
                 ((BITS(i,  7,  7))  << 11 ) |   \
                 ((BITS(i, 30, 25))  <<  5 ) |   \
                 ((BITS(i, 11,  8))  <<  1 ) ;   \
  *imm = SEXT(val,13)                        ;   \
} while(0)

static void decode_operand(Decode *s, int *rd, word_t *src1, word_t *src2, word_t *imm, int type) {
  uint32_t i = s->isa.inst;
  int rs1 = BITS(i, 19, 15);
  int rs2 = BITS(i, 24, 20);
  *rd     = BITS(i, 11, 7) ;
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

  // 1. 系统指令 (系统调用、断点) 放在最前面
  INSTPAT("0000000 00001 00000 000 00000 11100 11", ebreak , N, NEMUTRAP(s->pc, R(10))); 
  // 在 NEMU 的测试框架中，ebreak 被用作程序结束的标志，并根据 a0 的值判断测试是否通过：a0 = 0 表示成功（GOOD TRAP），a0 ≠ 0 表示失败（BAD TRAP）。

  // 2. 根据 opcode 进行 case 分类 (顺序: R I S B U J)
  switch (BITS(s->isa.inst, 6, 0)) {

    // =================================== R-Type (opcode = 0x33) ==================================================
    case 0x33:
      // 算数逻辑运算按常用程度排序
      INSTPAT("0000000 ????? ????? 000 ????? 01100 11", add    , R, R(rd) = src1 + src2);                     // x[rd] = x[rs1] + x[rs2]
      INSTPAT("0100000 ????? ????? 000 ????? 01100 11", sub    , R, R(rd) = src1 - src2);                     // x[rd] = x[rs1] - x[rs2]
      INSTPAT("0000000 ????? ????? 111 ????? 01100 11", and    , R, R(rd) = src1 & src2);                                                        // x[rd] = x[rs1] & x[rs2]
      INSTPAT("0000000 ????? ????? 110 ????? 01100 11", or     , R, R(rd) = src1 | src2);                                                        // x[rd] = x[rs1] | x[rs2]
      INSTPAT("0000000 ????? ????? 100 ????? 01100 11", xor    , R, R(rd) = src1 ^ src2);                                                        // x[rd] = x[rs1] ^ x[rs2]
      INSTPAT("0000000 ????? ????? 001 ????? 01100 11", sll    , R, R(rd) = src1 << (src2 & 0x1F));                                              // x[rd] = x[rs1] ≪ x[rs2]
      INSTPAT("0000000 ????? ????? 101 ????? 01100 11", srl    , R, R(rd) = src1 >> (src2 & 0x1F));                                              // x[rd] = (x[rs1] ≫𝑢 x[rs2])
      INSTPAT("0100000 ????? ????? 101 ????? 01100 11", sra    , R, R(rd) = (int32_t)src1 >> (src2 & 0x1F));                                     // x[rd] = (x[rs1] ≫𝑠 x[rs2])
      INSTPAT("0000000 ????? ????? 010 ????? 01100 11", slt    , R, R(rd) = (int32_t)src1 < (int32_t)src2);                                      // x[rd] = (x[rs1] <𝑠 x[rs2])
      INSTPAT("0000000 ????? ????? 011 ????? 01100 11", sltu   , R, R(rd) = src1 < src2);                                                        // x[rd] = (x[rs1] <𝑢 x[rs2])
      
      // 乘除法运算指令 (M-Extension)
      INSTPAT("0000001 ????? ????? 000 ????? 01100 11", mul    , R, R(rd) = src1 * src2);                                                        // x[rd] = x[rs1] × x[rs2]
      INSTPAT("0000001 ????? ????? 001 ????? 01100 11", mulh   , R, R(rd) = ((int64_t)(int32_t)src1 * (int64_t)(int32_t)src2) >> 32);            // x[rd] = (x[rs1] 𝑠 ×𝑠 x[rs2]) ≫𝑠 XLEN
      INSTPAT("0000001 ????? ????? 011 ????? 01100 11", mulhu  , R, R(rd) = ((uint64_t)src1 * (uint64_t)src2) >> 32);                            // x[rd] = (x[rs1] 𝑢 ×𝑢 x[rs2]) ≫𝑢 XLEN
      INSTPAT("0000001 ????? ????? 010 ????? 01100 11", mulhsu , R, R(rd) = ((int64_t)(int32_t)src1 * (uint64_t)src2) >> 32);                    // x[rd] = (x[rs1] 𝑠 ×𝑢 x[rs2]) ≫𝑠 XLEN
      INSTPAT("0000001 ????? ????? 100 ????? 01100 11", div    , R, R(rd) = ((src2 == 0 ) ? ~0 :                                                 //  x[rd] = x[rs1] ÷s x[rs2]
                                                                            ((src1 == 0x80000000 && src2 == -1) ? 0x80000000 :     
                                                                            (int32_t) src1 / (int32_t) src2 )));  
      INSTPAT("0000001 ????? ????? 101 ????? 01100 11", divu   , R, R(rd) = (src2 == 0 ) ? ~0: src1 / src2);                                     // x[rd] = x[rs1] ÷u x[rs2]
      INSTPAT("0000001 ????? ????? 110 ????? 01100 11", rem    , R, R(rd) = ((src2 == 0 ) ? src1 :                                               // x[rd] = x[rs1] %𝑠 x[rs2]
                                                                            ((src1 == 0x80000000 && src2 == -1) ? 0x00000000 : 
                                                                            (int32_t) src1 % (int32_t) src2 ))) ;  
      INSTPAT("0000001 ????? ????? 111 ????? 01100 11", remu   , R, R(rd) = (src2 == 0 ) ? src1: src1 % src2);                                   // x[rd] = x[rs1] %𝑢 x[rs2]
      break;

    // =================================== I-Type (opcode = 0x13, 0x03, 0x67) ==============================================
    case 0x13: // ALU-I
    case 0x03: // Load 
    case 0x67: // jalr
      // I-Type 按照 最常用内存读和立即数算数排序
      INSTPAT("??????? ????? ????? 000 ????? 00100 11", addi   , I, R(rd) = src1 + imm);                      // x[rd] = x[rs1] + sext(immediate)
      INSTPAT("??????? ????? ????? 010 ????? 00000 11", lw     , I, R(rd) = Mr(src1 + imm, 4));               // x[rd] = sext(M[x[rs1] + sext(offset)][31:0])
      INSTPAT("??????? ????? ????? 000 ????? 11001 11", jalr   , I, R(rd) = s->snpc; s->dnpc = (src1 + imm) & ~1;       // ret伪指令实际被扩展为 jalr x0, 0(x1)
          IFDEF(CONFIG_FTRACE,
            int rs1_idx = BITS(s->isa.inst, 19, 15);                                                                    // src1 对应源寄存器索引
            if (rd == 1) {
              ftrace_call(s->pc, s->dnpc, s->snpc);
            } else if (rd == 0 && rs1_idx == 1 && imm == 0) {       // ret 返程不需要返回地址。rd=x0,并且src1索引为返回寄存器，并且imm=0,说明执行返回栈 ret 伪指令
              ftrace_ret(s->pc, s->dnpc);                           
            }
          );
      );
      INSTPAT("0000000 ????? ????? 001 ????? 00100 11", slli   , I, R(rd) = src1 << (imm & 0x1F));            // x[rd] = x[rs1] ≪ shamt 当且仅当shamt[5]=0,有效
      INSTPAT("0000000 ????? ????? 101 ????? 00100 11", srli   , I, R(rd) = src1 >> (imm & 0x1F));            // x[rd] = (x[rs1] ≫𝑢 shamt)
      INSTPAT("0100000 ????? ????? 101 ????? 00100 11", srai   , I, R(rd) = (int32_t)src1 >> (imm & 0x1F));    // x[rd] = (x[rs1] ≫𝑠 shamt)
      INSTPAT("??????? ????? ????? 111 ????? 00100 11", andi   , I, R(rd) = src1 & imm);                      // x[rd] = x[rs1] & sext(immediate)
      INSTPAT("??????? ????? ????? 110 ????? 00100 11", ori    , I, R(rd) = src1 | imm);                      // x[rd] = x[rs1] | sext(immediate)
      INSTPAT("??????? ????? ????? 100 ????? 00100 11", xori   , I, R(rd) = src1 ^ imm);                      // x[rd] = x[rs1] ^ sext(immediate)
      INSTPAT("??????? ????? ????? 010 ????? 00100 11", slti   , I, R(rd) = (int32_t)src1 < (int32_t)imm) ;                                      // x[rd] = (x[rs1] <𝑠 sext(immediate))
      INSTPAT("??????? ????? ????? 011 ????? 00100 11", sltiu  , I, R(rd) = src1 < imm);                                                         // x[rd] = (x[rs1] <𝑢 sext(immediate))
      
      // 字寻址存储装载指令 (Sub-word Load)
      INSTPAT("??????? ????? ????? 101 ????? 00000 11", lhu    , I, R(rd) = Mr(src1 + imm, 2));               // x[rd] = M[x[rs1] + sext(offset)][15:0]
      INSTPAT("??????? ????? ????? 100 ????? 00000 11", lbu    , I, R(rd) = Mr(src1 + imm, 1));               // x[rd] = M[x[rs1] + sext(offset)][7:0]
      INSTPAT("??????? ????? ????? 001 ????? 00000 11", lh     , I, R(rd) = (int16_t)Mr(src1 + imm, 2));      // x[rd] = sext(M[x[rs1] + sext(offset)][15:0])
      INSTPAT("??????? ????? ????? 000 ????? 00000 11", lb     , I, R(rd) = (int8_t)Mr(src1 + imm, 1));       // x[rd] = sext(M[x[rs1] + sext(offset)][7:0])
      break;

    // =================================== S-Type (opcode = 0x23) ==================================================
    case 0x23:
      INSTPAT("??????? ????? ????? 010 ????? 01000 11", sw     , S, Mw(src1 + imm, 4, src2));                 // M[x[rs1] + sext(offset) = x[rs2][31: 0]
      INSTPAT("??????? ????? ????? 001 ????? 01000 11", sh     , S, Mw(src1 + imm, 2, src2));                 // M[x[rs1] + sext(offset) = x[rs2][15: 0]
      INSTPAT("??????? ????? ????? 000 ????? 01000 11", sb     , S, Mw(src1 + imm, 1, src2));                 // M[x[rs1] + sext(offset) = x[rs2][7: 0]
      break;

    // =================================== B-Type (opcode = 0x63) ==================================================
    case 0x63:
      INSTPAT("??????? ????? ????? 001 ????? 11000 11", bne    , B, if (src1 != src2) s->dnpc = s->pc + imm);
      INSTPAT("??????? ????? ????? 000 ????? 11000 11", beq    , B, if (src1 == src2) s->dnpc = s->pc + imm);
      INSTPAT("??????? ????? ????? 100 ????? 11000 11", blt    , B, if ((int32_t)src1 < (int32_t)src2) s->dnpc = s->pc + imm);
      INSTPAT("??????? ????? ????? 101 ????? 11000 11", bge    , B, if ((int32_t)src1 >= (int32_t)src2) s->dnpc = s->pc + imm);
      INSTPAT("??????? ????? ????? 110 ????? 11000 11", bltu   , B, if (src1 < src2) s->dnpc = s->pc + imm);
      INSTPAT("??????? ????? ????? 111 ????? 11000 11", bgeu   , B, if (src1 >= src2) s->dnpc = s->pc + imm);
      break;

    // =================================== U-Type (opcode = 0x37, 0x17) ============================================
    case 0x37: // lui
    case 0x17: // auipc
      INSTPAT("??????? ????? ????? ??? ????? 01101 11", lui    , U, R(rd) = imm);                             // x[rd] = sext(immediate[31:12] << 12)
      INSTPAT("??????? ????? ????? ??? ????? 00101 11", auipc  , U, R(rd) = s->pc + imm);                     // x[rd] = pc + sext(immediate[31:12] << 12)
      break;

    // =================================== J-Type (opcode = 0x6f) ==================================================
    case 0x6f: // jal
      INSTPAT("??????? ????? ????? ??? ????? 11011 11", jal    , J, R(rd) = s->snpc; s->dnpc = s->pc + imm;              // rd 默认为 x1(ra=返回寄存器)
          IFDEF(CONFIG_FTRACE, 
            if (rd == 1) {  // 约定：x1=ra返回寄存器在32个通用寄存器中的索引为：1 当该寄存器被写入数据时，说明存在函数调用。
            ftrace_call(s->pc, s->dnpc, s->snpc);
            }
          )
        );
      break;

  }

  // =================================== 末尾兜底无效指令 ==========================================================
  INSTPAT("??????? ????? ????? ??? ????? ????? ??", inv    , N, INV(s->pc));

// =================================== 2. 最常用算术指令 (I-Type ALU & Load/Store Word) ==================================================
  INSTPAT("??????? ????? ????? 000 ????? 00100 11", addi   , I, R(rd) = src1 + imm);                      // x[rd] = x[rs1] + sext(immediate)
  INSTPAT("??????? ????? ????? 010 ????? 00000 11", lw     , I, R(rd) = Mr(src1 + imm, 4));               // x[rd] = sext(M[x[rs1] + sext(offset)][31:0])
  INSTPAT("??????? ????? ????? 010 ????? 01000 11", sw     , S, Mw(src1 + imm, 4, src2));                 // M[x[rs1] + sext(offset) = x[rs2][31: 0]

// ==================================== 3. 分支跳转核心指令 (Branch & Jump) =================================================================
  INSTPAT("??????? ????? ????? 001 ????? 11000 11", bne    , B, if (src1 != src2) s->dnpc = s->pc + imm);
  INSTPAT("??????? ????? ????? 000 ????? 11000 11", beq    , B, if (src1 == src2) s->dnpc = s->pc + imm);
  INSTPAT("??????? ????? ????? 100 ????? 11000 11", blt    , B, if ((int32_t)src1 < (int32_t)src2) s->dnpc = s->pc + imm);
  INSTPAT("??????? ????? ????? 101 ????? 11000 11", bge    , B, if ((int32_t)src1 >= (int32_t)src2) s->dnpc = s->pc + imm);
  INSTPAT("??????? ????? ????? 110 ????? 11000 11", bltu   , B, if (src1 < src2) s->dnpc = s->pc + imm);
  INSTPAT("??????? ????? ????? 111 ????? 11000 11", bgeu   , B, if (src1 >= src2) s->dnpc = s->pc + imm);

  INSTPAT("??????? ????? ????? ??? ????? 11011 11", jal    , J, R(rd) = s->snpc; s->dnpc = s->pc + imm;              // rd 默认为 x1(ra=返回寄存器)
      IFDEF(CONFIG_FTRACE, 
        if (rd == 1) {  // 约定：x1=ra返回寄存器在32个通用寄存器中的索引为：1 当该寄存器被写入数据时，说明存在函数调用。
        ftrace_call(s->pc, s->dnpc, s->snpc);
        }
      )
    );
  INSTPAT("??????? ????? ????? 000 ????? 11001 11", jalr   , I, R(rd) = s->snpc; s->dnpc = (src1 + imm) & ~1;       // ret伪指令实际被扩展为 jalr x0, 0(x1)
      IFDEF(CONFIG_FTRACE,
        int rs1_idx = BITS(s->isa.inst, 19, 15);                                                                    // src1 对应源寄存器索引
        if (rd == 1) {
          ftrace_call(s->pc, s->dnpc, s->snpc);
        } else if (rd == 0 && rs1_idx == 1 && imm == 0) {       // ret 返程不需要返回地址。rd=x0,并且src1索引为返回寄存器，并且imm=0,说明执行返回栈 ret 伪指令
          ftrace_ret(s->pc, s->dnpc);                           
        }
      );
  );

// =================================== 4. 常见算术逻辑指令 (ALU R-type & I-Type Others) ==================================================
  INSTPAT("0000000 ????? ????? 000 ????? 01100 11", add    , R, R(rd) = src1 + src2);                     // x[rd] = x[rs1] + x[rs2]
  INSTPAT("0100000 ????? ????? 000 ????? 01100 11", sub    , R, R(rd) = src1 - src2);                     // x[rd] = x[rs1] - x[rs2]
  INSTPAT("0000000 ????? ????? 001 ????? 00100 11", slli   , I, R(rd) = src1 << (imm & 0x1F));            // x[rd] = x[rs1] ≪ shamt 当且仅当shamt[5]=0,有效
  INSTPAT("0000000 ????? ????? 101 ????? 00100 11", srli   , I, R(rd) = src1 >> (imm & 0x1F));            // x[rd] = (x[rs1] ≫𝑢 shamt)
  INSTPAT("0100000 ????? ????? 101 ????? 00100 11", srai   , I, R(rd) = (int32_t)src1 >> (imm & 0x1F));   // x[rd] = (x[rs1] ≫𝑠 shamt)
  INSTPAT("??????? ????? ????? 111 ????? 00100 11", andi   , I, R(rd) = src1 & imm);                      // x[rd] = x[rs1] & sext(immediate)
  INSTPAT("??????? ????? ????? 110 ????? 00100 11", ori    , I, R(rd) = src1 | imm);                      // x[rd] = x[rs1] | sext(immediate)
  INSTPAT("??????? ????? ????? 100 ????? 00100 11", xori   , I, R(rd) = src1 ^ imm);                      // x[rd] = x[rs1] ^ sext(immediate)

  INSTPAT("0000000 ????? ????? 111 ????? 01100 11", and    , R, R(rd) = src1 & src2);                                                        // x[rd] = x[rs1] & x[rs2]
  INSTPAT("0000000 ????? ????? 110 ????? 01100 11", or     , R, R(rd) = src1 | src2);                                                        // x[rd] = x[rs1] | x[rs2]
  INSTPAT("0000000 ????? ????? 100 ????? 01100 11", xor    , R, R(rd) = src1 ^ src2);                                                        // x[rd] = x[rs1] ^ x[rs2]
  INSTPAT("0000000 ????? ????? 001 ????? 01100 11", sll    , R, R(rd) = src1 << (src2 & 0x1F));                                              // x[rd] = x[rs1] ≪ x[rs2]
  INSTPAT("0000000 ????? ????? 101 ????? 01100 11", srl    , R, R(rd) = src1 >> (src2 & 0x1F));                                              // x[rd] = (x[rs1] ≫𝑢 x[rs2])
  INSTPAT("0100000 ????? ????? 101 ????? 01100 11", sra    , R, R(rd) = (int32_t)src1 >> (src2 & 0x1F));                                     // x[rd] = (x[rs1] ≫𝑠 x[rs2])

  INSTPAT("0000000 ????? ????? 010 ????? 01100 11", slt    , R, R(rd) = (int32_t)src1 < (int32_t)src2);                                      // x[rd] = (x[rs1] <𝑠 x[rs2])
  INSTPAT("0000000 ????? ????? 011 ????? 01100 11", sltu   , R, R(rd) = src1 < src2);                                                        // x[rd] = (x[rs1] <𝑢 x[rs2])
  INSTPAT("??????? ????? ????? 010 ????? 00100 11", slti   , I, R(rd) = (int32_t)src1 < (int32_t)imm) ;                                      // x[rd] = (x[rs1] <𝑠 sext(immediate))
  INSTPAT("??????? ????? ????? 011 ????? 00100 11", sltiu  , I, R(rd) = src1 < imm);                                                         // x[rd] = (x[rs1] <𝑢 sext(immediate))

// =================================== 5. 地址加载指令 (U-Type) ==================================================================
  INSTPAT("??????? ????? ????? ??? ????? 01101 11", lui    , U, R(rd) = imm);                             // x[rd] = sext(immediate[31:12] << 12)
  INSTPAT("??????? ????? ????? ??? ????? 00101 11", auipc  , U, R(rd) = s->pc + imm);                     // x[rd] = pc + sext(immediate[31:12] << 12)

// =================================== 6. 次寻址存储装载指令 (Sub-word Load/Store) ==================================================================
  INSTPAT("??????? ????? ????? 101 ????? 00000 11", lhu    , I, R(rd) = Mr(src1 + imm, 2));               // x[rd] = M[x[rs1] + sext(offset)][15:0]
  INSTPAT("??????? ????? ????? 100 ????? 00000 11", lbu    , I, R(rd) = Mr(src1 + imm, 1));               // x[rd] = M[x[rs1] + sext(offset)][7:0]
  INSTPAT("??????? ????? ????? 001 ????? 00000 11", lh     , I, R(rd) = (int16_t)Mr(src1 + imm, 2));      // x[rd] = sext(M[x[rs1] + sext(offset)][15:0])
  INSTPAT("??????? ????? ????? 000 ????? 00000 11", lb     , I, R(rd) = (int8_t)Mr(src1 + imm, 1));       // x[rd] = sext(M[x[rs1] + sext(offset)][7:0])
  INSTPAT("??????? ????? ????? 001 ????? 01000 11", sh     , S, Mw(src1 + imm, 2, src2));                 // M[x[rs1] + sext(offset) = x[rs2][15: 0]
  INSTPAT("??????? ????? ????? 000 ????? 01000 11", sb     , S, Mw(src1 + imm, 1, src2));                 // M[x[rs1] + sext(offset) = x[rs2][7: 0]

// ==================================== 7. 乘除法运算指令 (M-Extension) ==============================================================
  INSTPAT("0000001 ????? ????? 000 ????? 01100 11", mul    , R, R(rd) = src1 * src2);                                                        // x[rd] = x[rs1] × x[rs2]
  INSTPAT("0000001 ????? ????? 001 ????? 01100 11", mulh   , R, R(rd) = ((int64_t)(int32_t)src1 * (int64_t)(int32_t)src2) >> 32);            // x[rd] = (x[rs1] 𝑠 ×𝑠 x[rs2]) ≫𝑠 XLEN
  INSTPAT("0000001 ????? ????? 011 ????? 01100 11", mulhu  , R, R(rd) = ((uint64_t)src1 * (uint64_t)src2) >> 32);                            // x[rd] = (x[rs1] 𝑢 ×𝑢 x[rs2]) ≫𝑢 XLEN
  INSTPAT("0000001 ????? ????? 010 ????? 01100 11", mulhsu , R, R(rd) = ((int64_t)(int32_t)src1 * (uint64_t)src2) >> 32);                    // x[rd] = (x[rs1] 𝑠 ×𝑢 x[rs2]) ≫𝑠 XLEN
  INSTPAT("0000001 ????? ????? 100 ????? 01100 11", div    , R, R(rd) = ((src2 == 0 ) ? ~0 :                                                 //  x[rd] = x[rs1] ÷s x[rs2]
                                                                        ((src1 == 0x80000000 && src2 == -1) ? 0x80000000 :     
                                                                        (int32_t) src1 / (int32_t) src2 )));  
  INSTPAT("0000001 ????? ????? 101 ????? 01100 11", divu   , R, R(rd) = (src2 == 0 ) ? ~0: src1 / src2);                                     // x[rd] = x[rs1] ÷u x[rs2]
  INSTPAT("0000001 ????? ????? 110 ????? 01100 11", rem    , R, R(rd) = ((src2 == 0 ) ? src1 :                                               // x[rd] = x[rs1] %𝑠 x[rs2]
                                                                        ((src1 == 0x80000000 && src2 == -1) ? 0x00000000 : 
                                                                        (int32_t) src1 % (int32_t) src2 ))) ;  
  INSTPAT("0000001 ????? ????? 111 ????? 01100 11", remu   , R, R(rd) = (src2 == 0 ) ? src1: src1 % src2);                                   // x[rd] = x[rs1] %𝑢 x[rs2]

  INSTPAT_END();

  R(0) = 0; // reset $zero to 0

  return 0;
}

int isa_exec_once(Decode *s) {
  s->isa.inst = inst_fetch(&s->snpc, 4);
  return decode_exec(s);
}
