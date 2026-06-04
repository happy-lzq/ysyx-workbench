// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcore_top.h for the primary calling header

#include "Vcore_top__pch.h"
#include "Vcore_top__Syms.h"
#include "Vcore_top___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcore_top___024root___dump_triggers__ico(Vcore_top___024root* vlSelf);
#endif  // VL_DEBUG

void Vcore_top___024root___eval_triggers__ico(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_triggers__ico\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VicoTriggered.setBit(0U, (IData)(vlSelfRef.__VicoFirstIteration));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vcore_top___024root___dump_triggers__ico(vlSelf);
    }
#endif
}

void Vcore_top___024unit____Vdpiimwrap_dpi_mem_read_TOP____024unit(IData/*31:0*/ addr, IData/*31:0*/ is_load, IData/*31:0*/ &dpi_mem_read__Vfuncrtn);

VL_INLINE_OPT void Vcore_top___024root___ico_sequent__TOP__0(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___ico_sequent__TOP__0\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.core_top__DOT__mem_write_eff = ((~ (IData)(vlSelfRef.interrupt_valid)) 
                                              & (IData)(vlSelfRef.core_top__DOT__mem_write));
    vlSelfRef.core_top__DOT__csr_write_eff = ((~ (IData)(vlSelfRef.interrupt_valid)) 
                                              & (IData)(vlSelfRef.core_top__DOT__csr_write));
    vlSelfRef.core_top__DOT__is_ebreak_eff = ((~ (IData)(vlSelfRef.interrupt_valid)) 
                                              & (IData)(vlSelfRef.core_top__DOT__is_ebreak));
    vlSelfRef.core_top__DOT__trap_code_eff = ((IData)(vlSelfRef.interrupt_valid)
                                               ? vlSelfRef.interrupt_cause
                                               : vlSelfRef.core_top__DOT__trap_code);
    vlSelfRef.core_top__DOT__mret_eff = ((~ (IData)(vlSelfRef.interrupt_valid)) 
                                         & (IData)(vlSelfRef.core_top__DOT__mret));
    vlSelfRef.core_top__DOT__trap_enter_eff = ((IData)(vlSelfRef.core_top__DOT__trap_enter) 
                                               | (IData)(vlSelfRef.interrupt_valid));
    vlSelfRef.core_top__DOT__mem_read_eff = ((~ (IData)(vlSelfRef.interrupt_valid)) 
                                             & (IData)(vlSelfRef.core_top__DOT__mem_read));
    vlSelfRef.core_top__DOT__u_if_stage__DOT__pc_next 
        = (((IData)(vlSelfRef.core_top__DOT__mret_eff) 
            | (IData)(vlSelfRef.core_top__DOT__trap_enter_eff))
            ? ((IData)(vlSelfRef.core_top__DOT__trap_enter_eff)
                ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                : ((IData)(vlSelfRef.core_top__DOT__mret_eff)
                    ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                    : 0U)) : ((0U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                               ? ((IData)(4U) + vlSelfRef.core_top__DOT__pc)
                               : ((1U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                   ? vlSelfRef.__VdfgRegularize_h3c5bdd75_0_0
                                   : ((2U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                       ? (0xfffffffeU 
                                          & vlSelfRef.core_top__DOT__jump_jalr)
                                       : ((3U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                           ? (((4U 
                                                & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                ? (
                                                   (~ 
                                                    ((IData)(vlSelfRef.core_top__DOT__br_type) 
                                                     >> 1U)) 
                                                   & ((1U 
                                                       & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                       ? 
                                                      (vlSelfRef.core_top__DOT__rs1_rdata 
                                                       >= vlSelfRef.core_top__DOT__rs2_rdata)
                                                       : 
                                                      (vlSelfRef.core_top__DOT__rs1_rdata 
                                                       < vlSelfRef.core_top__DOT__rs2_rdata)))
                                                : (
                                                   (2U 
                                                    & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                     ? 
                                                    VL_GTES_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.core_top__DOT__rs2_rdata)
                                                     : 
                                                    VL_LTS_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.core_top__DOT__rs2_rdata))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                     ? 
                                                    (vlSelfRef.core_top__DOT__rs1_rdata 
                                                     != vlSelfRef.core_top__DOT__rs2_rdata)
                                                     : 
                                                    (vlSelfRef.core_top__DOT__rs1_rdata 
                                                     == vlSelfRef.core_top__DOT__rs2_rdata))))
                                               ? vlSelfRef.__VdfgRegularize_h3c5bdd75_0_0
                                               : ((IData)(4U) 
                                                  + vlSelfRef.core_top__DOT__pc))
                                           : ((IData)(4U) 
                                              + vlSelfRef.core_top__DOT__pc))))));
    Vcore_top___024unit____Vdpiimwrap_dpi_mem_read_TOP____024unit(vlSelfRef.core_top__DOT__jump_jalr, (IData)(vlSelfRef.core_top__DOT__mem_read_eff), vlSelfRef.__Vfunc_dpi_mem_read__1__Vfuncout);
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
        = vlSelfRef.__Vfunc_dpi_mem_read__1__Vfuncout;
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected 
        = (0xffU & ((0U == (3U & vlSelfRef.core_top__DOT__jump_jalr))
                     ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw
                     : ((1U == (3U & vlSelfRef.core_top__DOT__jump_jalr))
                         ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                            >> 8U) : ((2U == (3U & vlSelfRef.core_top__DOT__jump_jalr))
                                       ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 0x10U)
                                       : (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 0x18U)))));
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected 
        = (0xffffU & ((2U & vlSelfRef.core_top__DOT__jump_jalr)
                       ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                          >> 0x10U) : vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw));
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcore_top___024root___dump_triggers__act(Vcore_top___024root* vlSelf);
#endif  // VL_DEBUG

void Vcore_top___024root___eval_triggers__act(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_triggers__act\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered.setBit(0U, ((IData)(vlSelfRef.clk) 
                                          & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__clk__0))));
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vcore_top___024root___dump_triggers__act(vlSelf);
    }
#endif
}

void Vcore_top___024unit____Vdpiimwrap_dpi_mem_write_TOP____024unit(IData/*31:0*/ addr, IData/*31:0*/ data, IData/*31:0*/ wmask);

