// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcore_top.h for the primary calling header

#include "Vcore_top__pch.h"
#include "Vcore_top__Syms.h"
#include "Vcore_top_core_top.h"

void Vcore_top_core_top___ctor_var_reset(Vcore_top_core_top* vlSelf);

Vcore_top_core_top::Vcore_top_core_top(Vcore_top__Syms* symsp, const char* v__name)
    : VerilatedModule{v__name}
    , vlSymsp{symsp}
 {
    // Reset structure values
    Vcore_top_core_top___ctor_var_reset(this);
}

void Vcore_top_core_top::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

Vcore_top_core_top::~Vcore_top_core_top() {
}
