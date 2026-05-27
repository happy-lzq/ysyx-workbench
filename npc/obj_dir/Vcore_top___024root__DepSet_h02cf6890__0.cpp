// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcore_top.h for the primary calling header

#include "Vcore_top__pch.h"
#include "Vcore_top___024root.h"

void Vcore_top___024root___eval_act(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_act\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

void Vcore_top___024root___nba_sequent__TOP__0(Vcore_top___024root* vlSelf);

void Vcore_top___024root___eval_nba(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_nba\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vcore_top___024root___nba_sequent__TOP__0(vlSelf);
        vlSelfRef.__Vm_traceActivity[1U] = 1U;
    }
}

VL_INLINE_OPT void Vcore_top___024root___nba_sequent__TOP__0(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___nba_sequent__TOP__0\n"); );
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
    QData/*63:0*/ __Vdly__core_top__DOT__u_csr__DOT__mcycle_64;
    __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 = 0;
    IData/*31:0*/ __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus;
    __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus = 0;
    IData/*31:0*/ __VdlyVal__core_top__DOT__u_regfile__DOT__rf__v0;
    __VdlyVal__core_top__DOT__u_regfile__DOT__rf__v0 = 0;
    CData/*4:0*/ __VdlyDim0__core_top__DOT__u_regfile__DOT__rf__v0;
    __VdlyDim0__core_top__DOT__u_regfile__DOT__rf__v0 = 0;
    CData/*0:0*/ __VdlySet__core_top__DOT__u_regfile__DOT__rf__v0;
    __VdlySet__core_top__DOT__u_regfile__DOT__rf__v0 = 0;
    CData/*7:0*/ __VdlyVal__core_top__DOT__u_mem_stage__DOT__dmem__v0;
    __VdlyVal__core_top__DOT__u_mem_stage__DOT__dmem__v0 = 0;
    IData/*17:0*/ __VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v0;
    __VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v0 = 0;
    CData/*0:0*/ __VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v0;
    __VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v0 = 0;
    CData/*7:0*/ __VdlyVal__core_top__DOT__u_mem_stage__DOT__dmem__v1;
    __VdlyVal__core_top__DOT__u_mem_stage__DOT__dmem__v1 = 0;
    IData/*17:0*/ __VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v1;
    __VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v1 = 0;
    CData/*0:0*/ __VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v1;
    __VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v1 = 0;
    CData/*7:0*/ __VdlyVal__core_top__DOT__u_mem_stage__DOT__dmem__v2;
    __VdlyVal__core_top__DOT__u_mem_stage__DOT__dmem__v2 = 0;
    IData/*17:0*/ __VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v2;
    __VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v2 = 0;
    CData/*0:0*/ __VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v2;
    __VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v2 = 0;
    CData/*7:0*/ __VdlyVal__core_top__DOT__u_mem_stage__DOT__dmem__v3;
    __VdlyVal__core_top__DOT__u_mem_stage__DOT__dmem__v3 = 0;
    IData/*17:0*/ __VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v3;
    __VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v3 = 0;
    CData/*0:0*/ __VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v3;
    __VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v3 = 0;
    // Body
    __VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v0 = 0U;
    __VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v1 = 0U;
    __VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v2 = 0U;
    __VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v3 = 0U;
    __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 = vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64;
    __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus 
        = vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus;
    __VdlySet__core_top__DOT__u_regfile__DOT__rf__v0 = 0U;
    if (((IData)(vlSelfRef.core_top__DOT__reg_write) 
         & (0U != (IData)(vlSelfRef.core_top__DOT__rd_addr)))) {
        __VdlyVal__core_top__DOT__u_regfile__DOT__rf__v0 
            = ((0U == (IData)(vlSelfRef.core_top__DOT__reg_wdata_src))
                ? vlSelfRef.core_top__DOT__jump_jalr
                : ((1U == (IData)(vlSelfRef.core_top__DOT__reg_wdata_src))
                    ? ((IData)(vlSelfRef.core_top__DOT__mem_read)
                        ? ((0U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                            ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                           [(0x3ffffU & (vlSelfRef.core_top__DOT__jump_jalr 
                                         >> 2U))] : 
                           ((1U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                             ? (((- (IData)((1U & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                                   >> 0xfU)))) 
                                 << 0x10U) | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))
                             : ((2U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                 ? (((- (IData)((1U 
                                                 & ((IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected) 
                                                    >> 7U)))) 
                                     << 8U) | (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected))
                                 : ((3U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                     ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected)
                                     : ((4U == (IData)(vlSelfRef.core_top__DOT__lsu_type))
                                         ? (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected)
                                         : 0U))))) : 0U)
                    : ((2U == (IData)(vlSelfRef.core_top__DOT__reg_wdata_src))
                        ? ((IData)(4U) + vlSelfRef.core_top__DOT__pc)
                        : vlSelfRef.core_top__DOT__csr_rdata)));
        __VdlyDim0__core_top__DOT__u_regfile__DOT__rf__v0 
            = vlSelfRef.core_top__DOT__rd_addr;
        __VdlySet__core_top__DOT__u_regfile__DOT__rf__v0 = 1U;
    }
    if (vlSelfRef.rst) {
        __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 = 0ULL;
        __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus = 0x1800U;
        vlSelfRef.halt = 0U;
        vlSelfRef.halt_ret = 0U;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause = 0U;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec = 0x80000000U;
        vlSelfRef.halt_pc = 0x80000000U;
        vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc = 0U;
        vlSelfRef.core_top__DOT__pc = 0x80000000U;
    } else {
        __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 
            = (1ULL + vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64);
        if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__trap_enter)))) {
            if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__mret)))) {
                if (vlSelfRef.core_top__DOT__csr_write) {
                    if ((0x300U != (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                        if ((0x305U != (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                            if ((0x341U != (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                                if ((0x342U != (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                                    if ((0xb00U == (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                                        __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 
                                            = ((0xffffffff00000000ULL 
                                                & __Vdly__core_top__DOT__u_csr__DOT__mcycle_64) 
                                               | (IData)((IData)(vlSelfRef.core_top__DOT__csr_wdata)));
                                    } else if ((0xb80U 
                                                == (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                                        __Vdly__core_top__DOT__u_csr__DOT__mcycle_64 
                                            = ((0xffffffffULL 
                                                & __Vdly__core_top__DOT__u_csr__DOT__mcycle_64) 
                                               | ((QData)((IData)(vlSelfRef.core_top__DOT__csr_wdata)) 
                                                  << 0x20U));
                                    }
                                }
                            }
                        }
                        if ((0x305U == (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                            vlSelfRef.core_top__DOT__u_csr__DOT__csr_mtvec 
                                = vlSelfRef.core_top__DOT__csr_wdata;
                        }
                    }
                }
            }
        }
        if (vlSelfRef.core_top__DOT__trap_enter) {
            __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus 
                = (0x1800U | ((0xffffe000U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus) 
                              | (((0x700U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus) 
                                  | (0x80U & (vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus 
                                              << 4U))) 
                                 | (0x77U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus))));
            vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause 
                = vlSelfRef.core_top__DOT__trap_code;
            vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc 
                = vlSelfRef.core_top__DOT__pc;
        } else {
            if (vlSelfRef.core_top__DOT__mret) {
                __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus 
                    = (0x80U | ((0xffffe000U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus) 
                                | ((0x700U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus) 
                                   | ((0x70U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus) 
                                      | ((8U & (vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus 
                                                >> 4U)) 
                                         | (7U & vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus))))));
            } else if (vlSelfRef.core_top__DOT__csr_write) {
                if ((0x300U == (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                    __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus 
                        = vlSelfRef.core_top__DOT__csr_wdata;
                }
            }
            if ((1U & (~ (IData)(vlSelfRef.core_top__DOT__mret)))) {
                if (vlSelfRef.core_top__DOT__csr_write) {
                    if ((0x300U != (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                        if ((0x305U != (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                            if ((0x341U != (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                                if ((0x342U == (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                                    vlSelfRef.core_top__DOT__u_csr__DOT__csr_mcause 
                                        = vlSelfRef.core_top__DOT__csr_wdata;
                                }
                            }
                            if ((0x341U == (IData)(vlSelfRef.core_top__DOT__csr_addr))) {
                                vlSelfRef.core_top__DOT__u_csr__DOT__csr_mepc 
                                    = vlSelfRef.core_top__DOT__csr_wdata;
                            }
                        }
                    }
                }
            }
        }
        if (vlSelfRef.core_top__DOT__is_ebreak) {
            vlSelfRef.halt = 1U;
            vlSelfRef.halt_ret = vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                [0xaU];
            vlSelfRef.halt_pc = vlSelfRef.core_top__DOT__pc;
        }
        vlSelfRef.core_top__DOT__pc = vlSelfRef.core_top__DOT__u_if_stage__DOT__pc_next;
    }
    vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64 
        = __Vdly__core_top__DOT__u_csr__DOT__mcycle_64;
    vlSelfRef.core_top__DOT__u_csr__DOT__csr_mstatus 
        = __Vdly__core_top__DOT__u_csr__DOT__csr_mstatus;
    if (vlSelfRef.core_top__DOT__mem_write) {
        if ((1U & (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wmask))) {
            __VdlyVal__core_top__DOT__u_mem_stage__DOT__dmem__v0 
                = (0xffU & vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata);
            __VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v0 
                = (0x3ffffU & (vlSelfRef.core_top__DOT__jump_jalr 
                               >> 2U));
            __VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v0 = 1U;
        }
        if ((2U & (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wmask))) {
            __VdlyVal__core_top__DOT__u_mem_stage__DOT__dmem__v1 
                = (0xffU & (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata 
                            >> 8U));
            __VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v1 
                = (0x3ffffU & (vlSelfRef.core_top__DOT__jump_jalr 
                               >> 2U));
            __VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v1 = 1U;
        }
        if ((4U & (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wmask))) {
            __VdlyVal__core_top__DOT__u_mem_stage__DOT__dmem__v2 
                = (0xffU & (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata 
                            >> 0x10U));
            __VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v2 
                = (0x3ffffU & (vlSelfRef.core_top__DOT__jump_jalr 
                               >> 2U));
            __VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v2 = 1U;
        }
        if ((8U & (IData)(vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wmask))) {
            __VdlyVal__core_top__DOT__u_mem_stage__DOT__dmem__v3 
                = (vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata 
                   >> 0x18U);
            __VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v3 
                = (0x3ffffU & (vlSelfRef.core_top__DOT__jump_jalr 
                               >> 2U));
            __VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v3 = 1U;
        }
    }
    if (__VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v0) {
        vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem[__VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v0] 
            = ((0xffffff00U & vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                [__VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v0]) 
               | (IData)(__VdlyVal__core_top__DOT__u_mem_stage__DOT__dmem__v0));
    }
    if (__VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v1) {
        vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem[__VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v1] 
            = ((0xffff00ffU & vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                [__VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v1]) 
               | ((IData)(__VdlyVal__core_top__DOT__u_mem_stage__DOT__dmem__v1) 
                  << 8U));
    }
    if (__VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v2) {
        vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem[__VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v2] 
            = ((0xff00ffffU & vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                [__VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v2]) 
               | ((IData)(__VdlyVal__core_top__DOT__u_mem_stage__DOT__dmem__v2) 
                  << 0x10U));
    }
    if (__VdlySet__core_top__DOT__u_mem_stage__DOT__dmem__v3) {
        vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem[__VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v3] 
            = ((0xffffffU & vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                [__VdlyDim0__core_top__DOT__u_mem_stage__DOT__dmem__v3]) 
               | ((IData)(__VdlyVal__core_top__DOT__u_mem_stage__DOT__dmem__v3) 
                  << 0x18U));
    }
    if (__VdlySet__core_top__DOT__u_regfile__DOT__rf__v0) {
        vlSelfRef.core_top__DOT__u_regfile__DOT__rf[__VdlyDim0__core_top__DOT__u_regfile__DOT__rf__v0] 
            = __VdlyVal__core_top__DOT__u_regfile__DOT__rf__v0;
    }
    vlSelfRef.instr = vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
        [(0x3ffffU & (vlSelfRef.core_top__DOT__pc >> 2U))];
    vlSelfRef.core_top__DOT__rs2_addr = (0x1fU & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                  [
                                                  (0x3ffffU 
                                                   & (vlSelfRef.core_top__DOT__pc 
                                                      >> 2U))] 
                                                  >> 0x14U));
    vlSelfRef.core_top__DOT__rd_addr = (0x1fU & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                 [(0x3ffffU 
                                                   & (vlSelfRef.core_top__DOT__pc 
                                                      >> 2U))] 
                                                 >> 7U));
    __VdfgRegularize_h495687df_0_0 = (1U & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                            [(0x3ffffU 
                                              & (vlSelfRef.core_top__DOT__pc 
                                                 >> 2U))] 
                                            >> 5U));
    __VdfgRegularize_h495687df_0_1 = (1U & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                            [(0x3ffffU 
                                              & (vlSelfRef.core_top__DOT__pc 
                                                 >> 2U))] 
                                            >> 4U));
    vlSelfRef.core_top__DOT__rs1_addr = (0x1fU & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                  [
                                                  (0x3ffffU 
                                                   & (vlSelfRef.core_top__DOT__pc 
                                                      >> 2U))] 
                                                  >> 0xfU));
    vlSelfRef.core_top__DOT__u_id_stage__DOT__funct7 
        = (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
           [(0x3ffffU & (vlSelfRef.core_top__DOT__pc 
                         >> 2U))] >> 0x19U);
    __VdfgRegularize_h495687df_0_2 = (1U & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                            [(0x3ffffU 
                                              & (vlSelfRef.core_top__DOT__pc 
                                                 >> 2U))] 
                                            >> 3U));
    __VdfgRegularize_h495687df_0_3 = (1U & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                            [(0x3ffffU 
                                              & (vlSelfRef.core_top__DOT__pc 
                                                 >> 2U))] 
                                            >> 2U));
    vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3 
        = (7U & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                 [(0x3ffffU & (vlSelfRef.core_top__DOT__pc 
                               >> 2U))] >> 0xcU));
    vlSelfRef.core_top__DOT__csr_addr = (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                         [(0x3ffffU 
                                           & (vlSelfRef.core_top__DOT__pc 
                                              >> 2U))] 
                                         >> 0x14U);
    vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0 
        = (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
           [(0x3ffffU & (vlSelfRef.core_top__DOT__pc 
                         >> 2U))] >> 0x1fU);
    vlSelfRef.core_top__DOT__u_id_stage__DOT__opcode 
        = (0x7fU & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
           [(0x3ffffU & (vlSelfRef.core_top__DOT__pc 
                         >> 2U))]);
    __VdfgRegularize_h495687df_0_4 = (1U & (vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                            [(0x3ffffU 
                                              & (vlSelfRef.core_top__DOT__pc 
                                                 >> 2U))] 
                                            >> 1U));
    __VdfgRegularize_h495687df_0_5 = (1U & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                      [(0x3ffffU & 
                                        (vlSelfRef.core_top__DOT__pc 
                                         >> 2U))]);
    vlSelfRef.core_top__DOT__rs2_rdata = ((0U == (IData)(vlSelfRef.core_top__DOT__rs2_addr))
                                           ? 0U : vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                                          [vlSelfRef.core_top__DOT__rs2_addr]);
    vlSelfRef.core_top__DOT__rs1_rdata = ((0U == (IData)(vlSelfRef.core_top__DOT__rs1_addr))
                                           ? 0U : vlSelfRef.core_top__DOT__u_regfile__DOT__rf
                                          [vlSelfRef.core_top__DOT__rs1_addr]);
    vlSelfRef.core_top__DOT__mem_read = 0U;
    vlSelfRef.core_top__DOT__reg_wdata_src = 0U;
    vlSelfRef.core_top__DOT__reg_write = 0U;
    vlSelfRef.core_top__DOT__csr_write = 0U;
    vlSelfRef.core_top__DOT__is_ebreak = 0U;
    vlSelfRef.core_top__DOT__trap_code = 0U;
    vlSelfRef.core_top__DOT__csr_op = 0U;
    vlSelfRef.core_top__DOT__mem_write = 0U;
    vlSelfRef.core_top__DOT__lsu_type = 0U;
    vlSelfRef.core_top__DOT__csr_imm = 0U;
    vlSelfRef.core_top__DOT__pc_sel = 0U;
    vlSelfRef.core_top__DOT__br_type = 0U;
    vlSelfRef.core_top__DOT__csr_read = 0U;
    vlSelfRef.core_top__DOT__trap_enter = 0U;
    vlSelfRef.core_top__DOT__mret = 0U;
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
                                vlSelfRef.core_top__DOT__mem_write = 1U;
                                if ((2U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))) {
                                    vlSelfRef.core_top__DOT__lsu_type = 5U;
                                } else if ((1U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))) {
                                    vlSelfRef.core_top__DOT__lsu_type = 6U;
                                } else if ((0U == (IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct3))) {
                                    vlSelfRef.core_top__DOT__lsu_type = 7U;
                                }
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
    if (__VdfgRegularize_h495687df_0_4) {
        if (__VdfgRegularize_h495687df_0_5) {
            __VdfgRegularize_h495687df_0_8 = (0xfffff000U 
                                              & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                              [(0x3ffffU 
                                                & (vlSelfRef.core_top__DOT__pc 
                                                   >> 2U))]);
            __VdfgRegularize_h495687df_0_6 = (((- (IData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                               << 0xcU) 
                                              | (IData)(vlSelfRef.core_top__DOT__csr_addr));
        } else {
            __VdfgRegularize_h495687df_0_8 = 0U;
            __VdfgRegularize_h495687df_0_6 = 0U;
        }
    } else {
        __VdfgRegularize_h495687df_0_8 = 0U;
        __VdfgRegularize_h495687df_0_6 = 0U;
    }
    vlSelfRef.core_top__DOT__u_ex_stage__DOT__csr_src 
        = ((IData)(vlSelfRef.core_top__DOT__csr_imm)
            ? vlSelfRef.core_top__DOT__rs1_rdata : (IData)(vlSelfRef.core_top__DOT__rs1_addr));
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
                                                    ((0xb00U 
                                                      == (IData)(vlSelfRef.core_top__DOT__csr_addr))
                                                      ? (IData)(vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64)
                                                      : 
                                                     ((0xb80U 
                                                       == (IData)(vlSelfRef.core_top__DOT__csr_addr))
                                                       ? (IData)(
                                                                 (vlSelfRef.core_top__DOT__u_csr__DOT__mcycle_64 
                                                                  >> 0x20U))
                                                       : 0U))))))
                                           : 0U);
    vlSelfRef.core_top__DOT__u_ex_stage__DOT__src1 
        = ((IData)(vlSelfRef.core_top__DOT__alu_src_a)
            ? vlSelfRef.core_top__DOT__pc : vlSelfRef.core_top__DOT__rs1_rdata);
    __VdfgRegularize_h495687df_0_7 = ((IData)(__VdfgRegularize_h495687df_0_2)
                                       ? 0U : ((IData)(__VdfgRegularize_h495687df_0_3)
                                                ? 0U
                                                : __VdfgRegularize_h495687df_0_6));
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
                                         [(0x3ffffU 
                                           & (vlSelfRef.core_top__DOT__pc 
                                              >> 2U))])
                                         ? ((IData)(__VdfgRegularize_h495687df_0_0)
                                             ? ((IData)(__VdfgRegularize_h495687df_0_1)
                                                 ? __VdfgRegularize_h495687df_0_7
                                                 : 
                                                ((IData)(__VdfgRegularize_h495687df_0_2)
                                                  ? 
                                                 ((IData)(__VdfgRegularize_h495687df_0_3)
                                                   ? 
                                                  ((IData)(__VdfgRegularize_h495687df_0_4)
                                                    ? 
                                                   ((IData)(__VdfgRegularize_h495687df_0_5)
                                                     ? 
                                                    (((- (IData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                                      << 0x14U) 
                                                     | (((0xff000U 
                                                          & vlSelfRef.core_top__DOT__u_if_stage__DOT__imem
                                                          [
                                                          (0x3ffffU 
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
                                                              >> 0x14U))))
                                                     : 0U)
                                                    : 0U)
                                                   : 0U)
                                                  : 
                                                 ((IData)(__VdfgRegularize_h495687df_0_3)
                                                   ? __VdfgRegularize_h495687df_0_6
                                                   : 
                                                  ((IData)(__VdfgRegularize_h495687df_0_4)
                                                    ? 
                                                   ((IData)(__VdfgRegularize_h495687df_0_5)
                                                     ? 
                                                    (((- (IData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                                      << 0xdU) 
                                                     | ((((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0) 
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
                                                                 >> 7U)))))
                                                     : 0U)
                                                    : 0U))))
                                             : 0U) : 
                                        ((IData)(__VdfgRegularize_h495687df_0_0)
                                          ? ((IData)(__VdfgRegularize_h495687df_0_1)
                                              ? ((IData)(__VdfgRegularize_h495687df_0_2)
                                                  ? 0U
                                                  : 
                                                 ((IData)(__VdfgRegularize_h495687df_0_3)
                                                   ? __VdfgRegularize_h495687df_0_8
                                                   : 0U))
                                              : ((IData)(__VdfgRegularize_h495687df_0_2)
                                                  ? 0U
                                                  : 
                                                 ((IData)(__VdfgRegularize_h495687df_0_3)
                                                   ? 0U
                                                   : 
                                                  ((IData)(__VdfgRegularize_h495687df_0_4)
                                                    ? 
                                                   ((IData)(__VdfgRegularize_h495687df_0_5)
                                                     ? 
                                                    (((- (IData)((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__u_imm_gen__DOT____VdfgRegularize_hebd20cbd_0_0))) 
                                                      << 0xcU) 
                                                     | (((IData)(vlSelfRef.core_top__DOT__u_id_stage__DOT__funct7) 
                                                         << 5U) 
                                                        | (IData)(vlSelfRef.core_top__DOT__rd_addr)))
                                                     : 0U)
                                                    : 0U))))
                                          : ((IData)(__VdfgRegularize_h495687df_0_1)
                                              ? ((IData)(__VdfgRegularize_h495687df_0_2)
                                                  ? 0U
                                                  : 
                                                 ((IData)(__VdfgRegularize_h495687df_0_3)
                                                   ? __VdfgRegularize_h495687df_0_8
                                                   : __VdfgRegularize_h495687df_0_6))
                                              : __VdfgRegularize_h495687df_0_7)));
    vlSelfRef.__VdfgRegularize_h3c5bdd75_0_0 = (vlSelfRef.core_top__DOT__imm_jal 
                                                + vlSelfRef.core_top__DOT__pc);
    vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2 
        = ((0U == (IData)(vlSelfRef.core_top__DOT__alu_src_b))
            ? vlSelfRef.core_top__DOT__rs2_rdata : 
           ((1U == (IData)(vlSelfRef.core_top__DOT__alu_src_b))
             ? vlSelfRef.core_top__DOT__imm_jal : 4U));
    vlSelfRef.core_top__DOT__jump_jalr = ((0x10U & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                           ? 0U : (
                                                   (8U 
                                                    & (IData)(vlSelfRef.core_top__DOT__alu_op))
                                                    ? 
                                                   ((4U 
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
                                                    : 
                                                   ((4U 
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
                                                       - vlSelfRef.core_top__DOT__u_ex_stage__DOT__src2))))));
    if (vlSelfRef.core_top__DOT__mem_write) {
        if ((5U == (IData)(vlSelfRef.core_top__DOT__lsu_type))) {
            vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata 
                = vlSelfRef.core_top__DOT__rs2_rdata;
            vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wmask = 0xfU;
        } else if ((6U == (IData)(vlSelfRef.core_top__DOT__lsu_type))) {
            vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata 
                = ((vlSelfRef.core_top__DOT__rs2_rdata 
                    << 0x10U) | (0xffffU & vlSelfRef.core_top__DOT__rs2_rdata));
            vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wmask 
                = (0xfU & ((2U & vlSelfRef.core_top__DOT__jump_jalr)
                            ? 0xcU : 3U));
        } else {
            vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata 
                = ((vlSelfRef.core_top__DOT__rs2_rdata 
                    << 0x18U) | ((0xff0000U & (vlSelfRef.core_top__DOT__rs2_rdata 
                                               << 0x10U)) 
                                 | ((0xff00U & (vlSelfRef.core_top__DOT__rs2_rdata 
                                                << 8U)) 
                                    | (0xffU & vlSelfRef.core_top__DOT__rs2_rdata))));
            vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wmask 
                = (0xfU & ((IData)(1U) << (3U & vlSelfRef.core_top__DOT__jump_jalr)));
        }
    } else {
        vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wdata = 0U;
        vlSelfRef.core_top__DOT__u_mem_stage__DOT__mem_wmask = 0U;
    }
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected 
        = (0xffU & ((0U == (3U & vlSelfRef.core_top__DOT__jump_jalr))
                     ? vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                    [(0x3ffffU & (vlSelfRef.core_top__DOT__jump_jalr 
                                  >> 2U))] : ((1U == 
                                               (3U 
                                                & vlSelfRef.core_top__DOT__jump_jalr))
                                               ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                                                  [
                                                  (0x3ffffU 
                                                   & (vlSelfRef.core_top__DOT__jump_jalr 
                                                      >> 2U))] 
                                                  >> 8U)
                                               : ((2U 
                                                   == 
                                                   (3U 
                                                    & vlSelfRef.core_top__DOT__jump_jalr))
                                                   ? 
                                                  (vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                                                   [
                                                   (0x3ffffU 
                                                    & (vlSelfRef.core_top__DOT__jump_jalr 
                                                       >> 2U))] 
                                                   >> 0x10U)
                                                   : 
                                                  (vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                                                   [
                                                   (0x3ffffU 
                                                    & (vlSelfRef.core_top__DOT__jump_jalr 
                                                       >> 2U))] 
                                                   >> 0x18U)))));
    vlSelfRef.core_top__DOT__u_mem_stage__DOT__lsu_pic__DOT__half_selected 
        = (0xffffU & ((2U & vlSelfRef.core_top__DOT__jump_jalr)
                       ? (vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                          [(0x3ffffU & (vlSelfRef.core_top__DOT__jump_jalr 
                                        >> 2U))] >> 0x10U)
                       : vlSelfRef.core_top__DOT__u_mem_stage__DOT__dmem
                      [(0x3ffffU & (vlSelfRef.core_top__DOT__jump_jalr 
                                    >> 2U))]));
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
                                          & vlSelfRef.core_top__DOT__jump_jalr)
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
}

void Vcore_top___024root___eval_triggers__act(Vcore_top___024root* vlSelf);

bool Vcore_top___024root___eval_phase__act(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_phase__act\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    VlTriggerVec<1> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vcore_top___024root___eval_triggers__act(vlSelf);
    __VactExecute = vlSelfRef.__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelfRef.__VactTriggered, vlSelfRef.__VnbaTriggered);
        vlSelfRef.__VnbaTriggered.thisOr(vlSelfRef.__VactTriggered);
        Vcore_top___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vcore_top___024root___eval_phase__nba(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_phase__nba\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelfRef.__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vcore_top___024root___eval_nba(vlSelf);
        vlSelfRef.__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcore_top___024root___dump_triggers__nba(Vcore_top___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vcore_top___024root___dump_triggers__act(Vcore_top___024root* vlSelf);
#endif  // VL_DEBUG

void Vcore_top___024root___eval(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY(((0x64U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vcore_top___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("vsrc/core_top.v", 1, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelfRef.__VactIterCount = 0U;
        vlSelfRef.__VactContinue = 1U;
        while (vlSelfRef.__VactContinue) {
            if (VL_UNLIKELY(((0x64U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vcore_top___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("vsrc/core_top.v", 1, "", "Active region did not converge.");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactContinue = 0U;
            if (Vcore_top___024root___eval_phase__act(vlSelf)) {
                vlSelfRef.__VactContinue = 1U;
            }
        }
        if (Vcore_top___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vcore_top___024root___eval_debug_assertions(Vcore_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root___eval_debug_assertions\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY(((vlSelfRef.rst & 0xfeU)))) {
        Verilated::overWidthError("rst");}
}
#endif  // VL_DEBUG
