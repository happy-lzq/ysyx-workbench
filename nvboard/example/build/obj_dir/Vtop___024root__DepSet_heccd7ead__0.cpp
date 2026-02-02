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
    vlSelfRef.VGA_CLK = vlSelfRef.clk;
    vlSelfRef.uart_tx = vlSelfRef.uart_rx;
    vlSelfRef.ledr = ((0xe000U & ((IData)(vlSelfRef.top__DOT__my_led__DOT__led) 
                                  << 8U)) | ((0x1f00U 
                                              & (((IData)(vlSelfRef.top__DOT__my_led__DOT__led) 
                                                  ^ (IData)(vlSelfRef.btn)) 
                                                 << 8U)) 
                                             | (IData)(vlSelfRef.sw)));
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
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtop___024root___nba_sequent__TOP__0(vlSelf);
    }
}

VL_INLINE_OPT void Vtop___024root___nba_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__0\n"); );
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
    CData/*7:0*/ __Vdly__top__DOT__my_led__DOT__led;
    __Vdly__top__DOT__my_led__DOT__led = 0;
    IData/*31:0*/ __Vdly__top__DOT__my_led__DOT__count;
    __Vdly__top__DOT__my_led__DOT__count = 0;
    SData/*9:0*/ __Vdly__top__DOT__my_vga_ctrl__DOT__x_cnt;
    __Vdly__top__DOT__my_vga_ctrl__DOT__x_cnt = 0;
    SData/*9:0*/ __Vdly__top__DOT__my_vga_ctrl__DOT__y_cnt;
    __Vdly__top__DOT__my_vga_ctrl__DOT__y_cnt = 0;
    CData/*2:0*/ __Vdly__top__DOT__my_keyboard__DOT__ps2_clk_sync;
    __Vdly__top__DOT__my_keyboard__DOT__ps2_clk_sync = 0;
    CData/*3:0*/ __Vdly__top__DOT__my_keyboard__DOT__count;
    __Vdly__top__DOT__my_keyboard__DOT__count = 0;
    IData/*31:0*/ __Vdly__top__DOT__my_seg__DOT__count;
    __Vdly__top__DOT__my_seg__DOT__count = 0;
    CData/*2:0*/ __Vdly__top__DOT__my_seg__DOT__offset;
    __Vdly__top__DOT__my_seg__DOT__offset = 0;
    // Body
    __Vdly__top__DOT__my_keyboard__DOT__ps2_clk_sync 
        = vlSelfRef.top__DOT__my_keyboard__DOT__ps2_clk_sync;
    __Vdly__top__DOT__my_led__DOT__count = vlSelfRef.top__DOT__my_led__DOT__count;
    __Vdly__top__DOT__my_keyboard__DOT__count = vlSelfRef.top__DOT__my_keyboard__DOT__count;
    __Vdly__top__DOT__my_led__DOT__led = vlSelfRef.top__DOT__my_led__DOT__led;
    __Vdly__top__DOT__my_seg__DOT__count = vlSelfRef.top__DOT__my_seg__DOT__count;
    __Vdly__top__DOT__my_seg__DOT__offset = vlSelfRef.top__DOT__my_seg__DOT__offset;
    __Vdly__top__DOT__my_vga_ctrl__DOT__x_cnt = vlSelfRef.top__DOT__my_vga_ctrl__DOT__x_cnt;
    __Vdly__top__DOT__my_vga_ctrl__DOT__y_cnt = vlSelfRef.top__DOT__my_vga_ctrl__DOT__y_cnt;
    __Vdly__top__DOT__my_keyboard__DOT__ps2_clk_sync 
        = ((6U & ((IData)(vlSelfRef.top__DOT__my_keyboard__DOT__ps2_clk_sync) 
                  << 1U)) | (IData)(vlSelfRef.ps2_clk));
    if (vlSelfRef.rst) {
        __Vdly__top__DOT__my_keyboard__DOT__count = 0U;
    } else if ((IData)((4U == (6U & (IData)(vlSelfRef.top__DOT__my_keyboard__DOT__ps2_clk_sync))))) {
        if ((0xaU == (IData)(vlSelfRef.top__DOT__my_keyboard__DOT__count))) {
            if (VL_UNLIKELY(((((~ (IData)(vlSelfRef.top__DOT__my_keyboard__DOT__buffer)) 
                               & (IData)(vlSelfRef.ps2_data)) 
                              & VL_REDXOR_32((0x1ffU 
                                              & ((IData)(vlSelfRef.top__DOT__my_keyboard__DOT__buffer) 
                                                 >> 1U))))))) {
                VL_WRITEF_NX("receive %x\n",0,8,(0xffU 
                                                 & ((IData)(vlSelfRef.top__DOT__my_keyboard__DOT__buffer) 
                                                    >> 1U)));
            }
            __Vdly__top__DOT__my_keyboard__DOT__count = 0U;
        } else {
            vlSelfRef.top__DOT__my_keyboard__DOT____Vlvbound_h77389395__0 
                = vlSelfRef.ps2_data;
            if (VL_LIKELY(((9U >= (IData)(vlSelfRef.top__DOT__my_keyboard__DOT__count))))) {
                vlSelfRef.top__DOT__my_keyboard__DOT__buffer 
                    = (((~ ((IData)(1U) << (IData)(vlSelfRef.top__DOT__my_keyboard__DOT__count))) 
                        & (IData)(vlSelfRef.top__DOT__my_keyboard__DOT__buffer)) 
                       | (0x3ffU & ((IData)(vlSelfRef.top__DOT__my_keyboard__DOT____Vlvbound_h77389395__0) 
                                    << (IData)(vlSelfRef.top__DOT__my_keyboard__DOT__count))));
            }
            __Vdly__top__DOT__my_keyboard__DOT__count 
                = (0xfU & ((IData)(1U) + (IData)(vlSelfRef.top__DOT__my_keyboard__DOT__count)));
        }
    }
    if (vlSelfRef.rst) {
        __Vdly__top__DOT__my_led__DOT__led = 1U;
        __Vdly__top__DOT__my_led__DOT__count = 0U;
        __Vdly__top__DOT__my_seg__DOT__count = 0U;
        __Vdly__top__DOT__my_seg__DOT__offset = 0U;
        __Vdly__top__DOT__my_vga_ctrl__DOT__x_cnt = 1U;
        __Vdly__top__DOT__my_vga_ctrl__DOT__y_cnt = 1U;
    } else {
        if ((0U == vlSelfRef.top__DOT__my_led__DOT__count)) {
            __Vdly__top__DOT__my_led__DOT__led = ((0xfeU 
                                                   & ((IData)(vlSelfRef.top__DOT__my_led__DOT__led) 
                                                      << 1U)) 
                                                  | (1U 
                                                     & ((IData)(vlSelfRef.top__DOT__my_led__DOT__led) 
                                                        >> 7U)));
        }
        __Vdly__top__DOT__my_led__DOT__count = ((0x4c4b40U 
                                                 <= vlSelfRef.top__DOT__my_led__DOT__count)
                                                 ? 0U
                                                 : 
                                                ((IData)(1U) 
                                                 + vlSelfRef.top__DOT__my_led__DOT__count));
        if ((0x4c4b40U == vlSelfRef.top__DOT__my_seg__DOT__count)) {
            __Vdly__top__DOT__my_seg__DOT__offset = 
                (7U & ((IData)(1U) + (IData)(vlSelfRef.top__DOT__my_seg__DOT__offset)));
            __Vdly__top__DOT__my_seg__DOT__count = 0U;
        } else {
            __Vdly__top__DOT__my_seg__DOT__count = 
                ((IData)(1U) + vlSelfRef.top__DOT__my_seg__DOT__count);
        }
        if ((0x320U == (IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__x_cnt))) {
            __Vdly__top__DOT__my_vga_ctrl__DOT__y_cnt 
                = ((0x20dU == (IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__y_cnt))
                    ? 1U : (0x3ffU & ((IData)(1U) + (IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__y_cnt))));
            __Vdly__top__DOT__my_vga_ctrl__DOT__x_cnt = 1U;
        } else {
            __Vdly__top__DOT__my_vga_ctrl__DOT__x_cnt 
                = (0x3ffU & ((IData)(1U) + (IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__x_cnt)));
        }
    }
    vlSelfRef.top__DOT__my_keyboard__DOT__count = __Vdly__top__DOT__my_keyboard__DOT__count;
    vlSelfRef.top__DOT__my_keyboard__DOT__ps2_clk_sync 
        = __Vdly__top__DOT__my_keyboard__DOT__ps2_clk_sync;
    vlSelfRef.top__DOT__my_led__DOT__count = __Vdly__top__DOT__my_led__DOT__count;
    vlSelfRef.top__DOT__my_led__DOT__led = __Vdly__top__DOT__my_led__DOT__led;
    vlSelfRef.top__DOT__my_seg__DOT__count = __Vdly__top__DOT__my_seg__DOT__count;
    vlSelfRef.top__DOT__my_seg__DOT__offset = __Vdly__top__DOT__my_seg__DOT__offset;
    vlSelfRef.top__DOT__my_vga_ctrl__DOT__x_cnt = __Vdly__top__DOT__my_vga_ctrl__DOT__x_cnt;
    vlSelfRef.top__DOT__my_vga_ctrl__DOT__y_cnt = __Vdly__top__DOT__my_vga_ctrl__DOT__y_cnt;
    vlSelfRef.ledr = ((0xe000U & ((IData)(vlSelfRef.top__DOT__my_led__DOT__led) 
                                  << 8U)) | ((0x1f00U 
                                              & (((IData)(vlSelfRef.top__DOT__my_led__DOT__led) 
                                                  ^ (IData)(vlSelfRef.btn)) 
                                                 << 8U)) 
                                             | (IData)(vlSelfRef.sw)));
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
    vlSelfRef.VGA_HSYNC = (0x60U < (IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__x_cnt));
    top__DOT__my_vga_ctrl__DOT__h_valid = ((0x90U < (IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__x_cnt)) 
                                           & (0x310U 
                                              >= (IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__x_cnt)));
    vlSelfRef.VGA_VSYNC = (2U < (IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__y_cnt));
    top__DOT__my_vga_ctrl__DOT__v_valid = ((0x23U < (IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__y_cnt)) 
                                           & (0x203U 
                                              >= (IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__y_cnt)));
    if (top__DOT__my_vga_ctrl__DOT__h_valid) {
        top__DOT__h_addr = (0x3ffU & ((IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__x_cnt) 
                                      - (IData)(0x91U)));
        vlSelfRef.VGA_BLANK_N = top__DOT__my_vga_ctrl__DOT__v_valid;
    } else {
        top__DOT__h_addr = 0U;
        vlSelfRef.VGA_BLANK_N = 0U;
    }
    top__DOT__v_addr = ((IData)(top__DOT__my_vga_ctrl__DOT__v_valid)
                         ? (0x3ffU & ((IData)(vlSelfRef.top__DOT__my_vga_ctrl__DOT__y_cnt) 
                                      - (IData)(0x24U)))
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

void Vtop___024root___eval_triggers__act(Vtop___024root* vlSelf);

bool Vtop___024root___eval_phase__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    VlTriggerVec<1> __VpreTriggered;
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
            VL_FATAL_MT("/home/l/ysyx/ysyx-workbench/nvboard/example/vsrc/top.v", 1, "", "Input combinational region did not converge.");
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
            VL_FATAL_MT("/home/l/ysyx/ysyx-workbench/nvboard/example/vsrc/top.v", 1, "", "NBA region did not converge.");
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
                VL_FATAL_MT("/home/l/ysyx/ysyx-workbench/nvboard/example/vsrc/top.v", 1, "", "Active region did not converge.");
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
    if (VL_UNLIKELY(((vlSelfRef.rst & 0xfeU)))) {
        Verilated::overWidthError("rst");}
    if (VL_UNLIKELY(((vlSelfRef.btn & 0xe0U)))) {
        Verilated::overWidthError("btn");}
    if (VL_UNLIKELY(((vlSelfRef.ps2_clk & 0xfeU)))) {
        Verilated::overWidthError("ps2_clk");}
    if (VL_UNLIKELY(((vlSelfRef.ps2_data & 0xfeU)))) {
        Verilated::overWidthError("ps2_data");}
    if (VL_UNLIKELY(((vlSelfRef.uart_rx & 0xfeU)))) {
        Verilated::overWidthError("uart_rx");}
}
#endif  // VL_DEBUG
