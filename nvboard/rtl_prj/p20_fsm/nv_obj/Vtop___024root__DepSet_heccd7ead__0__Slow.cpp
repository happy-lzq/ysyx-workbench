// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"
#include "Vtop___024root.h"

VL_ATTR_COLD void Vtop___024root___eval_static(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_static\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0 = vlSelfRef.rst_n;
    vlSelfRef.__Vtrigprevexpr___TOP__top__DOT____Vcellinp__u_com_fsm__rst_n__0 
        = vlSelfRef.top__DOT____Vcellinp__u_com_fsm__rst_n;
}

VL_ATTR_COLD void Vtop___024root___eval_initial__TOP(Vtop___024root* vlSelf);
VL_ATTR_COLD void Vtop___024root____Vm_traceActivitySetAll(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_initial(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtop___024root___eval_initial__TOP(vlSelf);
    Vtop___024root____Vm_traceActivitySetAll(vlSelf);
}

VL_ATTR_COLD void Vtop___024root___eval_initial__TOP(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial__TOP\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.top__DOT__system_enable = 0U;
}

VL_ATTR_COLD void Vtop___024root___eval_final(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_final\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__stl(Vtop___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vtop___024root___eval_phase__stl(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_settle(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_settle\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
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
            Vtop___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("p20_fsm/vsrc/top.v", 1, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vtop___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelfRef.__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
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

VL_ATTR_COLD void Vtop___024root___stl_sequent__TOP__0(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        Vtop___024root___stl_sequent__TOP__0(vlSelf);
        Vtop___024root____Vm_traceActivitySetAll(vlSelf);
    }
}

extern const VlUnpacked<CData/*3:0*/, 16> Vtop__ConstPool__TABLE_h9615d6d5_0;
extern const VlUnpacked<CData/*3:0*/, 16> Vtop__ConstPool__TABLE_h8973c53f_0;
extern const VlUnpacked<CData/*6:0*/, 16> Vtop__ConstPool__TABLE_h51079ee3_0;

VL_ATTR_COLD void Vtop___024root___stl_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___stl_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*3:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    CData/*3:0*/ __Vtableidx2;
    __Vtableidx2 = 0;
    CData/*3:0*/ __Vtableidx5;
    __Vtableidx5 = 0;
    CData/*3:0*/ __Vtableidx6;
    __Vtableidx6 = 0;
    CData/*3:0*/ __Vtableidx7;
    __Vtableidx7 = 0;
    CData/*3:0*/ __Vtableidx8;
    __Vtableidx8 = 0;
    // Body
    vlSelfRef.top__DOT____Vcellinp__u_com_fsm__rst_n 
        = ((IData)(vlSelfRef.rst_n) & (IData)(vlSelfRef.top__DOT__system_enable));
    vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money = 
        (((IData)(vlSelfRef.top__DOT__one_pulse) << 1U) 
         | (IData)(vlSelfRef.top__DOT__half_pulse));
    vlSelfRef.led = (((IData)(vlSelfRef.rst_n) << 0xfU) 
                     | (((0U < vlSelfRef.top__DOT__refund_timer) 
                         << 1U) | ((4U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state)) 
                                   | (0U < vlSelfRef.top__DOT__cola_timer))));
    __Vtableidx1 = (((IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state) 
                     << 1U) | (IData)(vlSelfRef.top__DOT__system_enable));
    vlSelfRef.top__DOT__digit_int = Vtop__ConstPool__TABLE_h9615d6d5_0
        [__Vtableidx1];
    vlSelfRef.top__DOT__digit_dec = Vtop__ConstPool__TABLE_h8973c53f_0
        [__Vtableidx1];
    __Vtableidx2 = (((IData)(vlSelfRef.top__DOT__quit_out) 
                     << 1U) | (IData)(vlSelfRef.top__DOT__system_enable));
    vlSelfRef.top__DOT__change_int = Vtop__ConstPool__TABLE_h9615d6d5_0
        [__Vtableidx2];
    vlSelfRef.top__DOT__change_dec = Vtop__ConstPool__TABLE_h8973c53f_0
        [__Vtableidx2];
    __Vtableidx6 = vlSelfRef.top__DOT__digit_dec;
    vlSelfRef.top__DOT__h_dec = Vtop__ConstPool__TABLE_h51079ee3_0
        [__Vtableidx6];
    __Vtableidx5 = vlSelfRef.top__DOT__digit_int;
    vlSelfRef.top__DOT__h_int = Vtop__ConstPool__TABLE_h51079ee3_0
        [__Vtableidx5];
    __Vtableidx8 = vlSelfRef.top__DOT__change_dec;
    vlSelfRef.top__DOT__h_ch_dec = Vtop__ConstPool__TABLE_h51079ee3_0
        [__Vtableidx8];
    __Vtableidx7 = vlSelfRef.top__DOT__change_int;
    vlSelfRef.top__DOT__h_ch_int = Vtop__ConstPool__TABLE_h51079ee3_0
        [__Vtableidx7];
    vlSelfRef.seg0 = (0xffU & (~ ((IData)(vlSelfRef.top__DOT__h_dec) 
                                  << 1U)));
    vlSelfRef.seg1 = (0xffU & (~ (((IData)(vlSelfRef.top__DOT__h_int) 
                                   << 1U) | (IData)(vlSelfRef.top__DOT__system_enable))));
    vlSelfRef.seg2 = (0xffU & (~ ((IData)(vlSelfRef.top__DOT__h_ch_dec) 
                                  << 1U)));
    vlSelfRef.seg3 = (0xffU & (~ (((IData)(vlSelfRef.top__DOT__h_ch_int) 
                                   << 1U) | (IData)(vlSelfRef.top__DOT__system_enable))));
}

VL_ATTR_COLD void Vtop___024root___eval_triggers__stl(Vtop___024root* vlSelf);

VL_ATTR_COLD bool Vtop___024root___eval_phase__stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vtop___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelfRef.__VstlTriggered.any();
    if (__VstlExecute) {
        Vtop___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__ico(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__ico\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VicoTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VicoTriggered.word(0U))) {
        VL_DBG_MSGF("         'ico' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VactTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge clk)\n");
    }
    if ((2ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 1 is active: @(negedge rst_n)\n");
    }
    if ((4ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 2 is active: @(negedge top.__Vcellinp__u_com_fsm__rst_n)\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VnbaTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge clk)\n");
    }
    if ((2ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 1 is active: @(negedge rst_n)\n");
    }
    if ((4ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 2 is active: @(negedge top.__Vcellinp__u_com_fsm__rst_n)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtop___024root____Vm_traceActivitySetAll(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root____Vm_traceActivitySetAll\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vm_traceActivity[0U] = 1U;
    vlSelfRef.__Vm_traceActivity[1U] = 1U;
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    vlSelfRef.__Vm_traceActivity[3U] = 1U;
    vlSelfRef.__Vm_traceActivity[4U] = 1U;
}

VL_ATTR_COLD void Vtop___024root___ctor_var_reset(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ctor_var_reset\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->name());
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1638864771569018232ull);
    vlSelf->ps2_clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8416932302485436191ull);
    vlSelf->ps2_data = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12987539744063830537ull);
    vlSelf->led = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 14009161575225144129ull);
    vlSelf->seg0 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8493502720866162066ull);
    vlSelf->seg1 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 12479572413206868450ull);
    vlSelf->seg2 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8290370235953472720ull);
    vlSelf->seg3 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 10826635380300885453ull);
    vlSelf->top__DOT__scancode = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 4002380781412692995ull);
    vlSelf->top__DOT__valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18441342033171973176ull);
    vlSelf->top__DOT__released_key = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 10357183904125935840ull);
    vlSelf->top__DOT__key_release_pulse = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5976501926597899502ull);
    vlSelf->top__DOT__f0_seen = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3810035020339639948ull);
    vlSelf->top__DOT__system_enable = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2707611891847676255ull);
    vlSelf->top__DOT__one_pulse = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1684772801243456943ull);
    vlSelf->top__DOT__half_pulse = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17562898463417447006ull);
    vlSelf->top__DOT__pi_quit = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1911955363947351703ull);
    vlSelf->top__DOT__po_money = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10314384016395075867ull);
    vlSelf->top__DOT__po_cola = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12522940133228516935ull);
    vlSelf->top__DOT__quit_out = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 13562891281392294428ull);
    vlSelf->top__DOT____Vcellinp__u_com_fsm__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6941961116740406935ull);
    vlSelf->top__DOT__cola_timer = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 3290222744773053087ull);
    vlSelf->top__DOT__refund_timer = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 3574333180054094101ull);
    vlSelf->top__DOT__digit_int = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 6605125269389282927ull);
    vlSelf->top__DOT__digit_dec = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 13378556633337894279ull);
    vlSelf->top__DOT__h_int = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 12848781446347604915ull);
    vlSelf->top__DOT__h_dec = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 12353576080897294890ull);
    vlSelf->top__DOT__change_int = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14592548965573894531ull);
    vlSelf->top__DOT__change_dec = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 10602911315982844178ull);
    vlSelf->top__DOT__h_ch_int = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 10239494770666368500ull);
    vlSelf->top__DOT__h_ch_dec = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 6873747603938839686ull);
    vlSelf->top__DOT__u_ps2__DOT__buffer = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 5657464454403254777ull);
    vlSelf->top__DOT__u_ps2__DOT__count = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 1452473424882849845ull);
    vlSelf->top__DOT__u_ps2__DOT__ps2_clk_sync = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 7883581931086327020ull);
    vlSelf->top__DOT__u_ps2__DOT____Vlvbound_h77389395__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1691388471978018300ull);
    vlSelf->top__DOT__u_com_fsm__DOT__state = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 3020179309017894823ull);
    vlSelf->top__DOT__u_com_fsm__DOT__quit_clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1809100834140265158ull);
    vlSelf->top__DOT__u_com_fsm__DOT__pi_money = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 1551605395421640561ull);
    vlSelf->top__DOT__u_com_fsm__DOT__wait_timer = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16013184697115387222ull);
    vlSelf->__Vdly__top__DOT__f0_seen = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11417129982688703949ull);
    vlSelf->__Vdly__top__DOT__u_ps2__DOT__count = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 8562110833571562451ull);
    vlSelf->__Vdly__top__DOT__u_ps2__DOT__ps2_clk_sync = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 8574686717766817571ull);
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9526919608049418986ull);
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14803524876191471008ull);
    vlSelf->__Vtrigprevexpr___TOP__top__DOT____Vcellinp__u_com_fsm__rst_n__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1677547914065404787ull);
    for (int __Vi0 = 0; __Vi0 < 5; ++__Vi0) {
        vlSelf->__Vm_traceActivity[__Vi0] = 0;
    }
}
