#include <stdio.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include <Vtop.h>
#include <math.h>

static Vtop *dut = new Vtop;
static VerilatedVcdC *tfp = new VerilatedVcdC;
static vluint64_t sim_time = 0;
double sc_time_stamp(){return sim_time;}

void verilator_init(){
    Verilated::traceEverOn(true);
    dut->trace(tfp,99);
    tfp->open("top.vcd");
}

void verilator_quit(){

    tfp->close();
    dut->final();
    delete dut;
    delete tfp;
}
void dump_wave(){
    dut->eval();
    printf("Time: %3lu,      dout= 0x[%2x]\n", sim_time,dut->dout);
    sim_time +=5;
    tfp->dump(sim_time);
}

static void single_cycle() {
    dut->clk = 0; 
    dump_wave();
    dut->clk = 1; 
    dump_wave();
}



int main(){
    printf("\n=== gtkwave simulation start ===\n");
    verilator_init();
    
    printf("\nInitial state (no reset):   \n");
    dump_wave();
    
    // 应用复位
    dut->rst = 1;
    printf("\nAfter asserting reset:      \n");
    dump_wave();
    
    // 运行几个周期确保复位稳定
    for (int i = 0; i < 2; i++) {
        printf("Cycle %2d under reset:       \n", i);
        single_cycle();
    }
    
    // 释放复位
    dut->rst = 0;
    printf("\nAfter releasing reset:      \n");
    dump_wave();
    
    // 正常运行
    for (int i = 0; i < 10; i++) {
        single_cycle();
    }
    
    verilator_quit();
    printf("\n=== gtkwave simulation end ===\n");
}