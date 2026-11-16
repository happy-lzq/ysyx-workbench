`include "ctrl_defs.vh"
module if_id_reg (
    clk
    ,rst
    ,enable
    ,flush
    ,if_id_valid_in
    ,if_id_pc_in
    ,if_id_pc_plus4_in
    ,if_id_inst_in
    ,if_id_exc_valid_in
    ,if_id_exc_cause_in
    ,if_id_exc_tval_in
    ,if_id_valid
    ,if_id_pc
    ,if_id_pc_plus4
    ,if_id_inst
    ,if_id_exc_valid
    ,if_id_exc_cause
    ,if_id_exc_tval

);
    input  wire        clk;
    input  wire        rst;
    input  wire        enable;
    input  wire        flush;
    input  wire        if_id_valid_in;
    input  wire [31:0] if_id_pc_in;
    input  wire [31:0] if_id_pc_plus4_in;
    input  wire [31:0] if_id_inst_in;
    input  wire        if_id_exc_valid_in;
    input  wire [31:0] if_id_exc_cause_in;
    input  wire [31:0] if_id_exc_tval_in;


    output reg         if_id_valid;
    output reg  [31:0] if_id_pc;
    output reg  [31:0] if_id_pc_plus4;
    output reg  [31:0] if_id_inst;
    output reg         if_id_exc_valid;
    output reg  [31:0] if_id_exc_cause;
    output reg  [31:0] if_id_exc_tval;


    // enable 用于代表
    // if_id_flush 仅仅代表xxx 阶段有一条有效指令
    always @(posedge clk) begin
        if (rst) begin
            if_id_valid     <= 1'b0;            // 复位时无效
            if_id_pc        <= `RESET_VECTOR;
            if_id_pc_plus4  <= 32'h8000_0004;
            if_id_inst      <= 32'h0000_0013;   // nop     
            if_id_exc_valid <= 1'b0;
            if_id_exc_cause <= 32'b0;
            if_id_exc_tval  <= 32'b0;

    // flush 冲刷当前指令视为无效      
        end else if (flush) begin
            if_id_valid     <= 1'b0;
            if_id_exc_valid <= 1'b0;
        end else if (enable) begin
            if_id_valid     <= if_id_valid_in;           
            if_id_pc        <= if_id_pc_in;
            if_id_pc_plus4  <= if_id_pc_plus4_in;
            if_id_inst      <= if_id_inst_in; 
            if_id_exc_valid <= if_id_exc_valid_in;          
            if_id_exc_cause <= if_id_exc_cause_in;
            if_id_exc_tval  <= if_id_exc_tval_in;
        end
    end
    
endmodule