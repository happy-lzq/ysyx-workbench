// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Implementation of DPI export functions.
//
#include "Vcore_top.h"
#include "Vcore_top__Syms.h"
#include "verilated_dpi.h"


int Vcore_top::get_mstatus() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcore_top___024root::get_mstatus\n"); );
    // Init
    IData/*31:0*/ get_mstatus__Vfuncrtn__Vcvt;
    get_mstatus__Vfuncrtn__Vcvt = 0;
    // Body
    static int __Vfuncnum = -1;
    if (VL_UNLIKELY(__Vfuncnum == -1)) __Vfuncnum = Verilated::exportFuncNum("get_mstatus");
    const VerilatedScope* const __Vscopep = Verilated::dpiScope();
    Vcore_top__Vcb_get_mstatus_t __Vcb = (Vcore_top__Vcb_get_mstatus_t)(VerilatedScope::exportFind(__Vscopep, __Vfuncnum));
    (*__Vcb)((Vcore_top__Syms*)(__Vscopep->symsp()), get_mstatus__Vfuncrtn__Vcvt);
    int get_mstatus__Vfuncrtn;
    get_mstatus__Vfuncrtn = get_mstatus__Vfuncrtn__Vcvt;
    return get_mstatus__Vfuncrtn;
}
