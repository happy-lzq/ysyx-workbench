module top (
     clk
    ,rst
    ,dout
    ,seg0
    ,seg1
    ,seg2
    ,seg3
    ,seg4
    ,seg5
    ,seg6
    ,seg7
);
    input                                                   clk,rst;
    output          [7:0]   seg0,seg1,seg2,seg3,seg4,seg5,seg6,seg7;
    output  wire    [7:0]                                      dout;


    lfsr u_lfsr (
    .clk        (clk),
    .rst        (rst),
    .dout       (dout)
);

    seg_show u_seg_show (
    // < i
    .seg_in    (dout),
    // > o
    .seg0      (seg0),
    // > o
    .seg1      (seg1),
    // > o
    .seg2      (seg2),
    // > o
    .seg3      (seg3),
    // > o
    .seg4      (seg4),
    // > o
    .seg5      (seg5),
    // > o
    .seg6      (seg6),
    // > o
    .seg7      (seg7)
);

endmodule
