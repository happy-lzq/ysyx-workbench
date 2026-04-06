#***************************************************************************************
# Copyright (c) 2014-2024 Zihao Yu, Nanjing University
#
# NEMU is licensed under Mulan PSL v2.
# You can use this software according to the terms and conditions of the Mulan PSL v2.
# You may obtain a copy of Mulan PSL v2 at:
#          http://license.coscl.org.cn/MulanPSL2
#
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
# EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
# MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
#
# See the Mulan PSL v2 for more details.
#**************************************************************************************/

-include $(NEMU_HOME)/../Makefile
include $(NEMU_HOME)/scripts/build.mk

include $(NEMU_HOME)/tools/difftest.mk

compile_git:
	$(call git_commit, "compile NEMU")
$(BINARY):: compile_git

# Some convenient rules

override ARGS ?= --log=$(BUILD_DIR)/nemu-log.txt
override ARGS += $(ARGS_DIFF)

# Command to execute NEMU
# IMG ?=    可以省略当前行
NEMU_EXEC := $(BINARY) $(ARGS) $(IMG) # 传入$(IMG) 目的为了能够在nemu中运行nemu时传入对应的.bin文件进而运行。
# 确保二进制.bin存在，如果若启用 difftest，再确保参考 so 已经准备好。
run-env: $(BINARY) $(DIFF_REF_SO)

run: run-env
	$(call git_commit, "run NEMU")
	$(NEMU_EXEC)
# $(NEMU_EXEC) 展开变量，把它变成一条完整命令，然后交给 shell 执行
# /home/luo/ysyx/ysyx-workbench/nemu/build/riscv32-nemu-interpreter -b \
  -l /home/luo/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/nemu-log.txt \
  -e /home/luo/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/string-riscv32-nemu.elf \
  IMG=/home/luo/ysyx/ysyx-workbench/am-kernels/tests/cpu-tests/build/string-riscv32-nemu.bin \

gdb: run-env
	$(call git_commit, "gdb NEMU")
	gdb -s $(BINARY) --args $(NEMU_EXEC)

clean-tools = $(dir $(shell find ./tools -maxdepth 2 -mindepth 2 -name "Makefile"))
$(clean-tools):
	-@$(MAKE) -s -C $@ clean
clean-tools: $(clean-tools)
clean-all: clean distclean clean-tools

.PHONY: run gdb run-env clean-tools clean-all $(clean-tools)
