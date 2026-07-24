`include "ctrl_defs.vh"
module ex_mem_reg (
    clk
    ,rst
    ,ex_mem_valid_in
    ,ex_mem_pc_in
    ,ex_mem_pc_plus4_in
    ,ex_mem_inst_in
    ,ex_mem_rd_addr_in
    ,ex_mem_alu_result_in
    ,ex_mem_rs2_rdata_in
    ,ex_mem_mem_ctrl_in
    ,ex_mem_wb_ctrl_in
    ,ex_mem_csr_rdata_in
    ,ex_mem_csr_wdata_in
    ,ex_mem_csr_ctrl_in
    ,ex_mem_sys_ctrl_in
    ,ex_mem_valid
    ,ex_mem_pc
    ,ex_mem_pc_plus4
    ,ex_mem_inst
    ,ex_mem_rd_addr
    ,ex_mem_alu_result
    ,ex_mem_rs2_rdata
    ,ex_mem_mem_ctrl
    ,ex_mem_wb_ctrl
    ,ex_mem_csr_rdata
    ,ex_mem_csr_wdata
    ,ex_mem_csr_ctrl
    ,ex_mem_sys_ctrl

);
    input wire                          clk;
    input wire                          rst;
    input wire                          ex_mem_valid_in;
    input wire  [31:0]                  ex_mem_pc_in;
    input wire  [31:0]                  ex_mem_pc_plus4_in;
    input wire  [31:0]                  ex_mem_inst_in;
    input wire  [4 :0]                  ex_mem_rd_addr_in;
    input wire  [31:0]                  ex_mem_alu_result_in;
    input wire  [31:0]                  ex_mem_rs2_rdata_in;
    input wire  [31:0]                  ex_mem_csr_rdata_in;
    input wire  [31:0]                  ex_mem_csr_wdata_in;
    input wire  [`MEM_CTRL_WIDTH-1:0]   ex_mem_mem_ctrl_in;
    input wire  [`WB_CTRL_WIDTH-1 :0]   ex_mem_wb_ctrl_in;
    input wire  [`CSR_CTRL_WIDTH-1:0]   ex_mem_csr_ctrl_in;
    input wire  [`SYS_CTRL_WIDTH-1:0]   ex_mem_sys_ctrl_in;

    output reg                          ex_mem_valid     ;
    output reg  [31:0]                  ex_mem_pc ;
    output reg  [31:0]                  ex_mem_pc_plus4 ;
    output reg  [31:0]                  ex_mem_inst  ;
    output reg  [4 :0]                  ex_mem_rd_addr ;
    output reg  [31:0]                  ex_mem_alu_result ;
    output reg  [31:0]                  ex_mem_rs2_rdata ;
    output reg  [31:0]                  ex_mem_csr_rdata ;
    output reg  [31:0]                  ex_mem_csr_wdata ;
    output reg  [`MEM_CTRL_WIDTH-1:0]   ex_mem_mem_ctrl;
    output reg  [`WB_CTRL_WIDTH-1 :0]   ex_mem_wb_ctrl;
    output reg  [`CSR_CTRL_WIDTH-1:0]   ex_mem_csr_ctrl;
    output reg  [`SYS_CTRL_WIDTH-1:0]   ex_mem_sys_ctrl;

    always @(posedge clk) begin
        if (rst) begin
            ex_mem_valid      <= 1'b0;
            ex_mem_pc         <= `RESET_VECTOR;
            ex_mem_pc_plus4   <= `RESET_VECTOR + 32'd4;
            ex_mem_inst       <= 32'h0000_0013;   // nop
            ex_mem_rd_addr    <= 5'b0;
            ex_mem_alu_result <= 32'b0;
            ex_mem_rs2_rdata  <= 32'b0;
            ex_mem_mem_ctrl   <= {`MEM_CTRL_WIDTH{1'b0}};
            ex_mem_wb_ctrl    <= {`WB_CTRL_WIDTH{1'b0}};
            ex_mem_csr_rdata  <= 32'b0;
            ex_mem_csr_wdata  <= 32'b0;
            ex_mem_csr_ctrl   <= {`CSR_CTRL_WIDTH{1'b0}};
            ex_mem_sys_ctrl   <= {`SYS_CTRL_WIDTH{1'b0}};
        end else begin
            ex_mem_valid      <= ex_mem_valid_in;
            ex_mem_pc         <= ex_mem_pc_in;
            ex_mem_pc_plus4   <= ex_mem_pc_plus4_in;
            ex_mem_inst       <= ex_mem_inst_in;
            ex_mem_rd_addr    <= ex_mem_rd_addr_in;
            ex_mem_alu_result <= ex_mem_alu_result_in;
            ex_mem_rs2_rdata  <= ex_mem_rs2_rdata_in;
            ex_mem_mem_ctrl   <= ex_mem_mem_ctrl_in;
            ex_mem_wb_ctrl    <= ex_mem_wb_ctrl_in;
            ex_mem_csr_rdata  <= ex_mem_csr_rdata_in;
            ex_mem_csr_wdata  <= ex_mem_csr_wdata_in;
            ex_mem_csr_ctrl   <= ex_mem_csr_ctrl_in;
            ex_mem_sys_ctrl   <= ex_mem_sys_ctrl_in;
        end
    end


endmodule
