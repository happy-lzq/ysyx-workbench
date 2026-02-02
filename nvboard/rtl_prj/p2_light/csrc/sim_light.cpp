#include <stdio.h>
#include <Vlight.h>
#include <verilated.h>
#include <verilated_vcd_c.h>


static Vlight *dut = new Vlight;
static vluint64_t sim_time = 0;
static VerilatedVcdC *tfp = new VerilatedVcdC;
double sc_time_stamp(){return sim_time;}

// 电路单周期更替，模拟周期
static void single_cycle() {
    dut->clk = 0; 
    dut->eval();
    tfp->dump(sim_time);
    sim_time +=10;

    dut->clk = 1; 
    dut->eval();
    tfp->dump(sim_time);
    sim_time+=10;
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
    Verilated::traceEverOn(true);
    dut->trace(tfp,99);
    tfp->open("light.vcd");
    rst_funtion();
    int cycle_count = 0;
    int cycle_max = 100; 
    while (cycle_count < cycle_max )
    {
        single_cycle();
        cycle_count ++;
    }
    tfp->close();
    dut->final();
    delete dut;
    delete tfp;
    printf("=== simulation over ====\n");
    return 0;
}