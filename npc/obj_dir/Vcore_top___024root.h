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
    CData/*4:0*/ core_top__DOT__rd_addr;
    CData/*0:0*/ core_top__DOT__mem_read;
    CData/*0:0*/ core_top__DOT__mem_write;
    CData/*0:0*/ core_top__DOT__reg_write;
    CData/*2:0*/ core_top__DOT__lsu_type;
    CData/*1:0*/ core_top__DOT__reg_wdata_src;
    CData/*3:0*/ core_top__DOT__u_mem_stage__DOT__mem_wmask;
    CData/*7:0*/ core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    CData/*0:0*/ __VactContinue;
    SData/*15:0*/ core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected;
    VL_OUT(debug_pc,31,0);
    IData/*31:0*/ core_top__DOT__pc;
    IData/*31:0*/ core_top__DOT__alu_result;
    IData/*31:0*/ core_top__DOT__u_if_stage__DOT__pc_next;
    IData/*31:0*/ core_top__DOT__u_mem_stage__DOT__mem_wdata;
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<IData/*31:0*/, 32> core_top__DOT__u_regfile__DOT__rf;
    VlUnpacked<IData/*31:0*/, 65536> core_top__DOT__u_if_stage__DOT__imem;
    VlUnpacked<IData/*31:0*/, 65536> core_top__DOT__u_mem_stage__DOT__mem;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VactTriggered;
    VlTriggerVec<1> __VnbaTriggered;

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
