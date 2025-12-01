module vga_top(
     clk
    ,sys_rst_n
    ,hsync
    ,vsync
    ,rgb
);
    input   wire   clk, sys_rst_n;
    output  wire  hsync,vsync;
    output  wire  [15:0]  rgb;

    wire        vga_clk,rst_n;
    wire    [9:0] pix_x,pix_y;
    wire    [15:0]   pix_data;
    wire               locked;
    
    assign rst_n = sys_rst_n && locked;

    pll_div2 u_pll_div2 (
	 .areset   (!sys_rst_n)
    ,.inclk0   (clk)
    ,.c0       (vga_clk)
    ,.locked   (locked)
);



    vga_pic u_vga_pic (
    .vga_clk     (vga_clk),
    .rst_n       (rst_n),
    .pix_x       (pix_x),
    .pix_y       (pix_y),
    .pix_data    (pix_data)
);

    vga_ctrl u_vga_ctrl (
    .vga_clk     (vga_clk),
    .rst_n       (rst_n),
    .pix_data    (pix_data),
    .pix_x       (pix_x),
    .pix_y       (pix_y),
    .hsync       (hsync),
    .vsync       (vsync),
    .rgb         (rgb)
);

endmodule