module id_stage (
    instr
    // control
    ,rs1_addr
    ,rs2_addr
    ,rd_addr
    ,rs1_rdata
    ,rs2_rdata
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
    // imm_gen
    ,imm_out
    ,inst
);
    input  wire [31:0] instr;
    input wire [31:0] rs1_rdata,rs2_rdata;
    output wire [4 :0] alu_op;
    output wire [0 :0] alu_src_a;
    output wire [1 :0] alu_src_b;
    output wire [2 :0] br_type;
    output wire [0 :0] mem_read,mem_write,reg_write;
    output wire [2 :0] lsu_type;
    output wire [1 :0] reg_wdata_src;
    output wire [1 :0] pc_sel;
    output wire [31:0] imm_out,inst;
    output wire [4 :0] rd_addr,rs1_addr,rs2_addr;



    wire [6:0] opcode   = instr[6:0];
    wire [2:0] funct3   = instr[14:12];
    wire [6:0] funct7   = instr[31:25];
    assign rs1_addr = instr[19:15];
    assign rs2_addr = instr[24:20];
    assign rd_addr  = instr[11:7];
    assign inst = instr;
    imm_gen imm_gen_pic(
        .instr      (instr          )
        ,.imm_out   (imm_out        )
    );

    control u_control (
        .inst             (inst),
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
        .pc_sel           (pc_sel)
    );


endmodule
