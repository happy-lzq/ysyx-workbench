// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcore_top.h for the primary calling header

#include "Vcore_top__pch.h"
#include "Vcore_top__Syms.h"
#include "Vcore_top___024unit.h"

extern "C" int dpi_mmio_read(int addr);

VL_INLINE_OPT void Vcore_top___024unit____Vdpiimwrap_dpi_mmio_read_TOP____024unit(IData/*31:0*/ addr, IData/*31:0*/ &dpi_mmio_read__Vfuncrtn) {
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vcore_top___024unit____Vdpiimwrap_dpi_mmio_read_TOP____024unit\n"); );
    // Body
    int addr__Vcvt;
    addr__Vcvt = addr;
    int dpi_mmio_read__Vfuncrtn__Vcvt;
    dpi_mmio_read__Vfuncrtn__Vcvt = dpi_mmio_read(addr__Vcvt);
    dpi_mmio_read__Vfuncrtn = dpi_mmio_read__Vfuncrtn__Vcvt;
}
