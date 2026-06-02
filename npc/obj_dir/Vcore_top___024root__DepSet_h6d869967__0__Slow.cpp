// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcore_top.h for the primary calling header

#include "Vcore_top__pch.h"
#include "Vcore_top__Syms.h"
#include "Vcore_top___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcore_top___024root___dump_triggers__stl(Vcore_top___024root* vlSelf);
#endif  // VL_DEBUG

VL_ATTR_COLD void Vcore_top___024root___eval_triggers__stl(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_triggers__stl\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered.setBit(0U, (IData)(vlSelfRef.__VstlFirstIteration));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vcore_top___024root___dump_triggers__stl(vlSelf);
    }
#endif
}

void Vcore_top___024unit____Vdpiimwrap_dpi_mmio_read_TOP____024unit(IData/*31:0*/ addr, IData/*31:0*/ &dpi_mmio_read__Vfuncrtn);

VL_ATTR_COLD void Vcore_top___024root___stl_sequent__TOP__0(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___stl_sequent__TOP__0\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.instr = vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
        [(0x1ffffffU & ((vlSelfRef.core_top__DOT__pc 
                         - (IData)(0x80000000U)) >> 2U))];
    vlSelfRef.core_top__DOT__rs2_addr = (0x1fU & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                  [
                                                  (0x1ffffffU 
                                                   & ((vlSelfRef.core_top__DOT__pc 
                                                       - (IData)(0x80000000U)) 
                                                      >> 2U))] 
                                                  >> 0x14U));
    vlSelfRef.core_top__DOT__rd_addr = (0x1fU & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                 [(0x1ffffffU 
                                                   & ((vlSelfRef.core_top__DOT__pc 
                                                       - (IData)(0x80000000U)) 
                                                      >> 2U))] 
                                                 >> 7U));
    vlSelfRef.__VdfgRegularize_h495687df_0_0 = (1U 
                                                & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                   [
                                                   (0x1ffffffU 
                                                    & ((vlSelfRef.core_top__DOT__pc 
                                                        - (IData)(0x80000000U)) 
                                                       >> 2U))] 
                                                   >> 5U));
    vlSelfRef.__VdfgRegularize_h495687df_0_1 = (1U 
                                                & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                   [
                                                   (0x1ffffffU 
                                                    & ((vlSelfRef.core_top__DOT__pc 
                                                        - (IData)(0x80000000U)) 
                                                       >> 2U))] 
                                                   >> 4U));
    vlSelfRef.core_top__DOT__rs1_addr = (0x1fU & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                  [
                                                  (0x1ffffffU 
                                                   & ((vlSelfRef.core_top__DOT__pc 
                                                       - (IData)(0x80000000U)) 
                                                      >> 2U))] 
                                                  >> 0xfU));
    vlSelfRef.core_top__DOT__u_id_stage__DOT__funct7 
        = (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
           [(0x1ffffffU & ((vlSelfRef.core_top__DOT__pc 
                            - (IData)(0x80000000U)) 
                           >> 2U))] >> 0x19U);
    vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3 
        = (7U & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                 [(0x1ffffffU & ((vlSelfRef.core_top__DOT__pc 
                                  - (IData)(0x80000000U)) 
                                 >> 2U))] >> 0xcU));
    vlSelfRef.__VdfgRegularize_h495687df_0_2 = (1U 
                                                & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                   [
                                                   (0x1ffffffU 
                                                    & ((vlSelfRef.core_top__DOT__pc 
                                                        - (IData)(0x80000000U)) 
                                                       >> 2U))] 
                                                   >> 3U));
    vlSelfRef.__VdfgRegularize_h495687df_0_3 = (1U 
                                                & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                   [
                                                   (0x1ffffffU 
                                                    & ((vlSelfRef.core_top__DOT__pc 
                                                        - (IData)(0x80000000U)) 
                                                       >> 2U))] 
                                                   >> 2U));
    vlSelfRef.core_top__DOT__csr_addr = (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                         [(0x1ffffffU 
                                           & ((vlSelfRef.core_top__DOT__pc 
                                               - (IData)(0x80000000U)) 
                                              >> 2U))] 
                                         >> 0x14U);
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0 
        = (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
           [(0x1ffffffU & ((vlSelfRef.core_top__DOT__pc 
                            - (IData)(0x80000000U)) 
                           >> 2U))] >> 0x1fU);
    vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode 
        = (0x7fU & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
           [(0x1ffffffU & ((vlSelfRef.core_top__DOT__pc 
                            - (IData)(0x80000000U)) 
                           >> 2U))]);
    vlSelfRef.__VdfgRegularize_h495687df_0_4 = (1U 
                                                & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                   [
                                                   (0x1ffffffU 
                                                    & ((vlSelfRef.core_top__DOT__pc 
                                                        - (IData)(0x80000000U)) 
                                                       >> 2U))] 
                                                   >> 1U));
    vlSelfRef.__VdfgRegularize_h495687df_0_5 = (1U 
                                                & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                [(0x1ffffffU 
                                                  & ((vlSelfRef.core_top__DOT__pc 
                                                      - (IData)(0x80000000U)) 
                                                     >> 2U))]);
    vlSelfRef.dev_wdata = ((0U == (IData)(vlSelfRef.core_top__DOT__rs2_addr))
                            ? 0U : vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                           [vlSelfRef.core_top__DOT__rs2_addr]);
    vlSelfRef.core_top__DOT__rs1_rdata = ((0U == (IData)(vlSelfRef.core_top__DOT__rs1_addr))
                                           ? 0U : vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                                          [vlSelfRef.core_top__DOT__rs1_addr]);
    vlSelfRef.core_top__DOT__reg_wdata_src = 0U;
    vlSelfRef.core_top__DOT__reg_write = 0U;
    vlSelfRef.core_top__DOT__csr_write = 0U;
    vlSelfRef.core_top__DOT__is_ebreak = 0U;
    vlSelfRef.core_top__DOT__trap_code = 0U;
    vlSelfRef.core_top__DOT__mem_read = 0U;
    vlSelfRef.core_top__DOT__csr_op = 0U;
    vlSelfRef.core_top__DOT__lsu_type = 0U;
    vlSelfRef.core_top__DOT__csr_imm = 0U;
    vlSelfRef.core_top__DOT__pc_sel = 0U;
    vlSelfRef.core_top__DOT__br_type = 0U;
    vlSelfRef.core_top__DOT__trap_enter = 0U;
    vlSelfRef.core_top__DOT__mret = 0U;
    vlSelfRef.core_top__DOT__mem_write = 0U;
    vlSelfRef.core_top__DOT__csr_read = 0U;
    vlSelfRef.core_top__DOT__alu_op = 0U;
    vlSelfRef.core_top__DOT__alu_src_b = 0U;
    if ((0x40U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
        if ((0x20U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
            if ((0x10U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                                  >> 2U)))) {
                        if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                                if ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))) {
                                    if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))) {
                                        vlSelfRef.core_top__DOT__reg_wdata_src = 3U;
                                        vlSelfRef.core_top__DOT__reg_write = 1U;
                                        if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))) {
                                            vlSelfRef.core_top__DOT__csr_write 
                                                = (0U 
                                                   != (IData)(vlSelfRef.core_top__DOT__rs1_addr));
                                            vlSelfRef.core_top__DOT__csr_op = 2U;
                                        } else {
                                            vlSelfRef.core_top__DOT__csr_write 
                                                = (0U 
                                                   != (IData)(vlSelfRef.core_top__DOT__rs1_addr));
                                            vlSelfRef.core_top__DOT__csr_op = 1U;
                                        }
                                        vlSelfRef.core_top__DOT__csr_imm = 0U;
                                        vlSelfRef.core_top__DOT__csr_read = 1U;
                                    } else if ((1U 
                                                & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))) {
                                        vlSelfRef.core_top__DOT__reg_wdata_src = 3U;
                                        vlSelfRef.core_top__DOT__reg_write = 1U;
                                        vlSelfRef.core_top__DOT__csr_write = 1U;
                                        vlSelfRef.core_top__DOT__csr_op = 0U;
                                        vlSelfRef.core_top__DOT__csr_imm = 0U;
                                        vlSelfRef.core_top__DOT__csr_read = 1U;
                                    }
                                } else if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))) {
                                    vlSelfRef.core_top__DOT__reg_wdata_src = 3U;
                                    vlSelfRef.core_top__DOT__reg_write = 1U;
                                    if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))) {
                                        vlSelfRef.core_top__DOT__csr_write 
                                            = (0U != (IData)(vlSelfRef.core_top__DOT__rs1_addr));
                                        vlSelfRef.core_top__DOT__csr_op = 2U;
                                    } else {
                                        vlSelfRef.core_top__DOT__csr_write 
                                            = (0U != (IData)(vlSelfRef.core_top__DOT__rs1_addr));
                                        vlSelfRef.core_top__DOT__csr_op = 1U;
                                    }
                                    vlSelfRef.core_top__DOT__csr_imm = 1U;
                                    vlSelfRef.core_top__DOT__csr_read = 1U;
                                } else if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))) {
                                    vlSelfRef.core_top__DOT__reg_wdata_src = 3U;
                                    vlSelfRef.core_top__DOT__reg_write = 1U;
                                    vlSelfRef.core_top__DOT__csr_write = 1U;
                                    vlSelfRef.core_top__DOT__csr_op = 0U;
                                    vlSelfRef.core_top__DOT__csr_imm = 1U;
                                    vlSelfRef.core_top__DOT__csr_read = 1U;
                                }
                                if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3) 
                                              >> 2U)))) {
                                    if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3) 
                                                  >> 1U)))) {
                                        if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3)))) {
                                            if ((1U 
                                                 == (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                                                vlSelfRef.core_top__DOT__is_ebreak = 1U;
                                            }
                                            if ((1U 
                                                 != (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                                                if (
                                                    (0U 
                                                     == (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                                                    vlSelfRef.core_top__DOT__trap_code = 0xbU;
                                                    vlSelfRef.core_top__DOT__trap_enter = 1U;
                                                }
                                                if (
                                                    (0U 
                                                     != (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                                                    if (
                                                        (0x302U 
                                                         == (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
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
            } else if ((8U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                if ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                    if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                            vlSelfRef.core_top__DOT__reg_wdata_src = 2U;
                            vlSelfRef.core_top__DOT__reg_write = 1U;
                        }
                    }
                }
            } else if ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                    if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                        vlSelfRef.core_top__DOT__reg_wdata_src = 2U;
                        vlSelfRef.core_top__DOT__reg_write = 1U;
                    }
                }
            }
            if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                          >> 4U)))) {
                if ((8U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                    if ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.core_top__DOT__pc_sel = 1U;
                            }
                        }
                    }
                } else if ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                    if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                            vlSelfRef.core_top__DOT__pc_sel = 2U;
                        }
                    }
                } else if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                    if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                        vlSelfRef.core_top__DOT__pc_sel = 3U;
                    }
                }
                if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                                  >> 2U)))) {
                        if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.core_top__DOT__br_type = 0U;
                                if ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))) {
                                    vlSelfRef.core_top__DOT__br_type 
                                        = ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                            ? ((1U 
                                                & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                                ? 5U
                                                : 4U)
                                            : ((1U 
                                                & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                                ? 3U
                                                : 2U));
                                } else if ((1U & (~ 
                                                  ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3) 
                                                   >> 1U)))) {
                                    vlSelfRef.core_top__DOT__br_type 
                                        = ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                            ? 1U : 0U);
                                }
                            }
                        }
                    }
                    if ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.core_top__DOT__alu_op = 1U;
                                vlSelfRef.core_top__DOT__alu_src_b = 1U;
                            }
                        }
                    }
                }
            }
        }
    } else {
        if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                      >> 5U)))) {
            if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                          >> 4U)))) {
                if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                                  >> 2U)))) {
                        if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.core_top__DOT__reg_wdata_src = 1U;
                            }
                        }
                    }
                }
            }
        }
        if ((0x20U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
            if ((0x10U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.core_top__DOT__reg_write = 1U;
                                vlSelfRef.core_top__DOT__alu_op = 0xaU;
                                vlSelfRef.core_top__DOT__alu_src_b = 1U;
                            }
                        }
                    } else if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                            vlSelfRef.core_top__DOT__reg_write = 1U;
                            vlSelfRef.core_top__DOT__alu_op 
                                = ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                    ? ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                        ? ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                            ? 7U : 6U)
                                        : ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                            ? ((0x20U 
                                                & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct7))
                                                ? 9U
                                                : 8U)
                                            : 5U)) : 
                                   ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                     ? ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                         ? 4U : 3U)
                                     : ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                         ? 2U : ((0x20U 
                                                  & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct7))
                                                  ? 0U
                                                  : 1U))));
                        }
                    }
                }
            } else if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                                 >> 3U)))) {
                if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 2U)))) {
                    if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                            vlSelfRef.core_top__DOT__alu_op = 1U;
                            vlSelfRef.core_top__DOT__alu_src_b = 1U;
                        }
                    }
                }
            }
        } else if ((0x10U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
            if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                          >> 3U)))) {
                if ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                    if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                            vlSelfRef.core_top__DOT__reg_write = 1U;
                            vlSelfRef.core_top__DOT__alu_op = 1U;
                            vlSelfRef.core_top__DOT__alu_src_b = 1U;
                        }
                    }
                } else if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                    if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                        vlSelfRef.core_top__DOT__reg_write = 1U;
                        vlSelfRef.core_top__DOT__alu_op 
                            = ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                ? ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                    ? ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                        ? 7U : 6U) : 
                                   ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                     ? ((0x20U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct7))
                                         ? 9U : 8U)
                                     : 5U)) : ((2U 
                                                & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                                ? (
                                                   (1U 
                                                    & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                                    ? 4U
                                                    : 3U)
                                                : (
                                                   (1U 
                                                    & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                                    ? 2U
                                                    : 1U)));
                        vlSelfRef.core_top__DOT__alu_src_b = 1U;
                    }
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                             >> 3U)))) {
            if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                          >> 2U)))) {
                if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                    if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                        vlSelfRef.core_top__DOT__reg_write = 1U;
                        vlSelfRef.core_top__DOT__alu_op = 1U;
                        vlSelfRef.core_top__DOT__alu_src_b = 1U;
                    }
                }
            }
        }
    }
    vlSelfRef.core_top__DOT__alu_src_a = 0U;
    if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                  >> 6U)))) {
        if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                      >> 5U)))) {
            if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                          >> 4U)))) {
                if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                                  >> 2U)))) {
                        if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.core_top__DOT__mem_read = 1U;
                            }
                        }
                    }
                }
            }
            if ((0x10U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.core_top__DOT__alu_src_a = 1U;
                            }
                        }
                    }
                }
            }
        }
        if ((0x20U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
            if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                          >> 4U)))) {
                if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                                  >> 2U)))) {
                        if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                                if ((2U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))) {
                                    vlSelfRef.core_top__DOT__lsu_type = 5U;
                                } else if ((1U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))) {
                                    vlSelfRef.core_top__DOT__lsu_type = 6U;
                                } else if ((0U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))) {
                                    vlSelfRef.core_top__DOT__lsu_type = 7U;
                                }
                                vlSelfRef.core_top__DOT__mem_write = 1U;
                            }
                        }
                    }
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                             >> 4U)))) {
            if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                          >> 3U)))) {
                if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 2U)))) {
                    if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((4U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))) {
                                if ((1U & (~ ((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3) 
                                              >> 1U)))) {
                                    vlSelfRef.core_top__DOT__lsu_type 
                                        = ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                            ? 3U : 4U);
                                }
                            } else if ((2U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))) {
                                if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3)))) {
                                    vlSelfRef.core_top__DOT__lsu_type = 0U;
                                }
                            } else {
                                vlSelfRef.core_top__DOT__lsu_type 
                                    = ((1U & (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))
                                        ? 1U : 2U);
                            }
                        }
                    }
                }
            }
        }
    }
    if (vlSelfRef.__VdfgRegularize_h495687df_0_4) {
        if (vlSelfRef.__VdfgRegularize_h495687df_0_5) {
            vlSelfRef.__VdfgRegularize_h495687df_0_8 
                = (0xfffff000U & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                   [(0x1ffffffU & ((vlSelfRef.core_top__DOT__pc 
                                    - (IData)(0x80000000U)) 
                                   >> 2U))]);
            vlSelfRef.__VdfgRegularize_h495687df_0_6 
                = (((- (IData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                    << 0xcU) | (IData)(vlSelfRef.core_top__DOT__csr_addr));
        } else {
            vlSelfRef.__VdfgRegularize_h495687df_0_8 = 0U;
            vlSelfRef.__VdfgRegularize_h495687df_0_6 = 0U;
        }
    } else {
        vlSelfRef.__VdfgRegularize_h495687df_0_8 = 0U;
        vlSelfRef.__VdfgRegularize_h495687df_0_6 = 0U;
    }
    vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src 
        = ((IData)(vlSelfRef.core_top__DOT__csr_imm)
            ? vlSelfRef.core_top__DOT__rs1_rdata : (IData)(vlSelfRef.core_top__DOT__rs1_addr));
    vlSelfRef.dev_r_w = vlSelfRef.core_top__DOT__mem_write;
    vlSelfRef.core_top__DOT__csr_rdata = ((IData)(vlSelfRef.core_top__DOT__csr_read)
                                           ? ((0x300U 
                                               == (IData)(vlSelfRef.core_top__DOT__csr_addr))
                                               ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus
                                               : ((0x305U 
                                                   == (IData)(vlSelfRef.core_top__DOT__csr_addr))
                                                   ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                                                   : 
                                                  ((0x341U 
                                                    == (IData)(vlSelfRef.core_top__DOT__csr_addr))
                                                    ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                                                    : 
                                                   ((0x342U 
                                                     == (IData)(vlSelfRef.core_top__DOT__csr_addr))
                                                     ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause
                                                     : 
                                                    ((0x344U 
                                                      == (IData)(vlSelfRef.core_top__DOT__csr_addr))
                                                      ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mip
                                                      : 
                                                     ((0x304U 
                                                       == (IData)(vlSelfRef.core_top__DOT__csr_addr))
                                                       ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mie
                                                       : 
                                                      ((0x343U 
                                                        == (IData)(vlSelfRef.core_top__DOT__csr_addr))
                                                        ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtval
                                                        : 
                                                       ((0x340U 
                                                         == (IData)(vlSelfRef.core_top__DOT__csr_addr))
                                                         ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mscratch
                                                         : 
                                                        ((0xb00U 
                                                          == (IData)(vlSelfRef.core_top__DOT__csr_addr))
                                                          ? (IData)(vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64)
                                                          : 
                                                         ((0xb80U 
                                                           == (IData)(vlSelfRef.core_top__DOT__csr_addr))
                                                           ? (IData)(
                                                                     (vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64 
                                                                      >> 0x20U))
                                                           : 0U))))))))))
                                           : 0U);
    vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
        = ((IData)(vlSelfRef.core_top__DOT__alu_src_a)
            ? vlSelfRef.core_top__DOT__pc : vlSelfRef.core_top__DOT__rs1_rdata);
    vlSelfRef.__VdfgRegularize_h495687df_0_7 = ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_2)
                                                 ? 0U
                                                 : 
                                                ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_3)
                                                  ? 0U
                                                  : vlSelfRef.__VdfgRegularize_h495687df_0_6));
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
    vlSelfRef.core_top__DOT__imm_jal = ((0x40U & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                         [(0x1ffffffU 
                                           & ((vlSelfRef.core_top__DOT__pc 
                                               - (IData)(0x80000000U)) 
                                              >> 2U))])
                                         ? ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_0)
                                             ? ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_1)
                                                 ? vlSelfRef.__VdfgRegularize_h495687df_0_7
                                                 : 
                                                ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_2)
                                                  ? 
                                                 ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_3)
                                                   ? 
                                                  ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_4)
                                                    ? 
                                                   ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_5)
                                                     ? 
                                                    (((- (IData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                                      << 0x14U) 
                                                     | (((0xff000U 
                                                          & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                          [
                                                          (0x1ffffffU 
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
                                                              >> 0x14U))))
                                                     : 0U)
                                                    : 0U)
                                                   : 0U)
                                                  : 
                                                 ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_3)
                                                   ? vlSelfRef.__VdfgRegularize_h495687df_0_6
                                                   : 
                                                  ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_4)
                                                    ? 
                                                   ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_5)
                                                     ? 
                                                    (((- (IData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                                      << 0xdU) 
                                                     | ((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0) 
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
                                                                 >> 7U)))))
                                                     : 0U)
                                                    : 0U))))
                                             : 0U) : 
                                        ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_0)
                                          ? ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_1)
                                              ? ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_2)
                                                  ? 0U
                                                  : 
                                                 ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_3)
                                                   ? vlSelfRef.__VdfgRegularize_h495687df_0_8
                                                   : 0U))
                                              : ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_2)
                                                  ? 0U
                                                  : 
                                                 ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_3)
                                                   ? 0U
                                                   : 
                                                  ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_4)
                                                    ? 
                                                   ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_5)
                                                     ? 
                                                    (((- (IData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                                      << 0xcU) 
                                                     | (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct7) 
                                                         << 5U) 
                                                        | (IData)(vlSelfRef.core_top__DOT__rd_addr)))
                                                     : 0U)
                                                    : 0U))))
                                          : ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_1)
                                              ? ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_2)
                                                  ? 0U
                                                  : 
                                                 ((IData)(vlSelfRef.__VdfgRegularize_h495687df_0_3)
                                                   ? vlSelfRef.__VdfgRegularize_h495687df_0_8
                                                   : vlSelfRef.__VdfgRegularize_h495687df_0_6))
                                              : vlSelfRef.__VdfgRegularize_h495687df_0_7)));
    vlSelfRef.__VdfgRegularize_h3c5bdd75_0_0 = (vlSelfRef.core_top__DOT__imm_jal 
                                                + vlSelfRef.core_top__DOT__pc);
    vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2 
        = ((0U == (IData)(vlSelfRef.core_top__DOT__alu_src_b))
            ? vlSelfRef.dev_wdata : ((1U == (IData)(vlSelfRef.core_top__DOT__alu_src_b))
                                      ? vlSelfRef.core_top__DOT__imm_jal
                                      : 4U));
    vlSelfRef.dev_addr = ((0x10U & (IData)(vlSelfRef.core_top__DOT__alu_op))
                           ? 0U : ((8U & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                    ? ((4U & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                        ? 0U : ((2U 
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
                                    : ((4U & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                        ? ((2U & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                            ? ((1U 
                                                & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                ? (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                   & vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)
                                                : (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                   | vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))
                                            : ((1U 
                                                & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                ? (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                   ^ vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)
                                                : (
                                                   (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                    < vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)
                                                    ? 1U
                                                    : 0U)))
                                        : ((2U & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                            ? ((1U 
                                                & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                ? (
                                                   VL_LTS_III(32, vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1, vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)
                                                    ? 1U
                                                    : 0U)
                                                : (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                   << 
                                                   (0x1fU 
                                                    & vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)))
                                            : ((1U 
                                                & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                ? (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                   + vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)
                                                : (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                   - vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))))));
    if (vlSelfRef.core_top__DOT__mem_write) {
        if ((5U == (IData)(vlSelfRef.core_top__DOT__lsu_type))) {
            vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata 
                = vlSelfRef.dev_wdata;
            vlSelfRef.dev_wmask = 0xfU;
        } else if ((6U == (IData)(vlSelfRef.core_top__DOT__lsu_type))) {
            vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata 
                = ((vlSelfRef.dev_wdata << 0x10U) | 
                   (0xffffU & vlSelfRef.dev_wdata));
            vlSelfRef.dev_wmask = (0xfU & ((2U & vlSelfRef.dev_addr)
                                            ? 0xcU : 3U));
        } else {
            vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata 
                = ((vlSelfRef.dev_wdata << 0x18U) | 
                   ((0xff0000U & (vlSelfRef.dev_wdata 
                                  << 0x10U)) | ((0xff00U 
                                                 & (vlSelfRef.dev_wdata 
                                                    << 8U)) 
                                                | (0xffU 
                                                   & vlSelfRef.dev_wdata))));
            vlSelfRef.dev_wmask = (0xfU & ((IData)(1U) 
                                           << (3U & vlSelfRef.dev_addr)));
        }
    } else {
        vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata = 0U;
        vlSelfRef.dev_wmask = 0U;
    }
    vlSelfRef.core_top__DOT__u_if_stage__DOT__pc_next 
        = (((IData)(vlSelfRef.core_top__DOT__mret) 
            | (IData)(vlSelfRef.core_top__DOT__trap_enter))
            ? ((IData)(vlSelfRef.core_top__DOT__trap_enter)
                ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec
                : ((IData)(vlSelfRef.core_top__DOT__mret)
                    ? vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc
                    : 0U)) : ((0U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                               ? ((IData)(4U) + vlSelfRef.core_top__DOT__pc)
                               : ((1U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                   ? vlSelfRef.__VdfgRegularize_h3c5bdd75_0_0
                                   : ((2U == (IData)(vlSelfRef.core_top__DOT__pc_sel))
                                       ? (0xfffffffeU 
                                          & vlSelfRef.dev_addr)
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
                                                       >= vlSelfRef.dev_wdata)
                                                       : 
                                                      (vlSelfRef.core_top__DOT__rs1_rdata 
                                                       < vlSelfRef.dev_wdata)))
                                                : (
                                                   (2U 
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
                                               : ((IData)(4U) 
                                                  + vlSelfRef.core_top__DOT__pc))
                                           : ((IData)(4U) 
                                              + vlSelfRef.core_top__DOT__pc))))));
    Vcore_top___024unit____Vdpiimwrap_dpi_mmio_read_TOP____024unit(vlSelfRef.dev_addr, vlSelfRef.__Vfunc_dpi_mmio_read__0__Vfuncout);
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__dev_rdata 
        = vlSelfRef.__Vfunc_dpi_mmio_read__0__Vfuncout;
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__in_pmem 
        = ((0x80000000U <= vlSelfRef.dev_addr) & (0x88000000U 
                                                  > vlSelfRef.dev_addr));
    vlSelfRef.dev_req = ((~ (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__in_pmem)) 
                         & ((IData)(vlSelfRef.core_top__DOT__mem_read) 
                            | (IData)(vlSelfRef.core_top__DOT__mem_write)));
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
        = ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__in_pmem)
            ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
           [(0x1ffffffU & ((vlSelfRef.dev_addr - (IData)(0x80000000U)) 
                           >> 2U))] : vlSelfRef.core_top__DOT__u_mem_stage__DOT__dev_rdata);
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected 
        = (0xffU & ((0U == (3U & vlSelfRef.dev_addr))
                     ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw
                     : ((1U == (3U & vlSelfRef.dev_addr))
                         ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                            >> 8U) : ((2U == (3U & vlSelfRef.dev_addr))
                                       ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 0x10U)
                                       : (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 0x18U)))));
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected 
        = (0xffffU & ((2U & vlSelfRef.dev_addr) ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                                   >> 0x10U)
                       : vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw));
}
