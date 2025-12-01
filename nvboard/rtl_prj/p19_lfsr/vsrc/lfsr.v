module lfsr (
     clk
    ,rst
    ,dout

);
    input                       clk,rst;
    output      reg [7:0]          dout;
    wire                       feedback;

    assign feedback = (dout == 8'b0) ? (1'b1) :(dout[4] ^ dout[3] ^ dout[2] ^dout[0] );

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            dout <= 8'b0000_0001;
        end else 
            dout <= {feedback,dout[7:1]};
    end
endmodule
