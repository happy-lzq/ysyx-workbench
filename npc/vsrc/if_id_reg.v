`include "ctrl_defs.vh"
module if_id_reg (
    clk
    ,rst
    ,if_id_valid_in
    ,if_id_pc_in
    ,if_id_pc_plus4_in
    ,if_id_inst_in
    ,if_id_valid
    ,if_id_pc
    ,if_id_pc_plus4
    ,if_id_inst
);
    input wire          clk,rst;
    input wire          if_id_valid_in;
    input wire  [31:0]  if_id_pc_in;
    input wire  [31:0]  if_id_pc_plus4_in;
    input wire  [31:0]  if_id_inst_in;

    output reg          if_id_valid;
    output reg  [31:0]  if_id_pc;
    output reg  [31:0]  if_id_pc_plus4;
    output reg  [31:0]  if_id_inst;

    always @(posedge clk) begin
        if (rst) begin
            if_id_valid    <= 1'b0;            // 复位时无效
            if_id_pc       <= `RESET_VECTOR;
            if_id_pc_plus4 <= 32'h8000_0004;
            if_id_inst     <= 32'h0000_0013;   // nop            
        end else begin
            if_id_valid    <= if_id_valid_in;           
            if_id_pc       <= if_id_pc_in;
            if_id_pc_plus4 <= if_id_pc_plus4_in;
            if_id_inst     <= if_id_inst_in;           
        end
    end
    
endmodule