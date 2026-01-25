module vga_pic(
    input [9:0] h_addr,
    input [8:0] v_addr,
    output [23:0] vga_data
);

reg [23:0] vga_mem [524287:0];

initial begin
    $readmemh("/home/l/ysyx/ysyx-workbench/nvboard/rtl_prj/p21_vga/resource/picture.hex", vga_mem);
end

assign vga_data = vga_mem[{v_addr, h_addr}];  //行优先读取方式

endmodule

/*
$readmem读取方式：
    // 语法格式
$readmemh("文件名", 存储器数组 [, 起始地址 [, 结束地址]]);  // 十六进制
$readmemb("文件名", 存储器数组 [, 起始地址 [, 结束地址]]);  // 二进制

*/
