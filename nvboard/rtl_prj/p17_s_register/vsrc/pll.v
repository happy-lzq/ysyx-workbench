module pll (
     clk
    ,rst_n
    ,clk_out
);
    input         clk,rst_n;
    output reg       clk_out;

    parameter INPUT_FREQ = 5_000_000   ;
    parameter OUTPUT_FREQ = 1           ;
    parameter DUTY_CYCLE = 50           ; 
    // 计算分频系数
    localparam DIVIDER      = INPUT_FREQ / (2* OUTPUT_FREQ)     ;//25 000 000
    localparam HIGH_COUNT   = (DIVIDER * DUTY_CYCLE) / 100      ;//12 500 000
    //localparam LOW_COUNT    = DIVIDER - HIGH_COUNT              ;

    reg [31:0]  counter;

    always @(posedge clk or negedge rst_n ) begin
        if (!rst_n) begin
            counter <=0;
            clk_out <=0;
        end else begin
            if (counter >= DIVIDER -1) begin
                counter <= 0;
                clk_out <= 1'b1;
            end else begin
                counter <= counter +1;
                if (counter == HIGH_COUNT -1 ) begin
                    clk_out <=1'b0;
                end
            end
        end
    end
endmodule

