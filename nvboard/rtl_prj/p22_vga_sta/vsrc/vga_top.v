module vga_top(
    input clk,
    input rst,
    output VGA_HSYNC,
    output VGA_VSYNC,
    output VGA_BLANK_N,
    output [7:0] VGA_R,
    output [7:0] VGA_G,
    output [7:0] VGA_B
);
    wire [9:0] h_addr;
    wire [9:0] v_addr;
    wire [23:0] vga_data;
vga_ctrl u_vga_ctrl (
    .vga_clk      (clk),
    .sys_rst_n    (~rst),
    .pix_data     (vga_data),
    .pix_x        (h_addr),
    .pix_y        (v_addr),
    .hsync        (VGA_HSYNC),
    .vsync        (VGA_VSYNC),
    .rgb_r        (VGA_R),
    .rgb_g        (VGA_G),
    .rgb_b        (VGA_B),
    .rgb_valid    (VGA_BLANK_N)
);

vga_pic u_vga_pic (
    .clk         (clk   ),
    .h_addr      (h_addr),
    .v_addr      (v_addr),
    .vga_data    (vga_data)
);
endmodule
