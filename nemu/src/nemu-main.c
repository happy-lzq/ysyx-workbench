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
#include "monitor/sdb/sdb.h" 

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

  /* Start engine. */
  engine_start();

  return is_exit_status_bad();
}

/*
阶段一：开天辟地 —— NEMU 启动，建立 MMIO 映射（硬件阶段）
这发生在你终端敲下 make run 的那一瞬间，甚至在你的 main(mainargs) 被调用之前。

1、NEMU 开始运行：调用 init_monitor() -> init_device() -> init_timer()。
2、分配空间：NEMU 在宿主机（你的 Ubuntu）内存里 malloc 了一块 8 字节的空间，指针叫 rtc_port_base。这也是我们的“硬件寄存器”。
3、注册 MMIO 映射：NEMU 调用 add_mmio_map("rtc", 0xa0000048, rtc_port_base, 8, rtc_io_handler)。
  从这一刻起，NEMU 内部的总线裁判就记住了：“谁要是读写物理地址 0xa0000048 ~ 0xa000004F，我就把请求发给 rtc_port_base 和 rtc_io_handler”。
4、硬件初始化完毕：此时 NEMU 把虚拟的 RISC-V CPU 准备好，把你的 AM 镜像（am-tests）加载进内存，开始执行第一条 RISC-V 指令。
  关键点：ioe_init() 并没有创建 MMIO 映射，MMIO 映射在硬件通电（NEMU 启动）时就已经固化了。

阶段二：软件初始化 —— AM 执行 ioe_init()（软件握手阶段）
现在，虚拟的 CPU 一路执行，终于来到了你的 case 't': ioe_init();。这就是你当前关心的起点。

1、建立软件分发表：ioe_init() 首先遍历 lut 数组，把未定义的设备接口填上 fail，把定义了的（比如 AM_TIMER_UPTIME）绑定到 __am_timer_uptime 函数。
2、触发硬件驱动初始化：接着执行 __am_timer_init();。
3、产生第一次 MMIO 交互：在 __am_timer_init() 里，你需要记录一个“启动基准时间”。所以你写下了 inl(RTC_ADDR + 4)。
4、指令下发：这句 C 代码被编译成了一条 RISC-V 的 lw（Load Word）指令，试图读取 0xa000004c。
5、NEMU 拦截：NEMU 在模拟这条 lw 指令时，发现地址是在 0xa0000048 的范围内！
6、惰性获取当前时间（NEMU 怎么获取时间？）：
  NEMU 触发回调 rtc_io_handler(offset = 4)。
  在这个回调函数里，NEMU 调用了宿主机 Linux 系统的 API（get_time() 或 gettimeofday），拿到你电脑 CPU 当前的绝对真实时间。
  NEMU 把这个绝对时间存入 rtc_port_base（低位和高位）。
7、数据返回给 AM：NEMU 把刚才存进 rtc_port_base 的数据，当做那条 lw 指令的返回值，交还给 RISC-V CPU。
8、保存基准时间：你的 AM 代码拼好了 64 位的绝对时间，并存在了一个叫 boot_time 的静态变量里。ioe_init() 结束。


*/