#include "Vcore_top.h"
#include "Vcore_top___024root.h" 
#include "verilated.h"
#include "stdio.h"

Vcore_top* top = new Vcore_top;
const char* img_path = NULL;

static void load_bin(Vcore_top* top,const char*path){
    FILE* fp = fopen(path,"rb");
    if (!fp){
        fprintf(stderr, "Cannot open %s\n",path);
        exit(1);
    }
    // .bin load in imem 
    int idx =0;
    __uint8_t buf[4];
    while (fread(buf,1,4,fp) == 4){
        __uint32_t word = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
        top->rootp->core_top__DOT__u_if_stage__DOT__imem[idx] = word;
        idx++;
    }
    fclose(fp);
    printf("Loaded %d inst form %s",idx,path);
}

void single_cycle(){
    top->clk = 1; top->eval();
    top->clk = 0; top->eval();  
}

int main(int argc, char* argv[]){
    Verilated::commandArgs(argc, argv); 
    for (int i = 0; i < argc; i++){
        if (strncmp(argv[i],"--img=",6)==0)
        {
            img_path = argv[i]+6;
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
    for (int i = 0; i < 10; i++){
        single_cycle();
        printf("cycle %d  pc = 0x%08x\n",i,top->debug_pc);
    }
    delete top ;
    return 0;
}