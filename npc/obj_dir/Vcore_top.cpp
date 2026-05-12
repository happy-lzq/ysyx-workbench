// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vcore_top__pch.h"

//============================================================
// Constructors

Vcore_top::Vcore_top(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vcore_top__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , rst{vlSymsp->TOP.rst}
    , debug_pc{vlSymsp->TOP.debug_pc}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vcore_top::Vcore_top(const char* _vcname__)
    : Vcore_top(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vcore_top::~Vcore_top() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vcore_top___024root___eval_debug_assertions(Vcore_top___024root* vlSelf);
#endif  // VL_DEBUG
void Vcore_top___024root___eval_static(Vcore_top___024root* vlSelf);
void Vcore_top___024root___eval_initial(Vcore_top___024root* vlSelf);
void Vcore_top___024root___eval_settle(Vcore_top___024root* vlSelf);
void Vcore_top___024root___eval(Vcore_top___024root* vlSelf);

void Vcore_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vcore_top::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vcore_top___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vcore_top___024root___eval_static(&(vlSymsp->TOP));
        Vcore_top___024root___eval_initial(&(vlSymsp->TOP));
        Vcore_top___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vcore_top___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vcore_top::eventsPending() { return false; }

uint64_t Vcore_top::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vcore_top::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vcore_top___024root___eval_final(Vcore_top___024root* vlSelf);

VL_ATTR_COLD void Vcore_top::final() {
    Vcore_top___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vcore_top::hierName() const { return vlSymsp->name(); }
const char* Vcore_top::modelName() const { return "Vcore_top"; }
unsigned Vcore_top::threads() const { return 1; }
void Vcore_top::prepareClone() const { contextp()->prepareClone(); }
void Vcore_top::atClone() const {
    contextp()->threadPoolpOnClone();
}
