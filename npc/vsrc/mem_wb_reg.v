`include "ctrl_defs.vh"
module mem_wb_reg (
    clk
    ,rst
    ,mem_wb_valid_in
    ,mem_wb_pc_in
    ,mem_wb_pc_plus4_in
    ,mem_wb_inst_in
    ,mem_wb_rd_addr_in
    ,mem_wb_alu_result_in
    ,mem_wb_mem_rdata_in
    ,mem_wb_wb_ctrl_in
    ,mem_wb_csr_rdata_in
    ,mem_wb_csr_wdata_in
    ,mem_wb_csr_ctrl_in
    ,mem_wb_sys_ctrl_in
    ,mem_wb_valid
    ,mem_wb_pc
    ,mem_wb_pc_plus4
    ,mem_wb_inst
    ,mem_wb_rd_addr
    ,mem_wb_alu_result
    ,mem_wb_mem_rdata
    ,mem_wb_wb_ctrl
    ,mem_wb_csr_rdata
    ,mem_wb_csr_wdata
    ,mem_wb_csr_ctrl
    ,mem_wb_sys_ctrl

);
    input wire                          clk;
    input wire                          rst;
    input wire                          mem_wb_valid_in;
    input wire  [31:0]                  mem_wb_pc_in;
    input wire  [31:0]                  mem_wb_pc_plus4_in;
    input wire  [31:0]                  mem_wb_inst_in;
    input wire  [4 :0]                  mem_wb_rd_addr_in;
    input wire  [31:0]                  mem_wb_alu_result_in;
    input wire  [31:0]                  mem_wb_mem_rdata_in;
    input wire  [`WB_CTRL_WIDTH-1 :0]   mem_wb_wb_ctrl_in;
    input wire  [31:0]                  mem_wb_csr_rdata_in;
    input wire  [31:0]                  mem_wb_csr_wdata_in;
    input wire  [`CSR_CTRL_WIDTH-1:0]   mem_wb_csr_ctrl_in;
    input wire  [`SYS_CTRL_WIDTH-1:0]   mem_wb_sys_ctrl_in;

    output reg                          mem_wb_valid     ;
    output reg  [31:0]                  mem_wb_pc ;
    output reg  [31:0]                  mem_wb_pc_plus4 ;
    output reg  [31:0]                  mem_wb_inst  ;
    output reg  [4 :0]                  mem_wb_rd_addr ;
    output reg  [31:0]                  mem_wb_alu_result ;
    output reg  [31:0]                  mem_wb_mem_rdata ;
    output reg  [`WB_CTRL_WIDTH-1 :0]   mem_wb_wb_ctrl;
    output reg  [31:0]                  mem_wb_csr_rdata ;
    output reg  [31:0]                  mem_wb_csr_wdata ;
    output reg  [`CSR_CTRL_WIDTH-1:0]   mem_wb_csr_ctrl;
    output reg  [`SYS_CTRL_WIDTH-1:0]   mem_wb_sys_ctrl;

    always @(posedge clk) begin
        if (rst) begin
            mem_wb_valid      <= 1'b0;
            mem_wb_pc         <= `RESET_VECTOR;
            mem_wb_pc_plus4   <= `RESET_VECTOR + 32'd4;
            mem_wb_inst       <= 32'h0000_0013;   // nop
            mem_wb_rd_addr    <= 5'b0;
            mem_wb_alu_result <= 32'b0;
            mem_wb_mem_rdata  <= 32'b0;
            mem_wb_wb_ctrl    <= {`WB_CTRL_WIDTH{1'b0}};
            mem_wb_csr_rdata  <= 32'b0;
            mem_wb_csr_wdata  <= 32'b0;
            mem_wb_csr_ctrl   <= {`CSR_CTRL_WIDTH{1'b0}};
            mem_wb_sys_ctrl   <= {`SYS_CTRL_WIDTH{1'b0}};
        end else begin
            mem_wb_valid      <= mem_wb_valid_in;
            mem_wb_pc         <= mem_wb_pc_in;
            mem_wb_pc_plus4   <= mem_wb_pc_plus4_in;
            mem_wb_inst       <= mem_wb_inst_in;
            mem_wb_rd_addr    <= mem_wb_rd_addr_in;
            mem_wb_alu_result <= mem_wb_alu_result_in;
            mem_wb_mem_rdata  <= mem_wb_mem_rdata_in;
            mem_wb_wb_ctrl    <= mem_wb_wb_ctrl_in;
            mem_wb_csr_rdata  <= mem_wb_csr_rdata_in;
            mem_wb_csr_wdata  <= mem_wb_csr_wdata_in;
            mem_wb_csr_ctrl   <= mem_wb_csr_ctrl_in;
            mem_wb_sys_ctrl   <= mem_wb_sys_ctrl_in;
        end
    end


endmodule
