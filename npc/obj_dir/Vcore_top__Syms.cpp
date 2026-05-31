// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "Vcore_top__pch.h"
#include "Vcore_top.h"
#include "Vcore_top___024root.h"

void Vcore_top___024root____Vdpiexp_core_top__DOT__u_csr__DOT__get_mstatus_TOP(Vcore_top__Syms* __restrict vlSymsp, IData/*31:0*/ &get_mstatus__Vfuncrtn);

// FUNCTIONS
Vcore_top__Syms::~Vcore_top__Syms()
{
}

Vcore_top__Syms::Vcore_top__Syms(VerilatedContext* contextp, const char* namep, Vcore_top* modelp)
    : VerilatedSyms{contextp}
    // Setup internal state of the Syms class
    , __Vm_modelp{modelp}
    // Setup module instances
    , TOP{this, namep}
{
        // Check resources
        Verilated::stackCheck(190);
    // Configure time unit / time precision
    _vm_contextp__->timeunit(-12);
    _vm_contextp__->timeprecision(-12);
    // Setup each module's pointers to their submodules
    // Setup each module's pointer back to symbol table (for public functions)
    TOP.__Vconfigure(true);
    // Setup scopes
    __Vscope_core_top__u_csr.configure(this, name(), "core_top.u_csr", "u_csr", "<null>", -12, VerilatedScope::SCOPE_OTHER);
    // Setup export functions
    for (int __Vfinal = 0; __Vfinal < 2; ++__Vfinal) {
        __Vscope_core_top__u_csr.exportInsert(__Vfinal, "get_mstatus", (void*)(&Vcore_top___024root____Vdpiexp_core_top__DOT__u_csr__DOT__get_mstatus_TOP));
    }
}
