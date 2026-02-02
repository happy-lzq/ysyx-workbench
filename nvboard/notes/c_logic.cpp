// 组合逻辑
/*
无时钟信号
输出立即响应输入变化
不需要复位序列
*/

#include <verilated.h>
#include <verilated_vcd_c.h>     
#include <nvboard.h>
#include <Vtop.h>               // 替换为你的模块名

static Vtop dut;                // 实例化顶层
static vluint64_t main_time = 0;
double sc_time_stamp() { return main_time; }
void nvboard_bind_all_pins(Vtop* top);

int main() {
    // ===== 初始化 =====
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    dut.trace(tfp, 99);
    tfp->open("wave.vcd");

    nvboard_bind_all_pins(&dut);
    nvboard_init();

    // ===== 组合逻辑仿真循环 =====
    while (!Verilated::gotFinish()) {
        // 更新NVBoard输入（读取开关状态）
        nvboard_update();
        
        // 立即评估组合逻辑
        dut.eval();
        
        // 记录波形（每次输入变化都记录）
        tfp->dump(main_time);
        main_time += 5;  // 推进时间
        
    }

    // ===== 收尾 =====
    tfp->close();
    nvboard_quit();
    return 0;
}