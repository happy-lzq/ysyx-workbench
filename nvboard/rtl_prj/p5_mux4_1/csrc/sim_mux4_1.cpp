#include <verilated.h>
#include <verilated_vcd_c.h>
#include <Vmux4_1.h>
#include <stdio.h>

static Vmux4_1 *dut = NULL;
static VerilatedVcdC *tfp = NULL;
static vluint64_t sim_time = 0;
double sc_time_stamp(){return sim_time;}

void verilator_init(){
   dut = new Vmux4_1;
   tfp = new VerilatedVcdC;
   Verilated::traceEverOn(true);
   dut->trace(tfp,10);
   tfp->open("mux4_1.vcd");
}

void verilator_quit(){
    dut->final();
    tfp->close();
    delete dut;
    delete tfp;
}

void dump_wave(){
    dut->eval();
    sim_time +=5;
    tfp->dump(sim_time);
}
int main(){
    printf("\n=== simulation start ====\n");
    verilator_init();
    for (int i = 0; i < 4; i++)
    {
        switch (i)
        {
            case 0 :
                dut->s=0b00;   dut->a=0b1110;dump_wave();
                dut->a=0b1010; dump_wave();
                break;
            case 1 :
                dut->s=0b01;   dut->a=0b1110;dump_wave();
                dut->a=0b0011; dump_wave();
                break;
            case 2 :
                dut->s=0b10;   dut->a=0b1010;dump_wave();
                dut->a=0b1100; dump_wave();
                break;
            case 3 :
                dut->s=0b11;   dut->a=0b0111;dump_wave();
                dut->a=0b0101; dump_wave();
                break;
            default:
                break;
            }
    }
    verilator_quit();
    
}