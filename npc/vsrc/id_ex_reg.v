`include "ctrl_defs.vh"
module id_ex_reg (
    clk
    ,rst
    ,id_ex_valid_in
    ,id_ex_pc_in
    ,id_ex_pc_plus4_in
    ,id_ex_inst_in
    ,id_ex_rs1_addr_in
    ,id_ex_rs2_addr_in
    ,id_ex_rd_addr_in
    ,id_ex_rs1_rdata_in
    ,id_ex_rs2_rdata_in
    ,id_ex_imm_out_in
    ,id_ex_ex_ctrl_in
    ,id_ex_mem_ctrl_in
    ,id_ex_wb_ctrl_in
    ,id_ex_csr_ctrl_in
    ,id_ex_sys_ctrl_in
    ,id_ex_pc_ctrl_in
    ,id_ex_valid
    ,id_ex_pc
    ,id_ex_pc_plus4
    ,id_ex_inst
    ,id_ex_rs1_addr
    ,id_ex_rs2_addr
    ,id_ex_rd_addr
    ,id_ex_rs1_rdata
    ,id_ex_rs2_rdata
    ,id_ex_imm_out
    ,id_ex_ex_ctrl
    ,id_ex_mem_ctrl
    ,id_ex_wb_ctrl
    ,id_ex_csr_ctrl
    ,id_ex_sys_ctrl
    ,id_ex_pc_ctrl

);
    input wire                          clk;
    input wire                          rst;
    input wire                          id_ex_valid_in;
    input wire  [31:0]                  id_ex_pc_in;
    input wire  [31:0]                  id_ex_pc_plus4_in;
    input wire  [31:0]                  id_ex_inst_in;
    input wire  [4 :0]                  id_ex_rs1_addr_in;
    input wire  [4 :0]                  id_ex_rs2_addr_in;
    input wire  [4 :0]                  id_ex_rd_addr_in;
    input wire  [31:0]                  id_ex_rs1_rdata_in;
    input wire  [31:0]                  id_ex_rs2_rdata_in;
    input wire  [31:0]                  id_ex_imm_out_in;
    input wire  [`EX_CTRL_WIDTH-1 :0]   id_ex_ex_ctrl_in;
    input wire  [`MEM_CTRL_WIDTH-1:0]   id_ex_mem_ctrl_in;
    input wire  [`WB_CTRL_WIDTH-1 :0]   id_ex_wb_ctrl_in;
    input wire  [`CSR_CTRL_WIDTH-1:0]   id_ex_csr_ctrl_in;
    input wire  [`SYS_CTRL_WIDTH-1:0]   id_ex_sys_ctrl_in;
    input wire  [`PC_CTRL_WIDTH-1:0]    id_ex_pc_ctrl_in;

    output reg                          id_ex_valid     ;
    output reg  [31:0]                  id_ex_pc ;
    output reg  [31:0]                  id_ex_pc_plus4 ;
    output reg  [31:0]                  id_ex_inst  ;
    output reg  [4 :0]                  id_ex_rs1_addr ;
    output reg  [4 :0]                  id_ex_rs2_addr ;
    output reg  [4 :0]                  id_ex_rd_addr ;
    output reg  [31:0]                  id_ex_rs1_rdata ;
    output reg  [31:0]                  id_ex_rs2_rdata ;
    output reg  [31:0]                  id_ex_imm_out ;
    output reg  [`EX_CTRL_WIDTH-1 :0]   id_ex_ex_ctrl;
    output reg  [`MEM_CTRL_WIDTH-1:0]   id_ex_mem_ctrl;
    output reg  [`WB_CTRL_WIDTH-1 :0]   id_ex_wb_ctrl;
    output reg  [`CSR_CTRL_WIDTH-1:0]   id_ex_csr_ctrl;
    output reg  [`SYS_CTRL_WIDTH-1:0]   id_ex_sys_ctrl;
    output reg  [`PC_CTRL_WIDTH-1:0]    id_ex_pc_ctrl;

    always @(posedge clk) begin
        if (rst) begin
            id_ex_valid     <= 1'b0;
            id_ex_pc        <= `RESET_VECTOR;
            id_ex_pc_plus4  <= `RESET_VECTOR + 32'd4;
            id_ex_inst      <= 32'h0000_0013;   // nop
            id_ex_rs1_addr  <= 5'b0;
            id_ex_rs2_addr  <= 5'b0;
            id_ex_rd_addr   <= 5'b0;
            id_ex_rs1_rdata <= 32'b0;
            id_ex_rs2_rdata <= 32'b0;
            id_ex_imm_out   <= 32'b0;
            id_ex_ex_ctrl   <= {`EX_CTRL_WIDTH{1'b0}};
            id_ex_mem_ctrl  <= {`MEM_CTRL_WIDTH{1'b0}};
            id_ex_wb_ctrl   <= {`WB_CTRL_WIDTH{1'b0}};
            id_ex_csr_ctrl  <= {`CSR_CTRL_WIDTH{1'b0}};
            id_ex_sys_ctrl  <= {`SYS_CTRL_WIDTH{1'b0}};
            id_ex_pc_ctrl   <= {`PC_CTRL_WIDTH{1'b0}};
        end else begin
            id_ex_valid     <= id_ex_valid_in;
            id_ex_pc        <= id_ex_pc_in;
            id_ex_pc_plus4  <= id_ex_pc_plus4_in;
            id_ex_inst      <= id_ex_inst_in;
            id_ex_rs1_addr  <= id_ex_rs1_addr_in;
            id_ex_rs2_addr  <= id_ex_rs2_addr_in;
            id_ex_rd_addr   <= id_ex_rd_addr_in;
            id_ex_rs1_rdata <= id_ex_rs1_rdata_in;
            id_ex_rs2_rdata <= id_ex_rs2_rdata_in;
            id_ex_imm_out   <= id_ex_imm_out_in;
            id_ex_ex_ctrl   <= id_ex_ex_ctrl_in;
            id_ex_mem_ctrl  <= id_ex_mem_ctrl_in;
            id_ex_wb_ctrl   <= id_ex_wb_ctrl_in;
            id_ex_csr_ctrl  <= id_ex_csr_ctrl_in;
            id_ex_sys_ctrl  <= id_ex_sys_ctrl_in;
            id_ex_pc_ctrl   <= id_ex_pc_ctrl_in;
        end
    end


endmodule
