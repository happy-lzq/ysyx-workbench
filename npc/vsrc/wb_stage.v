module wb_stage (
    reg_wdata_src
    ,mem_rdata
    ,alu_result
    ,pc_plus4
    ,csr_rdata
    ,reg_wdata
);

    input  wire [1:0]  reg_wdata_src;    
    input  wire [31:0] mem_rdata    ;    
    input  wire [31:0] alu_result   ;        
    input  wire [31:0] pc_plus4     ;
    input  wire [31:0] csr_rdata    ;     
    output wire [31:0] reg_wdata    ; 

    assign reg_wdata = (reg_wdata_src == 2'b00) ? alu_result :
                       (reg_wdata_src == 2'b01) ? mem_rdata  : 
                       (reg_wdata_src == 2'b10) ? pc_plus4   : csr_rdata ;

endmodule
