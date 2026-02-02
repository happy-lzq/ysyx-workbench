
module top (
    input   [3:0]       a,b,
    input   [2:0]       sel,
    input               cin,
    output [7:0] seg0,
    output [7:0] seg1,
    output [7:0] seg2,
    output [7:0] seg3,
    output [7:0] seg4,
    output [7:0] seg5,
    output [7:0] seg6,
    output [7:0] seg7
);
    wire     cout,overflow;
    wire    [3:0]      sum;

    alu my_alu(
        .cin              (cin),
        .sel              (sel),
        .a                  (a),
        .b                  (b),
        .cout            (cout),
        .overflow    (overflow),
        .sum              (sum)
    );

    seg_show my_seg_show(
        .cout           (cout),
        .overflow   (overflow),
        .sum             (sum),
        .seg0           (seg0),
        .seg1           (seg1),
        .seg2           (seg2),
        .seg3           (seg3),
        .seg4           (seg4),
        .seg5           (seg5),
        .seg6           (seg6),
        .seg7           (seg7)
    );

endmodule
