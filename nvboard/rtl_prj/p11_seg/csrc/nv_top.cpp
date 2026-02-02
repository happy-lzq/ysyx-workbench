#include <stdio.h>
#include <nvboard.h>
#include <Vtop.h>

static Vtop *dut= new Vtop;
void nvboard_bind_all_pins(Vtop *top);
 
int main(){
    printf("=== nvboard simulation start===");
    nvboard_bind_all_pins(dut);
    nvboard_init();
    int count = 0;
    int cycle_max = 100000;
    while (count < cycle_max)
    {
        for (int i = 0; i < 100000; i++)
        {
            nvboard_update();
            dut->eval();
        }
        count++;
    }
    dut->final();
    delete dut;
    nvboard_quit();
    printf("\n=== nvboard simulation end ===\n");
    return 0;
}
