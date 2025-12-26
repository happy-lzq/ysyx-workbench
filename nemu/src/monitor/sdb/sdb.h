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

#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>
#define NR_WP 32
// 链表节点定义
typedef struct watchpoint {
  int NO;
  char exp[256];
  word_t prev_value;
  struct watchpoint *next;
} WP;

typedef struct ChangedInfo{
    int NO;
    char expr[256];
    word_t old_value;
    word_t new_value;
} ChangedInfo;

typedef struct wp_list
{
  WP* head;
  WP* tail;
  int size;
}wp_list;
extern ChangedInfo changed_list[NR_WP];
extern wp_list used_list;
extern wp_list free_list;
extern WP wp_pool[NR_WP];
int eval_input_file(const char *path) ;

void init_wp_pool();
word_t expr(char *e, bool *success,bool *hex);
void inserttail(wp_list *l, WP* wp);
void watchpoint_list(wp_list *l);
void new_wp(char* exp);
void unlink_wp(wp_list *l,int no);
int check_watchpoint(wp_list *l);
#endif
