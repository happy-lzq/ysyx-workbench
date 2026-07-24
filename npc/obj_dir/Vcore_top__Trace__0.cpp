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
        bufp->chgIData(oldp+0,(((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_read)
                                 ? ((0U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                     ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw
                                     : ((1U == (7U 
                                                & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                         ? (((- (IData)(
                                                        (1U 
                                                         & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                                            >> 0xfU)))) 
                                             << 0x10U) 
                                            | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))
                                         : ((2U == 
                                             (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                             ? (((- (IData)(
                                                            (1U 
                                                             & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected) 
                                                                >> 7U)))) 
                                                 << 8U) 
                                                | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected))
                                             : ((3U 
                                                 == 
                                                 (7U 
                                                  & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                                 ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected)
                                                 : 
                                                ((4U 
                                                  == 
                                                  (7U 
                                                   & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                                  ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected)
                                                  : 0U)))))
                                 : 0U)),32);
        bufp->chgIData(oldp+1,(((IData)(vlSelfRef.core_top__DOT__u_if_stage__DOT__trap_enter)
                                 ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                                 : ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int)
                                     ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                                     : 0U))),32);
        bufp->chgBit(oldp+2,(((IData)(vlSelfRef.core_top__DOT__u_csr__DOT____VdfgRegularize_h8136f0d8_0_0) 
                              & ((IData)(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl) 
                                 >> 2U))));
        bufp->chgBit(oldp+3,(vlSelfRef.core_top__DOT__is_ebreak_eff));
        bufp->chgBit(oldp+4,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_write));
        bufp->chgBit(oldp+5,(vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter));
        bufp->chgBit(oldp+6,(vlSelfRef.core_top__DOT__u_csr__DOT__mret));
        bufp->chgIData(oldp+7,(vlSelfRef.core_top__DOT__u_csr__DOT__trap_code),32);
        bufp->chgBit(oldp+8,(vlSelfRef.core_top__DOT__u_if_stage__DOT__trap_enter));
        bufp->chgBit(oldp+9,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_read));
        bufp->chgBit(oldp+10,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write));
        bufp->chgCData(oldp+11,(((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write)
                                  ? ((5U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                      ? 0xfU : (0xfU 
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
                                  : 0U)),4);
        bufp->chgIData(oldp+12,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw),32);
        bufp->chgIData(oldp+13,(((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write)
                                  ? ((5U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                      ? vlSelfRef.core_top__DOT__ex_mem_rs2_rdata
                                      : ((6U == (7U 
                                                 & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                          ? ((vlSelfRef.core_top__DOT__ex_mem_rs2_rdata 
                                              << 0x10U) 
                                             | (0xffffU 
                                                & vlSelfRef.core_top__DOT__ex_mem_rs2_rdata))
                                          : ((vlSelfRef.core_top__DOT__ex_mem_rs2_rdata 
                                              << 0x18U) 
                                             | ((0xff0000U 
                                                 & (vlSelfRef.core_top__DOT__ex_mem_rs2_rdata 
                                                    << 0x10U)) 
                                                | ((0xff00U 
                                                    & (vlSelfRef.core_top__DOT__ex_mem_rs2_rdata 
                                                       << 8U)) 
                                                   | (0xffU 
                                                      & vlSelfRef.core_top__DOT__ex_mem_rs2_rdata))))))
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
        bufp->chgIData(oldp+30,(((0U == (3U & (IData)(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl)))
                                  ? vlSelfRef.core_top__DOT__mem_wb_alu_result
                                  : ((1U == (3U & (IData)(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl)))
                                      ? vlSelfRef.core_top__DOT__mem_wb_mem_rdata
                                      : ((2U == (3U 
                                                 & (IData)(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl)))
                                          ? vlSelfRef.core_top__DOT__mem_wb_pc_plus4
                                          : vlSelfRef.core_top__DOT__mem_wb_csr_rdata)))),32);
        bufp->chgIData(oldp+31,(vlSelfRef.core_top__DOT__alu_result),32);
        bufp->chgIData(oldp+32,(vlSelfRef.core_top__DOT__imm_out),32);
        bufp->chgIData(oldp+33,(vlSelfRef.core_top__DOT__csr_rdata),32);
        bufp->chgIData(oldp+34,(((0U == (3U & (IData)(
                                                      (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                       >> 0x2dU))))
                                  ? vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src
                                  : ((1U == (3U & (IData)(
                                                          (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                           >> 0x2dU))))
                                      ? (vlSelfRef.core_top__DOT__csr_rdata 
                                         | vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src)
                                      : ((2U == (3U 
                                                 & (IData)(
                                                           (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                            >> 0x2dU))))
                                          ? ((~ vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src) 
                                             & vlSelfRef.core_top__DOT__csr_rdata)
                                          : 0U)))),32);
        bufp->chgIData(oldp+35,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                                [0xaU]),32);
        bufp->chgCData(oldp+36,((0x1fU & (vlSelfRef.core_top__DOT__if_id_inst 
                                          >> 0xfU))),5);
        bufp->chgCData(oldp+37,((0x1fU & (vlSelfRef.core_top__DOT__if_id_inst 
                                          >> 0x14U))),5);
        bufp->chgCData(oldp+38,((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op) 
                                  << 3U) | (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_a) 
                                             << 2U) 
                                            | (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b)))),8);
        bufp->chgCData(oldp+39,((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_read) 
                                  << 4U) | (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_write) 
                                             << 3U) 
                                            | (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type)))),5);
        bufp->chgCData(oldp+40,((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write) 
                                  << 2U) | (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src))),3);
        bufp->chgQData(oldp+41,((((QData)((IData)((
                                                   ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_read) 
                                                    << 1U) 
                                                   | (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_write)))) 
                                  << 0x2fU) | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_op)) 
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
                                                                           >> 0xfU))))))))),49);
        bufp->chgQData(oldp+43,((((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_enter_int)) 
                                  << 0x22U) | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int)) 
                                                << 0x21U) 
                                               | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_ebreak_int)) 
                                                   << 0x20U) 
                                                  | (QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_code_int)))))),35);
        bufp->chgQData(oldp+45,((((QData)((IData)((
                                                   ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel) 
                                                    << 1U) 
                                                   | ((4U 
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
                                                         == vlSelfRef.core_top__DOT__rs2_rdata))))))) 
                                  << 0x25U) | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type)) 
                                                << 0x22U) 
                                               | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jal_int)) 
                                                   << 0x21U) 
                                                  | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int)) 
                                                      << 0x20U) 
                                                     | (QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_code_int))))))),40);
        bufp->chgCData(oldp+47,((0x1fU & (vlSelfRef.core_top__DOT__if_id_inst 
                                          >> 7U))),5);
        bufp->chgBit(oldp+48,((1U & ((IData)(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl) 
                                     >> 2U))));
        bufp->chgBit(oldp+49,((1U & (IData)((vlSelfRef.core_top__DOT__mem_wb_sys_ctrl 
                                             >> 0x20U)))));
        bufp->chgIData(oldp+50,(((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int)
                                  ? (0xfffffffeU & vlSelfRef.core_top__DOT__u_id_stage__DOT__target_sum)
                                  : vlSelfRef.core_top__DOT__u_id_stage__DOT__target_sum)),32);
        bufp->chgBit(oldp+51,(vlSelfRef.core_top__DOT__if_id_valid));
        bufp->chgIData(oldp+52,(vlSelfRef.core_top__DOT__if_id_pc),32);
        bufp->chgIData(oldp+53,(vlSelfRef.core_top__DOT__if_id_pc_plus4),32);
        bufp->chgIData(oldp+54,(vlSelfRef.core_top__DOT__if_id_inst),32);
        bufp->chgBit(oldp+55,(vlSelfRef.core_top__DOT__id_ex_valid));
        bufp->chgIData(oldp+56,(vlSelfRef.core_top__DOT__id_ex_pc),32);
        bufp->chgIData(oldp+57,(vlSelfRef.core_top__DOT__id_ex_pc_plus4),32);
        bufp->chgIData(oldp+58,(vlSelfRef.core_top__DOT__id_ex_inst),32);
        bufp->chgCData(oldp+59,(vlSelfRef.core_top__DOT__id_ex_rs1_addr),5);
        bufp->chgCData(oldp+60,(vlSelfRef.core_top__DOT__id_ex_rs2_addr),5);
        bufp->chgCData(oldp+61,(vlSelfRef.core_top__DOT__id_ex_rd_addr),5);
        bufp->chgIData(oldp+62,(vlSelfRef.core_top__DOT__id_ex_rs1_rdata),32);
        bufp->chgIData(oldp+63,(vlSelfRef.core_top__DOT__id_ex_rs2_rdata),32);
        bufp->chgIData(oldp+64,(vlSelfRef.core_top__DOT__id_ex_imm_out),32);
        bufp->chgCData(oldp+65,(vlSelfRef.core_top__DOT__id_ex_ex_ctrl),8);
        bufp->chgCData(oldp+66,(vlSelfRef.core_top__DOT__id_ex_mem_ctrl),5);
        bufp->chgCData(oldp+67,(vlSelfRef.core_top__DOT__id_ex_wb_ctrl),3);
        bufp->chgQData(oldp+68,(vlSelfRef.core_top__DOT__id_ex_csr_ctrl),49);
        bufp->chgQData(oldp+70,(vlSelfRef.core_top__DOT__id_ex_sys_ctrl),35);
        bufp->chgBit(oldp+72,(vlSelfRef.core_top__DOT__ex_mem_valid));
        bufp->chgIData(oldp+73,(vlSelfRef.core_top__DOT__ex_mem_pc),32);
        bufp->chgIData(oldp+74,(vlSelfRef.core_top__DOT__ex_mem_pc_plus4),32);
        bufp->chgIData(oldp+75,(vlSelfRef.core_top__DOT__ex_mem_inst),32);
        bufp->chgCData(oldp+76,(vlSelfRef.core_top__DOT__ex_mem_rd_addr),5);
        bufp->chgIData(oldp+77,(vlSelfRef.core_top__DOT__ex_mem_alu_result),32);
        bufp->chgIData(oldp+78,(vlSelfRef.core_top__DOT__ex_mem_rs2_rdata),32);
        bufp->chgIData(oldp+79,(vlSelfRef.core_top__DOT__ex_mem_csr_rdata),32);
        bufp->chgIData(oldp+80,(vlSelfRef.core_top__DOT__ex_mem_csr_wdata),32);
        bufp->chgCData(oldp+81,(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl),5);
        bufp->chgCData(oldp+82,(vlSelfRef.core_top__DOT__ex_mem_wb_ctrl),3);
        bufp->chgQData(oldp+83,(vlSelfRef.core_top__DOT__ex_mem_csr_ctrl),49);
        bufp->chgQData(oldp+85,(vlSelfRef.core_top__DOT__ex_mem_sys_ctrl),35);
        bufp->chgBit(oldp+87,(vlSelfRef.core_top__DOT__mem_wb_valid));
        bufp->chgIData(oldp+88,(vlSelfRef.core_top__DOT__mem_wb_pc),32);
        bufp->chgIData(oldp+89,(vlSelfRef.core_top__DOT__mem_wb_pc_plus4),32);
        bufp->chgIData(oldp+90,(vlSelfRef.core_top__DOT__mem_wb_inst),32);
        bufp->chgCData(oldp+91,(vlSelfRef.core_top__DOT__mem_wb_rd_addr),5);
        bufp->chgIData(oldp+92,(vlSelfRef.core_top__DOT__mem_wb_alu_result),32);
        bufp->chgIData(oldp+93,(vlSelfRef.core_top__DOT__mem_wb_mem_rdata),32);
        bufp->chgIData(oldp+94,(vlSelfRef.core_top__DOT__mem_wb_csr_rdata),32);
        bufp->chgIData(oldp+95,(vlSelfRef.core_top__DOT__mem_wb_csr_wdata),32);
        bufp->chgCData(oldp+96,(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl),3);
        bufp->chgQData(oldp+97,(vlSelfRef.core_top__DOT__mem_wb_csr_ctrl),49);
        bufp->chgQData(oldp+99,(vlSelfRef.core_top__DOT__mem_wb_sys_ctrl),35);
        bufp->chgSData(oldp+101,((0xfffU & (IData)(
                                                   (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                    >> 0x20U)))),12);
        bufp->chgBit(oldp+102,((1U & (IData)((vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                              >> 0x30U)))));
        bufp->chgSData(oldp+103,((0xfffU & (IData)(
                                                   (vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                    >> 0x20U)))),12);
        bufp->chgBit(oldp+104,((1U & (IData)((vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                              >> 0x2fU)))));
        bufp->chgBit(oldp+105,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_enter_int));
        bufp->chgBit(oldp+106,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int));
        bufp->chgBit(oldp+107,((1U & (IData)((vlSelfRef.core_top__DOT__mem_wb_sys_ctrl 
                                              >> 0x22U)))));
        bufp->chgBit(oldp+108,((1U & (IData)((vlSelfRef.core_top__DOT__mem_wb_sys_ctrl 
                                              >> 0x21U)))));
        bufp->chgIData(oldp+109,((IData)(vlSelfRef.core_top__DOT__mem_wb_sys_ctrl)),32);
        bufp->chgIData(oldp+110,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus),32);
        bufp->chgIData(oldp+111,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec),32);
        bufp->chgIData(oldp+112,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc),32);
        bufp->chgIData(oldp+113,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause),32);
        bufp->chgIData(oldp+114,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip),32);
        bufp->chgIData(oldp+115,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mie),32);
        bufp->chgIData(oldp+116,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtval),32);
        bufp->chgIData(oldp+117,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mscratch),32);
        bufp->chgQData(oldp+118,(vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64),64);
        bufp->chgCData(oldp+120,((0x1fU & ((IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl) 
                                           >> 3U))),5);
        bufp->chgBit(oldp+121,((1U & ((IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl) 
                                      >> 2U))));
        bufp->chgCData(oldp+122,((3U & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))),2);
        bufp->chgCData(oldp+123,((3U & (IData)((vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                >> 0x2dU)))),2);
        bufp->chgBit(oldp+124,((1U & (IData)((vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                              >> 0x2cU)))));
        bufp->chgIData(oldp+125,((IData)(vlSelfRef.core_top__DOT__id_ex_csr_ctrl)),32);
        bufp->chgIData(oldp+126,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1),32);
        bufp->chgIData(oldp+127,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2),32);
        bufp->chgIData(oldp+128,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src),32);
        bufp->chgIData(oldp+129,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__abs_dividend),32);
        bufp->chgIData(oldp+130,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__abs_divisor),32);
        bufp->chgIData(oldp+131,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__quotient),32);
        bufp->chgIData(oldp+132,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__abs_dividend),32);
        bufp->chgIData(oldp+133,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__abs_divisor),32);
        bufp->chgIData(oldp+134,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__remainder),32);
        bufp->chgQData(oldp+135,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__src1_s64),64);
        bufp->chgQData(oldp+137,((((QData)((IData)(
                                                   (- (IData)(
                                                              (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2 
                                                               >> 0x1fU))))) 
                                   << 0x20U) | (QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)))),64);
        bufp->chgQData(oldp+139,(VL_MULS_QQQ(64, vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__src1_s64, 
                                             (((QData)((IData)(
                                                               (- (IData)(
                                                                          (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2 
                                                                           >> 0x1fU))))) 
                                               << 0x20U) 
                                              | (QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))))),64);
        bufp->chgQData(oldp+141,((QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1))),64);
        bufp->chgQData(oldp+143,((QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))),64);
        bufp->chgQData(oldp+145,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__mul_uu),64);
        bufp->chgQData(oldp+147,((vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__src1_s64 
                                  * (QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)))),64);
        bufp->chgCData(oldp+149,((0x7fU & vlSelfRef.core_top__DOT__if_id_inst)),7);
        bufp->chgCData(oldp+150,((7U & (vlSelfRef.core_top__DOT__if_id_inst 
                                        >> 0xcU))),3);
        bufp->chgCData(oldp+151,((vlSelfRef.core_top__DOT__if_id_inst 
                                  >> 0x19U)),7);
        bufp->chgBit(oldp+152,(((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                 ? ((~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type) 
                                        >> 1U)) & (
                                                   (1U 
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
        bufp->chgQData(oldp+153,((((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel)) 
                                   << 0x26U) | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type)) 
                                                 << 0x22U) 
                                                | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jal_int)) 
                                                    << 0x21U) 
                                                   | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int)) 
                                                       << 0x20U) 
                                                      | (QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_code_int))))))),40);
        bufp->chgCData(oldp+155,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type),3);
        bufp->chgBit(oldp+156,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int));
        bufp->chgIData(oldp+157,(vlSelfRef.core_top__DOT__u_id_stage__DOT__target_sum),32);
        bufp->chgCData(oldp+158,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op),5);
        bufp->chgBit(oldp+159,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_a));
        bufp->chgCData(oldp+160,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b),2);
        bufp->chgBit(oldp+161,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_read));
        bufp->chgBit(oldp+162,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_write));
        bufp->chgCData(oldp+163,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type),3);
        bufp->chgBit(oldp+164,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write));
        bufp->chgCData(oldp+165,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src),2);
        bufp->chgBit(oldp+166,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_read));
        bufp->chgBit(oldp+167,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_write));
        bufp->chgBit(oldp+168,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_imm));
        bufp->chgCData(oldp+169,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_op),2);
        bufp->chgCData(oldp+170,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel),2);
        bufp->chgBit(oldp+171,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_ebreak_int));
        bufp->chgBit(oldp+172,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jal_int));
        bufp->chgIData(oldp+173,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_code_int),32);
        bufp->chgSData(oldp+174,((vlSelfRef.core_top__DOT__if_id_inst 
                                  >> 0x14U)),12);
        bufp->chgIData(oldp+175,((0x1fU & (vlSelfRef.core_top__DOT__if_id_inst 
                                           >> 0xfU))),32);
        bufp->chgIData(oldp+176,((((- (IData)((vlSelfRef.core_top__DOT__if_id_inst 
                                               >> 0x1fU))) 
                                   << 0xcU) | (vlSelfRef.core_top__DOT__if_id_inst 
                                               >> 0x14U))),32);
        bufp->chgIData(oldp+177,((((- (IData)((vlSelfRef.core_top__DOT__if_id_inst 
                                               >> 0x1fU))) 
                                   << 0xcU) | ((0xfe0U 
                                                & (vlSelfRef.core_top__DOT__if_id_inst 
                                                   >> 0x14U)) 
                                               | (0x1fU 
                                                  & (vlSelfRef.core_top__DOT__if_id_inst 
                                                     >> 7U))))),32);
        bufp->chgIData(oldp+178,((((- (IData)((vlSelfRef.core_top__DOT__if_id_inst 
                                               >> 0x1fU))) 
                                   << 0xdU) | (((0x1000U 
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
                                                        >> 7U)))))),32);
        bufp->chgIData(oldp+179,((0xfffff000U & vlSelfRef.core_top__DOT__if_id_inst)),32);
        bufp->chgIData(oldp+180,(((((- (IData)((vlSelfRef.core_top__DOT__if_id_inst 
                                                >> 0x1fU))) 
                                    << 0x15U) | (0x100000U 
                                                 & (vlSelfRef.core_top__DOT__if_id_inst 
                                                    >> 0xbU))) 
                                  | (((0xff000U & vlSelfRef.core_top__DOT__if_id_inst) 
                                      | (0x800U & (vlSelfRef.core_top__DOT__if_id_inst 
                                                   >> 9U))) 
                                     | (0x7feU & (vlSelfRef.core_top__DOT__if_id_inst 
                                                  >> 0x14U))))),32);
        bufp->chgBit(oldp+181,(((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jal_int) 
                                | ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int) 
                                   | ((3U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel)) 
                                      & ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                          ? ((~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type) 
                                                 >> 1U)) 
                                             & ((1U 
                                                 & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                                 ? 
                                                (vlSelfRef.core_top__DOT__rs1_rdata 
                                                 >= vlSelfRef.core_top__DOT__rs2_rdata)
                                                 : 
                                                (vlSelfRef.core_top__DOT__rs1_rdata 
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
                                                  == vlSelfRef.core_top__DOT__rs2_rdata)))))))));
        bufp->chgCData(oldp+182,((7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl))),3);
        bufp->chgCData(oldp+183,((3U & vlSelfRef.core_top__DOT__ex_mem_alu_result)),2);
        bufp->chgBit(oldp+184,((1U & (vlSelfRef.core_top__DOT__ex_mem_alu_result 
                                      >> 1U))));
        bufp->chgBit(oldp+185,((0U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))));
        bufp->chgBit(oldp+186,((1U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))));
        bufp->chgBit(oldp+187,((2U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))));
        bufp->chgBit(oldp+188,((3U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))));
        bufp->chgBit(oldp+189,((4U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))));
        bufp->chgBit(oldp+190,((5U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))));
        bufp->chgBit(oldp+191,((6U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))));
        bufp->chgBit(oldp+192,((7U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))));
        bufp->chgIData(oldp+193,(((vlSelfRef.core_top__DOT__ex_mem_rs2_rdata 
                                   << 0x10U) | (0xffffU 
                                                & vlSelfRef.core_top__DOT__ex_mem_rs2_rdata))),32);
        bufp->chgIData(oldp+194,(((vlSelfRef.core_top__DOT__ex_mem_rs2_rdata 
                                   << 0x18U) | ((0xff0000U 
                                                 & (vlSelfRef.core_top__DOT__ex_mem_rs2_rdata 
                                                    << 0x10U)) 
                                                | ((0xff00U 
                                                    & (vlSelfRef.core_top__DOT__ex_mem_rs2_rdata 
                                                       << 8U)) 
                                                   | (0xffU 
                                                      & vlSelfRef.core_top__DOT__ex_mem_rs2_rdata))))),32);
        bufp->chgCData(oldp+195,(((2U & vlSelfRef.core_top__DOT__ex_mem_alu_result)
                                   ? 0xcU : 3U)),4);
        bufp->chgCData(oldp+196,((0xfU & ((IData)(1U) 
                                          << (3U & vlSelfRef.core_top__DOT__ex_mem_alu_result)))),4);
        bufp->chgIData(oldp+197,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[0]),32);
        bufp->chgIData(oldp+198,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[1]),32);
        bufp->chgIData(oldp+199,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[2]),32);
        bufp->chgIData(oldp+200,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[3]),32);
        bufp->chgIData(oldp+201,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[4]),32);
        bufp->chgIData(oldp+202,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[5]),32);
        bufp->chgIData(oldp+203,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[6]),32);
        bufp->chgIData(oldp+204,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[7]),32);
        bufp->chgIData(oldp+205,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[8]),32);
        bufp->chgIData(oldp+206,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[9]),32);
        bufp->chgIData(oldp+207,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[10]),32);
        bufp->chgIData(oldp+208,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[11]),32);
        bufp->chgIData(oldp+209,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[12]),32);
        bufp->chgIData(oldp+210,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[13]),32);
        bufp->chgIData(oldp+211,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[14]),32);
        bufp->chgIData(oldp+212,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[15]),32);
        bufp->chgIData(oldp+213,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[16]),32);
        bufp->chgIData(oldp+214,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[17]),32);
        bufp->chgIData(oldp+215,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[18]),32);
        bufp->chgIData(oldp+216,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[19]),32);
        bufp->chgIData(oldp+217,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[20]),32);
        bufp->chgIData(oldp+218,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[21]),32);
        bufp->chgIData(oldp+219,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[22]),32);
        bufp->chgIData(oldp+220,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[23]),32);
        bufp->chgIData(oldp+221,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[24]),32);
        bufp->chgIData(oldp+222,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[25]),32);
        bufp->chgIData(oldp+223,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[26]),32);
        bufp->chgIData(oldp+224,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[27]),32);
        bufp->chgIData(oldp+225,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[28]),32);
        bufp->chgIData(oldp+226,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[29]),32);
        bufp->chgIData(oldp+227,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[30]),32);
        bufp->chgIData(oldp+228,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[31]),32);
        bufp->chgCData(oldp+229,((3U & (IData)(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl))),2);
    }
    bufp->chgBit(oldp+230,(vlSelfRef.clk));
    bufp->chgBit(oldp+231,(vlSelfRef.rst));
    bufp->chgBit(oldp+232,(vlSelfRef.interrupt_valid));
    bufp->chgIData(oldp+233,(vlSelfRef.interrupt_cause),32);
    bufp->chgBit(oldp+234,(vlSelfRef.halt));
    bufp->chgIData(oldp+235,(vlSelfRef.instr),32);
    bufp->chgIData(oldp+236,(vlSelfRef.halt_pc),32);
    bufp->chgIData(oldp+237,(vlSelfRef.halt_ret),32);
    bufp->chgIData(oldp+238,((((IData)(vlSelfRef.core_top__DOT__u_if_stage__DOT__trap_enter) 
                               | ((~ (IData)(vlSelfRef.interrupt_valid)) 
                                  & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int)))
                               ? ((IData)(vlSelfRef.core_top__DOT__u_if_stage__DOT__trap_enter)
                                   ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                                   : ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int)
                                       ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                                       : 0U)) : (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jal_int) 
                                                  | ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int) 
                                                     | ((3U 
                                                         == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel)) 
                                                        & ((4U 
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
                                                              == vlSelfRef.core_top__DOT__rs2_rdata)))))))
                                                  ? 
                                                 ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int)
                                                   ? 
                                                  (0xfffffffeU 
                                                   & vlSelfRef.core_top__DOT__u_id_stage__DOT__target_sum)
                                                   : vlSelfRef.core_top__DOT__u_id_stage__DOT__target_sum)
                                                  : 
                                                 ((IData)(4U) 
                                                  + vlSelfRef.core_top__DOT__pc)))),32);
    bufp->chgBit(oldp+239,(((~ (IData)(vlSelfRef.interrupt_valid)) 
                            & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int))));
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
