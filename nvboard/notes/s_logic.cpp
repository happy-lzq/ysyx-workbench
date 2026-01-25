/*
有时钟信号
输出在时钟边沿变化
需要复位序列
状态在时钟控制下更新
*/

#include <verilated.h>
#include <verilated_vcd_c.h>     
#include <nvboard.h>
#include <Vtop.h>               // 替换为你的模块名

static Vtop dut;                
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

    // ===== 复位序列 =====
    dut.rst = 1;
    for (int i = 0; i < 5; i++) {
        // 时钟下降沿
        dut.clk = 0;
        dut.eval();
        tfp->dump(main_time);
        main_time += 5;
        
        // 时钟上升沿
        dut.clk = 1;
        dut.eval();
        tfp->dump(main_time);
        main_time += 5;
    }
    dut.rst = 0;

    // ===== 时序逻辑仿真循环 =====
    int cycle_count = 0;
    const int MAX_CYCLES = 1000;  // 防止无限循环
    
    while (!Verilated::gotFinish() && cycle_count < MAX_CYCLES) {
        // 时钟下降沿 - 准备阶段
        dut.clk = 0;
        nvboard_update();  // 在时钟低电平期间更新输入
        dut.eval();
        tfp->dump(main_time);
        main_time += 5;
        
        // 时钟上升沿 - 采样和更新阶段
        dut.clk = 1;
        dut.eval();
        tfp->dump(main_time);
        main_time += 5;
        
        cycle_count++;
    }

    // ===== 收尾 =====
    tfp->close();
    nvboard_quit();
    return 0;
}