// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vcore_top__Syms.h"


VL_ATTR_COLD void Vcore_top___024root__trace_init_sub__TOP__0(Vcore_top___024root* vlSelf, VerilatedVcd* tracep) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root__trace_init_sub__TOP__0\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->pushPrefix("$rootio", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+231,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+232,0,"rst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+233,0,"interrupt_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+234,0,"interrupt_cause",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+235,0,"halt",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+236,0,"instr",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+237,0,"halt_pc",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+238,0,"halt_ret",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("core_top", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+231,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+232,0,"rst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+233,0,"interrupt_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+234,0,"interrupt_cause",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+235,0,"halt",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+236,0,"instr",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+237,0,"halt_pc",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+238,0,"halt_ret",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+27,0,"pc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+28,0,"pc_plus4",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+29,0,"rs1_rdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+30,0,"rs2_rdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+31,0,"rd_wdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+32,0,"alu_result",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+33,0,"imm_out",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+241,0,"mem_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+242,0,"mem_wdata_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+1,0,"mem_rdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+32,0,"jump_jalr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+33,0,"imm_jal",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+33,0,"imm_br",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+34,0,"csr_rdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+35,0,"csr_wdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+2,0,"trap_target",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+243,0,"trap_pc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+36,0,"rs_a0",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+37,0,"rs1_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+38,0,"rs2_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+39,0,"ex_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+40,0,"mem_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+41,0,"wb_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declQuad(c+42,0,"csr_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+44,0,"sys_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->declQuad(c+46,0,"pc_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 39,0);
    tracep->declBus(c+48,0,"rd_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBit(c+49,0,"reg_write_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+50,0,"is_ebreak_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+3,0,"reg_write_eff",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+4,0,"is_ebreak_eff",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+244,0,"if_valid",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+51,0,"redirect_pc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+52,0,"if_id_valid",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+53,0,"if_id_pc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+54,0,"if_id_pc_plus4",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+55,0,"if_id_inst",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+56,0,"id_ex_valid",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+57,0,"id_ex_pc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+58,0,"id_ex_pc_plus4",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+59,0,"id_ex_inst",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+60,0,"id_ex_rs1_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+61,0,"id_ex_rs2_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+62,0,"id_ex_rd_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+63,0,"id_ex_rs1_rdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+64,0,"id_ex_rs2_rdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+65,0,"id_ex_imm_out",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+66,0,"id_ex_ex_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+67,0,"id_ex_mem_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+68,0,"id_ex_wb_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declQuad(c+69,0,"id_ex_csr_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+71,0,"id_ex_sys_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->declBit(c+73,0,"ex_mem_valid",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+74,0,"ex_mem_pc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+75,0,"ex_mem_pc_plus4",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+76,0,"ex_mem_inst",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+77,0,"ex_mem_rd_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+78,0,"ex_mem_alu_result",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+79,0,"ex_mem_rs2_rdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+80,0,"ex_mem_csr_rdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+81,0,"ex_mem_csr_wdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+82,0,"ex_mem_mem_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+83,0,"ex_mem_wb_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declQuad(c+84,0,"ex_mem_csr_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+86,0,"ex_mem_sys_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->declBit(c+88,0,"mem_wb_valid",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+89,0,"mem_wb_pc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+90,0,"mem_wb_pc_plus4",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+91,0,"mem_wb_inst",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+92,0,"mem_wb_rd_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+93,0,"mem_wb_alu_result",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+94,0,"mem_wb_mem_rdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+95,0,"mem_wb_csr_rdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+96,0,"mem_wb_csr_wdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+97,0,"mem_wb_wb_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declQuad(c+98,0,"mem_wb_csr_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+100,0,"mem_wb_sys_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->pushPrefix("u_csr", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+231,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+232,0,"rst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+88,0,"csr_commit_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declQuad(c+69,0,"csr_read_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+98,0,"csr_write_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+44,0,"sys_redirect_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->declQuad(c+100,0,"sys_commit_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->declBus(c+233,0,"interrupt_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+234,0,"interrupt_cause",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+96,0,"csr_wdata",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+89,0,"trap_pc",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+34,0,"csr_rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+2,0,"trap_target",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+102,0,"csr_read_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBit(c+103,0,"csr_read",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+104,0,"csr_write_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBit(c+105,0,"csr_write_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+106,0,"redirect_trap",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+107,0,"redirect_mret",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+108,0,"commit_trap_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+109,0,"commit_mret_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+110,0,"commit_trap_code_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+5,0,"csr_write",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+6,0,"trap_enter",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+7,0,"mret",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+8,0,"trap_code",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+245,0,"mstatus",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+246,0,"mtvec",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+247,0,"mepc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+248,0,"mcause",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+249,0,"mcyclel",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+250,0,"mcycleh",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+251,0,"mip",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+252,0,"mie",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+253,0,"mtval",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+254,0,"mscratch",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+255,0,"IRQ_M_TIMER",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+256,0,"IRQ_M_EXT",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+111,0,"csr_mstatus",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+112,0,"csr_mtvec",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+113,0,"csr_mepc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+114,0,"csr_mcause",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+115,0,"csr_mip",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+116,0,"csr_mie",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+117,0,"csr_mtval",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+118,0,"csr_mscratch",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declQuad(c+119,0,"mcycle_64",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 63,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_ex_mem_reg", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBit(c+231,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+232,0,"rst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+56,0,"ex_mem_valid_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+57,0,"ex_mem_pc_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+58,0,"ex_mem_pc_plus4_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+59,0,"ex_mem_inst_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+62,0,"ex_mem_rd_addr_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+32,0,"ex_mem_alu_result_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+64,0,"ex_mem_rs2_rdata_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+34,0,"ex_mem_csr_rdata_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+35,0,"ex_mem_csr_wdata_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+67,0,"ex_mem_mem_ctrl_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+68,0,"ex_mem_wb_ctrl_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declQuad(c+69,0,"ex_mem_csr_ctrl_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+71,0,"ex_mem_sys_ctrl_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->declBit(c+73,0,"ex_mem_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+74,0,"ex_mem_pc",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+75,0,"ex_mem_pc_plus4",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+76,0,"ex_mem_inst",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+77,0,"ex_mem_rd_addr",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+78,0,"ex_mem_alu_result",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+79,0,"ex_mem_rs2_rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+80,0,"ex_mem_csr_rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+81,0,"ex_mem_csr_wdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+82,0,"ex_mem_mem_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+83,0,"ex_mem_wb_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declQuad(c+84,0,"ex_mem_csr_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+86,0,"ex_mem_sys_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_ex_stage", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+57,0,"pc",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+63,0,"rs1_rdata",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+64,0,"rs2_rdata",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+65,0,"imm_out",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+34,0,"csr_rdata",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+66,0,"ex_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declQuad(c+69,0,"csr_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declBus(c+32,0,"alu_result",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+35,0,"csr_wdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+121,0,"alu_op",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBit(c+122,0,"alu_src_a",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+123,0,"alu_src_b",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+124,0,"csr_op",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBit(c+125,0,"csr_imm",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+126,0,"csr_zimm",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+127,0,"src1",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+128,0,"src2",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+129,0,"csr_src",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->pushPrefix("alu_pic", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+127,0,"src1",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+128,0,"src2",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+121,0,"alu_op",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+32,0,"result",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+130,0,"signed_div32__Vstatic__abs_dividend",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+131,0,"signed_div32__Vstatic__abs_divisor",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+132,0,"signed_div32__Vstatic__quotient",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+133,0,"signed_rem32__Vstatic__abs_dividend",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+134,0,"signed_rem32__Vstatic__abs_divisor",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+135,0,"signed_rem32__Vstatic__remainder",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declQuad(c+136,0,"src1_s64",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 63,0);
    tracep->declQuad(c+138,0,"src2_s64",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 63,0);
    tracep->declQuad(c+140,0,"mul_ss",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 63,0);
    tracep->declQuad(c+142,0,"src1_u64",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 63,0);
    tracep->declQuad(c+144,0,"src2_u64",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 63,0);
    tracep->declQuad(c+146,0,"mul_uu",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 63,0);
    tracep->declQuad(c+148,0,"mul_su",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 63,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("u_halt", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+257,0,"RESET_VECTOR",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+258,0,"RESET_x10",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+231,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+232,0,"rst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+4,0,"is_ebreak",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+89,0,"pc",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+36,0,"rs_a0",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+235,0,"halt_reg",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+237,0,"halt_pc",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+238,0,"halt_ret",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_id_ex_reg", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBit(c+231,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+232,0,"rst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+52,0,"id_ex_valid_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+53,0,"id_ex_pc_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+54,0,"id_ex_pc_plus4_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+55,0,"id_ex_inst_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+37,0,"id_ex_rs1_addr_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+38,0,"id_ex_rs2_addr_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+48,0,"id_ex_rd_addr_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+29,0,"id_ex_rs1_rdata_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+30,0,"id_ex_rs2_rdata_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+33,0,"id_ex_imm_out_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+39,0,"id_ex_ex_ctrl_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+40,0,"id_ex_mem_ctrl_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+41,0,"id_ex_wb_ctrl_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declQuad(c+42,0,"id_ex_csr_ctrl_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+44,0,"id_ex_sys_ctrl_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->declBit(c+56,0,"id_ex_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+57,0,"id_ex_pc",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+58,0,"id_ex_pc_plus4",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+59,0,"id_ex_inst",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+60,0,"id_ex_rs1_addr",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+61,0,"id_ex_rs2_addr",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+62,0,"id_ex_rd_addr",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+63,0,"id_ex_rs1_rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+64,0,"id_ex_rs2_rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+65,0,"id_ex_imm_out",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+66,0,"id_ex_ex_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+67,0,"id_ex_mem_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+68,0,"id_ex_wb_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declQuad(c+69,0,"id_ex_csr_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+71,0,"id_ex_sys_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_id_stage", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+55,0,"inst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+53,0,"if_id_pc",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+29,0,"rs1_data",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+30,0,"rs2_data",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+37,0,"rs1_addr",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+38,0,"rs2_addr",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+48,0,"rd_addr",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+33,0,"imm_out",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+51,0,"redirect_pc",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+39,0,"ex_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+40,0,"mem_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+41,0,"wb_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declQuad(c+42,0,"csr_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+44,0,"sys_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->declQuad(c+46,0,"pc_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 39,0);
    tracep->declBus(c+150,0,"opcode",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 6,0);
    tracep->declBus(c+151,0,"funct3",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+152,0,"funct7",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 6,0);
    tracep->declBus(c+153,0,"br_taken",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declQuad(c+154,0,"pc_ctrl_int",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 39,0);
    tracep->declBus(c+156,0,"br_type",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+157,0,"is_jalr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+158,0,"target_sum",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->pushPrefix("u_br_cond", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+156,0,"br_type",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+29,0,"rs1_data",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+30,0,"rs2_data",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+153,0,"br_taken",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_control", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+55,0,"inst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+150,0,"opcode",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 6,0);
    tracep->declBus(c+151,0,"funct3",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+152,0,"funct7",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 6,0);
    tracep->declBus(c+39,0,"ex_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+40,0,"mem_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+41,0,"wb_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declQuad(c+42,0,"csr_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+44,0,"sys_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->declQuad(c+154,0,"pc_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 39,0);
    tracep->declBus(c+159,0,"alu_op",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+160,0,"alu_src_a",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+161,0,"alu_src_b",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+156,0,"br_type",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+162,0,"mem_read",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+163,0,"mem_write",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+164,0,"lsu_type",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+165,0,"reg_write",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+166,0,"reg_wdata_src",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+167,0,"csr_read",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+168,0,"csr_write",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+169,0,"csr_imm",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+170,0,"csr_op",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+171,0,"pc_sel",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+106,0,"trap_enter_int",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+172,0,"is_ebreak_int",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+107,0,"mret_int",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+173,0,"is_jal_int",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+157,0,"is_jalr_int",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+174,0,"trap_code_int",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+37,0,"rs1_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+175,0,"funct12",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+175,0,"csr_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+176,0,"csr_zimm",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_imm_gen", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+55,0,"inst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+33,0,"imm_out",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+177,0,"imm_i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+178,0,"imm_s",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+179,0,"imm_b",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+180,0,"imm_u",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+181,0,"imm_j",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("u_if_id_reg", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBit(c+231,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+232,0,"rst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+244,0,"if_id_valid_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+27,0,"if_id_pc_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+28,0,"if_id_pc_plus4_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+236,0,"if_id_inst_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+52,0,"if_id_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+53,0,"if_id_pc",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+54,0,"if_id_pc_plus4",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+55,0,"if_id_inst",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_if_stage", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+231,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+232,0,"rst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declQuad(c+46,0,"pc_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 39,0);
    tracep->declQuad(c+44,0,"sys_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->declBus(c+233,0,"interrupt_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+51,0,"redirect_pc",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+2,0,"trap_target",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+27,0,"pc",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+28,0,"pc_plus4",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+236,0,"inst",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+239,0,"pc_next",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+171,0,"pc_sel",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBit(c+153,0,"br_taken",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+173,0,"is_jal",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+157,0,"is_jalr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+106,0,"trap_enter_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+107,0,"mret_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+9,0,"trap_enter",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+240,0,"mret",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+182,0,"redirect_valid",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->popPrefix();
    tracep->pushPrefix("u_mem_stage", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+231,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+73,0,"mem_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+233,0,"interrupt_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+78,0,"mem_addr",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+79,0,"mem_wdata_raw",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+82,0,"mem_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+1,0,"mem_rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+10,0,"mem_read",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+11,0,"mem_write",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+183,0,"lsu_type",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+12,0,"mem_wmask",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 3,0);
    tracep->declBus(c+13,0,"mem_rdata_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+14,0,"mem_wdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->pushPrefix("lsu_pic", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+78,0,"mem_addr",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+13,0,"mem_rdata_raw",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+79,0,"mem_wdata_raw",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+183,0,"lsu_type",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+10,0,"mem_read",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+11,0,"mem_write",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+1,0,"mem_rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+14,0,"mem_wdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+12,0,"mem_wmask",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 3,0);
    tracep->declBus(c+184,0,"sel_byte",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+185,0,"sel_half",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+15,0,"rdata_byte_0",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+16,0,"rdata_byte_1",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+17,0,"rdata_byte_2",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+18,0,"rdata_byte_3",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+19,0,"rdata_half_0",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 15,0);
    tracep->declBus(c+20,0,"rdata_half_1",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 15,0);
    tracep->declBus(c+21,0,"byte_selected",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+22,0,"half_selected",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 15,0);
    tracep->declBit(c+186,0,"is_lw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+187,0,"is_lh",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+188,0,"is_lb",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+189,0,"is_lhu",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+190,0,"is_lbu",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+191,0,"is_sw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+192,0,"is_sh",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+193,0,"is_sb",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+23,0,"lh_result",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+24,0,"lhu_result",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+25,0,"lb_result",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+26,0,"lbu_result",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+79,0,"sw_wdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+194,0,"sh_wdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+195,0,"sb_wdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+259,0,"sw_mask",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 3,0);
    tracep->declBus(c+196,0,"sh_mask",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 3,0);
    tracep->declBus(c+197,0,"sb_mask",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 3,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("u_mem_wb_reg", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBit(c+231,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+232,0,"rst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+73,0,"mem_wb_valid_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+74,0,"mem_wb_pc_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+75,0,"mem_wb_pc_plus4_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+76,0,"mem_wb_inst_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+77,0,"mem_wb_rd_addr_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+78,0,"mem_wb_alu_result_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+1,0,"mem_wb_mem_rdata_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+83,0,"mem_wb_wb_ctrl_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+80,0,"mem_wb_csr_rdata_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+81,0,"mem_wb_csr_wdata_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declQuad(c+84,0,"mem_wb_csr_ctrl_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+86,0,"mem_wb_sys_ctrl_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->declBit(c+88,0,"mem_wb_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+89,0,"mem_wb_pc",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+90,0,"mem_wb_pc_plus4",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+91,0,"mem_wb_inst",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+92,0,"mem_wb_rd_addr",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+93,0,"mem_wb_alu_result",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+94,0,"mem_wb_mem_rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+97,0,"mem_wb_wb_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+95,0,"mem_wb_csr_rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+96,0,"mem_wb_csr_wdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declQuad(c+98,0,"mem_wb_csr_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+100,0,"mem_wb_sys_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_regfile", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+231,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+3,0,"reg_write",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+37,0,"rs1_addr",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+38,0,"rs2_addr",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+92,0,"rd_addr",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+31,0,"rd_wdata",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+29,0,"rs1_rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+30,0,"rs2_rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+36,0,"rs_a0",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->pushPrefix("rf", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 32; ++i) {
        tracep->declBus(c+198+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 31,0);
    }
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("u_wb_stage", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+97,0,"wb_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+94,0,"mem_rdata",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+93,0,"alu_result",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+90,0,"pc_plus4",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+95,0,"csr_rdata",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+31,0,"rd_wdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+230,0,"rd_wdata_src",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->popPrefix();
    tracep->popPrefix();
}

VL_ATTR_COLD void Vcore_top___024root__trace_init_top(Vcore_top___024root* vlSelf, VerilatedVcd* tracep) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root__trace_init_top\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vcore_top___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void Vcore_top___024root__trace_const_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
VL_ATTR_COLD void Vcore_top___024root__trace_full_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vcore_top___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vcore_top___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void Vcore_top___024root__trace_register(Vcore_top___024root* vlSelf, VerilatedVcd* tracep) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root__trace_register\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    tracep->addConstCb(&Vcore_top___024root__trace_const_0, 0U, vlSelf);
    tracep->addFullCb(&Vcore_top___024root__trace_full_0, 0U, vlSelf);
    tracep->addChgCb(&Vcore_top___024root__trace_chg_0, 0U, vlSelf);
    tracep->addCleanupCb(&Vcore_top___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void Vcore_top___024root__trace_const_0_sub_0(Vcore_top___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vcore_top___024root__trace_const_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root__trace_const_0\n"); );
    // Init
    Vcore_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vcore_top___024root*>(voidSelf);
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vcore_top___024root__trace_const_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vcore_top___024root__trace_const_0_sub_0(Vcore_top___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root__trace_const_0_sub_0\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullIData(oldp+241,(vlSelfRef.core_top__DOT__mem_addr),32);
    bufp->fullIData(oldp+242,(vlSelfRef.core_top__DOT__mem_wdata_raw),32);
    bufp->fullIData(oldp+243,(vlSelfRef.core_top__DOT__trap_pc),32);
    bufp->fullBit(oldp+244,(1U));
    bufp->fullSData(oldp+245,(0x300U),12);
    bufp->fullSData(oldp+246,(0x305U),12);
    bufp->fullSData(oldp+247,(0x341U),12);
    bufp->fullSData(oldp+248,(0x342U),12);
    bufp->fullSData(oldp+249,(0xb00U),12);
    bufp->fullSData(oldp+250,(0xb80U),12);
    bufp->fullSData(oldp+251,(0x344U),12);
    bufp->fullSData(oldp+252,(0x304U),12);
    bufp->fullSData(oldp+253,(0x343U),12);
    bufp->fullSData(oldp+254,(0x340U),12);
    bufp->fullIData(oldp+255,(0x80000007U),32);
    bufp->fullIData(oldp+256,(0x8000000bU),32);
    bufp->fullIData(oldp+257,(0x80000000U),32);
    bufp->fullIData(oldp+258,(0U),32);
    bufp->fullCData(oldp+259,(0xfU),4);
}

VL_ATTR_COLD void Vcore_top___024root__trace_full_0_sub_0(Vcore_top___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vcore_top___024root__trace_full_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root__trace_full_0\n"); );
    // Init
    Vcore_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vcore_top___024root*>(voidSelf);
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vcore_top___024root__trace_full_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vcore_top___024root__trace_full_0_sub_0(Vcore_top___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root__trace_full_0_sub_0\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullIData(oldp+1,(((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_read)
                              ? ((0U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                  ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw
                                  : ((1U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                      ? (((- (IData)(
                                                     (1U 
                                                      & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                                         >> 0xfU)))) 
                                          << 0x10U) 
                                         | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))
                                      : ((2U == (7U 
                                                 & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                          ? (((- (IData)(
                                                         (1U 
                                                          & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected) 
                                                             >> 7U)))) 
                                              << 8U) 
                                             | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected))
                                          : ((3U == 
                                              (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                              ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected)
                                              : ((4U 
                                                  == 
                                                  (7U 
                                                   & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                                  ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected)
                                                  : 0U)))))
                              : 0U)),32);
    bufp->fullIData(oldp+2,(((IData)(vlSelfRef.core_top__DOT__u_if_stage__DOT__trap_enter)
                              ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                              : ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int)
                                  ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                                  : 0U))),32);
    bufp->fullBit(oldp+3,(((IData)(vlSelfRef.core_top__DOT__u_csr__DOT____VdfgRegularize_h8136f0d8_0_0) 
                           & ((IData)(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl) 
                              >> 2U))));
    bufp->fullBit(oldp+4,(vlSelfRef.core_top__DOT__is_ebreak_eff));
    bufp->fullBit(oldp+5,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_write));
    bufp->fullBit(oldp+6,(vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter));
    bufp->fullBit(oldp+7,(vlSelfRef.core_top__DOT__u_csr__DOT__mret));
    bufp->fullIData(oldp+8,(vlSelfRef.core_top__DOT__u_csr__DOT__trap_code),32);
    bufp->fullBit(oldp+9,(vlSelfRef.core_top__DOT__u_if_stage__DOT__trap_enter));
    bufp->fullBit(oldp+10,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_read));
    bufp->fullBit(oldp+11,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write));
    bufp->fullCData(oldp+12,(((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write)
                               ? ((5U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                   ? 0xfU : (0xfU & 
                                             ((6U == 
                                               (7U 
                                                & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                               ? ((2U 
                                                   & vlSelfRef.core_top__DOT__ex_mem_alu_result)
                                                   ? 0xcU
                                                   : 3U)
                                               : ((IData)(1U) 
                                                  << 
                                                  (3U 
                                                   & vlSelfRef.core_top__DOT__ex_mem_alu_result)))))
                               : 0U)),4);
    bufp->fullIData(oldp+13,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw),32);
    bufp->fullIData(oldp+14,(((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write)
                               ? ((5U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
                                   ? vlSelfRef.core_top__DOT__ex_mem_rs2_rdata
                                   : ((6U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))
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
    bufp->fullCData(oldp+15,((0xffU & vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw)),8);
    bufp->fullCData(oldp+16,((0xffU & (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                       >> 8U))),8);
    bufp->fullCData(oldp+17,((0xffU & (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                       >> 0x10U))),8);
    bufp->fullCData(oldp+18,((vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                              >> 0x18U)),8);
    bufp->fullSData(oldp+19,((0xffffU & vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw)),16);
    bufp->fullSData(oldp+20,((vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                              >> 0x10U)),16);
    bufp->fullCData(oldp+21,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected),8);
    bufp->fullSData(oldp+22,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected),16);
    bufp->fullIData(oldp+23,((((- (IData)((1U & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                                 >> 0xfU)))) 
                               << 0x10U) | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))),32);
    bufp->fullIData(oldp+24,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected),32);
    bufp->fullIData(oldp+25,((((- (IData)((1U & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected) 
                                                 >> 7U)))) 
                               << 8U) | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected))),32);
    bufp->fullIData(oldp+26,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected),32);
    bufp->fullIData(oldp+27,(vlSelfRef.core_top__DOT__pc),32);
    bufp->fullIData(oldp+28,(((IData)(4U) + vlSelfRef.core_top__DOT__pc)),32);
    bufp->fullIData(oldp+29,(vlSelfRef.core_top__DOT__rs1_rdata),32);
    bufp->fullIData(oldp+30,(vlSelfRef.core_top__DOT__rs2_rdata),32);
    bufp->fullIData(oldp+31,(((0U == (3U & (IData)(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl)))
                               ? vlSelfRef.core_top__DOT__mem_wb_alu_result
                               : ((1U == (3U & (IData)(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl)))
                                   ? vlSelfRef.core_top__DOT__mem_wb_mem_rdata
                                   : ((2U == (3U & (IData)(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl)))
                                       ? vlSelfRef.core_top__DOT__mem_wb_pc_plus4
                                       : vlSelfRef.core_top__DOT__mem_wb_csr_rdata)))),32);
    bufp->fullIData(oldp+32,(vlSelfRef.core_top__DOT__alu_result),32);
    bufp->fullIData(oldp+33,(vlSelfRef.core_top__DOT__imm_out),32);
    bufp->fullIData(oldp+34,(vlSelfRef.core_top__DOT__csr_rdata),32);
    bufp->fullIData(oldp+35,(((0U == (3U & (IData)(
                                                   (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                    >> 0x2dU))))
                               ? vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src
                               : ((1U == (3U & (IData)(
                                                       (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                        >> 0x2dU))))
                                   ? (vlSelfRef.core_top__DOT__csr_rdata 
                                      | vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src)
                                   : ((2U == (3U & (IData)(
                                                           (vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                            >> 0x2dU))))
                                       ? ((~ vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src) 
                                          & vlSelfRef.core_top__DOT__csr_rdata)
                                       : 0U)))),32);
    bufp->fullIData(oldp+36,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                             [0xaU]),32);
    bufp->fullCData(oldp+37,((0x1fU & (vlSelfRef.core_top__DOT__if_id_inst 
                                       >> 0xfU))),5);
    bufp->fullCData(oldp+38,((0x1fU & (vlSelfRef.core_top__DOT__if_id_inst 
                                       >> 0x14U))),5);
    bufp->fullCData(oldp+39,((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op) 
                               << 3U) | (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_a) 
                                          << 2U) | (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b)))),8);
    bufp->fullCData(oldp+40,((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_read) 
                               << 4U) | (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_write) 
                                          << 3U) | (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type)))),5);
    bufp->fullCData(oldp+41,((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write) 
                               << 2U) | (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src))),3);
    bufp->fullQData(oldp+42,((((QData)((IData)((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_read) 
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
    bufp->fullQData(oldp+44,((((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_enter_int)) 
                               << 0x22U) | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int)) 
                                             << 0x21U) 
                                            | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_ebreak_int)) 
                                                << 0x20U) 
                                               | (QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_code_int)))))),35);
    bufp->fullQData(oldp+46,((((QData)((IData)((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel) 
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
    bufp->fullCData(oldp+48,((0x1fU & (vlSelfRef.core_top__DOT__if_id_inst 
                                       >> 7U))),5);
    bufp->fullBit(oldp+49,((1U & ((IData)(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl) 
                                  >> 2U))));
    bufp->fullBit(oldp+50,((1U & (IData)((vlSelfRef.core_top__DOT__mem_wb_sys_ctrl 
                                          >> 0x20U)))));
    bufp->fullIData(oldp+51,(((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int)
                               ? (0xfffffffeU & vlSelfRef.core_top__DOT__u_id_stage__DOT__target_sum)
                               : vlSelfRef.core_top__DOT__u_id_stage__DOT__target_sum)),32);
    bufp->fullBit(oldp+52,(vlSelfRef.core_top__DOT__if_id_valid));
    bufp->fullIData(oldp+53,(vlSelfRef.core_top__DOT__if_id_pc),32);
    bufp->fullIData(oldp+54,(vlSelfRef.core_top__DOT__if_id_pc_plus4),32);
    bufp->fullIData(oldp+55,(vlSelfRef.core_top__DOT__if_id_inst),32);
    bufp->fullBit(oldp+56,(vlSelfRef.core_top__DOT__id_ex_valid));
    bufp->fullIData(oldp+57,(vlSelfRef.core_top__DOT__id_ex_pc),32);
    bufp->fullIData(oldp+58,(vlSelfRef.core_top__DOT__id_ex_pc_plus4),32);
    bufp->fullIData(oldp+59,(vlSelfRef.core_top__DOT__id_ex_inst),32);
    bufp->fullCData(oldp+60,(vlSelfRef.core_top__DOT__id_ex_rs1_addr),5);
    bufp->fullCData(oldp+61,(vlSelfRef.core_top__DOT__id_ex_rs2_addr),5);
    bufp->fullCData(oldp+62,(vlSelfRef.core_top__DOT__id_ex_rd_addr),5);
    bufp->fullIData(oldp+63,(vlSelfRef.core_top__DOT__id_ex_rs1_rdata),32);
    bufp->fullIData(oldp+64,(vlSelfRef.core_top__DOT__id_ex_rs2_rdata),32);
    bufp->fullIData(oldp+65,(vlSelfRef.core_top__DOT__id_ex_imm_out),32);
    bufp->fullCData(oldp+66,(vlSelfRef.core_top__DOT__id_ex_ex_ctrl),8);
    bufp->fullCData(oldp+67,(vlSelfRef.core_top__DOT__id_ex_mem_ctrl),5);
    bufp->fullCData(oldp+68,(vlSelfRef.core_top__DOT__id_ex_wb_ctrl),3);
    bufp->fullQData(oldp+69,(vlSelfRef.core_top__DOT__id_ex_csr_ctrl),49);
    bufp->fullQData(oldp+71,(vlSelfRef.core_top__DOT__id_ex_sys_ctrl),35);
    bufp->fullBit(oldp+73,(vlSelfRef.core_top__DOT__ex_mem_valid));
    bufp->fullIData(oldp+74,(vlSelfRef.core_top__DOT__ex_mem_pc),32);
    bufp->fullIData(oldp+75,(vlSelfRef.core_top__DOT__ex_mem_pc_plus4),32);
    bufp->fullIData(oldp+76,(vlSelfRef.core_top__DOT__ex_mem_inst),32);
    bufp->fullCData(oldp+77,(vlSelfRef.core_top__DOT__ex_mem_rd_addr),5);
    bufp->fullIData(oldp+78,(vlSelfRef.core_top__DOT__ex_mem_alu_result),32);
    bufp->fullIData(oldp+79,(vlSelfRef.core_top__DOT__ex_mem_rs2_rdata),32);
    bufp->fullIData(oldp+80,(vlSelfRef.core_top__DOT__ex_mem_csr_rdata),32);
    bufp->fullIData(oldp+81,(vlSelfRef.core_top__DOT__ex_mem_csr_wdata),32);
    bufp->fullCData(oldp+82,(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl),5);
    bufp->fullCData(oldp+83,(vlSelfRef.core_top__DOT__ex_mem_wb_ctrl),3);
    bufp->fullQData(oldp+84,(vlSelfRef.core_top__DOT__ex_mem_csr_ctrl),49);
    bufp->fullQData(oldp+86,(vlSelfRef.core_top__DOT__ex_mem_sys_ctrl),35);
    bufp->fullBit(oldp+88,(vlSelfRef.core_top__DOT__mem_wb_valid));
    bufp->fullIData(oldp+89,(vlSelfRef.core_top__DOT__mem_wb_pc),32);
    bufp->fullIData(oldp+90,(vlSelfRef.core_top__DOT__mem_wb_pc_plus4),32);
    bufp->fullIData(oldp+91,(vlSelfRef.core_top__DOT__mem_wb_inst),32);
    bufp->fullCData(oldp+92,(vlSelfRef.core_top__DOT__mem_wb_rd_addr),5);
    bufp->fullIData(oldp+93,(vlSelfRef.core_top__DOT__mem_wb_alu_result),32);
    bufp->fullIData(oldp+94,(vlSelfRef.core_top__DOT__mem_wb_mem_rdata),32);
    bufp->fullIData(oldp+95,(vlSelfRef.core_top__DOT__mem_wb_csr_rdata),32);
    bufp->fullIData(oldp+96,(vlSelfRef.core_top__DOT__mem_wb_csr_wdata),32);
    bufp->fullCData(oldp+97,(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl),3);
    bufp->fullQData(oldp+98,(vlSelfRef.core_top__DOT__mem_wb_csr_ctrl),49);
    bufp->fullQData(oldp+100,(vlSelfRef.core_top__DOT__mem_wb_sys_ctrl),35);
    bufp->fullSData(oldp+102,((0xfffU & (IData)((vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                                 >> 0x20U)))),12);
    bufp->fullBit(oldp+103,((1U & (IData)((vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                           >> 0x30U)))));
    bufp->fullSData(oldp+104,((0xfffU & (IData)((vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                                 >> 0x20U)))),12);
    bufp->fullBit(oldp+105,((1U & (IData)((vlSelfRef.core_top__DOT__mem_wb_csr_ctrl 
                                           >> 0x2fU)))));
    bufp->fullBit(oldp+106,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_enter_int));
    bufp->fullBit(oldp+107,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int));
    bufp->fullBit(oldp+108,((1U & (IData)((vlSelfRef.core_top__DOT__mem_wb_sys_ctrl 
                                           >> 0x22U)))));
    bufp->fullBit(oldp+109,((1U & (IData)((vlSelfRef.core_top__DOT__mem_wb_sys_ctrl 
                                           >> 0x21U)))));
    bufp->fullIData(oldp+110,((IData)(vlSelfRef.core_top__DOT__mem_wb_sys_ctrl)),32);
    bufp->fullIData(oldp+111,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus),32);
    bufp->fullIData(oldp+112,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec),32);
    bufp->fullIData(oldp+113,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc),32);
    bufp->fullIData(oldp+114,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause),32);
    bufp->fullIData(oldp+115,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip),32);
    bufp->fullIData(oldp+116,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mie),32);
    bufp->fullIData(oldp+117,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtval),32);
    bufp->fullIData(oldp+118,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mscratch),32);
    bufp->fullQData(oldp+119,(vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64),64);
    bufp->fullCData(oldp+121,((0x1fU & ((IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl) 
                                        >> 3U))),5);
    bufp->fullBit(oldp+122,((1U & ((IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl) 
                                   >> 2U))));
    bufp->fullCData(oldp+123,((3U & (IData)(vlSelfRef.core_top__DOT__id_ex_ex_ctrl))),2);
    bufp->fullCData(oldp+124,((3U & (IData)((vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                             >> 0x2dU)))),2);
    bufp->fullBit(oldp+125,((1U & (IData)((vlSelfRef.core_top__DOT__id_ex_csr_ctrl 
                                           >> 0x2cU)))));
    bufp->fullIData(oldp+126,((IData)(vlSelfRef.core_top__DOT__id_ex_csr_ctrl)),32);
    bufp->fullIData(oldp+127,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1),32);
    bufp->fullIData(oldp+128,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2),32);
    bufp->fullIData(oldp+129,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src),32);
    bufp->fullIData(oldp+130,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__abs_dividend),32);
    bufp->fullIData(oldp+131,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__abs_divisor),32);
    bufp->fullIData(oldp+132,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__quotient),32);
    bufp->fullIData(oldp+133,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__abs_dividend),32);
    bufp->fullIData(oldp+134,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__abs_divisor),32);
    bufp->fullIData(oldp+135,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__remainder),32);
    bufp->fullQData(oldp+136,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__src1_s64),64);
    bufp->fullQData(oldp+138,((((QData)((IData)((- (IData)(
                                                           (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2 
                                                            >> 0x1fU))))) 
                                << 0x20U) | (QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)))),64);
    bufp->fullQData(oldp+140,(VL_MULS_QQQ(64, vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__src1_s64, 
                                          (((QData)((IData)(
                                                            (- (IData)(
                                                                       (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2 
                                                                        >> 0x1fU))))) 
                                            << 0x20U) 
                                           | (QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))))),64);
    bufp->fullQData(oldp+142,((QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1))),64);
    bufp->fullQData(oldp+144,((QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))),64);
    bufp->fullQData(oldp+146,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__mul_uu),64);
    bufp->fullQData(oldp+148,((vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__src1_s64 
                               * (QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)))),64);
    bufp->fullCData(oldp+150,((0x7fU & vlSelfRef.core_top__DOT__if_id_inst)),7);
    bufp->fullCData(oldp+151,((7U & (vlSelfRef.core_top__DOT__if_id_inst 
                                     >> 0xcU))),3);
    bufp->fullCData(oldp+152,((vlSelfRef.core_top__DOT__if_id_inst 
                               >> 0x19U)),7);
    bufp->fullBit(oldp+153,(((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
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
    bufp->fullQData(oldp+154,((((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel)) 
                                << 0x26U) | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type)) 
                                              << 0x22U) 
                                             | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jal_int)) 
                                                 << 0x21U) 
                                                | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int)) 
                                                    << 0x20U) 
                                                   | (QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_code_int))))))),40);
    bufp->fullCData(oldp+156,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type),3);
    bufp->fullBit(oldp+157,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int));
    bufp->fullIData(oldp+158,(vlSelfRef.core_top__DOT__u_id_stage__DOT__target_sum),32);
    bufp->fullCData(oldp+159,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op),5);
    bufp->fullBit(oldp+160,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_a));
    bufp->fullCData(oldp+161,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b),2);
    bufp->fullBit(oldp+162,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_read));
    bufp->fullBit(oldp+163,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_write));
    bufp->fullCData(oldp+164,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type),3);
    bufp->fullBit(oldp+165,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write));
    bufp->fullCData(oldp+166,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src),2);
    bufp->fullBit(oldp+167,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_read));
    bufp->fullBit(oldp+168,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_write));
    bufp->fullBit(oldp+169,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_imm));
    bufp->fullCData(oldp+170,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_op),2);
    bufp->fullCData(oldp+171,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel),2);
    bufp->fullBit(oldp+172,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_ebreak_int));
    bufp->fullBit(oldp+173,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jal_int));
    bufp->fullIData(oldp+174,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_code_int),32);
    bufp->fullSData(oldp+175,((vlSelfRef.core_top__DOT__if_id_inst 
                               >> 0x14U)),12);
    bufp->fullIData(oldp+176,((0x1fU & (vlSelfRef.core_top__DOT__if_id_inst 
                                        >> 0xfU))),32);
    bufp->fullIData(oldp+177,((((- (IData)((vlSelfRef.core_top__DOT__if_id_inst 
                                            >> 0x1fU))) 
                                << 0xcU) | (vlSelfRef.core_top__DOT__if_id_inst 
                                            >> 0x14U))),32);
    bufp->fullIData(oldp+178,((((- (IData)((vlSelfRef.core_top__DOT__if_id_inst 
                                            >> 0x1fU))) 
                                << 0xcU) | ((0xfe0U 
                                             & (vlSelfRef.core_top__DOT__if_id_inst 
                                                >> 0x14U)) 
                                            | (0x1fU 
                                               & (vlSelfRef.core_top__DOT__if_id_inst 
                                                  >> 7U))))),32);
    bufp->fullIData(oldp+179,((((- (IData)((vlSelfRef.core_top__DOT__if_id_inst 
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
    bufp->fullIData(oldp+180,((0xfffff000U & vlSelfRef.core_top__DOT__if_id_inst)),32);
    bufp->fullIData(oldp+181,(((((- (IData)((vlSelfRef.core_top__DOT__if_id_inst 
                                             >> 0x1fU))) 
                                 << 0x15U) | (0x100000U 
                                              & (vlSelfRef.core_top__DOT__if_id_inst 
                                                 >> 0xbU))) 
                               | (((0xff000U & vlSelfRef.core_top__DOT__if_id_inst) 
                                   | (0x800U & (vlSelfRef.core_top__DOT__if_id_inst 
                                                >> 9U))) 
                                  | (0x7feU & (vlSelfRef.core_top__DOT__if_id_inst 
                                               >> 0x14U))))),32);
    bufp->fullBit(oldp+182,(((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jal_int) 
                             | ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_jalr_int) 
                                | ((3U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel)) 
                                   & ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                       ? ((~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type) 
                                              >> 1U)) 
                                          & ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                              ? (vlSelfRef.core_top__DOT__rs1_rdata 
                                                 >= vlSelfRef.core_top__DOT__rs2_rdata)
                                              : (vlSelfRef.core_top__DOT__rs1_rdata 
                                                 < vlSelfRef.core_top__DOT__rs2_rdata)))
                                       : ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                           ? ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                               ? VL_GTES_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.core_top__DOT__rs2_rdata)
                                               : VL_LTS_III(32, vlSelfRef.core_top__DOT__rs1_rdata, vlSelfRef.core_top__DOT__rs2_rdata))
                                           : ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                                               ? (vlSelfRef.core_top__DOT__rs1_rdata 
                                                  != vlSelfRef.core_top__DOT__rs2_rdata)
                                               : (vlSelfRef.core_top__DOT__rs1_rdata 
                                                  == vlSelfRef.core_top__DOT__rs2_rdata)))))))));
    bufp->fullCData(oldp+183,((7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl))),3);
    bufp->fullCData(oldp+184,((3U & vlSelfRef.core_top__DOT__ex_mem_alu_result)),2);
    bufp->fullBit(oldp+185,((1U & (vlSelfRef.core_top__DOT__ex_mem_alu_result 
                                   >> 1U))));
    bufp->fullBit(oldp+186,((0U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))));
    bufp->fullBit(oldp+187,((1U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))));
    bufp->fullBit(oldp+188,((2U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))));
    bufp->fullBit(oldp+189,((3U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))));
    bufp->fullBit(oldp+190,((4U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))));
    bufp->fullBit(oldp+191,((5U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))));
    bufp->fullBit(oldp+192,((6U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))));
    bufp->fullBit(oldp+193,((7U == (7U & (IData)(vlSelfRef.core_top__DOT__ex_mem_mem_ctrl)))));
    bufp->fullIData(oldp+194,(((vlSelfRef.core_top__DOT__ex_mem_rs2_rdata 
                                << 0x10U) | (0xffffU 
                                             & vlSelfRef.core_top__DOT__ex_mem_rs2_rdata))),32);
    bufp->fullIData(oldp+195,(((vlSelfRef.core_top__DOT__ex_mem_rs2_rdata 
                                << 0x18U) | ((0xff0000U 
                                              & (vlSelfRef.core_top__DOT__ex_mem_rs2_rdata 
                                                 << 0x10U)) 
                                             | ((0xff00U 
                                                 & (vlSelfRef.core_top__DOT__ex_mem_rs2_rdata 
                                                    << 8U)) 
                                                | (0xffU 
                                                   & vlSelfRef.core_top__DOT__ex_mem_rs2_rdata))))),32);
    bufp->fullCData(oldp+196,(((2U & vlSelfRef.core_top__DOT__ex_mem_alu_result)
                                ? 0xcU : 3U)),4);
    bufp->fullCData(oldp+197,((0xfU & ((IData)(1U) 
                                       << (3U & vlSelfRef.core_top__DOT__ex_mem_alu_result)))),4);
    bufp->fullIData(oldp+198,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[0]),32);
    bufp->fullIData(oldp+199,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[1]),32);
    bufp->fullIData(oldp+200,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[2]),32);
    bufp->fullIData(oldp+201,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[3]),32);
    bufp->fullIData(oldp+202,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[4]),32);
    bufp->fullIData(oldp+203,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[5]),32);
    bufp->fullIData(oldp+204,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[6]),32);
    bufp->fullIData(oldp+205,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[7]),32);
    bufp->fullIData(oldp+206,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[8]),32);
    bufp->fullIData(oldp+207,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[9]),32);
    bufp->fullIData(oldp+208,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[10]),32);
    bufp->fullIData(oldp+209,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[11]),32);
    bufp->fullIData(oldp+210,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[12]),32);
    bufp->fullIData(oldp+211,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[13]),32);
    bufp->fullIData(oldp+212,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[14]),32);
    bufp->fullIData(oldp+213,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[15]),32);
    bufp->fullIData(oldp+214,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[16]),32);
    bufp->fullIData(oldp+215,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[17]),32);
    bufp->fullIData(oldp+216,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[18]),32);
    bufp->fullIData(oldp+217,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[19]),32);
    bufp->fullIData(oldp+218,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[20]),32);
    bufp->fullIData(oldp+219,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[21]),32);
    bufp->fullIData(oldp+220,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[22]),32);
    bufp->fullIData(oldp+221,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[23]),32);
    bufp->fullIData(oldp+222,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[24]),32);
    bufp->fullIData(oldp+223,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[25]),32);
    bufp->fullIData(oldp+224,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[26]),32);
    bufp->fullIData(oldp+225,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[27]),32);
    bufp->fullIData(oldp+226,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[28]),32);
    bufp->fullIData(oldp+227,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[29]),32);
    bufp->fullIData(oldp+228,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[30]),32);
    bufp->fullIData(oldp+229,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[31]),32);
    bufp->fullCData(oldp+230,((3U & (IData)(vlSelfRef.core_top__DOT__mem_wb_wb_ctrl))),2);
    bufp->fullBit(oldp+231,(vlSelfRef.clk));
    bufp->fullBit(oldp+232,(vlSelfRef.rst));
    bufp->fullBit(oldp+233,(vlSelfRef.interrupt_valid));
    bufp->fullIData(oldp+234,(vlSelfRef.interrupt_cause),32);
    bufp->fullBit(oldp+235,(vlSelfRef.halt));
    bufp->fullIData(oldp+236,(vlSelfRef.instr),32);
    bufp->fullIData(oldp+237,(vlSelfRef.halt_pc),32);
    bufp->fullIData(oldp+238,(vlSelfRef.halt_ret),32);
    bufp->fullIData(oldp+239,((((IData)(vlSelfRef.core_top__DOT__u_if_stage__DOT__trap_enter) 
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
    bufp->fullBit(oldp+240,(((~ (IData)(vlSelfRef.interrupt_valid)) 
                             & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int))));
}
