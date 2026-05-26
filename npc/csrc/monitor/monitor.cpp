#include <sdb.h>
#include <difftest.h>

bool wave_enabled = false;
uint64_t sim_time = 0;
NPCSIM_State npc_sim_state;

void load_bin(Vcore_top* top,const char*path){
    Assert(path,"IMG-BIN-FILE IS FATL!\n");
    FILE* fp = fopen(path,"rb");
    if (!fp){
        fprintf(stderr, "Cannot open %s\n",path);
        exit(1);
    }
    // .bin load in imem 

    __uint8_t buf[4];
    while (fread(buf,1,4,fp) == 4){
        Assert((idx + 1) * 4 <= PMEM_SIZE, "image is too large for pmem\n");
        __uint32_t word = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
        npc_imem(top,idx,word,WRITE);           // bin load cpu-imem
        npc_dmem(top,idx,word,WRITE); 
        memcpy(&npc_pmem[idx * 4], buf, 4);     
        idx++;
    }
    img_size = idx * 4;  // 指令总数=idx 
    fclose(fp);
    printf("\nLoaded %d pc_addr to ref_mem and npc_mem form %s\n",idx,path);
}

int parse_agrs(int argc,char *argv[]){
    const struct option table[] = {
        {"bin"  , required_argument, NULL,'i'},
        {"diff" , required_argument, NULL,'d'},
        {"wave" , no_argument      , NULL,'w'},
        {"batch", no_argument      , NULL,'b'},
        {"help" , no_argument      , NULL,'h'},
        {0      , 0                , NULL, 0 }
    };
    int o;
    while ( (o = getopt_long(argc, argv, "-hbd:i:w", table, NULL)) != -1) {
        switch (o) {
            case 'i' : img_file     = optarg; break;
            case 'd' : diff_so_file = optarg; break;
            case 'w' : wave_enabled = true  ; break;
            case 'b' : sdb_set_batch_mode() ; break;
            default  : exit(0);
        }
    }
    return 0;
}

void halt(){
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
        if (npc_sim_state.halt_ret >= 0)
            Log("ABORT: gpr[%d] mismatch at pc=0x%08x",
                npc_sim_state.halt_ret, npc_sim_state.halt_pc);
        else
            Log("ABORT at pc=0x%08x", npc_sim_state.halt_pc);
        break;
    }
}

void npc_init(){
    // 复位
    top->clk = 0; top->rst = 1; top->eval();
    if (tfp) tfp->dump(sim_time+=5);

    top->clk = 1; top->rst = 1; top->eval();                  // 上升沿，rst=1复位 初始化
    if (tfp) tfp->dump(sim_time+=5);
    npc_sim_state.state = NPC_STOP;
    load_bin(top, img_file);
    top->clk = 0; top->rst = 0; top->eval();
    if (tfp) tfp->dump(sim_time+=5);
}


