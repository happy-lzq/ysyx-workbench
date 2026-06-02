`include "dpi_imports.vh"

module mem_stage (
    clk
    ,lsu_type
    ,mem_read
    ,mem_write
    ,mem_addr
    ,mem_wdata_raw
    ,mem_rdata
    ,dev_req  
    ,dev_r_w  
    ,dev_addr 
    ,dev_wdata
    ,dev_wmask
);
    input   wire [0 :0] clk,mem_read,mem_write;
    input   wire [2 :0] lsu_type;
    input   wire [31:0] mem_addr;        
    input   wire [31:0] mem_wdata_raw; 
    output  wire [31:0] mem_rdata ;

// ======================= 外设分发信号 =================
    output wire [0 :0] dev_req ;
    output wire [0 :0] dev_r_w ;
    output wire [31:0] dev_addr;
    output wire [31:0] dev_wdata;
    output wire [3 :0] dev_wmask;

    parameter DATA_ADDR_START = 32'h80000000;
    parameter DATA_ADDR_END   = 32'h88000000;
    parameter PMEM_BASE = 32'h8000_0000;
    parameter MMEM_SIZE = 33554432;  // 128MB / 4B
    reg  [31:0] dmem [0:MMEM_SIZE-1];
    wire [3 :0] mem_wmask ; 
    wire [31:0] mem_rdata_raw, mem_wdata,dev_rdata;
    wire [31:0] mem_off = mem_addr - PMEM_BASE;
    wire [24:0] mem_idx = mem_off[26:2]; // 字节地址 -> 字地址

    wire in_pmem = (mem_addr >= DATA_ADDR_START) && (mem_addr < DATA_ADDR_END);
    assign dev_req   = (mem_read | mem_write) && !in_pmem;
    assign dev_r_w   = mem_write;                      // 1=写, 0=读
    assign dev_addr  = mem_addr;
    assign dev_wdata = mem_wdata_raw;                  // 直连，不用 reg
    assign dev_wmask = mem_wmask;

        // Load
    assign dev_rdata = dpi_mmio_read(mem_addr);
    assign mem_rdata_raw = in_pmem ? dmem[mem_idx] : dev_rdata;
    // Store 
    always @(posedge clk ) begin
        if (mem_write && in_pmem) begin
            if(mem_wmask[0]) dmem[mem_idx][7 :0 ] <= mem_wdata[7 :0 ];
            if(mem_wmask[1]) dmem[mem_idx][15:8 ] <= mem_wdata[15:8 ];
            if(mem_wmask[2]) dmem[mem_idx][23:16] <= mem_wdata[23:16];
            if(mem_wmask[3]) dmem[mem_idx][31:24] <= mem_wdata[31:24];
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
