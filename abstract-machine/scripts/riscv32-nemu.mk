# 脚本作用：设定具体的指令架构

include $(AM_HOME)/scripts/isa/riscv.mk            #ISA指令架构脚本
include $(AM_HOME)/scripts/platform/nemu.mk        #操作平台脚本
CFLAGS  += -DISA_H=\"riscv/riscv.h\"
COMMON_CFLAGS += -march=rv32im_zicsr -mabi=ilp32   # overwrite
LDFLAGS       += -melf32lriscv                     # overwrite

AM_SRCS += riscv/nemu/start.S \
           riscv/nemu/cte.c \
           riscv/nemu/trap.S \
           riscv/nemu/vme.c
