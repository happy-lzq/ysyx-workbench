`include "ctrl_defs.vh"
module ex_stage (
    pc 
    ,rs1_rdata          
    ,rs2_rdata                 
    ,imm_out    
    ,csr_rdata
    ,ex_ctrl
    ,csr_ctrl
    ,alu_result                   
    ,csr_wdata          

);
    // 数据流
    input  wire [31:0] pc;
    input  wire [31:0] rs1_rdata;
    input  wire [31:0] rs2_rdata;
    input  wire [31:0] imm_out;
    input  wire [31:0] csr_rdata;
    // 控制流
    input  wire [`EX_CTRL_WIDTH-1:0]  ex_ctrl;
    input  wire [`CSR_CTRL_WIDTH-1:0] csr_ctrl;
    // 输出
    output wire [31:0] alu_result;
    output reg  [31:0] csr_wdata ;

    // === 从 ex_ctrl 拆包 ===
    wire [4:0] alu_op    = ex_ctrl[`EX_CTRL_ALU_OP_MSB:`EX_CTRL_ALU_OP_LSB];
    wire       alu_src_a = ex_ctrl[`EX_CTRL_ALU_SRC_A];
    wire [1:0] alu_src_b = ex_ctrl[`EX_CTRL_ALU_SRC_B_MSB:`EX_CTRL_ALU_SRC_B_LSB];

    // === 从 csr_ctrl 拆包 ===
    wire [1:0]  csr_op   = csr_ctrl[`CSR_CTRL_OP_MSB:`CSR_CTRL_OP_LSB];
    wire        csr_imm  = csr_ctrl[`CSR_CTRL_IMM];
    wire [31:0] csr_zimm = csr_ctrl[`CSR_CTRL_ZIMM_MSB:`CSR_CTRL_ZIMM_LSB];
    wire        [31:0] src1,src2;
    
    assign src1 = (alu_src_a == 1'b0 ) ? rs1_rdata : pc;
    assign src2 = (alu_src_b == 2'b00) ? rs2_rdata :
                  (alu_src_b == 2'b01) ? imm_out  : 32'd4;

    wire [31:0] csr_src = csr_imm ? rs1_rdata : csr_zimm;
    always @(*) begin
        case (csr_op)
           2'b00 : csr_wdata = csr_src;               // csrrw csrrwi
           2'b01 : csr_wdata = csr_rdata |  csr_src;  // csrrs csrrsi
           2'b10 : csr_wdata = csr_rdata & ~csr_src;  // csrrc csrrci
            default: csr_wdata = 32'b0;
    endcase
    end


    alu alu_pic(
        .src1           (src1      )
        ,.src2          (src2      )
        ,.alu_op        (alu_op    )
        ,.result        (alu_result)
    );
endmodule
