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
    vlSelfRef.top__DOT____Vcellinp__u_com_fsm__rst_n 
        = ((IData)(vlSelfRef.rst_n) & (IData)(vlSelfRef.top__DOT__system_enable));
    vlSelfRef.led = (((IData)(vlSelfRef.rst_n) << 0xfU) 
                     | (((0U < vlSelfRef.top__DOT__refund_timer) 
                         << 1U) | ((4U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state)) 
                                   | (0U < vlSelfRef.top__DOT__cola_timer))));
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
void Vtop___024root___nba_sequent__TOP__1(Vtop___024root* vlSelf);
void Vtop___024root___nba_sequent__TOP__2(Vtop___024root* vlSelf);
void Vtop___024root___nba_sequent__TOP__3(Vtop___024root* vlSelf);
void Vtop___024root___nba_comb__TOP__0(Vtop___024root* vlSelf);
void Vtop___024root___nba_sequent__TOP__4(Vtop___024root* vlSelf);

void Vtop___024root___eval_nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtop___024root___nba_sequent__TOP__0(vlSelf);
    }
    if ((3ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtop___024root___nba_sequent__TOP__1(vlSelf);
        vlSelfRef.__Vm_traceActivity[1U] = 1U;
    }
    if ((5ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtop___024root___nba_sequent__TOP__2(vlSelf);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    if ((3ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtop___024root___nba_sequent__TOP__3(vlSelf);
        vlSelfRef.__Vm_traceActivity[3U] = 1U;
    }
    if ((7ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtop___024root___nba_comb__TOP__0(vlSelf);
        vlSelfRef.__Vm_traceActivity[4U] = 1U;
    }
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtop___024root___nba_sequent__TOP__4(vlSelf);
    }
}

VL_INLINE_OPT void Vtop___024root___nba_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vdly__top__DOT__u_ps2__DOT__ps2_clk_sync 
        = vlSelfRef.top__DOT__u_ps2__DOT__ps2_clk_sync;
    vlSelfRef.__Vdly__top__DOT__u_ps2__DOT__ps2_clk_sync 
        = ((6U & ((IData)(vlSelfRef.top__DOT__u_ps2__DOT__ps2_clk_sync) 
                  << 1U)) | (IData)(vlSelfRef.ps2_clk));
}

VL_INLINE_OPT void Vtop___024root___nba_sequent__TOP__1(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__1\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*23:0*/ __Vdly__top__DOT__cola_timer;
    __Vdly__top__DOT__cola_timer = 0;
    IData/*23:0*/ __Vdly__top__DOT__refund_timer;
    __Vdly__top__DOT__refund_timer = 0;
    // Body
    vlSelfRef.__Vdly__top__DOT__u_ps2__DOT__count = vlSelfRef.top__DOT__u_ps2__DOT__count;
    __Vdly__top__DOT__cola_timer = vlSelfRef.top__DOT__cola_timer;
    vlSelfRef.__Vdly__top__DOT__f0_seen = vlSelfRef.top__DOT__f0_seen;
    __Vdly__top__DOT__refund_timer = vlSelfRef.top__DOT__refund_timer;
    if (vlSelfRef.rst_n) {
        if (vlSelfRef.top__DOT__po_cola) {
            __Vdly__top__DOT__cola_timer = 0x989680U;
        } else if ((0U < vlSelfRef.top__DOT__cola_timer)) {
            __Vdly__top__DOT__cola_timer = (0xffffffU 
                                            & (vlSelfRef.top__DOT__cola_timer 
                                               - (IData)(1U)));
        }
        if (vlSelfRef.top__DOT__po_money) {
            __Vdly__top__DOT__refund_timer = 0x989680U;
        } else if ((0U < vlSelfRef.top__DOT__refund_timer)) {
            __Vdly__top__DOT__refund_timer = (0xffffffU 
                                              & (vlSelfRef.top__DOT__refund_timer 
                                                 - (IData)(1U)));
        }
        vlSelfRef.top__DOT__one_pulse = 0U;
        vlSelfRef.top__DOT__half_pulse = 0U;
        if (((IData)(vlSelfRef.top__DOT__system_enable) 
             & (IData)(vlSelfRef.top__DOT__key_release_pulse))) {
            if ((0x3aU == (IData)(vlSelfRef.top__DOT__released_key))) {
                vlSelfRef.top__DOT__one_pulse = 1U;
            }
            if ((0x3aU != (IData)(vlSelfRef.top__DOT__released_key))) {
                if ((0x31U == (IData)(vlSelfRef.top__DOT__released_key))) {
                    vlSelfRef.top__DOT__half_pulse = 1U;
                }
            }
        }
    } else {
        __Vdly__top__DOT__cola_timer = 0U;
        __Vdly__top__DOT__refund_timer = 0U;
        vlSelfRef.top__DOT__one_pulse = 0U;
        vlSelfRef.top__DOT__half_pulse = 0U;
    }
    vlSelfRef.top__DOT__cola_timer = __Vdly__top__DOT__cola_timer;
    vlSelfRef.top__DOT__refund_timer = __Vdly__top__DOT__refund_timer;
}

extern const VlUnpacked<CData/*0:0*/, 256> Vtop__ConstPool__TABLE_hf72e5794_0;
extern const VlUnpacked<CData/*2:0*/, 256> Vtop__ConstPool__TABLE_he935353d_0;
extern const VlUnpacked<CData/*0:0*/, 64> Vtop__ConstPool__TABLE_he7e6f38e_0;

VL_INLINE_OPT void Vtop___024root___nba_sequent__TOP__2(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__2\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*5:0*/ __Vtableidx3;
    __Vtableidx3 = 0;
    CData/*7:0*/ __Vtableidx4;
    __Vtableidx4 = 0;
    CData/*2:0*/ __Vdly__top__DOT__u_com_fsm__DOT__state;
    __Vdly__top__DOT__u_com_fsm__DOT__state = 0;
    IData/*31:0*/ __Vdly__top__DOT__u_com_fsm__DOT__wait_timer;
    __Vdly__top__DOT__u_com_fsm__DOT__wait_timer = 0;
    // Body
    __Vdly__top__DOT__u_com_fsm__DOT__wait_timer = vlSelfRef.top__DOT__u_com_fsm__DOT__wait_timer;
    __Vdly__top__DOT__u_com_fsm__DOT__state = vlSelfRef.top__DOT__u_com_fsm__DOT__state;
    if (vlSelfRef.top__DOT____Vcellinp__u_com_fsm__rst_n) {
        if ((((IData)(vlSelfRef.top__DOT__pi_quit) 
              & (~ (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__quit_clk))) 
             & (0U != (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state)))) {
            __Vdly__top__DOT__u_com_fsm__DOT__state = 0U;
            __Vdly__top__DOT__u_com_fsm__DOT__wait_timer = 0U;
        } else {
            __Vdly__top__DOT__u_com_fsm__DOT__wait_timer 
                = ((4U != (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state))
                    ? 0U : ((IData)(1U) + vlSelfRef.top__DOT__u_com_fsm__DOT__wait_timer));
            __Vdly__top__DOT__u_com_fsm__DOT__state 
                = ((4U & (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state))
                    ? ((2U & (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state))
                        ? 0U : ((1U & (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state))
                                 ? 0U : ((1U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money))
                                          ? 0U : ((2U 
                                                   == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money))
                                                   ? 0U
                                                   : 
                                                  ((0x1312d00U 
                                                    <= vlSelfRef.top__DOT__u_com_fsm__DOT__wait_timer)
                                                    ? 0U
                                                    : 4U)))))
                    : ((2U & (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state))
                        ? ((1U & (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state))
                            ? ((1U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money))
                                ? 4U : ((2U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money))
                                         ? 0U : 3U))
                            : ((1U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money))
                                ? 3U : ((2U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money))
                                         ? 4U : 2U)))
                        : ((1U & (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state))
                            ? ((1U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money))
                                ? 2U : ((2U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money))
                                         ? 3U : 1U))
                            : ((1U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money))
                                ? 1U : ((2U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money))
                                         ? 2U : 0U)))));
        }
    } else {
        __Vdly__top__DOT__u_com_fsm__DOT__state = 0U;
        __Vdly__top__DOT__u_com_fsm__DOT__wait_timer = 0U;
    }
    __Vtableidx4 = (((IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money) 
                     << 6U) | (((IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state) 
                                << 3U) | (((IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__quit_clk) 
                                           << 2U) | 
                                          (((IData)(vlSelfRef.top__DOT__pi_quit) 
                                            << 1U) 
                                           | (IData)(vlSelfRef.top__DOT____Vcellinp__u_com_fsm__rst_n)))));
    if (Vtop__ConstPool__TABLE_hf72e5794_0[__Vtableidx4]) {
        vlSelfRef.top__DOT__quit_out = Vtop__ConstPool__TABLE_he935353d_0
            [__Vtableidx4];
    }
    __Vtableidx3 = (((IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money) 
                     << 4U) | (((IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state) 
                                << 1U) | (IData)(vlSelfRef.top__DOT____Vcellinp__u_com_fsm__rst_n)));
    vlSelfRef.top__DOT__po_cola = Vtop__ConstPool__TABLE_he7e6f38e_0
        [__Vtableidx3];
    vlSelfRef.top__DOT__po_money = ((IData)(vlSelfRef.top__DOT____Vcellinp__u_com_fsm__rst_n) 
                                    && ((((IData)(vlSelfRef.top__DOT__pi_quit) 
                                          & (~ (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__quit_clk))) 
                                         & (0U != (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state))) 
                                        || ((3U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__state)) 
                                            & (2U == (IData)(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money)))));
    vlSelfRef.top__DOT__u_com_fsm__DOT__wait_timer 
        = __Vdly__top__DOT__u_com_fsm__DOT__wait_timer;
    vlSelfRef.top__DOT__u_com_fsm__DOT__state = __Vdly__top__DOT__u_com_fsm__DOT__state;
    vlSelfRef.top__DOT__u_com_fsm__DOT__quit_clk = 
        ((IData)(vlSelfRef.top__DOT____Vcellinp__u_com_fsm__rst_n) 
         && (IData)(vlSelfRef.top__DOT__pi_quit));
}

