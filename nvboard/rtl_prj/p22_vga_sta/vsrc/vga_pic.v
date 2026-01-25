/* verilator lint_off UNUSEDSIGNAL */
module vga_pic(
    input clk,               
    input [9:0] h_addr,
    input [9:0] v_addr,
    output reg [23:0] vga_data 
);
    
    reg [23:0] vga_mem [524287:0];
    initial begin
        $readmemh("/home/l/ysyx/ysyx-workbench/nvboard/rtl_prj/p22_vga_sta/resource/picture.hex", vga_mem);
    end

    wire [18:0] ram_addr = {v_addr[8:0], h_addr};
    always @(posedge clk) begin
        vga_data <= vga_mem[ram_addr];
    end

endmodule
/* verilator lint_on UNUSEDSIGNAL */
