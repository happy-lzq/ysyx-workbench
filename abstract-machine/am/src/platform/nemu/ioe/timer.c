#include <am.h>
#include <nemu.h>

static uint64_t init_time = 0;   // 记住系统刚启动时的初始时间

/*系统启动时间记录初始化设置
1、使用 inl(RTC_ADDR + 4)（高 32 位）和 inl(RTC_ADDR + 0)（低 32 位）读取 NEMU 底层当前的时间。
  1、为什么分两次读，且先读高32位，再读低32位？ 
    1、RTC_ADDR提供的是8字节(64)，由于我们的cpu属于32位，故一次只能读取32位。
    2、先读高 32 位：触发硬件刷新全 64 位时间，并拿到高位；再读低 32 位：拿到刚才被同时刷新的低位。
      (如果反过来先读低位，你读到的将是“上一次”遗留下来的过期废数据！)
  2、RTC计时器的硬件实现：nemu/src/device/timer.c  触发硬件寄存器获取时间条件
    if (!is_write && offset == 4)： 读并且offset=4,高32位寄存器

2、把高位和低位拼成一个 64 位的微秒时间戳。
  1、如何建立从nemu硬件底层读取的高低32位时间数据写入的地址与am之间的关系？答案： 
    1、MMIO（内存映射I/O）    nemu/src/device/timer.c 
    2、硬件层面（NEMU模拟器）  add_mmio_map("rtc", CONFIG_RTC_MMIO, rtc_port_base, 8, rtc_io_handler);
    3、软件层面（AM驱动代码    #define RTC_ADDR        (DEVICE_BASE + 0x0000048) 
    4、
  2、如何让AM裸机去读取nemu底层硬件已经获取的时间？
    1、
3、把这个拼好的时间戳存到你刚定义的 boot_time 里。
4、以后所有“系统运行了多久”的计算，都以这个 boot_time 为基准。
*/

static uint64_t read_rtc(){
  uint32_t hi = inl(RTC_ADDR + 4);      
  // load 指令分发外设分支 MMIO映射触发 rtc_io_handler(offset = 4)
  // NEMU 调用了宿主机 Linux 系统的 API（get_time() 或 gettimeofday），CPU 当前的绝对真实时间
  uint32_t lo = inl(RTC_ADDR + 0);
  return ((uint64_t)hi << 32) | lo ;  // 先无符号扩展高位至64位，再hi左移至高位，最后与上lo 
}

void __am_timer_init() {
  init_time = read_rtc();
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = read_rtc() - init_time;

}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
