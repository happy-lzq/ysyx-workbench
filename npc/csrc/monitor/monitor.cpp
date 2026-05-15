#include <sdb.h>

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