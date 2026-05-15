#ifndef __NPC_H__
#define __NPC_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

#define DIFFTEST_REG_SIZE (sizeof(uint32_t) * 33)  // 32 GPR + 1 PC
#define RESET_VECTOR 0x80000000 
#define PMEM_SIZE (128*1024*1024)

typedef uint32_t paddr_t;  

typedef struct {
    uint32_t gpr[32];
    uint32_t pc;
} NPC_state;
extern NPC_state npc_s,ref_s; 

typedef struct {
    const char *diff;
    const char *ftrace;
    const char *itrace;
    const char *mtrace;
    const char *dtrace;
}DEBUG_FILE_PATH;
extern DEBUG_FILE_PATH dfp;


extern uint8_t npc_pmem[PMEM_SIZE];
extern uint8_t* guest_to_host(paddr_t paddr);


// ==================== 调试宏（精简版） ====================
#define ANSI_FG_RED     "\33[1;31m"
#define ANSI_FG_GREEN   "\33[1;32m"
#define ANSI_FG_YELLOW  "\33[1;33m"
#define ANSI_FG_BLUE    "\33[1;34m"
#define ANSI_FG_CYAN    "\33[1;36m"
#define ANSI_NONE       "\33[0m"
#define ANSI_FMT(str, fmt) fmt str ANSI_NONE

#define Log(format, ...) \
    printf(ANSI_FMT("[%s:%d %s] " format, ANSI_FG_BLUE) "\n", \
        __FILE__, __LINE__, __func__, ## __VA_ARGS__)

#define Assert(cond, format, ...) \
  do { \
    if (!(cond)) { \
      fprintf(stderr, ANSI_FMT(format, ANSI_FG_RED) "\n", ## __VA_ARGS__); \
      assert(0); \
    } \
  } while (0)

#define panic(format, ...) Assert(0, format, ## __VA_ARGS__)

#endif
