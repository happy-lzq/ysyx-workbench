#include <verilated.h>
#include <verilated_vcd_c.h>
#include <nvboard.h>
#include <Vtest.h>               
#include <stdio.h>

static Vtest *dut = new Vtest;                
static vluint64_t simu_time = 0;
static sc_time_stamp(){return simu_time;}
static VerilatedVcdC *tfp = new VerilatedVcdC;
void nvboard_bind_all_pins(Vtest* top);         // 自动生成的绑定函数

int main() {
    printf("=== Starting Simulation ===\n");
    Verilated::traceEverOn(true);
    dut->trace(tfp,99);
    tfp->open("test.vcd")
    // ===== NVBoard 初始化 =====
    nvboard_bind_all_pins(dut);                // 指针接受顶层模块地址用于访问dut内部变量地址进行引脚绑定和信号传输
    nvboard_init();

    // ===== 仿真循环 =====
    int count = 0 ;
    int cycle_max = 1000000;
    while( count < cycle_max )
    {
        nvboard_update();                      
        dut->eval();                             
        printf("a = %d, b = %d, f = %d\n", dut->a, dut->b, dut->f);
        count++;
    }
    
    // ===== 收尾 =====
    dut->final();
    delete dut;
    nvboard_quit();
    printf("\n=== simulation over ====\n");
    return 0;
    
}

/*
整体仿真设计逻辑：nvboard   verilatro   gtkwave

1 实例化verilog顶层模块：Vtest dut
2 verilator 初始化：
    启用波形跟踪
    创建波形对象
    选择波形推进深度
    打开vcd波形接受文件
2 初始化nvboard硬件仿真模拟器
    自动绑定引脚函数将实例化输入输出与模拟物理引脚进行绑定
    nvboard 初始化
3 仿真循环(设置仿真循环结束条件)
    nvboard界面更新接受模拟物理硬件改变带来的值变化
    eval 评估实例化模块组合、时序逻辑将nvboard更改的数值输出到实例化参数当中
    打印 实例化参数值检查是否仿真更新数值
    tfp仿真时间信号值写入vcd波形文件
    推进仿真时间，重复循环
4 仿真收尾
    tfp关闭vcd文件
    退出nvboard仿真界面

*/
