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
        bufp->chgCData(oldp+0,(vlSelfRef.core_top__DOT__rs1_addr),5);
        bufp->chgCData(oldp+1,(vlSelfRef.core_top__DOT__rs2_addr),5);
        bufp->chgCData(oldp+2,(vlSelfRef.core_top__DOT__rd_addr),5);
        bufp->chgBit(oldp+3,(vlSelfRef.core_top__DOT__mem_read));
        bufp->chgBit(oldp+4,(vlSelfRef.core_top__DOT__mem_write));
        bufp->chgBit(oldp+5,(vlSelfRef.core_top__DOT__reg_write));
        bufp->chgBit(oldp+6,(((4U & (IData)(vlSelfRef.core_top__DOT__br_type))
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
        bufp->chgIData(oldp+7,(((0U == (IData)(vlSelfRef.core_top__DOT__reg_wdata_src))
                                 ? vlSelfRef.core_top__DOT__jump_jalr
                                 : ((1U == (IData)(vlSelfRef.core_top__DOT__reg_wdata_src))
                                     ? ((IData)(vlSelfRef.core_top__DOT__mem_read)
                                         ? ((0U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                             ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                                            [(0x3ffffU 
                                              & (vlSelfRef.core_top__DOT__jump_jalr 
                                                 >> 2U))]
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
                                         : 0U) : ((IData)(4U) 
                                                  + vlSelfRef.core_top__DOT__pc)))),32);
        bufp->chgIData(oldp+8,(vlSelfRef.core_top__DOT__rs1_rdata),32);
        bufp->chgIData(oldp+9,(vlSelfRef.core_top__DOT__rs2_rdata),32);
        bufp->chgCData(oldp+10,(vlSelfRef.core_top__DOT__pc_sel),2);
        bufp->chgIData(oldp+11,(vlSelfRef.core_top__DOT__jump_jalr),32);
        bufp->chgIData(oldp+12,(vlSelfRef.core_top__DOT__imm_jal),32);
        bufp->chgIData(oldp+13,(vlSelfRef.core_top__DOT__pc),32);
        bufp->chgIData(oldp+14,(((IData)(4U) + vlSelfRef.core_top__DOT__pc)),32);
        bufp->chgIData(oldp+15,(vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                [(0x3ffffU & (vlSelfRef.core_top__DOT__pc 
                                              >> 2U))]),32);
        bufp->chgCData(oldp+16,(vlSelfRef.core_top__DOT__alu_op),5);
        bufp->chgBit(oldp+17,(vlSelfRef.core_top__DOT__alu_src_a));
        bufp->chgCData(oldp+18,(vlSelfRef.core_top__DOT__alu_src_b),2);
        bufp->chgCData(oldp+19,(vlSelfRef.core_top__DOT__br_type),3);
        bufp->chgCData(oldp+20,(vlSelfRef.core_top__DOT__lsu_type),3);
        bufp->chgCData(oldp+21,(vlSelfRef.core_top__DOT__reg_wdata_src),2);
        bufp->chgIData(oldp+22,(((IData)(vlSelfRef.core_top__DOT__mem_read)
                                  ? ((0U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                      ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                                     [(0x3ffffU & (vlSelfRef.core_top__DOT__jump_jalr 
                                                   >> 2U))]
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
        bufp->chgIData(oldp+23,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1),32);
        bufp->chgIData(oldp+24,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2),32);
        bufp->chgCData(oldp+25,(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode),7);
        bufp->chgCData(oldp+26,(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3),3);
        bufp->chgCData(oldp+27,(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct7),7);
        bufp->chgIData(oldp+28,((((- (IData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                  << 0xcU) | (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                              [(0x3ffffU 
                                                & (vlSelfRef.core_top__DOT__pc 
                                                   >> 2U))] 
                                              >> 0x14U))),32);
        bufp->chgIData(oldp+29,((((- (IData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                  << 0xcU) | (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct7) 
                                               << 5U) 
                                              | (IData)(vlSelfRef.core_top__DOT__rd_addr)))),32);
        bufp->chgIData(oldp+30,((((- (IData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                  << 0xdU) | ((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0) 
                                                << 0xcU) 
                                               | (0x800U 
                                                  & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                     [
                                                     (0x3ffffU 
                                                      & (vlSelfRef.core_top__DOT__pc 
                                                         >> 2U))] 
                                                     << 4U))) 
                                              | ((0x7e0U 
                                                  & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                     [
                                                     (0x3ffffU 
                                                      & (vlSelfRef.core_top__DOT__pc 
                                                         >> 2U))] 
                                                     >> 0x14U)) 
                                                 | (0x1eU 
                                                    & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                       [
                                                       (0x3ffffU 
                                                        & (vlSelfRef.core_top__DOT__pc 
                                                           >> 2U))] 
                                                       >> 7U)))))),32);
        bufp->chgIData(oldp+31,((0xfffff000U & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                 [(0x3ffffU & (vlSelfRef.core_top__DOT__pc 
                                               >> 2U))])),32);
        bufp->chgIData(oldp+32,((((- (IData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                  << 0x14U) | (((0xff000U 
                                                 & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                 [(0x3ffffU 
                                                   & (vlSelfRef.core_top__DOT__pc 
                                                      >> 2U))]) 
                                                | (0x800U 
                                                   & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                      [
                                                      (0x3ffffU 
                                                       & (vlSelfRef.core_top__DOT__pc 
                                                          >> 2U))] 
                                                      >> 9U))) 
                                               | (0x7feU 
                                                  & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                     [
                                                     (0x3ffffU 
                                                      & (vlSelfRef.core_top__DOT__pc 
                                                         >> 2U))] 
                                                     >> 0x14U))))),32);
        bufp->chgSData(oldp+33,((vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                 [(0x3ffffU & (vlSelfRef.core_top__DOT__pc 
                                               >> 2U))] 
                                 >> 0x14U)),12);
        bufp->chgIData(oldp+34,(((0U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                  ? ((IData)(4U) + vlSelfRef.core_top__DOT__pc)
                                  : ((1U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                      ? vlSelfRef.__VdfgRegularize_h3c5bdd75_0_0
                                      : ((2U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                          ? (0xfffffffeU 
                                             & vlSelfRef.core_top__DOT__jump_jalr)
                                          : ((3U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                              ? (((4U 
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
                                                 + vlSelfRef.core_top__DOT__pc)))))),32);
        bufp->chgCData(oldp+35,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wmask),4);
        bufp->chgIData(oldp+36,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                                [(0x3ffffU & (vlSelfRef.core_top__DOT__jump_jalr 
                                              >> 2U))]),32);
        bufp->chgIData(oldp+37,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata),32);
        bufp->chgIData(oldp+38,((0x3ffffU & (vlSelfRef.core_top__DOT__jump_jalr 
                                             >> 2U))),18);
        bufp->chgCData(oldp+39,((3U & vlSelfRef.core_top__DOT__jump_jalr)),2);
        bufp->chgBit(oldp+40,((1U & (vlSelfRef.core_top__DOT__jump_jalr 
                                     >> 1U))));
        bufp->chgCData(oldp+41,((0xffU & vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                                 [(0x3ffffU & (vlSelfRef.core_top__DOT__jump_jalr 
                                               >> 2U))])),8);
        bufp->chgCData(oldp+42,((0xffU & (vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                                          [(0x3ffffU 
                                            & (vlSelfRef.core_top__DOT__jump_jalr 
                                               >> 2U))] 
                                          >> 8U))),8);
        bufp->chgCData(oldp+43,((0xffU & (vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                                          [(0x3ffffU 
                                            & (vlSelfRef.core_top__DOT__jump_jalr 
                                               >> 2U))] 
                                          >> 0x10U))),8);
        bufp->chgCData(oldp+44,((vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                                 [(0x3ffffU & (vlSelfRef.core_top__DOT__jump_jalr 
                                               >> 2U))] 
                                 >> 0x18U)),8);
        bufp->chgSData(oldp+45,((0xffffU & vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                                 [(0x3ffffU & (vlSelfRef.core_top__DOT__jump_jalr 
                                               >> 2U))])),16);
        bufp->chgSData(oldp+46,((vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                                 [(0x3ffffU & (vlSelfRef.core_top__DOT__jump_jalr 
                                               >> 2U))] 
                                 >> 0x10U)),16);
        bufp->chgCData(oldp+47,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected),8);
        bufp->chgSData(oldp+48,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected),16);
        bufp->chgBit(oldp+49,((0U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+50,((1U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+51,((2U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+52,((3U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+53,((4U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+54,((5U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+55,((6U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+56,((7U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgIData(oldp+57,((((- (IData)((1U & 
                                              ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                               >> 0xfU)))) 
                                  << 0x10U) | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))),32);
        bufp->chgIData(oldp+58,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected),32);
        bufp->chgIData(oldp+59,((((- (IData)((1U & 
                                              ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected) 
                                               >> 7U)))) 
                                  << 8U) | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected))),32);
        bufp->chgIData(oldp+60,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected),32);
        bufp->chgIData(oldp+61,(((vlSelfRef.core_top__DOT__rs2_rdata 
                                  << 0x10U) | (0xffffU 
                                               & vlSelfRef.core_top__DOT__rs2_rdata))),32);
        bufp->chgIData(oldp+62,(((vlSelfRef.core_top__DOT__rs2_rdata 
                                  << 0x18U) | ((0xff0000U 
                                                & (vlSelfRef.core_top__DOT__rs2_rdata 
                                                   << 0x10U)) 
                                               | ((0xff00U 
                                                   & (vlSelfRef.core_top__DOT__rs2_rdata 
                                                      << 8U)) 
                                                  | (0xffU 
                                                     & vlSelfRef.core_top__DOT__rs2_rdata))))),32);
        bufp->chgCData(oldp+63,(((2U & vlSelfRef.core_top__DOT__jump_jalr)
                                  ? 0xcU : 3U)),4);
        bufp->chgCData(oldp+64,((0xfU & ((IData)(1U) 
                                         << (3U & vlSelfRef.core_top__DOT__jump_jalr)))),4);
        bufp->chgIData(oldp+65,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[0]),32);
        bufp->chgIData(oldp+66,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[1]),32);
        bufp->chgIData(oldp+67,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[2]),32);
        bufp->chgIData(oldp+68,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[3]),32);
        bufp->chgIData(oldp+69,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[4]),32);
        bufp->chgIData(oldp+70,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[5]),32);
        bufp->chgIData(oldp+71,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[6]),32);
        bufp->chgIData(oldp+72,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[7]),32);
        bufp->chgIData(oldp+73,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[8]),32);
        bufp->chgIData(oldp+74,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[9]),32);
        bufp->chgIData(oldp+75,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[10]),32);
        bufp->chgIData(oldp+76,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[11]),32);
        bufp->chgIData(oldp+77,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[12]),32);
        bufp->chgIData(oldp+78,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[13]),32);
        bufp->chgIData(oldp+79,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[14]),32);
        bufp->chgIData(oldp+80,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[15]),32);
        bufp->chgIData(oldp+81,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[16]),32);
        bufp->chgIData(oldp+82,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[17]),32);
        bufp->chgIData(oldp+83,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[18]),32);
        bufp->chgIData(oldp+84,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[19]),32);
        bufp->chgIData(oldp+85,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[20]),32);
        bufp->chgIData(oldp+86,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[21]),32);
        bufp->chgIData(oldp+87,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[22]),32);
        bufp->chgIData(oldp+88,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[23]),32);
        bufp->chgIData(oldp+89,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[24]),32);
        bufp->chgIData(oldp+90,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[25]),32);
        bufp->chgIData(oldp+91,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[26]),32);
        bufp->chgIData(oldp+92,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[27]),32);
        bufp->chgIData(oldp+93,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[28]),32);
        bufp->chgIData(oldp+94,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[29]),32);
        bufp->chgIData(oldp+95,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[30]),32);
        bufp->chgIData(oldp+96,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[31]),32);
    }
    bufp->chgBit(oldp+97,(vlSelfRef.clk));
    bufp->chgBit(oldp+98,(vlSelfRef.rst));
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
