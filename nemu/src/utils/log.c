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

#include <common.h>
#include <isa.h>
extern uint64_t g_nr_guest_inst;

#ifndef CONFIG_TARGET_AM
FILE *log_fp = NULL;
FILE *mtrace_fp = NULL;
void init_log(const char *log_file) {
  log_fp = stdout;
  if (log_file != NULL) {
    FILE *fp = fopen(log_file, "w");
    Assert(fp, "Can not open '%s'", log_file);
    log_fp = fp;
  }
  Log("Log is written to %s", log_file ? log_file : "stdout");
}

bool log_enable() {
  return MUXDEF(CONFIG_TRACE, (g_nr_guest_inst >= CONFIG_TRACE_START) &&
         (g_nr_guest_inst <= CONFIG_TRACE_END), false);
}

void init_mtrace_log(const char *mtrace_log_file) {
  mtrace_fp = fopen(mtrace_log_file, "w");
  Assert(mtrace_fp, "Can not open '%s'", mtrace_log_file);
  Log("Mtrace log is written to %s", mtrace_log_file);
}


void mtrace_write(char type, paddr_t addr, int len, word_t data){
  if (mtrace_fp != NULL)
  {
    fprintf(mtrace_fp,"pc=" FMT_WORD " %c addr=" FMT_PADDR " len=%d data="FMT_WORD "\n",
            cpu.pc,type,addr,len,data);
    fflush(mtrace_fp);
  }
  
}

#endif