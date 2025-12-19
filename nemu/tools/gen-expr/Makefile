CC := gcc
CFLAGS := -Wall -Wextra

gen_n ?= 10

SRC := gen-expr.c
OUTDIR := build
OUT := $(OUTDIR)/gen-expr

.PHONY: all clean run

all: $(OUT)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(OUT): $(SRC) | $(OUTDIR)
	$(CC) $(CFLAGS) -o $@ $(SRC)

run: $(OUT)
	@GEN_N=$(gen_n) GEN_OUT=$(abspath $(CURDIR)/../..)/input \
	$(OUT)
# 子进程环境：run 中设置临时变量GEN_N=$(gen_n),然后执行 ./$(out)
clean:
	rm -rf $(OUTDIR)
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

NAME = gen-expr
SRCS = gen-expr.c
include $(NEMU_HOME)/scripts/build.mk


