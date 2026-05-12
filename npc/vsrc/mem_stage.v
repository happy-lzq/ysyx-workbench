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
    input   wire [31:0] mem_wdata_raw ; 
    output  wire [31:0] mem_rdata ;

    parameter MMEM_SIZE = 65536;
    reg  [31:0] mem [0:MMEM_SIZE-1];
    wire [3 :0] mem_wmask ; 
    wire [31:0] mem_rdata_raw, mem_wdata;
    wire [15:0] mem_idx = mem_addr[17:2]; // 字节地址 ——> 字地址
    // Load
    assign mem_rdata_raw = mem[mem_idx];
    // Store 
    always @(posedge clk ) begin
        if(mem_write) begin
            if(mem_wmask[0]) mem[mem_idx][7 :0 ] <= mem_wdata[7 :0 ];
            if(mem_wmask[1]) mem[mem_idx][15:8 ] <= mem_wdata[15:8 ];
            if(mem_wmask[2]) mem[mem_idx][23:16] <= mem_wdata[23:16];
            if(mem_wmask[3]) mem[mem_idx][31:24] <= mem_wdata[31:24];
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

