// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vcore_top.h for the primary calling header

#ifndef VERILATED_VCORE_TOP___024ROOT_H_
#define VERILATED_VCORE_TOP___024ROOT_H_  // guard

#include "verilated.h"


class Vcore_top__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vcore_top___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst,0,0);
    CData/*0:0*/ __VactContinue;
    IData/*31:0*/ __VactIterCount;
    VlTriggerVec<0> __VactTriggered;
    VlTriggerVec<0> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vcore_top__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vcore_top___024root(Vcore_top__Syms* symsp, const char* v__name);
    ~Vcore_top___024root();
    VL_UNCOPYABLE(Vcore_top___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
