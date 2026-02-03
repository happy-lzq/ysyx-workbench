// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vvga_top__pch.h"
#include "verilated_vcd_c.h"

//============================================================
// Constructors

Vvga_top::Vvga_top(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vvga_top__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , rst{vlSymsp->TOP.rst}
    , VGA_HSYNC{vlSymsp->TOP.VGA_HSYNC}
    , VGA_VSYNC{vlSymsp->TOP.VGA_VSYNC}
    , VGA_BLANK_N{vlSymsp->TOP.VGA_BLANK_N}
    , VGA_R{vlSymsp->TOP.VGA_R}
    , VGA_G{vlSymsp->TOP.VGA_G}
    , VGA_B{vlSymsp->TOP.VGA_B}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
    contextp()->traceBaseModelCbAdd(
        [this](VerilatedTraceBaseC* tfp, int levels, int options) { traceBaseModel(tfp, levels, options); });
}

Vvga_top::Vvga_top(const char* _vcname__)
    : Vvga_top(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vvga_top::~Vvga_top() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vvga_top___024root___eval_debug_assertions(Vvga_top___024root* vlSelf);
#endif  // VL_DEBUG
void Vvga_top___024root___eval_static(Vvga_top___024root* vlSelf);
void Vvga_top___024root___eval_initial(Vvga_top___024root* vlSelf);
void Vvga_top___024root___eval_settle(Vvga_top___024root* vlSelf);
void Vvga_top___024root___eval(Vvga_top___024root* vlSelf);

void Vvga_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vvga_top::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vvga_top___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vvga_top___024root___eval_static(&(vlSymsp->TOP));
        Vvga_top___024root___eval_initial(&(vlSymsp->TOP));
        Vvga_top___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vvga_top___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vvga_top::eventsPending() { return false; }

uint64_t Vvga_top::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vvga_top::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vvga_top___024root___eval_final(Vvga_top___024root* vlSelf);

VL_ATTR_COLD void Vvga_top::final() {
    Vvga_top___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vvga_top::hierName() const { return vlSymsp->name(); }
const char* Vvga_top::modelName() const { return "Vvga_top"; }
unsigned Vvga_top::threads() const { return 1; }
void Vvga_top::prepareClone() const { contextp()->prepareClone(); }
void Vvga_top::atClone() const {
    contextp()->threadPoolpOnClone();
}
std::unique_ptr<VerilatedTraceConfig> Vvga_top::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false, false, false}};
};

//============================================================
// Trace configuration

void Vvga_top___024root__trace_decl_types(VerilatedVcd* tracep);

void Vvga_top___024root__trace_init_top(Vvga_top___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    Vvga_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vvga_top___024root*>(voidSelf);
    Vvga_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->pushPrefix(std::string{vlSymsp->name()}, VerilatedTracePrefixType::SCOPE_MODULE);
    Vvga_top___024root__trace_decl_types(tracep);
    Vvga_top___024root__trace_init_top(vlSelf, tracep);
    tracep->popPrefix();
}

VL_ATTR_COLD void Vvga_top___024root__trace_register(Vvga_top___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD void Vvga_top::traceBaseModel(VerilatedTraceBaseC* tfp, int levels, int options) {
    (void)levels; (void)options;
    VerilatedVcdC* const stfp = dynamic_cast<VerilatedVcdC*>(tfp);
    if (VL_UNLIKELY(!stfp)) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'Vvga_top::trace()' called on non-VerilatedVcdC object;"
            " use --trace-fst with VerilatedFst object, and --trace-vcd with VerilatedVcd object");
    }
    stfp->spTrace()->addModel(this);
    stfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->TOP));
    Vvga_top___024root__trace_register(&(vlSymsp->TOP), stfp->spTrace());
}
