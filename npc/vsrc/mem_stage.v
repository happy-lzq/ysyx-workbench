`include "dpi_imports.vh"

module mem_stage (
    clk
    ,lsu_type
    ,mem_read
    ,mem_write
    ,mem_addr
    ,mem_wdata_raw
    ,mem_rdata
);
    input   wire [0 :0] clk,mem_read,mem_write;
    input   wire [2 :0] lsu_type;
    input   wire [31:0] mem_addr;        
    input   wire [31:0] mem_wdata_raw; 
    output  wire [31:0] mem_rdata ;

    wire [3 :0] mem_wmask; 
    wire [31:0] mem_rdata_raw, mem_wdata;

    // 读：组合逻辑内通过 DPI-C 从 C++ 统一内存读取
    assign mem_rdata_raw = dpi_mem_read(mem_addr);

    // 写：时序逻辑内通过 DPI-C 写入 C++ 统一内存
    always @(posedge clk) begin
        if (mem_write) begin
            dpi_mem_write(mem_addr, mem_wdata, {28'b0, mem_wmask});
        end
    end

    lsu lsu_pic(
     .mem_addr          (mem_addr       )      
    ,.mem_rdata_raw     (mem_rdata_raw  )     
    ,.mem_wdata_raw     (mem_wdata_raw  )     
    ,.lsu_type          (lsu_type       )     
    ,.mem_read          (mem_read       )     
    ,.mem_write         (mem_write      )     
    ,.mem_rdata         (mem_rdata      )     
    ,.mem_wdata         (mem_wdata      )     
    ,.mem_wmask         (mem_wmask      )
    ); 
endmodule
