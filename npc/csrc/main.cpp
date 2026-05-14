#include <difftest.h>

NPC_state npc_s, ref_s;
Vcore_top *top = new Vcore_top;
int idx =0;
const char* img_path = NULL;
const char* diff_so_path = NULL;
long img_size = 0;
uint8_t npc_pmem[PMEM_SIZE];

uint8_t* guest_to_host(paddr_t paddr) {
    return npc_pmem + paddr - RESET_VECTOR;
}

static void load_bin(Vcore_top* top,const char*path){
    FILE* fp = fopen(path,"rb");
    if (!fp){
        fprintf(stderr, "Cannot open %s\n",path);
        exit(1);
    }
    // .bin load in imem 

    __uint8_t buf[4];
    while (fread(buf,1,4,fp) == 4){
        __uint32_t word = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
        npc_imem(top,idx,word,WRITE);
        memcpy(&npc_pmem[idx * 4], buf, 4);
        idx++;
    }
    img_size = idx * 4;  // 指令总数=idx 
    fclose(fp);
    printf("Loaded %d pc_addr form %s",idx,path);
}

void single_cycle(){
    top->clk = 1; top->eval();
    top->clk = 0; top->eval();  
}

int main(int argc, char* argv[]){
    Verilated::commandArgs(argc, argv); 
    for (int i = 0; i < argc; i++){
        if (strncmp(argv[i],"--bin=",6)== 0 ){
            img_path = argv[i] + 6;
        }
        if (strncmp(argv[i],"--diff=",7)== 0){
            diff_so_path = argv[i] + 7;
        }
    }

    if (img_path) {
        load_bin(top, img_path);
    } else {
        printf("No --img= specified, imem is all zeros (NOPs)\n");
    }
    
    // 复位
    top->clk = 0; top->rst = 1; top->eval();
    top->clk = 1; top->eval();                  // 上升沿，rst=1复位 初始化
    top->clk = 0; top->rst = 0; top->eval();
    // difftest-exec
    if (diff_so_path) {
        init_difftest(diff_so_path, img_size);
    }
    for (int i = 0; i < idx; i++){
        printf("cycle %d  pc = 0x%08x\n",i,top->debug_pc);
        single_cycle();
        difftest_step(top);
    }
    delete top ;
    return 0;
}
