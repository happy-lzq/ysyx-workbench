`include "ctrl_defs.vh"

module if_stage (
    clk
    ,rst
    ,pc_enable
    ,redirect_fire
    ,redirect_target
    ,inst_rdata
    ,pc
    ,pc_plus4
    ,inst
);
    input  wire        clk;
    input  wire        rst;
    input  wire        pc_enable;
    input  wire        redirect_fire;
    input  wire [31:0] redirect_target;
    // 统一访存端口返回的指令数据。
    input  wire [31:0] inst_rdata;

    output reg  [31:0] pc;
    output wire [31:0] pc_plus4;
    output wire [31:0] inst;

    wire [31:0] pc_next;

    assign pc_next = redirect_fire ? redirect_target :
                                     pc + 32'd4;

    assign pc_plus4 = pc + 32'd4;

    // IF 不再直接读取内存，只传递统一端口返回的数据。
    assign inst = inst_rdata;

    always @(posedge clk) begin
        if (rst)
            pc <= `RESET_VECTOR;
        else if (pc_enable)
            pc <= pc_next;
    end

endmodule