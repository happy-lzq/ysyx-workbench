`include "ctrl_defs.vh"

module mem_stage (
    mem_ctrl
    ,mem_valid
    ,mem_addr
    ,mem_wdata_raw
    ,mem_rdata_raw
    ,mem_rdata
    ,mem_wdata
    ,mem_wmask
    ,mem_is_mmio
    ,mem_read
    ,mem_write
    ,ram_fault
    ,device_fault
    ,local_range_fault
);
    parameter PMEM_BASE     = 32'h8000_0000;
    parameter PMEM_END      = 32'h8800_0000;
    parameter DEVICE_BASE   = 32'ha000_0000;
    parameter DEVICE_END    = 32'ha200_0000;
    input  wire [`MEM_CTRL_WIDTH-1:0] mem_ctrl;
    input  wire                       mem_valid;
    input  wire [31:0]                mem_addr;
    input  wire [31:0]                mem_wdata_raw;
    // 统一端口读取的原始 32 位数据。
    input  wire [31:0]                mem_rdata_raw;

    // LSU 处理后的 Load 结果，送往 MEM/WB。
    output wire [31:0]                mem_rdata;
    // LSU 处理后的 Store 数据及字节写掩码，送往统一端口。
    output wire [31:0]                mem_wdata;
    output wire [3:0]                 mem_wmask;
    output wire                       mem_is_mmio;
    // 向共享端口输出本阶段的有效读写需求。
    output wire                       mem_read;
    output wire                       mem_write;
    output wire ram_fault;
    output wire device_fault;
    output wire local_range_fault;

    assign mem_read = mem_valid && mem_ctrl[`MEM_CTRL_READ];
    assign mem_write = mem_valid && mem_ctrl[`MEM_CTRL_WRITE];
    wire   mem_active     = mem_read || mem_write;
    wire [2:0] lsu_type = mem_ctrl[`MEM_CTRL_LSU_MSB:`MEM_CTRL_LSU_LSB];  
    wire addr_is_ram    = (mem_addr >= PMEM_BASE) && (mem_addr <  PMEM_END);
    wire addr_is_device = (mem_addr >= DEVICE_BASE) && (mem_addr <  DEVICE_END);  
    assign mem_is_mmio  = mem_active && addr_is_device;

// ===================== 访问地址位宽对齐处理 ===============================

    wire access_half = (lsu_type == 3'b001) ||   // LH
                       (lsu_type == 3'b011) ||   // LHU
                       (lsu_type == 3'b110);     // SH

    wire access_word = (lsu_type == 3'b000) ||   // LW
                       (lsu_type == 3'b101);     // SW

    wire addr_misaligned = mem_active &&
                           ((access_half && mem_addr[0]) || 
                            (access_word && (mem_addr[1:0] != 2'b00)));

//  ==================== 访问 RAM 区域属性检查处理 ===============================
    wire [32:0] access_bytes = access_word ? 33'd4 :
                               access_half ? 33'd2 : 33'd1;
    wire [32:0] access_last  = {1'b0,mem_addr} + access_bytes -33'd1;
    wire access_in_ram       = (mem_addr >= PMEM_BASE)   && (access_last < {1'b0,PMEM_END});
    wire access_in_device    = (mem_addr >= DEVICE_BASE) && (access_last < {1'b0,DEVICE_END});


    wire device_align_fault  = mem_is_mmio && addr_misaligned;      // device 访问地址未对齐
    wire device_window_fault = mem_is_mmio && !access_in_device;    //        访问超出device地址
    // 三类内存地址访问错误输出
    // 第一类：RAM 地址未对齐错误
    wire ram_range_fault = mem_active  && 
                           addr_is_ram && 
                           !access_in_ram;

    wire ram_align_fault = mem_active  && 
                           addr_is_ram && 
                           access_in_ram && 
                           addr_misaligned;
    assign ram_fault = ram_range_fault || ram_align_fault;

    // 第二类：Device 本地错误
    assign device_fault = device_align_fault || device_window_fault;

    // 第三类：不属于RAM+device 范围内的非法错误地址访问
    assign local_range_fault   = mem_active && !(access_in_device || access_in_ram);
    
    lsu lsu_pic (
        .mem_addr      (mem_addr     ),    // << i
        .mem_rdata_raw (mem_rdata_raw),    // << i
        .mem_wdata_raw (mem_wdata_raw),    // << i
        .lsu_type      (lsu_type     ),    // << i
        .mem_read      (mem_read     ),    // << i
        .mem_write     (mem_write    ),    // << i

        .mem_rdata     (mem_rdata    ),    // >> o
        .mem_wdata     (mem_wdata    ),    // >> o
        .mem_wmask     (mem_wmask    )     // >> o
    );

endmodule


/*
访问宽度	    指令	            lsu_type
1 字节	        LB、LBU、SB	        010、100、111
2 字节	        LH、LHU、SH	        001、011、110
4 字节	        LW、SW	            000、101
*/
