// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vcore_top.h for the primary calling header

#ifndef VERILATED_VCORE_TOP_CORE_TOP_H_
#define VERILATED_VCORE_TOP_CORE_TOP_H_  // guard

#include "verilated.h"
class Vcore_top_regfile;


class Vcore_top__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vcore_top_core_top final : public VerilatedModule {
  public:
    // CELLS
    Vcore_top_regfile* u_regfile;

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst,0,0);
    CData/*4:0*/ __PVT__rs1_addr;
    CData/*4:0*/ __PVT__rs2_addr;
    CData/*4:0*/ __PVT__rd_addr;
    CData/*0:0*/ __PVT__mem_read;
    CData/*0:0*/ __PVT__mem_write;
    CData/*0:0*/ __PVT__reg_write;
    CData/*1:0*/ __PVT__pc_sel;
    CData/*4:0*/ __PVT__alu_op;
    CData/*0:0*/ __PVT__alu_src_a;
    CData/*1:0*/ __PVT__alu_src_b;
    CData/*2:0*/ __PVT__br_type;
    CData/*2:0*/ __PVT__lsu_type;
    CData/*1:0*/ __PVT__reg_wdata_src;
    CData/*6:0*/ __PVT__u_id_stage__DOT__opcode;
    CData/*2:0*/ __PVT__u_id_stage__DOT__funct3;
    CData/*6:0*/ __PVT__u_id_stage__DOT__funct7;
    CData/*0:0*/ u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0;
    CData/*3:0*/ __PVT__u_mem_stage__DOT__mem_wmask;
    CData/*7:0*/ __PVT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected;
    CData/*7:0*/ __VdlyVal__u_mem_stage__DOT__mem__v0;
    CData/*0:0*/ __VdlySet__u_mem_stage__DOT__mem__v0;
    CData/*7:0*/ __VdlyVal__u_mem_stage__DOT__mem__v1;
    CData/*0:0*/ __VdlySet__u_mem_stage__DOT__mem__v1;
    CData/*7:0*/ __VdlyVal__u_mem_stage__DOT__mem__v2;
    CData/*0:0*/ __VdlySet__u_mem_stage__DOT__mem__v2;
    CData/*7:0*/ __VdlyVal__u_mem_stage__DOT__mem__v3;
    CData/*0:0*/ __VdlySet__u_mem_stage__DOT__mem__v3;
    SData/*15:0*/ __PVT__u_mem_stage__DOT__lsu_pic__DOT__half_selected;
    SData/*15:0*/ __VdlyDim0__u_mem_stage__DOT__mem__v0;
    SData/*15:0*/ __VdlyDim0__u_mem_stage__DOT__mem__v1;
    SData/*15:0*/ __VdlyDim0__u_mem_stage__DOT__mem__v2;
    SData/*15:0*/ __VdlyDim0__u_mem_stage__DOT__mem__v3;
    VL_OUT(debug_pc,31,0);
    IData/*31:0*/ __PVT__rs1_rdata;
    IData/*31:0*/ __PVT__rs2_rdata;
    IData/*31:0*/ __PVT__jump_jalr;
    IData/*31:0*/ __PVT__imm_jal;
    IData/*31:0*/ __PVT__pc;
    IData/*31:0*/ __PVT__mem_rdata_raw;
    IData/*31:0*/ __PVT__u_if_stage__DOT__pc_next;
    IData/*31:0*/ __PVT__u_ex_stage__DOT__src1;
    IData/*31:0*/ __PVT__u_ex_stage__DOT__src2;
    IData/*31:0*/ __PVT__u_mem_stage__DOT__mem_wdata;
    IData/*31:0*/ __VdfgRegularize_h3877affd_0_0;
    VlUnpacked<IData/*31:0*/, 65536> __PVT__u_if_stage__DOT__imem;
    VlUnpacked<IData/*31:0*/, 65536> __PVT__u_mem_stage__DOT__mem;

    // INTERNAL VARIABLES
    Vcore_top__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vcore_top_core_top(Vcore_top__Syms* symsp, const char* v__name);
    ~Vcore_top_core_top();
    VL_UNCOPYABLE(Vcore_top_core_top);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
