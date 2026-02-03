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
    vlSelfRef.__Vtrigprevexpr___TOP__rst__0 = vlSelfRef.rst;
    vlSelfRef.__Vtrigprevexpr___TOP__top__DOT____Vcellinp__u_lfsr__clk__0 
        = vlSelfRef.top__DOT____Vcellinp__u_lfsr__clk;
}

VL_ATTR_COLD void Vtop___024root___eval_initial__TOP(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_initial(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtop___024root___eval_initial__TOP(vlSelf);
}

VL_ATTR_COLD void Vtop___024root___eval_initial__TOP(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial__TOP\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.seg2 = 3U;
    vlSelfRef.seg3 = 3U;
    vlSelfRef.seg4 = 3U;
    vlSelfRef.seg5 = 3U;
    vlSelfRef.seg6 = 3U;
    vlSelfRef.seg7 = 3U;
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
            VL_FATAL_MT("p19_lfsr/vsrc/top.v", 1, "", "Settle region did not converge.");
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
    }
}

VL_ATTR_COLD void Vtop___024root___stl_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___stl_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*7:0*/ __Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__Vfuncout;
    __Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__Vfuncout = 0;
    CData/*3:0*/ __Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex;
    __Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex = 0;
    CData/*7:0*/ __Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__Vfuncout;
    __Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__Vfuncout = 0;
    CData/*3:0*/ __Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex;
    __Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex = 0;
    // Body
    vlSelfRef.top__DOT__u_lfsr__DOT__feedback = (1U 
                                                 & ((0U 
                                                     == (IData)(vlSelfRef.dout)) 
                                                    | (1U 
                                                       & VL_REDXOR_8(
                                                                     (0x1dU 
                                                                      & (IData)(vlSelfRef.dout))))));
    vlSelfRef.top__DOT____Vcellinp__u_lfsr__clk = (1U 
                                                   & (vlSelfRef.top__DOT__count 
                                                      >> 0x14U));
    __Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex 
        = (0xfU & (IData)(vlSelfRef.dout));
    __Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__Vfuncout 
        = ((8U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex))
            ? ((4U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex))
                ? ((2U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex))
                    ? ((1U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex))
                        ? 0x71U : 0x61U) : ((1U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex))
                                             ? 0x85U
                                             : 0x63U))
                : ((2U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex))
                    ? ((1U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex))
                        ? 0xc1U : 0x11U) : ((1U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex))
                                             ? 9U : 1U)))
            : ((4U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex))
                ? ((2U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex))
                    ? ((1U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex))
                        ? 0x1fU : 0x41U) : ((1U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex))
                                             ? 0x49U
                                             : 0x99U))
                : ((2U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex))
                    ? ((1U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex))
                        ? 0xdU : 0x25U) : ((1U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__hex))
                                            ? 0x9fU
                                            : 3U))));
    vlSelfRef.seg0 = __Vfunc_top__DOT__u_seg_show__DOT__hex_seg__0__Vfuncout;
    __Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex 
        = (0xfU & ((IData)(vlSelfRef.dout) >> 4U));
    __Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__Vfuncout 
        = ((8U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex))
            ? ((4U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex))
                ? ((2U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex))
                    ? ((1U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex))
                        ? 0x71U : 0x61U) : ((1U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex))
                                             ? 0x85U
                                             : 0x63U))
                : ((2U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex))
                    ? ((1U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex))
                        ? 0xc1U : 0x11U) : ((1U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex))
                                             ? 9U : 1U)))
            : ((4U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex))
                ? ((2U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex))
                    ? ((1U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex))
                        ? 0x1fU : 0x41U) : ((1U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex))
                                             ? 0x49U
                                             : 0x99U))
                : ((2U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex))
                    ? ((1U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex))
                        ? 0xdU : 0x25U) : ((1U & (IData)(__Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__hex))
                                            ? 0x9fU
                                            : 3U))));
    vlSelfRef.seg1 = __Vfunc_top__DOT__u_seg_show__DOT__hex_seg__1__Vfuncout;
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
        VL_DBG_MSGF("         'act' region trigger index 1 is active: @(posedge rst)\n");
    }
    if ((4ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 2 is active: @(posedge top.__Vcellinp__u_lfsr__clk)\n");
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
        VL_DBG_MSGF("         'nba' region trigger index 1 is active: @(posedge rst)\n");
    }
    if ((4ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 2 is active: @(posedge top.__Vcellinp__u_lfsr__clk)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtop___024root___ctor_var_reset(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ctor_var_reset\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->name());
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18209466448985614591ull);
    vlSelf->stop = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16026904639853099817ull);
    vlSelf->dout = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 11474705599699299244ull);
    vlSelf->seg0 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8493502720866162066ull);
    vlSelf->seg1 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 12479572413206868450ull);
    vlSelf->seg2 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8290370235953472720ull);
    vlSelf->seg3 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 10826635380300885453ull);
    vlSelf->seg4 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 11022115687530795145ull);
    vlSelf->seg5 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 5844395792903037563ull);
    vlSelf->seg6 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 18091400542745428406ull);
    vlSelf->seg7 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8324453066673818596ull);
    vlSelf->top__DOT__count = VL_SCOPED_RAND_RESET_I(25, __VscopeHash, 893189210593458384ull);
    vlSelf->top__DOT____Vcellinp__u_lfsr__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12235568134084830310ull);
    vlSelf->top__DOT__u_lfsr__DOT__feedback = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9551990282746591556ull);
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9526919608049418986ull);
    vlSelf->__Vtrigprevexpr___TOP__rst__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3495601893105415319ull);
    vlSelf->__Vtrigprevexpr___TOP__top__DOT____Vcellinp__u_lfsr__clk__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11042583071182606110ull);
}
