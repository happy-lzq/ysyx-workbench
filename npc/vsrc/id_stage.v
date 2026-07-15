`include "ctrl_defs.vh"
module id_stage (
    inst
    ,rs1_addr
    ,rs2_addr
    ,imm_out
    ,ex_ctrl
    ,mem_ctrl
    ,wb_ctrl
    ,csr_ctrl
    ,sys_ctrl
    ,pc_sel
);
    
    input  wire [31:0] inst;
    // 数据信号

    output wire [4 :0] rs1_addr;
    output wire [4 :0] rs2_addr;
    output wire [31:0] imm_out;
    // 控制总线端口
    output wire [`EX_CTRL_WIDTH-1:0]  ex_ctrl;
    output wire [`MEM_CTRL_WIDTH-1:0] mem_ctrl;
    output wire [`WB_CTRL_WIDTH-1:0]  wb_ctrl;
    output wire [`CSR_CTRL_WIDTH-1:0] csr_ctrl;
    output wire [`SYS_CTRL_WIDTH-1:0] sys_ctrl;
    output wire [1:0] pc_sel;


    wire [6:0] opcode   = inst[6:0];
    wire [2:0] funct3   = inst[14:12];
    wire [6:0] funct7   = inst[31:25];
    assign rs1_addr = inst[19:15];
    assign rs2_addr = inst[24:20];
    
imm_gen u_imm_gen (
    .inst      (inst),
    .imm_out    (imm_out)
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
    .pc_sel      (pc_sel)
);
endmodule
