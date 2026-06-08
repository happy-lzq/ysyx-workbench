module alu (
    src1
    ,src2
    ,alu_op
    ,result
);
    input wire [31:0] src1;
    input wire [31:0] src2;
    input wire [4 :0] alu_op;
    output reg [31:0] result;

    always @(*) begin
        case (alu_op)
            5'b0_0000 :  result = src1 - src2;                                      // SUB 
            5'b0_0001 :  result = src1 + src2;                                      // ADD
            5'b0_0010 :  result = src1 << (src2[4:0]) ;                             // SLL  逻辑左移,RISCV规定移位只取低5位
            5'b0_0011 :  result = ($signed(src1) < $signed(src2)) ? 32'd1 : 32'd0;  // SLT  有符号小于置位
            5'b0_0100 :  result = (src1 < src2) ? 32'd1 : 32'd0;                    // SLTU 无符号小于置位
            5'b0_0101 :  result = src1 ^ src2;                                      // XOR  按位异或
            5'b0_0110 :  result = src1 | src2;                                      // OR   按位或
            5'b0_0111 :  result = src1 & src2;                                      // AND  按位与
            5'b0_1000 :  result = src1 >> src2[4:0];                                // SRL  逻辑右移
            5'b0_1001 :  result = $signed(src1) >>> src2[4:0];                      // SRA  算术右移 有符号
            5'b0_1010 :  result = src2;                                             // LUI  高位立即数

            5'b1_0000 : result = src1 * src2;                                           // MUL
            5'b1_0001 : result = (($signed(src1) * $signed(src2)) >> 32);               // MULH
            5'b1_0010 : result = (($signed(src1) * src2) >> 32);                        // MULHSU
            5'b1_0011 : result = ((src1 * src2) >> 32);                                 // MULHU
            5'b1_0100 : result = (src2 == 0) ? ~32'd0 : $signed(src1) / $signed(src2);  // DIV
            5'b1_0101 : result = (src2 == 0) ? ~32'd0 : src1 / src2;                    // DIVU
            5'b1_0110 : result = (src2 == 0) ? src1 : $signed(src1) % $signed(src2);    // REM
            5'b1_0111 : result = (src2 == 0) ? src1 : src1 % src2;                      // REMU
        
            default:    result = 32'd0;
        endcase
    end 
endmodule
