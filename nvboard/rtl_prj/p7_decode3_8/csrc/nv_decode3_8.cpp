#include <stdio.h>
#include <Vdecode3_8.h>
#include <nvboard.h>

static Vdecode3_8 *dut = new Vdecode3_8;
void nvboard_bind_all_pins(Vdecode3_8 *top);
int main(){
    printf("\n=== nvboard simulation start ===\n");
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

