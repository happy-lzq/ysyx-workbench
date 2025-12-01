module alu (
    input               cin,
    input      [2:0]    sel,
    input      [3:0]    a,b,
    output reg [3:0]    sum,
    output             cout,
    output         overflow
);
    // 减法不借位，正常取反加1，借位则加1减1抵消
    wire           able_cin = 1'b1 -cin;
    wire    [3:0]    able_b = ~b + {3'b0,able_cin};
    wire    [4:0]   sub_sum = {1'b0,a} + {1'b0,able_b};
    // 加法正常
    wire    [4:0]   add_sum = {1'b0,a} + {1'b0,b} + {4'b0,cin};
    // 进位处理
    assign       cout = (sel==3'b000) ? add_sum[4]:
                        (sel==3'b001) ? sub_sum[4]:1'b0;
    // 溢出处理
    wire sub_overflow = (a[3]==able_b[3]) && (sub_sum[3] != a[3]);
    wire add_overflow = (a[3]==b[3])      && (add_sum[3] != a[3]); 
    assign  overflow  = (sel==3'b000) ? add_overflow :
                        (sel==3'b001) ? sub_overflow :1'b0;

    always @(*) begin
        case (sel)
            3'b000 : sum = add_sum[3:0];
            3'b001 : sum = sub_sum[3:0];
            3'b010 : sum =           ~a;
            3'b011 : sum =        a & b;
            3'b100 : sum =        a | b;
            3'b101 : sum =        a ^ b;
            3'b110 : sum = ($signed(a) < $signed(b))  ? {4'b1} : 4'b0;
            3'b111 : sum = (a == b) ? {4'b1} : 4'b0;
        endcase
    end
endmodule
