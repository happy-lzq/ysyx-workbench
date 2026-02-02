module p_encode4_2 (en,x,y);
    input en;
    input [3:0] x;
    output reg [1:0] y;
    always @(*) begin
        if (en) begin
            if (x[3]==1) begin
                y = 2'b11;                
            end else if (x[2]==1) begin
                    y = 2'b10;
            end else if (x[1]==1)begin
                y = 2'b01;
            end else if (x[0]==1) begin
                y = 2'b00;
            end else y = 0;
        end
        else y =0;
    end
endmodule
