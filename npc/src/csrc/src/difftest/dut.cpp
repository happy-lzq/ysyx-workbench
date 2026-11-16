#include <difftest.h>
#include <dlfcn.h>
#include <difftest.h>
#include <memory.h>
#include <cycle_event.h>


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
void (*ref_difftest_set_intr_pending)(uint64_t) = NULL;
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
	ref_difftest_set_intr_pending = (void(*)(uint64_t))dlsym(h,"difftest_set_intr_pending");
	ref_difftest_init       = (void(*)(int))dlsym(h,"difftest_init");
	assert(ref_difftest_memcpy);
	assert(ref_difftest_regcpy);
	assert(ref_difftest_exec);
	assert(ref_difftest_raise_intr);
	assert(ref_difftest_set_intr_pending);
	assert(ref_difftest_init);	
	// init ref 
	ref_difftest_init(1234);
	// Match the complete initial RAM state, including stack/heap beyond the
	// image. REF may randomize RAM while NPC initializes it to zero.
    Assert(img_size >= 0 && static_cast<uint64_t>(img_size) <= PMEM_SIZE,
           "Invalid Difftest image size: %ld", img_size);
	ref_difftest_memcpy(PMEM_BASE, npc_pmem, PMEM_SIZE, DIFFTEST_TO_REF);
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
            fprintf(stderr,
                    "Difftest GPR mismatch: PC=0x%08x x%d DUT=0x%08x REF=0x%08x; "
                    "sp DUT=0x%08x REF=0x%08x\n",
                    npc_s.pc, i, npc_s.gpr[i], ref_s.gpr[i],
                    npc_s.gpr[2], ref_s.gpr[2]);
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

void difftest_set_intr_pending(uint64_t cause) {
    if (ref_difftest_set_intr_pending != NULL) {
        ref_difftest_set_intr_pending(cause);
    }
}

void difftest_step(
    Vcore_top* top, 
    int idx, 
    uint32_t npc_exec_pc,
    bool wb_valid, 
    bool irq_taken, 
    uint32_t interrupt_cause,
    bool wb_is_mmio,
    uint32_t wb_inst) 
{
    // ① 获取 NEMU 执行前的 PC
    ref_difftest_regcpy(&ref_s, DIFFTEST_TO_DUT);
    uint32_t ref_exec_pc = ref_s.pc;
    // 本拍存在 WB 提交，才检查指令地址是否对齐。
    if (wb_valid && npc_exec_pc != ref_exec_pc) {
        fprintf(stderr,
                "Difftest commit PC mismatch: DUT=0x%08x REF=0x%08x\n",
                npc_exec_pc, ref_exec_pc);

        npc_sim_state.state    = NPC_ABORT;
        npc_sim_state.halt_pc  = npc_exec_pc;
        npc_sim_state.halt_ret = -1;  // 现有错误处理用 -1 表示 PC mismatch
        return;
    }
    // ② 同拍先提交 WB 指令，再在 RTL 接受中断的精确边界注入中断。
    if (wb_valid) {
        if (wb_is_mmio) {
            // ref_s 是函数开头取得的 REF 执行前状态。
            NPC_state next_ref = ref_s;

            uint32_t opcode = wb_inst & 0x7f;
            uint32_t rd = (wb_inst >> 7) & 0x1f;

            // MMIO load：接收 NPC 已提交到目标寄存器的结果。
            if (opcode == 0x03 && rd != 0) {
                next_ref.gpr[rd] = npc_gpr(top, rd, 0, READ);
            }

            // MMIO load/store 的下一条架构指令地址。
            next_ref.pc = npc_exec_pc + 4;

            ref_difftest_regcpy(&next_ref, DIFFTEST_TO_REF);
        } else {
            ref_difftest_exec(1);           // 普通指令执行
        }
    }

    // 同拍若接受中断，在处理完更老的 WB 指令后进入 trap。
    if (irq_taken) {
        ref_difftest_raise_intr(interrupt_cause);
    }

    // ③ 获取执行后的 GPR/CSR
    ref_difftest_regcpy(&ref_s, DIFFTEST_TO_DUT);
    npc_state_data(top);

    // ④ 普通指令沿用旧的执行 PC 对齐方式；中断后比较真实的 mtvec PC。
    if (!irq_taken) {
        npc_s.pc = npc_exec_pc;
        ref_s.pc = ref_exec_pc;
    }

    // ⑤ 每个事件都比较状态；文件日志由独立配置控制。
    difftest_compare();
#ifdef CONFIG_DIFF_LOG_ENABLE
    // When file logging is enabled, an abort bypasses the record limit.
    diff_log_write(&npc_s, &ref_s, idx);
#endif
}

#ifdef CONFIG_DIFF_LOG_ENABLE
void init_diff_log(const char *path){
  build_named_log_file(dfp.diff,sizeof(dfp.diff),path,"build/diff-log-txt","diff-log-txt");
  diff_fp = fopen(dfp.diff,"w");
  Assert(diff_fp,"Can not open '%s'", dfp.diff);
  Log("diff log written to %s", dfp.diff);
}

void diff_log_write(NPC_state *npc, NPC_state *ref, int cycle) {
    if (!diff_fp) return;
    if (npc_log_limit != 0 && static_cast<uint64_t>(cycle) >= npc_log_limit &&
        npc_sim_state.state != NPC_ABORT) return;

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
#endif

void difftest_init(){
    if (diff_so_file) {
#ifdef CONFIG_DIFF_LOG_ENABLE
        init_diff_log(img_file);
#endif

init_difftest(diff_so_file, npc_img_size);
    }
}
