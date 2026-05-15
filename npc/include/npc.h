#ifndef __NPC_H__
#define __NPC_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <verilated.h>
#include <Vcore_top.h>
#include <Vcore_top___024root.h>

// ==================== 基础常量 ====================
#define RESET_VECTOR 0x80000000
#define PMEM_SIZE (128 * 1024 * 1024)

// ==================== 基础类型 ====================
typedef uint32_t paddr_t;

typedef struct {
    uint32_t gpr[32];
    uint32_t pc;
} NPC_state;

typedef struct {
    const char *diff;
    const char *ftrace;
    const char *itrace;
    const char *mtrace;
    const char *dtrace;
} DEBUG_FILE_PATH;

// ==================== 全局变量声明 ====================
extern Vcore_top *top;
extern NPC_state npc_s, ref_s;
extern DEBUG_FILE_PATH dfp;
extern uint8_t npc_pmem[PMEM_SIZE];
extern int idx;
extern long img_size;

// ==================== 函数声明 ====================
uint8_t *guest_to_host(paddr_t paddr);
void load_bin(Vcore_top *top, const char *path);

// ==================== Verilator RTL 访问器 ====================
enum { READ, WRITE };

static inline uint32_t npc_gpr(Vcore_top *top, int idx, uint32_t val, int r_w) {
    if (r_w == WRITE)
        return top->rootp->core_top__DOT__u_regfile__DOT__rf[idx] = val;
    else
        return top->rootp->core_top__DOT__u_regfile__DOT__rf[idx];
}

static inline uint32_t npc_pc(Vcore_top *top, uint32_t val, int r_w) {
    if (r_w == WRITE)
        return top->rootp->core_top__DOT__pc = val;
    else
        return top->rootp->core_top__DOT__pc;
}

static inline uint32_t npc_imem(Vcore_top *top, int idx, uint32_t val, int r_w) {
    if (r_w == WRITE)
        return top->rootp->core_top__DOT__u_if_stage__DOT__imem[idx] = val;
    else
        return top->rootp->core_top__DOT__u_if_stage__DOT__imem[idx];
}

// ==================== 调试宏 ====================
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
