module id_stage (
    instr
    ,rs1_addr
    ,rs2_addr
    ,rd_addr
    ,alu_op
    ,alu_src_a
    ,alu_src_b
    ,br_type
    ,mem_read
    ,mem_write
    ,lsu_type
    ,reg_write
    ,reg_wdata_src
    ,pc_sel
    ,imm_out
    ,csr_op
    ,csr_read
    ,csr_write
    ,csr_addr
    ,csr_zimm
    ,csr_imm
    ,mret
    ,trap_enter
    ,trap_code
    ,is_ebreak
);
    input  wire [31:0] instr;
    output wire [4 :0] alu_op;
    output wire [0 :0] alu_src_a;
    output wire [1 :0] alu_src_b;
    output wire [2 :0] br_type;
    output wire [0 :0] mem_read;
    output wire [0 :0] mem_write;
    output wire [0 :0] reg_write;
    output wire [0 :0] is_ebreak;
    output wire [2 :0] lsu_type;
    output wire [1 :0] reg_wdata_src;
    output wire [1 :0] pc_sel;
    output wire [31:0] imm_out;
    output wire [4 :0] rd_addr;
    output wire [4 :0] rs1_addr;
    output wire [4 :0] rs2_addr;
    // csr
    output wire [1 :0] csr_op;
    output wire [0 :0] csr_read;
    output wire [0 :0] csr_write;
    output wire [0 :0] csr_imm;
    output wire [0 :0] mret;
    output wire [0 :0] trap_enter;
    output wire [11:0] csr_addr;
    output wire [31:0] csr_zimm;
    output wire [31:0] trap_code; 

    wire [6:0] opcode   = instr[6:0];
    wire [2:0] funct3   = instr[14:12];
    wire [6:0] funct7   = instr[31:25];
    assign rs1_addr = instr[19:15];
    assign rs2_addr = instr[24:20];
    assign rd_addr  = instr[11:7];
    
imm_gen u_imm_gen (
    .instr      (instr),
    .imm_out    (imm_out)
);
control u_control (
    .inst             (instr),
    .opcode           (opcode),
    .funct3           (funct3),
    .funct7           (funct7),
    .alu_op           (alu_op),
    .alu_src_a        (alu_src_a),
    .alu_src_b        (alu_src_b),
    .br_type          (br_type),
    .mem_read         (mem_read),
    .mem_write        (mem_write),
    .lsu_type         (lsu_type),
    .reg_write        (reg_write),
    .reg_wdata_src    (reg_wdata_src),
    .pc_sel           (pc_sel),
    .csr_op           (csr_op),
    .csr_read         (csr_read),
    .csr_write        (csr_write),
    .csr_addr         (csr_addr),
    .csr_zimm         (csr_zimm),
    .csr_imm          (csr_imm),
    .trap_enter       (trap_enter),
    .trap_code        (trap_code),
    .is_ebreak        (is_ebreak),
    .mret             (mret)
);

endmodule
