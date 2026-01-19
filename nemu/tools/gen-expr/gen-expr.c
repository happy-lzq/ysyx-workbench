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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

// ================================ 数据结构与权重池初始化 =================================//
#define FIRST_PROBABILITY 20  // 控制递归表达式生成概率
#define SECOND_PROBABILITY 15 // 控制递归表达式生成概率
static char buf[65536] = {};
typedef struct {
  int max_depth;      // 递归嵌套深度
  int max_atoms;      // 单层操作数
  int max_length;     // 最终表达式字符串长度
} complexity_t;
// 操作数/操作符结构体
typedef struct {
  const char *name; 
  int weight;       
} weight_item_t;

// 操作数、操作符池以及权重
typedef struct {
  weight_item_t *items;
  int n;
  int total;       
} weight_pool_t;

// 缓冲区定义
struct buf_state { 
    char *ptr;       // 缓冲区当前写入地址
    int rem;         // 缓冲区可用字节数
};

 const char *regs_name[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6","pc"
};
static int regs_name_n = sizeof(regs_name)/sizeof(regs_name[0]);
// 总体复杂度
static complexity_t cfg = {.max_depth = 3,.max_atoms = 4,.max_length = 4096};
// 操作数以及权重
static weight_item_t op_items[]= {
  {"dec",50},{"hex",30},{"reg",20}
};
static weight_pool_t op_pool = {op_items,sizeof(op_items)/sizeof(op_items[0]),0};
// 操作符以及权重
static weight_item_t al_items[]= {
  {"+",30}, {"-",30}, {"*",25}, {"/",15},{"!=",15},{"==",20},{"&&",20},{"||",20}
};
static weight_pool_t al_pool = { al_items, sizeof(al_items)/sizeof(al_items[0]), 0 };

// ==================== 权重总和与随机数随机生成对应权重下的操作数、操作符函数 ===============//
// 权重池权重总数准备
static void pool_prepare(weight_pool_t *p) {
  int sum = 0;
  for (int i = 0; i < p->n; i++) {
    if (p->items[i].weight > 0) {
      sum += p->items[i].weight;
    }
  }
  p->total = (sum > 0) ? sum : 1;
}
// 生成一个 0 到 total 之间的随机数 r，通过累加权重并比较 r < acc，实现按概率抽取
// 例如：r 落在 0-49 选 dec，50-79 选 hex 。
static int pool_pick(weight_pool_t *p) {
  if (p->n <= 0) return -1;
  int r = rand() % p->total;
  int acc = 0;
  for (int i = 0; i < p->n; i++) {
    acc += p->items[i].weight;
    if (r < acc) return i;
  }
  return p->n - 1;
}

// ============================== 安全字符串拼接 ===================================//
// append_str 将常量字符串到缓冲区  主要用于构造表达式结构
static void append_str(struct buf_state *s, const char *c) {
  if (s->rem <= 0) return;
  int n = snprintf(s->ptr, s->rem, "%s", c);
  if (n <= 0 || n >= s->rem) { s->rem = 0; return; }
  s->ptr += n; s->rem -= n;
}
// append_fmt 将格式化字符串到缓冲区  处理随机生成的操作符或操作数
static void append_fmt(struct buf_state *s, const char *fmt, ...) {
  if (s->rem <= 0) return;
  va_list ap;          // 可变参数列表指针
  va_start(ap, fmt);   
  int n = vsnprintf(s->ptr, s->rem, fmt, ap);   // 返回实际写入缓冲区的字节数
  va_end(ap);
  if (n <= 0 || n >= s->rem) { s->rem = 0; return; }
  s->ptr += n; s->rem -= n;
}


// ============================== 寄存器表达式处理 ===================================//
// 从寄存器结构体中随机选取寄存器名，并配上表达式引导符号 & 
static void append_reg_from_white(struct buf_state *s) {
  if (s->rem <= 0) return;
  const char *r = regs_name[rand() % regs_name_n];
  if (r[0] == '$'){
    r = r + 1;
  } 
  append_fmt(s, "$%s", r);
}


