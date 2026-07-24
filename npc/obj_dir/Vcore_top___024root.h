// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vcore_top.h for the primary calling header

#ifndef VERILATED_VCORE_TOP___024ROOT_H_
#define VERILATED_VCORE_TOP___024ROOT_H_  // guard

#include "verilated.h"
class Vcore_top___024unit;


class Vcore_top__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vcore_top___024root final : public VerilatedModule {
  public:
    // CELLS
    Vcore_top___024unit* __PVT____024unit;

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_IN8(clk,0,0);
        VL_IN8(rst,0,0);
        VL_IN8(interrupt_valid,0,0);
        VL_OUT8(halt,0,0);
        CData/*0:0*/ core_top__DOT__is_ebreak_eff;
        CData/*0:0*/ core_top__DOT__if_id_valid;
        CData/*0:0*/ core_top__DOT__id_ex_valid;
        CData/*4:0*/ core_top__DOT__id_ex_rs1_addr;
        CData/*4:0*/ core_top__DOT__id_ex_rs2_addr;
        CData/*4:0*/ core_top__DOT__id_ex_rd_addr;
        CData/*7:0*/ core_top__DOT__id_ex_ex_ctrl;
        CData/*4:0*/ core_top__DOT__id_ex_mem_ctrl;
        CData/*2:0*/ core_top__DOT__id_ex_wb_ctrl;
        CData/*0:0*/ core_top__DOT__ex_mem_valid;
        CData/*4:0*/ core_top__DOT__ex_mem_rd_addr;
        CData/*4:0*/ core_top__DOT__ex_mem_mem_ctrl;
        CData/*2:0*/ core_top__DOT__ex_mem_wb_ctrl;
        CData/*0:0*/ core_top__DOT__mem_wb_valid;
        CData/*4:0*/ core_top__DOT__mem_wb_rd_addr;
        CData/*2:0*/ core_top__DOT__mem_wb_wb_ctrl;
        CData/*0:0*/ core_top__DOT__u_if_stage__DOT__trap_enter;
        CData/*4:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op;
        CData/*0:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_a;
        CData/*1:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b;
        CData/*2:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type;
        CData/*0:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_read;
        CData/*0:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_write;
        CData/*2:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type;
        CData/*0:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write;
        CData/*1:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src;
        CData/*0:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_read;
        CData/*0:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_write;
        CData/*0:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_imm;
        CData/*1:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_op;
        CData/*1:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel;
        CData/*0:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_enter_int;
        CData/*0:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__is_ebreak_int;
        CData/*0:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int;
        CData/*0:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jal_int;
        CData/*0:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int;
        CData/*0:0*/ core_top__DOT__u_mem_stage__DOT__mem_read;
        CData/*0:0*/ core_top__DOT__u_mem_stage__DOT__mem_write;
        CData/*7:0*/ core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected;
        CData/*0:0*/ core_top__DOT__u_csr__DOT__csr_write;
        CData/*0:0*/ core_top__DOT__u_csr__DOT__trap_enter;
        CData/*0:0*/ core_top__DOT__u_csr__DOT__mret;
        CData/*0:0*/ core_top__DOT__u_csr__DOT____VdfgRegularize_h8136f0d8_0_0;
        CData/*0:0*/ __VstlFirstIteration;
        CData/*0:0*/ __VicoFirstIteration;
        CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
        CData/*0:0*/ __VactContinue;
        SData/*15:0*/ core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected;
        VL_IN(interrupt_cause,31,0);
        VL_OUT(instr,31,0);
        VL_OUT(halt_pc,31,0);
        VL_OUT(halt_ret,31,0);
        IData/*31:0*/ core_top__DOT__pc;
        IData/*31:0*/ core_top__DOT__rs1_rdata;
        IData/*31:0*/ core_top__DOT__rs2_rdata;
        IData/*31:0*/ core_top__DOT__alu_result;
        IData/*31:0*/ core_top__DOT__imm_out;
        IData/*31:0*/ core_top__DOT__mem_addr;
        IData/*31:0*/ core_top__DOT__mem_wdata_raw;
        IData/*31:0*/ core_top__DOT__csr_rdata;
    };
    struct {
        IData/*31:0*/ core_top__DOT__trap_pc;
        IData/*31:0*/ core_top__DOT__if_id_pc;
        IData/*31:0*/ core_top__DOT__if_id_pc_plus4;
        IData/*31:0*/ core_top__DOT__if_id_inst;
        IData/*31:0*/ core_top__DOT__id_ex_pc;
        IData/*31:0*/ core_top__DOT__id_ex_pc_plus4;
        IData/*31:0*/ core_top__DOT__id_ex_inst;
        IData/*31:0*/ core_top__DOT__id_ex_rs1_rdata;
        IData/*31:0*/ core_top__DOT__id_ex_rs2_rdata;
        IData/*31:0*/ core_top__DOT__id_ex_imm_out;
        IData/*31:0*/ core_top__DOT__ex_mem_pc;
        IData/*31:0*/ core_top__DOT__ex_mem_pc_plus4;
        IData/*31:0*/ core_top__DOT__ex_mem_inst;
        IData/*31:0*/ core_top__DOT__ex_mem_alu_result;
        IData/*31:0*/ core_top__DOT__ex_mem_rs2_rdata;
        IData/*31:0*/ core_top__DOT__ex_mem_csr_rdata;
        IData/*31:0*/ core_top__DOT__ex_mem_csr_wdata;
        IData/*31:0*/ core_top__DOT__mem_wb_pc;
        IData/*31:0*/ core_top__DOT__mem_wb_pc_plus4;
        IData/*31:0*/ core_top__DOT__mem_wb_inst;
        IData/*31:0*/ core_top__DOT__mem_wb_alu_result;
        IData/*31:0*/ core_top__DOT__mem_wb_mem_rdata;
        IData/*31:0*/ core_top__DOT__mem_wb_csr_rdata;
        IData/*31:0*/ core_top__DOT__mem_wb_csr_wdata;
        IData/*31:0*/ core_top__DOT__u_if_stage__DOT__pc_next;
        IData/*31:0*/ core_top__DOT__u_id_stage__DOT__target_sum;
        IData/*31:0*/ core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_code_int;
        IData/*31:0*/ core_top__DOT__u_ex_stage__DOT__src1;
        IData/*31:0*/ core_top__DOT__u_ex_stage__DOT__src2;
        IData/*31:0*/ core_top__DOT__u_ex_stage__DOT__csr_src;
        IData/*31:0*/ core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__abs_dividend;
        IData/*31:0*/ core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__abs_divisor;
        IData/*31:0*/ core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__quotient;
        IData/*31:0*/ core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__abs_dividend;
        IData/*31:0*/ core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__abs_divisor;
        IData/*31:0*/ core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__remainder;
        IData/*31:0*/ core_top__DOT__u_mem_stage__DOT__mem_rdata_raw;
        IData/*31:0*/ core_top__DOT__u_csr__DOT__trap_code;
        IData/*31:0*/ core_top__DOT__u_csr__DOT__csr_mstatus;
        IData/*31:0*/ core_top__DOT__u_csr__DOT__csr_mtvec;
        IData/*31:0*/ core_top__DOT__u_csr__DOT__csr_mepc;
        IData/*31:0*/ core_top__DOT__u_csr__DOT__csr_mcause;
        IData/*31:0*/ core_top__DOT__u_csr__DOT__csr_mip;
        IData/*31:0*/ core_top__DOT__u_csr__DOT__csr_mie;
        IData/*31:0*/ core_top__DOT__u_csr__DOT__csr_mtval;
        IData/*31:0*/ core_top__DOT__u_csr__DOT__csr_mscratch;
        IData/*31:0*/ __Vfunc_dpi_mem_read__0__Vfuncout;
        IData/*31:0*/ __Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_rem32__1__Vfuncout;
        IData/*31:0*/ __Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_rem32__1__dividend;
        IData/*31:0*/ __Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_rem32__1__divisor;
        IData/*31:0*/ __Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__Vfuncout;
        IData/*31:0*/ __Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__dividend;
        IData/*31:0*/ __Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__divisor;
        IData/*31:0*/ __Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_div32__3__Vfuncout;
        IData/*31:0*/ __Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_div32__3__dividend;
        IData/*31:0*/ __Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_div32__3__divisor;
        IData/*31:0*/ __Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__Vfuncout;
        IData/*31:0*/ __Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__dividend;
        IData/*31:0*/ __Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__divisor;
        IData/*31:0*/ __Vfunc_dpi_mem_read__5__Vfuncout;
        IData/*31:0*/ __VdfgRegularize_h495687df_0_6;
        IData/*31:0*/ __VdfgRegularize_h495687df_0_7;
        IData/*31:0*/ __VdfgRegularize_h495687df_0_8;
        IData/*31:0*/ __VactIterCount;
    };
    struct {
        QData/*48:0*/ core_top__DOT__id_ex_csr_ctrl;
        QData/*34:0*/ core_top__DOT__id_ex_sys_ctrl;
        QData/*48:0*/ core_top__DOT__ex_mem_csr_ctrl;
        QData/*34:0*/ core_top__DOT__ex_mem_sys_ctrl;
        QData/*48:0*/ core_top__DOT__mem_wb_csr_ctrl;
        QData/*34:0*/ core_top__DOT__mem_wb_sys_ctrl;
        QData/*63:0*/ core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__src1_s64;
        QData/*63:0*/ core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__mul_uu;
        QData/*63:0*/ core_top__DOT__u_csr__DOT__mcycle_64;
        VlUnpacked<IData/*31:0*/, 32> core_top__DOT__u_regfile__DOT__rf;
        VlUnpacked<CData/*0:0*/, 3> __Vm_traceActivity;
    };
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
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
