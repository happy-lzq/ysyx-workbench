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

void Vcore_top___024unit____Vdpiimwrap_dpi_mem_read_TOP____024unit(IData/*31:0*/ addr, IData/*31:0*/ is_load, IData/*31:0*/ &dpi_mem_read__Vfuncrtn);

VL_ATTR_COLD void Vcore_top___024root___stl_sequent__TOP__0(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___stl_sequent__TOP__0\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
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
                            = ((1U == (vlSelfRef.instr 
                                       >> 0x19U)) ? 
                               ((0x4000U & vlSelfRef.instr)
                                 ? ((0x2000U & vlSelfRef.instr)
                                     ? ((0x1000U & vlSelfRef.instr)
                                         ? 0x17U : 0x16U)
                                     : ((0x1000U & vlSelfRef.instr)
                                         ? 0x15U : 0x14U))
                                 : ((0x2000U & vlSelfRef.instr)
                                     ? ((0x1000U & vlSelfRef.instr)
                                         ? 0x13U : 0x12U)
                                     : ((0x1000U & vlSelfRef.instr)
                                         ? 0x11U : 0x10U)))
                                : ((0x4000U & vlSelfRef.instr)
                                    ? ((0x2000U & vlSelfRef.instr)
                                        ? ((0x1000U 
                                            & vlSelfRef.instr)
                                            ? 7U : 6U)
                                        : ((0x1000U 
                                            & vlSelfRef.instr)
                                            ? ((0x40000000U 
                                                & vlSelfRef.instr)
                                                ? 9U
                                                : 8U)
                                            : 5U)) : 
                                   ((0x2000U & vlSelfRef.instr)
                                     ? ((0x1000U & vlSelfRef.instr)
                                         ? 4U : 3U)
                                     : ((0x1000U & vlSelfRef.instr)
                                         ? 2U : ((0x40000000U 
                                                  & vlSelfRef.instr)
                                                  ? 0U
                                                  : 1U)))));
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
    if ((0x10U & (IData)(vlSelfRef.core_top__DOT__alu_op))) {
        if ((8U & (IData)(vlSelfRef.core_top__DOT__alu_op))) {
            vlSelfRef.core_top__DOT__alu_result = 0U;
        } else if ((4U & (IData)(vlSelfRef.core_top__DOT__alu_op))) {
            if ((2U & (IData)(vlSelfRef.core_top__DOT__alu_op))) {
                if ((1U & (IData)(vlSelfRef.core_top__DOT__alu_op))) {
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
            } else if ((1U & (IData)(vlSelfRef.core_top__DOT__alu_op))) {
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
                                                   (2U 
                                                    & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                    ? 0U
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                     ? 0U
                                                     : 
                                                    (vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
                                                     * vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)));
        }
    } else {
        vlSelfRef.core_top__DOT__alu_result = ((8U 
                                                & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                ? (
                                                   (4U 
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
                                                : (
                                                   (4U 
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
                                                      - vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2)))));
    }
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
                                          & vlSelfRef.core_top__DOT__alu_result)
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
    Vcore_top___024unit____Vdpiimwrap_dpi_mem_read_TOP____024unit(vlSelfRef.core_top__DOT__alu_result, (IData)(vlSelfRef.core_top__DOT__mem_read_eff), vlSelfRef.__Vfunc_dpi_mem_read__5__Vfuncout);
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
        = vlSelfRef.__Vfunc_dpi_mem_read__5__Vfuncout;
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected 
        = (0xffU & ((0U == (3U & vlSelfRef.core_top__DOT__alu_result))
                     ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw
                     : ((1U == (3U & vlSelfRef.core_top__DOT__alu_result))
                         ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                            >> 8U) : ((2U == (3U & vlSelfRef.core_top__DOT__alu_result))
                                       ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 0x10U)
                                       : (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                                          >> 0x18U)))));
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected 
        = (0xffffU & ((2U & vlSelfRef.core_top__DOT__alu_result)
                       ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw 
                          >> 0x10U) : vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_rdata_raw));
}
