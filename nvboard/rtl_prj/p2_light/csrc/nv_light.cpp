#include <stdio.h>
#include <Vlight.h>
#include <nvboard.h>

static Vlight *dut = new Vlight; ;
void nvboard_bind_all_pins(Vlight *top);
// 电路单周期更替，模拟周期
static void single_cycle() {
    dut->clk = 0; 
    dut->eval();

    dut->clk = 1; 
    dut->eval();
}

static void rst_funtion(){
    dut->rst = 1;
    for (int i = 0; i < 10 ; i++){
        single_cycle();
    }
    dut->rst = 0;
}
int main(){
    printf("=== simulation start ====\n");
    nvboard_bind_all_pins(dut);
    nvboard_init();
    rst_funtion();
    int cycle_count = 0;
    int cycle_max = 10000; 
    while (cycle_count < cycle_max )
    {
        nvboard_update();
        for (int i = 0; i < 100000; i++)
        {
            single_cycle();
        }
        cycle_count++;
    }
    dut->final();
    delete dut;
    nvboard_quit();
    printf("=== simulation over ====\n");
    return 0;
}