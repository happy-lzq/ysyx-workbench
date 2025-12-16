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


static char buf[65536] = {};
typedef struct {
  int max_depth;      // 递归嵌套深度
  int max_atoms;      // 单层操作数
  int max_length;     // 最终表达式字符串长度
} complexity_t;

typedef struct {
  const char *name; 
  int weight;       
} weight_item_t;

typedef struct {
  weight_item_t *items;
  int n;
  int total;       
} weight_pool_t;

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
  {"dec",40},{"hex",10},{"reg",20}
};
static weight_pool_t op_pool = {op_items,sizeof(op_items)/sizeof(op_items[0]),0};
// 操作符以及权重
static weight_item_t al_items[]= {
  {"+",30}, {"-",30}, {"*",25}, {"/",15}
};
static weight_pool_t al_pool = { al_items, sizeof(al_items)/sizeof(al_items[0]), 0 };

// 权重池准备与抽样
static void pool_prepare(weight_pool_t *p) {
  int sum = 0;
  for (int i = 0; i < p->n; i++) {
    if (p->items[i].weight > 0) {
      sum += p->items[i].weight;
    }
  }
  p->total = (sum > 0) ? sum : 1;
}
// 利用随机生成一个权重总和为0~total之间的随机数，每次循环acc等于权重累加，
// 利用r与acc关系，当r<acc时，即是r随机数处于当前i对应的权重位。
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

/* safe append helpers */
static void append_str(char **pp, int *rem, const char *s) {
  if (*rem <= 0) return;
  int n = snprintf(*pp, *rem, "%s", s);
  if (n <= 0 || n >= *rem) { *rem = 0; return; }
  *pp += n; *rem -= n;
}

static void append_fmt(char **pp, int *rem, const char *fmt, ...) {
  if (*rem <= 0) return;
  va_list ap;
  va_start(ap, fmt);
  int n = vsnprintf(*pp, *rem, fmt, ap);
  va_end(ap);
  if (n <= 0 || n >= *rem) { *rem = 0; return; }
  *pp += n; *rem -= n;
}

// 从寄存器结构体中随机选取寄存器名，并配上表达式引导符号 & 
static void append_reg_from_white(char **pp, int *rem) {
  if (*rem <= 0) return;
  const char *r = regs_name[rand() % regs_name_n];
  if (r[0] == '$'){
    r = r + 1;
  } 
  append_fmt(pp, rem, "$%s", r);
}

  // 操作数选择
static void gen_operand(char **pp, int *rem, int depth) {
  if (*rem <= 0) return;
  (void)depth;
  int idx = pool_pick(&op_pool);                  // 返回值为随机从操作数池中选择对应的操作数类型标志  
  const char *kind = op_pool.items[idx].name;     // 根据返回值参数选定操作数池中的对应的字符串名称
  // 可以利用op_items[]，使用switch实现，直接避开字符串对比
  if (strcmp(kind, "dec") == 0) {
    append_fmt(pp, rem, "%d", rand() % 1000);
  } else if (strcmp(kind, "hex") == 0) {
    append_fmt(pp, rem, "0x%X", rand() % 0x10000);
  } else if (strcmp(kind, "reg") == 0) {
    append_reg_from_white(pp, rem);
  } else {
    append_fmt(pp, rem, "%d", rand() % 1000);
  }
}

static void gen_expr_rec(char **pp, int *rem, int depth) {
  if (*rem <= 0) return;
  // 确定当前层子表达式个数(第一层至少一个)，
  // 子表达式的操作符：atoms -1
  int atoms = 1 + rand() % cfg.max_atoms;
  // depth>0 则存在递归嵌套，将当前第一个操作数以15%的概率成为子表达式，即用()括起来。
  if (depth > 0 && (rand() % 100) < 15) {
    append_str(pp, rem, "(");
    gen_expr_rec(pp, rem, depth - 1);
    append_str(pp, rem, ")");
  } else {
    gen_operand(pp, rem, depth);
  }
  for (int i = 1; i < atoms && *rem > 0; i++) {
    int opi = pool_pick(&al_pool);
    const char *op = al_pool.items[opi].name;
    append_fmt(pp, rem, " %s ", op);
    if (depth > 0 && (rand() % 100) < 15) {
      append_str(pp, rem, "(");
      gen_expr_rec(pp, rem, depth - 1);
      append_str(pp, rem, ")");
    } else {
      gen_operand(pp, rem, depth);
    }
  }
}



static void gen_rand_expr() {
  char tmp[4096];             // 缓冲区
  int i = 5;
  while (i--) {
    tmp[0] = '\0';
    char *p = tmp;
    int rem = sizeof(tmp);
    gen_expr_rec(&p, &rem, cfg.max_depth);
    if (rem <= 0 || tmp[0] == '\0') continue;
   
    int bal = 0, ok = 1;
    // 循环条件下：字符串指针，作为结束条件时：*q 表示当*q='\0'结束
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


int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  // 操作数+操作符权重池初始化
  pool_prepare(&op_pool);
  pool_prepare(&al_pool);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }

  int i;
  FILE *out = fopen("input", "w");
  if (!out) {
    perror("fopen input");
    return 1;
  }
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();
    fprintf(out, "%s\n", buf);
    if (i < 10) {
      fprintf(stderr, "%s\n", buf);
    }
  }

  fclose(out);
  return 0;
}
