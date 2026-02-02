#include <stdio.h>
#include <nvboard.h>
#include <Vtop.h>

static Vtop *dut= new Vtop;
void nvboard_bind_all_pins(Vtop *top);
 
int main(){
    printf("=== nvboard simulation start===");
    nvboard_bind_all_pins(dut);
    nvboard_init();

    while (1)
    {
        nvboard_update();
        dut->eval();
    }
    
    dut->final();
    delete dut;
    nvboard_quit();
    printf("\n=== nvboard simulation end ===\n");
    return 0;
}
