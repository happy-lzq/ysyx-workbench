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
#include <common.h>
#include <isa.h>
#include <regex.h>
#include <stdbool.h>
#include <memory/vaddr.h> 
#include <cpu/cpu.h>
//========================= Token 类型 ====================================//
enum {
  TK_NOTYPE = 256, TK_EQ,TK_16NUM,TK_NUM,TK_REG,
  TK_NOTEQ,TK_AND, TK_OR,TK_VAR,TK_DEREF

  /* TODO: Add more token types */

};

//===========Token 结构体数组定义 正则规则（每个数组元素由结构体成员构成） ==================//
static struct rule {
  const char *regex;
  int token_type;
} rules[] = {
  {" +", TK_NOTYPE},                      // 空格       256
  {"==",TK_EQ},                           // 等于       257
  {"0[xX][0-9a-fA-F]+",TK_16NUM},         // 16进制     258
  {"[0-9]+", TK_NUM},                     // 十进制整数  259
  {"\\$([A-Za-z][A-Za-z0-9]*|[0-9]+)", TK_REG},        // 寄存器     260
  {"!=", TK_NOTEQ},                       // 不等于     261 
  {"&&",TK_AND},                          // 逻辑与     264 
  {"\\|\\|",TK_OR},                       // 逻辑或     265 
  {"[a-zA-Z_][a-zA-Z0-9_]*",TK_VAR},      // 变量名     266
  {"\\+", '+'},                            // 加号       43
  {"-", '-'},                              // 减号       45
  {"\\*", '*'},                            // 乘号       42
  {"/", '/'},                              // 除号       47
  {"\\(", '('},                            // 左括号     40
  {"\\)", ')'}                             // 右括号     41

};
#define NR_REGEX ARRLEN(rules)    // 自动计算rules结构体数组元素个数
// ==================================自动解析字符串数值================================//
static word_t parse_num(const char *s, bool *success){
  char *endptr = NULL;
  word_t val = strtoull(s,&endptr,0);
  if (endptr == s || *endptr != '\0' ){
    printf("自动解析数值转化失败！请检查输入");
    *success = false;
    return 0;
  } 
  return val;
}
// ===================================指针对地址解引用=================================//
// static word_t get_pointer_value(const char *s, bool *success){
//   word_t data;
//   word_t addr = parse_num(s,success);
//   if (addr < 0x80000000 || addr >=0xffffffff){
//     printf("输入解析地址不在地址范围内！请检查输入");
//     *success = false;
//     return 0;
//   }
//   return data = vaddr_read(addr,sizeof(int));
// }

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
  char str[256];
} Token;

