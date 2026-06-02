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
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[1U]))) {
        bufp->chgBit(oldp+0,(vlSelfRef.core_top__DOT__mem_read));
        bufp->chgBit(oldp+1,(vlSelfRef.core_top__DOT__mem_write));
        bufp->chgBit(oldp+2,(vlSelfRef.core_top__DOT__reg_write));
        bufp->chgBit(oldp+3,(((4U & (IData)(vlSelfRef.core_top__DOT__br_type))
                               ? ((~ ((IData)(vlSelfRef.core_top__DOT__br_type) 
                                      >> 1U)) & ((1U 
                                                  & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                  ? 
                                                 (vlSelfRef.core_top__DOT__rs1_rdata 
                                                  >= vlSelfRef.core_top__DOT__rs2_rdata)
                                                  : 
                                                 (vlSelfRef.core_top__DOT__rs1_rdata 
                                                  < vlSelfRef.core_top__DOT__rs2_rdata)))
                               : ((2U & (IData)(vlSelfRef.core_top__DOT__br_type))
                                   ? ((1U & (IData)(vlSelfRef.core_top__DOT__br_type))
                                       ? VL_GTES_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.core_top__DOT__rs2_rdata)
                                       : VL_LTS_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.core_top__DOT__rs2_rdata))
                                   : ((1U & (IData)(vlSelfRef.core_top__DOT__br_type))
                                       ? (vlSelfRef.core_top__DOT__rs1_rdata 
                                          != vlSelfRef.core_top__DOT__rs2_rdata)
                                       : (vlSelfRef.core_top__DOT__rs1_rdata 
                                          == vlSelfRef.core_top__DOT__rs2_rdata))))));
        bufp->chgIData(oldp+4,(((0U == (IData)(vlSelfRef.core_top__DOT__reg_wdata_src))
                                 ? vlSelfRef.core_top__DOT__jump_jalr
                                 : ((1U == (IData)(vlSelfRef.core_top__DOT__reg_wdata_src))
                                     ? ((IData)(vlSelfRef.core_top__DOT__mem_read)
                                         ? ((0U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                             ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw
                                             : ((1U 
                                                 == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                                 ? 
                                                (((- (IData)(
                                                             (1U 
                                                              & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                                                 >> 0xfU)))) 
                                                  << 0x10U) 
                                                 | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))
                                                 : 
                                                ((2U 
                                                  == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                                  ? 
                                                 (((- (IData)(
                                                              (1U 
                                                               & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected) 
                                                                  >> 7U)))) 
                                                   << 8U) 
                                                  | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected))
                                                  : 
                                                 ((3U 
                                                   == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                                   ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected)
                                                   : 
                                                  ((4U 
                                                    == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                                    ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected)
                                                    : 0U)))))
                                         : 0U) : ((2U 
                                                   == (IData)(vlSelfRef.core_top__DOT__reg_wdata_src))
                                                   ? 
                                                  ((IData)(4U) 
                                                   + vlSelfRef.core_top__DOT__pc)
                                                   : vlSelfRef.core_top__DOT__csr_rdata)))),32);
        bufp->chgIData(oldp+5,(vlSelfRef.core_top__DOT__rs1_rdata),32);
        bufp->chgIData(oldp+6,(vlSelfRef.core_top__DOT__rs2_rdata),32);
        bufp->chgCData(oldp+7,(vlSelfRef.core_top__DOT__pc_sel),2);
        bufp->chgIData(oldp+8,(vlSelfRef.core_top__DOT__jump_jalr),32);
        bufp->chgIData(oldp+9,(vlSelfRef.core_top__DOT__imm_jal),32);
        bufp->chgIData(oldp+10,(vlSelfRef.core_top__DOT__pc),32);
        bufp->chgIData(oldp+11,(((IData)(4U) + vlSelfRef.core_top__DOT__pc)),32);
        bufp->chgCData(oldp+12,(vlSelfRef.core_top__DOT__alu_op),5);
        bufp->chgBit(oldp+13,(vlSelfRef.core_top__DOT__is_ebreak));
        bufp->chgBit(oldp+14,(vlSelfRef.core_top__DOT__alu_src_a));
        bufp->chgCData(oldp+15,(vlSelfRef.core_top__DOT__alu_src_b),2);
        bufp->chgCData(oldp+16,(vlSelfRef.core_top__DOT__br_type),3);
        bufp->chgCData(oldp+17,(vlSelfRef.core_top__DOT__lsu_type),3);
        bufp->chgCData(oldp+18,(vlSelfRef.core_top__DOT__reg_wdata_src),2);
        bufp->chgIData(oldp+19,(((IData)(vlSelfRef.core_top__DOT__mem_read)
                                  ? ((0U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                      ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw
                                      : ((1U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                          ? (((- (IData)(
                                                         (1U 
                                                          & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                                             >> 0xfU)))) 
                                              << 0x10U) 
                                             | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))
                                          : ((2U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                              ? (((- (IData)(
                                                             (1U 
                                                              & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected) 
                                                                 >> 7U)))) 
                                                  << 8U) 
                                                 | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected))
                                              : ((3U 
                                                  == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                                  ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected)
                                                  : 
                                                 ((4U 
                                                   == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                                   ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected)
                                                   : 0U)))))
                                  : 0U)),32);
        bufp->chgCData(oldp+20,(vlSelfRef.core_top__DOT__csr_op),2);
        bufp->chgIData(oldp+21,(vlSelfRef.core_top__DOT__csr_wdata),32);
        bufp->chgBit(oldp+22,(vlSelfRef.core_top__DOT__csr_imm));
        bufp->chgBit(oldp+23,(vlSelfRef.core_top__DOT__csr_write));
        bufp->chgBit(oldp+24,(vlSelfRef.core_top__DOT__csr_read));
        bufp->chgBit(oldp+25,(vlSelfRef.core_top__DOT__mret));
        bufp->chgBit(oldp+26,(vlSelfRef.core_top__DOT__trap_enter));
        bufp->chgIData(oldp+27,(vlSelfRef.core_top__DOT__csr_rdata),32);
        bufp->chgIData(oldp+28,(((IData)(vlSelfRef.core_top__DOT__trap_enter)
                                  ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                                  : ((IData)(vlSelfRef.core_top__DOT__mret)
                                      ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                                      : 0U))),32);
        bufp->chgIData(oldp+29,(vlSelfRef.core_top__DOT__trap_code),32);
        bufp->chgIData(oldp+30,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                                [0xaU]),32);
        bufp->chgIData(oldp+31,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus),32);
        bufp->chgIData(oldp+32,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec),32);
        bufp->chgIData(oldp+33,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc),32);
        bufp->chgIData(oldp+34,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause),32);
        bufp->chgIData(oldp+35,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip),32);
        bufp->chgIData(oldp+36,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mie),32);
        bufp->chgIData(oldp+37,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtval),32);
        bufp->chgIData(oldp+38,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mscratch),32);
        bufp->chgQData(oldp+39,(vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64),64);
        bufp->chgIData(oldp+41,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1),32);
        bufp->chgIData(oldp+42,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2),32);
        bufp->chgIData(oldp+43,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src),32);
        bufp->chgIData(oldp+44,((((IData)(vlSelfRef.core_top__DOT__mret) 
                                  | (IData)(vlSelfRef.core_top__DOT__trap_enter))
                                  ? ((IData)(vlSelfRef.core_top__DOT__trap_enter)
                                      ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                                      : ((IData)(vlSelfRef.core_top__DOT__mret)
                                          ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                                          : 0U)) : 
                                 ((0U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                   ? ((IData)(4U) + vlSelfRef.core_top__DOT__pc)
                                   : ((1U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                       ? vlSelfRef.__VdfgRegularize_h3c5bdd75_0_0
                                       : ((2U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                           ? (0xfffffffeU 
                                              & vlSelfRef.core_top__DOT__jump_jalr)
                                           : ((3U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                               ? ((
                                                   (4U 
                                                    & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                    ? 
                                                   ((~ 
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
                                                    : 
                                                   ((2U 
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
                                                   : 
                                                  ((IData)(4U) 
                                                   + vlSelfRef.core_top__DOT__pc))
                                               : ((IData)(4U) 
                                                  + vlSelfRef.core_top__DOT__pc))))))),32);
        bufp->chgCData(oldp+45,(((IData)(vlSelfRef.core_top__DOT__mem_write)
                                  ? ((5U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                      ? 0xfU : (0xfU 
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
                                  : 0U)),4);
        bufp->chgIData(oldp+46,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw),32);
        bufp->chgIData(oldp+47,(((IData)(vlSelfRef.core_top__DOT__mem_write)
                                  ? ((5U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                      ? vlSelfRef.core_top__DOT__rs2_rdata
                                      : ((6U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
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
        bufp->chgCData(oldp+48,((3U & vlSelfRef.core_top__DOT__jump_jalr)),2);
        bufp->chgBit(oldp+49,((1U & (vlSelfRef.core_top__DOT__jump_jalr 
                                     >> 1U))));
        bufp->chgCData(oldp+50,((0xffU & vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw)),8);
        bufp->chgCData(oldp+51,((0xffU & (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 8U))),8);
        bufp->chgCData(oldp+52,((0xffU & (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 0x10U))),8);
        bufp->chgCData(oldp+53,((vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                 >> 0x18U)),8);
        bufp->chgSData(oldp+54,((0xffffU & vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw)),16);
        bufp->chgSData(oldp+55,((vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                 >> 0x10U)),16);
        bufp->chgCData(oldp+56,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected),8);
        bufp->chgSData(oldp+57,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected),16);
        bufp->chgBit(oldp+58,((0U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+59,((1U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+60,((2U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+61,((3U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+62,((4U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+63,((5U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+64,((6U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+65,((7U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgIData(oldp+66,((((- (IData)((1U & 
                                              ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                               >> 0xfU)))) 
                                  << 0x10U) | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))),32);
        bufp->chgIData(oldp+67,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected),32);
        bufp->chgIData(oldp+68,((((- (IData)((1U & 
                                              ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected) 
                                               >> 7U)))) 
                                  << 8U) | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected))),32);
        bufp->chgIData(oldp+69,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected),32);
        bufp->chgIData(oldp+70,(((vlSelfRef.core_top__DOT__rs2_rdata 
                                  << 0x10U) | (0xffffU 
                                               & vlSelfRef.core_top__DOT__rs2_rdata))),32);
        bufp->chgIData(oldp+71,(((vlSelfRef.core_top__DOT__rs2_rdata 
                                  << 0x18U) | ((0xff0000U 
                                                & (vlSelfRef.core_top__DOT__rs2_rdata 
                                                   << 0x10U)) 
                                               | ((0xff00U 
                                                   & (vlSelfRef.core_top__DOT__rs2_rdata 
                                                      << 8U)) 
                                                  | (0xffU 
                                                     & vlSelfRef.core_top__DOT__rs2_rdata))))),32);
        bufp->chgCData(oldp+72,(((2U & vlSelfRef.core_top__DOT__jump_jalr)
                                  ? 0xcU : 3U)),4);
        bufp->chgCData(oldp+73,((0xfU & ((IData)(1U) 
                                         << (3U & vlSelfRef.core_top__DOT__jump_jalr)))),4);
        bufp->chgIData(oldp+74,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[0]),32);
        bufp->chgIData(oldp+75,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[1]),32);
        bufp->chgIData(oldp+76,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[2]),32);
        bufp->chgIData(oldp+77,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[3]),32);
        bufp->chgIData(oldp+78,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[4]),32);
        bufp->chgIData(oldp+79,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[5]),32);
        bufp->chgIData(oldp+80,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[6]),32);
        bufp->chgIData(oldp+81,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[7]),32);
        bufp->chgIData(oldp+82,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[8]),32);
        bufp->chgIData(oldp+83,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[9]),32);
        bufp->chgIData(oldp+84,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[10]),32);
        bufp->chgIData(oldp+85,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[11]),32);
        bufp->chgIData(oldp+86,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[12]),32);
        bufp->chgIData(oldp+87,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[13]),32);
        bufp->chgIData(oldp+88,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[14]),32);
        bufp->chgIData(oldp+89,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[15]),32);
        bufp->chgIData(oldp+90,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[16]),32);
        bufp->chgIData(oldp+91,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[17]),32);
        bufp->chgIData(oldp+92,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[18]),32);
        bufp->chgIData(oldp+93,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[19]),32);
        bufp->chgIData(oldp+94,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[20]),32);
        bufp->chgIData(oldp+95,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[21]),32);
        bufp->chgIData(oldp+96,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[22]),32);
        bufp->chgIData(oldp+97,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[23]),32);
        bufp->chgIData(oldp+98,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[24]),32);
        bufp->chgIData(oldp+99,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[25]),32);
        bufp->chgIData(oldp+100,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[26]),32);
        bufp->chgIData(oldp+101,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[27]),32);
        bufp->chgIData(oldp+102,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[28]),32);
        bufp->chgIData(oldp+103,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[29]),32);
        bufp->chgIData(oldp+104,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[30]),32);
        bufp->chgIData(oldp+105,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[31]),32);
    }
    bufp->chgBit(oldp+106,(vlSelfRef.clk));
    bufp->chgBit(oldp+107,(vlSelfRef.rst));
    bufp->chgBit(oldp+108,(vlSelfRef.halt));
    bufp->chgIData(oldp+109,(vlSelfRef.instr),32);
    bufp->chgIData(oldp+110,(vlSelfRef.halt_pc),32);
    bufp->chgIData(oldp+111,(vlSelfRef.halt_ret),32);
    bufp->chgCData(oldp+112,((0x1fU & (vlSelfRef.instr 
                                       >> 0xfU))),5);
    bufp->chgCData(oldp+113,((0x1fU & (vlSelfRef.instr 
                                       >> 0x14U))),5);
    bufp->chgCData(oldp+114,((0x1fU & (vlSelfRef.instr 
                                       >> 7U))),5);
    bufp->chgSData(oldp+115,((vlSelfRef.instr >> 0x14U)),12);
    bufp->chgIData(oldp+116,((0x1fU & (vlSelfRef.instr 
                                       >> 0xfU))),32);
    bufp->chgCData(oldp+117,((0x7fU & vlSelfRef.instr)),7);
    bufp->chgCData(oldp+118,((7U & (vlSelfRef.instr 
                                    >> 0xcU))),3);
    bufp->chgCData(oldp+119,((vlSelfRef.instr >> 0x19U)),7);
    bufp->chgIData(oldp+120,((((- (IData)((vlSelfRef.instr 
                                           >> 0x1fU))) 
                               << 0xcU) | (vlSelfRef.instr 
                                           >> 0x14U))),32);
    bufp->chgIData(oldp+121,((((- (IData)((vlSelfRef.instr 
                                           >> 0x1fU))) 
                               << 0xcU) | ((0xfe0U 
                                            & (vlSelfRef.instr 
                                               >> 0x14U)) 
                                           | (0x1fU 
                                              & (vlSelfRef.instr 
                                                 >> 7U))))),32);
    bufp->chgIData(oldp+122,((((- (IData)((vlSelfRef.instr 
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
    bufp->chgIData(oldp+123,((0xfffff000U & vlSelfRef.instr)),32);
    bufp->chgIData(oldp+124,(((((- (IData)((vlSelfRef.instr 
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
}
