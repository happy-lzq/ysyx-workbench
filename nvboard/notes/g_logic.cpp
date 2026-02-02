#include <verilated.h>
#include <verilated_vcd_c.h>     
#include <nvboard.h>
#include <Vtop.h>

static Vtop dut;                
static vluint64_t main_time = 0;
double sc_time_stamp() { return main_time; }
void nvboard_bind_all_pins(Vtop* top);

// 检测电路类型的辅助函数
bool has_clock_signal() {
    // 通过检查dut是否有clk成员来判断
    // 这需要RTTI支持，或者可以通过预定义宏来确定
    return true; // 假设有时钟，更安全的做法
}

bool has_reset_signal() {
    return true; // 假设有复位
}

int main() {
    // ===== 初始化 =====
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    dut.trace(tfp, 99);
    tfp->open("wave.vcd");

    nvboard_bind_all_pins(&dut);
    nvboard_init();

    // ===== 自适应初始化 =====
    if (has_reset_signal()) {
        printf("Performing reset sequence...\n");
        dut.rst = 1;
        for (int i = 0; i < 5; i++) {
            if (has_clock_signal()) {
                dut.clk = 0; dut.eval(); tfp->dump(main_time); main_time += 5;
                dut.clk = 1; dut.eval(); tfp->dump(main_time); main_time += 5;
            } else {
                dut.eval(); tfp->dump(main_time); main_time += 10;
            }
        }
        dut.rst = 0;
    }

    // ===== 自适应仿真循环 =====
    int cycle_count = 0;
    const int MAX_CYCLES = 1000;
    
    while (!Verilated::gotFinish() && cycle_count < MAX_CYCLES) {
        if (has_clock_signal()) {
            // 时序逻辑模式
            dut.clk = 0;
            nvboard_update();
            dut.eval();
            tfp->dump(main_time);
            main_time += 5;
            
            dut.clk = 1;
            dut.eval();
            tfp->dump(main_time);
            main_time += 5;
        } else {
            // 组合逻辑模式
            nvboard_update();
            dut.eval();
            tfp->dump(main_time);
            main_time += 10;
        }
        
        cycle_count++;
    }

    // ===== 收尾 =====
    tfp->close();
    nvboard_quit();
    printf("Simulation finished.\n");
    return 0;
}