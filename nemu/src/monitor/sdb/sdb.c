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

#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <utils.h>
#include <memory/vaddr.h> 
#include "sdb.h"


// 自动计算数组元素个数，动态计算cmd_table[]中的指令数
#define NR_CMD ARRLEN(cmd_table)
// 定义NEMU进行交互模式，显示(nemu)提示符
static int is_batch_mode = false;

//============================ Command declarations ============================//
void init_regex();
void init_wp_pool(); 
static int cmd_help(char *args);
static int cmd_c(char *args);
static int cmd_q(char *args);
static int cmd_si(char *args);
static int cmd_info(char *args);
static int cmd_x(char *args);

//================================ Command table ================================//
static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display information about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  {"si", "Step execute",cmd_si},
  {"info","Generic program status (r: register, w: watchpoint)", cmd_info },
  {"x","read memery from addr (x n 0x80000000)",cmd_x}
  /* TODO: Add more commands */

};

  //======================== 用户界面交互处理 ====================================//
	// rl_gets 使用 readline 库读取用户输入的一行指令
static char* rl_gets() {
  static char *line_read = NULL;
  if (line_read) {
    free(line_read);
    line_read = NULL;
  }
  line_read = readline("(nemu) ");
	// 检查指针是否有效，同时在检查指针有效情况下首字母不为非空'\0'，防止上一指令输入回车后未清除
  if (line_read && *line_read) {								
    add_history(line_read);
  }
  return line_read;
}

//============================= Command functions =============================//
  // cmd_help 显示帮助信息的命令处理函数
static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

  // cmd_c 继续执行程序的命令处理函数
static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

  // cmd_q 退出程序的命令处理函数
static int cmd_q(char *args) {

  nemu_state.state = NEMU_QUIT;
  return -1;
}

  // cmd_si 单步执行指定指令数的命令处理函数
static int cmd_si(char *args) {
  char *endptr;
  int n;
  if (args==NULL){
    n=1;
  } else{
    n =strtoul(args,&endptr,10);
    if (endptr == args || *endptr !='\0' || n <= 0){
      printf("Invalid number of instructions: %s\n", args);
      return 0;
    }
  }

  cpu_exec(n);  // 执行n条指令
  return 0;
}

  // cmd_info 打印寄存器+监控点信息
static int cmd_info(char *args){
  char *arg = strtok (NULL," ");
  if (arg == NULL){
    printf("Usage: info [r/w]\n");
    return 0;
  }
  if (strcmp(arg,"r") == 0){
    isa_reg_display();
  } else if (strcmp(arg,"w") == 0){

    printf("Watchpoint info is not implemented yet.\n");
  } else{
    printf("Unknown argument '%s'. Usage: info [r|w]\n", args);
    return 0;
  }
  
  return 0;
}

  // cmd_x n [0x80000000] 
  static int cmd_x(char *args){
    if (args == NULL){
      printf("Usage: x N EXPR\n");
      return 0;
    }
    
    char *n_arg = strtok(NULL," ");
    if (n_arg == NULL){
      printf("Missing argument N\n");
      return 0;
    }
    int n = atoi(n_arg);

    char *expr_arg = strtok(NULL," ");
    if (expr_arg == NULL) {
      printf("Missing argument EXPR\n");
      return 0;
    }

    uint32_t addr;
    uint32_t data;
    sscanf(expr_arg,"%x",&addr);
    printf("Memory dump at 0x%08x for %d words:\n", addr, n);
    for (int i = 0; i < n; i++){
      data = vaddr_read(addr,sizeof(int));
      printf("0x%08x: 0x%08x\n", addr, data);
      addr += 4;
    }
    return 0;
  }

//============================= SDB main loop and initialization =============================//
void sdb_set_batch_mode() {
  is_batch_mode = true;
}
void sdb_mainloop() {
  // 持续运行直到指令结束或用户退出
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }
  // 用户交互循环处理输入指令模式
  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        // readline 输入与cmd_table匹配后回调函数执行，进入对应指令操作
        if (cmd_table[i].handler(args) < 0) { 
          return ;
        }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
