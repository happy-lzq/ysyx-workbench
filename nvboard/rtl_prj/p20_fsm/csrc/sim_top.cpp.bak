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

void dump_down_wave(){
    dut->eval();
    tfp->dump(sim_time);
    sim_time +=5;    
}

void dump_up_wave(){
    dut->eval();
    printf("\ntime=%3lu : one=%d, half= %d, pi_quit=%d\n",sim_time,dut->one,dut->half,dut->pi_quit);
    printf("           po_cola=%d, po_money=%d, dout=%d\n",dut->po_cola,dut->po_money,dut->dout);
    tfp->dump(sim_time);
    sim_time +=5;
}

void set_input(int one,int half,int quit){
    dut->one = one;
    dut->half = half;
    dut->pi_quit = quit;
}

void single_cycle(int a,int b,int c) { 
    set_input(a,b,c);
    dut->clk = 0; 
    dump_down_wave();   
    dut->eval();
    dut->clk = 1; 
    dump_up_wave();

}

int main(){
    printf("\n=== gtkwave simulation start ===\n");
    verilator_init();

    // 初始化所有
    printf("\n[1] initial state :\n");
    dut->rst_n = 1;
    single_cycle(0,0,0);

    // 应用复位
    printf("\n[2] applying reset :\n");
    dut->rst_n = 0;
    for (int i = 0; i < 2; i++)
    {
        single_cycle(1,0,1);
    }

    // 释放复位
    printf("\n[3] releaseing reset :\n");
    dut->rst_n = 1;
    single_cycle(0,0,0);
   
    // 测试场景1：正常购买：1+0.5+0.5+0.5
    printf("\n[4] Test: Normal purchase (1+0.5+0.5+0.5)\n");
    single_cycle(1,0,0); 
    single_cycle(0,1,0);
    single_cycle(0,1,0);
    single_cycle(0,1,0);
    // 清零输入，观察状态
    single_cycle(0, 0, 0);

     // 测试场景2：中途退钱
    printf("\n[5] Test: Refund midway\n");
    single_cycle(0,1,0);
    single_cycle(1,0,0);
    single_cycle(0,0,1);     // 按退钱键
    single_cycle(0,0,0);    // 释放退钱键

    // 测试场景3：找零测试
    printf("\n[6] Test: Change (1 + 1 + 1 = 找零0.5)\n");
    single_cycle(1,0,0);
    single_cycle(0,1,0);
    single_cycle(0,1,0);
    single_cycle(1,0,0);
    single_cycle(0,0,0);

    // fsm连续四个0、1测试 
    printf("\n[7] Test:FSM)\n");       
    single_cycle(0,1,0);           
    single_cycle(0,1,0);           
    single_cycle(0,1,0);           
    single_cycle(0,1,0);           
    single_cycle(0,1,0);           
    single_cycle(1,0,0);           
    single_cycle(0,1,0);           
    single_cycle(0,0,0);           
    single_cycle(0,0,0);           
    single_cycle(0,0,0);           
    single_cycle(0,0,0);                
    single_cycle(0,0,0);                  
    single_cycle(0,1,0);           
    single_cycle(0,1,0);           
    single_cycle(1,0,0);          
    single_cycle(0,1,0);           
    single_cycle(0,0,0);           
    single_cycle(0,0,0);           
    single_cycle(0,0,0);           
    single_cycle(0,0,0);                 
    single_cycle(0,0,0); 
    // 结束延迟一个0输入时钟周期
    single_cycle(0,0,0);
    verilator_quit();
    printf("\n=== gtkwave simulation end ===\n");
}

/*
时序逻辑仿真设计：
    1：设计输入函数，单周期波形推进
    2：初始化所有，应用复位，释放复位 （每一步都推进仿真）
    3：正常输入场景测试，使用输入函数+单周期波形推进
*/