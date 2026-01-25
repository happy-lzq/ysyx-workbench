module bcd7seg(
    input [3:0] in,
    output reg [6:0] bcd7seg_out
);

    always @(*) begin
        case(in)
            4'd0: bcd7seg_out = 7'b1111110;
            4'd1: bcd7seg_out = 7'b0110000;
            4'd2: bcd7seg_out = 7'b1101101;
            4'd3: bcd7seg_out = 7'b1111001;
            4'd4: bcd7seg_out = 7'b0110011;
            4'd5: bcd7seg_out = 7'b1011011;
            4'd6: bcd7seg_out = 7'b1011111;
            4'd7: bcd7seg_out = 7'b1110000;
            4'd8: bcd7seg_out = 7'b1111111;
            4'd9: bcd7seg_out = 7'b1111011;
            4'd10: bcd7seg_out = 7'b0000001; 
            default: bcd7seg_out = 7'b0000000;
        endcase
    end
endmodule
