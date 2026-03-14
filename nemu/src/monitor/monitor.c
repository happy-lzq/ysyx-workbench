/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <memory/paddr.h>

void init_rand();
void init_log(const char *log_file);    
void init_mem();
void init_difftest(char *ref_so_file, long img_size, int port);
void init_device();
void init_sdb();
void init_disasm();

static void welcome() {
  Log("Trace: %s", MUXDEF(CONFIG_TRACE, ANSI_FMT("ON", ANSI_FG_GREEN), ANSI_FMT("OFF", ANSI_FG_RED)));
  IFDEF(CONFIG_TRACE, Log("If trace is enabled, a log file will be generated "
        "to record the trace. This may lead to a large log file. "
        "If it is not necessary, you can disable it in menuconfig"));
  Log("Build time: %s, %s", __TIME__, __DATE__);
  printf("Welcome to %s-NEMU!\n", ANSI_FMT(str(__GUEST_ISA__), ANSI_FG_YELLOW ANSI_BG_RED));
  printf("For help, type \"help\"\n");
  //Log("Exercise: Please remove me in the source code and compile NEMU again.");
  // assert(0);
}

#ifndef CONFIG_TARGET_AM
#include <getopt.h>
#include <limits.h>
#include <string.h>

void sdb_set_batch_mode();            // 外部批处理函数声明
static char *log_file = NULL;         // 日志文件路径 对应（-l）
static char *diff_so_file = NULL;     // 差分测试参考实现库路径，对应（-d）
static char *img_file = NULL;         // 客户程序镜像路径
static char *elf_file = NULL;
static int difftest_port = 1234;      // 差分测试端口，对应（-p）

// ================================= 基于命令的路径处理 ===========================================
static const char *build_named_log_file(char *buf, size_t buf_size,
                                        const char *path,
                                        const char *default_path,
                                        const char *suffix) {
  if (path == NULL) {
    return default_path;
  }

  const char *slash = strrchr(path, '/');
  const char *name = slash == NULL ? path : slash + 1;              // 获取文件名起始地址
  const char *dot = strrchr(name, '.');

  size_t dir_len = slash == NULL ? 0 : (size_t)(slash - path + 1);  // 获取目录长度包括/
  size_t base_len = (dot != NULL && dot > name) ? (size_t)(dot - name) : strlen(name);   // 获取名字长度
// ret 返回值大于0  ret < buf_size 无溢出
// 拼接逻辑：fix
  int ret = snprintf(buf, buf_size, "%.*s%.*s-%s",
                     (int)dir_len, path,
                     (int)base_len, name,
                     suffix);
  // 按格式化将对应的内容写入buf，且最大数据为buf_size字节大小
  // %.*s 中：.* 对应(int)的动态长度，s 对应字符串
  Assert(ret > 0 && ret < buf_size, "log path is too long: %s", path);
  // Assert(条件，输出文本) 条件成立，不输出；条件不成立，中断输出红色文本
  return buf;
}


// ================================== Mtrace_log 路径处理 ==========================================
#ifdef CONFIG_MTRACE
static char mtrace_log_file[260] = {};
static const char *get_mtrace_log_file() {
  const char *path = img_file != NULL ? img_file : log_file;
  return build_named_log_file(mtrace_log_file, sizeof(mtrace_log_file),
                              path, "build/mtrace-log.txt", "mtrace-log.txt");
}
#endif

// ================================== Ftrace_log ==============================================
#ifdef CONFIG_FTRACE
static char ftrace_log_file[260] = {};

static const char *get_ftrace_log_file() {
  return build_named_log_file(ftrace_log_file, sizeof(ftrace_log_file),
                              elf_file, "build/ftrace-log.txt", "ftrace-log.txt");
}
#endif

