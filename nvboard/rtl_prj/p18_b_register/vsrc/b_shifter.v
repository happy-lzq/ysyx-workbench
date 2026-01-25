module b_shifter (
     din
    ,shmat
    ,l_r
    ,a_l
    ,dout
);
    input                       l_r,a_l;    // a_l=1：算术移位，否则逻辑移位;l_r=1：左移，否则右移
    input   wire    [2:0]         shmat;    // 移动位宽3位
    input   wire    [7:0]           din;    // 输入
    output  wire    [7:0]          dout;    // 输出
    wire            [7:0]   al,ar,ll,lr;    

    assign al = ($signed(din)) <<< shmat ;  // 算术左移：移动n位=乘以2的n次方
    assign ar = ($signed(din)) >>> shmat ;  // 算术右移=逻辑右移
    assign ll = din << shmat             ;
    assign lr = din >> shmat             ;
    assign dout = (a_l == 1'b1) ? (l_r == 1'b1 ) ? al : ar
                                : (l_r == 1'b1 ) ? ll : lr;
endmodule
