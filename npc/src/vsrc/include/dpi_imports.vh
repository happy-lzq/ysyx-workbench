
`ifndef NPC_DPI_IMPORTS_VH
`define NPC_DPI_IMPORTS_VH

// E backend: exactly once per accepted transaction, returning data or error.
import "DPI-C" function int dpi_mem_access(
    input longint unsigned addr, input int kind, input int size,
    input int device_attr, input int data, input int mask, output int rdata
);
import "DPI-C" function void dpi_mem_fault(input int pc, input int addr, input int kind);

// ========== 统一内存读写 ==========
import "DPI-C" function int  dpi_mem_read(
    input int addr, 
    input int is_load
    );
import "DPI-C" function void dpi_mem_write(
    input int addr, 
    input int data, 
    input int wmask
);

// ========== MMIO 设备访问查询 ======
import "DPI-C" function int dpi_is_mmio(
    input int addr 
);

// =========== wb 提交同一事件 ======
import "DPI-C" function void dpi_wb_event(
    input int mem_wb_valid, 
    input int pc, 
    input int inst, 
    input int irq_take,
    input int interrupt_cause,
    input int mem_wb_is_mmio
);

// load提交接受
import "DPI-C" function void dpi_mmio_read_event(
    input int valid,
    input int pc,
    input int addr,
    input int data
);

// ========== 预留：定时器中断 ==========
// import "DPI-C" function int dpi_timer_irq();
`endif
