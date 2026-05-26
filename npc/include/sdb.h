#ifndef __NPC_SDB_H__
#define __NPC_SDB_H__

#include <npc.h>
#include <getopt.h>
#include <regex.h>

// ==================== 通用宏 ====================
#define NR_WP 32
#define ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))

// ==================== SDB 用到的全局变量（在 main.cpp/monitor.cpp 定义） ====================
extern const char *img_file;
extern const char *diff_so_file;
void init_diff_log(const char *path);

// ==================== SDB API ====================
int parse_agrs(int argc, char *argv[]);

// ==================== 监视点数据结构 ====================
typedef struct watchpoint {
  int NO;
  char exp[256];
  word_t prev_value;
  struct watchpoint *next;
} WP;

typedef struct ChangedInfo {
    int NO;
    char expr[256];
    word_t old_value;
    word_t new_value;
} ChangedInfo;

typedef struct wp_list {
  WP* head;
  WP* tail;
  int size;
} wp_list;

extern ChangedInfo changed_list[NR_WP];
extern wp_list used_list;
extern wp_list free_list;
extern WP wp_pool[NR_WP];

// ==================== 监视点 API ====================
void init_wp_pool();
void new_wp(char* exp);
void unlink_wp(wp_list *l, int no);
void watchpoint_list(wp_list *l);
int  check_watchpoint(wp_list *l);
void inserttail(wp_list *l, WP* wp);
void unlink_init(WP* wp);
WP*  fetch_wp(wp_list *l);

// ==================== 表达式求值 API ====================
word_t expr(char *e, bool *success, bool *hex);
int eval_input_file(const char *path);

// ====================  sdb ================================
void vaddr_write(vaddr_t addr, int len, word_t data) ;
word_t vaddr_read(vaddr_t addr, int len) ;
word_t isa_reg_str2val(const char *s, bool *success) ;
void sdb_mainloop();
void init_sdb();
void init_regex();
void sdb_set_batch_mode();
void isa_reg_display() ;

#endif