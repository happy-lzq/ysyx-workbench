#include "Vcore_top.h"
#include "verilated.h"
#include "stdio.h"

   Vcore_top* top = new Vcore_top;
void single_cycle(){
    top->clk = 1; top->eval();
    top->clk = 0; top->eval();  
}

int main(){
    // 复位
    top->clk = 0; top->rst = 1; top->eval();
    top->clk = 1; top->eval();                  // 上升沿，rst=1复位 初始化
    top->clk = 0; top->rst = 0; top->eval();
    for (int i = 0; i < 10; i++){
        single_cycle();
        printf("cycle %d  pc = 0x%08x\n",i,top->debug_pc);
    }
    delete top ;
    return 0;
}