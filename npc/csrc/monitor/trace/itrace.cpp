#include <trace.h>
#include <difftest.h>
#include <memory.h>

char irbuf [IRING_BUF_SIZE] [128];     // 每行缓冲区
char logbuf [128];
bool irbuf_full = false;
int irbuf_pos = 0;
FILE *itrace_fp;

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

void itarce_log_file(const char *path){
  build_named_log_file(dfp.itrace,sizeof(dfp.itrace),path,"build/itrace-log-txt","itrace-log-txt");
  itrace_fp = fopen(dfp.itrace,"w");
  Assert(itrace_fp,"Can not open '%s'", dfp.itrace);
  Log("\nitrace log written to %s\n", dfp.itrace);

}



 static void iringbuf(const char *log) {
  strcpy(irbuf[irbuf_pos],log);
  irbuf_pos = (irbuf_pos + 1) % IRING_BUF_SIZE;
  if (irbuf_pos == 0){
    // 第一轮填满之后，irbuf-full 就会永远为ture;第一轮为填满，则一直处于fasle
    // 利用这一步来分两个打印输出方式z
    irbuf_full = true;
  }
  return ;
 }

void display_irbuf(void){
  int fail_inst = (irbuf_pos-1 +IRING_BUF_SIZE) % IRING_BUF_SIZE;
  if (!irbuf_full)
  {
    for (int i = 0; i < irbuf_pos; i++)
    {
      if (i == fail_inst)
      {
        printf("--> %s\n", irbuf[i]);
      } else{
        printf("    %s\n", irbuf[i]);
      }
    }   
  } else {
    int pos = irbuf_pos;
    for (int i = 0; i < IRING_BUF_SIZE; i++)
    {
      if (pos == fail_inst)
      {
        printf("--> %s\n", irbuf[pos]);
      } else{
        printf("    %s\n", irbuf[pos]);
      }
      pos = (pos +1) % IRING_BUF_SIZE;
    }
  }
  return ;
}

void itrace_log(uint32_t pc, uint32_t inst_word){
    char *p = logbuf;
    uint8_t *inst = (uint8_t *) &inst_word;

    p += snprintf(p, sizeof(logbuf), "0x%08x:", pc);
    for (int i = ILEN-1; i >=0; i--){
        p += snprintf(p,4," %02x",inst[i]);
    }
    memset(p,' ',ILEN);
    p += ILEN;
    disassemble(p, logbuf + sizeof(logbuf) - p, pc, inst, ILEN);
    fprintf(itrace_fp,"cycle : %05d    %s\n",cycle,logbuf);
    fflush(itrace_fp);
    iringbuf(logbuf);  // 记录到环形缓冲器
}
