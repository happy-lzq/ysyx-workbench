module top (
     din
    ,shmat
    ,l_r
    ,a_l
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
    input                                                   l_r,a_l;
    input   wire    [2:0]                                     shmat;
    input   wire    [7:0]                                       din;
    output          [7:0]   seg0,seg1,seg2,seg3,seg4,seg5,seg6,seg7;

    output  wire    [7:0]                                      dout;


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

    b_shifter u_b_shifter (
    .din      (din),
    .shmat    (shmat),
    .l_r      (l_r),
    .a_l      (a_l),
    .dout     (dout)
);
endmodule
