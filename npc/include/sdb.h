#ifndef __NPC_SDB_H__
#define __NPC_SDB_H__

#include <npc.h>
#include <getopt.h>

extern const char *diff_so_file;
extern const char *img_file ;
extern int parse_agrs(int argc,char *argv[]);
extern const char *build_named_log_file(char *buf, size_t buf_size,
                                        const char *path,
                                        const char *default_path,
                                        const char *suffix);

#endif