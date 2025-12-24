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

#include "sdb.h"

#define NR_WP 32

// 链表节点定义
typedef struct watchpoint {
  int NO;
  char expr[256];
  word_t prev_value;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */

} WP;

typedef struct wp_list
{
  WP* head;
  WP* tail;
  int size;
}wp_list;
static wp_list used_list = {NULL,NULL,0};
static wp_list free_list = {NULL,NULL,0};
// 利用结构体数组来表示监视点池，监视点池由两个链表组成：一个是空闲链表free，一个是使用中链表head
static WP wp_pool[NR_WP] = {};

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].expr[0] = '\0';  
    wp_pool[i].prev_value = 0;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }
  // free_list 未使用链表包含结构体数组监视池所有节点
  free_list.head = &wp_pool[0];
  free_list.tail = &wp_pool[NR_WP - 1];
  free_list.size = NR_WP;
  // used_list 初始化为空
  used_list.head = NULL;
  used_list.tail = NULL;
  used_list.size = 0;

}

/* TODO: Implement the functionality of watchpoint */

