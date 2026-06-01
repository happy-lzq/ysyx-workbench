.section entry, "ax"
.globl _start
.type _start, @function

_start:
  mv s0, zero                       #定义程序入口
  la sp, _stack_pointer             #初始化栈指针
  call _trm_init                    # 调到_trm_init,进入main

.size _start, . - _start