VL_INLINE_OPT void Vtop___024root___nba_sequent__TOP__3(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__3\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money = 
        (((IData)(vlSelfRef.top__DOT__one_pulse) << 1U) 
         | (IData)(vlSelfRef.top__DOT__half_pulse));
    if (vlSelfRef.rst_n) {
        vlSelfRef.top__DOT__pi_quit = 0U;
        if (((IData)(vlSelfRef.top__DOT__system_enable) 
             & (IData)(vlSelfRef.top__DOT__key_release_pulse))) {
            if ((0x3aU != (IData)(vlSelfRef.top__DOT__released_key))) {
                if ((0x31U != (IData)(vlSelfRef.top__DOT__released_key))) {
                    if ((0x29U == (IData)(vlSelfRef.top__DOT__released_key))) {
                        vlSelfRef.top__DOT__pi_quit = 1U;
                    }
                }
            }
        }
        if (vlSelfRef.top__DOT__key_release_pulse) {
            if ((0x5aU == (IData)(vlSelfRef.top__DOT__released_key))) {
                vlSelfRef.top__DOT__system_enable = 1U;
            } else if ((0x76U == (IData)(vlSelfRef.top__DOT__released_key))) {
                vlSelfRef.top__DOT__system_enable = 0U;
            }
        }
        vlSelfRef.top__DOT__key_release_pulse = 0U;
        if (vlSelfRef.top__DOT__valid) {
            if ((0xf0U == (IData)(vlSelfRef.top__DOT__scancode))) {
                vlSelfRef.__Vdly__top__DOT__f0_seen = 1U;
            } else if (vlSelfRef.top__DOT__f0_seen) {
                vlSelfRef.top__DOT__key_release_pulse = 1U;
                vlSelfRef.top__DOT__released_key = vlSelfRef.top__DOT__scancode;
                vlSelfRef.__Vdly__top__DOT__f0_seen = 0U;
            }
        }
        vlSelfRef.top__DOT__valid = 0U;
        if ((IData)((4U == (6U & (IData)(vlSelfRef.top__DOT__u_ps2__DOT__ps2_clk_sync))))) {
            if ((0xaU == (IData)(vlSelfRef.top__DOT__u_ps2__DOT__count))) {
                if ((((~ (IData)(vlSelfRef.top__DOT__u_ps2__DOT__buffer)) 
                      & (IData)(vlSelfRef.ps2_data)) 
                     & VL_REDXOR_32((0x1ffU & ((IData)(vlSelfRef.top__DOT__u_ps2__DOT__buffer) 
                                               >> 1U))))) {
                    vlSelfRef.top__DOT__scancode = 
                        (0xffU & ((IData)(vlSelfRef.top__DOT__u_ps2__DOT__buffer) 
                                  >> 1U));
                    vlSelfRef.top__DOT__valid = 1U;
                }
                vlSelfRef.__Vdly__top__DOT__u_ps2__DOT__count = 0U;
            } else {
                vlSelfRef.top__DOT__u_ps2__DOT____Vlvbound_h77389395__0 
                    = vlSelfRef.ps2_data;
                if ((9U >= (IData)(vlSelfRef.top__DOT__u_ps2__DOT__count))) {
                    vlSelfRef.top__DOT__u_ps2__DOT__buffer 
                        = (((~ ((IData)(1U) << (IData)(vlSelfRef.top__DOT__u_ps2__DOT__count))) 
                            & (IData)(vlSelfRef.top__DOT__u_ps2__DOT__buffer)) 
                           | (0x3ffU & ((IData)(vlSelfRef.top__DOT__u_ps2__DOT____Vlvbound_h77389395__0) 
                                        << (IData)(vlSelfRef.top__DOT__u_ps2__DOT__count))));
                }
                vlSelfRef.__Vdly__top__DOT__u_ps2__DOT__count 
                    = (0xfU & ((IData)(1U) + (IData)(vlSelfRef.top__DOT__u_ps2__DOT__count)));
            }
        }
        vlSelfRef.top__DOT____Vcellinp__u_com_fsm__rst_n 
            = vlSelfRef.top__DOT__system_enable;
    } else {
        vlSelfRef.top__DOT__pi_quit = 0U;
        vlSelfRef.top__DOT__system_enable = 0U;
        vlSelfRef.__Vdly__top__DOT__f0_seen = 0U;
        vlSelfRef.top__DOT__key_release_pulse = 0U;
        vlSelfRef.top__DOT__released_key = 0U;
        vlSelfRef.__Vdly__top__DOT__u_ps2__DOT__count = 0U;
        vlSelfRef.top__DOT__valid = 0U;
        vlSelfRef.top__DOT__scancode = 0U;
        vlSelfRef.top__DOT____Vcellinp__u_com_fsm__rst_n = 0U;
    }
    vlSelfRef.top__DOT__f0_seen = vlSelfRef.__Vdly__top__DOT__f0_seen;
    vlSelfRef.top__DOT__u_ps2__DOT__count = vlSelfRef.__Vdly__top__DOT__u_ps2__DOT__count;
}

