/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER expESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include "sdb.h"
// ======================== 数据定义与初始化 =========================================//

WP wp_pool[NR_WP] = {};
wp_list used_list = {NULL, NULL, 0};
wp_list free_list = {NULL, NULL, 0};

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].exp[0] = '\0';  
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
// ==================== 从free_list 取出一个节点用于存放监视点数据，并尾插入used_list ============
WP* fetch_wp(wp_list *l){
  if (!l || !l->head){
    return NULL;
  }
  WP* wp_curr = l->head;
  l->head = wp_curr->next;
  wp_curr->next = NULL;
  if (wp_curr == l->tail){
    l->tail = NULL;
  }
  l->size--;
  return wp_curr;
}

void new_wp(char* exp){
  bool success = false;
  bool hex = false;
  word_t expr_value = expr(exp,&success,&hex);
  if (success){
    WP* wp = fetch_wp(&free_list);
    if (! wp){
      printf("watchpoint list is empty, cannot pop wp\n");
      return ;
    }
    strncpy(wp->exp,exp,sizeof(wp->exp)-1);
    wp->exp[sizeof(wp->exp)-1] = '\0';
    wp->prev_value = expr_value;
    inserttail(&used_list,wp);
  }else {
    printf("Bad expression.\n");
    return ; 
  }
}

// ============== 从used_list unlink wp_curr and inserttail free_list ===================//
void inserttail(wp_list *l, WP* wp){
  if (!wp){
    printf("cannot add NULL watchpoint to list\n");
    return ;
  }
  if (l->tail == NULL){
    l->head = wp;
    l->tail = wp;
  } else {
    l->tail->next = wp;
    l->tail = wp;
  }
  l->size++;
  return ;
}
void unlink_init(WP* wp){
  wp->exp[0] = '\0';
  wp->prev_value = 0;
  wp->next = NULL;
}

void unlink_wp(wp_list *l,int no){
  if (!l || !l->head){
    printf("error: blank list !\n");
    return ;
  }
  WP* wp_curr = l->head;
  WP* wp_prev = NULL;
  // 1. 寻找节点 (单次遍历即可找到 curr 和 prev)
  while (wp_curr != NULL && wp_curr->NO != no){
    wp_prev = wp_curr;
    wp_curr = wp_curr->next;
  }
  // 2. 检查是否匹配成功
  if (wp_curr == NULL){
    printf("Watchpoint NO.%d not found!\n", no);
    return;
  }
  // 3. unlink 节点 同时维护used_list 头指针
  if (wp_prev == NULL){
    l->head = wp_curr->next;
  } else {
    wp_prev->next = wp_curr->next;
  }
  // 4. 维护used_list 尾指针 匹配的节点是唯一头节点，或者used_list多节点的尾节点
  if (wp_curr == l->tail){
    l->tail = wp_prev;
  }
  // 5. 统一处理数目
  l->size--;
  unlink_init(wp_curr);
  inserttail(&free_list,wp_curr);
}

// ================================= 监视点查看 ==============================================//
/*                                  监视点全局遍历                                             */
void watchpoint_list(wp_list * l){
  if (!l || !l->head){
    printf("error: blank list !\n");
    return ;
  }
  printf("NO  Address/Expression                        Value\n");
  WP* curr = l->head;
  while (curr != NULL)
  {
    printf("%-3d %-22s                0x%08x\n", curr->NO, curr->exp, curr->prev_value);
    curr = curr->next;
  }
}

/*                                  监视点 add                                             */

/* TODO: Implement the functionality of watchpoint */

