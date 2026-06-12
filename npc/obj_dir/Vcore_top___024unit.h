// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vcore_top.h for the primary calling header

#ifndef VERILATED_VCORE_TOP___024UNIT_H_
#define VERILATED_VCORE_TOP___024UNIT_H_  // guard

#include "verilated.h"


class Vcore_top__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vcore_top___024unit final : public VerilatedModule {
  public:

    // INTERNAL VARIABLES
    Vcore_top__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vcore_top___024unit(Vcore_top__Syms* symsp, const char* v__name);
    ~Vcore_top___024unit();
    VL_UNCOPYABLE(Vcore_top___024unit);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
