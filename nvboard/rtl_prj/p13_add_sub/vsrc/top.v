// top_v1.v  —— 第一版顶层：用开关输入 a,b，数码管显示二进制 sum
module top (
    //input        clk,
    //input        rstn,

    // 假设用 16 个开关输入两个操作数
    input  [7:0] sw_a,   // 用于输入 a
    input  [7:0] sw_b,   // 用于输入 b
    input         sel,
    input         cin,
    // 8 个数码管输出（名字可以根据 N4/seg.v 调整）
    output [7:0] seg0,
    output [7:0] seg1,
    output [7:0] seg2,
    output [7:0] seg3,
    output [7:0] seg4,
    output [7:0] seg5,
    output [7:0] seg6,
    output [7:0] seg7
);
    // 2. 通过 8 位加法器得到 sum & overflow
    wire [7:0] sum;
    wire       overflow;
    wire       cout;

    alu my_alu (
        .a       (sw_a),
        .b       (sw_b),
        .cin     (cin),
        .sel     (sel),
        .sum     (sum),
        .cout    (cout),
        .overflow(overflow)
    );

    // 3. 用数码管显示二进制加法结果
    seg_show my_seg_show(
        //.clk     (clk),
        //.rstn    (rstn),
        .sum     (sum),
        .overflow(overflow),
        .cout    (cout),
        .seg0    (seg0),
        .seg1    (seg1),
        .seg2    (seg2),
        .seg3    (seg3),
        .seg4    (seg4),
        .seg5    (seg5),
        .seg6    (seg6),
        .seg7    (seg7)
    );

endmodule
