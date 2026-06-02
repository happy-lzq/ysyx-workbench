// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcore_top.h for the primary calling header

#include "Vcore_top__pch.h"
#include "Vcore_top___024root.h"

VL_ATTR_COLD void Vcore_top___024root___eval_static(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_static\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
}

VL_ATTR_COLD void Vcore_top___024root___eval_initial(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_initial\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Vcore_top___024root___eval_final(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_final\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcore_top___024root___dump_triggers__stl(Vcore_top___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vcore_top___024root___eval_phase__stl(Vcore_top___024root* vlSelf);

VL_ATTR_COLD void Vcore_top___024root___eval_settle(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_settle\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ __VstlIterCount;
    CData/*0:0*/ __VstlContinue;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    __VstlContinue = 1U;
    while (__VstlContinue) {
        if (VL_UNLIKELY(((0x64U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vcore_top___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("vsrc/core_top.v", 1, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vcore_top___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelfRef.__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcore_top___024root___dump_triggers__stl(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___dump_triggers__stl\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VstlTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vcore_top___024root___stl_sequent__TOP__0(Vcore_top___024root* vlSelf);
VL_ATTR_COLD void Vcore_top___024root____Vm_traceActivitySetAll(Vcore_top___024root* vlSelf);

VL_ATTR_COLD void Vcore_top___024root___eval_stl(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_stl\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        Vcore_top___024root___stl_sequent__TOP__0(vlSelf);
        Vcore_top___024root____Vm_traceActivitySetAll(vlSelf);
    }
}

VL_ATTR_COLD void Vcore_top___024root___eval_triggers__stl(Vcore_top___024root* vlSelf);

VL_ATTR_COLD bool Vcore_top___024root___eval_phase__stl(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_phase__stl\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vcore_top___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelfRef.__VstlTriggered.any();
    if (__VstlExecute) {
        Vcore_top___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcore_top___024root___dump_triggers__act(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___dump_triggers__act\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VactTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcore_top___024root___dump_triggers__nba(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___dump_triggers__nba\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VnbaTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vcore_top___024root____Vm_traceActivitySetAll(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root____Vm_traceActivitySetAll\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vm_traceActivity[0U] = 1U;
    vlSelfRef.__Vm_traceActivity[1U] = 1U;
}

VL_ATTR_COLD void Vcore_top___024root___ctor_var_reset(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___ctor_var_reset\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->name());
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18209466448985614591ull);
    vlSelf->instr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 682939282188807998ull);
    vlSelf->halt = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11049222807502041558ull);
    vlSelf->halt_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14212054205702290404ull);
    vlSelf->halt_ret = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9337472404044710651ull);
    vlSelf->core_top__DOT__rs1_addr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 12294012412567293440ull);
    vlSelf->core_top__DOT__rs2_addr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 2711145848174800480ull);
    vlSelf->core_top__DOT__rd_addr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 9222939784570157495ull);
    vlSelf->core_top__DOT__mem_read = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3267123640257130700ull);
    vlSelf->core_top__DOT__mem_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6654782002655877227ull);
    vlSelf->core_top__DOT__reg_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13173339675713999511ull);
    vlSelf->core_top__DOT__rs1_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6408348091913204400ull);
    vlSelf->core_top__DOT__rs2_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1190243116441480001ull);
    vlSelf->core_top__DOT__pc_sel = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 14593574024321751254ull);
    vlSelf->core_top__DOT__jump_jalr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3056972484890003365ull);
    vlSelf->core_top__DOT__imm_jal = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10432847048213708804ull);
    vlSelf->core_top__DOT__pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4262964085017532224ull);
    vlSelf->core_top__DOT__alu_op = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 13506384018872244617ull);
    vlSelf->core_top__DOT__is_ebreak = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17646060618805297716ull);
    vlSelf->core_top__DOT__alu_src_a = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7235664305421915035ull);
    vlSelf->core_top__DOT__alu_src_b = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 16418293016274603308ull);
    vlSelf->core_top__DOT__br_type = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 1094326332245414675ull);
    vlSelf->core_top__DOT__lsu_type = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 10055842887531614569ull);
    vlSelf->core_top__DOT__reg_wdata_src = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 12464435452399026180ull);
    vlSelf->core_top__DOT__mem_rdata_raw = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16007864046223268843ull);
    vlSelf->core_top__DOT__csr_op = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 8898456078735120183ull);
    vlSelf->core_top__DOT__csr_rs1 = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 18407868830701705724ull);
    vlSelf->core_top__DOT__csr_addr = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 9744846254435848729ull);
    vlSelf->core_top__DOT__csr_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2895962255076696243ull);
    vlSelf->core_top__DOT__csr_imm = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7989393693410596397ull);
    vlSelf->core_top__DOT__csr_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3197394343526444606ull);
    vlSelf->core_top__DOT__csr_read = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13236858726449675667ull);
    vlSelf->core_top__DOT__mret = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15124069862541340812ull);
    vlSelf->core_top__DOT__trap_enter = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15524177137300609526ull);
    vlSelf->core_top__DOT__csr_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8226085165328711824ull);
    vlSelf->core_top__DOT__trap_code = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15203590872551550305ull);
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->core_top__DOT__u_regfile__DOT__rf[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1373196643498699064ull);
    }
    vlSelf->core_top__DOT__u_csr__DOT__csr_mstatus = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7351712080400600870ull);
    vlSelf->core_top__DOT__u_csr__DOT__csr_mtvec = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2128392280724534277ull);
    vlSelf->core_top__DOT__u_csr__DOT__csr_mepc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4655941538022509436ull);
    vlSelf->core_top__DOT__u_csr__DOT__csr_mcause = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1398072415419079427ull);
    vlSelf->core_top__DOT__u_csr__DOT__csr_mip = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4615660953795351235ull);
    vlSelf->core_top__DOT__u_csr__DOT__csr_mie = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2293174247179605152ull);
    vlSelf->core_top__DOT__u_csr__DOT__csr_mtval = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17117547537694025625ull);
    vlSelf->core_top__DOT__u_csr__DOT__csr_mscratch = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17179779027546281259ull);
    vlSelf->core_top__DOT__u_csr__DOT__mcycle_64 = VL_SCOPED_RAND_RESET_Q(64, __VscopeHash, 5407484547729697954ull);
    vlSelf->core_top__DOT__u_if_stage__DOT__pc_next = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6969335522121334582ull);
    for (int __Vi0 = 0; __Vi0 < 33554432; ++__Vi0) {
        vlSelf->core_top__DOT__u_if_stage__DOT__imem[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11290784894032362506ull);
    }
    vlSelf->core_top__DOT__u_id_stage__DOT__opcode = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 13432465368368238113ull);
    vlSelf->core_top__DOT__u_id_stage__DOT__funct3 = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 16168852615844717633ull);
    vlSelf->core_top__DOT__u_id_stage__DOT__funct7 = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 17477892406395432723ull);
    vlSelf->core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3328527732238494480ull);
    vlSelf->core_top__DOT__u_ex_stage__DOT__src1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5256965371199316806ull);
    vlSelf->core_top__DOT__u_ex_stage__DOT__src2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6802426516301857958ull);
    vlSelf->core_top__DOT__u_ex_stage__DOT__csr_src = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4761910183485316740ull);
    for (int __Vi0 = 0; __Vi0 < 33554432; ++__Vi0) {
        vlSelf->core_top__DOT__u_mem_stage__DOT__dmem[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17667369412193926368ull);
    }
    vlSelf->core_top__DOT__u_mem_stage__DOT__mem_wmask = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 16035830778309949707ull);
    vlSelf->core_top__DOT__u_mem_stage__DOT__mem_rdata_raw = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10475187206980374080ull);
    vlSelf->core_top__DOT__u_mem_stage__DOT__mem_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13701141684010482380ull);
    vlSelf->core_top__DOT__u_mem_stage__DOT__dev_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8696910862573106347ull);
    vlSelf->core_top__DOT__u_mem_stage__DOT__in_pmem = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8068672556005015128ull);
    vlSelf->core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 13132455624507066660ull);
    vlSelf->core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 1308833202003551685ull);
    vlSelf->__VdfgRegularize_h3c5bdd75_0_0 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17939933138940668268ull);
    vlSelf->__Vfunc_dpi_mmio_read__0__Vfuncout = 0;
    vlSelf->__VdfgRegularize_h495687df_0_0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16520908832604956152ull);
    vlSelf->__VdfgRegularize_h495687df_0_1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3539160165987519352ull);
    vlSelf->__VdfgRegularize_h495687df_0_2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4270883785288342645ull);
    vlSelf->__VdfgRegularize_h495687df_0_3 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11091221206914412449ull);
    vlSelf->__VdfgRegularize_h495687df_0_4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15745456915760200859ull);
    vlSelf->__VdfgRegularize_h495687df_0_5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4119050263676651847ull);
    vlSelf->__VdfgRegularize_h495687df_0_6 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7109853991297637045ull);
    vlSelf->__VdfgRegularize_h495687df_0_7 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13274218766982498660ull);
    vlSelf->__VdfgRegularize_h495687df_0_8 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14821175674958249185ull);
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9526919608049418986ull);
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        vlSelf->__Vm_traceActivity[__Vi0] = 0;
    }
}
