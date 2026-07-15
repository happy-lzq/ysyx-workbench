// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vcore_top__Syms.h"


void Vcore_top___024root__trace_chg_0_sub_0(Vcore_top___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vcore_top___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root__trace_chg_0\n"); );
    // Init
    Vcore_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vcore_top___024root*>(voidSelf);
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vcore_top___024root__trace_chg_0_sub_0((&vlSymsp->TOP), bufp);
}

void Vcore_top___024root__trace_chg_0_sub_0(Vcore_top___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root__trace_chg_0_sub_0\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    if (VL_UNLIKELY(((vlSelfRef.__Vm_traceActivity[1U] 
                      | vlSelfRef.__Vm_traceActivity
                      [2U])))) {
        bufp->chgIData(oldp+0,(((0U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src))
                                 ? vlSelfRef.core_top__DOT__alu_result
                                 : ((1U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src))
                                     ? ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_read)
                                         ? ((0U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                             ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw
                                             : ((1U 
                                                 == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                                 ? 
                                                (((- (IData)(
                                                             (1U 
                                                              & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                                                 >> 0xfU)))) 
                                                  << 0x10U) 
                                                 | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))
                                                 : 
                                                ((2U 
                                                  == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                                  ? 
                                                 (((- (IData)(
                                                              (1U 
                                                               & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected) 
                                                                  >> 7U)))) 
                                                   << 8U) 
                                                  | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected))
                                                  : 
                                                 ((3U 
                                                   == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                                   ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected)
                                                   : 
                                                  ((4U 
                                                    == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                                    ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected)
                                                    : 0U)))))
                                         : 0U) : ((2U 
                                                   == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src))
                                                   ? 
                                                  ((IData)(4U) 
                                                   + vlSelfRef.core_top__DOT__pc)
                                                   : vlSelfRef.core_top__DOT__csr_rdata)))),32);
        bufp->chgIData(oldp+1,(((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_read)
                                 ? ((0U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                     ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw
                                     : ((1U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                         ? (((- (IData)(
                                                        (1U 
                                                         & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                                            >> 0xfU)))) 
                                             << 0x10U) 
                                            | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))
                                         : ((2U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                             ? (((- (IData)(
                                                            (1U 
                                                             & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected) 
                                                                >> 7U)))) 
                                                 << 8U) 
                                                | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected))
                                             : ((3U 
                                                 == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                                 ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected)
                                                 : 
                                                ((4U 
                                                  == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                                  ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected)
                                                  : 0U)))))
                                 : 0U)),32);
        bufp->chgIData(oldp+2,(((IData)(vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter)
                                 ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                                 : ((IData)(vlSelfRef.core_top__DOT__u_csr__DOT__mret)
                                     ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                                     : 0U))),32);
        bufp->chgBit(oldp+3,(vlSelfRef.core_top__DOT__is_ebreak_eff));
        bufp->chgBit(oldp+4,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_write));
        bufp->chgBit(oldp+5,(vlSelfRef.core_top__DOT__u_csr__DOT__mret));
        bufp->chgBit(oldp+6,(vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter));
        bufp->chgIData(oldp+7,(vlSelfRef.core_top__DOT__u_csr__DOT__trap_code),32);
        bufp->chgIData(oldp+8,((((IData)(vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter) 
                                 | (IData)(vlSelfRef.core_top__DOT__u_csr__DOT__mret))
                                 ? ((IData)(vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter)
                                     ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                                     : ((IData)(vlSelfRef.core_top__DOT__u_csr__DOT__mret)
                                         ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                                         : 0U)) : (
                                                   (0U 
                                                    == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel_int))
                                                    ? 
                                                   ((IData)(4U) 
                                                    + vlSelfRef.core_top__DOT__pc)
                                                    : 
                                                   ((1U 
                                                     == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel_int))
                                                     ? vlSelfRef.__VdfgRegularize_h3c5bdd75_0_0
                                                     : 
                                                    ((2U 
                                                      == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel_int))
                                                      ? 
                                                     (0xfffffffeU 
                                                      & vlSelfRef.core_top__DOT__alu_result)
                                                      : 
                                                     ((3U 
                                                       == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel_int))
                                                       ? 
                                                      (((4U 
                                                         & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                                         ? 
                                                        ((~ 
                                                          ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type) 
                                                           >> 1U)) 
                                                         & ((1U 
                                                             & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                                             ? 
                                                            (vlSelfRef.core_top__DOT__rs1_rdata 
                                                             >= vlSelfRef.core_top__DOT__rs2_rdata)
                                                             : 
                                                            (vlSelfRef.core_top__DOT__rs1_rdata 
                                                             < vlSelfRef.core_top__DOT__rs2_rdata)))
                                                         : 
                                                        ((2U 
                                                          & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                                          ? 
                                                         ((1U 
                                                           & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                                           ? 
                                                          VL_GTES_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.core_top__DOT__rs2_rdata)
                                                           : 
                                                          VL_LTS_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.core_top__DOT__rs2_rdata))
                                                          : 
                                                         ((1U 
                                                           & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                                           ? 
                                                          (vlSelfRef.core_top__DOT__rs1_rdata 
                                                           != vlSelfRef.core_top__DOT__rs2_rdata)
                                                           : 
                                                          (vlSelfRef.core_top__DOT__rs1_rdata 
                                                           == vlSelfRef.core_top__DOT__rs2_rdata))))
                                                        ? vlSelfRef.__VdfgRegularize_h3c5bdd75_0_0
                                                        : 
                                                       ((IData)(4U) 
                                                        + vlSelfRef.core_top__DOT__pc))
                                                       : 
                                                      ((IData)(4U) 
                                                       + vlSelfRef.core_top__DOT__pc))))))),32);
        bufp->chgBit(oldp+9,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_read));
        bufp->chgBit(oldp+10,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write));
        bufp->chgCData(oldp+11,(((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write)
                                  ? ((5U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                      ? 0xfU : (0xfU 
                                                & ((6U 
                                                    == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                                    ? 
                                                   ((2U 
                                                     & vlSelfRef.core_top__DOT__alu_result)
                                                     ? 0xcU
                                                     : 3U)
                                                    : 
                                                   ((IData)(1U) 
                                                    << 
                                                    (3U 
                                                     & vlSelfRef.core_top__DOT__alu_result)))))
                                  : 0U)),4);
        bufp->chgIData(oldp+12,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw),32);
        bufp->chgIData(oldp+13,(((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write)
                                  ? ((5U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                      ? vlSelfRef.core_top__DOT__rs2_rdata
                                      : ((6U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                          ? ((vlSelfRef.core_top__DOT__rs2_rdata 
                                              << 0x10U) 
                                             | (0xffffU 
                                                & vlSelfRef.core_top__DOT__rs2_rdata))
                                          : ((vlSelfRef.core_top__DOT__rs2_rdata 
                                              << 0x18U) 
                                             | ((0xff0000U 
                                                 & (vlSelfRef.core_top__DOT__rs2_rdata 
                                                    << 0x10U)) 
                                                | ((0xff00U 
                                                    & (vlSelfRef.core_top__DOT__rs2_rdata 
                                                       << 8U)) 
                                                   | (0xffU 
                                                      & vlSelfRef.core_top__DOT__rs2_rdata))))))
                                  : 0U)),32);
        bufp->chgCData(oldp+14,((0xffU & vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw)),8);
        bufp->chgCData(oldp+15,((0xffU & (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 8U))),8);
        bufp->chgCData(oldp+16,((0xffU & (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 0x10U))),8);
        bufp->chgCData(oldp+17,((vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                 >> 0x18U)),8);
        bufp->chgSData(oldp+18,((0xffffU & vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw)),16);
        bufp->chgSData(oldp+19,((vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                 >> 0x10U)),16);
        bufp->chgCData(oldp+20,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected),8);
        bufp->chgSData(oldp+21,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected),16);
        bufp->chgIData(oldp+22,((((- (IData)((1U & 
                                              ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                               >> 0xfU)))) 
                                  << 0x10U) | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))),32);
        bufp->chgIData(oldp+23,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected),32);
        bufp->chgIData(oldp+24,((((- (IData)((1U & 
                                              ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected) 
                                               >> 7U)))) 
                                  << 8U) | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected))),32);
        bufp->chgIData(oldp+25,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected),32);
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[2U]))) {
        bufp->chgIData(oldp+26,(vlSelfRef.core_top__DOT__pc),32);
        bufp->chgIData(oldp+27,(((IData)(4U) + vlSelfRef.core_top__DOT__pc)),32);
        bufp->chgIData(oldp+28,(vlSelfRef.core_top__DOT__rs1_rdata),32);
        bufp->chgIData(oldp+29,(vlSelfRef.core_top__DOT__rs2_rdata),32);
        bufp->chgIData(oldp+30,(vlSelfRef.core_top__DOT__alu_result),32);
        bufp->chgIData(oldp+31,(vlSelfRef.core_top__DOT__imm_out),32);
        bufp->chgIData(oldp+32,(vlSelfRef.core_top__DOT__csr_rdata),32);
        bufp->chgIData(oldp+33,(vlSelfRef.core_top__DOT__csr_wdata),32);
        bufp->chgIData(oldp+34,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                                [0xaU]),32);
        bufp->chgBit(oldp+35,(((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                ? ((~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type) 
                                       >> 1U)) & ((1U 
                                                   & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                                   ? 
                                                  (vlSelfRef.core_top__DOT__rs1_rdata 
                                                   >= vlSelfRef.core_top__DOT__rs2_rdata)
                                                   : 
                                                  (vlSelfRef.core_top__DOT__rs1_rdata 
                                                   < vlSelfRef.core_top__DOT__rs2_rdata)))
                                : ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                    ? ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                        ? VL_GTES_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.core_top__DOT__rs2_rdata)
                                        : VL_LTS_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.core_top__DOT__rs2_rdata))
                                    : ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                        ? (vlSelfRef.core_top__DOT__rs1_rdata 
                                           != vlSelfRef.core_top__DOT__rs2_rdata)
                                        : (vlSelfRef.core_top__DOT__rs1_rdata 
                                           == vlSelfRef.core_top__DOT__rs2_rdata))))));
        bufp->chgSData(oldp+36,(((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op) 
                                   << 6U) | ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_a) 
                                             << 5U)) 
                                 | (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b) 
                                     << 3U) | (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type)))),11);
        bufp->chgCData(oldp+37,((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_read) 
                                  << 4U) | (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_write) 
                                             << 3U) 
                                            | (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type)))),5);
        bufp->chgQData(oldp+38,((((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_enter_int)) 
                                  << 0x22U) | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int)) 
                                                << 0x21U) 
                                               | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_ebreak_int)) 
                                                   << 0x20U) 
                                                  | (QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_code_int)))))),35);
        bufp->chgCData(oldp+40,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel_int),2);
        bufp->chgBit(oldp+41,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write));
        bufp->chgBit(oldp+42,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_ebreak_int));
        bufp->chgBit(oldp+43,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_write));
        bufp->chgBit(oldp+44,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_read));
        bufp->chgBit(oldp+45,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_enter_int));
        bufp->chgBit(oldp+46,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int));
        bufp->chgIData(oldp+47,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_code_int),32);
        bufp->chgIData(oldp+48,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus),32);
        bufp->chgIData(oldp+49,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec),32);
        bufp->chgIData(oldp+50,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc),32);
        bufp->chgIData(oldp+51,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause),32);
        bufp->chgIData(oldp+52,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip),32);
        bufp->chgIData(oldp+53,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mie),32);
        bufp->chgIData(oldp+54,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtval),32);
        bufp->chgIData(oldp+55,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mscratch),32);
        bufp->chgQData(oldp+56,(vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64),64);
        bufp->chgCData(oldp+58,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op),5);
        bufp->chgBit(oldp+59,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_a));
        bufp->chgCData(oldp+60,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b),2);
        bufp->chgCData(oldp+61,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type),3);
        bufp->chgCData(oldp+62,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_op),2);
        bufp->chgBit(oldp+63,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_imm));
        bufp->chgIData(oldp+64,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1),32);
        bufp->chgIData(oldp+65,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2),32);
        bufp->chgIData(oldp+66,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src),32);
        bufp->chgIData(oldp+67,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__abs_dividend),32);
        bufp->chgIData(oldp+68,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__abs_divisor),32);
        bufp->chgIData(oldp+69,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__quotient),32);
        bufp->chgIData(oldp+70,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__abs_dividend),32);
        bufp->chgIData(oldp+71,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__abs_divisor),32);
        bufp->chgIData(oldp+72,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__remainder),32);
        bufp->chgQData(oldp+73,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__src1_s64),64);
        bufp->chgQData(oldp+75,((((QData)((IData)((- (IData)(
                                                             (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2 
                                                              >> 0x1fU))))) 
                                  << 0x20U) | (QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)))),64);
        bufp->chgQData(oldp+77,(VL_MULS_QQQ(64, vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__src1_s64, 
                                            (((QData)((IData)(
                                                              (- (IData)(
                                                                         (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2 
                                                                          >> 0x1fU))))) 
                                              << 0x20U) 
                                             | (QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))))),64);
        bufp->chgQData(oldp+79,((QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1))),64);
        bufp->chgQData(oldp+81,((QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))),64);
        bufp->chgQData(oldp+83,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__mul_uu),64);
        bufp->chgQData(oldp+85,((vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__src1_s64 
                                 * (QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)))),64);
        bufp->chgBit(oldp+87,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_read));
        bufp->chgBit(oldp+88,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_write));
        bufp->chgCData(oldp+89,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type),3);
        bufp->chgCData(oldp+90,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src),2);
        bufp->chgCData(oldp+91,((3U & vlSelfRef.core_top__DOT__alu_result)),2);
        bufp->chgBit(oldp+92,((1U & (vlSelfRef.core_top__DOT__alu_result 
                                     >> 1U))));
        bufp->chgBit(oldp+93,((0U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))));
        bufp->chgBit(oldp+94,((1U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))));
        bufp->chgBit(oldp+95,((2U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))));
        bufp->chgBit(oldp+96,((3U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))));
        bufp->chgBit(oldp+97,((4U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))));
        bufp->chgBit(oldp+98,((5U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))));
        bufp->chgBit(oldp+99,((6U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))));
        bufp->chgBit(oldp+100,((7U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))));
        bufp->chgIData(oldp+101,(((vlSelfRef.core_top__DOT__rs2_rdata 
                                   << 0x10U) | (0xffffU 
                                                & vlSelfRef.core_top__DOT__rs2_rdata))),32);
        bufp->chgIData(oldp+102,(((vlSelfRef.core_top__DOT__rs2_rdata 
                                   << 0x18U) | ((0xff0000U 
                                                 & (vlSelfRef.core_top__DOT__rs2_rdata 
                                                    << 0x10U)) 
                                                | ((0xff00U 
                                                    & (vlSelfRef.core_top__DOT__rs2_rdata 
                                                       << 8U)) 
                                                   | (0xffU 
                                                      & vlSelfRef.core_top__DOT__rs2_rdata))))),32);
        bufp->chgCData(oldp+103,(((2U & vlSelfRef.core_top__DOT__alu_result)
                                   ? 0xcU : 3U)),4);
        bufp->chgCData(oldp+104,((0xfU & ((IData)(1U) 
                                          << (3U & vlSelfRef.core_top__DOT__alu_result)))),4);
        bufp->chgIData(oldp+105,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[0]),32);
        bufp->chgIData(oldp+106,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[1]),32);
        bufp->chgIData(oldp+107,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[2]),32);
        bufp->chgIData(oldp+108,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[3]),32);
        bufp->chgIData(oldp+109,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[4]),32);
        bufp->chgIData(oldp+110,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[5]),32);
        bufp->chgIData(oldp+111,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[6]),32);
        bufp->chgIData(oldp+112,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[7]),32);
        bufp->chgIData(oldp+113,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[8]),32);
        bufp->chgIData(oldp+114,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[9]),32);
        bufp->chgIData(oldp+115,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[10]),32);
        bufp->chgIData(oldp+116,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[11]),32);
        bufp->chgIData(oldp+117,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[12]),32);
        bufp->chgIData(oldp+118,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[13]),32);
        bufp->chgIData(oldp+119,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[14]),32);
        bufp->chgIData(oldp+120,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[15]),32);
        bufp->chgIData(oldp+121,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[16]),32);
        bufp->chgIData(oldp+122,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[17]),32);
        bufp->chgIData(oldp+123,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[18]),32);
        bufp->chgIData(oldp+124,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[19]),32);
        bufp->chgIData(oldp+125,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[20]),32);
        bufp->chgIData(oldp+126,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[21]),32);
        bufp->chgIData(oldp+127,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[22]),32);
        bufp->chgIData(oldp+128,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[23]),32);
        bufp->chgIData(oldp+129,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[24]),32);
        bufp->chgIData(oldp+130,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[25]),32);
        bufp->chgIData(oldp+131,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[26]),32);
        bufp->chgIData(oldp+132,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[27]),32);
        bufp->chgIData(oldp+133,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[28]),32);
        bufp->chgIData(oldp+134,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[29]),32);
        bufp->chgIData(oldp+135,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[30]),32);
        bufp->chgIData(oldp+136,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[31]),32);
    }
    bufp->chgBit(oldp+137,(vlSelfRef.clk));
    bufp->chgBit(oldp+138,(vlSelfRef.rst));
    bufp->chgBit(oldp+139,(vlSelfRef.interrupt_valid));
    bufp->chgIData(oldp+140,(vlSelfRef.interrupt_cause),32);
    bufp->chgBit(oldp+141,(vlSelfRef.halt));
    bufp->chgIData(oldp+142,(vlSelfRef.instr),32);
    bufp->chgIData(oldp+143,(vlSelfRef.halt_pc),32);
    bufp->chgIData(oldp+144,(vlSelfRef.halt_ret),32);
    bufp->chgCData(oldp+145,((0x1fU & (vlSelfRef.instr 
                                       >> 0xfU))),5);
    bufp->chgCData(oldp+146,((0x1fU & (vlSelfRef.instr 
                                       >> 0x14U))),5);
    bufp->chgCData(oldp+147,((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write) 
                               << 7U) | (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src) 
                                          << 5U) | 
                                         (0x1fU & (vlSelfRef.instr 
                                                   >> 7U))))),8);
    bufp->chgQData(oldp+148,((((QData)((IData)((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_read) 
                                                 << 1U) 
                                                | (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_write)))) 
                               << 0x2fU) | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_op)) 
                                             << 0x2dU) 
                                            | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_imm)) 
                                                << 0x2cU) 
                                               | (((QData)((IData)(
                                                                   (vlSelfRef.instr 
                                                                    >> 0x14U))) 
                                                   << 0x20U) 
                                                  | (QData)((IData)(
                                                                    (0x1fU 
                                                                     & (vlSelfRef.instr 
                                                                        >> 0xfU))))))))),49);
    bufp->chgCData(oldp+150,((0x1fU & (vlSelfRef.instr 
                                       >> 7U))),5);
    bufp->chgBit(oldp+151,(((~ (IData)(vlSelfRef.interrupt_valid)) 
                            & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write))));
    bufp->chgSData(oldp+152,((vlSelfRef.instr >> 0x14U)),12);
    bufp->chgIData(oldp+153,((0x1fU & (vlSelfRef.instr 
                                       >> 0xfU))),32);
    bufp->chgCData(oldp+154,((0x7fU & vlSelfRef.instr)),7);
    bufp->chgCData(oldp+155,((7U & (vlSelfRef.instr 
                                    >> 0xcU))),3);
    bufp->chgCData(oldp+156,((vlSelfRef.instr >> 0x19U)),7);
    bufp->chgIData(oldp+157,((((- (IData)((vlSelfRef.instr 
                                           >> 0x1fU))) 
                               << 0xcU) | (vlSelfRef.instr 
                                           >> 0x14U))),32);
    bufp->chgIData(oldp+158,((((- (IData)((vlSelfRef.instr 
                                           >> 0x1fU))) 
                               << 0xcU) | ((0xfe0U 
                                            & (vlSelfRef.instr 
                                               >> 0x14U)) 
                                           | (0x1fU 
                                              & (vlSelfRef.instr 
                                                 >> 7U))))),32);
    bufp->chgIData(oldp+159,((((- (IData)((vlSelfRef.instr 
                                           >> 0x1fU))) 
                               << 0xdU) | (((0x1000U 
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
                                                    >> 7U)))))),32);
    bufp->chgIData(oldp+160,((0xfffff000U & vlSelfRef.instr)),32);
    bufp->chgIData(oldp+161,(((((- (IData)((vlSelfRef.instr 
                                            >> 0x1fU))) 
                                << 0x15U) | (0x100000U 
                                             & (vlSelfRef.instr 
                                                >> 0xbU))) 
                              | (((0xff000U & vlSelfRef.instr) 
                                  | (0x800U & (vlSelfRef.instr 
                                               >> 9U))) 
                                 | (0x7feU & (vlSelfRef.instr 
                                              >> 0x14U))))),32);
}

void Vcore_top___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root__trace_cleanup\n"); );
    // Init
    Vcore_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vcore_top___024root*>(voidSelf);
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    vlSymsp->__Vm_activity = false;
    vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[2U] = 0U;
}
