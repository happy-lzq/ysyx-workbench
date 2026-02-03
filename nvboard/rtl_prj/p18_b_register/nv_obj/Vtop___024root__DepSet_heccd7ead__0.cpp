// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"
#include "Vtop___024root.h"

void Vtop___024root___ico_sequent__TOP__0(Vtop___024root* vlSelf);

void Vtop___024root___eval_ico(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_ico\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VicoTriggered.word(0U))) {
        Vtop___024root___ico_sequent__TOP__0(vlSelf);
    }
}

VL_INLINE_OPT void Vtop___024root___ico_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ico_sequent__TOP__0\n"); );
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
    vlSelfRef.top__DOT__u_b_shifter__DOT__al = (0xffU 
                                                & ((IData)(vlSelfRef.din) 
                                                   << (IData)(vlSelfRef.shmat)));
    vlSelfRef.dout = (0xffU & ((IData)(vlSelfRef.a_l)
                                ? ((IData)(vlSelfRef.l_r)
                                    ? (IData)(vlSelfRef.top__DOT__u_b_shifter__DOT__al)
                                    : VL_SHIFTRS_III(8,8,3, (IData)(vlSelfRef.din), (IData)(vlSelfRef.shmat)))
                                : ((IData)(vlSelfRef.l_r)
                                    ? (IData)(vlSelfRef.top__DOT__u_b_shifter__DOT__al)
                                    : ((IData)(vlSelfRef.din) 
                                       >> (IData)(vlSelfRef.shmat)))));
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

void Vtop___024root___eval_triggers__ico(Vtop___024root* vlSelf);

bool Vtop___024root___eval_phase__ico(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__ico\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VicoExecute;
    // Body
    Vtop___024root___eval_triggers__ico(vlSelf);
    __VicoExecute = vlSelfRef.__VicoTriggered.any();
    if (__VicoExecute) {
        Vtop___024root___eval_ico(vlSelf);
    }
    return (__VicoExecute);
}

void Vtop___024root___eval_act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

void Vtop___024root___eval_nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

void Vtop___024root___eval_triggers__act(Vtop___024root* vlSelf);

bool Vtop___024root___eval_phase__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    VlTriggerVec<0> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vtop___024root___eval_triggers__act(vlSelf);
    __VactExecute = vlSelfRef.__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelfRef.__VactTriggered, vlSelfRef.__VnbaTriggered);
        vlSelfRef.__VnbaTriggered.thisOr(vlSelfRef.__VactTriggered);
        Vtop___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vtop___024root___eval_phase__nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelfRef.__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vtop___024root___eval_nba(vlSelf);
        vlSelfRef.__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__ico(Vtop___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__nba(Vtop___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(Vtop___024root* vlSelf);
#endif  // VL_DEBUG

void Vtop___024root___eval(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ __VicoIterCount;
    CData/*0:0*/ __VicoContinue;
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VicoIterCount = 0U;
    vlSelfRef.__VicoFirstIteration = 1U;
    __VicoContinue = 1U;
    while (__VicoContinue) {
        if (VL_UNLIKELY(((0x64U < __VicoIterCount)))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__ico(vlSelf);
#endif
            VL_FATAL_MT("p18_b_register/vsrc/top.v", 1, "", "Input combinational region did not converge.");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
        __VicoContinue = 0U;
        if (Vtop___024root___eval_phase__ico(vlSelf)) {
            __VicoContinue = 1U;
        }
        vlSelfRef.__VicoFirstIteration = 0U;
    }
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY(((0x64U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("p18_b_register/vsrc/top.v", 1, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelfRef.__VactIterCount = 0U;
        vlSelfRef.__VactContinue = 1U;
        while (vlSelfRef.__VactContinue) {
            if (VL_UNLIKELY(((0x64U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtop___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("p18_b_register/vsrc/top.v", 1, "", "Active region did not converge.");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactContinue = 0U;
            if (Vtop___024root___eval_phase__act(vlSelf)) {
                vlSelfRef.__VactContinue = 1U;
            }
        }
        if (Vtop___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vtop___024root___eval_debug_assertions(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_debug_assertions\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.shmat & 0xf8U)))) {
        Verilated::overWidthError("shmat");}
    if (VL_UNLIKELY(((vlSelfRef.l_r & 0xfeU)))) {
        Verilated::overWidthError("l_r");}
    if (VL_UNLIKELY(((vlSelfRef.a_l & 0xfeU)))) {
        Verilated::overWidthError("a_l");}
}
#endif  // VL_DEBUG
