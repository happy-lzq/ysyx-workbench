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
    // Body
    vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money = 
        (((IData)(vlSelfRef.one) << 1U) | (IData)(vlSelfRef.half));
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

void Vtop___024root___nba_sequent__TOP__0(Vtop___024root* vlSelf);

void Vtop___024root___eval_nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((3ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtop___024root___nba_sequent__TOP__0(vlSelf);
    }
}

extern const VlUnpacked<CData/*3:0*/, 64> Vtop__ConstPool__TABLE_hab426afe_0;
extern const VlUnpacked<CData/*2:0*/, 256> Vtop__ConstPool__TABLE_h2fecf251_0;

VL_INLINE_OPT void Vtop___024root___nba_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*5:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    CData/*7:0*/ __Vtableidx2;
    __Vtableidx2 = 0;
    CData/*3:0*/ __Vdly__top__DOT__u_fsm__DOT__state;
    __Vdly__top__DOT__u_fsm__DOT__state = 0;
    CData/*2:0*/ __Vdly__top__DOT__u_com_fsm__DOT__state;
    __Vdly__top__DOT__u_com_fsm__DOT__state = 0;
    // Body
    __Vdly__top__DOT__u_fsm__DOT__state = vlSelfRef.top__DOT__u_fsm__DOT__state;
    __Vdly__top__DOT__u_com_fsm__DOT__state = vlSelfRef.top__DOT__u_com_fsm__DOT__state;
    __Vtableidx1 = (((IData)(vlSelfRef.half) << 5U) 
                    | (((IData)(vlSelfRef.top__DOT__u_fsm__DOT__state) 
                        << 1U) | (IData)(vlSelfRef.rst_n)));
    __Vdly__top__DOT__u_fsm__DOT__state = Vtop__ConstPool__TABLE_hab426afe_0
        [__Vtableidx1];
    __Vtableidx2 = (((IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money) 
                     << 6U) | (((IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state) 
                                << 3U) | (((IData)(vlSelfRef.quit_clk) 
                                           << 2U) | 
                                          (((IData)(vlSelfRef.pi_quit) 
                                            << 1U) 
                                           | (IData)(vlSelfRef.rst_n)))));
    __Vdly__top__DOT__u_com_fsm__DOT__state = Vtop__ConstPool__TABLE_h2fecf251_0
        [__Vtableidx2];
    vlSelfRef.dout = ((IData)(vlSelfRef.rst_n) && (
                                                   (4U 
                                                    == (IData)(vlSelfRef.top__DOT__u_fsm__DOT__state)) 
                                                   | (8U 
                                                      == (IData)(vlSelfRef.top__DOT__u_fsm__DOT__state))));
    vlSelfRef.po_cola = ((IData)(vlSelfRef.rst_n) && 
                         ((((3U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state)) 
                            & (2U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money))) 
                           | ((4U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state)) 
                              & (2U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money)))) 
                          | ((4U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state)) 
                             & (1U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money)))));
    vlSelfRef.po_money = ((IData)(vlSelfRef.rst_n) 
                          && ((((IData)(vlSelfRef.pi_quit) 
                                & (~ (IData)(vlSelfRef.quit_clk))) 
                               & (0U != (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state))) 
                              || ((4U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state)) 
                                  & (2U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money)))));
    vlSelfRef.top__DOT__u_fsm__DOT__state = __Vdly__top__DOT__u_fsm__DOT__state;
    vlSelfRef.top__DOT__u_com_fsm__DOT__state = __Vdly__top__DOT__u_com_fsm__DOT__state;
    vlSelfRef.quit_clk = ((IData)(vlSelfRef.rst_n) 
                          && (IData)(vlSelfRef.pi_quit));
}

void Vtop___024root___eval_triggers__act(Vtop___024root* vlSelf);

bool Vtop___024root___eval_phase__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    VlTriggerVec<2> __VpreTriggered;
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
            VL_FATAL_MT("p20_fsm/vsrc/top.v", 1, "", "Input combinational region did not converge.");
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
            VL_FATAL_MT("p20_fsm/vsrc/top.v", 1, "", "NBA region did not converge.");
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
                VL_FATAL_MT("p20_fsm/vsrc/top.v", 1, "", "Active region did not converge.");
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
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY(((vlSelfRef.rst_n & 0xfeU)))) {
        Verilated::overWidthError("rst_n");}
    if (VL_UNLIKELY(((vlSelfRef.one & 0xfeU)))) {
        Verilated::overWidthError("one");}
    if (VL_UNLIKELY(((vlSelfRef.half & 0xfeU)))) {
        Verilated::overWidthError("half");}
    if (VL_UNLIKELY(((vlSelfRef.pi_quit & 0xfeU)))) {
        Verilated::overWidthError("pi_quit");}
}
#endif  // VL_DEBUG
