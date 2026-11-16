#include <cycle_event.h>
#include <dpi_export.h>

static CycleEvent current_event{};
void cycle_event_clear(){
    current_event= {};
}

CycleEvent cycle_event_get(){
    return current_event;
}

// 基于DPI-C 实现函数功能本身
extern "C" void dpi_wb_event(
    int valid, 
    int pc, 
    int inst, 
    int irq_take, 
    int interrupt_cause,
    int wb_is_mmio
){
    current_event.mem_wb_valid      = (valid !=0);
    current_event.wb_pc             = static_cast<uint32_t>(pc);
    current_event.wb_inst           = static_cast<uint32_t>(inst);
    current_event.irq_taken         = (irq_take !=0);
    current_event.interrupt_cause   = static_cast<uint32_t>(interrupt_cause);
    current_event.wb_is_mmio        = (wb_is_mmio !=0);
}

extern "C" void dpi_mmio_read_event(
    int valid,
    int pc,
    int addr,
    int data
){
    current_event.mmio_read_accepted    = (valid !=0);
    current_event.mmio_read_pc          = static_cast<uint32_t>(pc);
    current_event.mmio_read_addr        = static_cast<uint32_t>(addr);
    current_event.mmio_read_data        = static_cast<uint32_t>(data);
}   