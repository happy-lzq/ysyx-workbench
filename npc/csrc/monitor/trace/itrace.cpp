#include <trace.h>

#define IRING_BUF_SIZE  16              
#define ILEN 4
char irbuf [IRING_BUF_SIZE] [128];     // 每行缓冲区
bool irbuf_full = false;
int irbuf_pos = 0;
FILE *itrace_fp = NULL;

void itrace_log(uint32_t pc, uint32_t inst_word){
    char logbuf[128];
    char *p = logbuf;
    uint8_t *inst = (uint8_t *) &inst_word;

    p += snprintf(p, sizeof(logbuf), "0x%08x:", pc);
    for (int i = ILEN-1; i >=0; i--){
        p += snprintf(p,4," %02x",inst[i]);
    }
    memset(p,' ',ILEN);
    p += ILEN;
    disassemble(p, logbuf + sizeof(logbuf) - p, pc, inst, ILEN);
    printf("%s\n", logbuf);
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

static void display_irbuf(void){
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