VL_INLINE_OPT void Vcore_top___024root___nba_sequent__TOP__0(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___nba_sequent__TOP__0\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ __Vdly__core_top__DOT__u_csr__DOT__csr_mip;
    __Vdly__core_top__DOT__u_csr__DOT__csr_mip = 0;
    IData/*31:0*/ __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus;
    __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus = 0;
    QData/*63:0*/ __Vdly__core_top__DOT__u_csr__DOT__mcycle_64;
    __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 = 0;
    IData/*31:0*/ __VdlyVal__core_top__DOT__u_regfile__DOT__rf__v0;
    __VdlyVal__core_top__DOT__u_regfile__DOT__rf__v0 = 0;
    CData/*4:0*/ __VdlyDim0__core_top__DOT__u_regfile__DOT__rf__v0;
    __VdlyDim0__core_top__DOT__u_regfile__DOT__rf__v0 = 0;
    CData/*0:0*/ __VdlySet__core_top__DOT__u_regfile__DOT__rf__v0;
    __VdlySet__core_top__DOT__u_regfile__DOT__rf__v0 = 0;
    // Body
    if (vlSelfRef.core_top__DOT__mem_write_eff) {
        Vcore_top___024unit____Vdpiimwrap_dpi_mem_write_TOP____024unit(vlSelfRef.core_top__DOT__jump_jalr, 
                                                                       ((IData)(vlSelfRef.core_top__DOT__mem_write_eff)
                                                                         ? 
                                                                        ((5U 
                                                                          == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                                                          ? vlSelfRef.core_top__DOT__rs2_rdata
                                                                          : 
                                                                         ((6U 
                                                                           == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                                                           ? 
                                                                          ((vlSelfRef.core_top__DOT__rs2_rdata 
                                                                            << 0x10U) 
                                                                           | (0xffffU 
                                                                              & vlSelfRef.core_top__DOT__rs2_rdata))
                                                                           : 
                                                                          ((vlSelfRef.core_top__DOT__rs2_rdata 
                                                                            << 0x18U) 
                                                                           | ((0xff0000U 
                                                                               & (vlSelfRef.core_top__DOT__rs2_rdata 
                                                                                << 0x10U)) 
                                                                              | ((0xff00U 
                                                                                & (vlSelfRef.core_top__DOT__rs2_rdata 
                                                                                << 8U)) 
                                                                                | (0xffU 
                                                                                & vlSelfRef.core_top__DOT__rs2_rdata))))))
                                                                         : 0U), 
                                                                       ((IData)(vlSelfRef.core_top__DOT__mem_write_eff)
                                                                         ? 
                                                                        ((5U 
                                                                          == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                                                          ? 0xfU
                                                                          : 
                                                                         (0xfU 
                                                                          & ((6U 
                                                                              == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                                                              ? 
                                                                             ((2U 
                                                                               & vlSelfRef.core_top__DOT__jump_jalr)
                                                                               ? 0xcU
                                                                               : 3U)
                                                                              : 
                                                                             ((IData)(1U) 
                                                                              << 
                                                                              (3U 
                                                                               & vlSelfRef.core_top__DOT__jump_jalr)))))
                                                                         : 0U));
    }
    __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus 
        = vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus;
    __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 = vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64;
    __Vdly__core_top__DOT__u_csr__DOT__csr_mip = vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip;
    __VdlySet__core_top__DOT__u_regfile__DOT__rf__v0 = 0U;
    if ((((~ (IData)(vlSelfRef.interrupt_valid)) & (IData)(vlSelfRef.core_top__DOT__reg_write)) 
         & (0U != (0x1fU & (vlSelfRef.instr >> 7U))))) {
        __VdlyVal__core_top__DOT__u_regfile__DOT__rf__v0 
            = ((0U == (IData)(vlSelfRef.core_top__DOT__reg_wdata_src))
                ? vlSelfRef.core_top__DOT__jump_jalr
                : ((1U == (IData)(vlSelfRef.core_top__DOT__reg_wdata_src))
                    ? ((IData)(vlSelfRef.core_top__DOT__mem_read_eff)
                        ? ((0U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                            ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw
                            : ((1U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                ? (((- (IData)((1U 
                                                & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                                   >> 0xfU)))) 
                                    << 0x10U) | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))
                                : ((2U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                    ? (((- (IData)(
                                                   (1U 
                                                    & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected) 
                                                       >> 7U)))) 
                                        << 8U) | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected))
                                    : ((3U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                        ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected)
                                        : ((4U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                            ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected)
                                            : 0U)))))
                        : 0U) : ((2U == (IData)(vlSelfRef.core_top__DOT__reg_wdata_src))
                                  ? ((IData)(4U) + vlSelfRef.core_top__DOT__pc)
                                  : vlSelfRef.core_top__DOT__csr_rdata)));
        __VdlyDim0__core_top__DOT__u_regfile__DOT__rf__v0 
            = (0x1fU & (vlSelfRef.instr >> 7U));
        __VdlySet__core_top__DOT__u_regfile__DOT__rf__v0 = 1U;
    }
    if (vlSelfRef.rst) {
        __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus = 0x1800U;
        __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 = 0ULL;
        __Vdly__core_top__DOT__u_csr__DOT__csr_mip = 0U;
        vlSelfRef.halt = 0U;
        vlSelfRef.halt_ret = 0U;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mscratch = 0U;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtval = 0U;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mie = 0U;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause = 0U;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec = 0x80000000U;
        vlSelfRef.halt_pc = 0x80000000U;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc = 0U;
        vlSelfRef.core_top__DOT__pc = 0x80000000U;
    } else {
        if (vlSelfRef.core_top__DOT__trap_enter_eff) {
            __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus 
                = (0x1800U | ((0xffffe000U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus) 
                              | (((0x700U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus) 
                                  | (0x80U & (vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus 
                                              << 4U))) 
                                 | (0x77U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus))));
            if ((0x80000007U == vlSelfRef.core_top__DOT__trap_code_eff)) {
                __Vdly__core_top__DOT__u_csr__DOT__csr_mip 
                    = (0xffffff7fU & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip);
            } else if ((0x8000000bU == vlSelfRef.core_top__DOT__trap_code_eff)) {
                __Vdly__core_top__DOT__u_csr__DOT__csr_mip 
                    = (0xfffff7ffU & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip);
            }
            vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause 
                = vlSelfRef.core_top__DOT__trap_code_eff;
            vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc 
                = vlSelfRef.core_top__DOT__pc;
        } else {
            if (vlSelfRef.core_top__DOT__mret_eff) {
                __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus 
                    = (0x80U | ((0xffffe000U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus) 
                                | ((0x700U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus) 
                                   | ((0x70U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus) 
                                      | ((8U & (vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus 
                                                >> 4U)) 
                                         | (7U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus))))));
            } else if (vlSelfRef.core_top__DOT__csr_write_eff) {
                if (((((((((0x300U == (vlSelfRef.instr 
                                       >> 0x14U)) | 
                           (0x305U == (vlSelfRef.instr 
                                       >> 0x14U))) 
                          | (0x341U == (vlSelfRef.instr 
                                        >> 0x14U))) 
                         | (0x342U == (vlSelfRef.instr 
                                       >> 0x14U))) 
                        | (0x344U == (vlSelfRef.instr 
                                      >> 0x14U))) | 
                       (0x304U == (vlSelfRef.instr 
                                   >> 0x14U))) | (0x343U 
                                                  == 
                                                  (vlSelfRef.instr 
                                                   >> 0x14U))) 
                     | (0x340U == (vlSelfRef.instr 
                                   >> 0x14U)))) {
                    if ((0x300U == (vlSelfRef.instr 
                                    >> 0x14U))) {
                        __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus 
                            = vlSelfRef.core_top__DOT__csr_wdata;
                    }
                }
            }
            if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__mret_eff)))) {
                if (vlSelfRef.core_top__DOT__csr_write_eff) {
                    if (((((((((0x300U == (vlSelfRef.instr 
                                           >> 0x14U)) 
                               | (0x305U == (vlSelfRef.instr 
                                             >> 0x14U))) 
                              | (0x341U == (vlSelfRef.instr 
                                            >> 0x14U))) 
                             | (0x342U == (vlSelfRef.instr 
                                           >> 0x14U))) 
                            | (0x344U == (vlSelfRef.instr 
                                          >> 0x14U))) 
                           | (0x304U == (vlSelfRef.instr 
                                         >> 0x14U))) 
                          | (0x343U == (vlSelfRef.instr 
                                        >> 0x14U))) 
                         | (0x340U == (vlSelfRef.instr 
                                       >> 0x14U)))) {
                        if ((0x300U != (vlSelfRef.instr 
                                        >> 0x14U))) {
                            if ((0x305U != (vlSelfRef.instr 
                                            >> 0x14U))) {
                                if ((0x341U != (vlSelfRef.instr 
                                                >> 0x14U))) {
                                    if ((0x342U != 
                                         (vlSelfRef.instr 
                                          >> 0x14U))) {
                                        if ((0x344U 
                                             == (vlSelfRef.instr 
                                                 >> 0x14U))) {
                                            __Vdly__core_top__DOT__u_csr__DOT__csr_mip 
                                                = vlSelfRef.core_top__DOT__csr_wdata;
                                        }
                                    }
                                    if ((0x342U == 
                                         (vlSelfRef.instr 
                                          >> 0x14U))) {
                                        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause 
                                            = vlSelfRef.core_top__DOT__csr_wdata;
                                    }
                                }
                                if ((0x341U == (vlSelfRef.instr 
                                                >> 0x14U))) {
                                    vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc 
                                        = vlSelfRef.core_top__DOT__csr_wdata;
                                }
                            }
                        }
                    }
                }
            }
        }
        __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 
            = (1ULL + vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64);
        if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__trap_enter_eff)))) {
            if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__mret_eff)))) {
                if (vlSelfRef.core_top__DOT__csr_write_eff) {
                    if ((1U & (~ ((((((((0x300U == 
                                         (vlSelfRef.instr 
                                          >> 0x14U)) 
                                        | (0x305U == 
                                           (vlSelfRef.instr 
                                            >> 0x14U))) 
                                       | (0x341U == 
                                          (vlSelfRef.instr 
                                           >> 0x14U))) 
                                      | (0x342U == 
                                         (vlSelfRef.instr 
                                          >> 0x14U))) 
                                     | (0x344U == (vlSelfRef.instr 
                                                   >> 0x14U))) 
                                    | (0x304U == (vlSelfRef.instr 
                                                  >> 0x14U))) 
                                   | (0x343U == (vlSelfRef.instr 
                                                 >> 0x14U))) 
                                  | (0x340U == (vlSelfRef.instr 
                                                >> 0x14U)))))) {
                        if ((0xb00U == (vlSelfRef.instr 
                                        >> 0x14U))) {
                            __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 
                                = ((0xffffffff00000000ULL 
                                    & __Vdly__core_top__DOT__u_csr__DOT__mcycle_64) 
                                   | (IData)((IData)(vlSelfRef.core_top__DOT__csr_wdata)));
                        } else if ((0xb80U == (vlSelfRef.instr 
                                               >> 0x14U))) {
                            __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 
                                = ((0xffffffffULL & __Vdly__core_top__DOT__u_csr__DOT__mcycle_64) 
                                   | ((QData)((IData)(vlSelfRef.core_top__DOT__csr_wdata)) 
                                      << 0x20U));
                        }
                    }
                    if (((((((((0x300U == (vlSelfRef.instr 
                                           >> 0x14U)) 
                               | (0x305U == (vlSelfRef.instr 
                                             >> 0x14U))) 
                              | (0x341U == (vlSelfRef.instr 
                                            >> 0x14U))) 
                             | (0x342U == (vlSelfRef.instr 
                                           >> 0x14U))) 
                            | (0x344U == (vlSelfRef.instr 
                                          >> 0x14U))) 
                           | (0x304U == (vlSelfRef.instr 
                                         >> 0x14U))) 
                          | (0x343U == (vlSelfRef.instr 
                                        >> 0x14U))) 
                         | (0x340U == (vlSelfRef.instr 
                                       >> 0x14U)))) {
                        if ((0x300U != (vlSelfRef.instr 
                                        >> 0x14U))) {
                            if ((0x305U != (vlSelfRef.instr 
                                            >> 0x14U))) {
                                if ((0x341U != (vlSelfRef.instr 
                                                >> 0x14U))) {
                                    if ((0x342U != 
                                         (vlSelfRef.instr 
                                          >> 0x14U))) {
                                        if ((0x344U 
                                             != (vlSelfRef.instr 
                                                 >> 0x14U))) {
                                            if ((0x304U 
                                                 != 
                                                 (vlSelfRef.instr 
                                                  >> 0x14U))) {
                                                if (
                                                    (0x343U 
                                                     != 
                                                     (vlSelfRef.instr 
                                                      >> 0x14U))) {
                                                    vlSelfRef.core_top__DOT__u_csr__DOT__csr_mscratch 
                                                        = vlSelfRef.core_top__DOT__csr_wdata;
                                                }
                                                if (
                                                    (0x343U 
                                                     == 
                                                     (vlSelfRef.instr 
                                                      >> 0x14U))) {
                                                    vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtval 
                                                        = vlSelfRef.core_top__DOT__csr_wdata;
                                                }
                                            }
                                            if ((0x304U 
                                                 == 
                                                 (vlSelfRef.instr 
                                                  >> 0x14U))) {
                                                vlSelfRef.core_top__DOT__u_csr__DOT__csr_mie 
                                                    = vlSelfRef.core_top__DOT__csr_wdata;
                                            }
                                        }
                                    }
                                }
                            }
                            if ((0x305U == (vlSelfRef.instr 
                                            >> 0x14U))) {
                                vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec 
                                    = vlSelfRef.core_top__DOT__csr_wdata;
                            }
                        }
                    }
                }
            }
        }
        if (vlSelfRef.core_top__DOT__is_ebreak_eff) {
            vlSelfRef.halt = 1U;
            vlSelfRef.halt_ret = vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                [0xaU];
            vlSelfRef.halt_pc = vlSelfRef.core_top__DOT__pc;
        }
        vlSelfRef.core_top__DOT__pc = vlSelfRef.core_top__DOT__u_if_stage__DOT__pc_next;
    }
    vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus 
        = __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus;
    vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64 
        = __Vdly__core_top__DOT__u_csr__DOT__mcycle_64;
    vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip = __Vdly__core_top__DOT__u_csr__DOT__csr_mip;
    if (__VdlySet__core_top__DOT__u_regfile__DOT__rf__v0) {
        vlSelfRef.core_top__DOT__u_regfile__DOT__rf[__VdlyDim0__core_top__DOT__u_regfile__DOT__rf__v0] 
            = __VdlyVal__core_top__DOT__u_regfile__DOT__rf__v0;
    }
    Vcore_top___024unit____Vdpiimwrap_dpi_mem_read_TOP____024unit(vlSelfRef.core_top__DOT__pc, 0U, vlSelfRef.__Vfunc_dpi_mem_read__0__Vfuncout);
    vlSelfRef.instr = vlSelfRef.__Vfunc_dpi_mem_read__0__Vfuncout;
    vlSelfRef.core_top__DOT__reg_wdata_src = 0U;
    vlSelfRef.core_top__DOT__reg_write = 0U;
    vlSelfRef.core_top__DOT__lsu_type = 0U;
    vlSelfRef.core_top__DOT__mem_write = 0U;
    vlSelfRef.core_top__DOT__csr_write = 0U;
    vlSelfRef.core_top__DOT__is_ebreak = 0U;
    vlSelfRef.core_top__DOT__csr_op = 0U;
    vlSelfRef.core_top__DOT__trap_code = 0U;
    vlSelfRef.core_top__DOT__csr_imm = 0U;
    vlSelfRef.core_top__DOT__pc_sel = 0U;
    vlSelfRef.core_top__DOT__br_type = 0U;
    vlSelfRef.core_top__DOT__trap_enter = 0U;
    vlSelfRef.core_top__DOT__mret = 0U;
    vlSelfRef.core_top__DOT__mem_read = 0U;
    vlSelfRef.core_top__DOT__csr_read = 0U;
    vlSelfRef.core_top__DOT__alu_op = 0U;
    vlSelfRef.core_top__DOT__alu_src_b = 0U;
    vlSelfRef.core_top__DOT__alu_src_a = 0U;
    if ((1U & (~ (vlSelfRef.instr >> 6U)))) {
        if ((0x20U & vlSelfRef.instr)) {
            if ((1U & (~ (vlSelfRef.instr >> 4U)))) {
                if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                    if ((1U & (~ (vlSelfRef.instr >> 2U)))) {
                        if ((2U & vlSelfRef.instr)) {
                            if ((1U & vlSelfRef.instr)) {
                                if ((2U == (7U & (vlSelfRef.instr 
                                                  >> 0xcU)))) {
                                    vlSelfRef.core_top__DOT__lsu_type = 5U;
                                } else if ((1U == (7U 
                                                   & (vlSelfRef.instr 
                                                      >> 0xcU)))) {
                                    vlSelfRef.core_top__DOT__lsu_type = 6U;
                                } else if ((0U == (7U 
                                                   & (vlSelfRef.instr 
                                                      >> 0xcU)))) {
                                    vlSelfRef.core_top__DOT__lsu_type = 7U;
                                }
                                vlSelfRef.core_top__DOT__mem_write = 1U;
                            }
                        }
                    }
                }
            }
        } else if ((1U & (~ (vlSelfRef.instr >> 4U)))) {
            if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                if ((1U & (~ (vlSelfRef.instr >> 2U)))) {
                    if ((2U & vlSelfRef.instr)) {
                        if ((1U & vlSelfRef.instr)) {
                            if ((0x4000U & vlSelfRef.instr)) {
                                if ((1U & (~ (vlSelfRef.instr 
                                              >> 0xdU)))) {
                                    vlSelfRef.core_top__DOT__lsu_type 
                                        = ((0x1000U 
                                            & vlSelfRef.instr)
                                            ? 3U : 4U);
                                }
                            } else if ((0x2000U & vlSelfRef.instr)) {
                                if ((1U & (~ (vlSelfRef.instr 
                                              >> 0xcU)))) {
                                    vlSelfRef.core_top__DOT__lsu_type = 0U;
                                }
                            } else {
                                vlSelfRef.core_top__DOT__lsu_type 
                                    = ((0x1000U & vlSelfRef.instr)
                                        ? 1U : 2U);
                            }
                        }
                    }
                }
            }
        }
        if ((1U & (~ (vlSelfRef.instr >> 5U)))) {
            if ((1U & (~ (vlSelfRef.instr >> 4U)))) {
                if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                    if ((1U & (~ (vlSelfRef.instr >> 2U)))) {
                        if ((2U & vlSelfRef.instr)) {
                            if ((1U & vlSelfRef.instr)) {
                                vlSelfRef.core_top__DOT__mem_read = 1U;
                            }
                        }
                    }
                }
            }
            if ((0x10U & vlSelfRef.instr)) {
                if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                    if ((4U & vlSelfRef.instr)) {
                        if ((2U & vlSelfRef.instr)) {
                            if ((1U & vlSelfRef.instr)) {
                                vlSelfRef.core_top__DOT__alu_src_a = 1U;
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.core_top__DOT__rs2_rdata = ((0U == (0x1fU 
                                                  & (vlSelfRef.instr 
                                                     >> 0x14U)))
                                           ? 0U : vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                                          [(0x1fU & 
                                            (vlSelfRef.instr 
                                             >> 0x14U))]);
    if ((2U & vlSelfRef.instr)) {
        if ((1U & vlSelfRef.instr)) {
            vlSelfRef.__VdfgRegularize_h495687df_0_8 
                = (0xfffff000U & vlSelfRef.instr);
            vlSelfRef.__VdfgRegularize_h495687df_0_6 
                = (((- (IData)((vlSelfRef.instr >> 0x1fU))) 
                    << 0xcU) | (vlSelfRef.instr >> 0x14U));
        } else {
            vlSelfRef.__VdfgRegularize_h495687df_0_8 = 0U;
            vlSelfRef.__VdfgRegularize_h495687df_0_6 = 0U;
        }
    } else {
        vlSelfRef.__VdfgRegularize_h495687df_0_8 = 0U;
        vlSelfRef.__VdfgRegularize_h495687df_0_6 = 0U;
    }
    vlSelfRef.core_top__DOT__rs1_rdata = ((0U == (0x1fU 
                                                  & (vlSelfRef.instr 
                                                     >> 0xfU)))
                                           ? 0U : vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                                          [(0x1fU & 
                                            (vlSelfRef.instr 
                                             >> 0xfU))]);
    vlSelfRef.core_top__DOT__mem_write_eff = ((~ (IData)(vlSelfRef.interrupt_valid)) 
                                              & (IData)(vlSelfRef.core_top__DOT__mem_write));
    if ((0x40U & vlSelfRef.instr)) {
        if ((0x20U & vlSelfRef.instr)) {
            if ((0x10U & vlSelfRef.instr)) {
                if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                    if ((1U & (~ (vlSelfRef.instr >> 2U)))) {
                        if ((2U & vlSelfRef.instr)) {
                            if ((1U & vlSelfRef.instr)) {
                                if ((0x4000U & vlSelfRef.instr)) {
                                    if ((0x2000U & vlSelfRef.instr)) {
                                        vlSelfRef.core_top__DOT__reg_wdata_src = 3U;
                                        vlSelfRef.core_top__DOT__reg_write = 1U;
                                        vlSelfRef.core_top__DOT__csr_write 
                                            = ((0x1000U 
                                                & vlSelfRef.instr)
                                                ? (0U 
                                                   != 
                                                   (0x1fU 
                                                    & (vlSelfRef.instr 
                                                       >> 0xfU)))
                                                : (0U 
                                                   != 
                                                   (0x1fU 
                                                    & (vlSelfRef.instr 
                                                       >> 0xfU))));
                                    } else if ((0x1000U 
                                                & vlSelfRef.instr)) {
                                        vlSelfRef.core_top__DOT__reg_wdata_src = 3U;
                                        vlSelfRef.core_top__DOT__reg_write = 1U;
                                        vlSelfRef.core_top__DOT__csr_write = 1U;
                                    }
                                } else if ((0x2000U 
                                            & vlSelfRef.instr)) {
                                    vlSelfRef.core_top__DOT__reg_wdata_src = 3U;
                                    vlSelfRef.core_top__DOT__reg_write = 1U;
                                    vlSelfRef.core_top__DOT__csr_write 
                                        = ((0x1000U 
                                            & vlSelfRef.instr)
                                            ? (0U != 
                                               (0x1fU 
                                                & (vlSelfRef.instr 
                                                   >> 0xfU)))
                                            : (0U != 
                                               (0x1fU 
                                                & (vlSelfRef.instr 
                                                   >> 0xfU))));
                                } else if ((0x1000U 
                                            & vlSelfRef.instr)) {
                                    vlSelfRef.core_top__DOT__reg_wdata_src = 3U;
                                    vlSelfRef.core_top__DOT__reg_write = 1U;
                                    vlSelfRef.core_top__DOT__csr_write = 1U;
                                }
                            }
                        }
                    }
                }
            } else if ((8U & vlSelfRef.instr)) {
                if ((4U & vlSelfRef.instr)) {
                    if ((2U & vlSelfRef.instr)) {
                        if ((1U & vlSelfRef.instr)) {
                            vlSelfRef.core_top__DOT__reg_wdata_src = 2U;
                            vlSelfRef.core_top__DOT__reg_write = 1U;
                        }
                    }
                }
            } else if ((4U & vlSelfRef.instr)) {
                if ((2U & vlSelfRef.instr)) {
                    if ((1U & vlSelfRef.instr)) {
                        vlSelfRef.core_top__DOT__reg_wdata_src = 2U;
                        vlSelfRef.core_top__DOT__reg_write = 1U;
                    }
                }
            }
        }
    } else {
        if ((1U & (~ (vlSelfRef.instr >> 5U)))) {
            if ((1U & (~ (vlSelfRef.instr >> 4U)))) {
                if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                    if ((1U & (~ (vlSelfRef.instr >> 2U)))) {
                        if ((2U & vlSelfRef.instr)) {
                            if ((1U & vlSelfRef.instr)) {
                                vlSelfRef.core_top__DOT__reg_wdata_src = 1U;
                            }
                        }
                    }
                }
            }
        }
        if ((0x20U & vlSelfRef.instr)) {
            if ((0x10U & vlSelfRef.instr)) {
                if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                    if ((4U & vlSelfRef.instr)) {
                        if ((2U & vlSelfRef.instr)) {
                            if ((1U & vlSelfRef.instr)) {
                                vlSelfRef.core_top__DOT__reg_write = 1U;
                            }
                        }
                    } else if ((2U & vlSelfRef.instr)) {
                        if ((1U & vlSelfRef.instr)) {
                            vlSelfRef.core_top__DOT__reg_write = 1U;
                        }
                    }
                }
            }
        } else if ((0x10U & vlSelfRef.instr)) {
            if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                if ((4U & vlSelfRef.instr)) {
                    if ((2U & vlSelfRef.instr)) {
                        if ((1U & vlSelfRef.instr)) {
                            vlSelfRef.core_top__DOT__reg_write = 1U;
                        }
                    }
                } else if ((2U & vlSelfRef.instr)) {
                    if ((1U & vlSelfRef.instr)) {
                        vlSelfRef.core_top__DOT__reg_write = 1U;
                    }
                }
            }
        } else if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
            if ((1U & (~ (vlSelfRef.instr >> 2U)))) {
                if ((2U & vlSelfRef.instr)) {
                    if ((1U & vlSelfRef.instr)) {
                        vlSelfRef.core_top__DOT__reg_write = 1U;
                    }
                }
            }
        }
    }
    vlSelfRef.core_top__DOT__csr_write_eff = ((~ (IData)(vlSelfRef.interrupt_valid)) 
                                              & (IData)(vlSelfRef.core_top__DOT__csr_write));
    if ((0x40U & vlSelfRef.instr)) {
        if ((0x20U & vlSelfRef.instr)) {
            if ((0x10U & vlSelfRef.instr)) {
                if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                    if ((1U & (~ (vlSelfRef.instr >> 2U)))) {
                        if ((2U & vlSelfRef.instr)) {
                            if ((1U & vlSelfRef.instr)) {
                                if ((1U & (~ (vlSelfRef.instr 
                                              >> 0xeU)))) {
                                    if ((1U & (~ (vlSelfRef.instr 
                                                  >> 0xdU)))) {
                                        if ((1U & (~ 
                                                   (vlSelfRef.instr 
                                                    >> 0xcU)))) {
                                            if ((1U 
                                                 == 
                                                 (vlSelfRef.instr 
                                                  >> 0x14U))) {
                                                vlSelfRef.core_top__DOT__is_ebreak = 1U;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.core_top__DOT__is_ebreak_eff = ((~ (IData)(vlSelfRef.interrupt_valid)) 
                                              & (IData)(vlSelfRef.core_top__DOT__is_ebreak));
    if ((0x40U & vlSelfRef.instr)) {
        if ((0x20U & vlSelfRef.instr)) {
            if ((0x10U & vlSelfRef.instr)) {
                if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                    if ((1U & (~ (vlSelfRef.instr >> 2U)))) {
                        if ((2U & vlSelfRef.instr)) {
                            if ((1U & vlSelfRef.instr)) {
                                if ((0x4000U & vlSelfRef.instr)) {
                                    if ((0x2000U & vlSelfRef.instr)) {
                                        vlSelfRef.core_top__DOT__csr_op 
                                            = ((0x1000U 
                                                & vlSelfRef.instr)
                                                ? 2U
                                                : 1U);
                                    } else if ((0x1000U 
                                                & vlSelfRef.instr)) {
                                        vlSelfRef.core_top__DOT__csr_op = 0U;
                                    }
                                } else if ((0x2000U 
                                            & vlSelfRef.instr)) {
                                    vlSelfRef.core_top__DOT__csr_op 
                                        = ((0x1000U 
                                            & vlSelfRef.instr)
                                            ? 2U : 1U);
                                } else if ((0x1000U 
                                            & vlSelfRef.instr)) {
                                    vlSelfRef.core_top__DOT__csr_op = 0U;
                                }
                                if ((1U & (~ (vlSelfRef.instr 
                                              >> 0xeU)))) {
                                    if ((1U & (~ (vlSelfRef.instr 
                                                  >> 0xdU)))) {
                                        if ((1U & (~ 
                                                   (vlSelfRef.instr 
                                                    >> 0xcU)))) {
                                            if ((1U 
                                                 != 
                                                 (vlSelfRef.instr 
                                                  >> 0x14U))) {
                                                if (
                                                    (0U 
                                                     == 
                                                     (vlSelfRef.instr 
                                                      >> 0x14U))) {
                                                    vlSelfRef.core_top__DOT__trap_code = 0xbU;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.core_top__DOT__trap_code_eff = ((IData)(vlSelfRef.interrupt_valid)
                                               ? vlSelfRef.interrupt_cause
                                               : vlSelfRef.core_top__DOT__trap_code);
    if ((0x40U & vlSelfRef.instr)) {
        if ((0x20U & vlSelfRef.instr)) {
            if ((0x10U & vlSelfRef.instr)) {
                if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                    if ((1U & (~ (vlSelfRef.instr >> 2U)))) {
                        if ((2U & vlSelfRef.instr)) {
                            if ((1U & vlSelfRef.instr)) {
                                if ((0x4000U & vlSelfRef.instr)) {
                                    if ((0x2000U & vlSelfRef.instr)) {
                                        vlSelfRef.core_top__DOT__csr_imm = 0U;
                                    } else if ((0x1000U 
                                                & vlSelfRef.instr)) {
                                        vlSelfRef.core_top__DOT__csr_imm = 0U;
                                    }
                                } else if ((0x2000U 
                                            & vlSelfRef.instr)) {
                                    vlSelfRef.core_top__DOT__csr_imm = 1U;
                                } else if ((0x1000U 
                                            & vlSelfRef.instr)) {
                                    vlSelfRef.core_top__DOT__csr_imm = 1U;
                                }
                                if ((1U & (~ (vlSelfRef.instr 
                                              >> 0xeU)))) {
                                    if ((1U & (~ (vlSelfRef.instr 
                                                  >> 0xdU)))) {
                                        if ((1U & (~ 
                                                   (vlSelfRef.instr 
                                                    >> 0xcU)))) {
                                            if ((1U 
                                                 != 
                                                 (vlSelfRef.instr 
                                                  >> 0x14U))) {
                                                if (
                                                    (0U 
                                                     == 
                                                     (vlSelfRef.instr 
                                                      >> 0x14U))) {
                                                    vlSelfRef.core_top__DOT__trap_enter = 1U;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if ((1U & (~ (vlSelfRef.instr >> 4U)))) {
                if ((8U & vlSelfRef.instr)) {
                    if ((4U & vlSelfRef.instr)) {
                        if ((2U & vlSelfRef.instr)) {
                            if ((1U & vlSelfRef.instr)) {
                                vlSelfRef.core_top__DOT__pc_sel = 1U;
                            }
                        }
                    }
                } else if ((4U & vlSelfRef.instr)) {
                    if ((2U & vlSelfRef.instr)) {
                        if ((1U & vlSelfRef.instr)) {
                            vlSelfRef.core_top__DOT__pc_sel = 2U;
                        }
                    }
                } else if ((2U & vlSelfRef.instr)) {
                    if ((1U & vlSelfRef.instr)) {
                        vlSelfRef.core_top__DOT__pc_sel = 3U;
                    }
                }
                if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                    if ((1U & (~ (vlSelfRef.instr >> 2U)))) {
                        if ((2U & vlSelfRef.instr)) {
                            if ((1U & vlSelfRef.instr)) {
                                vlSelfRef.core_top__DOT__br_type = 0U;
                                if ((0x4000U & vlSelfRef.instr)) {
                                    vlSelfRef.core_top__DOT__br_type 
                                        = ((0x2000U 
                                            & vlSelfRef.instr)
                                            ? ((0x1000U 
                                                & vlSelfRef.instr)
                                                ? 5U
                                                : 4U)
                                            : ((0x1000U 
                                                & vlSelfRef.instr)
                                                ? 3U
                                                : 2U));
                                } else if ((1U & (~ 
                                                  (vlSelfRef.instr 
                                                   >> 0xdU)))) {
                                    vlSelfRef.core_top__DOT__br_type 
                                        = ((0x1000U 
                                            & vlSelfRef.instr)
                                            ? 1U : 0U);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.core_top__DOT__trap_enter_eff = ((IData)(vlSelfRef.core_top__DOT__trap_enter) 
                                               | (IData)(vlSelfRef.interrupt_valid));
    if ((0x40U & vlSelfRef.instr)) {
        if ((0x20U & vlSelfRef.instr)) {
            if ((0x10U & vlSelfRef.instr)) {
                if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                    if ((1U & (~ (vlSelfRef.instr >> 2U)))) {
                        if ((2U & vlSelfRef.instr)) {
                            if ((1U & vlSelfRef.instr)) {
                                if ((1U & (~ (vlSelfRef.instr 
                                              >> 0xeU)))) {
                                    if ((1U & (~ (vlSelfRef.instr 
                                                  >> 0xdU)))) {
                                        if ((1U & (~ 
                                                   (vlSelfRef.instr 
                                                    >> 0xcU)))) {
                                            if ((1U 
                                                 != 
                                                 (vlSelfRef.instr 
                                                  >> 0x14U))) {
                                                if (
                                                    (0U 
                                                     != 
                                                     (vlSelfRef.instr 
                                                      >> 0x14U))) {
                                                    if (
                                                        (0x302U 
                                                         == 
                                                         (vlSelfRef.instr 
                                                          >> 0x14U))) {
                                                        vlSelfRef.core_top__DOT__mret = 1U;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.core_top__DOT__mret_eff = ((~ (IData)(vlSelfRef.interrupt_valid)) 
                                         & (IData)(vlSelfRef.core_top__DOT__mret));
    vlSelfRef.core_top__DOT__mem_read_eff = ((~ (IData)(vlSelfRef.interrupt_valid)) 
                                             & (IData)(vlSelfRef.core_top__DOT__mem_read));
    if ((0x40U & vlSelfRef.instr)) {
        if ((0x20U & vlSelfRef.instr)) {
            if ((0x10U & vlSelfRef.instr)) {
                if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                    if ((1U & (~ (vlSelfRef.instr >> 2U)))) {
                        if ((2U & vlSelfRef.instr)) {
                            if ((1U & vlSelfRef.instr)) {
                                if ((0x4000U & vlSelfRef.instr)) {
                                    if ((0x2000U & vlSelfRef.instr)) {
                                        vlSelfRef.core_top__DOT__csr_read = 1U;
                                    } else if ((0x1000U 
                                                & vlSelfRef.instr)) {
                                        vlSelfRef.core_top__DOT__csr_read = 1U;
                                    }
                                } else if ((0x2000U 
                                            & vlSelfRef.instr)) {
                                    vlSelfRef.core_top__DOT__csr_read = 1U;
                                } else if ((0x1000U 
                                            & vlSelfRef.instr)) {
                                    vlSelfRef.core_top__DOT__csr_read = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.core_top__DOT__csr_rdata = ((IData)(vlSelfRef.core_top__DOT__csr_read)
                                           ? ((0x300U 
                                               == (vlSelfRef.instr 
                                                   >> 0x14U))
                                               ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus
                                               : ((0x305U 
                                                   == 
                                                   (vlSelfRef.instr 
                                                    >> 0x14U))
                                                   ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                                                   : 
                                                  ((0x341U 
                                                    == 
                                                    (vlSelfRef.instr 
                                                     >> 0x14U))
                                                    ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                                                    : 
                                                   ((0x342U 
                                                     == 
                                                     (vlSelfRef.instr 
                                                      >> 0x14U))
                                                     ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause
                                                     : 
                                                    ((0x344U 
                                                      == 
                                                      (vlSelfRef.instr 
                                                       >> 0x14U))
                                                      ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip
                                                      : 
                                                     ((0x304U 
                                                       == 
                                                       (vlSelfRef.instr 
                                                        >> 0x14U))
                                                       ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mie
                                                       : 
                                                      ((0x343U 
                                                        == 
                                                        (vlSelfRef.instr 
                                                         >> 0x14U))
                                                        ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtval
                                                        : 
                                                       ((0x340U 
                                                         == 
                                                         (vlSelfRef.instr 
                                                          >> 0x14U))
                                                         ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mscratch
                                                         : 
                                                        ((0xb00U 
                                                          == 
                                                          (vlSelfRef.instr 
                                                           >> 0x14U))
                                                          ? (IData)(vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64)
                                                          : 
                                                         ((0xb80U 
                                                           == 
                                                           (vlSelfRef.instr 
                                                            >> 0x14U))
                                                           ? (IData)(
                                                                     (vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64 
                                                                      >> 0x20U))
                                                           : 0U))))))))))
                                           : 0U);
    vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src 
        = ((IData)(vlSelfRef.core_top__DOT__csr_imm)
            ? vlSelfRef.core_top__DOT__rs1_rdata : 
           (0x1fU & (vlSelfRef.instr >> 0xfU)));
    vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
        = ((IData)(vlSelfRef.core_top__DOT__alu_src_a)
            ? vlSelfRef.core_top__DOT__pc : vlSelfRef.core_top__DOT__rs1_rdata);
    vlSelfRef.__VdfgRegularize_h495687df_0_7 = ((8U 
                                                 & vlSelfRef.instr)
                                                 ? 0U
                                                 : 
                                                ((4U 
                                                  & vlSelfRef.instr)
                                                  ? 0U
                                                  : vlSelfRef.__VdfgRegularize_h495687df_0_6));
    if ((0x40U & vlSelfRef.instr)) {
        if ((0x20U & vlSelfRef.instr)) {
            if ((1U & (~ (vlSelfRef.instr >> 4U)))) {
                if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                    if ((4U & vlSelfRef.instr)) {
                        if ((2U & vlSelfRef.instr)) {
                            if ((1U & vlSelfRef.instr)) {
                                vlSelfRef.core_top__DOT__alu_op = 1U;
                                vlSelfRef.core_top__DOT__alu_src_b = 1U;
                            }
                        }
                    }
                }
            }
            vlSelfRef.core_top__DOT__imm_jal = ((0x10U 
                                                 & vlSelfRef.instr)
                                                 ? vlSelfRef.__VdfgRegularize_h495687df_0_7
                                                 : 
                                                ((8U 
                                                  & vlSelfRef.instr)
                                                  ? 
                                                 ((4U 
                                                   & vlSelfRef.instr)
                                                   ? 
                                                  ((2U 
                                                    & vlSelfRef.instr)
                                                    ? 
                                                   ((1U 
                                                     & vlSelfRef.instr)
                                                     ? 
                                                    ((((- (IData)(
                                                                  (vlSelfRef.instr 
                                                                   >> 0x1fU))) 
                                                       << 0x15U) 
                                                      | (0x100000U 
                                                         & (vlSelfRef.instr 
                                                            >> 0xbU))) 
                                                     | (((0xff000U 
                                                          & vlSelfRef.instr) 
                                                         | (0x800U 
                                                            & (vlSelfRef.instr 
                                                               >> 9U))) 
                                                        | (0x7feU 
                                                           & (vlSelfRef.instr 
                                                              >> 0x14U))))
                                                     : 0U)
                                                    : 0U)
                                                   : 0U)
                                                  : 
                                                 ((4U 
                                                   & vlSelfRef.instr)
                                                   ? vlSelfRef.__VdfgRegularize_h495687df_0_6
                                                   : 
                                                  ((2U 
                                                    & vlSelfRef.instr)
                                                    ? 
                                                   ((1U 
                                                     & vlSelfRef.instr)
                                                     ? 
                                                    (((- (IData)(
                                                                 (vlSelfRef.instr 
                                                                  >> 0x1fU))) 
                                                      << 0xdU) 
                                                     | (((0x1000U 
                                                          & (vlSelfRef.instr 
                                                             >> 0x13U)) 
                                                         | (0x800U 
                                                            & (vlSelfRef.instr 
                                                               << 4U))) 
                                                        | ((0x7e0U 
                                                            & (vlSelfRef.instr 
                                                               >> 0x14U)) 
                                                           | (0x1eU 
                                                              & (vlSelfRef.instr 
                                                                 >> 7U)))))
                                                     : 0U)
                                                    : 0U))));
        } else {
            vlSelfRef.core_top__DOT__imm_jal = 0U;
        }
    } else if ((0x20U & vlSelfRef.instr)) {
        if ((0x10U & vlSelfRef.instr)) {
            if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                if ((4U & vlSelfRef.instr)) {
                    if ((2U & vlSelfRef.instr)) {
                        if ((1U & vlSelfRef.instr)) {
                            vlSelfRef.core_top__DOT__alu_op = 0xaU;
                            vlSelfRef.core_top__DOT__alu_src_b = 1U;
                        }
                    }
                } else if ((2U & vlSelfRef.instr)) {
                    if ((1U & vlSelfRef.instr)) {
                        vlSelfRef.core_top__DOT__alu_op 
                            = ((0x4000U & vlSelfRef.instr)
                                ? ((0x2000U & vlSelfRef.instr)
                                    ? ((0x1000U & vlSelfRef.instr)
                                        ? 7U : 6U) : 
                                   ((0x1000U & vlSelfRef.instr)
                                     ? ((0x40000000U 
                                         & vlSelfRef.instr)
                                         ? 9U : 8U)
                                     : 5U)) : ((0x2000U 
                                                & vlSelfRef.instr)
                                                ? (
                                                   (0x1000U 
                                                    & vlSelfRef.instr)
                                                    ? 4U
                                                    : 3U)
                                                : (
                                                   (0x1000U 
                                                    & vlSelfRef.instr)
                                                    ? 2U
                                                    : 
                                                   ((0x40000000U 
                                                     & vlSelfRef.instr)
                                                     ? 0U
                                                     : 1U))));
                    }
                }
            }
            vlSelfRef.core_top__DOT__imm_jal = ((8U 
                                                 & vlSelfRef.instr)
                                                 ? 0U
                                                 : 
                                                ((4U 
                                                  & vlSelfRef.instr)
                                                  ? vlSelfRef.__VdfgRegularize_h495687df_0_8
                                                  : 0U));
        } else {
            if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
                if ((1U & (~ (vlSelfRef.instr >> 2U)))) {
                    if ((2U & vlSelfRef.instr)) {
                        if ((1U & vlSelfRef.instr)) {
                            vlSelfRef.core_top__DOT__alu_op = 1U;
                            vlSelfRef.core_top__DOT__alu_src_b = 1U;
                        }
                    }
                }
            }
            vlSelfRef.core_top__DOT__imm_jal = ((8U 
                                                 & vlSelfRef.instr)
                                                 ? 0U
                                                 : 
                                                ((4U 
                                                  & vlSelfRef.instr)
                                                  ? 0U
                                                  : 
                                                 ((2U 
                                                   & vlSelfRef.instr)
                                                   ? 
                                                  ((1U 
                                                    & vlSelfRef.instr)
                                                    ? 
                                                   (((- (IData)(
                                                                (vlSelfRef.instr 
                                                                 >> 0x1fU))) 
                                                     << 0xcU) 
                                                    | ((0xfe0U 
                                                        & (vlSelfRef.instr 
                                                           >> 0x14U)) 
                                                       | (0x1fU 
                                                          & (vlSelfRef.instr 
                                                             >> 7U))))
                                                    : 0U)
                                                   : 0U)));
        }
    } else if ((0x10U & vlSelfRef.instr)) {
        if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
            if ((4U & vlSelfRef.instr)) {
                if ((2U & vlSelfRef.instr)) {
                    if ((1U & vlSelfRef.instr)) {
                        vlSelfRef.core_top__DOT__alu_op = 1U;
                        vlSelfRef.core_top__DOT__alu_src_b = 1U;
                    }
                }
            } else if ((2U & vlSelfRef.instr)) {
                if ((1U & vlSelfRef.instr)) {
                    vlSelfRef.core_top__DOT__alu_op 
                        = ((0x4000U & vlSelfRef.instr)
                            ? ((0x2000U & vlSelfRef.instr)
                                ? ((0x1000U & vlSelfRef.instr)
                                    ? 7U : 6U) : ((0x1000U 
                                                   & vlSelfRef.instr)
                                                   ? 
                                                  ((0x40000000U 
                                                    & vlSelfRef.instr)
                                                    ? 9U
                                                    : 8U)
                                                   : 5U))
                            : ((0x2000U & vlSelfRef.instr)
                                ? ((0x1000U & vlSelfRef.instr)
                                    ? 4U : 3U) : ((0x1000U 
                                                   & vlSelfRef.instr)
                                                   ? 2U
                                                   : 1U)));
                    vlSelfRef.core_top__DOT__alu_src_b = 1U;
                }
            }
        }
        vlSelfRef.core_top__DOT__imm_jal = ((8U & vlSelfRef.instr)
                                             ? 0U : 
                                            ((4U & vlSelfRef.instr)
                                              ? vlSelfRef.__VdfgRegularize_h495687df_0_8
                                              : vlSelfRef.__VdfgRegularize_h495687df_0_6));
    } else {
        if ((1U & (~ (vlSelfRef.instr >> 3U)))) {
            if ((1U & (~ (vlSelfRef.instr >> 2U)))) {
                if ((2U & vlSelfRef.instr)) {
                    if ((1U & vlSelfRef.instr)) {
                        vlSelfRef.core_top__DOT__alu_op = 1U;
                        vlSelfRef.core_top__DOT__alu_src_b = 1U;
                    }
                }
            }
        }
        vlSelfRef.core_top__DOT__imm_jal = vlSelfRef.__VdfgRegularize_h495687df_0_7;
    }
    vlSelfRef.core_top__DOT__csr_wdata = ((0U == (IData)(vlSelfRef.core_top__DOT__csr_op))
                                           ? vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src
                                           : ((1U == (IData)(vlSelfRef.core_top__DOT__csr_op))
                                               ? (vlSelfRef.core_top__DOT__csr_rdata 
                                                  | vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src)
                                               : ((2U 
                                                   == (IData)(vlSelfRef.core_top__DOT__csr_op))
                                                   ? 
                                                  ((~ vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src) 
                                                   & vlSelfRef.core_top__DOT__csr_rdata)
                                                   : 0U)));
    vlSelfRef.__VdfgRegularize_h3c5bdd75_0_0 = (vlSelfRef.core_top__DOT__imm_jal 
                                                + vlSelfRef.core_top__DOT__pc);
    vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2 
        = ((0U == (IData)(vlSelfRef.core_top__DOT__alu_src_b))
            ? vlSelfRef.core_top__DOT__rs2_rdata : 
           ((1U == (IData)(vlSelfRef.core_top__DOT__alu_src_b))
             ? vlSelfRef.core_top__DOT__imm_jal : 4U));
    vlSelfRef.core_top__DOT__jump_jalr = ((0x10U & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                           ? 0U : (
                                                   (8U 
                                                    & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                    ? 
                                                   ((4U 
                                                     & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                     ? 0U
                                                     : 
                                                    ((2U 
                                                      & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                      ? 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                       ? 0U
                                                       : vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)
                                                      : 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                       ? 
                                                      VL_SHIFTRS_III(32,32,5, vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1, 
                                                                     (0x1fU 
                                                                      & vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))
                                                       : 
                                                      (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                       >> 
                                                       (0x1fU 
                                                        & vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)))))
                                                    : 
                                                   ((4U 
                                                     & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                     ? 
                                                    ((2U 
                                                      & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                      ? 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                       ? 
                                                      (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                       & vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)
                                                       : 
                                                      (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                       | vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))
                                                      : 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                       ? 
                                                      (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                       ^ vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)
                                                       : 
                                                      ((vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                        < vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)
                                                        ? 1U
                                                        : 0U)))
                                                     : 
                                                    ((2U 
                                                      & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                      ? 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                       ? 
                                                      (VL_LTS_III(32, vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1, vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)
                                                        ? 1U
                                                        : 0U)
                                                       : 
                                                      (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                       << 
                                                       (0x1fU 
                                                        & vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)))
                                                      : 
                                                     ((1U 
                                                       & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                       ? 
                                                      (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                       + vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)
                                                       : 
                                                      (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                       - vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))))));
    vlSelfRef.core_top__DOT__u_if_stage__DOT__pc_next 
        = (((IData)(vlSelfRef.core_top__DOT__mret_eff) 
            | (IData)(vlSelfRef.core_top__DOT__trap_enter_eff))
            ? ((IData)(vlSelfRef.core_top__DOT__trap_enter_eff)
                ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                : ((IData)(vlSelfRef.core_top__DOT__mret_eff)
                    ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                    : 0U)) : ((0U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                               ? ((IData)(4U) + vlSelfRef.core_top__DOT__pc)
                               : ((1U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                   ? vlSelfRef.__VdfgRegularize_h3c5bdd75_0_0
                                   : ((2U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                       ? (0xfffffffeU 
                                          & vlSelfRef.core_top__DOT__jump_jalr)
                                       : ((3U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                           ? (((4U 
                                                & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                ? (
                                                   (~ 
                                                    ((IData)(vlSelfRef.core_top__DOT__br_type) 
                                                     >> 1U)) 
                                                   & ((1U 
                                                       & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                       ? 
                                                      (vlSelfRef.core_top__DOT__rs1_rdata 
                                                       >= vlSelfRef.core_top__DOT__rs2_rdata)
                                                       : 
                                                      (vlSelfRef.core_top__DOT__rs1_rdata 
                                                       < vlSelfRef.core_top__DOT__rs2_rdata)))
                                                : (
                                                   (2U 
                                                    & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                     ? 
                                                    VL_GTES_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.core_top__DOT__rs2_rdata)
                                                     : 
                                                    VL_LTS_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.core_top__DOT__rs2_rdata))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                     ? 
                                                    (vlSelfRef.core_top__DOT__rs1_rdata 
                                                     != vlSelfRef.core_top__DOT__rs2_rdata)
                                                     : 
                                                    (vlSelfRef.core_top__DOT__rs1_rdata 
                                                     == vlSelfRef.core_top__DOT__rs2_rdata))))
                                               ? vlSelfRef.__VdfgRegularize_h3c5bdd75_0_0
                                               : ((IData)(4U) 
                                                  + vlSelfRef.core_top__DOT__pc))
                                           : ((IData)(4U) 
                                              + vlSelfRef.core_top__DOT__pc))))));
    Vcore_top___024unit____Vdpiimwrap_dpi_mem_read_TOP____024unit(vlSelfRef.core_top__DOT__jump_jalr, (IData)(vlSelfRef.core_top__DOT__mem_read_eff), vlSelfRef.__Vfunc_dpi_mem_read__1__Vfuncout);
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
        = vlSelfRef.__Vfunc_dpi_mem_read__1__Vfuncout;
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected 
        = (0xffU & ((0U == (3U & vlSelfRef.core_top__DOT__jump_jalr))
                     ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw
                     : ((1U == (3U & vlSelfRef.core_top__DOT__jump_jalr))
                         ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                            >> 8U) : ((2U == (3U & vlSelfRef.core_top__DOT__jump_jalr))
                                       ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 0x10U)
                                       : (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 0x18U)))));
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected 
        = (0xffffU & ((2U & vlSelfRef.core_top__DOT__jump_jalr)
                       ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                          >> 0x10U) : vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw));
}
