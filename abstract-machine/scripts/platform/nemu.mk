AM_SRCS := platform/nemu/trm.c \
           platform/nemu/ioe/ioe.c \
           platform/nemu/ioe/timer.c \
           platform/nemu/ioe/input.c \
           platform/nemu/ioe/gpu.c \
           platform/nemu/ioe/audio.c \
           platform/nemu/ioe/disk.c \
           platform/nemu/mpe.c

CFLAGS    += -fdata-sections -ffunction-sections -g
CFLAGS    += -I$(AM_HOME)/am/src/platform/nemu/include
LDSCRIPTS += $(AM_HOME)/scripts/linker.ld
LDFLAGS   += --defsym=_pmem_start=0x80000000 --defsym=_entry_offset=0x0
LDFLAGS   += --gc-sections -e _start

NEMUFLAGS += -l $(shell dirname $(IMAGE).elf)/nemu-log.txt      
NEMUFLAGS += -e $(IMAGE).elf -i $(IMAGE).bin
# NEMUFLAGS += -b
 # 增加 -b 选择nemu平台的批处理模式

# python脚本处理insert-arg 
MAINARGS_MAX_LEN = 128
MAINARGS_PLACEHOLDER = the_insert-arg_rule_in_Makefile_will_insert_mainargs_here
CFLAGS += -DMAINARGS_MAX_LEN=$(MAINARGS_MAX_LEN)
CFLAGS += -DMAINARGS_PLACEHOLDER=$(MAINARGS_PLACEHOLDER)

insert-arg: image
	@python $(AM_HOME)/tools/insert-arg.py $(IMAGE).bin $(MAINARGS_MAX_LEN) $(MAINARGS_PLACEHOLDER) "$(mainargs)"
# 利用python脚本规定 修改 guest 二进制文件（image.bin），写入占位符处 → 运行时 guest 程序内的 mainargs 读取到这个值   该脚本在.bin生成之后，run之前处理完成


# ARGS="$(NEMUFLAGS)" :传给宿主模拟器 NEMU 的命令行选项，控制模拟器行为（启动镜像、日志、端口等）
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
