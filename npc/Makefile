VSRCS=$(shell find vsrc/ -name "*.v")
CSRCS=$(shell find csrc/ -name "*.cpp")
TOP=core_top

SIM_CFLAGS = -I$(NPC_HOME)/csrc -I$(NPC_HOME)/include -I$(NPC_HOME)/obj_dir
LDFLAGS    = -ldl
FILE_PATH += -bin
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
	./build/sim $(ARGS)
	gtkwave build/wave.vcd

clean:
	rm -rf build obj_dir

include ../Makefile