#include <sdb.h>
#include <difftest.h>
#include <trace.h>
#include <memory.h>

uint64_t sim_time = 0;
NPCSIM_State npc_sim_state;
const char *elf_file = NULL;

int parse_agrs(int argc,char *argv[]){
    const struct option table[] = {
        {"elf"  , required_argument, NULL,'e'},
        {"bin"  , required_argument, NULL,'i'},
        {"diff" , required_argument, NULL,'d'},
        {"help" , no_argument      , NULL,'h'},
        {0      , 0                , NULL, 0 }
    };
    int o;
    while ( (o = getopt_long(argc, argv, "-hd:e:i:", table, NULL)) != -1) {
        switch (o) {
            case 'e' : elf_file     = optarg; break;
            case 'i' : img_file     = optarg; break;
            case 'd' : diff_so_file = optarg; break;
            default  : exit(0);
        }
    }
    return 0;
}

void halt_check(){
        if (top->halt){
        npc_sim_state.state    = NPC_END;
        npc_sim_state.halt_pc  = top->halt_pc;
        npc_sim_state.halt_ret = top->halt_ret;
    }
}

void npc_state_check() {
    switch (npc_sim_state.state) {
    case NPC_END:
        if (npc_sim_state.halt_ret == 0)
            Log("HIT GOOD TRAP at pc=0x%08x", npc_sim_state.halt_pc);
        else
            Log("HIT BAD TRAP at pc=0x%08x, ret=%d", 
                npc_sim_state.halt_pc, npc_sim_state.halt_ret);
        break;
    case NPC_ABORT:
        if (npc_sim_state.halt_ret == -1)
            Log("ABORT: PC mismatch at pc=0x%08x", npc_sim_state.halt_pc);
        else if (npc_sim_state.halt_ret < 32)
            Log("ABORT: GPR[%d] mismatch at pc=0x%08x",
                npc_sim_state.halt_ret, npc_sim_state.halt_pc);
        else
            Log("ABORT: CSR[%d] mismatch at pc=0x%08x",
                npc_sim_state.halt_ret - 32, npc_sim_state.halt_pc);
        break;
    }
}

void npc_init(){
    // 复位
    top->interrupt_valid = 0;
    top->interrupt_cause = 0;
    top->clk = 0; top->rst = 1; top->eval();
    if (tfp) tfp->dump(sim_time+=5);
    top->clk = 1; top->rst = 1; top->eval();                  // 上升沿，rst=1复位 初始化
    npc_sim_state.state = NPC_STOP;

    if (tfp) tfp->dump(sim_time+=5);

    top->clk = 0; top->rst = 0; top->eval();
    if (tfp) tfp->dump(sim_time+=5);
}

void assert_fail_msg() {
    isa_reg_display();
    #ifdef CONFIG_ITRACE
        assert_fail_msg();
    #endif
}


void monitor_init(int argc, char* argv[]){
    parse_agrs(argc,argv);
    pmem_init();
    pmem_load_bin(img_file);
    npc_init();
    #ifdef CONFIG_DIFFTEST 
        difftest_init();
    #endif
    #ifdef CONFIG_ITRACE
        init_disasm();
        itarce_log_file(img_file);
    #endif
    init_sdb();  

}
