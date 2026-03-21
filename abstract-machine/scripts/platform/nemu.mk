AM_SRCS := platform/nemu/trm.c \
           platform/nemu/ioe/ioe.c \
           platform/nemu/ioe/timer.c \
           platform/nemu/ioe/input.c \
           platform/nemu/ioe/gpu.c \
           platform/nemu/ioe/audio.c \
           platform/nemu/ioe/disk.c \
           platform/nemu/mpe.c

CFLAGS    += -fdata-sections -ffunction-sections
CFLAGS    += -I$(AM_HOME)/am/src/platform/nemu/include
LDSCRIPTS += $(AM_HOME)/scripts/linker.ld

# 链接定义：结合linke.ld 定义_pmem_start 和entry_offset 规定程序链接地址从 0x80000000 开始
# 链接地址 = 加载地址 = CPU 启动地址 = 0x80000000 从而确定.bin文件对应的程序指令数据内容是从0x80000000开始
LDFLAGS   += --defsym=_pmem_start=0x80000000 --defsym=_entry_offset=0x0
LDFLAGS   += --gc-sections -e _start
NEMUFLAGS += -l $(shell dirname $(IMAGE).elf)/nemu-log.txt      
NEMUFLAGS += -e $(IMAGE).elf -i $(IMAGE).bin
#NEMUFLAGS += -b 
#增加 -b 选择nemu平台的批处理模式
MAINARGS_MAX_LEN = 64
MAINARGS_PLACEHOLDER = the_insert-arg_rule_in_Makefile_will_insert_mainargs_here
CFLAGS += -DMAINARGS_MAX_LEN=$(MAINARGS_MAX_LEN) -DMAINARGS_PLACEHOLDER=$(MAINARGS_PLACEHOLDER)
# 在abstract-machine/Makefile 中确定 image-dep 对$(IMAGE).elf 链接规则依赖
insert-arg: image
	@python $(AM_HOME)/tools/insert-arg.py $(IMAGE).bin $(MAINARGS_MAX_LEN) $(MAINARGS_PLACEHOLDER) "$(mainargs)"
# 本系统仅有 python3 并无python 而是使用软链接至python3   
# -O $(IMAGE).elf &(IMGAE).bin   构建.bin文件
image: image-dep
	@$(OBJDUMP) -d $(IMAGE).elf > $(IMAGE).txt
	@echo + OBJCOPY "->" $(IMAGE_REL).bin
	@$(OBJCOPY) -S --set-section-flags .bss=alloc,contents -O binary $(IMAGE).elf $(IMAGE).bin

run: insert-arg
	$(MAKE) -C $(NEMU_HOME) ISA=$(ISA) run ARGS="$(NEMUFLAGS)"
# 对应到命令行参数匹配：
# 长选项用 --长选项=值 或 --长选项 值。
# 短选项用 -短选项 值（或紧跟值）。
# 添加：ARGS IMG 命令行参数变量传给nemu启动命令行      -C 切换目录
# nemu 启动行真实命令：nemu -b -l /path/nemu-log.txt -e /path/string-riscv32-nemu.elf -i /path/string-riscv32-nemu.bin

gdb: insert-arg
	$(MAKE) -C $(NEMU_HOME) ISA=$(ISA) gdb ARGS="$(NEMUFLAGS)"

.PHONY: insert-arg
# run/gdb ——> insert-arg ——> image ——> image-dep ——> abstract-machine/Makefile中 $(IMAGE).elf ——> (LINKAGE) 和 $(LDSCRIPTS) 链接