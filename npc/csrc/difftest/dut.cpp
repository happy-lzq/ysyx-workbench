#include <difftest.h>
#include <dlfcn.h>
#include <difftest.h>
#include <memory.h>

extern VerilatedVcdC* tfp;
const char* img_file = NULL;
const char* diff_so_file = NULL;
DEBUG_FILE_PATH dfp;
FILE *diff_fp = NULL;
// bool difftest_gpr = false;
// bool difftest_csr = false;
// bool difftest_pc  = false;
void (*ref_difftest_memcpy)(paddr_t, void*, size_t, bool) = NULL;
void (*ref_difftest_regcpy)(void*, bool) = NULL;
void (*ref_difftest_exec)(uint64_t) = NULL;
void (*ref_difftest_raise_intr)(uint64_t) = NULL;
void (*ref_difftest_init)(int) = NULL;


void npc_state_data(Vcore_top* top){
    npc_s.pc = npc_pc(top, 0, READ);
    for (int i = 0; i < 32; i++) {
        npc_s.gpr[i] = npc_gpr(top, i, 0, READ);
    }
    // 新增：读取 CSR
    npc_s.csr[0] = npc_csr(top, CSR_MSTATUS,   0, READ);
    npc_s.csr[1] = npc_csr(top, CSR_MIP,       0, READ);
    npc_s.csr[2] = npc_csr(top, CSR_MIE,       0, READ);
    npc_s.csr[3] = npc_csr(top, CSR_MCAUSE,    0, READ);
    npc_s.csr[4] = npc_csr(top, CSR_MTVEC,     0, READ);
    npc_s.csr[5] = npc_csr(top, CSR_MTVAL,     0, READ);
    npc_s.csr[6] = npc_csr(top, CSR_MEPC,      0, READ);
    npc_s.csr[7] = npc_csr(top, CSR_MSCRATCH,  0, READ);
}

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
	// 同步 NPC 全部物理内存到 NEMU（而非仅 .bin 大小）
	// 原因：堆栈/BSS 区域超出 img_size，不同步会导致 NEMU 读 CONFIG_MEM_RANDOM 随机值
	ref_difftest_memcpy(RESET_VECTOR, npc_pmem, PMEM_SIZE, DIFFTEST_TO_REF);
	// read npc-gpr+pc to ref
    npc_state_data(top);
	ref_difftest_regcpy(&npc_s,DIFFTEST_TO_REF);
}

void difftest_compare(){
    // PC 对比
    if (npc_s.pc != ref_s.pc) {
        npc_sim_state.state    = NPC_ABORT;
        npc_sim_state.halt_pc  = npc_s.pc;
        npc_sim_state.halt_ret = -1;
        return;
    }
    // GPR 对比
    for (int i = 0; i < 32; i++) {
        if (npc_s.gpr[i] != ref_s.gpr[i]) {
            npc_sim_state.state    = NPC_ABORT;
            npc_sim_state.halt_pc  = npc_s.pc;
            npc_sim_state.halt_ret = i;
            return;
        }
    }

    // CSR 对比
    for (int i = 0; i < 8; i++) {
        if (npc_s.csr[i] != ref_s.csr[i]) {
            npc_sim_state.state    = NPC_ABORT;
            npc_sim_state.halt_pc  = npc_s.pc;
            npc_sim_state.halt_ret = 32 + i;  // CSR[i] → 32..39
            return;
        }
    }
}

void difftest_step(Vcore_top* top, int idx, uint32_t npc_exec_pc) {
    // ① 获取 NEMU 执行前的 PC
    ref_difftest_regcpy(&ref_s, DIFFTEST_TO_DUT);
    uint32_t ref_exec_pc = ref_s.pc;

    // ② NEMU 执行一条指令
    ref_difftest_exec(1);

    // ③ 获取执行后的 GPR/CSR
    ref_difftest_regcpy(&ref_s, DIFFTEST_TO_DUT);
    npc_state_data(top);

    // ④ 用执行 PC 覆盖 post-exec PC：后续比较和 log 都基于"刚执行指令的 PC"
    npc_s.pc = npc_exec_pc;
    ref_s.pc = ref_exec_pc;

    // ⑤ 写 log + 比较（PC、GPR、CSR 全部对齐到同一条指令）
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
    fprintf(diff_fp, "  Exec PC: NPC=0x%08x  REF=0x%08x  %s\n",
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

    // CSR 寄存器对比：2行 × 4列，与 GPR 网格对齐
    static const char *csr_names[] = {"mst","mip","mie","mca","mtv","mta","mep","msc"};
    fprintf(diff_fp, "  --- CSR ---\n");
    for (int row = 0; row < 2; row++) {
        fprintf(diff_fp, "  %s-%-3s", csr_names[row * 4], csr_names[row * 4 + 3]);
        for (int col = 0; col < 4; col++) {
            int i = row * 4 + col;
            bool ok = (npc->csr[i] == ref->csr[i]);
            fprintf(diff_fp, "  %s %08x  %08x",
                    ok ? "[✔]" : "[✘]",
                    npc->csr[i], ref->csr[i]);
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
