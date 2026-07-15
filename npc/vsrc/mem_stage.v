`include "dpi_imports.vh"
`include "ctrl_defs.vh"
module mem_stage (
    clk
    ,mem_ctrl
    ,interrupt_valid
    ,mem_addr
    ,mem_wdata_raw
    ,mem_rdata
);
    // 控制流
    input   wire [0 :0] clk                     ;
    input   wire [0 :0] interrupt_valid         ;
    input   wire [`MEM_CTRL_WIDTH-1:0] mem_ctrl ;
    // 数据流
    input   wire [31:0] mem_addr                ;        
    input   wire [31:0] mem_wdata_raw           ; 
    output  wire [31:0] mem_rdata               ;
    // 控制总线拆包
    wire [0 :0] mem_read    = interrupt_valid ? 1'b0 : mem_ctrl[`MEM_CTRL_READ]                        ;
    wire [0 :0 ]mem_write   = interrupt_valid ? 1'b0 : mem_ctrl[`MEM_CTRL_WRITE]                       ;
    wire [2 :0] lsu_type    = mem_ctrl[`MEM_CTRL_LSU_MSB : `MEM_CTRL_LSU_LSB] ;
    // interrupt 门控
    wire [3 :0] mem_wmask; 
    wire [31:0] mem_rdata_raw, mem_wdata;

    // 读：组合逻辑内通过 DPI-C 从 C++ 统一内存读取；mem_read 门控 MMIO 副作用
    assign mem_rdata_raw = dpi_mem_read(mem_addr, {31'b0, mem_read});

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
