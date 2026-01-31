// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtop.h for the primary calling header

#ifndef VERILATED_VTOP___024ROOT_H_
#define VERILATED_VTOP___024ROOT_H_  // guard

#include "verilated.h"


class Vtop__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtop___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst_n,0,0);
    CData/*0:0*/ top__DOT____Vcellinp__u_com_fsm__rst_n;
    VL_IN8(ps2_clk,0,0);
    VL_IN8(ps2_data,0,0);
    VL_OUT8(seg0,7,0);
    VL_OUT8(seg1,7,0);
    VL_OUT8(seg2,7,0);
    VL_OUT8(seg3,7,0);
    VL_OUT8(seg4,7,0);
    VL_OUT8(seg5,7,0);
    VL_OUT8(seg6,7,0);
    VL_OUT8(seg7,7,0);
    CData/*7:0*/ top__DOT__scancode;
    CData/*6:0*/ top__DOT__press_count;
    CData/*0:0*/ top__DOT__valid;
    CData/*7:0*/ top__DOT__released_data;
    CData/*0:0*/ top__DOT__key_release_pulse;
    CData/*0:0*/ top__DOT__f0_seen;
    CData/*0:0*/ top__DOT__system_enable;
    CData/*0:0*/ top__DOT__one_pulse;
    CData/*0:0*/ top__DOT__half_pulse;
    CData/*0:0*/ top__DOT__pi_quit;
    CData/*0:0*/ top__DOT__po_money;
    CData/*0:0*/ top__DOT__po_cola;
    CData/*2:0*/ top__DOT__quit_out;
    CData/*3:0*/ top__DOT__digit_int;
    CData/*3:0*/ top__DOT__digit_dec;
    CData/*6:0*/ top__DOT__h_int;
    CData/*6:0*/ top__DOT__h_dec;
    CData/*3:0*/ top__DOT__change_int;
    CData/*3:0*/ top__DOT__change_dec;
    CData/*6:0*/ top__DOT__h_ch_int;
    CData/*6:0*/ top__DOT__h_ch_dec;
    CData/*6:0*/ top__DOT__count_tens;
    CData/*6:0*/ top__DOT__count_units;
    CData/*3:0*/ top__DOT__u_ps2__DOT__count;
    CData/*2:0*/ top__DOT__u_ps2__DOT__ps2_clk_sync;
    CData/*7:0*/ top__DOT__u_ps2__DOT__prev_scancode;
    CData/*0:0*/ top__DOT__u_ps2__DOT____Vlvbound_h77389395__0;
    CData/*2:0*/ top__DOT__u_com_fsm__DOT__state;
    CData/*0:0*/ top__DOT__u_com_fsm__DOT__quit_clk;
    CData/*1:0*/ top__DOT__u_com_fsm__DOT__pi_money;
    CData/*0:0*/ __Vdly__top__DOT__f0_seen;
    CData/*6:0*/ __Vdly__top__DOT__press_count;
    CData/*7:0*/ __Vdly__top__DOT__u_ps2__DOT__prev_scancode;
    CData/*3:0*/ __Vdly__top__DOT__u_ps2__DOT__count;
    CData/*2:0*/ __Vdly__top__DOT__u_ps2__DOT__ps2_clk_sync;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __VicoFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    CData/*0:0*/ __Vtrigprevexpr___TOP__rst_n__0;
    CData/*0:0*/ __Vtrigprevexpr___TOP__top__DOT____Vcellinp__u_com_fsm__rst_n__0;
    CData/*0:0*/ __VactContinue;
    VL_OUT16(led,15,0);
    SData/*9:0*/ top__DOT__u_ps2__DOT__buffer;
    IData/*23:0*/ top__DOT__cola_timer;
    IData/*23:0*/ top__DOT__refund_timer;
    IData/*31:0*/ top__DOT__u_com_fsm__DOT__wait_timer;
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<CData/*0:0*/, 5> __Vm_traceActivity;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
    VlTriggerVec<3> __VactTriggered;
    VlTriggerVec<3> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vtop__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vtop___024root(Vtop__Syms* symsp, const char* v__name);
    ~Vtop___024root();
    VL_UNCOPYABLE(Vtop___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
