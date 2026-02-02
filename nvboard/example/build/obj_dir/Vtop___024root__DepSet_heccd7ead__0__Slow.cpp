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
    // Init
    VlWide<5>/*159:0*/ __Vtemp_1;
    // Body
    vlSelfRef.top__DOT__my_seg__DOT__segs[0U] = 0xfdU;
    vlSelfRef.top__DOT__my_seg__DOT__segs[1U] = 0x60U;
    vlSelfRef.top__DOT__my_seg__DOT__segs[2U] = 0xdaU;
    vlSelfRef.top__DOT__my_seg__DOT__segs[3U] = 0xf2U;
    vlSelfRef.top__DOT__my_seg__DOT__segs[4U] = 0x66U;
    vlSelfRef.top__DOT__my_seg__DOT__segs[5U] = 0xb6U;
    vlSelfRef.top__DOT__my_seg__DOT__segs[6U] = 0xbeU;
    vlSelfRef.top__DOT__my_seg__DOT__segs[7U] = 0xe0U;
    __Vtemp_1[0U] = 0x2e686578U;
    __Vtemp_1[1U] = 0x74757265U;
    __Vtemp_1[2U] = 0x2f706963U;
    __Vtemp_1[3U] = 0x75726365U;
    __Vtemp_1[4U] = 0x7265736fU;
    VL_READMEM_N(true, 24, 524288, 0, VL_CVT_PACK_STR_NW(5, __Vtemp_1)
                 ,  &(vlSelfRef.top__DOT__my_vmem__DOT__vga_mem)
                 , 0, ~0ULL);
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
            VL_FATAL_MT("/home/l/ysyx/ysyx-workbench/nvboard/example/vsrc/top.v", 1, "", "Settle region did not converge.");
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
    SData/*9:0*/ top__DOT__h_addr;
    top__DOT__h_addr = 0;
    SData/*9:0*/ top__DOT__v_addr;
    top__DOT__v_addr = 0;
    CData/*0:0*/ top__DOT__my_vga_ctrl__DOT__h_valid;
    top__DOT__my_vga_ctrl__DOT__h_valid = 0;
    CData/*0:0*/ top__DOT__my_vga_ctrl__DOT__v_valid;
    top__DOT__my_vga_ctrl__DOT__v_valid = 0;
    // Body
    vlSelfRef.VGA_CLK = vlSelfRef.clk;
    vlSelfRef.VGA_HSYNC = (0x60U < (IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__x_cnt));
    vlSelfRef.VGA_VSYNC = (2U < (IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__y_cnt));
    vlSelfRef.uart_tx = vlSelfRef.uart_rx;
    vlSelfRef.seg0 = (0xffU & (~ vlSelfRef.top__DOT__my_seg__DOT__segs
                               [vlSelfRef.top__DOT__my_seg__DOT__offset]));
    vlSelfRef.seg1 = (0xffU & (~ vlSelfRef.top__DOT__my_seg__DOT__segs
                               [(7U & ((IData)(1U) 
                                       + (IData)(vlSelfRef.top__DOT__my_seg__DOT__offset)))]));
    vlSelfRef.seg2 = (0xffU & (~ vlSelfRef.top__DOT__my_seg__DOT__segs
                               [(7U & ((IData)(2U) 
                                       + (IData)(vlSelfRef.top__DOT__my_seg__DOT__offset)))]));
    vlSelfRef.seg3 = (0xffU & (~ vlSelfRef.top__DOT__my_seg__DOT__segs
                               [(7U & ((IData)(3U) 
                                       + (IData)(vlSelfRef.top__DOT__my_seg__DOT__offset)))]));
    vlSelfRef.seg4 = (0xffU & (~ vlSelfRef.top__DOT__my_seg__DOT__segs
                               [(7U & ((IData)(4U) 
                                       + (IData)(vlSelfRef.top__DOT__my_seg__DOT__offset)))]));
    vlSelfRef.seg5 = (0xffU & (~ vlSelfRef.top__DOT__my_seg__DOT__segs
                               [(7U & ((IData)(5U) 
                                       + (IData)(vlSelfRef.top__DOT__my_seg__DOT__offset)))]));
    vlSelfRef.seg6 = (0xffU & (~ vlSelfRef.top__DOT__my_seg__DOT__segs
                               [(7U & ((IData)(6U) 
                                       + (IData)(vlSelfRef.top__DOT__my_seg__DOT__offset)))]));
    vlSelfRef.seg7 = (0xffU & (~ vlSelfRef.top__DOT__my_seg__DOT__segs
                               [(7U & ((IData)(7U) 
                                       + (IData)(vlSelfRef.top__DOT__my_seg__DOT__offset)))]));
    vlSelfRef.ledr = ((0xe000U & ((IData)(vlSelfRef.top__DOT__my_led__DOT__led) 
                                  << 8U)) | ((0x1f00U 
                                              & (((IData)(vlSelfRef.top__DOT__my_led__DOT__led) 
                                                  ^ (IData)(vlSelfRef.btn)) 
                                                 << 8U)) 
                                             | (IData)(vlSelfRef.sw)));
    top__DOT__my_vga_ctrl__DOT__v_valid = ((0x23U < (IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__y_cnt)) 
                                           & (0x203U 
                                              >= (IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__y_cnt)));
    top__DOT__my_vga_ctrl__DOT__h_valid = ((0x90U < (IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__x_cnt)) 
                                           & (0x310U 
                                              >= (IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__x_cnt)));
    if (top__DOT__my_vga_ctrl__DOT__v_valid) {
        top__DOT__v_addr = (0x3ffU & ((IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__y_cnt) 
                                      - (IData)(0x24U)));
        vlSelfRef.VGA_BLANK_N = top__DOT__my_vga_ctrl__DOT__h_valid;
    } else {
        top__DOT__v_addr = 0U;
        vlSelfRef.VGA_BLANK_N = 0U;
    }
    top__DOT__h_addr = ((IData)(top__DOT__my_vga_ctrl__DOT__h_valid)
                         ? (0x3ffU & ((IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__x_cnt) 
                                      - (IData)(0x91U)))
                         : 0U);
    vlSelfRef.VGA_R = (0xffU & (vlSelfRef.top__DOT__my_vmem__DOT__vga_mem
                                [(((IData)(top__DOT__h_addr) 
                                   << 9U) | (0x1ffU 
                                             & (IData)(top__DOT__v_addr)))] 
                                >> 0x10U));
    vlSelfRef.VGA_G = (0xffU & (vlSelfRef.top__DOT__my_vmem__DOT__vga_mem
                                [(((IData)(top__DOT__h_addr) 
                                   << 9U) | (0x1ffU 
                                             & (IData)(top__DOT__v_addr)))] 
                                >> 8U));
    vlSelfRef.VGA_B = (0xffU & vlSelfRef.top__DOT__my_vmem__DOT__vga_mem
                       [(((IData)(top__DOT__h_addr) 
                          << 9U) | (0x1ffU & (IData)(top__DOT__v_addr)))]);
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
VL_ATTR_COLD void Vtop___024root___dump_triggers__ico(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__ico\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VicoTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VicoTriggered.word(0U))) {
        VL_DBG_MSGF("         'ico' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

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
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtop___024root___ctor_var_reset(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ctor_var_reset\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelf->clk = 0;
    vlSelf->rst = 0;
    vlSelf->btn = 0;
    vlSelf->sw = 0;
    vlSelf->ps2_clk = 0;
    vlSelf->ps2_data = 0;
    vlSelf->uart_rx = 0;
    vlSelf->uart_tx = 0;
    vlSelf->ledr = 0;
    vlSelf->VGA_CLK = 0;
    vlSelf->VGA_HSYNC = 0;
    vlSelf->VGA_VSYNC = 0;
    vlSelf->VGA_BLANK_N = 0;
    vlSelf->VGA_R = 0;
    vlSelf->VGA_G = 0;
    vlSelf->VGA_B = 0;
    vlSelf->seg0 = 0;
    vlSelf->seg1 = 0;
    vlSelf->seg2 = 0;
    vlSelf->seg3 = 0;
    vlSelf->seg4 = 0;
    vlSelf->seg5 = 0;
    vlSelf->seg6 = 0;
    vlSelf->seg7 = 0;
    vlSelf->top__DOT__my_led__DOT__count = 0;
    vlSelf->top__DOT__my_led__DOT__led = 0;
    vlSelf->top__DOT__my_vga_ctrl__DOT__x_cnt = 0;
    vlSelf->top__DOT__my_vga_ctrl__DOT__y_cnt = 0;
    vlSelf->top__DOT__my_keyboard__DOT__buffer = 0;
    vlSelf->top__DOT__my_keyboard__DOT__count = 0;
    vlSelf->top__DOT__my_keyboard__DOT__ps2_clk_sync = 0;
    vlSelf->top__DOT__my_keyboard__DOT____Vlvbound_h77389395__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 8; ++__Vi0) {
        vlSelf->top__DOT__my_seg__DOT__segs[__Vi0] = 0;
    }
    vlSelf->top__DOT__my_seg__DOT__count = 0;
    vlSelf->top__DOT__my_seg__DOT__offset = 0;
    for (int __Vi0 = 0; __Vi0 < 524288; ++__Vi0) {
        vlSelf->top__DOT__my_vmem__DOT__vga_mem[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = 0;
}
