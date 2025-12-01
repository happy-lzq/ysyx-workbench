module light (
    input clk,input rst,output reg [15:0] led);
    reg[31:0] count;
    always @(posedge clk) begin
        if (rst) begin
            led <= 1;
            count <= 0;
        end else begin
            if (count == 0) 
            led <={led[14:0],led[15]};
            count <=(count >= 500000 ? 32'b0 : count +1);
        // 实现一个模500000的计数器，用于产生一个周期为500000个时钟周期的使能信号，控制LED每隔500000个时钟周期移位一次
        end
    end
endmodule   
