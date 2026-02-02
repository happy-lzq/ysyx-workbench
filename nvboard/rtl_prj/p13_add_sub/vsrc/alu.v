
module alu(
    input        sel,
    input  [7:0] a, b,
    input        cin,
    output [7:0] sum,
    output       cout,
    output       overflow
);
    wire [7:0] invert_b = {8{sel}}^b;
    wire [8:0] add_sum = {1'b0, a} + {1'b0, b} + {8'b0, cin};
    wire sub_cin = 1'b1 - cin;
    // cin=1，低位借位，多减1，取反+1，与b取反码+1相抵消。

    wire [8:0] sub_sum = {1'b0, a} + {1'b0, invert_b} + {8'b0, sub_cin};

    assign {cout,sum} = (sel==1'b0) ? add_sum : sub_sum;
    assign overflow   = (sel==1'b0) ? ((a[7] == b[7])        && (sum[7] != a[7])):
                                      ((a[7] == invert_b[7]) && (sum[7] != a[7]));

        // 溢出为1，非溢出为0，同时成立为溢出，
        // 数字逻辑当中，在硬件是实现上，取反操作：按位异或，异或特性：与 1 异或会翻转，与 0 异或保持不变。

endmodule

/*
运算模式	cin（输入）	cout（输出）
加法	    来自低位的进位	向高位的进位
减法	    来自低位的借位	向高位的借位（0表示需要借位，1表示不需要）




module alu (
    input [7:0] a, b,
    input cin, sel,
    output reg cout,
    output reg [7:0] sum,
    output reg overflow
);


    always @(*) begin
        invert_b = {8{sel}} ^ b;
        if (!sel) begin
            // 加法
            {cout, sum} = {1'b0, a} + {1'b0, b} + {8'b0, cin};
            overflow = (a[7] == b[7]) && (sum[7] != a[7]);

        end else begin
            // 减法  
            wire [7:0] invert_b = ~b;
            wire cin_effective = 1'b1 - cin;  // 关键！
            {cout, sum} = {1'b0,a} + {1'b0,invert_b} + {8'b0, cin_effective};
            overflow = (a[7] == invert_b[7]) && (sum[7] != a[7]);
        end
    end
endmodule
*/
