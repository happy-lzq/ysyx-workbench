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
        bufp->chgIData(oldp+6,(vlSelfRef.core_top__DOT__rs1_rdata),32);
        bufp->chgCData(oldp+7,(vlSelfRef.core_top__DOT__pc_sel),2);
        bufp->chgIData(oldp+8,(vlSelfRef.core_top__DOT__imm_jal),32);
        bufp->chgIData(oldp+9,(vlSelfRef.core_top__DOT__pc),32);
        bufp->chgIData(oldp+10,(((IData)(4U) + vlSelfRef.core_top__DOT__pc)),32);
        bufp->chgIData(oldp+11,(vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                [(0x1ffffffU & ((vlSelfRef.core_top__DOT__pc 
                                                 - (IData)(0x80000000U)) 
                                                >> 2U))]),32);
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
        bufp->chgSData(oldp+21,(vlSelfRef.core_top__DOT__csr_addr),12);
        bufp->chgIData(oldp+22,(vlSelfRef.core_top__DOT__csr_wdata),32);
        bufp->chgBit(oldp+23,(vlSelfRef.core_top__DOT__csr_imm));
        bufp->chgBit(oldp+24,(vlSelfRef.core_top__DOT__csr_write));
        bufp->chgBit(oldp+25,(vlSelfRef.core_top__DOT__csr_read));
        bufp->chgBit(oldp+26,(vlSelfRef.core_top__DOT__mret));
        bufp->chgBit(oldp+27,(vlSelfRef.core_top__DOT__trap_enter));
        bufp->chgIData(oldp+28,(vlSelfRef.core_top__DOT__csr_rdata),32);
        bufp->chgIData(oldp+29,(((IData)(vlSelfRef.core_top__DOT__trap_enter)
                                  ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                                  : ((IData)(vlSelfRef.core_top__DOT__mret)
                                      ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                                      : 0U))),32);
        bufp->chgIData(oldp+30,(vlSelfRef.core_top__DOT__rs1_addr),32);
        bufp->chgIData(oldp+31,(vlSelfRef.core_top__DOT__trap_code),32);
        bufp->chgIData(oldp+32,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                                [0xaU]),32);
        bufp->chgIData(oldp+33,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus),32);
        bufp->chgIData(oldp+34,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec),32);
        bufp->chgIData(oldp+35,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc),32);
        bufp->chgIData(oldp+36,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause),32);
        bufp->chgIData(oldp+37,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip),32);
        bufp->chgIData(oldp+38,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mie),32);
        bufp->chgIData(oldp+39,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtval),32);
        bufp->chgIData(oldp+40,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mscratch),32);
        bufp->chgQData(oldp+41,(vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64),64);
        bufp->chgIData(oldp+43,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1),32);
        bufp->chgIData(oldp+44,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2),32);
        bufp->chgIData(oldp+45,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src),32);
        bufp->chgCData(oldp+46,(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode),7);
        bufp->chgCData(oldp+47,(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3),3);
        bufp->chgCData(oldp+48,(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct7),7);
        bufp->chgIData(oldp+49,((((- (IData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                  << 0xcU) | (IData)(vlSelfRef.core_top__DOT__csr_addr))),32);
        bufp->chgIData(oldp+50,((((- (IData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                  << 0xcU) | (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct7) 
                                               << 5U) 
                                              | (IData)(vlSelfRef.core_top__DOT__rd_addr)))),32);
        bufp->chgIData(oldp+51,((((- (IData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                  << 0xdU) | ((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0) 
                                                << 0xcU) 
                                               | (0x800U 
                                                  & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                     [
                                                     (0x1ffffffU 
                                                      & ((vlSelfRef.core_top__DOT__pc 
                                                          - (IData)(0x80000000U)) 
                                                         >> 2U))] 
                                                     << 4U))) 
                                              | ((0x7e0U 
                                                  & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                     [
                                                     (0x1ffffffU 
                                                      & ((vlSelfRef.core_top__DOT__pc 
                                                          - (IData)(0x80000000U)) 
                                                         >> 2U))] 
                                                     >> 0x14U)) 
                                                 | (0x1eU 
                                                    & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                       [
                                                       (0x1ffffffU 
                                                        & ((vlSelfRef.core_top__DOT__pc 
                                                            - (IData)(0x80000000U)) 
                                                           >> 2U))] 
                                                       >> 7U)))))),32);
        bufp->chgIData(oldp+52,((0xfffff000U & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                 [(0x1ffffffU & ((vlSelfRef.core_top__DOT__pc 
                                                  - (IData)(0x80000000U)) 
                                                 >> 2U))])),32);
        bufp->chgIData(oldp+53,((((- (IData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                  << 0x14U) | (((0xff000U 
                                                 & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                 [(0x1ffffffU 
                                                   & ((vlSelfRef.core_top__DOT__pc 
                                                       - (IData)(0x80000000U)) 
                                                      >> 2U))]) 
                                                | (0x800U 
                                                   & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                      [
                                                      (0x1ffffffU 
                                                       & ((vlSelfRef.core_top__DOT__pc 
                                                           - (IData)(0x80000000U)) 
                                                          >> 2U))] 
                                                      >> 9U))) 
                                               | (0x7feU 
                                                  & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                     [
                                                     (0x1ffffffU 
                                                      & ((vlSelfRef.core_top__DOT__pc 
                                                          - (IData)(0x80000000U)) 
                                                         >> 2U))] 
                                                     >> 0x14U))))),32);
        bufp->chgIData(oldp+54,((vlSelfRef.core_top__DOT__pc 
                                 - (IData)(0x80000000U))),32);
        bufp->chgIData(oldp+55,((0x1ffffffU & ((vlSelfRef.core_top__DOT__pc 
                                                - (IData)(0x80000000U)) 
                                               >> 2U))),25);
        bufp->chgIData(oldp+56,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw),32);
        bufp->chgIData(oldp+57,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata),32);
        bufp->chgIData(oldp+58,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__dev_rdata),32);
        bufp->chgBit(oldp+59,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__in_pmem));
        bufp->chgCData(oldp+60,((0xffU & vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw)),8);
        bufp->chgCData(oldp+61,((0xffU & (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 8U))),8);
        bufp->chgCData(oldp+62,((0xffU & (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 0x10U))),8);
        bufp->chgCData(oldp+63,((vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                 >> 0x18U)),8);
        bufp->chgSData(oldp+64,((0xffffU & vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw)),16);
        bufp->chgSData(oldp+65,((vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                 >> 0x10U)),16);
        bufp->chgCData(oldp+66,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected),8);
        bufp->chgSData(oldp+67,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected),16);
        bufp->chgBit(oldp+68,((0U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+69,((1U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+70,((2U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+71,((3U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+72,((4U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+73,((5U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+74,((6U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgBit(oldp+75,((7U == (IData)(vlSelfRef.core_top__DOT__lsu_type))));
        bufp->chgIData(oldp+76,((((- (IData)((1U & 
                                              ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                               >> 0xfU)))) 
                                  << 0x10U) | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))),32);
        bufp->chgIData(oldp+77,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected),32);
        bufp->chgIData(oldp+78,((((- (IData)((1U & 
                                              ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected) 
                                               >> 7U)))) 
                                  << 8U) | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected))),32);
        bufp->chgIData(oldp+79,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected),32);
        bufp->chgIData(oldp+80,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[0]),32);
        bufp->chgIData(oldp+81,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[1]),32);
        bufp->chgIData(oldp+82,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[2]),32);
        bufp->chgIData(oldp+83,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[3]),32);
        bufp->chgIData(oldp+84,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[4]),32);
        bufp->chgIData(oldp+85,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[5]),32);
        bufp->chgIData(oldp+86,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[6]),32);
        bufp->chgIData(oldp+87,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[7]),32);
        bufp->chgIData(oldp+88,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[8]),32);
        bufp->chgIData(oldp+89,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[9]),32);
        bufp->chgIData(oldp+90,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[10]),32);
        bufp->chgIData(oldp+91,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[11]),32);
        bufp->chgIData(oldp+92,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[12]),32);
        bufp->chgIData(oldp+93,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[13]),32);
        bufp->chgIData(oldp+94,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[14]),32);
        bufp->chgIData(oldp+95,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[15]),32);
        bufp->chgIData(oldp+96,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[16]),32);
        bufp->chgIData(oldp+97,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[17]),32);
        bufp->chgIData(oldp+98,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[18]),32);
        bufp->chgIData(oldp+99,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[19]),32);
        bufp->chgIData(oldp+100,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[20]),32);
        bufp->chgIData(oldp+101,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[21]),32);
        bufp->chgIData(oldp+102,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[22]),32);
        bufp->chgIData(oldp+103,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[23]),32);
        bufp->chgIData(oldp+104,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[24]),32);
        bufp->chgIData(oldp+105,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[25]),32);
        bufp->chgIData(oldp+106,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[26]),32);
        bufp->chgIData(oldp+107,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[27]),32);
        bufp->chgIData(oldp+108,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[28]),32);
        bufp->chgIData(oldp+109,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[29]),32);
        bufp->chgIData(oldp+110,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[30]),32);
        bufp->chgIData(oldp+111,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[31]),32);
    }
    bufp->chgBit(oldp+112,(vlSelfRef.clk));
    bufp->chgBit(oldp+113,(vlSelfRef.rst));
    bufp->chgBit(oldp+114,(vlSelfRef.halt));
    bufp->chgIData(oldp+115,(vlSelfRef.instr),32);
    bufp->chgIData(oldp+116,(vlSelfRef.halt_pc),32);
    bufp->chgIData(oldp+117,(vlSelfRef.halt_ret),32);
    bufp->chgBit(oldp+118,(vlSelfRef.dev_req));
    bufp->chgBit(oldp+119,(vlSelfRef.dev_r_w));
    bufp->chgIData(oldp+120,(vlSelfRef.dev_addr),32);
    bufp->chgIData(oldp+121,(vlSelfRef.dev_wdata),32);
    bufp->chgCData(oldp+122,(vlSelfRef.dev_wmask),4);
    bufp->chgBit(oldp+123,(((4U & (IData)(vlSelfRef.core_top__DOT__br_type))
                             ? ((~ ((IData)(vlSelfRef.core_top__DOT__br_type) 
                                    >> 1U)) & ((1U 
                                                & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                ? (vlSelfRef.core_top__DOT__rs1_rdata 
                                                   >= vlSelfRef.dev_wdata)
                                                : (vlSelfRef.core_top__DOT__rs1_rdata 
                                                   < vlSelfRef.dev_wdata)))
                             : ((2U & (IData)(vlSelfRef.core_top__DOT__br_type))
                                 ? ((1U & (IData)(vlSelfRef.core_top__DOT__br_type))
                                     ? VL_GTES_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.dev_wdata)
                                     : VL_LTS_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.dev_wdata))
                                 : ((1U & (IData)(vlSelfRef.core_top__DOT__br_type))
                                     ? (vlSelfRef.core_top__DOT__rs1_rdata 
                                        != vlSelfRef.dev_wdata)
                                     : (vlSelfRef.core_top__DOT__rs1_rdata 
                                        == vlSelfRef.dev_wdata))))));
    bufp->chgIData(oldp+124,(((0U == (IData)(vlSelfRef.core_top__DOT__reg_wdata_src))
                               ? vlSelfRef.dev_addr
                               : ((1U == (IData)(vlSelfRef.core_top__DOT__reg_wdata_src))
                                   ? ((IData)(vlSelfRef.core_top__DOT__mem_read)
                                       ? ((0U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                           ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw
                                           : ((1U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                               ? ((
                                                   (- (IData)(
                                                              (1U 
                                                               & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                                                  >> 0xfU)))) 
                                                   << 0x10U) 
                                                  | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))
                                               : ((2U 
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
    bufp->chgIData(oldp+125,((((IData)(vlSelfRef.core_top__DOT__mret) 
                               | (IData)(vlSelfRef.core_top__DOT__trap_enter))
                               ? ((IData)(vlSelfRef.core_top__DOT__trap_enter)
                                   ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                                   : ((IData)(vlSelfRef.core_top__DOT__mret)
                                       ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                                       : 0U)) : ((0U 
                                                  == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                                  ? 
                                                 ((IData)(4U) 
                                                  + vlSelfRef.core_top__DOT__pc)
                                                  : 
                                                 ((1U 
                                                   == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                                   ? vlSelfRef.__VdfgRegularize_h3c5bdd75_0_0
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                                    ? 
                                                   (0xfffffffeU 
                                                    & vlSelfRef.dev_addr)
                                                    : 
                                                   ((3U 
                                                     == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                                     ? 
                                                    (((4U 
                                                       & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                       ? 
                                                      ((~ 
                                                        ((IData)(vlSelfRef.core_top__DOT__br_type) 
                                                         >> 1U)) 
                                                       & ((1U 
                                                           & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                           ? 
                                                          (vlSelfRef.core_top__DOT__rs1_rdata 
                                                           >= vlSelfRef.dev_wdata)
                                                           : 
                                                          (vlSelfRef.core_top__DOT__rs1_rdata 
                                                           < vlSelfRef.dev_wdata)))
                                                       : 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                         ? 
                                                        VL_GTES_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.dev_wdata)
                                                         : 
                                                        VL_LTS_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.dev_wdata))
                                                        : 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.core_top__DOT__br_type))
                                                         ? 
                                                        (vlSelfRef.core_top__DOT__rs1_rdata 
                                                         != vlSelfRef.dev_wdata)
                                                         : 
                                                        (vlSelfRef.core_top__DOT__rs1_rdata 
                                                         == vlSelfRef.dev_wdata))))
                                                      ? vlSelfRef.__VdfgRegularize_h3c5bdd75_0_0
                                                      : 
                                                     ((IData)(4U) 
                                                      + vlSelfRef.core_top__DOT__pc))
                                                     : 
                                                    ((IData)(4U) 
                                                     + vlSelfRef.core_top__DOT__pc))))))),32);
    bufp->chgIData(oldp+126,((vlSelfRef.dev_addr - (IData)(0x80000000U))),32);
    bufp->chgIData(oldp+127,((0x1ffffffU & ((vlSelfRef.dev_addr 
                                             - (IData)(0x80000000U)) 
                                            >> 2U))),25);
    bufp->chgCData(oldp+128,((3U & vlSelfRef.dev_addr)),2);
    bufp->chgBit(oldp+129,((1U & (vlSelfRef.dev_addr 
                                  >> 1U))));
    bufp->chgIData(oldp+130,(((vlSelfRef.dev_wdata 
                               << 0x10U) | (0xffffU 
                                            & vlSelfRef.dev_wdata))),32);
    bufp->chgIData(oldp+131,(((vlSelfRef.dev_wdata 
                               << 0x18U) | ((0xff0000U 
                                             & (vlSelfRef.dev_wdata 
                                                << 0x10U)) 
                                            | ((0xff00U 
                                                & (vlSelfRef.dev_wdata 
                                                   << 8U)) 
                                               | (0xffU 
                                                  & vlSelfRef.dev_wdata))))),32);
    bufp->chgCData(oldp+132,(((2U & vlSelfRef.dev_addr)
                               ? 0xcU : 3U)),4);
    bufp->chgCData(oldp+133,((0xfU & ((IData)(1U) << 
                                      (3U & vlSelfRef.dev_addr)))),4);
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
