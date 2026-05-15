// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcore_top.h for the primary calling header

#include "Vcore_top__pch.h"
#include "Vcore_top__Syms.h"
#include "Vcore_top_regfile.h"

VL_INLINE_OPT void Vcore_top_regfile___nba_sequent__TOP__core_top__u_regfile__0(Vcore_top_regfile* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vcore_top_regfile___nba_sequent__TOP__core_top__u_regfile__0\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ __VdlyVal__rf__v0;
    __VdlyVal__rf__v0 = 0;
    CData/*4:0*/ __VdlyDim0__rf__v0;
    __VdlyDim0__rf__v0 = 0;
    CData/*0:0*/ __VdlySet__rf__v0;
    __VdlySet__rf__v0 = 0;
    // Body
    __VdlySet__rf__v0 = 0U;
    if (((IData)(vlSymsp->TOP__core_top.__PVT__reg_write) 
         & (0U != (IData)(vlSymsp->TOP__core_top.__PVT__rd_addr)))) {
        __VdlyVal__rf__v0 = ((0U == (IData)(vlSymsp->TOP__core_top.__PVT__reg_wdata_src))
                              ? vlSymsp->TOP__core_top.__PVT__jump_jalr
                              : ((1U == (IData)(vlSymsp->TOP__core_top.__PVT__reg_wdata_src))
                                  ? ((IData)(vlSymsp->TOP__core_top.__PVT__mem_read)
                                      ? ((0U == (IData)(vlSymsp->TOP__core_top.__PVT__lsu_type))
                                          ? vlSymsp->TOP__core_top.__PVT__u_mem_stage__DOT__mem
                                         [(0xffffU 
                                           & (vlSymsp->TOP__core_top.__PVT__jump_jalr 
                                              >> 2U))]
                                          : ((1U == (IData)(vlSymsp->TOP__core_top.__PVT__lsu_type))
                                              ? (((- (IData)(
                                                             (1U 
                                                              & ((IData)(vlSymsp->TOP__core_top.__PVT__u_mem_stage__DOT__lsu_pic__DOT__half_selected) 
                                                                 >> 0xfU)))) 
                                                  << 0x10U) 
                                                 | (IData)(vlSymsp->TOP__core_top.__PVT__u_mem_stage__DOT__lsu_pic__DOT__half_selected))
                                              : ((2U 
                                                  == (IData)(vlSymsp->TOP__core_top.__PVT__lsu_type))
                                                  ? 
                                                 (((- (IData)(
                                                              (1U 
                                                               & ((IData)(vlSymsp->TOP__core_top.__PVT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected) 
                                                                  >> 7U)))) 
                                                   << 8U) 
                                                  | (IData)(vlSymsp->TOP__core_top.__PVT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected))
                                                  : 
                                                 ((3U 
                                                   == (IData)(vlSymsp->TOP__core_top.__PVT__lsu_type))
                                                   ? (IData)(vlSymsp->TOP__core_top.__PVT__u_mem_stage__DOT__lsu_pic__DOT__half_selected)
                                                   : 
                                                  ((4U 
                                                    == (IData)(vlSymsp->TOP__core_top.__PVT__lsu_type))
                                                    ? (IData)(vlSymsp->TOP__core_top.__PVT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected)
                                                    : 0U)))))
                                      : 0U) : ((IData)(4U) 
                                               + vlSymsp->TOP__core_top.__PVT__pc)));
        __VdlyDim0__rf__v0 = vlSymsp->TOP__core_top.__PVT__rd_addr;
        __VdlySet__rf__v0 = 1U;
    }
    if (__VdlySet__rf__v0) {
        vlSelfRef.rf[__VdlyDim0__rf__v0] = __VdlyVal__rf__v0;
    }
}
