// seg_show.v
module seg_show (
    //input        clk,
    //input        rstn,
    input  [7:0] sum,       // 要显示的 8 位结果（二进制）
    input        overflow,  // 溢出标志 → 最高位小数点
    output [7:0] seg0,
    output [7:0] seg1,
    output [7:0] seg2,
    output [7:0] seg3,
    output [7:0] seg4,
    output [7:0] seg5,
    output [7:0] seg6,
    output [7:0] seg7
);
    //   段码编码：{dp,g,f,e,d,c,b,a}
    //   共阳极数码管：1表示熄灭，0表示点亮
    //   数字 0: g=0, f=1, e=1, d=1, c=1, b=1, a=1 → 11000000
    //   数字 1: g=1, f=1, e=1, d=1, c=0, b=0, a=1 → 11111001
    localparam [7:0] SEG_0_DP_NO = 8'b00000011;  // 显示 0，小数点灭
    localparam [7:0] SEG_1_DP_NO = 8'b10011111;  // 显示 1，小数点灭

    //  处理输入8位计算结果sum分不到8个数码管、以及溢出处理
    function [7:0] encode_bit;
        input bit_val;
        input dp_on;
        begin
            encode_bit = (bit_val ? SEG_1_DP_NO : SEG_0_DP_NO) & 
            (dp_on ? 8'b1111_1110 : 8'b1111_111_1);
        end
    endfunction

    // 8 个数码管分别显示 sum[0]~sum[7]
    wire [7:0] seg0_int = encode_bit(sum[0], 1'b0);
    wire [7:0] seg1_int = encode_bit(sum[1], 1'b0);
    wire [7:0] seg2_int = encode_bit(sum[2], 1'b0);
    wire [7:0] seg3_int = encode_bit(sum[3], 1'b0);
    wire [7:0] seg4_int = encode_bit(sum[4], 1'b0);
    wire [7:0] seg5_int = encode_bit(sum[5], 1'b0);
    wire [7:0] seg6_int = encode_bit(sum[6], 1'b0);
    wire [7:0] seg7_int = encode_bit(sum[7], overflow); // 最高位小数点 = overflow

    // 共阳极数码管：直接输出，不需要取反
    assign seg0 = seg0_int;
    assign seg1 = seg1_int;
    assign seg2 = seg2_int;
    assign seg3 = seg3_int;
    assign seg4 = seg4_int;
    assign seg5 = seg5_int;
    assign seg6 = seg6_int;
    assign seg7 = seg7_int;

endmodule
