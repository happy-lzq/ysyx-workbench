#include <verilated_vcd_c.h>
#include <difftest.h>
#include <getopt.h>
#include <sdb.h>


NPC_state npc_s, ref_s;
Vcore_top *top = NULL;
VerilatedVcdC* tfp = NULL;
uint64_t sim_time = 0;
int idx =0;
long img_size = 0;

const char* img_file = NULL;
const char* diff_so_file = NULL;
uint8_t npc_pmem[PMEM_SIZE];

void single_cycle(){
    top->clk = 1; top->eval();
    if (tfp) tfp->dump(sim_time++);
    top->clk = 0; top->eval();  
    if (tfp) tfp->dump(sim_time++);
}

int main(int argc, char* argv[]){
    Verilated::commandArgs(argc, argv);
    Verilated::traceEverOn(true);
    top = new Vcore_top;
    parse_agrs(argc,argv);

    tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("build/wave.vcd");

    load_bin(top, img_file);
    // 复位
    top->clk = 0; top->rst = 1; top->eval();
    top->clk = 1; top->eval();                  // 上升沿，rst=1复位 初始化
    printf("\ncycle %d  pc = 0x%08x\n",0,top->debug_pc);
    top->clk = 0; top->rst = 0; top->eval();
    // difftest-exec
    if (diff_so_file) {
        init_diff_log(img_file);
        init_difftest(diff_so_file, img_size);
    }
    for (int i = 1; i < idx+1; i++){
        single_cycle();
        difftest_step(top,i);
    }
    
    if (tfp) {
        tfp->close();
        delete tfp;
    }
    delete top ;
    return 0;
}
