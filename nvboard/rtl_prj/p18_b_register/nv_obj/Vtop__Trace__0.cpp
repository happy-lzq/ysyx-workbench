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
    bufp->chgBit(oldp+0,(vlSelfRef.l_r));
    bufp->chgBit(oldp+1,(vlSelfRef.a_l));
    bufp->chgCData(oldp+2,(vlSelfRef.shmat),3);
    bufp->chgCData(oldp+3,(vlSelfRef.din),8);
    bufp->chgCData(oldp+4,(vlSelfRef.seg0),8);
    bufp->chgCData(oldp+5,(vlSelfRef.seg1),8);
    bufp->chgCData(oldp+6,(vlSelfRef.seg2),8);
    bufp->chgCData(oldp+7,(vlSelfRef.seg3),8);
    bufp->chgCData(oldp+8,(vlSelfRef.seg4),8);
    bufp->chgCData(oldp+9,(vlSelfRef.seg5),8);
    bufp->chgCData(oldp+10,(vlSelfRef.seg6),8);
    bufp->chgCData(oldp+11,(vlSelfRef.seg7),8);
    bufp->chgCData(oldp+12,(vlSelfRef.dout),8);
    bufp->chgCData(oldp+13,(vlSelfRef.top__DOT__u_b_shifter__DOT__al),8);
    bufp->chgCData(oldp+14,((0xffU & VL_SHIFTRS_III(8,8,3, (IData)(vlSelfRef.din), (IData)(vlSelfRef.shmat)))),8);
    bufp->chgCData(oldp+15,((0xffU & ((IData)(vlSelfRef.din) 
                                      >> (IData)(vlSelfRef.shmat)))),8);
}

void Vtop___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_cleanup\n"); );
    // Init
    Vtop___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtop___024root*>(voidSelf);
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VlUnpacked<CData/*0:0*/, 1> __Vm_traceActivity;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        __Vm_traceActivity[__Vi0] = 0;
    }
    // Body
    vlSymsp->__Vm_activity = false;
    __Vm_traceActivity[0U] = 0U;
}
