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
    printf("Starting reset sequence...\n");
    dut.rst = 1;
    for (int i = 0; i < 5; i++) {
        dut.clk = 0;
        dut.eval();
        tfp->dump(main_time);
        main_time += 5;
        
        dut.clk = 1;
        dut.eval();
        tfp->dump(main_time);
        main_time += 5;
    }
    dut.rst = 0;
    printf("Reset complete.\n");

    // ===== 混合电路仿真循环 =====
    int cycle_count = 0;
    const int MAX_CYCLES = 2000;
    
    while (!Verilated::gotFinish() && cycle_count < MAX_CYCLES) {
        // 阶段1: 时钟低电平 - 输入稳定期
        dut.clk = 0;
        nvboard_update();  // 更新输入信号
        
        // 评估组合逻辑对输入变化的立即响应
        dut.eval();
        tfp->dump(main_time);
        main_time += 3;
        
        // 阶段2: 时钟上升沿前 - 建立时间
        dut.eval();  // 确保信号稳定
        tfp->dump(main_time);
        main_time += 2;
        
        // 阶段3: 时钟上升沿 - 时序逻辑采样
        dut.clk = 1;
        dut.eval();  // 时序逻辑更新
        tfp->dump(main_time);
        main_time += 3;
        
        // 阶段4: 时钟高电平 - 组合逻辑传播
        dut.eval();  // 组合逻辑响应时序逻辑的变化
        tfp->dump(main_time);
        main_time += 2;
        
        cycle_count++;
        
        // 每100个周期打印进度
        if (cycle_count % 100 == 0) {
            printf("Simulated %d cycles...\n", cycle_count);
        }
    }

    // ===== 收尾 =====
    printf("Simulation completed after %d cycles.\n", cycle_count);
    tfp->close();
    nvboard_quit();
    return 0;
}