module ram #(
    parameter DATA_WIDTH = 8,
    parameter ADDR_WIDTH = 4
)(
    clk             // << i
    ,we             // << i
    ,w_addr         // << i
    ,r_addr         // << i
    ,w_data         // << i
    ,r_data         // >> o
);
    input           clk,we,w_addr,r_addr,w_data;
    output                               r_data;
    wire                                 clk,we;
    wire    [ADDR_WIDTH-1:0]             w_addr;
    wire    [DATA_WIDTH-1:0]             w_data;
    wire    [ADDR_WIDTH-1:0]             r_addr;
    wire     [DATA_WIDTH-1:0]            r_data;    
    // 定义寄存器堆  数据位宽=8，地址位宽=4，总容量=8*16=128位
    reg [DATA_WIDTH-1:0] ram [(2**ADDR_WIDTH-1):0];
    always @(posedge clk ) begin
        if (we) begin
            ram[w_addr] <= w_data;
        end 
    end
    assign r_data = ram[r_addr]; 
endmodule
