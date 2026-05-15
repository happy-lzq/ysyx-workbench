// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcore_top.h for the primary calling header

#include "Vcore_top__pch.h"
#include "Vcore_top_regfile.h"

VL_ATTR_COLD void Vcore_top_regfile___ctor_var_reset(Vcore_top_regfile* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vcore_top_regfile___ctor_var_reset\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->name());
    vlSelf->__PVT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->__PVT__rs1_addr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 7583249219640028756ull);
    vlSelf->__PVT__rs2_addr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 16827201314045723821ull);
    vlSelf->__PVT__rd_addr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 7950012703377089919ull);
    vlSelf->__PVT__rd_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10314548674056696702ull);
    vlSelf->__PVT__reg_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12827810274228629793ull);
    vlSelf->__PVT__rs1_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11750827288182224484ull);
    vlSelf->__PVT__rs2_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17559880426433436776ull);
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->rf[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17363488919829229760ull);
    }
}
