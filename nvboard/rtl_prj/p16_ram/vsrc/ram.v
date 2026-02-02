module ram #(
    parameter DATA_WIDTH = 8,
    parameter ADDR_WIDTH = 4,
    parameter DEPTH = 2**ADDR_WIDTH  // 明确深度参数
)(
    // 端口声明
    input                       clk,
    input                       we,
    input  [ADDR_WIDTH-1:0]     w_addr,
    input  [ADDR_WIDTH-1:0]     r_addr,
    input  [DATA_WIDTH-1:0]     w_data,
    output [DATA_WIDTH-1:0]     r_data
);
    
    // 定义寄存器堆
    reg [DATA_WIDTH-1:0] ram [0:DEPTH-1];  // 使用 0 起始索引更清晰
    
    // 可选：初始化 RAM（用于仿真）
    // initial begin
    //     $readmemh("ram_init.hex", ram);
    // end
    
    // 写操作
    always @(posedge clk) begin
        if (we) begin
            // 可选添加地址范围检查
            if (w_addr < DEPTH) begin
                ram[w_addr] <= w_data;
            end
        end
    end
    
    // 读操作（异步读）
    assign r_data = ram[r_addr];
/*
    如果要同步读，可以这样写：
    reg [DATA_WIDTH-1:0] r_data_reg;
    always @(posedge clk) begin
        r_data_reg <= ram[r_addr];
    end
    assign r_data = r_data_reg;
*/

endmodule

/*
存储阵列定义：二维数组
    reg [数据位宽-1 : 0] 数组名称 [存储深度-1 : 0];
    定义一个深度为 1024，位宽为 8 bit 的内存： 1KB 的存储器
    reg [7:0] my_memory [0:1023]; 

    定义一个 32 个寄存器，每个寄存器 32 bit 的寄存器堆
    常见于 MIPS/RISC-V 处理器
    reg [31:0] reg_file [0:31];
*/