module vga_pic(
     vga_clk
    ,rst_n
    ,pix_x
    ,pix_y
    ,pix_data
);
    input            vga_clk,rst_n;
    input wire  [9:0]  pix_x,pix_y; 
    output reg  [15:0]    pix_data;

    parameter 
    H_VALID = 10'd640       , //行有效数据
    V_VALID = 10'd480       ; //场有效数据

    parameter 
    RED     = 16'hF800      , //红色
    ORANGE  = 16'hFC00      , //橙色
    YELLOW  = 16'hFFE0      , //黄色
    GREEN   = 16'h07E0      , //绿色
    CYAN    = 16'h07FF      , //青色
    BLUE    = 16'h001F      , //蓝色
    PURPLE = 16'hF81F      , //紫色
    BLACK   = 16'h0000      , //黑色
    WHITE   = 16'hFFFF      , //白色
    GRAY    = 16'hD69A      ; //灰色

always@(posedge vga_clk or negedge rst_n) begin
    if(rst_n == 1'b0)
        pix_data <= 16'd0;
    else case(1'b1)  // 真值表方式
        (pix_y < (V_VALID/10)*1): pix_data <= RED;
        (pix_y < (V_VALID/10)*2): pix_data <= ORANGE;
        (pix_y < (V_VALID/10)*3): pix_data <= YELLOW;
        (pix_y < (V_VALID/10)*4): pix_data <= GREEN;
        (pix_y < (V_VALID/10)*5): pix_data <= CYAN;
        (pix_y < (V_VALID/10)*6): pix_data <= BLUE;
        (pix_y < (V_VALID/10)*7): pix_data <= PURPLE;
        (pix_y < (V_VALID/10)*8): pix_data <= BLACK;
        (pix_y < (V_VALID/10)*9): pix_data <= WHITE;
        (pix_y < V_VALID): pix_data <= GRAY;
        default: pix_data <= BLACK;
    endcase
end

endmodule
