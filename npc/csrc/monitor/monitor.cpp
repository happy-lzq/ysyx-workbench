#include <sdb.h>
#include <difftest.h>


void load_bin(Vcore_top* top,const char*path){
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

int parse_agrs(int argc,char *argv[]){
    const struct option table[] = {
        {"bin"  , required_argument, NULL,'i'},
        {"diff" , required_argument, NULL,'d'},
        {"help" , no_argument      , NULL,'h'},
        {0      , 0                , NULL, 0 }
    };
    int o;
    while ( (o = getopt_long(argc, argv, "-h:d:i:", table, NULL)) != -1) {
        switch (o) {
            case 'i' : img_file     = optarg; break;
            case 'd' : diff_so_file = optarg; break;
            default  : exit(0);
        }
    }
    return 0;
}