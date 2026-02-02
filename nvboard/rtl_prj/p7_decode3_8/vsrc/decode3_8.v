/*
module decode3_8 (en,x,y);
    input en;
    input [2:0] x;
    output reg [7:0] y;
    always @(*) begin
        if (en) begin
            case (x)
                3'd0 : y = 8'b00000001;
                3'd1 : y = 8'b00000010;
                3'd2 : y = 8'b00000100;
                3'd3 : y = 8'b00001000;
                3'd4 : y = 8'b00010000;
                3'd5 : y = 8'b00100000;
                3'd6 : y = 8'b01000000;
                3'd7 : y = 8'b10000000;
                default : y = 8'b11111111;             
            endcase
        end else y = 8'b00000000;
    end
endmodule

*/
module decode3_8(en, x, y);
    input en;
    input [2:0] x;
    output reg [7:0] y;
    integer i;
    
    always@(*) begin
        if(en) begin
            for(i = 0; i < 8; i = i + 1) begin
                //integer 位宽大小为32位  x与i位宽必须保持一致
                if(x == i[2:0])
                    y[i] = 1'b1;
                else
                    y[i] = 1'b0;
            end
        end
        else 
            y = 8'b00000000;
    end
endmodule