// ============================== 操作数选择 =======================================//
static void gen_operand(struct buf_state *s) {
  if (s->rem <= 0) return;
  int idx = pool_pick(&op_pool);                  // 返回值为随机从操作数池中选择对应的操作数类型标志  
  switch (idx){
    case 0 : append_fmt(s, "%d", rand() % 1000); 
      break;
    case 1 : append_fmt(s, "0x%X", rand() % 0x10000);
      break;
    case 2 : append_reg_from_white(s);
      break;
    default: append_fmt(s, "%d", rand() % 1000);
      break;
  }
}

// ============================= 递归生成表达式 ====================================//
static void gen_expr_rec(struct buf_state *s, int depth) {
  if (s->rem <= 0) return;
  // 每一层的操作数原子至少为1；
  int atoms = 1 + rand() % cfg.max_atoms;
  // depth>0 则存在递归嵌套，将当前第一个操作数以设定的概率成为子表达式，即用()括起来。
  if (depth > 0 && (rand() % 100) < FIRST_PROBABILITY) {
    append_str(s, "(");
    gen_expr_rec(s, depth - 1);
    append_str(s, ")");
  } else {
    gen_operand(s);
  }
  // 循环生成后续的“运算符 + 操作数”
  for (int i = 1; i < atoms && s->rem > 0; i++) {
    int opi = pool_pick(&al_pool);
    const char *op = al_pool.items[opi].name;
    append_fmt(s, " %s ", op);
  // 第二个操作数生成逻辑与第一个操作数生成逻辑一致
    if (depth > 0 && (rand() % 100) < SECOND_PROBABILITY) {
      append_str(s, "(");
      gen_expr_rec(s, depth - 1);
      append_str(s, ")");
    } else {
      gen_operand(s);
    }
  }
}

// ============================ 递归表达式 写入缓冲区 ==============================//
static void gen_rand_expr() {
  char tmp[4096];             // 缓冲区
  int i = 5;
  while (i--) {
    tmp[0] = '\0';
    struct buf_state s = {.ptr = tmp, .rem = (int)sizeof(tmp)};
    // 将tmp ,rem 用局部结构体定义，传递结构体指针来利用或者改变其对应值。
    gen_expr_rec(&s, cfg.max_depth);
    if (s.rem <= 0 || tmp[0] == '\0') continue;
     
    // 检查每个表达式的括号对是否合理
    int bal = 0, ok = 1;
    for (char *q = tmp; *q; q++) {
      if (*q == '(') {
        bal++;
      } else if (*q == ')') {
        if (bal == 0) {
           ok = 0; 
           break; 
          } 
        bal--; 
        }
    }
    if (!ok || bal != 0) continue;
    snprintf(buf, sizeof(buf), "%s", tmp);
    return;
  }
  snprintf(buf, sizeof(buf), "1+1");
}


int main() {
  int seed = time(0);
  srand(seed);
  // 操作数+操作符权重池初始化
  pool_prepare(&op_pool);
  pool_prepare(&al_pool);

  /* 从环境变量 GEN_N 获取生成条数，默认 1 */
  int gen_n = 1;  
  // 从进程环境变量中读取值
  const char *env = getenv("GEN_N");
  if (env) {
    char *end = NULL;
    long v = strtol(env, &end, 10);
    if (end != env && *end == '\0' && v > 0){ 
      gen_n = (int)v;
    }
  }
  const char *out_path = getenv("GEN_OUT");
  if (!out_path) out_path = "input";          // 相当于给该环境地址贴上一个标签，用input来标识
  FILE *out = fopen(out_path, "w");
  if (!out) {
    perror(out_path);
    return 1;
  }
  for (int i = 0; i < gen_n; i ++) {
    gen_rand_expr();
    fprintf(out, "%s\n", buf);
    if (i < 10) {
      fprintf(stderr, "%s\n", buf);
    }
  }

  fclose(out);
  return 0;
}
