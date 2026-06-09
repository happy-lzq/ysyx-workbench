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

    function [31:0] unsigned_div32;
        input [31:0] dividend;
        input [31:0] divisor;
        begin
            unsigned_div32 = (divisor == 32'd0) ? 32'hffff_ffff : dividend / divisor;
        end
    endfunction

    function [31:0] unsigned_rem32;
        input [31:0] dividend;
        input [31:0] divisor;
        begin
            unsigned_rem32 = (divisor == 32'd0) ? dividend : dividend % divisor;
        end
    endfunction

    function [31:0] signed_div32;
        input [31:0] dividend;
        input [31:0] divisor;
        reg [31:0] abs_dividend;
        reg [31:0] abs_divisor;
        reg [31:0] quotient;
        begin
            if (divisor == 32'd0) begin
                signed_div32 = 32'hffff_ffff;
            end else if (dividend == 32'h8000_0000 && divisor == 32'hffff_ffff) begin
                signed_div32 = 32'h8000_0000;
            end else begin
                abs_dividend = dividend[31] ? (~dividend + 32'd1) : dividend;
                abs_divisor  = divisor[31]  ? (~divisor  + 32'd1) : divisor;
                quotient = abs_dividend / abs_divisor;
                signed_div32 = (dividend[31] ^ divisor[31]) ? (~quotient + 32'd1) : quotient;
            end
        end
    endfunction

    function [31:0] signed_rem32;
        input [31:0] dividend;
        input [31:0] divisor;
        reg [31:0] abs_dividend;
        reg [31:0] abs_divisor;
        reg [31:0] remainder;
        begin
            if (divisor == 32'd0) begin
                signed_rem32 = dividend;
            end else if (dividend == 32'h8000_0000 && divisor == 32'hffff_ffff) begin
                signed_rem32 = 32'd0;
            end else begin
                abs_dividend = dividend[31] ? (~dividend + 32'd1) : dividend;
                abs_divisor  = divisor[31]  ? (~divisor  + 32'd1) : divisor;
                remainder = abs_dividend % abs_divisor;
                signed_rem32 = dividend[31] ? (~remainder + 32'd1) : remainder;
            end
        end
    endfunction

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
            5'b1_0100 : result = signed_div32(src1, src2);                               // DIV
            5'b1_0101 : result = unsigned_div32(src1, src2);                             // DIVU
            5'b1_0110 : result = signed_rem32(src1, src2);                               // REM
            5'b1_0111 : result = unsigned_rem32(src1, src2);                             // REMU
        
            default:    result = 32'd0;
        endcase
    end 
endmodule
