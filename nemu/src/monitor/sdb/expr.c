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



//========================= Token 类型 ====================================//
enum {
  TK_NOTYPE = 256, TK_EQ,TK_NUM,TK_REG

  /* TODO: Add more token types */

};

//===========Token 结构体数组定义 正则规则（每个数组元素由结构体成员构成） ==================//
static struct rule {
  const char *regex;
  int token_type;
} rules[] = {
  {" +", TK_NOTYPE},                      // 空格       256
  {"==",TK_EQ},                           // 等于       257
  {"[0-9]+", TK_NUM},                     // 十进制整数  258
  {"\\$[a-z][0-9]+",TK_REG},                // 寄存器     259
  {"\\$[a-z]{2}",TK_REG},  
  {"\\$[0-9]+",TK_REG}, 
  {"\\+", '+'},                           // 加号       43
  {"-", '-'},                             // 减号       45
  {"\\*", '*'},                           // 乘号       42
  {"/", '/'},                             // 除号       47
  {"\\(", '('},                           // 左括号     40
  {"\\)", ')'}                            // 右括号     41

};
#define NR_REGEX ARRLEN(rules)    // 自动计算rules结构体数组元素个数

//============== 编译 rules[] Tokens 与正则表达式一一对应 ==============================//
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
  regmatch_t pmatch;      // regmatch_t 结构体用于存放正则表达式匹配结果的位置，so: 目标字符串中的起始位置，eo:目标字符串的结束位置
  nr_token = 0;           // 用于记录有效token_type
  // e[position] 实际上等价于 *(e + position)，即“从 e 指向的起始地址偏移 position 个字节后的内容”。
  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      // 对于 pmatch 而言：目标字符串为：e+position，利用position，substr_len，rm_eo 来记录每一个正确匹配的token的长度
// 用 rules[] 结构体数组里定义的每个正则表达式规则，依次去匹配输入字符串的当前位置，只要某个规则能从当前位置开始匹配成功，就把它当作一个 token
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;
        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;
// 用正则表达式识别出一个 token（记号），将其信息（类型和内容）保存到 tokens 数组里，并维护 nr_token 计数。
        switch (rules[i].token_type) {
          case TK_NOTYPE: // 空格
            // 空格，不保存，跳过
            break;
          default: 
            tokens[nr_token].type = rules[i].token_type;
            int copy_len = substr_len < 31 ? substr_len : 31;
            strncpy(tokens[nr_token].str,substr_start,copy_len);
            tokens[nr_token].str[copy_len] = '\0';
            nr_token++;
            break;
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

// ============================== 递归求值 =======================================//
// 检查tokens[]表达式始末是否有括号，以及括号是否合法
int check_parentheses(int l, int r) {
  // 表达式最边侧主要有一个不是括号对，直接返回-1表达整个表达式肯定不是被()括起来
  if (tokens[l].type != '(' || tokens[r].type != ')') return -1;
  int paren_level = 0;
  // 表达式左右两边有括号对，但是要去检验是不是将整个表达式括起来，防止：（）+（）系列
  // 进入for循环则表达式左侧一定为'()'，那么需要在检查到最左侧')' 之前，l'('闭合，说明表达式整体未被扩 
  // 所以在遇到一个')'就立即检查是否闭合
  for (int i = l + 1; i < r; i++) {
    if (tokens[i].type == '(') paren_level++;       // (())+()
    else if (tokens[i].type == ')') {
      if (paren_level == 0) return -1; 
        paren_level--;
    }
  }
  return paren_level == 0 ? 0 : -1;
}
// 利用运算法规则寻找主运算法即是最低等级运算符位置
int get_priortiy(int type){
  switch (type){
    case TK_EQ : return 0;
    case   '+' : return 1;
    case   '-' : return 1;
    case   '*' : return 2;
    case   '/' : return 2;
    default    : return 20;   // 非运算符
  }
}
int find_main_operator(int l, int r,bool *success){
  int paren_level = 0;
  int min_priority = 20;
  int op = -1;
  for ( int i = l; i <= r; i++){
    if (tokens[i].type == '('){
      paren_level++;
    } else if (tokens[i].type == ')'){
      paren_level--;
    } else if (paren_level == 0){
  // 逐个扫描获取非括号内的运算符等级
      int pri = get_priortiy(tokens[i].type);
  // 设计逻辑：逐个比较等级：等级低的覆盖等级大的，同一等级的，位于表达式更后面的覆盖前面的作为主运算符
      if (pri < min_priority || (pri == min_priority && i > op)){
        min_priority = pri;
        op = i;
      }
    }
  }

  if (op == -1) {
    *success = false;
    printf("No main operator found\n");
    return 0;
  }
  return op;
}
// 递归处理表达式求值
word_t eval(int l,int r,bool *success){
  word_t val1,val2;
  if (l > r){
    printf("Bad expression\n");
    *success =false;
    return 0;
  } else if (l == r){
    // l==r ：数字类型，寄存器类型
    switch (tokens[l].type){
      case TK_NUM : return atoi(tokens[l].str);
      case TK_REG : return isa_reg_str2val(tokens[l].str,success);
      default: *success = false; return 0;
    }
  } else if (check_parentheses(l,r) == 0){
    // 括号对检查合法，且表达式两边都存在括号，去除括号再次递归
    return eval(l+1,r-1,success);
  } else {                                                                                       
    int op = find_main_operator(l,r,success);
    val1 = eval(l,op-1,success);
    val2 = eval(op+1,r,success);
    switch (tokens[op].type) {
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': return val1 / val2;
      case TK_EQ: return val1 == val2;
      // 其它类型如 TK_NUM、TK_REG、括号等在递归出口已处理
      default: assert(0); // 未知类型直接报错
    }

  }

}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  } else{
    *success = true;
  return eval(0,nr_token-1,success);
  }
}
