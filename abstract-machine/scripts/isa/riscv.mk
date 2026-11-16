# 脚本的核心作用：指令集设定，设定RSICV编译参数

CROSS_COMPILE ?= riscv64-linux-gnu-

# The NPC bare-metal compiler does not search the separately installed Newlib
# headers. Share the path with all AM libraries and applications, including C++.
# Runtime functions still come from the existing AM/RT-Thread link inputs.
ifneq ($(findstring unknown-elf,$(CROSS_COMPILE)),)
NEWLIB_INCLUDE ?= /usr/include/newlib
CFLAGS += -isystem $(NEWLIB_INCLUDE)
endif
COMMON_CFLAGS := -fno-pic -march=rv64g -mcmodel=medany -mstrict-align
CFLAGS        += $(COMMON_CFLAGS) -static
ASFLAGS       += $(COMMON_CFLAGS) -O0
LDFLAGS       += -melf64lriscv

# overwrite ARCH_H defined in $(AM_HOME)/Makefile
ARCH_H := arch/riscv.h
