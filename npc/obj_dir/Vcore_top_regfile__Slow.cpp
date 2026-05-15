// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcore_top.h for the primary calling header

#include "Vcore_top__pch.h"
#include "Vcore_top__Syms.h"
#include "Vcore_top_regfile.h"

void Vcore_top_regfile___ctor_var_reset(Vcore_top_regfile* vlSelf);

Vcore_top_regfile::Vcore_top_regfile(Vcore_top__Syms* symsp, const char* v__name)
    : VerilatedModule{v__name}
    , vlSymsp{symsp}
 {
    // Reset structure values
    Vcore_top_regfile___ctor_var_reset(this);
}

void Vcore_top_regfile::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

Vcore_top_regfile::~Vcore_top_regfile() {
}
