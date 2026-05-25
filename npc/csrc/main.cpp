
#include <difftest.h>
#include <sdb.h>


Vcore_top *top = NULL;
VerilatedVcdC* tfp = NULL;
NPC_state npc_s, ref_s;

int idx =0;
long img_size = 0;
int cycle = 0;
const char* img_file = NULL;
const char* diff_so_file = NULL;
uint8_t npc_pmem[PMEM_SIZE];

void single_cycle(){
    top->clk = 1; top->eval();
    halt();
    if (tfp) tfp->dump(sim_time+=5);
    top->clk = 0; top->eval();  
    if (tfp) tfp->dump(sim_time+=5);
}

int main(int argc, char* argv[]){
    Verilated::commandArgs(argc, argv);
    top = new Vcore_top;
    parse_agrs(argc,argv);
    
    if (wave_enabled) {
        Verilated::traceEverOn(true);
        tfp = new VerilatedVcdC;
        top->trace(tfp, 99);
        tfp->open("build/wave.vcd");
    }

    load_bin(top, img_file);
    npc_init();

    // difftest-exec
    if (diff_so_file) {
        init_diff_log(img_file);
        init_difftest(diff_so_file, img_size);
    }
    while (npc_sim_state.state == NPC_RUNNING && cycle < MAX_CYCLE ){
        single_cycle();
        difftest_step(top,cycle);
        cycle++;
    }
    
    npc_state_check();

    if (tfp) {
        tfp->close();
        delete tfp;
    }
    delete top ;
    return 0;
}
