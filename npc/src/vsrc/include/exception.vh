`ifndef NPC_EXCEPTION_VH
`define NPC_EXCEPTION_VH

// RISC-V RV32 trap causes: Machine-Level ISA 1.13 / H extension 1.0.
// 参考：npc/notes/Reference Manual/exception.md。
// 所有常量均为 32 位；EXC_* 的中断位为 0，IRQ_* 包含 mcause[31]=1。
// 定义编号不表示当前核已实现对应模式、扩展或异常处理。
// cause=0 是有效异常编号，是否有异常必须另外检查 exc_valid。

// ==================== 1. 基础同步异常 ====================
`define EXC_INST_ADDR_MISALIGNED    32'd0   // 指令地址未对齐
`define EXC_INST_ACCESS_FAULT       32'd1   // 指令访问错误
`define EXC_ILLEGAL_INST            32'd2   // 非法指令
`define EXC_BREAKPOINT              32'd3   // 断点
`define EXC_LOAD_ADDR_MISALIGNED    32'd4   // Load 地址未对齐
`define EXC_LOAD_ACCESS_FAULT       32'd5   // Load 访问错误
`define EXC_STORE_ADDR_MISALIGNED   32'd6   // Store/AMO 地址未对齐
`define EXC_STORE_ACCESS_FAULT      32'd7   // Store/AMO 访问错误
`define EXC_ECALL_U                 32'd8   // U-mode ECALL；H 扩展下包含 VU
`define EXC_ECALL_S                 32'd9   // S-mode ECALL；H 扩展下为 HS
`define EXC_ECALL_M                 32'd11  // M-mode ECALL
`define EXC_INST_PAGE_FAULT         32'd12  // 指令缺页
`define EXC_LOAD_PAGE_FAULT         32'd13  // Load 缺页
`define EXC_STORE_PAGE_FAULT        32'd15  // Store/AMO 缺页
`define EXC_DOUBLE_TRAP             32'd16  // 双重 trap，需相关扩展
`define EXC_SOFTWARE_CHECK          32'd18  // 软件检查，需相关扩展
`define EXC_HARDWARE_ERROR          32'd19  // 硬件错误，需相关机制

// ==================== 2. H 扩展同步异常 ====================
// 未实现 H 扩展时，不应产生以下异常。
`define EXC_ECALL_VS                32'd10  // VS-mode ECALL
`define EXC_INST_GUEST_PAGE_FAULT   32'd20  // 指令 Guest-page fault
`define EXC_LOAD_GUEST_PAGE_FAULT   32'd21  // Load Guest-page fault
`define EXC_VIRTUAL_INST            32'd22  // 虚拟指令异常
`define EXC_STORE_GUEST_PAGE_FAULT  32'd23  // Store/AMO Guest-page fault

// ==================== 3. 基础中断 ====================
// 以下是完整 RV32 mcause 值，不是 mie/mip 位号或外设中断源 ID。
`define IRQ_S_SOFT                 32'h8000_0001  // S-mode 软件中断
`define IRQ_M_SOFT                 32'h8000_0003  // M-mode 软件中断
`define IRQ_S_TIMER                32'h8000_0005  // S-mode 定时器中断
`define IRQ_M_TIMER                32'h8000_0007  // M-mode 定时器中断
`define IRQ_S_EXT                  32'h8000_0009  // S-mode 外部中断
`define IRQ_M_EXT                  32'h8000_000b  // M-mode 外部中断
`define IRQ_COUNTER_OVERFLOW       32'h8000_000d  // 计数器溢出，需相关扩展

// ==================== 4. H 扩展中断 ====================
// 以下是 M/HS 级观察的编号，委托到 VS 模式时部分编号会转换。
`define IRQ_VS_SOFT                32'h8000_0002  // VS-mode 软件中断
`define IRQ_VS_TIMER               32'h8000_0006  // VS-mode 定时器中断
`define IRQ_VS_EXT                 32'h8000_000a  // VS-mode 外部中断
`define IRQ_S_GUEST_EXT            32'h8000_000c  // S-mode Guest 外部中断

// 保留编号、自定义异常范围和平台中断编号不在此分配具体含义。

`endif

/*
 ================= 当前五级流水线：精确同步异常完善范围 =================
 当前目标为 M-only、无 C/无 MMU 的已有指令支持集，优先收敛 cause=0~7、11。
 以下是实现路线，不表示这些异常均已完成；具体通过范围以验收笔记为准。

 已有闭环：
   cause=2：非法编码的指定场景已贯通 ID 检测、载荷传递、WB trap、
            软件 handler/MRET 和独立 REF Difftest；不等于 CSR 合法性已完成。

 接下来依次完善：
   1) cause=2，CSR 访问合法性：先确定 CSR 支持表，再检查真实读写许可。
      本项目对未支持 CSR 选择报非法；只读 CSR 写入/权限不足也需检查。
      CSR 字段的 WARL 取值约束与访问非法分开处理，不能一律转成异常。
   2) cause=0，跳转目标未对齐：无 C 时检查实际采用目标的 4 字节对齐；
      未 taken 的分支不报错，JALR 先清 bit0，再检查剩余对齐条件。
   3) cause=4/6，Load/Store 未对齐：按首版策略在请求发出前检查；
      Device 的非法宽度/对齐还需结合区域属性，不能透明拆成多次设备访问。
   4) cause=1，取指访问错误：把错误附到原取指 PC；旧路径错误只排空。
   5) cause=5/7，Load/Store 访问错误：把错误响应绑定原指令及有效地址，
      禁止错误 Load 写 rd，并检查 Store/MMIO 已接受事务的副作用边界。
      当前 IF/D 错误仍走 faulted/dpi_mem_fault 停机，尚未接入精确 WB trap。
   6) cause=11，统一 M-mode ECALL：复用异常路径，故障指令不普通退休。
   7) cause=3，架构 EBREAK：先迁移现有 EBREAK 仿真 HALT/测试退出合同，
      再让普通 EBREAK 产生 breakpoint 异常，不能直接破坏既有测试出口。

 所有新增异常共用：有效指令的 PC/inst + exc_valid/cause/tval 随流水线传递；
 WB 按年龄处理最老故障，抑制普通 GPR/CSR 写，保存现场并 redirect/kill。
 Store/MMIO 的不可撤销许可必须在请求呈现前满足，不能仅靠 WB 禁止写回。
 REF 独立处理相同故障；同时验收错误路径取消、停顿、IRQ 碰撞及软件恢复。
 合法 MRET 是正常退休/返回操作，没有单独异常编号；非法使用归 cause=2。

 后续能力再启用：8/9 随 U/S；12/13/15 随 MMU；H 类随虚拟化扩展；
 16/18/19 随对应扩展或错误机制。IRQ_* 属于异步中断，另按中断源/SoC 完善。
 路线：npc/notes/design-process/cpu-design-roadmap-e-to-linux-dualissue.md 第 5 节。
 已验收范围：npc/notes/cpu-fuzzing/nemu-illegal-difftest-acceptance.md。

*/