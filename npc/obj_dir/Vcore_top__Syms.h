// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VCORE_TOP__SYMS_H_
#define VERILATED_VCORE_TOP__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vcore_top.h"

// INCLUDE MODULE CLASSES
#include "Vcore_top___024root.h"

// DPI TYPES for DPI Export callbacks (Internal use)
using Vcore_top__Vcb_get_mstatus_t = void (*) (Vcore_top__Syms* __restrict vlSymsp, IData/*31:0*/ &get_mstatus__Vfuncrtn);

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES)Vcore_top__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vcore_top* const __Vm_modelp;
    bool __Vm_activity = false;  ///< Used by trace routines to determine change occurred
    uint32_t __Vm_baseCode = 0;  ///< Used by trace routines when tracing multiple models
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vcore_top___024root            TOP;

    // SCOPE NAMES
    VerilatedScope __Vscope_core_top__u_csr;

    // CONSTRUCTORS
    Vcore_top__Syms(VerilatedContext* contextp, const char* namep, Vcore_top* modelp);
    ~Vcore_top__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
};

#endif  // guard
