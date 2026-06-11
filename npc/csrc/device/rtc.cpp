#include <device.h>
#include <interrupt.h>


uint64_t host_time_us() {
    using namespace std::chrono;
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}
// ============ 宿主闹钟：信号处理器只设标志 ============
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
