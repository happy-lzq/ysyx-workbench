#include <stdio.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include <Vencode4_2.h>
#include <math.h>

static Vencode4_2 *dut = new Vencode4_2;
static VerilatedVcdC *tfp = new VerilatedVcdC;
static vluint64_t sim_time = 0;
double sc_time_stamp(){return sim_time;}

void verilator_init(){
    Verilated::traceEverOn(true);
    dut->trace(tfp,99);
    tfp->open("encode4_2.vcd");
}

void verilator_quit(){

    tfp->close();
    dut->final();
    delete dut;
    delete tfp;
}

void dump_wave(){
    dut->eval();
    printf("Time: %lu, en=%d, x=0x%x, y=%d\n", sim_time, dut->en, dut->x, dut->y);
    sim_time +=5;
    tfp->dump(sim_time);
}

int main(){
    printf("\n=== gtkwave simulation start ===\n");
    verilator_init();
    int count = 0;
    int cycle_max = 4;
    dut->en = 0;
    dump_wave();
    dut->en  =1;
    for (int i = 0; i < 2; i++)
    {
        while (count < cycle_max && dut->en==1) {
            dut->x=pow(2,count);                //2的n次放进移位
            dump_wave();
            count++;  
        }
        count = 0;
        printf("\n");
    }
    dump_wave();
    verilator_quit();
    printf("\n=== gtkwave simulation end ===\n");
}