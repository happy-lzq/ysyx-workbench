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
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write 
        = ((IData)(vlSelfRef.core_top__DOT__ex_mem_valid) 
           & ((~ (IData)(vlSelfRef.interrupt_valid)) 
              & ((IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl) 
                 >> 3U)));
    vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter 
        = ((IData)(vlSelfRef.interrupt_valid) | ((IData)(vlSelfRef.core_top__DOT__mem_wb_valid) 
                                                 & (IData)(
                                                           (vlSelfRef.core_top__DOT__mem_wb_sys_ctrl 
                                                            >> 0x22U))));
    vlSelfRef.core_top__DOT__u_csr__DOT__trap_code 
        = ((IData)(vlSelfRef.interrupt_valid) ? vlSelfRef.interrupt_cause
            : (IData)(vlSelfRef.core_top__DOT__mem_wb_sys_ctrl));
    vlSelfRef.core_top__DOT__u_csr__DOT____VdfgRegularize_h8136f0d8_0_0 
        = ((~ (IData)(vlSelfRef.interrupt_valid)) & (IData)(vlSelfRef.core_top__DOT__mem_wb_valid));
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_read 
        = ((IData)(vlSelfRef.core_top__DOT__ex_mem_valid) 
           & ((~ (IData)(vlSelfRef.interrupt_valid)) 
              & ((IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl) 
                 >> 4U)));
    vlSelfRef.core_top__DOT__u_if_stage__DOT__trap_enter 
        = ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_enter_int) 
           | (IData)(vlSelfRef.interrupt_valid));
    vlSelfRef.core_top__DOT__is_ebreak_eff = ((IData)(vlSelfRef.core_top__DOT__u_csr__DOT____VdfgRegularize_h8136f0d8_0_0) 
                                              & (IData)(
                                                        (vlSelfRef.core_top__DOT__mem_wb_sys_ctrl 
                                                         >> 0x20U)));
    vlSelfRef.core_top__DOT__u_csr__DOT__csr_write 
        = ((IData)(vlSelfRef.core_top__DOT__u_csr__DOT____VdfgRegularize_h8136f0d8_0_0) 
           & (IData)((vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                      >> 0x2fU)));
    vlSelfRef.core_top__DOT__u_csr__DOT__mret = ((IData)(vlSelfRef.core_top__DOT__u_csr__DOT____VdfgRegularize_h8136f0d8_0_0) 
                                                 & (IData)(
                                                           (vlSelfRef.core_top__DOT__mem_wb_sys_ctrl 
                                                            >> 0x21U)));
    Vcore_top___024unit____Vdpiimwrap_dpi_mem_read_TOP____024unit(vlSelfRef.core_top__DOT__ex_mem_alu_result, (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_read), vlSelfRef.__Vfunc_dpi_mem_read__5__Vfuncout);
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
        = vlSelfRef.__Vfunc_dpi_mem_read__5__Vfuncout;
    vlSelfRef.core_top__DOT__u_if_stage__DOT__pc_next 
        = (((IData)(vlSelfRef.core_top__DOT__u_if_stage__DOT__trap_enter) 
            | ((~ (IData)(vlSelfRef.interrupt_valid)) 
               & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int)))
            ? ((IData)(vlSelfRef.core_top__DOT__u_if_stage__DOT__trap_enter)
                ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                : ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int)
                    ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                    : 0U)) : (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jal_int) 
                               | ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int) 
                                  | ((3U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel)) 
                                     & ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                         ? ((~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type) 
                                                >> 1U)) 
                                            & ((1U 
                                                & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                                ? (vlSelfRef.core_top__DOT__rs1_rdata 
                                                   >= vlSelfRef.core_top__DOT__rs2_rdata)
                                                : (vlSelfRef.core_top__DOT__rs1_rdata 
                                                   < vlSelfRef.core_top__DOT__rs2_rdata)))
                                         : ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                             ? ((1U 
                                                 & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                                 ? 
                                                VL_GTES_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.core_top__DOT__rs2_rdata)
                                                 : 
                                                VL_LTS_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.core_top__DOT__rs2_rdata))
                                             : ((1U 
                                                 & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                                 ? 
                                                (vlSelfRef.core_top__DOT__rs1_rdata 
                                                 != vlSelfRef.core_top__DOT__rs2_rdata)
                                                 : 
                                                (vlSelfRef.core_top__DOT__rs1_rdata 
                                                 == vlSelfRef.core_top__DOT__rs2_rdata)))))))
                               ? ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int)
                                   ? (0xfffffffeU & vlSelfRef.core_top__DOT__u_id_stage__DOT__target_sum)
                                   : vlSelfRef.core_top__DOT__u_id_stage__DOT__target_sum)
                               : ((IData)(4U) + vlSelfRef.core_top__DOT__pc)));
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected 
        = (0xffU & ((0U == (3U & vlSelfRef.core_top__DOT__ex_mem_alu_result))
                     ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw
                     : ((1U == (3U & vlSelfRef.core_top__DOT__ex_mem_alu_result))
                         ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                            >> 8U) : ((2U == (3U & vlSelfRef.core_top__DOT__ex_mem_alu_result))
                                       ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 0x10U)
                                       : (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 0x18U)))));
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected 
        = (0xffffU & ((2U & vlSelfRef.core_top__DOT__ex_mem_alu_result)
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
    if (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write) {
        Vcore_top___024unit____Vdpiimwrap_dpi_mem_write_TOP____024unit(vlSelfRef.core_top__DOT__ex_mem_alu_result, 
                                                                       ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write)
                                                                         ? 
                                                                        ((5U 
                                                                          == 
                                                                          (7U 
                                                                           & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                                                          ? vlSelfRef.core_top__DOT__ex_mem_rs2_rdata
                                                                          : 
                                                                         ((6U 
                                                                           == 
                                                                           (7U 
                                                                            & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                                                           ? 
                                                                          ((vlSelfRef.core_top__DOT__ex_mem_rs2_rdata 
                                                                            << 0x10U) 
                                                                           | (0xffffU 
                                                                              & vlSelfRef.core_top__DOT__ex_mem_rs2_rdata))
                                                                           : 
                                                                          ((vlSelfRef.core_top__DOT__ex_mem_rs2_rdata 
                                                                            << 0x18U) 
                                                                           | ((0xff0000U 
                                                                               & (vlSelfRef.core_top__DOT__ex_mem_rs2_rdata 
                                                                                << 0x10U)) 
                                                                              | ((0xff00U 
                                                                                & (vlSelfRef.core_top__DOT__ex_mem_rs2_rdata 
                                                                                << 8U)) 
                                                                                | (0xffU 
                                                                                & vlSelfRef.core_top__DOT__ex_mem_rs2_rdata))))))
                                                                         : 0U), 
                                                                       ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write)
                                                                         ? 
                                                                        ((5U 
                                                                          == 
                                                                          (7U 
                                                                           & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                                                          ? 0xfU
                                                                          : 
                                                                         (0xfU 
                                                                          & ((6U 
                                                                              == 
                                                                              (7U 
                                                                               & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                                                              ? 
                                                                             ((2U 
                                                                               & vlSelfRef.core_top__DOT__ex_mem_alu_result)
                                                                               ? 0xcU
                                                                               : 3U)
                                                                              : 
                                                                             ((IData)(1U) 
                                                                              << 
                                                                              (3U 
                                                                               & vlSelfRef.core_top__DOT__ex_mem_alu_result)))))
                                                                         : 0U));
    }
    __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus 
        = vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus;
    __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 = vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64;
    __Vdly__core_top__DOT__u_csr__DOT__csr_mip = vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip;
    __VdlySet__core_top__DOT__u_regfile__DOT__rf__v0 = 0U;
    if ((((IData)(vlSelfRef.core_top__DOT__u_csr__DOT____VdfgRegularize_h8136f0d8_0_0) 
          & ((IData)(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl) 
             >> 2U)) & (0U != (IData)(vlSelfRef.core_top__DOT__mem_wb_rd_addr)))) {
        __VdlyVal__core_top__DOT__u_regfile__DOT__rf__v0 
            = ((0U == (3U & (IData)(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl)))
                ? vlSelfRef.core_top__DOT__mem_wb_alu_result
                : ((1U == (3U & (IData)(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl)))
                    ? vlSelfRef.core_top__DOT__mem_wb_mem_rdata
                    : ((2U == (3U & (IData)(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl)))
                        ? vlSelfRef.core_top__DOT__mem_wb_pc_plus4
                        : vlSelfRef.core_top__DOT__mem_wb_csr_rdata)));
        __VdlyDim0__core_top__DOT__u_regfile__DOT__rf__v0 
            = vlSelfRef.core_top__DOT__mem_wb_rd_addr;
        __VdlySet__core_top__DOT__u_regfile__DOT__rf__v0 = 1U;
    }
    vlSelfRef.core_top__DOT__mem_wb_valid = ((1U & 
                                              (~ (IData)(vlSelfRef.rst))) 
                                             && (IData)(vlSelfRef.core_top__DOT__ex_mem_valid));
    if (vlSelfRef.rst) {
        __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus = 0x1800U;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus 
            = __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus;
        __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 = 0ULL;
        vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64 
            = __Vdly__core_top__DOT__u_csr__DOT__mcycle_64;
        __Vdly__core_top__DOT__u_csr__DOT__csr_mip = 0U;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip 
            = __Vdly__core_top__DOT__u_csr__DOT__csr_mip;
        vlSelfRef.halt = 0U;
        vlSelfRef.halt_ret = 0U;
    } else {
        if (vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter) {
            __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus 
                = (0x1800U | ((0xffffe000U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus) 
                              | (((0x700U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus) 
                                  | (0x80U & (vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus 
                                              << 4U))) 
                                 | (0x77U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus))));
            if ((0x80000007U == vlSelfRef.core_top__DOT__u_csr__DOT__trap_code)) {
                __Vdly__core_top__DOT__u_csr__DOT__csr_mip 
                    = (0xffffff7fU & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip);
            } else if ((0x8000000bU == vlSelfRef.core_top__DOT__u_csr__DOT__trap_code)) {
                __Vdly__core_top__DOT__u_csr__DOT__csr_mip 
                    = (0xfffff7ffU & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip);
            }
        } else {
            if (vlSelfRef.core_top__DOT__u_csr__DOT__mret) {
                __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus 
                    = (0x80U | ((0xffffe000U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus) 
                                | ((0x700U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus) 
                                   | ((0x70U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus) 
                                      | ((8U & (vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus 
                                                >> 4U)) 
                                         | (7U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus))))));
            } else if (vlSelfRef.core_top__DOT__u_csr__DOT__csr_write) {
                if (((((((((0x300U == (0xfffU & (IData)(
                                                        (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                         >> 0x20U)))) 
                           | (0x305U == (0xfffU & (IData)(
                                                          (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                           >> 0x20U))))) 
                          | (0x341U == (0xfffU & (IData)(
                                                         (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                          >> 0x20U))))) 
                         | (0x342U == (0xfffU & (IData)(
                                                        (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                         >> 0x20U))))) 
                        | (0x344U == (0xfffU & (IData)(
                                                       (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                        >> 0x20U))))) 
                       | (0x304U == (0xfffU & (IData)(
                                                      (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                       >> 0x20U))))) 
                      | (0x343U == (0xfffU & (IData)(
                                                     (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                      >> 0x20U))))) 
                     | (0x340U == (0xfffU & (IData)(
                                                    (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                     >> 0x20U)))))) {
                    if ((0x300U == (0xfffU & (IData)(
                                                     (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                      >> 0x20U))))) {
                        __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus 
                            = vlSelfRef.core_top__DOT__mem_wb_csr_wdata;
                    }
                }
            }
            if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__u_csr__DOT__mret)))) {
                if (vlSelfRef.core_top__DOT__u_csr__DOT__csr_write) {
                    if (((((((((0x300U == (0xfffU & (IData)(
                                                            (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                             >> 0x20U)))) 
                               | (0x305U == (0xfffU 
                                             & (IData)(
                                                       (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                        >> 0x20U))))) 
                              | (0x341U == (0xfffU 
                                            & (IData)(
                                                      (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                       >> 0x20U))))) 
                             | (0x342U == (0xfffU & (IData)(
                                                            (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                             >> 0x20U))))) 
                            | (0x344U == (0xfffU & (IData)(
                                                           (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                            >> 0x20U))))) 
                           | (0x304U == (0xfffU & (IData)(
                                                          (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                           >> 0x20U))))) 
                          | (0x343U == (0xfffU & (IData)(
                                                         (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                          >> 0x20U))))) 
                         | (0x340U == (0xfffU & (IData)(
                                                        (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                         >> 0x20U)))))) {
                        if ((0x300U != (0xfffU & (IData)(
                                                         (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                          >> 0x20U))))) {
                            if ((0x305U != (0xfffU 
                                            & (IData)(
                                                      (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                       >> 0x20U))))) {
                                if ((0x341U != (0xfffU 
                                                & (IData)(
                                                          (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                           >> 0x20U))))) {
                                    if ((0x342U != 
                                         (0xfffU & (IData)(
                                                           (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                            >> 0x20U))))) {
                                        if ((0x344U 
                                             == (0xfffU 
                                                 & (IData)(
                                                           (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                            >> 0x20U))))) {
                                            __Vdly__core_top__DOT__u_csr__DOT__csr_mip 
                                                = vlSelfRef.core_top__DOT__mem_wb_csr_wdata;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus 
            = __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus;
        __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 
            = (1ULL + vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64);
        if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter)))) {
            if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__u_csr__DOT__mret)))) {
                if (vlSelfRef.core_top__DOT__u_csr__DOT__csr_write) {
                    if ((1U & (~ ((((((((0x300U == 
                                         (0xfffU & (IData)(
                                                           (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                            >> 0x20U)))) 
                                        | (0x305U == 
                                           (0xfffU 
                                            & (IData)(
                                                      (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                       >> 0x20U))))) 
                                       | (0x341U == 
                                          (0xfffU & (IData)(
                                                            (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                             >> 0x20U))))) 
                                      | (0x342U == 
                                         (0xfffU & (IData)(
                                                           (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                            >> 0x20U))))) 
                                     | (0x344U == (0xfffU 
                                                   & (IData)(
                                                             (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                              >> 0x20U))))) 
                                    | (0x304U == (0xfffU 
                                                  & (IData)(
                                                            (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                             >> 0x20U))))) 
                                   | (0x343U == (0xfffU 
                                                 & (IData)(
                                                           (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                            >> 0x20U))))) 
                                  | (0x340U == (0xfffU 
                                                & (IData)(
                                                          (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                           >> 0x20U)))))))) {
                        if ((0xb00U == (0xfffU & (IData)(
                                                         (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                          >> 0x20U))))) {
                            __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 
                                = ((0xffffffff00000000ULL 
                                    & __Vdly__core_top__DOT__u_csr__DOT__mcycle_64) 
                                   | (IData)((IData)(vlSelfRef.core_top__DOT__mem_wb_csr_wdata)));
                        } else if ((0xb80U == (0xfffU 
                                               & (IData)(
                                                         (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                          >> 0x20U))))) {
                            __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 
                                = ((0xffffffffULL & __Vdly__core_top__DOT__u_csr__DOT__mcycle_64) 
                                   | ((QData)((IData)(vlSelfRef.core_top__DOT__mem_wb_csr_wdata)) 
                                      << 0x20U));
                        }
                    }
                }
            }
        }
        vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64 
            = __Vdly__core_top__DOT__u_csr__DOT__mcycle_64;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip 
            = __Vdly__core_top__DOT__u_csr__DOT__csr_mip;
        if (vlSelfRef.core_top__DOT__is_ebreak_eff) {
            vlSelfRef.halt = 1U;
            vlSelfRef.halt_ret = vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                [0xaU];
        }
    }
    if (__VdlySet__core_top__DOT__u_regfile__DOT__rf__v0) {
        vlSelfRef.core_top__DOT__u_regfile__DOT__rf[__VdlyDim0__core_top__DOT__u_regfile__DOT__rf__v0] 
            = __VdlyVal__core_top__DOT__u_regfile__DOT__rf__v0;
    }
    if (vlSelfRef.rst) {
        vlSelfRef.core_top__DOT__id_ex_imm_out = 0U;
        vlSelfRef.core_top__DOT__id_ex_rs1_rdata = 0U;
        vlSelfRef.core_top__DOT__mem_wb_sys_ctrl = 0ULL;
        vlSelfRef.core_top__DOT__id_ex_ex_ctrl = 0U;
        vlSelfRef.halt_pc = 0x80000000U;
        vlSelfRef.core_top__DOT__id_ex_rs2_addr = 0U;
        vlSelfRef.core_top__DOT__id_ex_rs1_addr = 0U;
        vlSelfRef.core_top__DOT__mem_wb_inst = 0x13U;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mscratch = 0U;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtval = 0U;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mie = 0U;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause = 0U;
    } else {
        vlSelfRef.core_top__DOT__id_ex_imm_out = vlSelfRef.core_top__DOT__imm_out;
        vlSelfRef.core_top__DOT__id_ex_rs1_rdata = vlSelfRef.core_top__DOT__rs1_rdata;
        vlSelfRef.core_top__DOT__mem_wb_sys_ctrl = vlSelfRef.core_top__DOT__ex_mem_sys_ctrl;
        vlSelfRef.core_top__DOT__id_ex_ex_ctrl = (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op) 
                                                   << 3U) 
                                                  | (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_a) 
                                                      << 2U) 
                                                     | (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b)));
        if (vlSelfRef.core_top__DOT__is_ebreak_eff) {
            vlSelfRef.halt_pc = vlSelfRef.core_top__DOT__mem_wb_pc;
        }
        vlSelfRef.core_top__DOT__id_ex_rs2_addr = (0x1fU 
                                                   & (vlSelfRef.core_top__DOT__if_id_inst 
                                                      >> 0x14U));
        vlSelfRef.core_top__DOT__id_ex_rs1_addr = (0x1fU 
                                                   & (vlSelfRef.core_top__DOT__if_id_inst 
                                                      >> 0xfU));
        vlSelfRef.core_top__DOT__mem_wb_inst = vlSelfRef.core_top__DOT__ex_mem_inst;
        if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter)))) {
            if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__u_csr__DOT__mret)))) {
                if (vlSelfRef.core_top__DOT__u_csr__DOT__csr_write) {
                    if (((((((((0x300U == (0xfffU & (IData)(
                                                            (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                             >> 0x20U)))) 
                               | (0x305U == (0xfffU 
                                             & (IData)(
                                                       (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                        >> 0x20U))))) 
                              | (0x341U == (0xfffU 
                                            & (IData)(
                                                      (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                       >> 0x20U))))) 
                             | (0x342U == (0xfffU & (IData)(
                                                            (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                             >> 0x20U))))) 
                            | (0x344U == (0xfffU & (IData)(
                                                           (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                            >> 0x20U))))) 
                           | (0x304U == (0xfffU & (IData)(
                                                          (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                           >> 0x20U))))) 
                          | (0x343U == (0xfffU & (IData)(
                                                         (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                          >> 0x20U))))) 
                         | (0x340U == (0xfffU & (IData)(
                                                        (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                         >> 0x20U)))))) {
                        if ((0x300U != (0xfffU & (IData)(
                                                         (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                          >> 0x20U))))) {
                            if ((0x305U != (0xfffU 
                                            & (IData)(
                                                      (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                       >> 0x20U))))) {
                                if ((0x341U != (0xfffU 
                                                & (IData)(
                                                          (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                           >> 0x20U))))) {
                                    if ((0x342U != 
                                         (0xfffU & (IData)(
                                                           (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                            >> 0x20U))))) {
                                        if ((0x344U 
                                             != (0xfffU 
                                                 & (IData)(
                                                           (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                            >> 0x20U))))) {
                                            if ((0x304U 
                                                 != 
                                                 (0xfffU 
                                                  & (IData)(
                                                            (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                             >> 0x20U))))) {
                                                if (
                                                    (0x343U 
                                                     != 
                                                     (0xfffU 
                                                      & (IData)(
                                                                (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                                 >> 0x20U))))) {
                                                    vlSelfRef.core_top__DOT__u_csr__DOT__csr_mscratch 
                                                        = vlSelfRef.core_top__DOT__mem_wb_csr_wdata;
                                                }
                                                if (
                                                    (0x343U 
                                                     == 
                                                     (0xfffU 
                                                      & (IData)(
                                                                (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                                 >> 0x20U))))) {
                                                    vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtval 
                                                        = vlSelfRef.core_top__DOT__mem_wb_csr_wdata;
                                                }
                                            }
                                            if ((0x304U 
                                                 == 
                                                 (0xfffU 
                                                  & (IData)(
                                                            (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                             >> 0x20U))))) {
                                                vlSelfRef.core_top__DOT__u_csr__DOT__csr_mie 
                                                    = vlSelfRef.core_top__DOT__mem_wb_csr_wdata;
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
        if (vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter) {
            vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause 
                = vlSelfRef.core_top__DOT__u_csr__DOT__trap_code;
        } else if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__u_csr__DOT__mret)))) {
            if (vlSelfRef.core_top__DOT__u_csr__DOT__csr_write) {
                if (((((((((0x300U == (0xfffU & (IData)(
                                                        (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                         >> 0x20U)))) 
                           | (0x305U == (0xfffU & (IData)(
                                                          (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                           >> 0x20U))))) 
                          | (0x341U == (0xfffU & (IData)(
                                                         (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                          >> 0x20U))))) 
                         | (0x342U == (0xfffU & (IData)(
                                                        (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                         >> 0x20U))))) 
                        | (0x344U == (0xfffU & (IData)(
                                                       (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                        >> 0x20U))))) 
                       | (0x304U == (0xfffU & (IData)(
                                                      (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                       >> 0x20U))))) 
                      | (0x343U == (0xfffU & (IData)(
                                                     (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                      >> 0x20U))))) 
                     | (0x340U == (0xfffU & (IData)(
                                                    (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                     >> 0x20U)))))) {
                    if ((0x300U != (0xfffU & (IData)(
                                                     (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                      >> 0x20U))))) {
                        if ((0x305U != (0xfffU & (IData)(
                                                         (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                          >> 0x20U))))) {
                            if ((0x341U != (0xfffU 
                                            & (IData)(
                                                      (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                       >> 0x20U))))) {
                                if ((0x342U == (0xfffU 
                                                & (IData)(
                                                          (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                           >> 0x20U))))) {
                                    vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause 
                                        = vlSelfRef.core_top__DOT__mem_wb_csr_wdata;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.core_top__DOT__u_csr__DOT__trap_code 
        = ((IData)(vlSelfRef.interrupt_valid) ? vlSelfRef.interrupt_cause
            : (IData)(vlSelfRef.core_top__DOT__mem_wb_sys_ctrl));
    if (vlSelfRef.rst) {
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec = 0x80000000U;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc = 0U;
    } else {
        if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter)))) {
            if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__u_csr__DOT__mret)))) {
                if (vlSelfRef.core_top__DOT__u_csr__DOT__csr_write) {
                    if (((((((((0x300U == (0xfffU & (IData)(
                                                            (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                             >> 0x20U)))) 
                               | (0x305U == (0xfffU 
                                             & (IData)(
                                                       (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                        >> 0x20U))))) 
                              | (0x341U == (0xfffU 
                                            & (IData)(
                                                      (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                       >> 0x20U))))) 
                             | (0x342U == (0xfffU & (IData)(
                                                            (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                             >> 0x20U))))) 
                            | (0x344U == (0xfffU & (IData)(
                                                           (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                            >> 0x20U))))) 
                           | (0x304U == (0xfffU & (IData)(
                                                          (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                           >> 0x20U))))) 
                          | (0x343U == (0xfffU & (IData)(
                                                         (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                          >> 0x20U))))) 
                         | (0x340U == (0xfffU & (IData)(
                                                        (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                         >> 0x20U)))))) {
                        if ((0x300U != (0xfffU & (IData)(
                                                         (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                          >> 0x20U))))) {
                            if ((0x305U == (0xfffU 
                                            & (IData)(
                                                      (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                       >> 0x20U))))) {
                                vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec 
                                    = vlSelfRef.core_top__DOT__mem_wb_csr_wdata;
                            }
                        }
                    }
                }
            }
        }
        if (vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter) {
            vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc 
                = vlSelfRef.core_top__DOT__mem_wb_pc;
        } else if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__u_csr__DOT__mret)))) {
            if (vlSelfRef.core_top__DOT__u_csr__DOT__csr_write) {
                if (((((((((0x300U == (0xfffU & (IData)(
                                                        (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                         >> 0x20U)))) 
                           | (0x305U == (0xfffU & (IData)(
                                                          (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                           >> 0x20U))))) 
                          | (0x341U == (0xfffU & (IData)(
                                                         (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                          >> 0x20U))))) 
                         | (0x342U == (0xfffU & (IData)(
                                                        (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                         >> 0x20U))))) 
                        | (0x344U == (0xfffU & (IData)(
                                                       (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                        >> 0x20U))))) 
                       | (0x304U == (0xfffU & (IData)(
                                                      (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                       >> 0x20U))))) 
                      | (0x343U == (0xfffU & (IData)(
                                                     (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                      >> 0x20U))))) 
                     | (0x340U == (0xfffU & (IData)(
                                                    (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                     >> 0x20U)))))) {
                    if ((0x300U != (0xfffU & (IData)(
                                                     (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                      >> 0x20U))))) {
                        if ((0x305U != (0xfffU & (IData)(
                                                         (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                          >> 0x20U))))) {
                            if ((0x341U == (0xfffU 
                                            & (IData)(
                                                      (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                       >> 0x20U))))) {
                                vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc 
                                    = vlSelfRef.core_top__DOT__mem_wb_csr_wdata;
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter 
        = ((IData)(vlSelfRef.interrupt_valid) | ((IData)(vlSelfRef.core_top__DOT__mem_wb_valid) 
                                                 & (IData)(
                                                           (vlSelfRef.core_top__DOT__mem_wb_sys_ctrl 
                                                            >> 0x22U))));
    vlSelfRef.core_top__DOT__u_csr__DOT____VdfgRegularize_h8136f0d8_0_0 
        = ((~ (IData)(vlSelfRef.interrupt_valid)) & (IData)(vlSelfRef.core_top__DOT__mem_wb_valid));
    vlSelfRef.core_top__DOT__ex_mem_valid = ((1U & 
                                              (~ (IData)(vlSelfRef.rst))) 
                                             && (IData)(vlSelfRef.core_top__DOT__id_ex_valid));
    vlSelfRef.core_top__DOT__is_ebreak_eff = ((IData)(vlSelfRef.core_top__DOT__u_csr__DOT____VdfgRegularize_h8136f0d8_0_0) 
                                              & (IData)(
                                                        (vlSelfRef.core_top__DOT__mem_wb_sys_ctrl 
                                                         >> 0x20U)));
    vlSelfRef.core_top__DOT__u_csr__DOT__mret = ((IData)(vlSelfRef.core_top__DOT__u_csr__DOT____VdfgRegularize_h8136f0d8_0_0) 
                                                 & (IData)(
                                                           (vlSelfRef.core_top__DOT__mem_wb_sys_ctrl 
                                                            >> 0x21U)));
    vlSelfRef.core_top__DOT__id_ex_valid = ((1U & (~ (IData)(vlSelfRef.rst))) 
                                            && (IData)(vlSelfRef.core_top__DOT__if_id_valid));
    if (vlSelfRef.rst) {
        vlSelfRef.core_top__DOT__ex_mem_rs2_rdata = 0U;
        vlSelfRef.core_top__DOT__mem_wb_csr_rdata = 0U;
        vlSelfRef.core_top__DOT__mem_wb_alu_result = 0U;
        vlSelfRef.core_top__DOT__mem_wb_wb_ctrl = 0U;
        vlSelfRef.core_top__DOT__mem_wb_mem_rdata = 0U;
        vlSelfRef.core_top__DOT__mem_wb_pc_plus4 = 0x80000004U;
        vlSelfRef.core_top__DOT__mem_wb_rd_addr = 0U;
        vlSelfRef.core_top__DOT__ex_mem_sys_ctrl = 0ULL;
        vlSelfRef.core_top__DOT__ex_mem_inst = 0x13U;
        vlSelfRef.core_top__DOT__mem_wb_pc = 0x80000000U;
        vlSelfRef.core_top__DOT__mem_wb_csr_ctrl = 0ULL;
    } else {
        vlSelfRef.core_top__DOT__ex_mem_rs2_rdata = vlSelfRef.core_top__DOT__id_ex_rs2_rdata;
        vlSelfRef.core_top__DOT__mem_wb_csr_rdata = vlSelfRef.core_top__DOT__ex_mem_csr_rdata;
        vlSelfRef.core_top__DOT__mem_wb_alu_result 
            = vlSelfRef.core_top__DOT__ex_mem_alu_result;
        vlSelfRef.core_top__DOT__mem_wb_wb_ctrl = vlSelfRef.core_top__DOT__ex_mem_wb_ctrl;
        vlSelfRef.core_top__DOT__mem_wb_mem_rdata = 
            ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_read)
              ? ((0U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                  ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw
                  : ((1U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                      ? (((- (IData)((1U & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                            >> 0xfU)))) 
                          << 0x10U) | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))
                      : ((2U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                          ? (((- (IData)((1U & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected) 
                                                >> 7U)))) 
                              << 8U) | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected))
                          : ((3U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                              ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected)
                              : ((4U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                  ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected)
                                  : 0U))))) : 0U);
        vlSelfRef.core_top__DOT__mem_wb_pc_plus4 = vlSelfRef.core_top__DOT__ex_mem_pc_plus4;
        vlSelfRef.core_top__DOT__mem_wb_rd_addr = vlSelfRef.core_top__DOT__ex_mem_rd_addr;
        vlSelfRef.core_top__DOT__ex_mem_sys_ctrl = vlSelfRef.core_top__DOT__id_ex_sys_ctrl;
        vlSelfRef.core_top__DOT__ex_mem_inst = vlSelfRef.core_top__DOT__id_ex_inst;
        vlSelfRef.core_top__DOT__mem_wb_pc = vlSelfRef.core_top__DOT__ex_mem_pc;
        vlSelfRef.core_top__DOT__mem_wb_csr_ctrl = vlSelfRef.core_top__DOT__ex_mem_csr_ctrl;
    }
    vlSelfRef.core_top__DOT__u_csr__DOT__csr_write 
        = ((IData)(vlSelfRef.core_top__DOT__u_csr__DOT____VdfgRegularize_h8136f0d8_0_0) 
           & (IData)((vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                      >> 0x2fU)));
    if (vlSelfRef.rst) {
        vlSelfRef.core_top__DOT__mem_wb_csr_wdata = 0U;
        vlSelfRef.core_top__DOT__id_ex_rs2_rdata = 0U;
    } else {
        vlSelfRef.core_top__DOT__mem_wb_csr_wdata = vlSelfRef.core_top__DOT__ex_mem_csr_wdata;
        vlSelfRef.core_top__DOT__id_ex_rs2_rdata = vlSelfRef.core_top__DOT__rs2_rdata;
    }
    vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2 
        = ((0U == (3U & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl)))
            ? vlSelfRef.core_top__DOT__id_ex_rs2_rdata
            : ((1U == (3U & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl)))
                ? vlSelfRef.core_top__DOT__id_ex_imm_out
                : 4U));
    if (vlSelfRef.rst) {
        vlSelfRef.core_top__DOT__ex_mem_csr_rdata = 0U;
        vlSelfRef.core_top__DOT__ex_mem_alu_result = 0U;
        vlSelfRef.core_top__DOT__ex_mem_wb_ctrl = 0U;
        vlSelfRef.core_top__DOT__ex_mem_mem_ctrl = 0U;
    } else {
        vlSelfRef.core_top__DOT__ex_mem_csr_rdata = vlSelfRef.core_top__DOT__csr_rdata;
        vlSelfRef.core_top__DOT__ex_mem_alu_result 
            = vlSelfRef.core_top__DOT__alu_result;
        vlSelfRef.core_top__DOT__ex_mem_wb_ctrl = vlSelfRef.core_top__DOT__id_ex_wb_ctrl;
        vlSelfRef.core_top__DOT__ex_mem_mem_ctrl = vlSelfRef.core_top__DOT__id_ex_mem_ctrl;
    }
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write 
        = ((IData)(vlSelfRef.core_top__DOT__ex_mem_valid) 
           & ((~ (IData)(vlSelfRef.interrupt_valid)) 
              & ((IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl) 
                 >> 3U)));
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_read 
        = ((IData)(vlSelfRef.core_top__DOT__ex_mem_valid) 
           & ((~ (IData)(vlSelfRef.interrupt_valid)) 
              & ((IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl) 
                 >> 4U)));
    vlSelfRef.core_top__DOT__if_id_valid = (1U & (~ (IData)(vlSelfRef.rst)));
    if (vlSelfRef.rst) {
        vlSelfRef.core_top__DOT__ex_mem_pc_plus4 = 0x80000004U;
        vlSelfRef.core_top__DOT__ex_mem_rd_addr = 0U;
        vlSelfRef.core_top__DOT__id_ex_sys_ctrl = 0ULL;
        vlSelfRef.core_top__DOT__id_ex_inst = 0x13U;
        vlSelfRef.core_top__DOT__ex_mem_pc = 0x80000000U;
        vlSelfRef.core_top__DOT__ex_mem_csr_ctrl = 0ULL;
        vlSelfRef.core_top__DOT__ex_mem_csr_wdata = 0U;
        vlSelfRef.core_top__DOT__id_ex_wb_ctrl = 0U;
        vlSelfRef.core_top__DOT__id_ex_mem_ctrl = 0U;
        vlSelfRef.core_top__DOT__id_ex_pc_plus4 = 0x80000004U;
        vlSelfRef.core_top__DOT__id_ex_rd_addr = 0U;
        vlSelfRef.core_top__DOT__id_ex_pc = 0x80000000U;
        vlSelfRef.core_top__DOT__id_ex_csr_ctrl = 0ULL;
    } else {
        vlSelfRef.core_top__DOT__ex_mem_pc_plus4 = vlSelfRef.core_top__DOT__id_ex_pc_plus4;
        vlSelfRef.core_top__DOT__ex_mem_rd_addr = vlSelfRef.core_top__DOT__id_ex_rd_addr;
        vlSelfRef.core_top__DOT__id_ex_sys_ctrl = (
                                                   ((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_enter_int)) 
                                                    << 0x22U) 
                                                   | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int)) 
                                                       << 0x21U) 
                                                      | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_ebreak_int)) 
                                                          << 0x20U) 
                                                         | (QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_code_int)))));
        vlSelfRef.core_top__DOT__id_ex_inst = vlSelfRef.core_top__DOT__if_id_inst;
        vlSelfRef.core_top__DOT__ex_mem_pc = vlSelfRef.core_top__DOT__id_ex_pc;
        vlSelfRef.core_top__DOT__ex_mem_csr_ctrl = vlSelfRef.core_top__DOT__id_ex_csr_ctrl;
        vlSelfRef.core_top__DOT__ex_mem_csr_wdata = 
            ((0U == (3U & (IData)((vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                   >> 0x2dU)))) ? vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src
              : ((1U == (3U & (IData)((vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                       >> 0x2dU))))
                  ? (vlSelfRef.core_top__DOT__csr_rdata 
                     | vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src)
                  : ((2U == (3U & (IData)((vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                           >> 0x2dU))))
                      ? ((~ vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src) 
                         & vlSelfRef.core_top__DOT__csr_rdata)
                      : 0U)));
        vlSelfRef.core_top__DOT__id_ex_wb_ctrl = (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write) 
                                                   << 2U) 
                                                  | (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src));
        vlSelfRef.core_top__DOT__id_ex_mem_ctrl = (
                                                   ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_read) 
                                                    << 4U) 
                                                   | (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_write) 
                                                       << 3U) 
                                                      | (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type)));
        vlSelfRef.core_top__DOT__id_ex_pc_plus4 = vlSelfRef.core_top__DOT__if_id_pc_plus4;
        vlSelfRef.core_top__DOT__id_ex_rd_addr = (0x1fU 
                                                  & (vlSelfRef.core_top__DOT__if_id_inst 
                                                     >> 7U));
        vlSelfRef.core_top__DOT__id_ex_pc = vlSelfRef.core_top__DOT__if_id_pc;
        vlSelfRef.core_top__DOT__id_ex_csr_ctrl = (
                                                   ((QData)((IData)(
                                                                    (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_read) 
                                                                      << 1U) 
                                                                     | (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_write)))) 
                                                    << 0x2fU) 
                                                   | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_op)) 
                                                       << 0x2dU) 
                                                      | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_imm)) 
                                                          << 0x2cU) 
                                                         | (((QData)((IData)(
                                                                             (vlSelfRef.core_top__DOT__if_id_inst 
                                                                              >> 0x14U))) 
                                                             << 0x20U) 
                                                            | (QData)((IData)(
                                                                              (0x1fU 
                                                                               & (vlSelfRef.core_top__DOT__if_id_inst 
                                                                                >> 0xfU))))))));
    }
    Vcore_top___024unit____Vdpiimwrap_dpi_mem_read_TOP____024unit(vlSelfRef.core_top__DOT__ex_mem_alu_result, (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_read), vlSelfRef.__Vfunc_dpi_mem_read__5__Vfuncout);
    if (vlSelfRef.rst) {
        vlSelfRef.core_top__DOT__if_id_pc_plus4 = 0x80000004U;
        vlSelfRef.core_top__DOT__if_id_pc = 0x80000000U;
        vlSelfRef.core_top__DOT__if_id_inst = 0x13U;
        vlSelfRef.core_top__DOT__pc = 0x80000000U;
    } else {
        vlSelfRef.core_top__DOT__if_id_pc_plus4 = ((IData)(4U) 
                                                   + vlSelfRef.core_top__DOT__pc);
        vlSelfRef.core_top__DOT__if_id_pc = vlSelfRef.core_top__DOT__pc;
        vlSelfRef.core_top__DOT__if_id_inst = vlSelfRef.instr;
        vlSelfRef.core_top__DOT__pc = vlSelfRef.core_top__DOT__u_if_stage__DOT__pc_next;
    }
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
        = vlSelfRef.__Vfunc_dpi_mem_read__5__Vfuncout;
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected 
        = (0xffU & ((0U == (3U & vlSelfRef.core_top__DOT__ex_mem_alu_result))
                     ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw
                     : ((1U == (3U & vlSelfRef.core_top__DOT__ex_mem_alu_result))
                         ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                            >> 8U) : ((2U == (3U & vlSelfRef.core_top__DOT__ex_mem_alu_result))
                                       ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 0x10U)
                                       : (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 0x18U)))));
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected 
        = (0xffffU & ((2U & vlSelfRef.core_top__DOT__ex_mem_alu_result)
                       ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                          >> 0x10U) : vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw));
    vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
        = ((4U & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))
            ? vlSelfRef.core_top__DOT__id_ex_pc : vlSelfRef.core_top__DOT__id_ex_rs1_rdata);
    vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src 
        = ((1U & (IData)((vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                          >> 0x2cU))) ? vlSelfRef.core_top__DOT__id_ex_rs1_rdata
            : (IData)(vlSelfRef.core_top__DOT__id_ex_csr_ctrl));
    vlSelfRef.core_top__DOT__csr_rdata = ((1U & (IData)(
                                                        (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                         >> 0x30U)))
                                           ? ((0x300U 
                                               == (0xfffU 
                                                   & (IData)(
                                                             (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                              >> 0x20U))))
                                               ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus
                                               : ((0x305U 
                                                   == 
                                                   (0xfffU 
                                                    & (IData)(
                                                              (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                               >> 0x20U))))
                                                   ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                                                   : 
                                                  ((0x341U 
                                                    == 
                                                    (0xfffU 
                                                     & (IData)(
                                                               (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                                >> 0x20U))))
                                                    ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                                                    : 
                                                   ((0x342U 
                                                     == 
                                                     (0xfffU 
                                                      & (IData)(
                                                                (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                                 >> 0x20U))))
                                                     ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause
                                                     : 
                                                    ((0x344U 
                                                      == 
                                                      (0xfffU 
                                                       & (IData)(
                                                                 (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                                  >> 0x20U))))
                                                      ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip
                                                      : 
                                                     ((0x304U 
                                                       == 
                                                       (0xfffU 
                                                        & (IData)(
                                                                  (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                                   >> 0x20U))))
                                                       ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mie
                                                       : 
                                                      ((0x343U 
                                                        == 
                                                        (0xfffU 
                                                         & (IData)(
                                                                   (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                                    >> 0x20U))))
                                                        ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtval
                                                        : 
                                                       ((0x340U 
                                                         == 
                                                         (0xfffU 
                                                          & (IData)(
                                                                    (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                                     >> 0x20U))))
                                                         ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mscratch
                                                         : 
                                                        ((0xb00U 
                                                          == 
                                                          (0xfffU 
                                                           & (IData)(
                                                                     (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                                      >> 0x20U))))
                                                          ? (IData)(vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64)
                                                          : 
                                                         ((0xb80U 
                                                           == 
                                                           (0xfffU 
                                                            & (IData)(
                                                                      (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                                       >> 0x20U))))
                                                           ? (IData)(
                                                                     (vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64 
                                                                      >> 0x20U))
                                                           : 0U))))))))))
                                           : 0U);
    vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__src1_s64 
        = (((QData)((IData)((- (IData)((vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                        >> 0x1fU))))) 
            << 0x20U) | (QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1)));
    vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__mul_uu 
        = ((QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1)) 
           * (QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)));
    if ((0x80U & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))) {
        if ((0x40U & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))) {
            vlSelfRef.core_top__DOT__alu_result = 0U;
        } else if ((0x20U & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))) {
            if ((0x10U & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))) {
                if ((8U & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))) {
                    vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_rem32__1__divisor 
                        = vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2;
                    vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_rem32__1__dividend 
                        = vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1;
                    vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_rem32__1__Vfuncout 
                        = ((0U == vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_rem32__1__divisor)
                            ? vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_rem32__1__dividend
                            : VL_MODDIV_III(32, vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_rem32__1__dividend, vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_rem32__1__divisor));
                    vlSelfRef.core_top__DOT__alu_result 
                        = vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_rem32__1__Vfuncout;
                } else {
                    vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__divisor 
                        = vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2;
                    vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__dividend 
                        = vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1;
                    if ((0U == vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__divisor)) {
                        vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__Vfuncout 
                            = vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__dividend;
                    } else if (((0x80000000U == vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__dividend) 
                                & (0xffffffffU == vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__divisor))) {
                        vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__Vfuncout = 0U;
                    } else {
                        vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__abs_divisor 
                            = ((vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__divisor 
                                >> 0x1fU) ? ((IData)(1U) 
                                             + (~ vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__divisor))
                                : vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__divisor);
                        vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__abs_dividend 
                            = ((vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__dividend 
                                >> 0x1fU) ? ((IData)(1U) 
                                             + (~ vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__dividend))
                                : vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__dividend);
                        vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__remainder 
                            = VL_MODDIV_III(32, vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__abs_dividend, vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__abs_divisor);
                        vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__Vfuncout 
                            = ((vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__dividend 
                                >> 0x1fU) ? ((IData)(1U) 
                                             + (~ vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__remainder))
                                : vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__remainder);
                    }
                    vlSelfRef.core_top__DOT__alu_result 
                        = vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__2__Vfuncout;
                }
            } else if ((8U & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))) {
                vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_div32__3__divisor 
                    = vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2;
                vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_div32__3__dividend 
                    = vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1;
                vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_div32__3__Vfuncout 
                    = ((0U == vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_div32__3__divisor)
                        ? 0xffffffffU : VL_DIV_III(32, vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_div32__3__dividend, vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_div32__3__divisor));
                vlSelfRef.core_top__DOT__alu_result 
                    = vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__unsigned_div32__3__Vfuncout;
            } else {
                vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__divisor 
                    = vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2;
                vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__dividend 
                    = vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1;
                if ((0U == vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__divisor)) {
                    vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__Vfuncout = 0xffffffffU;
                } else if (((0x80000000U == vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__dividend) 
                            & (0xffffffffU == vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__divisor))) {
                    vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__Vfuncout = 0x80000000U;
                } else {
                    vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__abs_dividend 
                        = ((vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__dividend 
                            >> 0x1fU) ? ((IData)(1U) 
                                         + (~ vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__dividend))
                            : vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__dividend);
                    vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__abs_divisor 
                        = ((vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__divisor 
                            >> 0x1fU) ? ((IData)(1U) 
                                         + (~ vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__divisor))
                            : vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__divisor);
                    vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__quotient 
                        = VL_DIV_III(32, vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__abs_dividend, vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__abs_divisor);
                    vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__Vfuncout 
                        = (((vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__dividend 
                             ^ vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__divisor) 
                            >> 0x1fU) ? ((IData)(1U) 
                                         + (~ vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__quotient))
                            : vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__quotient);
                }
                vlSelfRef.core_top__DOT__alu_result 
                    = vlSelfRef.__Vfunc_core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__4__Vfuncout;
            }
        } else {
            vlSelfRef.core_top__DOT__alu_result = (
                                                   (0x10U 
                                                    & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))
                                                    ? 
                                                   ((8U 
                                                     & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))
                                                     ? (IData)(
                                                               (vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__mul_uu 
                                                                >> 0x20U))
                                                     : (IData)(
                                                               ((vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__src1_s64 
                                                                 * (QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))) 
                                                                >> 0x20U)))
                                                    : 
                                                   ((8U 
                                                     & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))
                                                     ? (IData)(
                                                               (VL_MULS_QQQ(64, vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__src1_s64, 
                                                                            (((QData)((IData)(
                                                                                (- (IData)(
                                                                                (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2 
                                                                                >> 0x1fU))))) 
                                                                              << 0x20U) 
                                                                             | (QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)))) 
                                                                >> 0x20U))
                                                     : (IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__mul_uu)));
        }
    } else {
        vlSelfRef.core_top__DOT__alu_result = ((0x40U 
                                                & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))
                                                ? (
                                                   (0x20U 
                                                    & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))
                                                    ? 0U
                                                    : 
                                                   ((0x10U 
                                                     & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))
                                                     ? 
                                                    ((8U 
                                                      & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))
                                                      ? 0U
                                                      : vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)
                                                     : 
                                                    ((8U 
                                                      & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))
                                                      ? 
                                                     VL_SHIFTRS_III(32,32,5, vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1, 
                                                                    (0x1fU 
                                                                     & vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))
                                                      : 
                                                     (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                      >> 
                                                      (0x1fU 
                                                       & vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)))))
                                                : (
                                                   (0x20U 
                                                    & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))
                                                    ? 
                                                   ((0x10U 
                                                     & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))
                                                     ? 
                                                    ((8U 
                                                      & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))
                                                      ? 
                                                     (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                      & vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)
                                                      : 
                                                     (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                      | vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))
                                                     : 
                                                    ((8U 
                                                      & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))
                                                      ? 
                                                     (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                      ^ vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)
                                                      : 
                                                     ((vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                       < vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)
                                                       ? 1U
                                                       : 0U)))
                                                    : 
                                                   ((0x10U 
                                                     & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))
                                                     ? 
                                                    ((8U 
                                                      & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))
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
                                                    ((8U 
                                                      & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))
                                                      ? 
                                                     (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                      + vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)
                                                      : 
                                                     (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                      - vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)))));
    }
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src = 0U;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write = 0U;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_write = 0U;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_a = 0U;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op = 0U;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b = 0U;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_write = 0U;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_read = 0U;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_op = 0U;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type = 0U;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_read = 0U;
    if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                  >> 6U)))) {
        if ((0x20U & vlSelfRef.core_top__DOT__if_id_inst)) {
            if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                          >> 4U)))) {
                if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                              >> 3U)))) {
                    if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                                  >> 2U)))) {
                        if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                            if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                                vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_write = 1U;
                                if ((2U == (7U & (vlSelfRef.core_top__DOT__if_id_inst 
                                                  >> 0xcU)))) {
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type = 5U;
                                } else if ((1U == (7U 
                                                   & (vlSelfRef.core_top__DOT__if_id_inst 
                                                      >> 0xcU)))) {
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type = 6U;
                                } else if ((0U == (7U 
                                                   & (vlSelfRef.core_top__DOT__if_id_inst 
                                                      >> 0xcU)))) {
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type = 7U;
                                }
                            }
                        }
                    }
                }
            }
        } else if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                             >> 4U)))) {
            if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                          >> 3U)))) {
                if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                              >> 2U)))) {
                    if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                        if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                            if ((0x4000U & vlSelfRef.core_top__DOT__if_id_inst)) {
                                if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                                              >> 0xdU)))) {
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type 
                                        = ((0x1000U 
                                            & vlSelfRef.core_top__DOT__if_id_inst)
                                            ? 3U : 4U);
                                }
                            } else if ((0x2000U & vlSelfRef.core_top__DOT__if_id_inst)) {
                                if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                                              >> 0xcU)))) {
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type = 0U;
                                }
                            } else {
                                vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type 
                                    = ((0x1000U & vlSelfRef.core_top__DOT__if_id_inst)
                                        ? 1U : 2U);
                            }
                        }
                    }
                }
            }
        }
        if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                      >> 5U)))) {
            if ((0x10U & vlSelfRef.core_top__DOT__if_id_inst)) {
                if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                              >> 3U)))) {
                    if ((4U & vlSelfRef.core_top__DOT__if_id_inst)) {
                        if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                            if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                                vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_a = 1U;
                            }
                        }
                    }
                }
            }
            if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                          >> 4U)))) {
                if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                              >> 3U)))) {
                    if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                                  >> 2U)))) {
                        if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                            if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                                vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_read = 1U;
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_imm = 0U;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_ebreak_int = 0U;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_code_int = 0U;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel = 0U;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type = 0U;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int = 0U;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jal_int = 0U;
    vlSelfRef.core_top__DOT__rs2_rdata = ((0U == (0x1fU 
                                                  & (vlSelfRef.core_top__DOT__if_id_inst 
                                                     >> 0x14U)))
                                           ? 0U : vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                                          [(0x1fU & 
                                            (vlSelfRef.core_top__DOT__if_id_inst 
                                             >> 0x14U))]);
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_enter_int = 0U;
    if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
        if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
            vlSelfRef.__VdfgRegularize_h495687df_0_8 
                = (0xfffff000U & vlSelfRef.core_top__DOT__if_id_inst);
            vlSelfRef.__VdfgRegularize_h495687df_0_6 
                = (((- (IData)((vlSelfRef.core_top__DOT__if_id_inst 
                                >> 0x1fU))) << 0xcU) 
                   | (vlSelfRef.core_top__DOT__if_id_inst 
                      >> 0x14U));
        } else {
            vlSelfRef.__VdfgRegularize_h495687df_0_8 = 0U;
            vlSelfRef.__VdfgRegularize_h495687df_0_6 = 0U;
        }
    } else {
        vlSelfRef.__VdfgRegularize_h495687df_0_8 = 0U;
        vlSelfRef.__VdfgRegularize_h495687df_0_6 = 0U;
    }
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int = 0U;
    vlSelfRef.core_top__DOT__rs1_rdata = ((0U == (0x1fU 
                                                  & (vlSelfRef.core_top__DOT__if_id_inst 
                                                     >> 0xfU)))
                                           ? 0U : vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                                          [(0x1fU & 
                                            (vlSelfRef.core_top__DOT__if_id_inst 
                                             >> 0xfU))]);
    if ((0x40U & vlSelfRef.core_top__DOT__if_id_inst)) {
        if ((0x20U & vlSelfRef.core_top__DOT__if_id_inst)) {
            if ((0x10U & vlSelfRef.core_top__DOT__if_id_inst)) {
                if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                              >> 3U)))) {
                    if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                                  >> 2U)))) {
                        if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                            if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                                if ((0x4000U & vlSelfRef.core_top__DOT__if_id_inst)) {
                                    if ((0x2000U & vlSelfRef.core_top__DOT__if_id_inst)) {
                                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src = 3U;
                                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write = 1U;
                                        if ((0x1000U 
                                             & vlSelfRef.core_top__DOT__if_id_inst)) {
                                            vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_write 
                                                = (0U 
                                                   != 
                                                   (0x1fU 
                                                    & (vlSelfRef.core_top__DOT__if_id_inst 
                                                       >> 0xfU)));
                                            vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_op = 2U;
                                        } else {
                                            vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_write 
                                                = (0U 
                                                   != 
                                                   (0x1fU 
                                                    & (vlSelfRef.core_top__DOT__if_id_inst 
                                                       >> 0xfU)));
                                            vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_op = 1U;
                                        }
                                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_read = 1U;
                                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_imm = 0U;
                                    } else if ((0x1000U 
                                                & vlSelfRef.core_top__DOT__if_id_inst)) {
                                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src = 3U;
                                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write = 1U;
                                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_write = 1U;
                                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_read = 1U;
                                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_op = 0U;
                                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_imm = 0U;
                                    }
                                } else if ((0x2000U 
                                            & vlSelfRef.core_top__DOT__if_id_inst)) {
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src = 3U;
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write = 1U;
                                    if ((0x1000U & vlSelfRef.core_top__DOT__if_id_inst)) {
                                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_write 
                                            = (0U != 
                                               (0x1fU 
                                                & (vlSelfRef.core_top__DOT__if_id_inst 
                                                   >> 0xfU)));
                                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_op = 2U;
                                    } else {
                                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_write 
                                            = (0U != 
                                               (0x1fU 
                                                & (vlSelfRef.core_top__DOT__if_id_inst 
                                                   >> 0xfU)));
                                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_op = 1U;
                                    }
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_read = 1U;
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_imm = 1U;
                                } else if ((0x1000U 
                                            & vlSelfRef.core_top__DOT__if_id_inst)) {
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src = 3U;
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write = 1U;
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_write = 1U;
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_read = 1U;
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_op = 0U;
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_imm = 1U;
                                }
                                if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                                              >> 0xeU)))) {
                                    if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                                                  >> 0xdU)))) {
                                        if ((1U & (~ 
                                                   (vlSelfRef.core_top__DOT__if_id_inst 
                                                    >> 0xcU)))) {
                                            if ((1U 
                                                 == 
                                                 (vlSelfRef.core_top__DOT__if_id_inst 
                                                  >> 0x14U))) {
                                                vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_ebreak_int = 1U;
                                            }
                                            if ((1U 
                                                 != 
                                                 (vlSelfRef.core_top__DOT__if_id_inst 
                                                  >> 0x14U))) {
                                                if (
                                                    (0U 
                                                     == 
                                                     (vlSelfRef.core_top__DOT__if_id_inst 
                                                      >> 0x14U))) {
                                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_code_int = 0xbU;
                                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_enter_int = 1U;
                                                }
                                                if (
                                                    (0U 
                                                     != 
                                                     (vlSelfRef.core_top__DOT__if_id_inst 
                                                      >> 0x14U))) {
                                                    if (
                                                        (0x302U 
                                                         == 
                                                         (vlSelfRef.core_top__DOT__if_id_inst 
                                                          >> 0x14U))) {
                                                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int = 1U;
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
            } else if ((8U & vlSelfRef.core_top__DOT__if_id_inst)) {
                if ((4U & vlSelfRef.core_top__DOT__if_id_inst)) {
                    if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                        if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                            vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src = 2U;
                            vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write = 1U;
                        }
                    }
                }
            } else if ((4U & vlSelfRef.core_top__DOT__if_id_inst)) {
                if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                    if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src = 2U;
                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write = 1U;
                    }
                }
            }
            if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                          >> 4U)))) {
                if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                              >> 3U)))) {
                    if ((4U & vlSelfRef.core_top__DOT__if_id_inst)) {
                        if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                            if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                                vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op = 1U;
                                vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b = 1U;
                            }
                        }
                    }
                    if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                                  >> 2U)))) {
                        if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                            if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                                vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type = 0U;
                                if ((0x4000U & vlSelfRef.core_top__DOT__if_id_inst)) {
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type 
                                        = ((0x2000U 
                                            & vlSelfRef.core_top__DOT__if_id_inst)
                                            ? ((0x1000U 
                                                & vlSelfRef.core_top__DOT__if_id_inst)
                                                ? 5U
                                                : 4U)
                                            : ((0x1000U 
                                                & vlSelfRef.core_top__DOT__if_id_inst)
                                                ? 3U
                                                : 2U));
                                } else if ((1U & (~ 
                                                  (vlSelfRef.core_top__DOT__if_id_inst 
                                                   >> 0xdU)))) {
                                    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type 
                                        = ((0x1000U 
                                            & vlSelfRef.core_top__DOT__if_id_inst)
                                            ? 1U : 0U);
                                }
                            }
                        }
                    }
                }
                if ((8U & vlSelfRef.core_top__DOT__if_id_inst)) {
                    if ((4U & vlSelfRef.core_top__DOT__if_id_inst)) {
                        if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                            if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                                vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel = 1U;
                                vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jal_int = 1U;
                            }
                        }
                    }
                } else if ((4U & vlSelfRef.core_top__DOT__if_id_inst)) {
                    if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                        if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                            vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel = 2U;
                        }
                    }
                } else if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                    if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel = 3U;
                    }
                }
            }
        }
    } else {
        if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                      >> 5U)))) {
            if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                          >> 4U)))) {
                if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                              >> 3U)))) {
                    if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                                  >> 2U)))) {
                        if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                            if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                                vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src = 1U;
                            }
                        }
                    }
                }
            }
        }
        if ((0x20U & vlSelfRef.core_top__DOT__if_id_inst)) {
            if ((0x10U & vlSelfRef.core_top__DOT__if_id_inst)) {
                if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                              >> 3U)))) {
                    if ((4U & vlSelfRef.core_top__DOT__if_id_inst)) {
                        if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                            if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                                vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write = 1U;
                                vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op = 0xaU;
                                vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b = 1U;
                            }
                        }
                    } else if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                        if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                            vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write = 1U;
                            vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op 
                                = ((1U == (vlSelfRef.core_top__DOT__if_id_inst 
                                           >> 0x19U))
                                    ? ((0x4000U & vlSelfRef.core_top__DOT__if_id_inst)
                                        ? ((0x2000U 
                                            & vlSelfRef.core_top__DOT__if_id_inst)
                                            ? ((0x1000U 
                                                & vlSelfRef.core_top__DOT__if_id_inst)
                                                ? 0x17U
                                                : 0x16U)
                                            : ((0x1000U 
                                                & vlSelfRef.core_top__DOT__if_id_inst)
                                                ? 0x15U
                                                : 0x14U))
                                        : ((0x2000U 
                                            & vlSelfRef.core_top__DOT__if_id_inst)
                                            ? ((0x1000U 
                                                & vlSelfRef.core_top__DOT__if_id_inst)
                                                ? 0x13U
                                                : 0x12U)
                                            : ((0x1000U 
                                                & vlSelfRef.core_top__DOT__if_id_inst)
                                                ? 0x11U
                                                : 0x10U)))
                                    : ((0x4000U & vlSelfRef.core_top__DOT__if_id_inst)
                                        ? ((0x2000U 
                                            & vlSelfRef.core_top__DOT__if_id_inst)
                                            ? ((0x1000U 
                                                & vlSelfRef.core_top__DOT__if_id_inst)
                                                ? 7U
                                                : 6U)
                                            : ((0x1000U 
                                                & vlSelfRef.core_top__DOT__if_id_inst)
                                                ? (
                                                   (0x40000000U 
                                                    & vlSelfRef.core_top__DOT__if_id_inst)
                                                    ? 9U
                                                    : 8U)
                                                : 5U))
                                        : ((0x2000U 
                                            & vlSelfRef.core_top__DOT__if_id_inst)
                                            ? ((0x1000U 
                                                & vlSelfRef.core_top__DOT__if_id_inst)
                                                ? 4U
                                                : 3U)
                                            : ((0x1000U 
                                                & vlSelfRef.core_top__DOT__if_id_inst)
                                                ? 2U
                                                : (
                                                   (0x40000000U 
                                                    & vlSelfRef.core_top__DOT__if_id_inst)
                                                    ? 0U
                                                    : 1U)))));
                        }
                    }
                }
            } else if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                                 >> 3U)))) {
                if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                              >> 2U)))) {
                    if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                        if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                            vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op = 1U;
                            vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b = 1U;
                        }
                    }
                }
            }
        } else if ((0x10U & vlSelfRef.core_top__DOT__if_id_inst)) {
            if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                          >> 3U)))) {
                if ((4U & vlSelfRef.core_top__DOT__if_id_inst)) {
                    if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                        if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                            vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write = 1U;
                            vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op = 1U;
                            vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b = 1U;
                        }
                    }
                } else if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                    if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write = 1U;
                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op 
                            = ((0x4000U & vlSelfRef.core_top__DOT__if_id_inst)
                                ? ((0x2000U & vlSelfRef.core_top__DOT__if_id_inst)
                                    ? ((0x1000U & vlSelfRef.core_top__DOT__if_id_inst)
                                        ? 7U : 6U) : 
                                   ((0x1000U & vlSelfRef.core_top__DOT__if_id_inst)
                                     ? ((0x40000000U 
                                         & vlSelfRef.core_top__DOT__if_id_inst)
                                         ? 9U : 8U)
                                     : 5U)) : ((0x2000U 
                                                & vlSelfRef.core_top__DOT__if_id_inst)
                                                ? (
                                                   (0x1000U 
                                                    & vlSelfRef.core_top__DOT__if_id_inst)
                                                    ? 4U
                                                    : 3U)
                                                : (
                                                   (0x1000U 
                                                    & vlSelfRef.core_top__DOT__if_id_inst)
                                                    ? 2U
                                                    : 1U)));
                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b = 1U;
                    }
                }
            }
        } else if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                             >> 3U)))) {
            if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                          >> 2U)))) {
                if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                    if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write = 1U;
                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op = 1U;
                        vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b = 1U;
                    }
                }
            }
        }
    }
    vlSelfRef.core_top__DOT__u_if_stage__DOT__trap_enter 
        = ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_enter_int) 
           | (IData)(vlSelfRef.interrupt_valid));
    vlSelfRef.__VdfgRegularize_h495687df_0_7 = ((8U 
                                                 & vlSelfRef.core_top__DOT__if_id_inst)
                                                 ? 0U
                                                 : 
                                                ((4U 
                                                  & vlSelfRef.core_top__DOT__if_id_inst)
                                                  ? 0U
                                                  : vlSelfRef.__VdfgRegularize_h495687df_0_6));
    if ((0x40U & vlSelfRef.core_top__DOT__if_id_inst)) {
        if ((0x20U & vlSelfRef.core_top__DOT__if_id_inst)) {
            if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                          >> 4U)))) {
                if ((1U & (~ (vlSelfRef.core_top__DOT__if_id_inst 
                              >> 3U)))) {
                    if ((4U & vlSelfRef.core_top__DOT__if_id_inst)) {
                        if ((2U & vlSelfRef.core_top__DOT__if_id_inst)) {
                            if ((1U & vlSelfRef.core_top__DOT__if_id_inst)) {
                                vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int = 1U;
                            }
                        }
                    }
                }
            }
        }
    }
    Vcore_top___024unit____Vdpiimwrap_dpi_mem_read_TOP____024unit(vlSelfRef.core_top__DOT__pc, 0U, vlSelfRef.__Vfunc_dpi_mem_read__0__Vfuncout);
    vlSelfRef.core_top__DOT__imm_out = ((0x40U & vlSelfRef.core_top__DOT__if_id_inst)
                                         ? ((0x20U 
                                             & vlSelfRef.core_top__DOT__if_id_inst)
                                             ? ((0x10U 
                                                 & vlSelfRef.core_top__DOT__if_id_inst)
                                                 ? vlSelfRef.__VdfgRegularize_h495687df_0_7
                                                 : 
                                                ((8U 
                                                  & vlSelfRef.core_top__DOT__if_id_inst)
                                                  ? 
                                                 ((4U 
                                                   & vlSelfRef.core_top__DOT__if_id_inst)
                                                   ? 
                                                  ((2U 
                                                    & vlSelfRef.core_top__DOT__if_id_inst)
                                                    ? 
                                                   ((1U 
                                                     & vlSelfRef.core_top__DOT__if_id_inst)
                                                     ? 
                                                    ((((- (IData)(
                                                                  (vlSelfRef.core_top__DOT__if_id_inst 
                                                                   >> 0x1fU))) 
                                                       << 0x15U) 
                                                      | (0x100000U 
                                                         & (vlSelfRef.core_top__DOT__if_id_inst 
                                                            >> 0xbU))) 
                                                     | (((0xff000U 
                                                          & vlSelfRef.core_top__DOT__if_id_inst) 
                                                         | (0x800U 
                                                            & (vlSelfRef.core_top__DOT__if_id_inst 
                                                               >> 9U))) 
                                                        | (0x7feU 
                                                           & (vlSelfRef.core_top__DOT__if_id_inst 
                                                              >> 0x14U))))
                                                     : 0U)
                                                    : 0U)
                                                   : 0U)
                                                  : 
                                                 ((4U 
                                                   & vlSelfRef.core_top__DOT__if_id_inst)
                                                   ? vlSelfRef.__VdfgRegularize_h495687df_0_6
                                                   : 
                                                  ((2U 
                                                    & vlSelfRef.core_top__DOT__if_id_inst)
                                                    ? 
                                                   ((1U 
                                                     & vlSelfRef.core_top__DOT__if_id_inst)
                                                     ? 
                                                    (((- (IData)(
                                                                 (vlSelfRef.core_top__DOT__if_id_inst 
                                                                  >> 0x1fU))) 
                                                      << 0xdU) 
                                                     | (((0x1000U 
                                                          & (vlSelfRef.core_top__DOT__if_id_inst 
                                                             >> 0x13U)) 
                                                         | (0x800U 
                                                            & (vlSelfRef.core_top__DOT__if_id_inst 
                                                               << 4U))) 
                                                        | ((0x7e0U 
                                                            & (vlSelfRef.core_top__DOT__if_id_inst 
                                                               >> 0x14U)) 
                                                           | (0x1eU 
                                                              & (vlSelfRef.core_top__DOT__if_id_inst 
                                                                 >> 7U)))))
                                                     : 0U)
                                                    : 0U))))
                                             : 0U) : 
                                        ((0x20U & vlSelfRef.core_top__DOT__if_id_inst)
                                          ? ((0x10U 
                                              & vlSelfRef.core_top__DOT__if_id_inst)
                                              ? ((8U 
                                                  & vlSelfRef.core_top__DOT__if_id_inst)
                                                  ? 0U
                                                  : 
                                                 ((4U 
                                                   & vlSelfRef.core_top__DOT__if_id_inst)
                                                   ? vlSelfRef.__VdfgRegularize_h495687df_0_8
                                                   : 0U))
                                              : ((8U 
                                                  & vlSelfRef.core_top__DOT__if_id_inst)
                                                  ? 0U
                                                  : 
                                                 ((4U 
                                                   & vlSelfRef.core_top__DOT__if_id_inst)
                                                   ? 0U
                                                   : 
                                                  ((2U 
                                                    & vlSelfRef.core_top__DOT__if_id_inst)
                                                    ? 
                                                   ((1U 
                                                     & vlSelfRef.core_top__DOT__if_id_inst)
                                                     ? 
                                                    (((- (IData)(
                                                                 (vlSelfRef.core_top__DOT__if_id_inst 
                                                                  >> 0x1fU))) 
                                                      << 0xcU) 
                                                     | ((0xfe0U 
                                                         & (vlSelfRef.core_top__DOT__if_id_inst 
                                                            >> 0x14U)) 
                                                        | (0x1fU 
                                                           & (vlSelfRef.core_top__DOT__if_id_inst 
                                                              >> 7U))))
                                                     : 0U)
                                                    : 0U))))
                                          : ((0x10U 
                                              & vlSelfRef.core_top__DOT__if_id_inst)
                                              ? ((8U 
                                                  & vlSelfRef.core_top__DOT__if_id_inst)
                                                  ? 0U
                                                  : 
                                                 ((4U 
                                                   & vlSelfRef.core_top__DOT__if_id_inst)
                                                   ? vlSelfRef.__VdfgRegularize_h495687df_0_8
                                                   : vlSelfRef.__VdfgRegularize_h495687df_0_6))
                                              : vlSelfRef.__VdfgRegularize_h495687df_0_7)));
    vlSelfRef.instr = vlSelfRef.__Vfunc_dpi_mem_read__0__Vfuncout;
    vlSelfRef.core_top__DOT__u_id_stage__DOT__target_sum 
        = (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int)
             ? vlSelfRef.core_top__DOT__rs1_rdata : vlSelfRef.core_top__DOT__if_id_pc) 
           + vlSelfRef.core_top__DOT__imm_out);
    vlSelfRef.core_top__DOT__u_if_stage__DOT__pc_next 
        = (((IData)(vlSelfRef.core_top__DOT__u_if_stage__DOT__trap_enter) 
            | ((~ (IData)(vlSelfRef.interrupt_valid)) 
               & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int)))
            ? ((IData)(vlSelfRef.core_top__DOT__u_if_stage__DOT__trap_enter)
                ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                : ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int)
                    ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                    : 0U)) : (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jal_int) 
                               | ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int) 
                                  | ((3U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel)) 
                                     & ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                         ? ((~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type) 
                                                >> 1U)) 
                                            & ((1U 
                                                & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                                ? (vlSelfRef.core_top__DOT__rs1_rdata 
                                                   >= vlSelfRef.core_top__DOT__rs2_rdata)
                                                : (vlSelfRef.core_top__DOT__rs1_rdata 
                                                   < vlSelfRef.core_top__DOT__rs2_rdata)))
                                         : ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                             ? ((1U 
                                                 & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                                 ? 
                                                VL_GTES_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.core_top__DOT__rs2_rdata)
                                                 : 
                                                VL_LTS_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.core_top__DOT__rs2_rdata))
                                             : ((1U 
                                                 & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                                 ? 
                                                (vlSelfRef.core_top__DOT__rs1_rdata 
                                                 != vlSelfRef.core_top__DOT__rs2_rdata)
                                                 : 
                                                (vlSelfRef.core_top__DOT__rs1_rdata 
                                                 == vlSelfRef.core_top__DOT__rs2_rdata)))))))
                               ? ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int)
                                   ? (0xfffffffeU & vlSelfRef.core_top__DOT__u_id_stage__DOT__target_sum)
                                   : vlSelfRef.core_top__DOT__u_id_stage__DOT__target_sum)
                               : ((IData)(4U) + vlSelfRef.core_top__DOT__pc)));
}
