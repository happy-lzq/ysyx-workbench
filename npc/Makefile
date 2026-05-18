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

# 根据 CONFIG_WAVE 决定是否传递 --trace 给 Verilator
VERILATOR_FLAGS = --cc --exe --build
ifdef CONFIG_WAVE
VERILATOR_FLAGS += --trace
endif

sim: $(VSRCS) $(CSRCS)
	verilator $(VERILATOR_FLAGS) \
	--top-module $(TOP) $(VSRCS) $(CSRCS) \
	-CFLAGS "$(SIM_CFLAGS)" -LDFLAGS "$(LDFLAGS)"
	@mkdir -p build
	mv obj_dir/V$(TOP) build/sim
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!

run: sim
	./build/sim $(ARGS)

vcd: $(WAVE_FILE)
	gtkwave $(WAVE_FILE)

clean:
	rm -rf build obj_dir

include ../Makefile
include $(NPC_HOME)/scripts/config.mk