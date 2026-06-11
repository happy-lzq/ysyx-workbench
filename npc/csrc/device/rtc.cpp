#include <device.h>
#include <interrupt.h>
static uint64_t rtc_latched_us = 0;

// ================= RTC 时钟读取 ===================
uint64_t host_time_us() {
    using namespace std::chrono;
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}
// ============ 宿主闹钟：时钟中断源处理 ============
volatile sig_atomic_t alarm_fired = 0;

static void alarm_handler(int signum) {
    alarm_fired = 1;  
}

void init_timer_alarm() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = alarm_handler;
    sigaction(SIGVTALRM, &sa, NULL);

    struct itimerval it = {};
    it.it_value.tv_sec     = 0;
    it.it_value.tv_usec    = 1000000 / 60;   // TIMER_HZ=60, ≈16.7ms
    it.it_interval         = it.it_value;    // 周期性触发
    setitimer(ITIMER_VIRTUAL, &it, NULL);
}


uint32_t rtc_read_handler(MMIODevice *dev, uint32_t offset) {
    switch (offset) {
        case 0:
            if (rtc_latched_us == 0) rtc_latched_us = host_time_us();
            return (uint32_t)(rtc_latched_us & 0xFFFFFFFF);
        case 4:
        rtc_latched_us = host_time_us();   // 读高位时重新锁存
        return (uint32_t)(rtc_latched_us >> 32);
    }
}

/*
启动时一次调用
    │
    ▼
init_timer_alarm()          ← monitor.cpp:93 调用
    │  setitimer(ITIMER_VIRTUAL, 16.7ms)
    ▼
========== OS 层 ==========
每 16.7ms：OS 发送 SIGVTALRM
    │
    ▼
alarm_handler()             ← 信号上下文，只做一件事
    │  alarm_fired = 1
    ▼
========== 每周期检查 ==========
interrupt_check()           ← single_cycle() 每周期调用 (main.cpp:15)
    │  if (alarm_fired) {
    │      alarm_fired = 0;
    │      npc_csr(... MIP | MTIP ...)   // 写入 MIP，置 MTIP 位
    │  }
    ▼
isa_query_intr()            ← 检查 MIP.MTIP & MIE.MTIE
    │  return 0x80000007   (M-mode timer interrupt)
    ▼
single_cycle()              ← top->interrupt_valid = 1
    ▼
CPU RTL 下周期              ← trap 到 M-mode 中断处理

*/
