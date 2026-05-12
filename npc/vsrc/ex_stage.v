module ex_stage (
    rs1_rdata           // I
    ,rs2_rdata          // I
    ,br_type            // I
    ,imm_out            // I
    ,alu_op             // I
    ,alu_src_a          // I
    ,alu_src_b          // I
    ,pc                 // I
    ,alu_result         // O
    ,br_taken           // O
    

);

    input  wire [4 :0] alu_op;
    input  wire [0 :0] alu_src_a;
    input  wire [1 :0] alu_src_b;
    input  wire [2 :0] br_type;
    input  wire [31:0] pc    ;
    input  wire [31:0] imm_out;
    input  wire [31:0] rs1_rdata,rs2_rdata;

    output wire [31:0] alu_result;
    output wire [0 :0] br_taken  ;


    wire  [31:0] src1,src2;
    assign src1 = (alu_src_a == 1'b0 ) ? rs1_rdata : pc;
    assign src2 = (alu_src_b == 2'b00) ? rs2_rdata :
                  (alu_src_b == 2'b01) ? imm_out  : 32'd4;
    
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
