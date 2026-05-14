VSRCS=$(shell find vsrc/ -name "*.v")
CSRCS=$(shell find csrc/ -name "*.cpp")
DIFF_SO_PATH=nemu/build/riscv32-nemu-interpreter-so
TOP = core_top 

sim: $(VSRCS) $(CSRCS)
	verilator --cc --exe --build \
	--top-module $(TOP) $(VSRCS) $(CSRCS) \
	-CFLAGS "-I$(NPC_HOME)/csrc -I$(NPC_HOME)/include -I$(NPC_HOME/npc/obj_dir)"
	mv obj_dir/V$(TOP) build/sim
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!

run: sim
	@echo ".bin = [$(IMG)]"
	./build/sim .bin=$(IMG) --diff=$(DIFF_SO_PATH)
include ../Makefile
clean:
	rm -rf build obj_dir