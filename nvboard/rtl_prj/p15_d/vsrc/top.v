module top#(
    parameter WIDTH = 8
)(
    input wire clk,
    input wire clr,
    input wire en ,
    input wire set,
    input wire [WIDTH-1:0]d,
    output reg [WIDTH-1:0]q
);
    // 同步置数，异步清零
    always @(posedge clk or posedge clr) begin
        if (clr) begin
            q <= {WIDTH{1'b0}};
        end else begin
            if (set) begin
                q <= {WIDTH{1'b1}};
            end else if (en) begin
                q <= d;
            end
        end

    end
endmodule

/*
    always @(posedge clk or posedge rst)
    含义： 只要 rst 信号从 0 变 1（上升沿），或者 clk 上升沿到来，就会触发逻辑。
    配合代码： 内部必须写成 if (rst)。
    结果： 这是高电平复位。当 rst 为 1 时，寄存器被复位。
    always @(posedge clk or negedge rst)
    含义： 只要 rst 信号从 1 变 0（下降沿），或者 clk 上升沿到来，就会触发逻辑。
    配合代码： 内部必须写成 if (!rst) 或 if (rst == 0)。
    结果： 这是低电平复位。当 rst 为 0 时，寄存器被复位。通常这种信号命名会带后缀，如 rst_n。
*/
