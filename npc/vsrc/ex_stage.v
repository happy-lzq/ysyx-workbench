module ex_stage (
    rs1_rdata          
    ,rs2_rdata         
    ,br_type           
    ,imm_out           
    ,alu_op            
    ,alu_src_a         
    ,alu_src_b         
    ,pc                
    ,csr_op
    ,csr_imm
    ,csr_zimm
    ,csr_rdata
    ,alu_result         
    ,br_taken           
    ,csr_wdata          

);

    input  wire [4 :0] alu_op;
    input  wire [0 :0] alu_src_a;
    input  wire [1 :0] alu_src_b;
    input  wire [2 :0] br_type;
    input  wire [31:0] pc    ;
    input  wire [31:0] imm_out;
    input  wire [31:0] rs1_rdata,rs2_rdata;
    input  wire [1 :0] csr_op;
    input  wire [31:0] csr_rdata,csr_zimm;
    input  wire [0 :0] csr_imm;

    output wire [31:0] alu_result;
    output wire [0 :0] br_taken  ;
    output reg  [31:0] csr_wdata ;
    wire        [31:0] src1,src2;
    
    assign src1 = (alu_src_a == 1'b0 ) ? rs1_rdata : pc;
    assign src2 = (alu_src_b == 2'b00) ? rs2_rdata :
                  (alu_src_b == 2'b01) ? imm_out  : 32'd4;
    wire [31:0] csr_src = csr_imm ? rs1_rdata : csr_zimm;
    always @(*) begin
        case (csr_op)
           2'b00 : csr_wdata = csr_rdata ;            // csrrw csrrwi
           2'b01 : csr_wdata = csr_rdata |  csr_src;  // csrrs csrrsi
           2'b10 : csr_wdata = csr_rdata & ~csr_src;  // csrrc csrrci
            default: csr_wdata = 32'b0;
    endcase
    end

    br_cond br_cond_pic(
        .rs1_data       (rs1_rdata )
        ,.rs2_data      (rs2_rdata )
        ,.br_type       (br_type   )
        ,.br_taken      (br_taken  )
    );

    alu alu_pic(
        .src1           (src1      )
        ,.src2          (src2      )
        ,.alu_op        (alu_op    )
        ,.result        (alu_result)
    );
endmodule
