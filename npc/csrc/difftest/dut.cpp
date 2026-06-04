#include <difftest.h>
#include <dlfcn.h>
#include <difftest.h>
#include <memory.h>

extern VerilatedVcdC* tfp;
const char* img_file = NULL;
const char* diff_so_file = NULL;
DEBUG_FILE_PATH dfp;
FILE *diff_fp = NULL;

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
	ref_difftest_regcpy(&npc_s,DIFFTEST_TO_REF);
}

void npc_state_data(Vcore_top* top){
    npc_s.pc = npc_pc(top, 0, READ);
    for (int i = 0; i < 32; i++) {
        npc_s.gpr[i] = npc_gpr(top, i, 0, READ);
    }
}

void difftest_compare(){
    // PC 对比
    if (npc_s.pc != ref_s.pc) {
        npc_sim_state.state    = NPC_ABORT;
        npc_sim_state.halt_pc  = npc_s.pc;
        npc_sim_state.halt_ret = -1;
        return;     // ← 不再 panic，让主循环退出
    }
    // GPR 对比
    for (int i = 0; i < 32; i++) {
        if (npc_s.gpr[i] != ref_s.gpr[i]) {
            npc_sim_state.state    = NPC_ABORT;
            npc_sim_state.halt_pc  = npc_s.pc;
            npc_sim_state.halt_ret = i;  // ← 用 halt_ret 传失败寄存器号
            return;
        }
    }
}
void difftest_step(Vcore_top* top, int idx) {
    ref_difftest_exec(1);
    ref_difftest_regcpy(&ref_s, DIFFTEST_TO_DUT);
    npc_state_data(top);
    diff_log_write(&npc_s, &ref_s, idx);
    difftest_compare();
}

void init_diff_log(const char *path){
  build_named_log_file(dfp.diff,sizeof(dfp.diff),path,"build/diff-log-txt","diff-log-txt");
  diff_fp = fopen(dfp.diff,"w");
  Assert(diff_fp,"Can not open '%s'", dfp.diff);
  Log("diff log written to %s", dfp.diff);
}

void diff_log_write(NPC_state *npc, NPC_state *ref, int cycle) {
    if (!diff_fp) return;

    bool pc_ok = (npc->pc == ref->pc);

    fprintf(diff_fp, "--- cycle %d ---\n", cycle);
    fprintf(diff_fp, "  PC:  NPC=0x%08x  REF=0x%08x  %s\n",
            npc->pc, ref->pc,
            pc_ok ? "[✔]" : "[✘]");

    // 4列 × 8行 寄存器网格: NPC/REF 格式，匹配 [✔]，不匹配 [✘]
    for (int row = 0; row < 8; row++) {
        fprintf(diff_fp, "  x%02d-x%02d", row * 4, row * 4 + 3);
        for (int col = 0; col < 4; col++) {
            int i = row * 4 + col;
            bool ok = (npc->gpr[i] == ref->gpr[i]);
            fprintf(diff_fp, "  %s %08x  %08x",
                    ok ? "[✔]" : "[✘]",
                    npc->gpr[i], ref->gpr[i]);
        }
        fprintf(diff_fp, "\n");
    }
    fprintf(diff_fp, "\n");
    fflush(diff_fp);
}

void difftest_init(){
    if (diff_so_file) {
        init_diff_log(img_file);
        init_difftest(diff_so_file, npc_img_size);
    }
}
