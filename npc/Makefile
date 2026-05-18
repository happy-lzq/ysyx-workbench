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

# ==================== sim: 无波形，快速 ====================
sim: $(VSRCS) $(CSRCS)
	verilator --cc --exe --build \
	--top-module $(TOP) $(VSRCS) $(CSRCS) \
	-CFLAGS "$(SIM_CFLAGS)" -LDFLAGS "$(LDFLAGS)"
	@mkdir -p build
	mv obj_dir/V$(TOP) build/sim
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!

# ==================== sim-trace: 带 --trace，生成 VCD ====================
sim-trace: $(VSRCS) $(CSRCS)
	verilator --cc --exe --build --trace \
	--top-module $(TOP) $(VSRCS) $(CSRCS) \
	-CFLAGS "$(SIM_CFLAGS)" -LDFLAGS "$(LDFLAGS)"
	@mkdir -p build
	mv obj_dir/V$(TOP) build/sim-trace
	$(call git_commit, "sim RTL with trace") # DO NOT REMOVE THIS LINE!!!

run: sim
	./build/sim $(ARGS)

vcd: sim-trace
	./build/sim-trace $(ARGS)
	gtkwave $(WAVE_FILE)

clean:
	rm -rf build obj_dir

include ../Makefile