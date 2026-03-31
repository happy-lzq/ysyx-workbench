#include <am.h>
#include <nemu.h>

extern char _heap_start;
int main(const char *args);
// 用于指示堆区
Area heap = RANGE(&_heap_start, PMEM_END);
// mainargs[]即是数组，则mainargs即为字符串指针类型
static const char mainargs[MAINARGS_MAX_LEN] = TOSTRING(MAINARGS_PLACEHOLDER); // defined in CFLAGS

/*
编译链接时，mainargs[] 数组在二进制文件中的地址（偏移量）被固定下来（通常在只读数据段 .rodata 中）。
Python 脚本通过搜索占位符字符串定位到该数组在文件中的起始位置，直接原地覆盖其内容为实际参数（并填充空字符至固定长度）。
因此，程序加载到内存后，main 函数通过 args 指针访问到的就是替换后的字符串。
*/

void putch(char ch) {             
  outb(SERIAL_PORT, ch);    //把字节写到一个映射在 guest 物理地址空间的 MMIO 区
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
