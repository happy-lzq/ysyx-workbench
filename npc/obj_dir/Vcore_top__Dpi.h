// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Prototypes for DPI import and export functions.
//
// Verilator includes this file in all generated .cpp files that use DPI functions.
// Manually include this file where DPI .c import functions are declared to ensure
// the C functions match the expectations of the DPI imports.

#ifndef VERILATED_VCORE_TOP__DPI_H_
#define VERILATED_VCORE_TOP__DPI_H_  // guard

#include "svdpi.h"

#ifdef __cplusplus
extern "C" {
#endif


    // DPI IMPORTS
    // DPI import at /home/luo/ysyx/ysyx-workbench/npc/vsrc/dpi_imports.vh:6:30
    extern int dpi_mem_read(int addr, int is_load);
    // DPI import at /home/luo/ysyx/ysyx-workbench/npc/vsrc/dpi_imports.vh:7:30
    extern void dpi_mem_write(int addr, int data, int wmask);

#ifdef __cplusplus
}
#endif

#endif  // guard
