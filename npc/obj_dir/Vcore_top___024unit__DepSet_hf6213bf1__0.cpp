// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcore_top.h for the primary calling header

#include "Vcore_top__pch.h"
#include "Vcore_top__Syms.h"
#include "Vcore_top___024unit.h"

extern "C" int dpi_mem_read(int addr);

VL_INLINE_OPT void Vcore_top___024unit____Vdpiimwrap_dpi_mem_read_TOP____024unit(IData/*31:0*/ addr, IData/*31:0*/ &dpi_mem_read__Vfuncrtn) {
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vcore_top___024unit____Vdpiimwrap_dpi_mem_read_TOP____024unit\n"); );
    // Body
    int addr__Vcvt;
    addr__Vcvt = addr;
    int dpi_mem_read__Vfuncrtn__Vcvt;
    dpi_mem_read__Vfuncrtn__Vcvt = dpi_mem_read(addr__Vcvt);
    dpi_mem_read__Vfuncrtn = dpi_mem_read__Vfuncrtn__Vcvt;
}

extern "C" void dpi_mem_write(int addr, int data, int wmask);

VL_INLINE_OPT void Vcore_top___024unit____Vdpiimwrap_dpi_mem_write_TOP____024unit(IData/*31:0*/ addr, IData/*31:0*/ data, IData/*31:0*/ wmask) {
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vcore_top___024unit____Vdpiimwrap_dpi_mem_write_TOP____024unit\n"); );
    // Body
    int addr__Vcvt;
    addr__Vcvt = addr;
    int data__Vcvt;
    data__Vcvt = data;
    int wmask__Vcvt;
    wmask__Vcvt = wmask;
    dpi_mem_write(addr__Vcvt, data__Vcvt, wmask__Vcvt);
}
