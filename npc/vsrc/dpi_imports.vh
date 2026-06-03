
`ifndef NPC_DPI_IMPORTS_VH
`define NPC_DPI_IMPORTS_VH

// ========== 统一内存读写 ==========
import "DPI-C" function int  dpi_mem_read(input int addr, input int is_load);
import "DPI-C" function void dpi_mem_write(input int addr, input int data, input int wmask);

// ========== 预留：定时器中断 ==========
// import "DPI-C" function int dpi_timer_irq();
`endif
