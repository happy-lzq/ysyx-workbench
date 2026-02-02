// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vvga_top.h for the primary calling header

#include "Vvga_top__pch.h"
#include "Vvga_top__Syms.h"
#include "Vvga_top___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vvga_top___024root___dump_triggers__act(Vvga_top___024root* vlSelf);
#endif  // VL_DEBUG

void Vvga_top___024root___eval_triggers__act(Vvga_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root___eval_triggers__act\n"); );
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered.setBit(0U, ((IData)(vlSelfRef.clk) 
                                          & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__clk__0))));
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vvga_top___024root___dump_triggers__act(vlSelf);
    }
#endif
}
