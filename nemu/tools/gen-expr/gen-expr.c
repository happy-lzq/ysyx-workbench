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

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u 0x%%X\", result, result); "
"  return 0; "
"}";

/* forward */
static void gen_rand_expr();



typedef struct {
  int max_depth;
  int max_atoms;
  int max_length;
} complexity_t;

typedef struct {
  const char *name; // 标识，如 "dec","hex","reg","subexpr" 或 "+","-","*","/"
  int weight;       // 相对权重
} weight_item_t;

typedef struct {
  weight_item_t *items;
  int n;
  int total;        // 预计算权重和
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
// 操作数以及权重
static weight_item_t al_items[]= {
  {"+",30}, {"-",30}, {"*",25}, {"/",15}
};
static weight_pool_t al_pool = { al_items, sizeof(al_items)/sizeof(al_items[0]), 0 };

// 权重池准备与抽样
static void pool_prepare(weight_pool_t *p) {
  int sum = 0;
  for (int i = 0; i < p->n; i++) {
    if (p->items[i].weight > 0) sum += p->items[i].weight;
  }
  p->total = (sum > 0) ? sum : 1;
}

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

/* append register from white list, always output single leading '$' */
static void append_reg_from_white(char **pp, int *rem) {
  if (*rem <= 0) return;
  const char *r = regs_name[rand() % regs_name_n];
  while (*r == '$') r++;
  /* validate name */
  int ok = 0;
  if (isdigit((unsigned char)r[0])) {
    ok = 1;
    for (const char *p = r; *p; p++) if (!isdigit((unsigned char)*p)) { ok = 0; break; }
  } else if (isalpha((unsigned char)r[0])) {
    ok = 1;
    for (const char *p = r + 1; *p; p++) if (!isalnum((unsigned char)*p)) { ok = 0; break; }
  }
  if (!ok) r = "0";
  append_fmt(pp, rem, "$%s", r);
}

/* forward declarations for recursive generators */
static void gen_expr_rec(char **pp, int *rem, int depth);

/* generate a single operand */
static void gen_operand(char **pp, int *rem, int depth) {
  if (*rem <= 0) return;
  (void)depth;
  int idx = pool_pick(&op_pool);
  const char *kind = op_pool.items[idx].name;
  if (strcmp(kind, "dec") == 0) {
    append_fmt(pp, rem, "%d", rand() % 1000);
  } else if (strcmp(kind, "hex") == 0) {
    append_fmt(pp, rem, "0x%X", rand() % 0x10000);
  } else if (strcmp(kind, "reg") == 0) {
    append_reg_from_white(pp, rem);
  } else {
    /* fallback to decimal */
    append_fmt(pp, rem, "%d", rand() % 1000);
  }
}

/* generate an operand that is guaranteed not to be zero (avoid 0 literal and $0 reg)
   used for right-hand side of division to reduce chance of division-by-zero */
static void gen_operand_nonzero(char **pp, int *rem, int depth) {
  if (*rem <= 0) return;
  (void)depth;
  /* try picking kinds but ensure non-zero output */
  for (int tries = 0; tries < 10; tries++) {
    int idx = pool_pick(&op_pool);
    const char *kind = op_pool.items[idx].name;
    if (strcmp(kind, "dec") == 0) {
      int v = (rand() % 999) + 1; /* 1..999 */
      append_fmt(pp, rem, "%d", v);
      return;
    } else if (strcmp(kind, "hex") == 0) {
      int v = (rand() % 0xFFFF) + 1; /* 1..0xFFFF */
      append_fmt(pp, rem, "0x%X", v);
      return;
    } else if (strcmp(kind, "reg") == 0) {
      /* pick a non-$0 register */
      const char *r;
      int guard = 0;
      do {
        r = regs_name[rand() % regs_name_n];
        guard++;
      } while (strcmp(r, "$0") == 0 && guard < 20);
      while (*r == '$') r++;
      append_fmt(pp, rem, "$%s", r);
      return;
    }
  }
  /* fallback */
  append_fmt(pp, rem, "1");
}

/* generate expression at given depth */
static void gen_expr_rec(char **pp, int *rem, int depth) {
  if (*rem <= 0) return;
  int atoms = 1 + rand() % cfg.max_atoms;
  /* first operand */
  if (depth > 0 && (rand() % 100) < 15) {
    /* with small probability, make first operand a subexpr */
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
    /* right operand */
    if (strcmp(op, "/") == 0) {
      /* avoid generating a zero RHS: don't use subexprs here, pick non-zero operand */
      gen_operand_nonzero(pp, rem, depth);
    } else {
      if (depth > 0 && (rand() % 100) < 15) {
        append_str(pp, rem, "(");
        gen_expr_rec(pp, rem, depth - 1);
        append_str(pp, rem, ")");
      } else {
        gen_operand(pp, rem, depth);
      }
    }
  }
}

/* top-level generator: try a few times then fallback */
static void gen_rand_expr() {
  char tmp[4096];
  int tries = 5;
  while (tries--) {
    tmp[0] = '\0';
    char *p = tmp;
    int rem = sizeof(tmp);
    gen_expr_rec(&p, &rem, cfg.max_depth);
    if (rem <= 0 || tmp[0] == '\0') continue;
    /* simple bracket balance check */
    int bal = 0, ok = 1;
    for (char *q = tmp; *q; q++) {
      if (*q == '(') bal++;
      else if (*q == ')') { if (bal == 0) { ok = 0; break; } bal--; }
    }
    if (!ok || bal != 0) continue;
    /* copy to global buf */
    snprintf(buf, sizeof(buf), "%s", tmp);
    return;
  }
  snprintf(buf, sizeof(buf), "1+1");
}

/* sanitize expression for embedding into C source
   replace $<name> with a numeric literal '1' to avoid undeclared identifiers
*/
static void sanitize_for_c(const char *src, char *dst, int dstsz) {
  const char *p = src;
  char *q = dst;
  int rem = dstsz;
  while (*p && rem > 1) {
    if (*p == '$') {
      /* skip $ and following alnum */
      p++;
      while (*p && isalnum((unsigned char)*p)) p++;
      /* insert safe non-zero literal */
      int n = snprintf(q, rem, "1");
      if (n <= 0 || n >= rem) break;
      q += n; rem -= n;
    } else {
      int n = snprintf(q, rem, "%c", *p);
      if (n <= 0 || n >= rem) break;
      q += n; rem -= n;
      p++;
    }
  }
  *q = '\0';
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  /* prepare weight pools */
  pool_prepare(&op_pool);
  pool_prepare(&al_pool);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  /* write all generated expressions to local file `input` (one per line)
     and print only the first 10 to stdout */
  FILE *out = fopen("input", "w");
  if (!out) {
    perror("fopen input");
    return 1;
  }

  /* avoid unused-variable warnings for code_buf/code_format */
  (void)code_buf; (void)code_format;

  for (i = 0; i < loop; i ++) {
    gen_rand_expr();
    char c_expr[sizeof(buf)];
    /* keep calling sanitize_for_c to mark it as used (silence -Werror=unused-function)
       but we still write the original expression containing `$` to the file. */
    sanitize_for_c(buf, c_expr, sizeof(c_expr));

    fprintf(out, "%s\n", buf);
    if (i < 10) {
      fprintf(stderr, "%s\n", buf);
    }
  }

  fclose(out);
  return 0;
}
