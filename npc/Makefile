NPC_HOME = $(shell pwd)

# ==================== Kconfig 配置系统 ====================
include $(NPC_HOME)/scripts/config.mk
-include $(NPC_HOME)/include/config/auto.conf

VSRCS=$(shell find vsrc/ -name "*.v")
CSRCS=$(shell find csrc/ -name "*.cpp")
TOP=core_top
WAVE_FILE=build/wave.vcd
SIM_CFLAGS = -g -O0 \
			 -I$(NPC_HOME)/csrc \
			 -I$(NPC_HOME)/include \
			 -I$(NPC_HOME)/obj_dir \
			 -I$(NPC_HOME)/include/generated \
			 -I$(NPC_HOME)/tools/capstone/repo/include

LDFLAGS = -ldl -lreadline -lSDL2
SIM_CFLAGS += $(shell pkg-config --cflags sdl2)

# ==================== run-vcd 执行逻辑 ====================

sim: $(VSRCS) $(CSRCS)
	verilator --cc --exe --build --trace \
	--top-module $(TOP) $(VSRCS) $(CSRCS) \
	-I$(NPC_HOME)/vsrc \
	-I$(NPC_HOME)/include \
	-CFLAGS "$(SIM_CFLAGS)" -LDFLAGS "$(LDFLAGS)"
	@mkdir -p build
	mv obj_dir/V$(TOP) build/sim
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!

run: sim
	./build/sim $(ARGS)
	
gdb: sim
	gdb --args ./build/sim $(ARGS)

vcd: sim
	@echo "Running simulation..."
	@./build/sim $(ARGS); STATUS=$$?; \
	if [ $$STATUS -eq 0 ]; then \
		echo "✅ Simulation PASSED"; \
	else \
		echo "❌ Simulation FAILED (exit code $$STATUS)"; \
	fi; \
	echo "Opening waveform..."; \
	gtkwave $(WAVE_FILE)

clean:
	rm -rf build obj_dir

.PHONY: sim run gdb vcd clean

include ../Makefile

# - 单横线 → 短选项，如 -i file.bin、-w
#-- 双横线 → 长选项，如 --bin file.bin、--wave
