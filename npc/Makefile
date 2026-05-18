NPC_HOME = $(shell pwd)

# ==================== Kconfig 配置系统 ====================
include $(NPC_HOME)/scripts/config.mk
-include $(NPC_HOME)/include/config/auto.conf

VSRCS=$(shell find vsrc/ -name "*.v")
CSRCS=$(shell find csrc/ -name "*.cpp")
TOP=core_top
WAVE_FILE=build/wave.vcd
SIM_CFLAGS = -I$(NPC_HOME)/csrc \
			 -I$(NPC_HOME)/include \
			 -I$(NPC_HOME)/obj_dir \
			 -I$(NPC_HOME)/include/generated

LDFLAGS    = -ldl

# ==================== sim: 始终编译带 --trace（信号暴露必需） ====================
# --trace 不仅控制VCD，还让Verilator把所有内部信号暴露到头文件，
# 否则 npc.h 中的 npc_gpr/npc_pc/npc_imem/npc_dmem 访问器全部失效。
# VCD 是否生成由运行时 --wave 参数控制。
sim: $(VSRCS) $(CSRCS)
	verilator --cc --exe --build --trace \
	--top-module $(TOP) $(VSRCS) $(CSRCS) \
	-CFLAGS "$(SIM_CFLAGS)" -LDFLAGS "$(LDFLAGS)"
	@mkdir -p build
	mv obj_dir/V$(TOP) build/sim
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!

run: sim
	./build/sim $(ARGS)

vcd: sim
	./build/sim $(ARGS) --wave
	gtkwave $(WAVE_FILE)

clean:
	rm -rf build obj_dir

include ../Makefile