module seg (
    input [2:0] seg_in,
    output reg [7:0] seg_out
);
    always @(*) begin
        // 共阳
        case (seg_in)
            3'd000 : seg_out = 8'b00000010;
            3'b001 : seg_out = 8'b10011111;
            3'b010 : seg_out = 8'b00100101;
            3'b011 : seg_out = 8'b00001101;
            3'b100 : seg_out = 8'b10011001;
            3'b101 : seg_out = 8'b01001001;
            3'b110 : seg_out = 8'b01000001;
            3'b111 : seg_out = 8'b00011111;
        endcase
    end
endmodule
