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
    VL_OUT8(halt,0,0);
    CData/*4:0*/ core_top__DOT__rs1_addr;
    CData/*4:0*/ core_top__DOT__rs2_addr;
    CData/*4:0*/ core_top__DOT__rd_addr;
    CData/*0:0*/ core_top__DOT__mem_read;
    CData/*0:0*/ core_top__DOT__mem_write;
    CData/*0:0*/ core_top__DOT__reg_write;
    CData/*1:0*/ core_top__DOT__pc_sel;
    CData/*4:0*/ core_top__DOT__alu_op;
    CData/*0:0*/ core_top__DOT__is_ebreak;
    CData/*0:0*/ core_top__DOT__alu_src_a;
    CData/*1:0*/ core_top__DOT__alu_src_b;
    CData/*2:0*/ core_top__DOT__br_type;
    CData/*2:0*/ core_top__DOT__lsu_type;
    CData/*1:0*/ core_top__DOT__reg_wdata_src;
    CData/*1:0*/ core_top__DOT__csr_op;
    CData/*4:0*/ core_top__DOT__csr_rs1;
    CData/*0:0*/ core_top__DOT__csr_imm;
    CData/*0:0*/ core_top__DOT__csr_write;
    CData/*0:0*/ core_top__DOT__csr_read;
    CData/*0:0*/ core_top__DOT__mret;
    CData/*0:0*/ core_top__DOT__trap_enter;
    CData/*3:0*/ core_top__DOT__trap_code;
    CData/*6:0*/ core_top__DOT__u_id_stage__DOT__opcode;
    CData/*2:0*/ core_top__DOT__u_id_stage__DOT__funct3;
    CData/*6:0*/ core_top__DOT__u_id_stage__DOT__funct7;
    CData/*0:0*/ core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0;
    CData/*3:0*/ core_top__DOT__u_mem_stage__DOT__mem_wmask;
    CData/*7:0*/ core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    CData/*0:0*/ __VactContinue;
    SData/*11:0*/ core_top__DOT__csr_addr;
    SData/*15:0*/ core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected;
    VL_OUT(halt_pc,31,0);
    VL_OUT(halt_ret,31,0);
    IData/*31:0*/ core_top__DOT__rs1_rdata;
    IData/*31:0*/ core_top__DOT__rs2_rdata;
    IData/*31:0*/ core_top__DOT__jump_jalr;
    IData/*31:0*/ core_top__DOT__imm_jal;
    IData/*31:0*/ core_top__DOT__pc;
    IData/*31:0*/ core_top__DOT__mem_rdata_raw;
    IData/*31:0*/ core_top__DOT__csr_wdata;
    IData/*31:0*/ core_top__DOT__csr_rdata;
    IData/*31:0*/ core_top__DOT__u_csr__DOT__csr_mstatus;
    IData/*31:0*/ core_top__DOT__u_csr__DOT__csr_mtvec;
    IData/*31:0*/ core_top__DOT__u_csr__DOT__csr_mepc;
    IData/*31:0*/ core_top__DOT__u_csr__DOT__csr_mcause;
    IData/*31:0*/ core_top__DOT__u_if_stage__DOT__pc_next;
    IData/*31:0*/ core_top__DOT__u_ex_stage__DOT__src1;
    IData/*31:0*/ core_top__DOT__u_ex_stage__DOT__src2;
    IData/*31:0*/ core_top__DOT__u_ex_stage__DOT__csr_src;
    IData/*31:0*/ core_top__DOT__u_mem_stage__DOT__mem_wdata;
    IData/*31:0*/ __VdfgRegularize_h3c5bdd75_0_0;
    IData/*31:0*/ __VactIterCount;
    QData/*63:0*/ core_top__DOT__u_csr__DOT__mcycle_64;
    VlUnpacked<IData/*31:0*/, 32> core_top__DOT__u_regfile__DOT__rf;
    VlUnpacked<IData/*31:0*/, 262144> core_top__DOT__u_if_stage__DOT__imem;
    VlUnpacked<IData/*31:0*/, 262144> core_top__DOT__u_mem_stage__DOT__dmem;
    VlUnpacked<CData/*0:0*/, 2> __Vm_traceActivity;
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
