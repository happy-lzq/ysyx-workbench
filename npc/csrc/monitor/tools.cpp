#include <difftest.h>

DEBUG_FILE_PATH dfp;
FILE *diff_fp = NULL;

uint8_t* guest_to_host(paddr_t paddr) {
    return npc_pmem + paddr - RESET_VECTOR;
}

 void build_named_log_file(char *buf, size_t buf_size,
                                        const char *path,
                                        const char *default_path,
                                        const char *suffix) {
  Assert(path,"IMG-BIN FILE PATH IS FAIL\n");

  const char *slash = strrchr(path, '/');
  const char *name = slash == NULL ? path : slash + 1;              // 获取文件名起始地址
  const char *dot = strrchr(name, '.');

  size_t dir_len = slash == NULL ? 0 : (size_t)(slash - path + 1);  // 获取目录长度包括/
  size_t base_len = (dot != NULL && dot > name) ? (size_t)(dot - name) : strlen(name);   // 获取名字长度
  // ret 返回值大于0  ret < buf_size 无溢出
  // 拼接逻辑：fix
  int ret = snprintf(buf, buf_size, "%.*s%.*s-%s",
                     (int)dir_len, path,
                     (int)base_len, name,
                     suffix);
  // 按格式化将对应的内容写入buf，且最大数据为buf_size字节大小
  // %.*s 中：.* 对应(int)的动态长度，s 对应字符串
  Assert(ret > 0 && ret < buf_size, "log path is too long: %s", path);
  // Assert(条件，输出文本) 条件成立，不输出；条件不成立，中断输出红色文本
}

void init_diff_log(const char *path){
  build_named_log_file(dfp.diff,sizeof(dfp.diff),path,"build/diff-log-txt","diff-log-txt");
  diff_fp = fopen(dfp.diff,"w");
  Assert(diff_fp,"Can not open '%s'", dfp.diff);
  Log("diff log written to %s", dfp.diff);
}

void diff_log_write(NPC_state *npc, NPC_state *ref, int cycle) {
    if (!diff_fp) return;

    bool pc_ok = (npc->pc == ref->pc);
    fprintf(diff_fp, "--- cycle %d ---  PC: %s0x%08x" ANSI_NONE "  %s",
            cycle,
            pc_ok ? ANSI_FG_GREEN : ANSI_FG_RED,
            pc_ok ? npc->pc : npc->pc,
            pc_ok ? "OK" : "MISMATCH");
    if (!pc_ok)
        fprintf(diff_fp, "                    REF=0x%08x\n", ref->pc);

    // 4列 × 8行 寄存器网格
    for (int row = 0; row < 8; row++) {
        fprintf(diff_fp, "%sx%-2d-x%-2d" ANSI_NONE, ANSI_FG_CYAN, row * 4, row * 4 + 3);
        for (int col = 0; col < 4; col++) {
            int i = row * 4 + col;
            bool ok = (npc->gpr[i] == ref->gpr[i]);
            fprintf(diff_fp, "\t%sNPC=%08x REF=%08x" ANSI_NONE,
                    ok ? ANSI_FG_GREEN : ANSI_FG_RED,
                    npc->gpr[i], ref->gpr[i]);
        }
        fprintf(diff_fp, "\n");
    }
    fprintf(diff_fp, "\n");
    fflush(diff_fp);
}