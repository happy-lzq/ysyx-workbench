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

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <isa.h>
#include <regex.h>
#include <stdbool.h>
// #include </home/l/ysyx/ysyx-workbench/nemu/include/common.h>


//========================= Token 类型 ====================================//
enum {
  TK_NOTYPE = 256, TK_EQ,TK_NUM

  /* TODO: Add more token types */

};

//===========Token 结构体数组定义 正则规则（每个数组元素由结构体成员构成） ==================//
static struct rule {
  const char *regex;
  int token_type;
} rules[] = {
  {" +", TK_NOTYPE},    // 空格      256
  {"==",TK_EQ},         // 等于      257
  {"[0-9]+", TK_NUM},   // 十进制整数 
  {"\\+", '+'},         // 加号      43
  {"-", '-'},           // 减号      45
  {"\\*", '*'},         // 乘号      42
  {"/", '/'},           // 除号      47
  {"\\(", '('},         // 左括号    40
  {"\\)", ')'}          // 右括号    41
  
};
#define NR_REGEX ARRLEN(rules)    // 自动计算rules结构体数组元素个数

//============== 利用正则表达式库函数进行正则表达式字符串编译初始化 ==============================//
static regex_t re[NR_REGEX] = {}; // re[] 是一个regex_t 结构体数组，每个 regex_t 结构体代表一个已编译的正则表达式。
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);  // REG_EXENDED扩展正则
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

// ================================ 正则表达式词法分析 ===============================//
typedef struct token {
  int type;
  char str[32];
} Token;

// 结构体数组tokens 用于存放词法分析得到的所有 token（记号） __attribute__((used))：防止编译器因“未使用”而优化掉该变量。
static Token tokens[32] __attribute__((used)) = {};       
static int nr_token __attribute__((used))  = 0;           // 记录当前已经识别出的 token 数量

static bool make_token(char *e) {
  int position = 0;
  int i;
  // regmatch_t 结构体用于存放正则表达式匹配结果的位置
  regmatch_t pmatch;      
  nr_token = 0;
  // e[position] 实际上等价于 *(e + position)，即“从 e 指向的起始地址偏移 position 个字节后的内容”。
  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {

          default: TODO();
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  TODO();

  return 0;
}
