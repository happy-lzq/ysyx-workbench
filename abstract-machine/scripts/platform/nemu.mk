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
NEMUFLAGS += -b -l $(shell dirname $(IMAGE).elf)/nemu-log.txt       # 增加 -b 选择nemu平台的批处理模式

MAINARGS_MAX_LEN = 64
MAINARGS_PLACEHOLDER = the_insert-arg_rule_in_Makefile_will_insert_mainargs_here
CFLAGS += -DMAINARGS_MAX_LEN=$(MAINARGS_MAX_LEN) -DMAINARGS_PLACEHOLDER=$(MAINARGS_PLACEHOLDER)

insert-arg: image
	@python $(AM_HOME)/tools/insert-arg.py $(IMAGE).bin $(MAINARGS_MAX_LEN) $(MAINARGS_PLACEHOLDER) "$(mainargs)"
# 本机python环境仅有Python3,无python环境，故此将python修改为python3
# 使用ubuntu专门管理python 替换pthon3的包  来处理替换：sudo apt install python-is-python3
image: image-dep
	@$(OBJDUMP) -d $(IMAGE).elf > $(IMAGE).txt
	@echo + OBJCOPY "->" $(IMAGE_REL).bin
	@$(OBJCOPY) -S --set-section-flags .bss=alloc,contents -O binary $(IMAGE).elf $(IMAGE).bin

run: insert-arg
	$(MAKE) -C $(NEMU_HOME) ISA=$(ISA) run ARGS="$(NEMUFLAGS) --elf=$(IMAGE).elf" IMG=$(IMAGE).bin
# 添加：--elf=$(IMAGE).elf 然后通过ARGS传给nemu启动命令行
# nemu 启动行真实命令：nemu -b -l /path/nemu-log.txt --elf=/path/string-riscv32-nemu.elf /path/string-riscv32-nemu.bin
gdb: insert-arg
	$(MAKE) -C $(NEMU_HOME) ISA=$(ISA) gdb ARGS="$(NEMUFLAGS) --elf=$(IMAGE).elf" IMG=$(IMAGE).bin

.PHONY: insert-arg