// ================================== 加载客户程序镜像函数 ==========================================
static long load_img() {
  // 区别于内部的 img[]指令数组，检查外部镜像文件是否存在，存在则执行镜像文件往物理内存起始地址加载覆盖内置指令数据内容
  if (img_file == NULL) {
    Log("No image is given. Use the default build-in image.");
    return 4096; // built-in image size
  }
  FILE *fp = fopen(img_file, "rb");
  Assert(fp, "Can not open '%s'", img_file);
  fseek(fp, 0, SEEK_END);          
  long size = ftell(fp); 
  Log("The image is %s, size = %ld", img_file, size);
  fseek(fp, 0, SEEK_SET);
  int ret = fread(guest_to_host(RESET_VECTOR), size, 1, fp);
  assert(ret == 1);
  fclose(fp);
  return size;
}
// ================ 配置 nemu 启动命令行的选项和参数进行解析 写入nmeu的全局配置变量 =================
static int parse_args(int argc, char *argv[]) {
// {name, has_arg, flag, val}   GNU/Linux 提供的命令行解析库 getopt_long() 规定的接口格式
  const struct option table[] = {
    {"batch"    , no_argument      , NULL, 'b'},
    {"log"      , required_argument, NULL, 'l'},
    {"diff"     , required_argument, NULL, 'd'},
    {"port"     , required_argument, NULL, 'p'},
    {"elf"      , required_argument, NULL, 'e'},
    {"help"     , no_argument      , NULL, 'h'},
    {0          , 0                , NULL,  0 },       // 选项表结束标记
  };
  int o;  
/*
  1、它是 GNU 提供的命令行参数解析函数，可以同时处理：短(-b)-长选项(--batch) 以及非选项参数(.bin)
  2、格式串："-bhl:d:p"；
    最前'-':如果遇到“不是选项”的普通参数——>特殊返回值交出来,返回值：1 (case 1)
    bh 表示不带参；l:d:p: 必须带参
    -b                批处理
    -h                帮助
    -l file           log文件
    -d ref.so         差分文件
    -p 1234           端口
  3、optarg ：选项后面对应的参数字符串

*/ 
  while ( (o = getopt_long(argc, argv, "-bhl:d:p:e:", table, NULL)) != -1) {
    switch (o) {
      case 'b': sdb_set_batch_mode(); break;
      case 'p': sscanf(optarg, "%d", &difftest_port); break;
      case 'e': elf_file = optarg;  break;        // elf_file = /path/NAME-riscv32-nmeu.elf
      case 'l': log_file = optarg;  break;        // log_file = /path/nemu-log.txt
      case 'd': diff_so_file = optarg; break;      
      case 1: img_file = optarg; break;           // img_file = /path/NAME-riscv32-nemu.bin
      default:
        printf("Usage: %s [OPTION...] IMAGE [args]\n\n", argv[0]);
        printf("\t-b,--batch              run with batch mode\n");      
        printf("\t-l,--log=FILE           output log to FILE\n");
        printf("\t-d,--diff=REF_SO        run DiffTest with reference REF_SO\n");
        printf("\t-p,--port=PORT          run DiffTest with port PORT\n");
        printf("\t-e,--elf=FILE           use ELF file for ftrace\n");
        printf("\n");
        exit(0);
    }
  }
  return 0;
}

// ============================================ ftrace ==========================================





void init_monitor(int argc, char *argv[]) {
  /* Perform some global initialization. */

  /* 用户启动 NEMU 时在命令行输入的选项和参数，解析后写入 NEMU 的全局配置变量里，供后面的初始化流程使用 */
  parse_args(argc, argv);

  /* Set random seed. */
  init_rand();

  /* Open the log file. */
  init_log(log_file);

  IFDEF(CONFIG_MTRACE, init_mtrace_log(get_mtrace_log_file()));
  // 条件避开最小内置镜像 无elf文件的处理方式。
    #ifdef CONFIG_FTRACE
      if (elf_file != NULL) {
        init_ftrace_log(get_ftrace_log_file());
        init_ftrace(elf_file);
      } else {
        Log("ftrace is enabled but --elf is missing, skip ftrace initialization");
      }
    #endif

  /* Initialize memory. */
  init_mem();

  /* Initialize devices. */
  IFDEF(CONFIG_DEVICE, init_device());

  /* 准备最小内置程序镜像，初始化CPU相关状态 */
  init_isa();

  /* 外部镜像存在，则加载镜像数据覆盖物理内存内置的最小程序镜像
     NEMU 把外部镜像字节流加载到“宿主机进程中的 pmem 数组”里 */
  long img_size = load_img();

  /* Initialize differential testing. */
  init_difftest(diff_so_file, img_size, difftest_port);

  /* Initialize the simple debugger. */
  init_sdb();

  IFDEF(CONFIG_ITRACE, init_disasm());

  /* Display welcome message. */
  welcome();
}
#else // CONFIG_TARGET_AM
static long load_img() {
  extern char bin_start, bin_end;
  size_t size = &bin_end - &bin_start;
  Log("img size = %ld", size);
  memcpy(guest_to_host(RESET_VECTOR), &bin_start, size);
  return size;
}

void am_init_monitor() {
  init_rand();
  init_mem();
  init_isa();
  load_img();
  IFDEF(CONFIG_DEVICE, init_device());
  welcome();
}
#endif
