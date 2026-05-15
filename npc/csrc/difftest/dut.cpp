#include <difftest.h>
#include <dlfcn.h>

void (*ref_difftest_memcpy)(paddr_t, void*, size_t, bool) = NULL;
void (*ref_difftest_regcpy)(void*, bool) = NULL;
void (*ref_difftest_exec)(uint64_t) = NULL;
void (*ref_difftest_raise_intr)(uint64_t) = NULL;
void (*ref_difftest_init)(int) = NULL;



void init_difftest(const char* so_path,long img_size){
	// get nemu-functs-API
	void *h = dlopen(so_path,RTLD_LAZY);
	assert(h);
	ref_difftest_memcpy     = (void(*)(paddr_t, void*, size_t, bool))dlsym(h,"difftest_memcpy");
	ref_difftest_regcpy     = (void(*)(void*,bool))dlsym(h,"difftest_regcpy");
	ref_difftest_exec       = (void(*)(uint64_t))dlsym(h,"difftest_exec");
	ref_difftest_raise_intr = (void(*)(uint64_t))dlsym(h,"difftest_raise_intr");
	ref_difftest_init       = (void(*)(int))dlsym(h,"difftest_init");
	assert(ref_difftest_memcpy);
	assert(ref_difftest_regcpy);
	assert(ref_difftest_exec);
	assert(ref_difftest_raise_intr);
	assert(ref_difftest_init);	
	// init ref 
	ref_difftest_init(1234);
	// read npc-imem to ref
	ref_difftest_memcpy(RESET_VECTOR, npc_pmem, img_size, DIFFTEST_TO_REF);
	// read npc-gpr+pc to ref
	for (int i = 0; i < 32; i++){
	 npc_s.gpr[i] = npc_gpr(top,i,0,READ);
	}
	npc_s.pc     = npc_pc(top,0,READ);
	ref_difftest_regcpy(npc_s.gpr,DIFFTEST_TO_REF);
}

void difftest_step(Vcore_top* top){
	ref_difftest_exec(1);
	ref_difftest_regcpy(&ref_s,DIFFTEST_TO_DUT);
	for (int i = 0; i < 32; i++)
	{
		printf("REF-GPR-VAL : x%d: %08x\n",i,ref_s.gpr[i]);
	}
	
	npc_s.pc  = npc_pc(top,0,READ); 
	if (ref_s.pc != npc_s.pc){
		printf("ERROR PC : NPC=%08x NEMU=%08x\n",npc_s.pc,ref_s.pc);
	}
	for (int i = 0; i < 32; i++){
		uint32_t cur_val = npc_gpr(top,i,0,READ);
		if (ref_s.gpr[i] != cur_val){
			printf("ERROR x%d: NPC=%08x NEMU=%08x\n", i,cur_val,ref_s.gpr[i]);
		}
	}
}