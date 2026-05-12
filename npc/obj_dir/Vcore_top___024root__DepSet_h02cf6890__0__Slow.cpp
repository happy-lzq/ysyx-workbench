// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcore_top.h for the primary calling header

#include "Vcore_top__pch.h"
#include "Vcore_top___024root.h"

VL_ATTR_COLD void Vcore_top___024root___eval_static(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_static\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
}

VL_ATTR_COLD void Vcore_top___024root___eval_initial(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_initial\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Vcore_top___024root___eval_final(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_final\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcore_top___024root___dump_triggers__stl(Vcore_top___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vcore_top___024root___eval_phase__stl(Vcore_top___024root* vlSelf);

VL_ATTR_COLD void Vcore_top___024root___eval_settle(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_settle\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ __VstlIterCount;
    CData/*0:0*/ __VstlContinue;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    __VstlContinue = 1U;
    while (__VstlContinue) {
        if (VL_UNLIKELY(((0x64U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vcore_top___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("vsrc/core_top.v", 1, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vcore_top___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelfRef.__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcore_top___024root___dump_triggers__stl(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___dump_triggers__stl\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VstlTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vcore_top___024root___stl_sequent__TOP__0(Vcore_top___024root* vlSelf);

VL_ATTR_COLD void Vcore_top___024root___eval_stl(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_stl\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        Vcore_top___024root___stl_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD void Vcore_top___024root___stl_sequent__TOP__0(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___stl_sequent__TOP__0\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*4:0*/ core_top__DOT__rs1_addr;
    core_top__DOT__rs1_addr = 0;
    CData/*4:0*/ core_top__DOT__rs2_addr;
    core_top__DOT__rs2_addr = 0;
    IData/*31:0*/ core_top__DOT__rs1_rdata;
    core_top__DOT__rs1_rdata = 0;
    IData/*31:0*/ core_top__DOT__rs2_rdata;
    core_top__DOT__rs2_rdata = 0;
    CData/*1:0*/ core_top__DOT__pc_sel;
    core_top__DOT__pc_sel = 0;
    CData/*4:0*/ core_top__DOT__alu_op;
    core_top__DOT__alu_op = 0;
    CData/*0:0*/ core_top__DOT__alu_src_a;
    core_top__DOT__alu_src_a = 0;
    CData/*1:0*/ core_top__DOT__alu_src_b;
    core_top__DOT__alu_src_b = 0;
    CData/*2:0*/ core_top__DOT__br_type;
    core_top__DOT__br_type = 0;
    IData/*31:0*/ core_top__DOT__imm_out;
    core_top__DOT__imm_out = 0;
    CData/*6:0*/ core_top__DOT__u_id_stage__DOT__opcode;
    core_top__DOT__u_id_stage__DOT__opcode = 0;
    CData/*2:0*/ core_top__DOT__u_id_stage__DOT__funct3;
    core_top__DOT__u_id_stage__DOT__funct3 = 0;
    CData/*6:0*/ core_top__DOT__u_id_stage__DOT__funct7;
    core_top__DOT__u_id_stage__DOT__funct7 = 0;
    CData/*0:0*/ core_top__DOT__u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0;
    core_top__DOT__u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0 = 0;
    IData/*31:0*/ core_top__DOT__u_ex_stage__DOT__src1;
    core_top__DOT__u_ex_stage__DOT__src1 = 0;
    IData/*31:0*/ core_top__DOT__u_ex_stage__DOT__src2;
    core_top__DOT__u_ex_stage__DOT__src2 = 0;
    IData/*31:0*/ __VdfgRegularize_h3c5bdd75_0_0;
    __VdfgRegularize_h3c5bdd75_0_0 = 0;
    CData/*0:0*/ __VdfgRegularize_h495687df_0_0;
    __VdfgRegularize_h495687df_0_0 = 0;
    CData/*0:0*/ __VdfgRegularize_h495687df_0_1;
    __VdfgRegularize_h495687df_0_1 = 0;
    CData/*0:0*/ __VdfgRegularize_h495687df_0_2;
    __VdfgRegularize_h495687df_0_2 = 0;
    CData/*0:0*/ __VdfgRegularize_h495687df_0_3;
    __VdfgRegularize_h495687df_0_3 = 0;
    CData/*0:0*/ __VdfgRegularize_h495687df_0_4;
    __VdfgRegularize_h495687df_0_4 = 0;
    CData/*0:0*/ __VdfgRegularize_h495687df_0_5;
    __VdfgRegularize_h495687df_0_5 = 0;
    IData/*31:0*/ __VdfgRegularize_h495687df_0_6;
    __VdfgRegularize_h495687df_0_6 = 0;
    IData/*31:0*/ __VdfgRegularize_h495687df_0_7;
    __VdfgRegularize_h495687df_0_7 = 0;
    IData/*31:0*/ __VdfgRegularize_h495687df_0_8;
    __VdfgRegularize_h495687df_0_8 = 0;
    // Body
    vlSelfRef.debug_pc = vlSelfRef.core_top__DOT__pc;
    core_top__DOT__rs1_addr = (0x1fU & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                        [(0xffffU & 
                                          (vlSelfRef.core_top__DOT__pc 
                                           >> 2U))] 
                                        >> 0xfU));
    core_top__DOT__rs2_addr = (0x1fU & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                        [(0xffffU & 
                                          (vlSelfRef.core_top__DOT__pc 
                                           >> 2U))] 
                                        >> 0x14U));
    core_top__DOT__u_id_stage__DOT__funct3 = (7U & 
                                              (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                               [(0xffffU 
                                                 & (vlSelfRef.core_top__DOT__pc 
                                                    >> 2U))] 
                                               >> 0xcU));
    vlSelfRef.core_top__DOT__rd_addr = (0x1fU & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                 [(0xffffU 
                                                   & (vlSelfRef.core_top__DOT__pc 
                                                      >> 2U))] 
                                                 >> 7U));
    __VdfgRegularize_h495687df_0_0 = (1U & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                            [(0xffffU 
                                              & (vlSelfRef.core_top__DOT__pc 
                                                 >> 2U))] 
                                            >> 5U));
    __VdfgRegularize_h495687df_0_1 = (1U & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                            [(0xffffU 
                                              & (vlSelfRef.core_top__DOT__pc 
                                                 >> 2U))] 
                                            >> 4U));
    core_top__DOT__u_id_stage__DOT__funct7 = (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                              [(0xffffU 
                                                & (vlSelfRef.core_top__DOT__pc 
                                                   >> 2U))] 
                                              >> 0x19U);
    __VdfgRegularize_h495687df_0_2 = (1U & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                            [(0xffffU 
                                              & (vlSelfRef.core_top__DOT__pc 
                                                 >> 2U))] 
                                            >> 3U));
    __VdfgRegularize_h495687df_0_3 = (1U & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                            [(0xffffU 
                                              & (vlSelfRef.core_top__DOT__pc 
                                                 >> 2U))] 
                                            >> 2U));
    core_top__DOT__u_id_stage__DOT__opcode = (0x7fU 
                                              & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                              [(0xffffU 
                                                & (vlSelfRef.core_top__DOT__pc 
                                                   >> 2U))]);
    core_top__DOT__u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0 
        = (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
           [(0xffffU & (vlSelfRef.core_top__DOT__pc 
                        >> 2U))] >> 0x1fU);
    __VdfgRegularize_h495687df_0_4 = (1U & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                            [(0xffffU 
                                              & (vlSelfRef.core_top__DOT__pc 
                                                 >> 2U))] 
                                            >> 1U));
    __VdfgRegularize_h495687df_0_5 = (1U & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                      [(0xffffU & (vlSelfRef.core_top__DOT__pc 
                                                   >> 2U))]);
    core_top__DOT__rs1_rdata = ((0U == (IData)(core_top__DOT__rs1_addr))
                                 ? 0U : vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                                [core_top__DOT__rs1_addr]);
    core_top__DOT__rs2_rdata = ((0U == (IData)(core_top__DOT__rs2_addr))
                                 ? 0U : vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                                [core_top__DOT__rs2_addr]);
    vlSelfRef.core_top__DOT__mem_read = 0U;
    vlSelfRef.core_top__DOT__reg_write = 0U;
    vlSelfRef.core_top__DOT__reg_wdata_src = 0U;
    core_top__DOT__pc_sel = 0U;
    core_top__DOT__br_type = 0U;
    vlSelfRef.core_top__DOT__mem_write = 0U;
    vlSelfRef.core_top__DOT__lsu_type = 0U;
    core_top__DOT__alu_op = 0U;
    core_top__DOT__alu_src_a = 0U;
    if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                  >> 6U)))) {
        if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                      >> 5U)))) {
            if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                          >> 4U)))) {
                if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                                  >> 2U)))) {
                        if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.core_top__DOT__mem_read = 1U;
                            }
                        }
                    }
                }
            }
            if ((0x10U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((4U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                                core_top__DOT__alu_src_a = 1U;
                            }
                        }
                    }
                }
            }
        }
        if ((0x20U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
            if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                          >> 4U)))) {
                if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                                  >> 2U)))) {
                        if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.core_top__DOT__mem_write = 1U;
                                if ((2U == (IData)(core_top__DOT__u_id_stage__DOT__funct3))) {
                                    vlSelfRef.core_top__DOT__lsu_type = 5U;
                                } else if ((1U == (IData)(core_top__DOT__u_id_stage__DOT__funct3))) {
                                    vlSelfRef.core_top__DOT__lsu_type = 6U;
                                } else if ((0U == (IData)(core_top__DOT__u_id_stage__DOT__funct3))) {
                                    vlSelfRef.core_top__DOT__lsu_type = 7U;
                                }
                            }
                        }
                    }
                }
            }
        } else if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                             >> 4U)))) {
            if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                          >> 3U)))) {
                if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 2U)))) {
                    if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((1U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((4U & (IData)(core_top__DOT__u_id_stage__DOT__funct3))) {
                                if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__funct3) 
                                              >> 1U)))) {
                                    vlSelfRef.core_top__DOT__lsu_type 
                                        = ((1U & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                            ? 3U : 4U);
                                }
                            } else if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__funct3))) {
                                if ((1U & (~ (IData)(core_top__DOT__u_id_stage__DOT__funct3)))) {
                                    vlSelfRef.core_top__DOT__lsu_type = 0U;
                                }
                            } else {
                                vlSelfRef.core_top__DOT__lsu_type 
                                    = ((1U & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                        ? 1U : 2U);
                            }
                        }
                    }
                }
            }
        }
    }
    core_top__DOT__alu_src_b = 0U;
    if ((0x40U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
        if ((0x20U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
            if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                          >> 4U)))) {
                if ((8U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                    if ((4U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.core_top__DOT__reg_write = 1U;
                                vlSelfRef.core_top__DOT__reg_wdata_src = 2U;
                                core_top__DOT__pc_sel = 1U;
                            }
                        }
                    }
                } else if ((4U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                    if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((1U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                            vlSelfRef.core_top__DOT__reg_write = 1U;
                            vlSelfRef.core_top__DOT__reg_wdata_src = 2U;
                            core_top__DOT__pc_sel = 2U;
                        }
                    }
                } else if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                    if ((1U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                        core_top__DOT__pc_sel = 3U;
                    }
                }
                if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                                  >> 2U)))) {
                        if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                                core_top__DOT__br_type = 0U;
                                if ((4U & (IData)(core_top__DOT__u_id_stage__DOT__funct3))) {
                                    core_top__DOT__br_type 
                                        = ((2U & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                            ? ((1U 
                                                & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                                ? 5U
                                                : 4U)
                                            : ((1U 
                                                & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                                ? 3U
                                                : 2U));
                                } else if ((1U & (~ 
                                                  ((IData)(core_top__DOT__u_id_stage__DOT__funct3) 
                                                   >> 1U)))) {
                                    core_top__DOT__br_type 
                                        = ((1U & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                            ? 1U : 0U);
                                }
                            }
                        }
                    }
                    if ((4U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                                core_top__DOT__alu_op = 1U;
                                core_top__DOT__alu_src_b = 1U;
                            }
                        }
                    }
                }
            }
        }
    } else {
        if ((0x20U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
            if ((0x10U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((4U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.core_top__DOT__reg_write = 1U;
                                core_top__DOT__alu_op = 0xaU;
                                core_top__DOT__alu_src_b = 1U;
                            }
                        }
                    } else if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((1U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                            vlSelfRef.core_top__DOT__reg_write = 1U;
                            core_top__DOT__alu_op = 
                                ((4U & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                  ? ((2U & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                      ? ((1U & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                          ? 7U : 6U)
                                      : ((1U & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                          ? ((0x20U 
                                              & (IData)(core_top__DOT__u_id_stage__DOT__funct7))
                                              ? 9U : 8U)
                                          : 5U)) : 
                                 ((2U & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                   ? ((1U & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                       ? 4U : 3U) : 
                                  ((1U & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                    ? 2U : ((0x20U 
                                             & (IData)(core_top__DOT__u_id_stage__DOT__funct7))
                                             ? 0U : 1U))));
                        }
                    }
                }
            } else if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                                 >> 3U)))) {
                if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 2U)))) {
                    if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((1U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                            core_top__DOT__alu_op = 1U;
                            core_top__DOT__alu_src_b = 1U;
                        }
                    }
                }
            }
        } else if ((0x10U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
            if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                          >> 3U)))) {
                if ((4U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                    if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                        if ((1U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                            vlSelfRef.core_top__DOT__reg_write = 1U;
                            core_top__DOT__alu_op = 1U;
                            core_top__DOT__alu_src_b = 1U;
                        }
                    }
                } else if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                    if ((1U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                        vlSelfRef.core_top__DOT__reg_write = 1U;
                        core_top__DOT__alu_op = ((4U 
                                                  & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                                    ? 7U
                                                    : 6U)
                                                   : 
                                                  ((1U 
                                                    & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                                    ? 
                                                   ((0x20U 
                                                     & (IData)(core_top__DOT__u_id_stage__DOT__funct7))
                                                     ? 9U
                                                     : 8U)
                                                    : 5U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                                    ? 4U
                                                    : 3U)
                                                   : 
                                                  ((1U 
                                                    & (IData)(core_top__DOT__u_id_stage__DOT__funct3))
                                                    ? 2U
                                                    : 1U)));
                        core_top__DOT__alu_src_b = 1U;
                    }
                }
            }
        } else if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                             >> 3U)))) {
            if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                          >> 2U)))) {
                if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                    if ((1U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                        vlSelfRef.core_top__DOT__reg_write = 1U;
                        core_top__DOT__alu_op = 1U;
                        core_top__DOT__alu_src_b = 1U;
                    }
                }
            }
        }
        if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                      >> 5U)))) {
            if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                          >> 4U)))) {
                if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((1U & (~ ((IData)(core_top__DOT__u_id_stage__DOT__opcode) 
                                  >> 2U)))) {
                        if ((2U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(core_top__DOT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.core_top__DOT__reg_wdata_src = 1U;
                            }
                        }
                    }
                }
            }
        }
    }
    if (__VdfgRegularize_h495687df_0_4) {
        if (__VdfgRegularize_h495687df_0_5) {
            __VdfgRegularize_h495687df_0_8 = (0xfffff000U 
                                              & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                              [(0xffffU 
                                                & (vlSelfRef.core_top__DOT__pc 
                                                   >> 2U))]);
            __VdfgRegularize_h495687df_0_6 = (((- (IData)((IData)(core_top__DOT__u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                               << 0xcU) 
                                              | (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                 [(0xffffU 
                                                   & (vlSelfRef.core_top__DOT__pc 
                                                      >> 2U))] 
                                                 >> 0x14U));
        } else {
            __VdfgRegularize_h495687df_0_8 = 0U;
            __VdfgRegularize_h495687df_0_6 = 0U;
        }
    } else {
        __VdfgRegularize_h495687df_0_8 = 0U;
        __VdfgRegularize_h495687df_0_6 = 0U;
    }
    core_top__DOT__u_ex_stage__DOT__src1 = ((IData)(core_top__DOT__alu_src_a)
                                             ? vlSelfRef.core_top__DOT__pc
                                             : core_top__DOT__rs1_rdata);
    __VdfgRegularize_h495687df_0_7 = ((IData)(__VdfgRegularize_h495687df_0_2)
                                       ? 0U : ((IData)(__VdfgRegularize_h495687df_0_3)
                                                ? 0U
                                                : __VdfgRegularize_h495687df_0_6));
    core_top__DOT__imm_out = ((0x40U & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                               [(0xffffU & (vlSelfRef.core_top__DOT__pc 
                                            >> 2U))])
                               ? ((IData)(__VdfgRegularize_h495687df_0_0)
                                   ? ((IData)(__VdfgRegularize_h495687df_0_1)
                                       ? __VdfgRegularize_h495687df_0_7
                                       : ((IData)(__VdfgRegularize_h495687df_0_2)
                                           ? ((IData)(__VdfgRegularize_h495687df_0_3)
                                               ? ((IData)(__VdfgRegularize_h495687df_0_4)
                                                   ? 
                                                  ((IData)(__VdfgRegularize_h495687df_0_5)
                                                    ? 
                                                   (((- (IData)((IData)(core_top__DOT__u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                                     << 0x14U) 
                                                    | (((0xff000U 
                                                         & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                         [
                                                         (0xffffU 
                                                          & (vlSelfRef.core_top__DOT__pc 
                                                             >> 2U))]) 
                                                        | (0x800U 
                                                           & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                              [
                                                              (0xffffU 
                                                               & (vlSelfRef.core_top__DOT__pc 
                                                                  >> 2U))] 
                                                              >> 9U))) 
                                                       | (0x7feU 
                                                          & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                             [
                                                             (0xffffU 
                                                              & (vlSelfRef.core_top__DOT__pc 
                                                                 >> 2U))] 
                                                             >> 0x14U))))
                                                    : 0U)
                                                   : 0U)
                                               : 0U)
                                           : ((IData)(__VdfgRegularize_h495687df_0_3)
                                               ? __VdfgRegularize_h495687df_0_6
                                               : ((IData)(__VdfgRegularize_h495687df_0_4)
                                                   ? 
                                                  ((IData)(__VdfgRegularize_h495687df_0_5)
                                                    ? 
                                                   (((- (IData)((IData)(core_top__DOT__u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                                     << 0xdU) 
                                                    | ((((IData)(core_top__DOT__u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0) 
                                                         << 0xcU) 
                                                        | (0x800U 
                                                           & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                              [
                                                              (0xffffU 
                                                               & (vlSelfRef.core_top__DOT__pc 
                                                                  >> 2U))] 
                                                              << 4U))) 
                                                       | ((0x7e0U 
                                                           & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                              [
                                                              (0xffffU 
                                                               & (vlSelfRef.core_top__DOT__pc 
                                                                  >> 2U))] 
                                                              >> 0x14U)) 
                                                          | (0x1eU 
                                                             & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                                [
                                                                (0xffffU 
                                                                 & (vlSelfRef.core_top__DOT__pc 
                                                                    >> 2U))] 
                                                                >> 7U)))))
                                                    : 0U)
                                                   : 0U))))
                                   : 0U) : ((IData)(__VdfgRegularize_h495687df_0_0)
                                             ? ((IData)(__VdfgRegularize_h495687df_0_1)
                                                 ? 
                                                ((IData)(__VdfgRegularize_h495687df_0_2)
                                                  ? 0U
                                                  : 
                                                 ((IData)(__VdfgRegularize_h495687df_0_3)
                                                   ? __VdfgRegularize_h495687df_0_8
                                                   : 0U))
                                                 : 
                                                ((IData)(__VdfgRegularize_h495687df_0_2)
                                                  ? 0U
                                                  : 
                                                 ((IData)(__VdfgRegularize_h495687df_0_3)
                                                   ? 0U
                                                   : 
                                                  ((IData)(__VdfgRegularize_h495687df_0_4)
                                                    ? 
                                                   ((IData)(__VdfgRegularize_h495687df_0_5)
                                                     ? 
                                                    (((- (IData)((IData)(core_top__DOT__u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                                      << 0xcU) 
                                                     | (((IData)(core_top__DOT__u_id_stage__DOT__funct7) 
                                                         << 5U) 
                                                        | (IData)(vlSelfRef.core_top__DOT__rd_addr)))
                                                     : 0U)
                                                    : 0U))))
                                             : ((IData)(__VdfgRegularize_h495687df_0_1)
                                                 ? 
                                                ((IData)(__VdfgRegularize_h495687df_0_2)
                                                  ? 0U
                                                  : 
                                                 ((IData)(__VdfgRegularize_h495687df_0_3)
                                                   ? __VdfgRegularize_h495687df_0_8
                                                   : __VdfgRegularize_h495687df_0_6))
                                                 : __VdfgRegularize_h495687df_0_7)));
    __VdfgRegularize_h3c5bdd75_0_0 = (core_top__DOT__imm_out 
                                      + vlSelfRef.core_top__DOT__pc);
    core_top__DOT__u_ex_stage__DOT__src2 = ((0U == (IData)(core_top__DOT__alu_src_b))
                                             ? core_top__DOT__rs2_rdata
                                             : ((1U 
                                                 == (IData)(core_top__DOT__alu_src_b))
                                                 ? core_top__DOT__imm_out
                                                 : 4U));
    vlSelfRef.core_top__DOT__alu_result = ((0x10U & (IData)(core_top__DOT__alu_op))
                                            ? 0U : 
                                           ((8U & (IData)(core_top__DOT__alu_op))
                                             ? ((4U 
                                                 & (IData)(core_top__DOT__alu_op))
                                                 ? 0U
                                                 : 
                                                ((2U 
                                                  & (IData)(core_top__DOT__alu_op))
                                                  ? 
                                                 ((1U 
                                                   & (IData)(core_top__DOT__alu_op))
                                                   ? 0U
                                                   : core_top__DOT__u_ex_stage__DOT__src2)
                                                  : 
                                                 ((1U 
                                                   & (IData)(core_top__DOT__alu_op))
                                                   ? 
                                                  VL_SHIFTRS_III(32,32,5, core_top__DOT__u_ex_stage__DOT__src1, 
                                                                 (0x1fU 
                                                                  & core_top__DOT__u_ex_stage__DOT__src2))
                                                   : 
                                                  (core_top__DOT__u_ex_stage__DOT__src1 
                                                   >> 
                                                   (0x1fU 
                                                    & core_top__DOT__u_ex_stage__DOT__src2)))))
                                             : ((4U 
                                                 & (IData)(core_top__DOT__alu_op))
                                                 ? 
                                                ((2U 
                                                  & (IData)(core_top__DOT__alu_op))
                                                  ? 
                                                 ((1U 
                                                   & (IData)(core_top__DOT__alu_op))
                                                   ? 
                                                  (core_top__DOT__u_ex_stage__DOT__src1 
                                                   & core_top__DOT__u_ex_stage__DOT__src2)
                                                   : 
                                                  (core_top__DOT__u_ex_stage__DOT__src1 
                                                   | core_top__DOT__u_ex_stage__DOT__src2))
                                                  : 
                                                 ((1U 
                                                   & (IData)(core_top__DOT__alu_op))
                                                   ? 
                                                  (core_top__DOT__u_ex_stage__DOT__src1 
                                                   ^ core_top__DOT__u_ex_stage__DOT__src2)
                                                   : 
                                                  ((core_top__DOT__u_ex_stage__DOT__src1 
                                                    < core_top__DOT__u_ex_stage__DOT__src2)
                                                    ? 1U
                                                    : 0U)))
                                                 : 
                                                ((2U 
                                                  & (IData)(core_top__DOT__alu_op))
                                                  ? 
                                                 ((1U 
                                                   & (IData)(core_top__DOT__alu_op))
                                                   ? 
                                                  (VL_LTS_III(32, core_top__DOT__u_ex_stage__DOT__src1, core_top__DOT__u_ex_stage__DOT__src2)
                                                    ? 1U
                                                    : 0U)
                                                   : 
                                                  (core_top__DOT__u_ex_stage__DOT__src1 
                                                   << 
                                                   (0x1fU 
                                                    & core_top__DOT__u_ex_stage__DOT__src2)))
                                                  : 
                                                 ((1U 
                                                   & (IData)(core_top__DOT__alu_op))
                                                   ? 
                                                  (core_top__DOT__u_ex_stage__DOT__src1 
                                                   + core_top__DOT__u_ex_stage__DOT__src2)
                                                   : 
                                                  (core_top__DOT__u_ex_stage__DOT__src1 
                                                   - core_top__DOT__u_ex_stage__DOT__src2))))));
    if (vlSelfRef.core_top__DOT__mem_write) {
        if ((5U == (IData)(vlSelfRef.core_top__DOT__lsu_type))) {
            vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata 
                = core_top__DOT__rs2_rdata;
            vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wmask = 0xfU;
        } else if ((6U == (IData)(vlSelfRef.core_top__DOT__lsu_type))) {
            vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata 
                = ((core_top__DOT__rs2_rdata << 0x10U) 
                   | (0xffffU & core_top__DOT__rs2_rdata));
            vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wmask 
                = (0xfU & ((2U & vlSelfRef.core_top__DOT__alu_result)
                            ? 0xcU : 3U));
        } else {
            vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata 
                = ((core_top__DOT__rs2_rdata << 0x18U) 
                   | ((0xff0000U & (core_top__DOT__rs2_rdata 
                                    << 0x10U)) | ((0xff00U 
                                                   & (core_top__DOT__rs2_rdata 
                                                      << 8U)) 
                                                  | (0xffU 
                                                     & core_top__DOT__rs2_rdata))));
            vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wmask 
                = (0xfU & ((IData)(1U) << (3U & vlSelfRef.core_top__DOT__alu_result)));
        }
    } else {
        vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata = 0U;
        vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wmask = 0U;
    }
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected 
        = (0xffU & ((0U == (3U & vlSelfRef.core_top__DOT__alu_result))
                     ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem
                    [(0xffffU & (vlSelfRef.core_top__DOT__alu_result 
                                 >> 2U))] : ((1U == 
                                              (3U & vlSelfRef.core_top__DOT__alu_result))
                                              ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem
                                                 [(0xffffU 
                                                   & (vlSelfRef.core_top__DOT__alu_result 
                                                      >> 2U))] 
                                                 >> 8U)
                                              : ((2U 
                                                  == 
                                                  (3U 
                                                   & vlSelfRef.core_top__DOT__alu_result))
                                                  ? 
                                                 (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem
                                                  [
                                                  (0xffffU 
                                                   & (vlSelfRef.core_top__DOT__alu_result 
                                                      >> 2U))] 
                                                  >> 0x10U)
                                                  : 
                                                 (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem
                                                  [
                                                  (0xffffU 
                                                   & (vlSelfRef.core_top__DOT__alu_result 
                                                      >> 2U))] 
                                                  >> 0x18U)))));
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected 
        = (0xffffU & ((2U & vlSelfRef.core_top__DOT__alu_result)
                       ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem
                          [(0xffffU & (vlSelfRef.core_top__DOT__alu_result 
                                       >> 2U))] >> 0x10U)
                       : vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem
                      [(0xffffU & (vlSelfRef.core_top__DOT__alu_result 
                                   >> 2U))]));
    vlSelfRef.core_top__DOT__u_if_stage__DOT__pc_next 
        = ((0U == (IData)(core_top__DOT__pc_sel)) ? 
           ((IData)(4U) + vlSelfRef.core_top__DOT__pc)
            : ((1U == (IData)(core_top__DOT__pc_sel))
                ? __VdfgRegularize_h3c5bdd75_0_0 : 
               ((2U == (IData)(core_top__DOT__pc_sel))
                 ? (0xfffffffeU & vlSelfRef.core_top__DOT__alu_result)
                 : ((3U == (IData)(core_top__DOT__pc_sel))
                     ? (((4U & (IData)(core_top__DOT__br_type))
                          ? ((~ ((IData)(core_top__DOT__br_type) 
                                 >> 1U)) & ((1U & (IData)(core_top__DOT__br_type))
                                             ? (core_top__DOT__rs1_rdata 
                                                >= core_top__DOT__rs2_rdata)
                                             : (core_top__DOT__rs1_rdata 
                                                < core_top__DOT__rs2_rdata)))
                          : ((2U & (IData)(core_top__DOT__br_type))
                              ? ((1U & (IData)(core_top__DOT__br_type))
                                  ? VL_GTES_III(32, core_top__DOT__rs1_rdata, core_top__DOT__rs2_rdata)
                                  : VL_LTS_III(32, core_top__DOT__rs1_rdata, core_top__DOT__rs2_rdata))
                              : ((1U & (IData)(core_top__DOT__br_type))
                                  ? (core_top__DOT__rs1_rdata 
                                     != core_top__DOT__rs2_rdata)
                                  : (core_top__DOT__rs1_rdata 
                                     == core_top__DOT__rs2_rdata))))
                         ? __VdfgRegularize_h3c5bdd75_0_0
                         : ((IData)(4U) + vlSelfRef.core_top__DOT__pc))
                     : ((IData)(4U) + vlSelfRef.core_top__DOT__pc)))));
}

VL_ATTR_COLD void Vcore_top___024root___eval_triggers__stl(Vcore_top___024root* vlSelf);

VL_ATTR_COLD bool Vcore_top___024root___eval_phase__stl(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_phase__stl\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vcore_top___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelfRef.__VstlTriggered.any();
    if (__VstlExecute) {
        Vcore_top___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcore_top___024root___dump_triggers__act(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___dump_triggers__act\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VactTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcore_top___024root___dump_triggers__nba(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___dump_triggers__nba\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VnbaTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vcore_top___024root___ctor_var_reset(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___ctor_var_reset\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->name());
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18209466448985614591ull);
    vlSelf->debug_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4804012649788666537ull);
    vlSelf->core_top__DOT__rd_addr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 9222939784570157495ull);
    vlSelf->core_top__DOT__mem_read = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3267123640257130700ull);
    vlSelf->core_top__DOT__mem_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6654782002655877227ull);
    vlSelf->core_top__DOT__reg_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13173339675713999511ull);
    vlSelf->core_top__DOT__pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4262964085017532224ull);
    vlSelf->core_top__DOT__lsu_type = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 10055842887531614569ull);
    vlSelf->core_top__DOT__reg_wdata_src = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 12464435452399026180ull);
    vlSelf->core_top__DOT__alu_result = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6180818066597375051ull);
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->core_top__DOT__u_regfile__DOT__rf[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1373196643498699064ull);
    }
    vlSelf->core_top__DOT__u_if_stage__DOT__pc_next = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6969335522121334582ull);
    for (int __Vi0 = 0; __Vi0 < 65536; ++__Vi0) {
        vlSelf->core_top__DOT__u_if_stage__DOT__imem[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11290784894032362506ull);
    }
    for (int __Vi0 = 0; __Vi0 < 65536; ++__Vi0) {
        vlSelf->core_top__DOT__u_mem_stage__DOT__mem[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7803367757265537014ull);
    }
    vlSelf->core_top__DOT__u_mem_stage__DOT__mem_wmask = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 16035830778309949707ull);
    vlSelf->core_top__DOT__u_mem_stage__DOT__mem_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13701141684010482380ull);
    vlSelf->core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 13132455624507066660ull);
    vlSelf->core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 1308833202003551685ull);
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9526919608049418986ull);
}
