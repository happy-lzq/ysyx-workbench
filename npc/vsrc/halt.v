module halt (
    clk
    ,rst
    ,is_ebreak
    ,pc
    ,rs_a0
    ,halt_reg
    ,halt_pc
    ,halt_ret
);
    parameter RESET_VECTOR = 32'h8000_0000;
    parameter RESET_x10    = 32'b0;
    input  wire [0 :0] clk;
    input  wire [0 :0] rst;
    input  wire [0 :0] is_ebreak;
    input  wire [31:0] pc;
    input  wire [31:0] rs_a0;
    output reg  [0 :0] halt_reg;
    output reg  [31:0] halt_pc;
    output reg  [31:0] halt_ret;

    always @(posedge clk) begin
        if (rst) begin
            halt_reg <= 0;
            halt_pc  <= RESET_VECTOR;
            halt_ret <= RESET_x10;
        end else begin
            if (is_ebreak) begin
                halt_reg <= 1'b1;
                halt_pc  <= pc;
                halt_ret <= rs_a0;
            end
        end
    end
endmodule
