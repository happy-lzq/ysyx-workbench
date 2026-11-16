#ifndef NPC_CYCLE_EVENT_H
#define NPC_CYCLE_EVENT_H
#include <cstdint>

struct CycleEvent{
    bool mem_wb_valid;
    uint32_t wb_pc;
    uint32_t wb_inst;
    bool irq_taken;
    uint32_t interrupt_cause;
    bool wb_is_mmio;

    // 本拍 MEM → WB 接受的 MMIO load。
    bool mmio_read_accepted;
    uint32_t mmio_read_pc;
    uint32_t mmio_read_addr;
    uint32_t mmio_read_data;
};

CycleEvent cycle_event_get();           // 获取本拍事件信号
void cycle_event_clear();               // 清除上一拍事件






#endif
