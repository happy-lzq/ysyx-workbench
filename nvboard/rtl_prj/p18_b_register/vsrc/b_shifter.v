module b_shifter (
     din
    ,shmat
    ,l_r
    ,a_l
    ,dout
);
    input                       l_r,a_l;
    input   wire    [2:0]         shmat;
    input   wire    [7:0]           din;
    output  wire    [7:0]          dout;
    wire            [7:0]   al,ar,ll,lr;

    assign al = ($signed(din)) <<< shmat ;
    assign ar = ($signed(din)) >>> shmat ;
    assign ll = din << shmat             ;
    assign lr = din >> shmat             ;
    assign dout = (a_l == 1'b1) ? (l_r == 1'b1 ) ? al : ar
                                : (l_r == 1'b1 ) ? ll : lr;
endmodule
