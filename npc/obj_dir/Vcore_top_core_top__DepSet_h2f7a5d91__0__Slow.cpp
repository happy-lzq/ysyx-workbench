// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcore_top.h for the primary calling header

#include "Vcore_top__pch.h"
#include "Vcore_top__Syms.h"
#include "Vcore_top_core_top.h"

VL_ATTR_COLD void Vcore_top_core_top___stl_sequent__TOP__core_top__0(Vcore_top_core_top* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+      Vcore_top_core_top___stl_sequent__TOP__core_top__0\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
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
    vlSelfRef.__PVT__rs1_addr = (0x1fU & (vlSelfRef.__PVT__u_if_stage__DOT__imem
                                          [(0xffffU 
                                            & (vlSelfRef.__PVT__pc 
                                               >> 2U))] 
                                          >> 0xfU));
    vlSelfRef.__PVT__rs2_addr = (0x1fU & (vlSelfRef.__PVT__u_if_stage__DOT__imem
                                          [(0xffffU 
                                            & (vlSelfRef.__PVT__pc 
                                               >> 2U))] 
                                          >> 0x14U));
    vlSelfRef.__PVT__u_id_stage__DOT__funct3 = (7U 
                                                & (vlSelfRef.__PVT__u_if_stage__DOT__imem
                                                   [
                                                   (0xffffU 
                                                    & (vlSelfRef.__PVT__pc 
                                                       >> 2U))] 
                                                   >> 0xcU));
    vlSelfRef.__PVT__rd_addr = (0x1fU & (vlSelfRef.__PVT__u_if_stage__DOT__imem
                                         [(0xffffU 
                                           & (vlSelfRef.__PVT__pc 
                                              >> 2U))] 
                                         >> 7U));
    __VdfgRegularize_h495687df_0_0 = (1U & (vlSelfRef.__PVT__u_if_stage__DOT__imem
                                            [(0xffffU 
                                              & (vlSelfRef.__PVT__pc 
                                                 >> 2U))] 
                                            >> 5U));
    __VdfgRegularize_h495687df_0_1 = (1U & (vlSelfRef.__PVT__u_if_stage__DOT__imem
                                            [(0xffffU 
                                              & (vlSelfRef.__PVT__pc 
                                                 >> 2U))] 
                                            >> 4U));
    vlSelfRef.__PVT__u_id_stage__DOT__funct7 = (vlSelfRef.__PVT__u_if_stage__DOT__imem
                                                [(0xffffU 
                                                  & (vlSelfRef.__PVT__pc 
                                                     >> 2U))] 
                                                >> 0x19U);
    __VdfgRegularize_h495687df_0_2 = (1U & (vlSelfRef.__PVT__u_if_stage__DOT__imem
                                            [(0xffffU 
                                              & (vlSelfRef.__PVT__pc 
                                                 >> 2U))] 
                                            >> 3U));
    __VdfgRegularize_h495687df_0_3 = (1U & (vlSelfRef.__PVT__u_if_stage__DOT__imem
                                            [(0xffffU 
                                              & (vlSelfRef.__PVT__pc 
                                                 >> 2U))] 
                                            >> 2U));
    vlSelfRef.__PVT__u_id_stage__DOT__opcode = (0x7fU 
                                                & vlSelfRef.__PVT__u_if_stage__DOT__imem
                                                [(0xffffU 
                                                  & (vlSelfRef.__PVT__pc 
                                                     >> 2U))]);
    vlSelfRef.u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0 
        = (vlSelfRef.__PVT__u_if_stage__DOT__imem[(0xffffU 
                                                   & (vlSelfRef.__PVT__pc 
                                                      >> 2U))] 
           >> 0x1fU);
    __VdfgRegularize_h495687df_0_4 = (1U & (vlSelfRef.__PVT__u_if_stage__DOT__imem
                                            [(0xffffU 
                                              & (vlSelfRef.__PVT__pc 
                                                 >> 2U))] 
                                            >> 1U));
    __VdfgRegularize_h495687df_0_5 = (1U & vlSelfRef.__PVT__u_if_stage__DOT__imem
                                      [(0xffffU & (vlSelfRef.__PVT__pc 
                                                   >> 2U))]);
    vlSelfRef.__PVT__rs1_rdata = ((0U == (IData)(vlSelfRef.__PVT__rs1_addr))
                                   ? 0U : vlSymsp->TOP__core_top__u_regfile.rf
                                  [vlSelfRef.__PVT__rs1_addr]);
    vlSelfRef.__PVT__rs2_rdata = ((0U == (IData)(vlSelfRef.__PVT__rs2_addr))
                                   ? 0U : vlSymsp->TOP__core_top__u_regfile.rf
                                  [vlSelfRef.__PVT__rs2_addr]);
    vlSelfRef.__PVT__mem_read = 0U;
    vlSelfRef.__PVT__reg_write = 0U;
    vlSelfRef.__PVT__reg_wdata_src = 0U;
    vlSelfRef.__PVT__pc_sel = 0U;
    vlSelfRef.__PVT__br_type = 0U;
    vlSelfRef.__PVT__mem_write = 0U;
    vlSelfRef.__PVT__lsu_type = 0U;
    vlSelfRef.__PVT__alu_op = 0U;
    vlSelfRef.__PVT__alu_src_a = 0U;
    if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                  >> 6U)))) {
        if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                      >> 5U)))) {
            if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                          >> 4U)))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                                  >> 2U)))) {
                        if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.__PVT__mem_read = 1U;
                            }
                        }
                    }
                }
            }
            if ((0x10U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((4U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                        if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.__PVT__alu_src_a = 1U;
                            }
                        }
                    }
                }
            }
        }
        if ((0x20U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
            if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                          >> 4U)))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                                  >> 2U)))) {
                        if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.__PVT__mem_write = 1U;
                                if ((2U == (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))) {
                                    vlSelfRef.__PVT__lsu_type = 5U;
                                } else if ((1U == (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))) {
                                    vlSelfRef.__PVT__lsu_type = 6U;
                                } else if ((0U == (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))) {
                                    vlSelfRef.__PVT__lsu_type = 7U;
                                }
                            }
                        }
                    }
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                             >> 4U)))) {
            if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                          >> 3U)))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                              >> 2U)))) {
                    if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                        if ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                            if ((4U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))) {
                                if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3) 
                                              >> 1U)))) {
                                    vlSelfRef.__PVT__lsu_type 
                                        = ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                            ? 3U : 4U);
                                }
                            } else if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))) {
                                if ((1U & (~ (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3)))) {
                                    vlSelfRef.__PVT__lsu_type = 0U;
                                }
                            } else {
                                vlSelfRef.__PVT__lsu_type 
                                    = ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                        ? 1U : 2U);
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.__PVT__alu_src_b = 0U;
    if ((0x40U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
        if ((0x20U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
            if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                          >> 4U)))) {
                if ((8U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                    if ((4U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                        if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.__PVT__reg_write = 1U;
                                vlSelfRef.__PVT__reg_wdata_src = 2U;
                                vlSelfRef.__PVT__pc_sel = 1U;
                            }
                        }
                    }
                } else if ((4U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                    if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                        if ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                            vlSelfRef.__PVT__reg_write = 1U;
                            vlSelfRef.__PVT__reg_wdata_src = 2U;
                            vlSelfRef.__PVT__pc_sel = 2U;
                        }
                    }
                } else if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                    if ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                        vlSelfRef.__PVT__pc_sel = 3U;
                    }
                }
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                                  >> 2U)))) {
                        if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.__PVT__br_type = 0U;
                                if ((4U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))) {
                                    vlSelfRef.__PVT__br_type 
                                        = ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                            ? ((1U 
                                                & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                                ? 5U
                                                : 4U)
                                            : ((1U 
                                                & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                                ? 3U
                                                : 2U));
                                } else if ((1U & (~ 
                                                  ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3) 
                                                   >> 1U)))) {
                                    vlSelfRef.__PVT__br_type 
                                        = ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                            ? 1U : 0U);
                                }
                            }
                        }
                    }
                    if ((4U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                        if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.__PVT__alu_op = 1U;
                                vlSelfRef.__PVT__alu_src_b = 1U;
                            }
                        }
                    }
                }
            }
        }
    } else {
        if ((0x20U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
            if ((0x10U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((4U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                        if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.__PVT__reg_write = 1U;
                                vlSelfRef.__PVT__alu_op = 0xaU;
                                vlSelfRef.__PVT__alu_src_b = 1U;
                            }
                        }
                    } else if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                        if ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                            vlSelfRef.__PVT__reg_write = 1U;
                            vlSelfRef.__PVT__alu_op 
                                = ((4U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                    ? ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                        ? ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                            ? 7U : 6U)
                                        : ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                            ? ((0x20U 
                                                & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct7))
                                                ? 9U
                                                : 8U)
                                            : 5U)) : 
                                   ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                     ? ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                         ? 4U : 3U)
                                     : ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                         ? 2U : ((0x20U 
                                                  & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct7))
                                                  ? 0U
                                                  : 1U))));
                        }
                    }
                }
            } else if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                                 >> 3U)))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                              >> 2U)))) {
                    if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                        if ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                            vlSelfRef.__PVT__alu_op = 1U;
                            vlSelfRef.__PVT__alu_src_b = 1U;
                        }
                    }
                }
            }
        } else if ((0x10U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
            if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                          >> 3U)))) {
                if ((4U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                    if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                        if ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                            vlSelfRef.__PVT__reg_write = 1U;
                            vlSelfRef.__PVT__alu_op = 1U;
                            vlSelfRef.__PVT__alu_src_b = 1U;
                        }
                    }
                } else if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                    if ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                        vlSelfRef.__PVT__reg_write = 1U;
                        vlSelfRef.__PVT__alu_op = (
                                                   (4U 
                                                    & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                                    ? 
                                                   ((2U 
                                                     & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                                     ? 
                                                    ((1U 
                                                      & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                                      ? 7U
                                                      : 6U)
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                                      ? 
                                                     ((0x20U 
                                                       & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct7))
                                                       ? 9U
                                                       : 8U)
                                                      : 5U))
                                                    : 
                                                   ((2U 
                                                     & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                                     ? 
                                                    ((1U 
                                                      & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                                      ? 4U
                                                      : 3U)
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct3))
                                                      ? 2U
                                                      : 1U)));
                        vlSelfRef.__PVT__alu_src_b = 1U;
                    }
                }
            }
        } else if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                             >> 3U)))) {
            if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                          >> 2U)))) {
                if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                    if ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                        vlSelfRef.__PVT__reg_write = 1U;
                        vlSelfRef.__PVT__alu_op = 1U;
                        vlSelfRef.__PVT__alu_src_b = 1U;
                    }
                }
            }
        }
        if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                      >> 5U)))) {
            if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                          >> 4U)))) {
                if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                              >> 3U)))) {
                    if ((1U & (~ ((IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode) 
                                  >> 2U)))) {
                        if ((2U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                            if ((1U & (IData)(vlSelfRef.__PVT__u_id_stage__DOT__opcode))) {
                                vlSelfRef.__PVT__reg_wdata_src = 1U;
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
                                              & vlSelfRef.__PVT__u_if_stage__DOT__imem
                                              [(0xffffU 
                                                & (vlSelfRef.__PVT__pc 
                                                   >> 2U))]);
            __VdfgRegularize_h495687df_0_6 = (((- (IData)((IData)(vlSelfRef.u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                               << 0xcU) 
                                              | (vlSelfRef.__PVT__u_if_stage__DOT__imem
                                                 [(0xffffU 
                                                   & (vlSelfRef.__PVT__pc 
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
    vlSelfRef.__PVT__u_ex_stage__DOT__src1 = ((IData)(vlSelfRef.__PVT__alu_src_a)
                                               ? vlSelfRef.__PVT__pc
                                               : vlSelfRef.__PVT__rs1_rdata);
    __VdfgRegularize_h495687df_0_7 = ((IData)(__VdfgRegularize_h495687df_0_2)
                                       ? 0U : ((IData)(__VdfgRegularize_h495687df_0_3)
                                                ? 0U
                                                : __VdfgRegularize_h495687df_0_6));
    vlSelfRef.__PVT__imm_jal = ((0x40U & vlSelfRef.__PVT__u_if_stage__DOT__imem
                                 [(0xffffU & (vlSelfRef.__PVT__pc 
                                              >> 2U))])
                                 ? ((IData)(__VdfgRegularize_h495687df_0_0)
                                     ? ((IData)(__VdfgRegularize_h495687df_0_1)
                                         ? __VdfgRegularize_h495687df_0_7
                                         : ((IData)(__VdfgRegularize_h495687df_0_2)
                                             ? ((IData)(__VdfgRegularize_h495687df_0_3)
                                                 ? 
                                                ((IData)(__VdfgRegularize_h495687df_0_4)
                                                  ? 
                                                 ((IData)(__VdfgRegularize_h495687df_0_5)
                                                   ? 
                                                  (((- (IData)((IData)(vlSelfRef.u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                                    << 0x14U) 
                                                   | (((0xff000U 
                                                        & vlSelfRef.__PVT__u_if_stage__DOT__imem
                                                        [
                                                        (0xffffU 
                                                         & (vlSelfRef.__PVT__pc 
                                                            >> 2U))]) 
                                                       | (0x800U 
                                                          & (vlSelfRef.__PVT__u_if_stage__DOT__imem
                                                             [
                                                             (0xffffU 
                                                              & (vlSelfRef.__PVT__pc 
                                                                 >> 2U))] 
                                                             >> 9U))) 
                                                      | (0x7feU 
                                                         & (vlSelfRef.__PVT__u_if_stage__DOT__imem
                                                            [
                                                            (0xffffU 
                                                             & (vlSelfRef.__PVT__pc 
                                                                >> 2U))] 
                                                            >> 0x14U))))
                                                   : 0U)
                                                  : 0U)
                                                 : 0U)
                                             : ((IData)(__VdfgRegularize_h495687df_0_3)
                                                 ? __VdfgRegularize_h495687df_0_6
                                                 : 
                                                ((IData)(__VdfgRegularize_h495687df_0_4)
                                                  ? 
                                                 ((IData)(__VdfgRegularize_h495687df_0_5)
                                                   ? 
                                                  (((- (IData)((IData)(vlSelfRef.u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                                    << 0xdU) 
                                                   | ((((IData)(vlSelfRef.u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0) 
                                                        << 0xcU) 
                                                       | (0x800U 
                                                          & (vlSelfRef.__PVT__u_if_stage__DOT__imem
                                                             [
                                                             (0xffffU 
                                                              & (vlSelfRef.__PVT__pc 
                                                                 >> 2U))] 
                                                             << 4U))) 
                                                      | ((0x7e0U 
                                                          & (vlSelfRef.__PVT__u_if_stage__DOT__imem
                                                             [
                                                             (0xffffU 
                                                              & (vlSelfRef.__PVT__pc 
                                                                 >> 2U))] 
                                                             >> 0x14U)) 
                                                         | (0x1eU 
                                                            & (vlSelfRef.__PVT__u_if_stage__DOT__imem
                                                               [
                                                               (0xffffU 
                                                                & (vlSelfRef.__PVT__pc 
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
                                                      (((- (IData)((IData)(vlSelfRef.u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                                        << 0xcU) 
                                                       | (((IData)(vlSelfRef.__PVT__u_id_stage__DOT__funct7) 
                                                           << 5U) 
                                                          | (IData)(vlSelfRef.__PVT__rd_addr)))
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
    vlSelfRef.__VdfgRegularize_h3877affd_0_0 = (vlSelfRef.__PVT__imm_jal 
                                                + vlSelfRef.__PVT__pc);
    vlSelfRef.__PVT__u_ex_stage__DOT__src2 = ((0U == (IData)(vlSelfRef.__PVT__alu_src_b))
                                               ? vlSelfRef.__PVT__rs2_rdata
                                               : ((1U 
                                                   == (IData)(vlSelfRef.__PVT__alu_src_b))
                                                   ? vlSelfRef.__PVT__imm_jal
                                                   : 4U));
    vlSelfRef.__PVT__jump_jalr = ((0x10U & (IData)(vlSelfRef.__PVT__alu_op))
                                   ? 0U : ((8U & (IData)(vlSelfRef.__PVT__alu_op))
                                            ? ((4U 
                                                & (IData)(vlSelfRef.__PVT__alu_op))
                                                ? 0U
                                                : (
                                                   (2U 
                                                    & (IData)(vlSelfRef.__PVT__alu_op))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__alu_op))
                                                     ? 0U
                                                     : vlSelfRef.__PVT__u_ex_stage__DOT__src2)
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__alu_op))
                                                     ? 
                                                    VL_SHIFTRS_III(32,32,5, vlSelfRef.__PVT__u_ex_stage__DOT__src1, 
                                                                   (0x1fU 
                                                                    & vlSelfRef.__PVT__u_ex_stage__DOT__src2))
                                                     : 
                                                    (vlSelfRef.__PVT__u_ex_stage__DOT__src1 
                                                     >> 
                                                     (0x1fU 
                                                      & vlSelfRef.__PVT__u_ex_stage__DOT__src2)))))
                                            : ((4U 
                                                & (IData)(vlSelfRef.__PVT__alu_op))
                                                ? (
                                                   (2U 
                                                    & (IData)(vlSelfRef.__PVT__alu_op))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__alu_op))
                                                     ? 
                                                    (vlSelfRef.__PVT__u_ex_stage__DOT__src1 
                                                     & vlSelfRef.__PVT__u_ex_stage__DOT__src2)
                                                     : 
                                                    (vlSelfRef.__PVT__u_ex_stage__DOT__src1 
                                                     | vlSelfRef.__PVT__u_ex_stage__DOT__src2))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__alu_op))
                                                     ? 
                                                    (vlSelfRef.__PVT__u_ex_stage__DOT__src1 
                                                     ^ vlSelfRef.__PVT__u_ex_stage__DOT__src2)
                                                     : 
                                                    ((vlSelfRef.__PVT__u_ex_stage__DOT__src1 
                                                      < vlSelfRef.__PVT__u_ex_stage__DOT__src2)
                                                      ? 1U
                                                      : 0U)))
                                                : (
                                                   (2U 
                                                    & (IData)(vlSelfRef.__PVT__alu_op))
                                                    ? 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__alu_op))
                                                     ? 
                                                    (VL_LTS_III(32, vlSelfRef.__PVT__u_ex_stage__DOT__src1, vlSelfRef.__PVT__u_ex_stage__DOT__src2)
                                                      ? 1U
                                                      : 0U)
                                                     : 
                                                    (vlSelfRef.__PVT__u_ex_stage__DOT__src1 
                                                     << 
                                                     (0x1fU 
                                                      & vlSelfRef.__PVT__u_ex_stage__DOT__src2)))
                                                    : 
                                                   ((1U 
                                                     & (IData)(vlSelfRef.__PVT__alu_op))
                                                     ? 
                                                    (vlSelfRef.__PVT__u_ex_stage__DOT__src1 
                                                     + vlSelfRef.__PVT__u_ex_stage__DOT__src2)
                                                     : 
                                                    (vlSelfRef.__PVT__u_ex_stage__DOT__src1 
                                                     - vlSelfRef.__PVT__u_ex_stage__DOT__src2))))));
    if (vlSelfRef.__PVT__mem_write) {
        if ((5U == (IData)(vlSelfRef.__PVT__lsu_type))) {
            vlSelfRef.__PVT__u_mem_stage__DOT__mem_wdata 
                = vlSelfRef.__PVT__rs2_rdata;
            vlSelfRef.__PVT__u_mem_stage__DOT__mem_wmask = 0xfU;
        } else if ((6U == (IData)(vlSelfRef.__PVT__lsu_type))) {
            vlSelfRef.__PVT__u_mem_stage__DOT__mem_wdata 
                = ((vlSelfRef.__PVT__rs2_rdata << 0x10U) 
                   | (0xffffU & vlSelfRef.__PVT__rs2_rdata));
            vlSelfRef.__PVT__u_mem_stage__DOT__mem_wmask 
                = (0xfU & ((2U & vlSelfRef.__PVT__jump_jalr)
                            ? 0xcU : 3U));
        } else {
            vlSelfRef.__PVT__u_mem_stage__DOT__mem_wdata 
                = ((vlSelfRef.__PVT__rs2_rdata << 0x18U) 
                   | ((0xff0000U & (vlSelfRef.__PVT__rs2_rdata 
                                    << 0x10U)) | ((0xff00U 
                                                   & (vlSelfRef.__PVT__rs2_rdata 
                                                      << 8U)) 
                                                  | (0xffU 
                                                     & vlSelfRef.__PVT__rs2_rdata))));
            vlSelfRef.__PVT__u_mem_stage__DOT__mem_wmask 
                = (0xfU & ((IData)(1U) << (3U & vlSelfRef.__PVT__jump_jalr)));
        }
    } else {
        vlSelfRef.__PVT__u_mem_stage__DOT__mem_wdata = 0U;
        vlSelfRef.__PVT__u_mem_stage__DOT__mem_wmask = 0U;
    }
    vlSelfRef.__PVT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected 
        = (0xffU & ((0U == (3U & vlSelfRef.__PVT__jump_jalr))
                     ? vlSelfRef.__PVT__u_mem_stage__DOT__mem
                    [(0xffffU & (vlSelfRef.__PVT__jump_jalr 
                                 >> 2U))] : ((1U == 
                                              (3U & vlSelfRef.__PVT__jump_jalr))
                                              ? (vlSelfRef.__PVT__u_mem_stage__DOT__mem
                                                 [(0xffffU 
                                                   & (vlSelfRef.__PVT__jump_jalr 
                                                      >> 2U))] 
                                                 >> 8U)
                                              : ((2U 
                                                  == 
                                                  (3U 
                                                   & vlSelfRef.__PVT__jump_jalr))
                                                  ? 
                                                 (vlSelfRef.__PVT__u_mem_stage__DOT__mem
                                                  [
                                                  (0xffffU 
                                                   & (vlSelfRef.__PVT__jump_jalr 
                                                      >> 2U))] 
                                                  >> 0x10U)
                                                  : 
                                                 (vlSelfRef.__PVT__u_mem_stage__DOT__mem
                                                  [
                                                  (0xffffU 
                                                   & (vlSelfRef.__PVT__jump_jalr 
                                                      >> 2U))] 
                                                  >> 0x18U)))));
    vlSelfRef.__PVT__u_mem_stage__DOT__lsu_pic__DOT__half_selected 
        = (0xffffU & ((2U & vlSelfRef.__PVT__jump_jalr)
                       ? (vlSelfRef.__PVT__u_mem_stage__DOT__mem
                          [(0xffffU & (vlSelfRef.__PVT__jump_jalr 
                                       >> 2U))] >> 0x10U)
                       : vlSelfRef.__PVT__u_mem_stage__DOT__mem
                      [(0xffffU & (vlSelfRef.__PVT__jump_jalr 
                                   >> 2U))]));
    vlSelfRef.__PVT__u_if_stage__DOT__pc_next = ((0U 
                                                  == (IData)(vlSelfRef.__PVT__pc_sel))
                                                  ? 
                                                 ((IData)(4U) 
                                                  + vlSelfRef.__PVT__pc)
                                                  : 
                                                 ((1U 
                                                   == (IData)(vlSelfRef.__PVT__pc_sel))
                                                   ? vlSelfRef.__VdfgRegularize_h3877affd_0_0
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.__PVT__pc_sel))
                                                    ? 
                                                   (0xfffffffeU 
                                                    & vlSelfRef.__PVT__jump_jalr)
                                                    : 
                                                   ((3U 
                                                     == (IData)(vlSelfRef.__PVT__pc_sel))
                                                     ? 
                                                    (((4U 
                                                       & (IData)(vlSelfRef.__PVT__br_type))
                                                       ? 
                                                      ((~ 
                                                        ((IData)(vlSelfRef.__PVT__br_type) 
                                                         >> 1U)) 
                                                       & ((1U 
                                                           & (IData)(vlSelfRef.__PVT__br_type))
                                                           ? 
                                                          (vlSelfRef.__PVT__rs1_rdata 
                                                           >= vlSelfRef.__PVT__rs2_rdata)
                                                           : 
                                                          (vlSelfRef.__PVT__rs1_rdata 
                                                           < vlSelfRef.__PVT__rs2_rdata)))
                                                       : 
                                                      ((2U 
                                                        & (IData)(vlSelfRef.__PVT__br_type))
                                                        ? 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__PVT__br_type))
                                                         ? 
                                                        VL_GTES_III(32, vlSelfRef.__PVT__rs1_rdata, vlSelfRef.__PVT__rs2_rdata)
                                                         : 
                                                        VL_LTS_III(32, vlSelfRef.__PVT__rs1_rdata, vlSelfRef.__PVT__rs2_rdata))
                                                        : 
                                                       ((1U 
                                                         & (IData)(vlSelfRef.__PVT__br_type))
                                                         ? 
                                                        (vlSelfRef.__PVT__rs1_rdata 
                                                         != vlSelfRef.__PVT__rs2_rdata)
                                                         : 
                                                        (vlSelfRef.__PVT__rs1_rdata 
                                                         == vlSelfRef.__PVT__rs2_rdata))))
                                                      ? vlSelfRef.__VdfgRegularize_h3877affd_0_0
                                                      : 
                                                     ((IData)(4U) 
                                                      + vlSelfRef.__PVT__pc))
                                                     : 
                                                    ((IData)(4U) 
                                                     + vlSelfRef.__PVT__pc)))));
}
