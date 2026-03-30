#include <am.h>
#include <nemu.h>

extern char _heap_start;
int main(const char *args);
// 用于指示堆区
Area heap = RANGE(&_heap_start, PMEM_END);
// mainargs[]即是数组，则mainargs即为字符串指针类型
static const char mainargs[MAINARGS_MAX_LEN] = TOSTRING(MAINARGS_PLACEHOLDER); // defined in CFLAGS

void putch(char ch) {             // 
  outb(SERIAL_PORT, ch);
}

void halt(int code) {
  nemu_trap(code);

  // should not reach here
  while (1);
}

void _trm_init() {
  int ret = main(mainargs);  // 指向只读数据段的 mainargs 数组
  halt(ret);
}
