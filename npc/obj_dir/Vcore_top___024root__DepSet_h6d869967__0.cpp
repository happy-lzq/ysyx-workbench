// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcore_top.h for the primary calling header

#include "Vcore_top__pch.h"
#include "Vcore_top__Syms.h"
#include "Vcore_top___024root.h"

void Vcore_top___024root____Vdpiexp_core_top__DOT__u_csr__DOT__get_mstatus_TOP(Vcore_top__Syms* __restrict vlSymsp, IData/*31:0*/ &get_mstatus__Vfuncrtn) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root____Vdpiexp_core_top__DOT__u_csr__DOT__get_mstatus_TOP\n"); );
    // Init
    // Body
    get_mstatus__Vfuncrtn = vlSymsp->TOP.core_top__DOT__u_csr__DOT__csr_mstatus;
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcore_top___024root___dump_triggers__act(Vcore_top___024root* vlSelf);
#endif  // VL_DEBUG

void Vcore_top___024root___eval_triggers__act(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_triggers__act\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered.setBit(0U, ((IData)(vlSelfRef.clk) 
                                          & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__clk__0))));
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vcore_top___024root___dump_triggers__act(vlSelf);
    }
#endif
}
