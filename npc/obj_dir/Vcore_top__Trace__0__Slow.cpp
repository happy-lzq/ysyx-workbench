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
    tracep->declBus(c+138,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+139,0,"rst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+140,0,"interrupt_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+141,0,"interrupt_cause",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+142,0,"halt",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+143,0,"instr",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+144,0,"halt_pc",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+145,0,"halt_ret",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("core_top", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+138,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+139,0,"rst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+140,0,"interrupt_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+141,0,"interrupt_cause",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+142,0,"halt",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+143,0,"instr",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+144,0,"halt_pc",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+145,0,"halt_ret",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+27,0,"pc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+28,0,"pc_plus4",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+29,0,"rs1_rdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+30,0,"rs2_rdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+1,0,"rd_wdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+31,0,"alu_result",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+32,0,"imm_out",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+31,0,"mem_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+30,0,"mem_wdata_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+2,0,"mem_rdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+31,0,"jump_jalr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+32,0,"imm_jal",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+32,0,"imm_br",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+33,0,"csr_rdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+34,0,"csr_wdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+3,0,"trap_target",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+27,0,"trap_pc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+35,0,"rs_a0",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+36,0,"br_taken",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+146,0,"rs1_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+147,0,"rs2_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+37,0,"ex_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 10,0);
    tracep->declBus(c+38,0,"mem_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+148,0,"wb_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declQuad(c+149,0,"csr_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+39,0,"sys_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->declBus(c+41,0,"pc_sel",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+151,0,"rd_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBit(c+42,0,"reg_write_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+43,0,"is_ebreak_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+152,0,"reg_write_eff",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+4,0,"is_ebreak_eff",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("u_csr", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+138,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+139,0,"rst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declQuad(c+149,0,"csr_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+39,0,"sys_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->declBus(c+140,0,"interrupt_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+141,0,"interrupt_cause",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+34,0,"csr_wdata",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+27,0,"trap_pc",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+33,0,"csr_rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+3,0,"trap_target",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+153,0,"csr_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBit(c+44,0,"csr_write_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+45,0,"csr_read",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+46,0,"trap_enter_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+47,0,"mret_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+48,0,"trap_code_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+5,0,"csr_write",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+6,0,"mret",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+7,0,"trap_enter",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+8,0,"trap_code",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+163,0,"mstatus",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+164,0,"mtvec",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+165,0,"mepc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+166,0,"mcause",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+167,0,"mcyclel",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+168,0,"mcycleh",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+169,0,"mip",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+170,0,"mie",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+171,0,"mtval",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+172,0,"mscratch",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+173,0,"IRQ_M_TIMER",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+174,0,"IRQ_M_EXT",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+49,0,"csr_mstatus",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+50,0,"csr_mtvec",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+51,0,"csr_mepc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+52,0,"csr_mcause",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+53,0,"csr_mip",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+54,0,"csr_mie",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+55,0,"csr_mtval",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+56,0,"csr_mscratch",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declQuad(c+57,0,"mcycle_64",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 63,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_ex_stage", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+27,0,"pc",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+29,0,"rs1_rdata",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+30,0,"rs2_rdata",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+32,0,"imm_out",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+33,0,"csr_rdata",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+37,0,"ex_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 10,0);
    tracep->declQuad(c+149,0,"csr_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declBus(c+31,0,"alu_result",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+36,0,"br_taken",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+34,0,"csr_wdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+59,0,"alu_op",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBit(c+60,0,"alu_src_a",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+61,0,"alu_src_b",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+62,0,"br_type",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+63,0,"csr_op",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBit(c+64,0,"csr_imm",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+154,0,"csr_zimm",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+65,0,"src1",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+66,0,"src2",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+67,0,"csr_src",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->pushPrefix("alu_pic", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+65,0,"src1",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+66,0,"src2",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+59,0,"alu_op",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+31,0,"result",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+68,0,"signed_div32__Vstatic__abs_dividend",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+69,0,"signed_div32__Vstatic__abs_divisor",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+70,0,"signed_div32__Vstatic__quotient",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+71,0,"signed_rem32__Vstatic__abs_dividend",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+72,0,"signed_rem32__Vstatic__abs_divisor",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+73,0,"signed_rem32__Vstatic__remainder",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declQuad(c+74,0,"src1_s64",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 63,0);
    tracep->declQuad(c+76,0,"src2_s64",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 63,0);
    tracep->declQuad(c+78,0,"mul_ss",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 63,0);
    tracep->declQuad(c+80,0,"src1_u64",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 63,0);
    tracep->declQuad(c+82,0,"src2_u64",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 63,0);
    tracep->declQuad(c+84,0,"mul_uu",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 63,0);
    tracep->declQuad(c+86,0,"mul_su",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 63,0);
    tracep->popPrefix();
    tracep->pushPrefix("br_cond_pic", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+62,0,"br_type",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+29,0,"rs1_data",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+30,0,"rs2_data",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+36,0,"br_taken",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("u_halt", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+175,0,"RESET_VECTOR",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+176,0,"RESET_x10",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+138,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+139,0,"rst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+4,0,"is_ebreak",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+27,0,"pc",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+35,0,"rs_a0",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+142,0,"halt_reg",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+144,0,"halt_pc",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+145,0,"halt_ret",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_id_stage", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+143,0,"inst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+146,0,"rs1_addr",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+147,0,"rs2_addr",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+32,0,"imm_out",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+37,0,"ex_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 10,0);
    tracep->declBus(c+38,0,"mem_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+148,0,"wb_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declQuad(c+149,0,"csr_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+39,0,"sys_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->declBus(c+41,0,"pc_sel",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+155,0,"opcode",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 6,0);
    tracep->declBus(c+156,0,"funct3",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+157,0,"funct7",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 6,0);
    tracep->pushPrefix("u_control", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+143,0,"inst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+155,0,"opcode",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 6,0);
    tracep->declBus(c+156,0,"funct3",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+157,0,"funct7",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 6,0);
    tracep->declBus(c+37,0,"ex_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 10,0);
    tracep->declBus(c+38,0,"mem_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+148,0,"wb_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declQuad(c+149,0,"csr_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 48,0);
    tracep->declQuad(c+39,0,"sys_ctrl",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->declBus(c+41,0,"pc_sel",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+59,0,"alu_op",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+60,0,"alu_src_a",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+61,0,"alu_src_b",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+62,0,"br_type",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+88,0,"mem_read",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+89,0,"mem_write",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+90,0,"lsu_type",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+42,0,"reg_write",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+91,0,"reg_wdata_src",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+45,0,"csr_read",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+44,0,"csr_write",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+64,0,"csr_imm",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+63,0,"csr_op",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+41,0,"pc_sel_int",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+46,0,"trap_enter_int",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+43,0,"is_ebreak_int",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+47,0,"mret_int",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+48,0,"trap_code_int",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+146,0,"rs1_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+153,0,"funct12",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+151,0,"rd_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+153,0,"csr_addr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 11,0);
    tracep->declBus(c+154,0,"csr_zimm",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_imm_gen", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+143,0,"inst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+32,0,"imm_out",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+158,0,"imm_i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+159,0,"imm_s",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+160,0,"imm_b",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+161,0,"imm_u",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+162,0,"imm_j",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("u_if_stage", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+138,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+139,0,"rst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declQuad(c+39,0,"sys_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 34,0);
    tracep->declBus(c+140,0,"interrupt_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+36,0,"br_taken",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+41,0,"pc_sel",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+31,0,"jump_jalr",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+32,0,"imm_jal",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+32,0,"imm_br",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+3,0,"trap_target",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+27,0,"pc",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+28,0,"pc_plus4",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+143,0,"inst",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+46,0,"trap_enter_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+47,0,"mret_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+7,0,"trap_enter",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+6,0,"mret",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+175,0,"RESET_VECTOR",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+9,0,"pc_next",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_mem_stage", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+138,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+140,0,"interrupt_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+38,0,"mem_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+31,0,"mem_addr",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+30,0,"mem_wdata_raw",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+2,0,"mem_rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+10,0,"mem_read",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+11,0,"mem_write",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+90,0,"lsu_type",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+12,0,"mem_wmask",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 3,0);
    tracep->declBus(c+13,0,"mem_rdata_raw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+14,0,"mem_wdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->pushPrefix("lsu_pic", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+31,0,"mem_addr",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+13,0,"mem_rdata_raw",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+30,0,"mem_wdata_raw",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+90,0,"lsu_type",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+10,0,"mem_read",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+11,0,"mem_write",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+2,0,"mem_rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+14,0,"mem_wdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+12,0,"mem_wmask",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 3,0);
    tracep->declBus(c+92,0,"sel_byte",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+93,0,"sel_half",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+15,0,"rdata_byte_0",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+16,0,"rdata_byte_1",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+17,0,"rdata_byte_2",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+18,0,"rdata_byte_3",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+19,0,"rdata_half_0",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 15,0);
    tracep->declBus(c+20,0,"rdata_half_1",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 15,0);
    tracep->declBus(c+21,0,"byte_selected",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+22,0,"half_selected",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 15,0);
    tracep->declBit(c+94,0,"is_lw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+95,0,"is_lh",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+96,0,"is_lb",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+97,0,"is_lhu",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+98,0,"is_lbu",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+99,0,"is_sw",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+100,0,"is_sh",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+101,0,"is_sb",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+23,0,"lh_result",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+24,0,"lhu_result",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+25,0,"lb_result",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+26,0,"lbu_result",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+30,0,"sw_wdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+102,0,"sh_wdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+103,0,"sb_wdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+177,0,"sw_mask",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 3,0);
    tracep->declBus(c+104,0,"sh_mask",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 3,0);
    tracep->declBus(c+105,0,"sb_mask",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 3,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("u_regfile", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+138,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+152,0,"reg_write",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 0,0);
    tracep->declBus(c+146,0,"rs1_addr",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+147,0,"rs2_addr",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+151,0,"rd_addr",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+1,0,"rd_wdata",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+29,0,"rs1_rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+30,0,"rs2_rdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+35,0,"rs_a0",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->pushPrefix("rf", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 32; ++i) {
        tracep->declBus(c+106+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 31,0);
    }
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("u_wb_stage", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+148,0,"wb_ctrl",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 7,0);
    tracep->declBus(c+2,0,"mem_rdata",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+31,0,"alu_result",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+28,0,"pc_plus4",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+33,0,"csr_rdata",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+1,0,"rd_wdata",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+91,0,"rd_wdata_src",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
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
    bufp->fullSData(oldp+163,(0x300U),12);
    bufp->fullSData(oldp+164,(0x305U),12);
    bufp->fullSData(oldp+165,(0x341U),12);
    bufp->fullSData(oldp+166,(0x342U),12);
    bufp->fullSData(oldp+167,(0xb00U),12);
    bufp->fullSData(oldp+168,(0xb80U),12);
    bufp->fullSData(oldp+169,(0x344U),12);
    bufp->fullSData(oldp+170,(0x304U),12);
    bufp->fullSData(oldp+171,(0x343U),12);
    bufp->fullSData(oldp+172,(0x340U),12);
    bufp->fullIData(oldp+173,(0x80000007U),32);
    bufp->fullIData(oldp+174,(0x8000000bU),32);
    bufp->fullIData(oldp+175,(0x80000000U),32);
    bufp->fullIData(oldp+176,(0U),32);
    bufp->fullCData(oldp+177,(0xfU),4);
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
    bufp->fullIData(oldp+1,(((0U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src))
                              ? vlSelfRef.core_top__DOT__alu_result
                              : ((1U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src))
                                  ? ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_read)
                                      ? ((0U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                          ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw
                                          : ((1U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                              ? (((- (IData)(
                                                             (1U 
                                                              & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                                                 >> 0xfU)))) 
                                                  << 0x10U) 
                                                 | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))
                                              : ((2U 
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
                                                ? ((IData)(4U) 
                                                   + vlSelfRef.core_top__DOT__pc)
                                                : vlSelfRef.core_top__DOT__csr_rdata)))),32);
    bufp->fullIData(oldp+2,(((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_read)
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
                                          : ((3U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                              ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected)
                                              : ((4U 
                                                  == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                                  ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected)
                                                  : 0U)))))
                              : 0U)),32);
    bufp->fullIData(oldp+3,(((IData)(vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter)
                              ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                              : ((IData)(vlSelfRef.core_top__DOT__u_csr__DOT__mret)
                                  ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                                  : 0U))),32);
    bufp->fullBit(oldp+4,(vlSelfRef.core_top__DOT__is_ebreak_eff));
    bufp->fullBit(oldp+5,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_write));
    bufp->fullBit(oldp+6,(vlSelfRef.core_top__DOT__u_csr__DOT__mret));
    bufp->fullBit(oldp+7,(vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter));
    bufp->fullIData(oldp+8,(vlSelfRef.core_top__DOT__u_csr__DOT__trap_code),32);
    bufp->fullIData(oldp+9,((((IData)(vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter) 
                              | (IData)(vlSelfRef.core_top__DOT__u_csr__DOT__mret))
                              ? ((IData)(vlSelfRef.core_top__DOT__u_csr__DOT__trap_enter)
                                  ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                                  : ((IData)(vlSelfRef.core_top__DOT__u_csr__DOT__mret)
                                      ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                                      : 0U)) : ((0U 
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
    bufp->fullBit(oldp+10,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_read));
    bufp->fullBit(oldp+11,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write));
    bufp->fullCData(oldp+12,(((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write)
                               ? ((5U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                   ? 0xfU : (0xfU & 
                                             ((6U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))
                                               ? ((2U 
                                                   & vlSelfRef.core_top__DOT__alu_result)
                                                   ? 0xcU
                                                   : 3U)
                                               : ((IData)(1U) 
                                                  << 
                                                  (3U 
                                                   & vlSelfRef.core_top__DOT__alu_result)))))
                               : 0U)),4);
    bufp->fullIData(oldp+13,(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw),32);
    bufp->fullIData(oldp+14,(((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_write)
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
    bufp->fullIData(oldp+31,(vlSelfRef.core_top__DOT__alu_result),32);
    bufp->fullIData(oldp+32,(vlSelfRef.core_top__DOT__imm_out),32);
    bufp->fullIData(oldp+33,(vlSelfRef.core_top__DOT__csr_rdata),32);
    bufp->fullIData(oldp+34,(vlSelfRef.core_top__DOT__csr_wdata),32);
    bufp->fullIData(oldp+35,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                             [0xaU]),32);
    bufp->fullBit(oldp+36,(((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
                             ? ((~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type) 
                                    >> 1U)) & ((1U 
                                                & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type))
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
                                        == vlSelfRef.core_top__DOT__rs2_rdata))))));
    bufp->fullSData(oldp+37,(((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op) 
                                << 6U) | ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_a) 
                                          << 5U)) | 
                              (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b) 
                                << 3U) | (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type)))),11);
    bufp->fullCData(oldp+38,((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_read) 
                               << 4U) | (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_write) 
                                          << 3U) | (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type)))),5);
    bufp->fullQData(oldp+39,((((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_enter_int)) 
                               << 0x22U) | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int)) 
                                             << 0x21U) 
                                            | (((QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_ebreak_int)) 
                                                << 0x20U) 
                                               | (QData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_code_int)))))),35);
    bufp->fullCData(oldp+41,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__pc_sel_int),2);
    bufp->fullBit(oldp+42,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write));
    bufp->fullBit(oldp+43,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__is_ebreak_int));
    bufp->fullBit(oldp+44,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_write));
    bufp->fullBit(oldp+45,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_read));
    bufp->fullBit(oldp+46,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_enter_int));
    bufp->fullBit(oldp+47,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mret_int));
    bufp->fullIData(oldp+48,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__trap_code_int),32);
    bufp->fullIData(oldp+49,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus),32);
    bufp->fullIData(oldp+50,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec),32);
    bufp->fullIData(oldp+51,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc),32);
    bufp->fullIData(oldp+52,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause),32);
    bufp->fullIData(oldp+53,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip),32);
    bufp->fullIData(oldp+54,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mie),32);
    bufp->fullIData(oldp+55,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtval),32);
    bufp->fullIData(oldp+56,(vlSelfRef.core_top__DOT__u_csr__DOT__csr_mscratch),32);
    bufp->fullQData(oldp+57,(vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64),64);
    bufp->fullCData(oldp+59,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_op),5);
    bufp->fullBit(oldp+60,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_a));
    bufp->fullCData(oldp+61,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__alu_src_b),2);
    bufp->fullCData(oldp+62,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__br_type),3);
    bufp->fullCData(oldp+63,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_op),2);
    bufp->fullBit(oldp+64,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_imm));
    bufp->fullIData(oldp+65,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1),32);
    bufp->fullIData(oldp+66,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2),32);
    bufp->fullIData(oldp+67,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src),32);
    bufp->fullIData(oldp+68,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__abs_dividend),32);
    bufp->fullIData(oldp+69,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__abs_divisor),32);
    bufp->fullIData(oldp+70,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_div32__Vstatic__quotient),32);
    bufp->fullIData(oldp+71,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__abs_dividend),32);
    bufp->fullIData(oldp+72,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__abs_divisor),32);
    bufp->fullIData(oldp+73,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__signed_rem32__Vstatic__remainder),32);
    bufp->fullQData(oldp+74,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__src1_s64),64);
    bufp->fullQData(oldp+76,((((QData)((IData)((- (IData)(
                                                          (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2 
                                                           >> 0x1fU))))) 
                               << 0x20U) | (QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)))),64);
    bufp->fullQData(oldp+78,(VL_MULS_QQQ(64, vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__src1_s64, 
                                         (((QData)((IData)(
                                                           (- (IData)(
                                                                      (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2 
                                                                       >> 0x1fU))))) 
                                           << 0x20U) 
                                          | (QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))))),64);
    bufp->fullQData(oldp+80,((QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1))),64);
    bufp->fullQData(oldp+82,((QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))),64);
    bufp->fullQData(oldp+84,(vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__mul_uu),64);
    bufp->fullQData(oldp+86,((vlSelfRef.core_top__DOT__u_ex_stage__DOT__alu_pic__DOT__src1_s64 
                              * (QData)((IData)(vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)))),64);
    bufp->fullBit(oldp+88,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_read));
    bufp->fullBit(oldp+89,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__mem_write));
    bufp->fullCData(oldp+90,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type),3);
    bufp->fullCData(oldp+91,(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src),2);
    bufp->fullCData(oldp+92,((3U & vlSelfRef.core_top__DOT__alu_result)),2);
    bufp->fullBit(oldp+93,((1U & (vlSelfRef.core_top__DOT__alu_result 
                                  >> 1U))));
    bufp->fullBit(oldp+94,((0U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))));
    bufp->fullBit(oldp+95,((1U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))));
    bufp->fullBit(oldp+96,((2U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))));
    bufp->fullBit(oldp+97,((3U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))));
    bufp->fullBit(oldp+98,((4U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))));
    bufp->fullBit(oldp+99,((5U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))));
    bufp->fullBit(oldp+100,((6U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))));
    bufp->fullBit(oldp+101,((7U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__lsu_type))));
    bufp->fullIData(oldp+102,(((vlSelfRef.core_top__DOT__rs2_rdata 
                                << 0x10U) | (0xffffU 
                                             & vlSelfRef.core_top__DOT__rs2_rdata))),32);
    bufp->fullIData(oldp+103,(((vlSelfRef.core_top__DOT__rs2_rdata 
                                << 0x18U) | ((0xff0000U 
                                              & (vlSelfRef.core_top__DOT__rs2_rdata 
                                                 << 0x10U)) 
                                             | ((0xff00U 
                                                 & (vlSelfRef.core_top__DOT__rs2_rdata 
                                                    << 8U)) 
                                                | (0xffU 
                                                   & vlSelfRef.core_top__DOT__rs2_rdata))))),32);
    bufp->fullCData(oldp+104,(((2U & vlSelfRef.core_top__DOT__alu_result)
                                ? 0xcU : 3U)),4);
    bufp->fullCData(oldp+105,((0xfU & ((IData)(1U) 
                                       << (3U & vlSelfRef.core_top__DOT__alu_result)))),4);
    bufp->fullIData(oldp+106,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[0]),32);
    bufp->fullIData(oldp+107,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[1]),32);
    bufp->fullIData(oldp+108,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[2]),32);
    bufp->fullIData(oldp+109,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[3]),32);
    bufp->fullIData(oldp+110,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[4]),32);
    bufp->fullIData(oldp+111,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[5]),32);
    bufp->fullIData(oldp+112,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[6]),32);
    bufp->fullIData(oldp+113,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[7]),32);
    bufp->fullIData(oldp+114,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[8]),32);
    bufp->fullIData(oldp+115,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[9]),32);
    bufp->fullIData(oldp+116,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[10]),32);
    bufp->fullIData(oldp+117,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[11]),32);
    bufp->fullIData(oldp+118,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[12]),32);
    bufp->fullIData(oldp+119,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[13]),32);
    bufp->fullIData(oldp+120,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[14]),32);
    bufp->fullIData(oldp+121,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[15]),32);
    bufp->fullIData(oldp+122,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[16]),32);
    bufp->fullIData(oldp+123,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[17]),32);
    bufp->fullIData(oldp+124,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[18]),32);
    bufp->fullIData(oldp+125,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[19]),32);
    bufp->fullIData(oldp+126,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[20]),32);
    bufp->fullIData(oldp+127,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[21]),32);
    bufp->fullIData(oldp+128,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[22]),32);
    bufp->fullIData(oldp+129,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[23]),32);
    bufp->fullIData(oldp+130,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[24]),32);
    bufp->fullIData(oldp+131,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[25]),32);
    bufp->fullIData(oldp+132,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[26]),32);
    bufp->fullIData(oldp+133,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[27]),32);
    bufp->fullIData(oldp+134,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[28]),32);
    bufp->fullIData(oldp+135,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[29]),32);
    bufp->fullIData(oldp+136,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[30]),32);
    bufp->fullIData(oldp+137,(vlSelfRef.core_top__DOT__u_regfile__DOT__rf[31]),32);
    bufp->fullBit(oldp+138,(vlSelfRef.clk));
    bufp->fullBit(oldp+139,(vlSelfRef.rst));
    bufp->fullBit(oldp+140,(vlSelfRef.interrupt_valid));
    bufp->fullIData(oldp+141,(vlSelfRef.interrupt_cause),32);
    bufp->fullBit(oldp+142,(vlSelfRef.halt));
    bufp->fullIData(oldp+143,(vlSelfRef.instr),32);
    bufp->fullIData(oldp+144,(vlSelfRef.halt_pc),32);
    bufp->fullIData(oldp+145,(vlSelfRef.halt_ret),32);
    bufp->fullCData(oldp+146,((0x1fU & (vlSelfRef.instr 
                                        >> 0xfU))),5);
    bufp->fullCData(oldp+147,((0x1fU & (vlSelfRef.instr 
                                        >> 0x14U))),5);
    bufp->fullCData(oldp+148,((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write) 
                                << 7U) | (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_wdata_src) 
                                           << 5U) | 
                                          (0x1fU & 
                                           (vlSelfRef.instr 
                                            >> 7U))))),8);
    bufp->fullQData(oldp+149,((((QData)((IData)((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__csr_read) 
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
    bufp->fullCData(oldp+151,((0x1fU & (vlSelfRef.instr 
                                        >> 7U))),5);
    bufp->fullBit(oldp+152,(((~ (IData)(vlSelfRef.interrupt_valid)) 
                             & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_control__DOT__reg_write))));
    bufp->fullSData(oldp+153,((vlSelfRef.instr >> 0x14U)),12);
    bufp->fullIData(oldp+154,((0x1fU & (vlSelfRef.instr 
                                        >> 0xfU))),32);
    bufp->fullCData(oldp+155,((0x7fU & vlSelfRef.instr)),7);
    bufp->fullCData(oldp+156,((7U & (vlSelfRef.instr 
                                     >> 0xcU))),3);
    bufp->fullCData(oldp+157,((vlSelfRef.instr >> 0x19U)),7);
    bufp->fullIData(oldp+158,((((- (IData)((vlSelfRef.instr 
                                            >> 0x1fU))) 
                                << 0xcU) | (vlSelfRef.instr 
                                            >> 0x14U))),32);
    bufp->fullIData(oldp+159,((((- (IData)((vlSelfRef.instr 
                                            >> 0x1fU))) 
                                << 0xcU) | ((0xfe0U 
                                             & (vlSelfRef.instr 
                                                >> 0x14U)) 
                                            | (0x1fU 
                                               & (vlSelfRef.instr 
                                                  >> 7U))))),32);
    bufp->fullIData(oldp+160,((((- (IData)((vlSelfRef.instr 
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
    bufp->fullIData(oldp+161,((0xfffff000U & vlSelfRef.instr)),32);
    bufp->fullIData(oldp+162,(((((- (IData)((vlSelfRef.instr 
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
