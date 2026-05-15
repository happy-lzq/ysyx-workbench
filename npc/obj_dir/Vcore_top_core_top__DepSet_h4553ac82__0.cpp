// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcore_top.h for the primary calling header

#include "Vcore_top__pch.h"
#include "Vcore_top_core_top.h"

VL_INLINE_OPT void Vcore_top_core_top___nba_sequent__TOP__core_top__0(Vcore_top_core_top* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+      Vcore_top_core_top___nba_sequent__TOP__core_top__0\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VdlySet__u_mem_stage__DOT__mem__v0 = 0U;
    vlSelfRef.__VdlySet__u_mem_stage__DOT__mem__v1 = 0U;
    vlSelfRef.__VdlySet__u_mem_stage__DOT__mem__v2 = 0U;
    vlSelfRef.__VdlySet__u_mem_stage__DOT__mem__v3 = 0U;
    if (vlSelfRef.__PVT__mem_write) {
        if ((1U & (IData)(vlSelfRef.__PVT__u_mem_stage__DOT__mem_wmask))) {
            vlSelfRef.__VdlyVal__u_mem_stage__DOT__mem__v0 
                = (0xffU & vlSelfRef.__PVT__u_mem_stage__DOT__mem_wdata);
            vlSelfRef.__VdlyDim0__u_mem_stage__DOT__mem__v0 
                = (0xffffU & (vlSelfRef.__PVT__jump_jalr 
                              >> 2U));
            vlSelfRef.__VdlySet__u_mem_stage__DOT__mem__v0 = 1U;
        }
        if ((2U & (IData)(vlSelfRef.__PVT__u_mem_stage__DOT__mem_wmask))) {
            vlSelfRef.__VdlyVal__u_mem_stage__DOT__mem__v1 
                = (0xffU & (vlSelfRef.__PVT__u_mem_stage__DOT__mem_wdata 
                            >> 8U));
            vlSelfRef.__VdlyDim0__u_mem_stage__DOT__mem__v1 
                = (0xffffU & (vlSelfRef.__PVT__jump_jalr 
                              >> 2U));
            vlSelfRef.__VdlySet__u_mem_stage__DOT__mem__v1 = 1U;
        }
        if ((4U & (IData)(vlSelfRef.__PVT__u_mem_stage__DOT__mem_wmask))) {
            vlSelfRef.__VdlyVal__u_mem_stage__DOT__mem__v2 
                = (0xffU & (vlSelfRef.__PVT__u_mem_stage__DOT__mem_wdata 
                            >> 0x10U));
            vlSelfRef.__VdlyDim0__u_mem_stage__DOT__mem__v2 
                = (0xffffU & (vlSelfRef.__PVT__jump_jalr 
                              >> 2U));
            vlSelfRef.__VdlySet__u_mem_stage__DOT__mem__v2 = 1U;
        }
        if ((8U & (IData)(vlSelfRef.__PVT__u_mem_stage__DOT__mem_wmask))) {
            vlSelfRef.__VdlyVal__u_mem_stage__DOT__mem__v3 
                = (vlSelfRef.__PVT__u_mem_stage__DOT__mem_wdata 
                   >> 0x18U);
            vlSelfRef.__VdlyDim0__u_mem_stage__DOT__mem__v3 
                = (0xffffU & (vlSelfRef.__PVT__jump_jalr 
                              >> 2U));
            vlSelfRef.__VdlySet__u_mem_stage__DOT__mem__v3 = 1U;
        }
    }
}
