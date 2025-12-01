#include <stdio.h>
#include <nvboard.h>
#include <Vtop.h>

static Vtop *dut= new Vtop;
void nvboard_bind_all_pins(Vtop *top);
 
static void single_cycle() {
    dut->clk = 0; 
    dut->eval();

    dut->clk = 1; 
    dut->eval();
}

static void top_init (){
    printf("top 初始化\n");
    for (int i = 0; i < 16; i++)
    {
        dut->w_addr = i;
        dut->w_data = i * 0x11;
        dut->we = 1;
        single_cycle();
        dut->we = 0;
        single_cycle();
    }
}

int main(){
    printf("=== nvboard simulation start===");
    top_init();
    nvboard_bind_all_pins(dut);
    nvboard_init();
    while (1)
    {
        nvboard_update();
        single_cycle();
    }
    
    dut->final();
    delete dut;
    nvboard_quit();
    printf("\n=== nvboard simulation end ===\n");
    return 0;
}
