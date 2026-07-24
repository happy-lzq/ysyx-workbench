`include "ctrl_defs.vh"
module wb_stage (
    wb_ctrl
    ,mem_rdata
    ,alu_result
    ,pc_plus4
    ,csr_rdata
    ,rd_wdata
);

    input  wire [`WB_CTRL_WIDTH-1 :0] wb_ctrl;
    input  wire [31:0] mem_rdata    ;    
    input  wire [31:0] alu_result   ;        
    input  wire [31:0] pc_plus4     ;
    input  wire [31:0] csr_rdata    ;     
    output wire [31:0] rd_wdata     ; 
    wire [1:0]  rd_wdata_src = wb_ctrl [`WB_CTRL_WDATA_SRC_MSB : `WB_CTRL_WDATA_SRC_LSB];  
    assign rd_wdata = (rd_wdata_src == 2'b00) ? alu_result :
                      (rd_wdata_src == 2'b01) ? mem_rdata  : 
                      (rd_wdata_src == 2'b10) ? pc_plus4   : csr_rdata ;

endmodule
