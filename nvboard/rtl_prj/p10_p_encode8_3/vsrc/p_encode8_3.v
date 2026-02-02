module p_encode8_3 (en,x,y);
    input en;
    input [7:0] x;
    output reg [2:0] y;
    always @(*) begin
        if (en) begin
            if (x[7]==1) begin
                y = 3'b111;                
            end else if (x[6]==1) begin
                y = 3'b110;
            end else if (x[5]==1)begin
                y = 3'b101;
            end else if (x[4]==1) begin
                y = 3'b100;
            end else if (x[3]==1) begin
                y = 3'b011;
            end else if (x[2]==1)begin
                y = 3'b010;
            end else if (x[1]==1) begin
                y = 3'b001; 
            end else if (x[0]==1) begin
                y = 3'b000; 
            end else y =0;
        end
        else y =0;
    end
endmodule
