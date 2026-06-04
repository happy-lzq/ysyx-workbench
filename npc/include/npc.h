#ifndef __NPC_H__
#define __NPC_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>   
#include <stddef.h>
#include <regex.h>
#include <getopt.h>
#include <cstdio>
#include <cstring>
#include <cstddef>
#include <readline/readline.h>
#include <readline/history.h>
#include <verilated.h>
#include <Vcore_top.h>
#include <verilated_vcd_c.h>
#include <Vcore_top___024root.h>
#include <generated/autoconf.h>
#include <memory.h>

// ==================== 基础常量（由 Kconfig 生成） ====================
#define RESET_VECTOR CONFIG_RESET_VECTOR
#define MAX_CYCLE 1000000
#define INTR_EMPTY ((word_t)-1)
// ==================== 定义 CSR 地址宏（与 Verilog 保持一致）
#define CSR_MSTATUS   0x300
#define CSR_MTVEC     0x305
#define CSR_MEPC      0x341
#define CSR_MCAUSE    0x342
#define CSR_MIP       0x344
#define CSR_MIE       0x304
#define CSR_MTVAL     0x343
#define CSR_MSCRATCH  0x340

// ==================== 基础类型 ====================
typedef uint32_t paddr_t;
typedef uint32_t vaddr_t;
typedef uint32_t word_t;
enum {NPC_STOP,NPC_RUNNING, NPC_END, NPC_ABORT, NPC_QUIT};

typedef struct {
  int state;
  vaddr_t halt_pc;
  uint32_t halt_ret;
}NPCSIM_State;

typedef struct {
    uint32_t gpr[32];
    uint32_t pc;
    uint32_t csr[8];   // ← 新增，顺序需与 NEMU isa-def.h 的 enum 一致
} NPC_state;

typedef struct {
     char diff[128];
     char ftrace[128];
     char itrace[128];
     char mtrace[128];
     char dtrace[128];
} DEBUG_FILE_PATH;

// ==================== 全局变量声明 ====================
extern Vcore_top *top;
extern VerilatedVcdC* tfp ;
extern NPC_state npc_s, ref_s;
extern NPCSIM_State npc_sim_state;
extern DEBUG_FILE_PATH dfp;
extern int idx;
extern long img_size;
extern const char *img_file;
extern const char *diff_so_file;
extern FILE *diff_fp;
extern bool wave_enabled;
extern uint64_t sim_time;
extern int cycle;
// ==================== 函数声明 ====================
void halt_check();
void npc_init();
void npc_state_check();
void single_cycle();
void sdb_mainloop();
void init_sdb();
void build_named_log_file(char *buf, size_t buf_size,
                                        const char *path,
                                        const char *default_path,
                                        const char *suffix) ;
void assert_fail_msg();

void monitor_init(int argc, char* argv[]);
// ==================== Verilator RTL 访问器 ====================
enum { READ, WRITE };

static inline uint32_t npc_gpr(Vcore_top *top, int idx, uint32_t val, int r_w) {
    if (r_w == WRITE)
        return top->rootp->core_top__DOT__u_regfile__DOT__rf[idx] = val;
    else
        return top->rootp->core_top__DOT__u_regfile__DOT__rf[idx];
}

// 每个周期的当前pc
static inline uint32_t npc_pc(Vcore_top *top, uint32_t val, int r_w) {
    if (r_w == WRITE)
        return top->rootp->core_top__DOT__pc = val;
    else
        return top->rootp->core_top__DOT__pc;
}

static inline uint32_t npc_npc(Vcore_top *top, uint32_t val, int r_w) {
    if (r_w == WRITE)
        return top->rootp->core_top__DOT__u_if_stage__DOT__pc_next = val;
    else
        return top->rootp->core_top__DOT__u_if_stage__DOT__pc_next;
}
static inline uint32_t npc_inst(Vcore_top *top, uint32_t val, int r_w) {
    if (r_w == WRITE)
        return top->instr = val;
    else
        return top->instr;
}

static inline uint32_t npc_csr(Vcore_top *top, uint32_t idx, uint32_t val, int r_w) {
    switch (idx) {
        case CSR_MCAUSE:
            if (r_w == WRITE) top->rootp->core_top__DOT__u_csr__DOT__csr_mcause = val;
            else              return top->rootp->core_top__DOT__u_csr__DOT__csr_mcause;
            break;
        case CSR_MEPC:
            if (r_w == WRITE) top->rootp->core_top__DOT__u_csr__DOT__csr_mepc = val;
            else              return top->rootp->core_top__DOT__u_csr__DOT__csr_mepc;
            break;
        case CSR_MIE:
            if (r_w == WRITE) top->rootp->core_top__DOT__u_csr__DOT__csr_mie = val;
            else              return top->rootp->core_top__DOT__u_csr__DOT__csr_mie;
            break;
        case CSR_MIP:
            if (r_w == WRITE) top->rootp->core_top__DOT__u_csr__DOT__csr_mip = val;
            else              return top->rootp->core_top__DOT__u_csr__DOT__csr_mip;
            break;
        case CSR_MSCRATCH:
            if (r_w == WRITE) top->rootp->core_top__DOT__u_csr__DOT__csr_mscratch = val;
            else              return top->rootp->core_top__DOT__u_csr__DOT__csr_mscratch;
            break;
        case CSR_MSTATUS:
            if (r_w == WRITE) top->rootp->core_top__DOT__u_csr__DOT__csr_mstatus = val;
            else              return top->rootp->core_top__DOT__u_csr__DOT__csr_mstatus;
            break;
        case CSR_MTVAL:
            if (r_w == WRITE) top->rootp->core_top__DOT__u_csr__DOT__csr_mtval = val;
            else              return top->rootp->core_top__DOT__u_csr__DOT__csr_mtval;
            break;
        case CSR_MTVEC:
            if (r_w == WRITE) top->rootp->core_top__DOT__u_csr__DOT__csr_mtvec = val;
            else              return top->rootp->core_top__DOT__u_csr__DOT__csr_mtvec;
            break;
        default:
            return 0;
    }
    return 0;
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
      assert_fail_msg(); \
      assert(0); \
    } \
  } while (0)

#define panic(format, ...) Assert(0, format, ## __VA_ARGS__)

#endif
