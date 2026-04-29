#include <am.h>
#include <nemu.h>

extern char _heap_start;
int main(const char *args);

Area heap = RANGE(&_heap_start, PMEM_END);
// 构建一个全局的堆区结构体，范围从 _heap_start 到 PMEM_END

void putch(char ch) {
  outb(0x10000000, ch);
}

__attribute__((noinline))
void halt(int code) {
  while (1);
}

void _trm_init() {
  int ret = main("");
  halt(ret);
}
