module regfile (
    clk
    ,rs1_addr
    ,rs2_addr
    ,rd_addr
    ,rd_wdata
    ,reg_write
    ,rs1_rdata
    ,rs2_rdata
    ,rs_a0
);
    input  wire [0 :0] clk;
    input  wire [0 :0] reg_write;
    input  wire [4 :0] rs1_addr;
    input  wire [4 :0] rs2_addr;
    input  wire [4 :0] rd_addr;
    input  wire [31:0] rd_wdata;
    output wire [31:0] rs1_rdata;
    output wire [31:0] rs2_rdata;
    output wire [31:0] rs_a0;


    reg [31:0] rf [0:31];
    assign rs_a0     = rf[10] ;
    assign rs1_rdata = (rs1_addr == 5'b0) ? 32'b0 : rf[rs1_addr];
    assign rs2_rdata = (rs2_addr == 5'b0) ? 32'b0 : rf[rs2_addr];

    always @(posedge clk) begin
        if (reg_write && rd_addr !=5'b0 ) begin
            rf[rd_addr] <= rd_wdata;
        end
    end

endmodule
