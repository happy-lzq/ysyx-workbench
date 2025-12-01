#include <verilated.h>
#include <verilated_vcd_c.h>
#include <Vtest.h>               
#include <stdio.h>
#include <assert.h>

static Vtest *dut = new Vtest;                
static vluint64_t sim_time = 0;
double sc_time_stamp(){return sim_time;}
static VerilatedVcdC *tfp = new VerilatedVcdC;

int main() {
    printf("=== Starting Simulation ===\n");
    // ===== NVBoard 初始化 ====
    Verilated::traceEverOn(true);
    dut->trace(tfp,99);
    tfp->open("test.vcd");

    // ===== 仿真循环 =====
    int count = 0 ;
    int cycle_max = 1000;
    while( count < cycle_max )
    {
        int m = rand() & 1;
        int n = rand() & 1;
        dut->a = m;
        dut->b = n;
        dut->eval();     
        tfp->dump(sim_time);
        sim_time +=10;                        
        printf("a = %d, b = %d, f = %d\n", dut->a, dut->b, dut->f);
        assert(dut->f == (m ^ n));
        count++;
    }
    
    // ===== 收尾 =====
    dut->final();
    tfp->close();
    delete tfp;
    delete dut;
    printf("\n=== simulation over ====\n");
    return 0;
    
}