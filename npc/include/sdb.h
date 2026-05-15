#ifndef __NPC_SDB_H__
#define __NPC_SDB_H__

#include <npc.h>
#include <getopt.h>

// ==================== SDB 用到的全局变量（在 main.cpp/monitor.cpp 定义） ====================
extern const char *img_file;
extern const char *diff_so_file;

// ==================== SDB API ====================
int parse_agrs(int argc, char *argv[]);

#endif