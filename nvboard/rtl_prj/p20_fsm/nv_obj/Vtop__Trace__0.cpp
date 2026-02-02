// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vtop__Syms.h"


void Vtop___024root__trace_chg_0_sub_0(Vtop___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vtop___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_chg_0\n"); );
    // Init
    Vtop___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtop___024root*>(voidSelf);
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vtop___024root__trace_chg_0_sub_0((&vlSymsp->TOP), bufp);
}

void Vtop___024root__trace_chg_0_sub_0(Vtop___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_chg_0_sub_0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[1U]))) {
        bufp->chgBit(oldp+0,(vlSelfRef.top__DOT__one_pulse));
        bufp->chgBit(oldp+1,(vlSelfRef.top__DOT__half_pulse));
        bufp->chgIData(oldp+2,(vlSelfRef.top__DOT__cola_timer),24);
        bufp->chgIData(oldp+3,(vlSelfRef.top__DOT__refund_timer),24);
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[2U]))) {
        bufp->chgBit(oldp+4,(vlSelfRef.top__DOT__po_money));
        bufp->chgBit(oldp+5,(vlSelfRef.top__DOT__po_cola));
        bufp->chgCData(oldp+6,(vlSelfRef.top__DOT__u_com_fsm__DOT__state),3);
        bufp->chgCData(oldp+7,(vlSelfRef.top__DOT__quit_out),3);
        bufp->chgBit(oldp+8,(vlSelfRef.top__DOT__u_com_fsm__DOT__quit_clk));
        bufp->chgIData(oldp+9,(vlSelfRef.top__DOT__u_com_fsm__DOT__wait_timer),32);
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[3U]))) {
        bufp->chgCData(oldp+10,(vlSelfRef.top__DOT__scancode),8);
        bufp->chgCData(oldp+11,(vlSelfRef.top__DOT__press_count),7);
        bufp->chgBit(oldp+12,(vlSelfRef.top__DOT__valid));
        bufp->chgCData(oldp+13,(vlSelfRef.top__DOT__released_data),8);
        bufp->chgBit(oldp+14,(vlSelfRef.top__DOT__key_release_pulse));
        bufp->chgBit(oldp+15,(vlSelfRef.top__DOT__f0_seen));
        bufp->chgBit(oldp+16,(vlSelfRef.top__DOT__system_enable));
        bufp->chgBit(oldp+17,(vlSelfRef.top__DOT__pi_quit));
        bufp->chgCData(oldp+18,(((IData)(vlSelfRef.top__DOT__system_enable)
                                  ? (0xfU & VL_DIV_III(7, (IData)(vlSelfRef.top__DOT__press_count), (IData)(0xaU)))
                                  : 0xaU)),4);
        bufp->chgCData(oldp+19,(((IData)(vlSelfRef.top__DOT__system_enable)
                                  ? (0xfU & VL_MODDIV_III(7, (IData)(vlSelfRef.top__DOT__press_count), (IData)(0xaU)))
                                  : 0xaU)),4);
        bufp->chgCData(oldp+20,(vlSelfRef.top__DOT__count_tens),7);
        bufp->chgCData(oldp+21,(vlSelfRef.top__DOT__count_units),7);
        bufp->chgCData(oldp+22,(vlSelfRef.top__DOT__u_com_fsm__DOT__pi_money),2);
        bufp->chgSData(oldp+23,(vlSelfRef.top__DOT__u_ps2__DOT__buffer),10);
        bufp->chgCData(oldp+24,(vlSelfRef.top__DOT__u_ps2__DOT__count),4);
        bufp->chgCData(oldp+25,(vlSelfRef.top__DOT__u_ps2__DOT__prev_scancode),8);
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[4U]))) {
        bufp->chgCData(oldp+26,(vlSelfRef.top__DOT__digit_int),4);
        bufp->chgCData(oldp+27,(vlSelfRef.top__DOT__digit_dec),4);
        bufp->chgCData(oldp+28,(vlSelfRef.top__DOT__h_int),7);
        bufp->chgCData(oldp+29,(vlSelfRef.top__DOT__h_dec),7);
        bufp->chgCData(oldp+30,(vlSelfRef.top__DOT__change_int),4);
        bufp->chgCData(oldp+31,(vlSelfRef.top__DOT__change_dec),4);
        bufp->chgCData(oldp+32,(vlSelfRef.top__DOT__h_ch_int),7);
        bufp->chgCData(oldp+33,(vlSelfRef.top__DOT__h_ch_dec),7);
    }
    bufp->chgBit(oldp+34,(vlSelfRef.clk));
    bufp->chgBit(oldp+35,(vlSelfRef.rst_n));
    bufp->chgBit(oldp+36,(vlSelfRef.ps2_clk));
    bufp->chgBit(oldp+37,(vlSelfRef.ps2_data));
    bufp->chgSData(oldp+38,(vlSelfRef.led),16);
    bufp->chgCData(oldp+39,(vlSelfRef.seg0),8);
    bufp->chgCData(oldp+40,(vlSelfRef.seg1),8);
    bufp->chgCData(oldp+41,(vlSelfRef.seg2),8);
    bufp->chgCData(oldp+42,(vlSelfRef.seg3),8);
    bufp->chgCData(oldp+43,(vlSelfRef.seg4),8);
    bufp->chgCData(oldp+44,(vlSelfRef.seg5),8);
    bufp->chgCData(oldp+45,(vlSelfRef.seg6),8);
    bufp->chgCData(oldp+46,(vlSelfRef.seg7),8);
    bufp->chgBit(oldp+47,(vlSelfRef.top__DOT____Vcellinp__u_com_fsm__rst_n));
    bufp->chgCData(oldp+48,(vlSelfRef.top__DOT__u_ps2__DOT__ps2_clk_sync),3);
    bufp->chgBit(oldp+49,((IData)((4U == (6U & (IData)(vlSelfRef.top__DOT__u_ps2__DOT__ps2_clk_sync))))));
}

void Vtop___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_cleanup\n"); );
    // Init
    Vtop___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtop___024root*>(voidSelf);
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    vlSymsp->__Vm_activity = false;
    vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[2U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[3U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[4U] = 0U;
}
