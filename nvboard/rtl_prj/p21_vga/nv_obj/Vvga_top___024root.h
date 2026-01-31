// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vvga_top.h for the primary calling header

#ifndef VERILATED_VVGA_TOP___024ROOT_H_
#define VERILATED_VVGA_TOP___024ROOT_H_  // guard

#include "verilated.h"


class Vvga_top__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vvga_top___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst,0,0);
    VL_OUT8(VGA_HSYNC,0,0);
    VL_OUT8(VGA_VSYNC,0,0);
    VL_OUT8(VGA_BLANK_N,0,0);
    VL_OUT8(VGA_R,7,0);
    VL_OUT8(VGA_G,7,0);
    VL_OUT8(VGA_B,7,0);
    CData/*0:0*/ vga_top__DOT__my_vga_ctrl__DOT__h_valid;
    CData/*0:0*/ vga_top__DOT__my_vga_ctrl__DOT__v_valid;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    CData/*0:0*/ __VactContinue;
    SData/*9:0*/ vga_top__DOT__h_addr;
    SData/*9:0*/ vga_top__DOT__v_addr;
    SData/*9:0*/ vga_top__DOT__my_vga_ctrl__DOT__x_cnt;
    SData/*9:0*/ vga_top__DOT__my_vga_ctrl__DOT__y_cnt;
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<IData/*23:0*/, 524288> vga_top__DOT__U_vga_pic__DOT__vga_mem;
    VlUnpacked<CData/*0:0*/, 2> __Vm_traceActivity;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VactTriggered;
    VlTriggerVec<1> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vvga_top__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vvga_top___024root(Vvga_top__Syms* symsp, const char* v__name);
    ~Vvga_top___024root();
    VL_UNCOPYABLE(Vvga_top___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