extern const VlUnpacked<CData/*3:0*/, 16> Vtop__ConstPool__TABLE_h9615d6d5_0;
extern const VlUnpacked<CData/*3:0*/, 16> Vtop__ConstPool__TABLE_h8973c53f_0;
extern const VlUnpacked<CData/*6:0*/, 16> Vtop__ConstPool__TABLE_h51079ee3_0;

VL_INLINE_OPT void Vtop___024root___nba_comb__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_comb__TOP__0\n"); );
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

VL_INLINE_OPT void Vtop___024root___nba_sequent__TOP__4(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__4\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.top__DOT__u_ps2__DOT__ps2_clk_sync = vlSelfRef.__Vdly__top__DOT__u_ps2__DOT__ps2_clk_sync;
}

void Vtop___024root___eval_triggers__act(Vtop___024root* vlSelf);

bool Vtop___024root___eval_phase__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    VlTriggerVec<3> __VpreTriggered;
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
    if (VL_UNLIKELY(((vlSelfRef.ps2_clk & 0xfeU)))) {
        Verilated::overWidthError("ps2_clk");}
    if (VL_UNLIKELY(((vlSelfRef.ps2_data & 0xfeU)))) {
        Verilated::overWidthError("ps2_data");}
}
#endif  // VL_DEBUG
