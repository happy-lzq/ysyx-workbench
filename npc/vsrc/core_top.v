`include "ctrl_defs.vh"

module core_top (
    clk
    ,rst
    ,interrupt_valid
    ,interrupt_cause
    ,instr
    ,halt
    ,halt_pc
    ,halt_ret
);
    input  wire [0 :0]                 clk,rst,interrupt_valid;
    input  wire [31:0]                         interrupt_cause;
    output wire [0 :0]                                    halt;
    output wire [31:0]                   instr,halt_pc,halt_ret;

    // ========== 数据通路 ==========
    wire [31:0] pc, pc_plus4;
    wire [31:0] rs1_rdata, rs2_rdata, rd_wdata;
    wire [31:0] alu_result, imm_out;
    wire [31:0] mem_addr, mem_wdata_raw, mem_rdata;
    wire [31:0] jump_jalr, imm_jal, imm_br;
    wire [31:0] csr_rdata, csr_wdata, trap_target, trap_pc;
    wire [31:0] rs_a0;
    wire [0:0]  br_taken;
    wire [4:0] rs1_addr, rs2_addr;
    // ========== 控制总线（id_stage → 各模块）==========
    wire [`EX_CTRL_WIDTH-1:0]  ex_ctrl;
    wire [`MEM_CTRL_WIDTH-1:0] mem_ctrl;
    wire [`WB_CTRL_WIDTH-1:0]  wb_ctrl;
    wire [`CSR_CTRL_WIDTH-1:0] csr_ctrl;
    wire [`SYS_CTRL_WIDTH-1:0] sys_ctrl;
    wire [1:0] pc_sel;
    
    // ========== 从总线提取 + 门控（简单模块用）==========
    wire [4:0] rd_addr = wb_ctrl[`WB_CTRL_RD_ADDR_MSB:`WB_CTRL_RD_ADDR_LSB];
    wire reg_write_raw  = wb_ctrl[`WB_CTRL_REG_WRITE];
    wire is_ebreak_raw  = sys_ctrl[`SYS_CTRL_IS_EBREAK];
    wire reg_write_eff  = interrupt_valid ? 1'b0 : reg_write_raw;
    wire is_ebreak_eff  = interrupt_valid ? 1'b0 : is_ebreak_raw;

    assign jump_jalr     = alu_result;
    assign imm_jal       = imm_out;
    assign imm_br        = imm_out;
    assign mem_addr      = alu_result;
    assign mem_wdata_raw = rs2_rdata;
    assign trap_pc       = pc;

regfile u_regfile (
    .clk          (clk),
    .rs1_addr     (rs1_addr),
    .rs2_addr     (rs2_addr),
    .rd_addr      (rd_addr),
    .rd_wdata     (rd_wdata),
    .reg_write    (reg_write_eff),
    .rs1_rdata    (rs1_rdata),
    .rs2_rdata    (rs2_rdata),
    .rs_a0        (rs_a0)
);

csr u_csr (
    .clk                (clk),
    .rst                (rst),
    .csr_ctrl           (csr_ctrl),
    .sys_ctrl           (sys_ctrl),
    .interrupt_valid    (interrupt_valid),
    .interrupt_cause    (interrupt_cause),
    .csr_wdata          (csr_wdata),
    .trap_pc            (trap_pc),
    .trap_target        (trap_target),
    .csr_rdata          (csr_rdata)
);

if_stage u_if_stage (
    .clk                (clk),
    .rst                (rst),
    .sys_ctrl           (sys_ctrl),
    .interrupt_valid    (interrupt_valid),
    .trap_target        (trap_target),
    .pc_sel             (pc_sel),
    .br_taken           (br_taken),
    .jump_jalr          (jump_jalr),
    .imm_jal            (imm_jal),
    .imm_br             (imm_br),
    .pc                 (pc),
    .pc_plus4           (pc_plus4),
    .inst               (instr)
);

id_stage u_id_stage (
    .inst       (instr),
    .rs1_addr    (rs1_addr),
    .rs2_addr    (rs2_addr),
    .imm_out     (imm_out),
    .ex_ctrl     (ex_ctrl),
    .mem_ctrl    (mem_ctrl),
    .wb_ctrl     (wb_ctrl),
    .csr_ctrl    (csr_ctrl),
    .sys_ctrl    (sys_ctrl),
    .pc_sel      (pc_sel)
);

ex_stage u_ex_stage (
    .pc            (pc),
    .rs1_rdata     (rs1_rdata),
    .rs2_rdata     (rs2_rdata),
    .imm_out       (imm_out),
    .csr_rdata     (csr_rdata),
    .ex_ctrl       (ex_ctrl),
    .csr_ctrl      (csr_ctrl),
    .alu_result    (alu_result),
    .br_taken      (br_taken),
    .csr_wdata     (csr_wdata)
);

mem_stage u_mem_stage (
    .clk                (clk),
    .mem_ctrl           (mem_ctrl),
    .interrupt_valid    (interrupt_valid),
    .mem_addr           (mem_addr),
    .mem_wdata_raw      (mem_wdata_raw),
    .mem_rdata          (mem_rdata)
);

wb_stage u_wb_stage (
    .wb_ctrl       (wb_ctrl),
    .mem_rdata     (mem_rdata),
    .alu_result    (alu_result),
    .pc_plus4      (pc_plus4),
    .csr_rdata     (csr_rdata),
    .rd_wdata      (rd_wdata)
);

halt u_halt (
    .clk          (clk),
    .rst          (rst),
    .is_ebreak    (is_ebreak_eff),
    .pc           (pc),
    .rs_a0        (rs_a0),
    .halt_reg     (halt),
    .halt_pc      (halt_pc),
    .halt_ret     (halt_ret)
);

endmodule
