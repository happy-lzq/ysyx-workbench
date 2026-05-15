// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vcore_top.h for the primary calling header

#ifndef VERILATED_VCORE_TOP_REGFILE_H_
#define VERILATED_VCORE_TOP_REGFILE_H_  // guard

#include "verilated.h"


class Vcore_top__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vcore_top_regfile final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(__PVT__clk,0,0);
    VL_IN8(__PVT__rs1_addr,4,0);
    VL_IN8(__PVT__rs2_addr,4,0);
    VL_IN8(__PVT__rd_addr,4,0);
    VL_IN8(__PVT__reg_write,0,0);
    VL_IN(__PVT__rd_wdata,31,0);
    VL_OUT(__PVT__rs1_rdata,31,0);
    VL_OUT(__PVT__rs2_rdata,31,0);
    VlUnpacked<IData/*31:0*/, 32> rf;

    // INTERNAL VARIABLES
    Vcore_top__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vcore_top_regfile(Vcore_top__Syms* symsp, const char* v__name);
    ~Vcore_top_regfile();
    VL_UNCOPYABLE(Vcore_top_regfile);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
