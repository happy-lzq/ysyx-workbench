ENTRY(_start)                             #1、入口设置：_start
PHDRS { text PT_LOAD; data PT_LOAD; }     #2、可加载段：将代码分解成代码段和数据段两个PT_LOAD

SECTIONS {
  /* _pmem_start and _entry_offset are defined in LDFLAGS */
  . = _pmem_start + _entry_offset;        #3、section布局：先设定基址，也就是物理起始地址
  .text : {                               #4、程序指令段
    *(entry)
    *(.text*)
  } : text
  etext = .;
  _etext = .;

  .rodata : {                             #5、常量、字符变量只读数据段
    *(.rodata*)
  }

  .data : {                               #6、数据段——初始化的全局/静态变量
    *(.data*)
  } : data
  edata = .;
  _data = .;

  .bss : {                                #7、未初始化的全局/静态变量
	_bss_start = .;
    *(.bss*)
    *(.sbss*)
    *(.scommon)
  }
  _stack_top = ALIGN(0x1000);             # BSS 结束后对齐页边界
  . = _stack_top + 0x8000;                # 为栈预留 32 kb空间
  _stack_pointer = .;                     # 栈指针指向栈顶
  end = .;
  _end = .;
  _heap_start = ALIGN(0x1000);            # 堆区从栈区之后开始
}

/*
1、栈的作用：
  函数调用：保存返回地址(ra)，传递函数参数，分配局部变量
  RISCV约定：sp栈指针向下增长，从高地址向低地址开辟空间分配给函数调用
  生命周期：栈上数据在函数返回时自动释放；后进先出的自动内存管理，根据函数调用嵌套与返回的逻辑可以理解
2、堆的作用
  动态开辟物理内存，用于大型的全局结构体数据变量
  主要是以malloc开辟。
*/