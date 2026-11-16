#include <difftest.h>
#include <sdb.h>
#include <trace.h>
#include <interrupt.h>
#include <device.h>
#include <memory.h>
#include <SDL2/SDL.h>
#include <cycle_event.h>
#include <mmio.h>


Vcore_top *top = NULL;
VerilatedVcdC* tfp = NULL;
NPC_state npc_s, ref_s;

int idx =0;
uint64_t cycle = 0;

void single_cycle(){
    // 1. 清除上一拍保存的提交事件
    cycle_event_clear();
    
    // 2. 本拍外部中断请求监测
    interrupt_check();
    
    // 3. 上升沿驱动
    top->clk = 1;
    top->eval();

    // 4. 事件获取
    CycleEvent event = cycle_event_get();
    if (event.mmio_read_accepted) {
        mmio_read_accept(event.mmio_read_addr);
    }
    // 5. 撤销外部中断输入
    top->interrupt_valid = 0;
    top->interrupt_cause = 0;

    // 6. 上升沿之后检查ebreak 
    halt_check();
    
    // 7. difftest 
    #ifdef CONFIG_DIFFTEST
        if (diff_so_file && (event.mem_wb_valid || event.irq_taken)){
            difftest_step(
                    top, 
                    idx, 
                    event.wb_pc, 
                    event.mem_wb_valid,
                    event.irq_taken,
                    event.interrupt_cause,
                    event.wb_is_mmio,
                    event.wb_inst);
            idx++;
        }
    #endif

    // 8. watchpoint
    #ifdef CONFIG_WATCHPOINT
        if (event.mem_wb_valid &&
            npc_sim_state.state == NPC_RUNNING &&
            check_watchpoint(&used_list) > 0) {
            npc_sim_state.state = NPC_STOP;
        }
    #endif

    // 8. itrace
    #ifdef CONFIG_ITRACE
        if (event.mem_wb_valid) {
        itrace_log(event.wb_pc, event.wb_inst);
    }
    #endif


    // 9. npc 状态检查
    npc_state_check();

    if (tfp) {
        tfp->dump(sim_time += 5);
    }

    // 10. 驱动下降沿
    top->clk = 0;
    top->eval();

    if (tfp) {
        tfp->dump(sim_time += 5);
    }
    cycle++;
}

/*   单周期仿真-difftest
void single_cycle(){
    interrupt_check();                           // 每个周期前执行检查中断
    uint32_t this_pc = npc_pc(top, 0, READ);     // 保存：本周期的 PC
    uint32_t this_inst = top->instr;             // 保存：本周期的指令   
    bool is_trap = top->interrupt_valid;         // 保存：本周期是否响应中断（kill 指令）
    
    top->clk = 1; top->eval();
    top->interrupt_valid = 0;
    top->interrupt_cause = 0;
    halt_check();                               // 每个周期后执行检查 halt
    bool post_intr_pending = !is_trap && (isa_query_intr() != INTR_EMPTY);
    #ifdef CONFIG_DIFFTEST 
    if (diff_so_file){
        bool mmio_accessed = pmem_mmio_accessed();
        // 外部事件（MMIO / 异步中断）：NEMU 无法复现 → 同步代替对比
        if (mmio_accessed || difftest_sync_needed || post_intr_pending){   
            if (difftest_sync_needed && !mmio_accessed && !is_trap && !post_intr_pending) {
                ref_difftest_exec(1);
            }
            npc_state_data(top);
            ref_difftest_regcpy(&npc_s,DIFFTEST_TO_REF);
            difftest_sync_needed = false;
        } else{
            difftest_step(top, cycle, this_pc);
        }
    }
    #endif
    
    #ifdef CONFIG_WATCHPOINT
        if (npc_sim_state.state == NPC_RUNNING && check_watchpoint(&used_list) > 0) {
            npc_sim_state.state = NPC_STOP;
        }
    #endif

    #ifdef CONFIG_ITRACE
        itrace_log(this_pc, this_inst);
        
    #endif
    npc_state_check();
    
    if (tfp) tfp->dump(sim_time+=5);
    top->clk = 0; top->eval();  
    if (tfp) tfp->dump(sim_time+=5);
}
*/
void npc_exec(){
    int vga_skip = 0;
    while (npc_sim_state.state != NPC_QUIT) {
        switch (npc_sim_state.state) {
        case NPC_RUNNING:
            single_cycle();
            #ifdef CONFIG_HAS_VGA
            if (++vga_skip >= 1000) {
                vga_update_screen();
                vga_skip = 0;
            }
            #endif
            #ifdef CONFIG_HAS_KEYBOARD
            {
              SDL_Event event;
              while (SDL_PollEvent(&event)) {
                switch (event.type) {
                  case SDL_QUIT:
                    npc_sim_state.state = NPC_QUIT;
                    break;
                  case SDL_KEYDOWN:
                  case SDL_KEYUP: {
                    uint8_t k = event.key.keysym.scancode;
                    bool is_keydown = (event.key.type == SDL_KEYDOWN);
                    send_key(k, is_keydown);
                    break;
                  }
                  default: break;
                }
              }
            }
            #endif
            break;
        case NPC_STOP:
            sdb_mainloop();  
            break;
        case NPC_END:
        case NPC_ABORT:
            return;
        }
    }
}

int main(int argc, char* argv[]){
    Verilated::commandArgs(argc, argv);
    top = new Vcore_top;

    #ifdef CONFIG_WAVE
        Verilated::traceEverOn(true);
        tfp = new VerilatedVcdC;
        top->trace(tfp, 99);
        tfp->open("build/wave.vcd");
    #endif

    monitor_init(argc,argv);
    npc_exec();

    if (tfp) {
        tfp->close();
        delete tfp;
    }
    delete top ;
    // 根据真实状态返回退出码
    if (npc_sim_state.state == NPC_END && npc_sim_state.halt_ret == 0)
        return 0;   // 真正跑完且通过
    else
        return 1;   // 非正常退出
}

/*

NPC_END (ebreak):
  halt_ret = x10 (a0)    → "程序的退出码"
  意义: 0=测试通过, 非0=测试失败

NPC_ABORT (difftest mismatch):
  halt_ret = 出错的寄存器号  → "哪个寄存器不匹配"
  意义: 告诉 npc_state_check 打印详细的错误信息

NPC_ABORT (断言/非法指令):
  halt_ret = -1           → "非正常终止"
  意义: 标记异常
*/
