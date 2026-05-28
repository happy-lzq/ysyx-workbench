
#include <difftest.h>
#include <sdb.h>
#include <trace.h>

Vcore_top *top = NULL;
VerilatedVcdC* tfp = NULL;
NPC_state npc_s, ref_s;

int idx =0;
int cycle = 0;
uint8_t npc_pmem[PMEM_SIZE];

void single_cycle(){
    uint32_t this_pc = npc_pc(top, 0, READ);
    uint32_t this_inst = npc_imem(top, (this_pc - RESET_VECTOR) >> 2, 0, READ);
    top->clk = 1; top->eval();
    halt_check();
    // printf("\n[cycle %d] pc=0x%08x trap_enter=%d mret=%d trap_target=0x%08x\n",
    //    cycle,
    //    npc_pc(top, 0, READ),
    //    top->rootp->core_top__DOT__trap_enter,
    //    top->rootp->core_top__DOT__mret,
    //    top->rootp->core_top__DOT__u_csr__DOT__csr_mtvec);
    // isa_reg_display(); 

    #ifdef CONFIG_DIFFTEST 
    if (diff_so_file) {
        difftest_step(top, cycle);
    }
    #endif
    
    #ifdef CONFIG_WATCHPOINT
        if (npc_sim_state.state == NPC_RUNNING && check_watchpoint(&used_list) > 0) {
            npc_sim_state.state = NPC_STOP;
        }
    #endif

    #ifdef CONFIG_ITRACE
        itrace_log(this_pc, this_inst);
        
    #endif

    npc_state_check();
    
    if (tfp) tfp->dump(sim_time+=5);
    top->clk = 0; top->eval();  
    if (tfp) tfp->dump(sim_time+=5);
}

void npc_exec(){
    while (npc_sim_state.state != NPC_QUIT) {
        switch (npc_sim_state.state) {
        case NPC_RUNNING:
            single_cycle();
            cycle++;
            break;
        case NPC_STOP:
            sdb_mainloop();  
            break;
        case NPC_END:
        case NPC_ABORT:
            return;
        }
    }
}

int main(int argc, char* argv[]){
    Verilated::commandArgs(argc, argv);
    top = new Vcore_top;

    #ifdef CONFIG_WAVE
        Verilated::traceEverOn(true);
        tfp = new VerilatedVcdC;
        top->trace(tfp, 99);
        tfp->open("build/wave.vcd");
    #endif

    parse_agrs(argc,argv);
    npc_init();
    init_sdb();  
    #ifdef CONFIG_ITRACE
        init_disasm();
    #endif
    npc_exec();

    if (tfp) {
        tfp->close();
        delete tfp;
    }
    delete top ;
    // 根据真实状态返回退出码
    if (npc_sim_state.state == NPC_END && npc_sim_state.halt_ret == 0)
        return 0;   // 真正跑完且通过
    else
        return 1;   // 非正常退出
}

/*

NPC_END (ebreak):
  halt_ret = x10 (a0)    → "程序的退出码"
  意义: 0=测试通过, 非0=测试失败

NPC_ABORT (difftest mismatch):
  halt_ret = 出错的寄存器号  → "哪个寄存器不匹配"
  意义: 告诉 npc_state_check 打印详细的错误信息

NPC_ABORT (断言/非法指令):
  halt_ret = -1           → "非正常终止"
  意义: 标记异常
*/
