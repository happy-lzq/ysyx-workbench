module mux2_4_1 (x0,x1,x2,x3,s,y);
    input [1:0] x0,x1,x2,x3,s;
    output reg [1:0] y;
    always@(*)
        case (s)
            2'b00 : y = x0;
            2'b01 : y = x1;
            2'b10 : y = x2;
            2'b11 : y = x3;
            default : y = 2'b0;
        endcase
endmodule
