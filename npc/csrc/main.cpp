#include <difftest.h>
#include <getopt.h>
#include <sdb.h>
NPC_state npc_s, ref_s;
DEBUG_FILE_PATH dfp;
Vcore_top *top = new Vcore_top;
int idx =0;
const char* img_file = NULL;
const char* diff_so_file = NULL;
long img_size = 0;
uint8_t npc_pmem[PMEM_SIZE];




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
    parse_agrs(argc,argv);
    
    if (img_file) {
        load_bin(top, img_file);
    } else {
        printf("No --img= specified, imem is all zeros (NOPs)\n");
    }
    
    // 复位
    top->clk = 0; top->rst = 1; top->eval();
    top->clk = 1; top->eval();                  // 上升沿，rst=1复位 初始化
    printf("\ncycle %d  pc = 0x%08x\n",0,top->debug_pc);
    top->clk = 0; top->rst = 0; top->eval();
    // difftest-exec
    if (diff_so_file) {
        init_difftest(diff_so_file, img_size);
    }
    for (int i = 1; i < idx+1; i++){
        single_cycle();
        difftest_step(top);
        printf("cycle %d  pc = 0x%08x\n",i,top->debug_pc);
    }
    delete top ;
    return 0;
}
