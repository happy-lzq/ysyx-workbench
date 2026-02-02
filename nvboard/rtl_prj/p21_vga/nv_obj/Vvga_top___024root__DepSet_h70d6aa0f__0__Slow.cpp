// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vvga_top.h for the primary calling header

#include "Vvga_top__pch.h"
#include "Vvga_top___024root.h"

VL_ATTR_COLD void Vvga_top___024root___eval_static(Vvga_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root___eval_static\n"); );
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
}

VL_ATTR_COLD void Vvga_top___024root___eval_initial__TOP(Vvga_top___024root* vlSelf);
VL_ATTR_COLD void Vvga_top___024root____Vm_traceActivitySetAll(Vvga_top___024root* vlSelf);

VL_ATTR_COLD void Vvga_top___024root___eval_initial(Vvga_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root___eval_initial\n"); );
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vvga_top___024root___eval_initial__TOP(vlSelf);
    Vvga_top___024root____Vm_traceActivitySetAll(vlSelf);
}

extern const VlWide<18>/*575:0*/ Vvga_top__ConstPool__CONST_h9e16131e_0;

VL_ATTR_COLD void Vvga_top___024root___eval_initial__TOP(Vvga_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root___eval_initial__TOP\n"); );
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    VL_READMEM_N(true, 24, 524288, 0, VL_CVT_PACK_STR_NW(18, Vvga_top__ConstPool__CONST_h9e16131e_0)
                 ,  &(vlSelfRef.vga_top__DOT__U_vga_pic__DOT__vga_mem)
                 , 0, ~0ULL);
}

VL_ATTR_COLD void Vvga_top___024root___eval_final(Vvga_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root___eval_final\n"); );
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vvga_top___024root___dump_triggers__stl(Vvga_top___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vvga_top___024root___eval_phase__stl(Vvga_top___024root* vlSelf);

VL_ATTR_COLD void Vvga_top___024root___eval_settle(Vvga_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root___eval_settle\n"); );
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
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
            Vvga_top___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("p21_vga/vsrc/vga_top.v", 1, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vvga_top___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelfRef.__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vvga_top___024root___dump_triggers__stl(Vvga_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root___dump_triggers__stl\n"); );
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
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

VL_ATTR_COLD void Vvga_top___024root___stl_sequent__TOP__0(Vvga_top___024root* vlSelf);

VL_ATTR_COLD void Vvga_top___024root___eval_stl(Vvga_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root___eval_stl\n"); );
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        Vvga_top___024root___stl_sequent__TOP__0(vlSelf);
        Vvga_top___024root____Vm_traceActivitySetAll(vlSelf);
    }
}

VL_ATTR_COLD void Vvga_top___024root___stl_sequent__TOP__0(Vvga_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root___stl_sequent__TOP__0\n"); );
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.VGA_HSYNC = (0x60U < (IData)(vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__x_cnt));
    vlSelfRef.VGA_VSYNC = (2U < (IData)(vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__y_cnt));
    vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__v_valid 
        = ((0x23U < (IData)(vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__y_cnt)) 
           & (0x203U >= (IData)(vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__y_cnt)));
    vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__h_valid 
        = ((0x90U < (IData)(vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__x_cnt)) 
           & (0x310U >= (IData)(vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__x_cnt)));
    if (vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__v_valid) {
        vlSelfRef.vga_top__DOT__v_addr = (0x3ffU & 
                                          ((IData)(vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__y_cnt) 
                                           - (IData)(0x23U)));
        vlSelfRef.VGA_BLANK_N = vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__h_valid;
    } else {
        vlSelfRef.vga_top__DOT__v_addr = 0U;
        vlSelfRef.VGA_BLANK_N = 0U;
    }
    vlSelfRef.vga_top__DOT__h_addr = ((IData)(vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__h_valid)
                                       ? (0x3ffU & 
                                          ((IData)(vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__x_cnt) 
                                           - (IData)(0x90U)))
                                       : 0U);
    vlSelfRef.VGA_R = (0xffU & (vlSelfRef.vga_top__DOT__U_vga_pic__DOT__vga_mem
                                [((0x7fc00U & ((IData)(vlSelfRef.vga_top__DOT__v_addr) 
                                               << 0xaU)) 
                                  | (IData)(vlSelfRef.vga_top__DOT__h_addr))] 
                                >> 0x10U));
    vlSelfRef.VGA_G = (0xffU & (vlSelfRef.vga_top__DOT__U_vga_pic__DOT__vga_mem
                                [((0x7fc00U & ((IData)(vlSelfRef.vga_top__DOT__v_addr) 
                                               << 0xaU)) 
                                  | (IData)(vlSelfRef.vga_top__DOT__h_addr))] 
                                >> 8U));
    vlSelfRef.VGA_B = (0xffU & vlSelfRef.vga_top__DOT__U_vga_pic__DOT__vga_mem
                       [((0x7fc00U & ((IData)(vlSelfRef.vga_top__DOT__v_addr) 
                                      << 0xaU)) | (IData)(vlSelfRef.vga_top__DOT__h_addr))]);
}

VL_ATTR_COLD void Vvga_top___024root___eval_triggers__stl(Vvga_top___024root* vlSelf);

VL_ATTR_COLD bool Vvga_top___024root___eval_phase__stl(Vvga_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root___eval_phase__stl\n"); );
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vvga_top___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelfRef.__VstlTriggered.any();
    if (__VstlExecute) {
        Vvga_top___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vvga_top___024root___dump_triggers__act(Vvga_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root___dump_triggers__act\n"); );
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VactTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vvga_top___024root___dump_triggers__nba(Vvga_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root___dump_triggers__nba\n"); );
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VnbaTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vvga_top___024root____Vm_traceActivitySetAll(Vvga_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root____Vm_traceActivitySetAll\n"); );
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vm_traceActivity[0U] = 1U;
    vlSelfRef.__Vm_traceActivity[1U] = 1U;
}

VL_ATTR_COLD void Vvga_top___024root___ctor_var_reset(Vvga_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root___ctor_var_reset\n"); );
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->name());
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18209466448985614591ull);
    vlSelf->VGA_HSYNC = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1085272180465251897ull);
    vlSelf->VGA_VSYNC = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1001503147665162868ull);
    vlSelf->VGA_BLANK_N = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4207106729344237849ull);
    vlSelf->VGA_R = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8713651417395270250ull);
    vlSelf->VGA_G = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2930052969200674513ull);
    vlSelf->VGA_B = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 18057832749456581997ull);
    vlSelf->vga_top__DOT__h_addr = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 9092048359541713116ull);
    vlSelf->vga_top__DOT__v_addr = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 11258252383761419540ull);
    vlSelf->vga_top__DOT__my_vga_ctrl__DOT__x_cnt = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 14235085920434978369ull);
    vlSelf->vga_top__DOT__my_vga_ctrl__DOT__y_cnt = VL_SCOPED_RAND_RESET_I(10, __VscopeHash, 497104729745680770ull);
    vlSelf->vga_top__DOT__my_vga_ctrl__DOT__h_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13969551789608662371ull);
    vlSelf->vga_top__DOT__my_vga_ctrl__DOT__v_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14644962933724808825ull);
    for (int __Vi0 = 0; __Vi0 < 524288; ++__Vi0) {
        vlSelf->vga_top__DOT__U_vga_pic__DOT__vga_mem[__Vi0] = VL_SCOPED_RAND_RESET_I(24, __VscopeHash, 410178419498219225ull);
    }
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9526919608049418986ull);
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        vlSelf->__Vm_traceActivity[__Vi0] = 0;
    }
}
