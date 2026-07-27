`include "ctrl_defs.vh"
module id_stage (
    inst
    ,if_id_pc
    ,rs1_data
    ,rs2_data
    ,rs1_addr
    ,rs2_addr
    ,rd_addr
    ,id_uses_rs1
    ,id_uses_rs2
    ,imm_out
    ,redirect_pc
    ,ex_ctrl
    ,mem_ctrl
    ,wb_ctrl
    ,csr_ctrl
    ,sys_ctrl
    ,pc_ctrl
);
    
    input  wire [31:0] inst;
    input  wire [31:0] if_id_pc;
    input  wire [31:0] rs1_data;
    input  wire [31:0] rs2_data;
    // 输出数据
    output wire [4 :0] rs1_addr;
    output wire [4 :0] rs2_addr;
    output wire [4 :0] rd_addr;
    output wire id_uses_rs1;
    output wire id_uses_rs2;
    output wire [31:0] imm_out;
    output wire [31:0] redirect_pc;
    // 控制总线端口
    output wire [`EX_CTRL_WIDTH-1:0]  ex_ctrl;
    output wire [`MEM_CTRL_WIDTH-1:0] mem_ctrl;
    output wire [`WB_CTRL_WIDTH-1:0]  wb_ctrl;
    output wire [`CSR_CTRL_WIDTH-1:0] csr_ctrl;
    output wire [`SYS_CTRL_WIDTH-1:0] sys_ctrl;
    output wire [`PC_CTRL_WIDTH-1 :0] pc_ctrl;

    assign rs1_addr = inst[19:15];
    assign rs2_addr = inst[24:20];
    assign rd_addr  = inst[11: 7];

    wire [6:0] opcode   = inst[6:0];
    wire [2:0] funct3   = inst[14:12];
    wire [6:0] funct7   = inst[31:25];
    wire [0:0] br_taken;
    wire [`PC_CTRL_WIDTH-1:0] pc_ctrl_int;
    wire [2:0] br_type  = pc_ctrl_int[`BR_TYPE_MSB : `BR_TYPE_LSB] ;
    wire [0:0] is_jalr = pc_ctrl_int[`IS_JALR];  
    wire [31:0] target_sum = (is_jalr ? rs1_data : if_id_pc) + imm_out;
    // jalr         next_pc = rs1_data + imm_out 且 需要低位清零。
    // jal + branch next_pc = pc + imm_out 
    assign redirect_pc = is_jalr ? {target_sum[31:1], 1'b0} : target_sum;
    assign pc_ctrl = {pc_ctrl_int[`PC_CTRL_WIDTH-1:`BR_TAKEN+1],
                      br_taken,
                      pc_ctrl_int[`BR_TAKEN-1:0]};
                      
    // =============== 指令分类输出，防止假stall ================
    wire is_r_type  = (opcode == 7'b0110011);
    wire is_i_alu   = (opcode == 7'b0010011);
    wire is_load    = (opcode == 7'b0000011);
    wire is_store   = (opcode == 7'b0100011);
    wire is_branch  = (opcode == 7'b1100011);
    wire jalr_inst  = (opcode == 7'b1100111);
    wire is_csr_reg = (opcode == 7'b1110011) &&
                      ((funct3 == 3'b001) ||
                       (funct3 == 3'b010) ||
                       (funct3 == 3'b011));
    assign id_uses_rs1 = is_r_type ||
                         is_i_alu  ||
                         is_load   ||
                         is_store  ||
                         is_branch ||
                         jalr_inst ||
                         is_csr_reg;

    assign id_uses_rs2 = is_r_type ||
                         is_store  ||
                         is_branch;
    
imm_gen u_imm_gen (
    .inst      (inst),
    .imm_out    (imm_out)
);

br_cond u_br_cond (
    .rs1_data    (rs1_data),
    .rs2_data    (rs2_data),
    .br_type     (br_type),
    .br_taken    (br_taken)
);
control u_control (
    .inst        (inst),
    .opcode      (opcode),
    .funct3      (funct3),
    .funct7      (funct7),
    .ex_ctrl     (ex_ctrl),
    .mem_ctrl    (mem_ctrl),
    .wb_ctrl     (wb_ctrl),
    .csr_ctrl    (csr_ctrl),
    .sys_ctrl    (sys_ctrl),
    .pc_ctrl     (pc_ctrl_int)
);
endmodule
