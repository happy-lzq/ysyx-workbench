// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vvga_top__Syms.h"


void Vvga_top___024root__trace_chg_0_sub_0(Vvga_top___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vvga_top___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root__trace_chg_0\n"); );
    // Init
    Vvga_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vvga_top___024root*>(voidSelf);
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vvga_top___024root__trace_chg_0_sub_0((&vlSymsp->TOP), bufp);
}

void Vvga_top___024root__trace_chg_0_sub_0(Vvga_top___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root__trace_chg_0_sub_0\n"); );
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[1U]))) {
        bufp->chgSData(oldp+0,(vlSelfRef.vga_top__DOT__h_addr),10);
        bufp->chgSData(oldp+1,(vlSelfRef.vga_top__DOT__v_addr),10);
        bufp->chgIData(oldp+2,(vlSelfRef.vga_top__DOT__U_vga_pic__DOT__vga_mem
                               [((0x7fc00U & ((IData)(vlSelfRef.vga_top__DOT__v_addr) 
                                              << 0xaU)) 
                                 | (IData)(vlSelfRef.vga_top__DOT__h_addr))]),24);
        bufp->chgSData(oldp+3,((0x1ffU & (IData)(vlSelfRef.vga_top__DOT__v_addr))),9);
        bufp->chgSData(oldp+4,(vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__x_cnt),10);
        bufp->chgSData(oldp+5,(vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__y_cnt),10);
        bufp->chgBit(oldp+6,(vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__h_valid));
        bufp->chgBit(oldp+7,(vlSelfRef.vga_top__DOT__my_vga_ctrl__DOT__v_valid));
    }
    bufp->chgBit(oldp+8,(vlSelfRef.clk));
    bufp->chgBit(oldp+9,(vlSelfRef.rst));
    bufp->chgBit(oldp+10,(vlSelfRef.VGA_HSYNC));
    bufp->chgBit(oldp+11,(vlSelfRef.VGA_VSYNC));
    bufp->chgBit(oldp+12,(vlSelfRef.VGA_BLANK_N));
    bufp->chgCData(oldp+13,(vlSelfRef.VGA_R),8);
    bufp->chgCData(oldp+14,(vlSelfRef.VGA_G),8);
    bufp->chgCData(oldp+15,(vlSelfRef.VGA_B),8);
}

void Vvga_top___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vvga_top___024root__trace_cleanup\n"); );
    // Init
    Vvga_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vvga_top___024root*>(voidSelf);
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    vlSymsp->__Vm_activity = false;
    vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
}
