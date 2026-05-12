VSRCS=$(shell find vsrc/ -name "*.v")
CSRCS=$(shell find csrc/ -name "*.cpp")
TOP = core_top 

sim: $(VSRCS) $(CSRCS)
	verilator --cc --exe --build \
	--top-module $(TOP) $(VSRCS) $(CSRCS) \
	-CFLAGS "-I$(NPC_HOME)/csrc"
	mv obj_dir/V$(TOP) build/sim
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!

run: sim
	@echo "IMG = [$(IMG)]"           # ← 加这行看 IMG 是否有值
	./build/sim --img=$(IMG)
include ../Makefile
	