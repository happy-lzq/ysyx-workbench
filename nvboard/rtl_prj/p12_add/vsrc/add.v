module add(
    input  [7:0] a, b,
    input        cin,
    output [7:0] sum,
    output       cout,
    output overflow
);
    assign {cout, sum} = {1'b0, a} + {1'b0, b} + {8'b0, cin};
    // 溢出为1，非溢出为0，同时成立为溢出，
    assign overflow = (a[7] == b[7] && (sum[7] != a[7]));
endmodule