// 结构体数组tokens 用于存放词法分析得到的所有 token（记号）
// 增加容量并在词法分析时检查边界，防止长表达式导致缓冲区溢出
#define MAX_TOKENS 256
static Token tokens[MAX_TOKENS] __attribute__((used)) = {};
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
            if (nr_token >= MAX_TOKENS) {
              printf("too many tokens: exceed %d\n", MAX_TOKENS);
              return false;
            }
            tokens[nr_token].type = rules[i].token_type;
            int copy_len = substr_len < (int)sizeof(tokens[nr_token].str) - 1 ? substr_len : (int)sizeof(tokens[nr_token].str) - 1;
            strncpy(tokens[nr_token].str, substr_start, copy_len);
            
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
  for ( i = 0; i < nr_token; i++){
 // 如果 * 出现在表达式开头，或出现在另一个运算符之后，或出现在左括号 ( 之后，则它是 unary deref（TK_DEREF）。
    if (tokens[i].type == '*'){
      if (i ==0                       ||
        tokens[i-1].type == '+'       ||
        tokens[i-1].type == '-'       ||
        tokens[i-1].type == '*'       ||
        tokens[i-1].type == '/'       ||
        tokens[i-1].type == TK_EQ     ||
        tokens[i-1].type == TK_NOTEQ  ||
        tokens[i-1].type == TK_AND    ||
        tokens[i-1].type == TK_OR     ||
        tokens[i-1].type == '('       ){
          tokens[i].type = TK_DEREF;    
      }
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
  /* 新约定：返回值越小表示优先级越高（binding 越强），越大表示优先级越低（更容易成为主运算符）。
     映射基于常见 C 运算符优先级（此处列出当前实现需要的运算符等级）。
     小数值 = 高优先级（先计算）；大数值 = 低优先级（更可能被选为主运算符）。
  */
  switch (type){
    case TK_DEREF : return 1;   /* 一元解引用/一元运算：最高优先级（最强绑定） */
    case   '*'    : return 4;
    case   '/'    : return 4;
    case   '+'    : return 5;
    case   '-'    : return 5;
    case TK_EQ    : return 8;
    case TK_NOTEQ : return 9;
    case TK_AND   : return 12;   /* 逻辑与 */
    case TK_OR    : return 13;   /* 逻辑或，最低优先级（最弱绑定） */
    default       : return -1;   /* 非运算符，返回 -1 表示不是运算符，find_main_operator 会忽略 */
  }
}

int find_main_operator(int l, int r,bool *success){
  int paren_level = 0;
  int max_priority = -1;
  int op = -1;
  for ( int i = l; i <= r; i++){
    if (tokens[i].type == '('){
      paren_level++;
    } else if (tokens[i].type == ')'){
      paren_level--;
    } else if (paren_level == 0){
      /* 逐个扫描获取非括号内的运算符等级。
         新逻辑：选取区间内优先级数值最大的运算符作为主运算符（数值越大＝优先级越低＝成为主运算符的可能性越大）。*/
      int pri = get_priortiy(tokens[i].type);
      /* 设计逻辑：当 pri > max_priority 时更新；若 pri==max_priority，保留右侧靠后的运算符（i>op），
         以保持左结合的默认行为（与之前实现一致）。 */
      if (pri > max_priority || (pri == max_priority && i > op)) {
        max_priority = pri;
        op = i;
      }
    }
  }
  /* op 未变 找不到主運算符则算法表达式错误，传出success = false  */
  if (op == -1) {
    *success = false;
    printf("No main operator found\n");
    return 0;
  }
  return op;
}
// 递归处理表达式求值
word_t eval(int l,int r,bool *success,bool *hex){
  word_t val1,val2;
  if (l > r){
    printf("Bad expression\n");
    *success =false;
    return 0;
  } else if (l == r){
    // l==r ：数字类型，寄存器类型
    switch (tokens[l].type){
      case TK_16NUM : return parse_num(tokens[l].str,success);
      case TK_NUM   : return parse_num(tokens[l].str,success);
      case TK_REG   : *hex = true ;return isa_reg_str2val(tokens[l].str,success); 
      default: 
      *success = false; return 0;
    }
  } else if (check_parentheses(l,r) == 0){
    // 括号对检查合法，且表达式两边都存在括号，去除括号再次递归
      return eval(l+1,r-1,success,hex);
  } else {        
    // 表达式非整体被括号，进入找主运算符拆分两个表达式重复递归                                                                          
      int op = find_main_operator(l,r,success);
      if (tokens[op].type == TK_DEREF){
        word_t addr = eval(l+1,r,success,hex);
        return vaddr_read(addr,sizeof(int));
      } else {
      // 利用*success 检查主运算符
      if (*success == false) return 0;
      val1 = eval(l,op-1,success,hex);
      val2 = eval(op+1,r,success,hex);
      switch (tokens[op].type) {
        case '+': return val1 + val2;
        case '-': return val1 - val2;
        case '*': return val1 * val2;
        case TK_EQ    : return (word_t) (val1 == val2 ? 1 : 0); 
        case TK_NOTEQ : return (word_t) (val1 != val2 ? 1 : 0); 
        case TK_AND   : return (word_t) (val1 && val2 );
        case TK_OR    : return (word_t) (val1 || val2 );
        case '/':
          if (val2 == 0) {
            printf("division by zero\n");
            *success = false;
            return 0;
          }
          return val1 / val2;
        // 其它类型如 TK_NUM、TK_REG、括号等在递归出口已处理
        default: assert(0); // 未知类型直接报错
      }
     }
    }
  }

word_t expr(char *e, bool *success, bool *hex) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  } else{
  // 检查表达式是否存在16进制token
    for (int i = 0; i < nr_token; i++){
      if (tokens[i].type == TK_16NUM){
        *hex = true;
        break;
      } 
    } 
    *success = true;
  // 进入表达式求值递归求值处理之前，先将success设置为ture,eval求值过程中如果有错误提前返回false 结束当前求值
  return eval(0,nr_token-1,success,hex);
  }
}


int eval_input_file(const char *path) {
  if (!path) return -1;
  FILE *f = fopen(path, "r");
  if (!f) {
    return -1; // 打不开文件，caller 可决定是否报错
  }

  char line[4096];
  while (fgets(line, sizeof(line), f)) {
    /* strip newline & leading/trailing whitespace */
    char *p = line;
    while (*p && (*p == ' ' || *p == '\t')) p++; /* skip leading ws */
    char *end = p + strlen(p);
    while (end > p && (end[-1] == '\n' || end[-1] == '\r' || end[-1] == ' ' || end[-1] == '\t')) end--;
    *end = '\0';
    if (*p == '\0') continue;

    bool success = false;
    bool ishex = false;
    printf("\ncurrent expr: %s\n", p);
    word_t val = expr(p, &success, &ishex);
    if (success) {
      if (ishex) printf("value: 0x%08" PRIx32 "\n", (uint32_t)val);
      else printf("value: %u\n", (unsigned)val);
    } else {
      printf("Bad expression: %s\n", p);
    }
  }
  fclose(f);
  return 0 ;
}