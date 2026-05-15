module core_top (
    clk
    ,rst
);
    input  wire [0 :0]                            clk,rst;
    wire [4 :0]                 rs1_addr,rs2_addr,rd_addr;
    wire [0 :0]     mem_read,mem_write,reg_write,br_taken;
    wire [31:0]              rd_wdata,rs1_rdata,rs2_rdata;
    wire [1 :0]                                    pc_sel;
    wire [31:0]                  jump_jalr,imm_jal,imm_br;
    wire [31:0]                                        pc;
    wire [31:0]                            pc_plus4,instr;
    wire [4 :0]                                    alu_op;
    wire [0 :0]                                 alu_src_a;
    wire [1 :0]                                 alu_src_b;
    wire [2 :0]                                   br_type;
    wire [2 :0]                                  lsu_type;
    wire [1 :0]                             reg_wdata_src;
    wire [31:0]                        alu_result,imm_out;
    wire [31:0]                                  mem_addr;        
    wire [31:0]               mem_rdata_raw,mem_wdata_raw; 
    wire [31:0]                                 mem_rdata;


    assign jump_jalr        = alu_result;
    assign imm_jal          = imm_out;
    assign imm_br           = imm_out;
    assign mem_addr         = alu_result;
    assign mem_wdata_raw    = rs2_rdata;    
    
    regfile u_regfile (
    .clk          (clk),       
    .rs1_addr     (rs1_addr),
    .rs2_addr     (rs2_addr),
    .rd_addr      (rd_addr),
    .rd_wdata     (rd_wdata),
    .reg_write    (reg_write),
    .rs1_rdata    (rs1_rdata),
    .rs2_rdata    (rs2_rdata)
);
    if_stage u_if_stage (
    .clk          (clk),
    .rst          (rst),
    .pc_sel       (pc_sel),
    .br_taken     (br_taken),
    .jump_jalr    (jump_jalr),
    .imm_jal      (imm_jal),
    .imm_br       (imm_br),
    .pc           (pc),
    // 用于regfile 写回
    .pc_plus4     (pc_plus4),
    .instr        (instr)
);
    id_stage u_id_stage (
    // control
    .instr            (instr),
    .rs1_addr         (rs1_addr),
    .rs2_addr         (rs2_addr),
    .rd_addr          (rd_addr),
    .rs1_rdata        (rs1_rdata),
    .rs2_rdata        (rs2_rdata),
    .alu_op           (alu_op),
    .alu_src_a        (alu_src_a),
    .alu_src_b        (alu_src_b),
    .br_type          (br_type),
    .mem_read         (mem_read),
    .mem_write        (mem_write),
    .lsu_type         (lsu_type),
    .reg_write        (reg_write),
    .reg_wdata_src    (reg_wdata_src),
    // imm_gen
    .pc_sel           (pc_sel),
    .imm_out          (imm_out)
);
    ex_stage u_ex_stage (
    .rs1_rdata     (rs1_rdata),
    .rs2_rdata     (rs2_rdata),
    .br_type       (br_type),
    .imm_out       (imm_out),
    .alu_op        (alu_op),
    .alu_src_a     (alu_src_a),
    .alu_src_b     (alu_src_b),
    .pc            (pc),
    .alu_result    (alu_result),
    .br_taken      (br_taken)
);
    mem_stage u_mem_stage (
    .clk              (clk),
    .lsu_type         (lsu_type),
    .mem_read         (mem_read),
    .mem_write        (mem_write),
    .mem_addr         (mem_addr),
    .mem_wdata_raw    (mem_wdata_raw),
    .mem_rdata        (mem_rdata)
);
wb_stage u_wb_stage (
    .reg_wdata_src    (reg_wdata_src),
    .mem_rdata        (mem_rdata),
    .alu_result       (alu_result),
    .pc_plus4         (pc_plus4),
    .reg_wdata        (rd_wdata)
);
endmodule
