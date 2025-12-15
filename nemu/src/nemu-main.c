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
#include <stdio.h>
#include <inttypes.h>
/* forward declare expr from sdb */
extern word_t expr(char *e, bool *success, bool *hex);
#include <stdbool.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif

  /* If an input file exists, run expressions from it and exit. */
  // 表达式求值输入文件验证
  {
    FILE *f = fopen("input", "r");
    if (f) {
      char line[4096];
      while (fgets(line, sizeof(line), f)) {
        /* strip newline */
        char *p = line;
        while (*p && (*p == ' ' || *p == '\t')) p++; /* skip leading ws */
        char *end = p + strlen(p);
        while (end > p && (end[-1] == '\n' || end[-1] == '\r' || end[-1] == ' ' || end[-1] == '\t')) end--;
        *end = '\0';
        if (*p == '\0') continue;
        bool success = false; bool ishex = false;
        word_t val = expr(p, &success, &ishex);
        if (success) {
          if (ishex) printf("0x%08" PRIx32 "\n", val);
          else printf("%u\n", (unsigned)val);
        } else {
          printf("Bad expression: %s\n", p);
        }
      }
      fclose(f);
      return 0;
    }
  }

  /* Start engine. */
  engine_start();

  return is_exit_status_bad();
}
