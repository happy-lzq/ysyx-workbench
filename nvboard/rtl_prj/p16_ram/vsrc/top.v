module top #(
    parameter DATA_WIDTH = 8,
    parameter ADDR_WIDTH = 4
)
(
     clk            // << i
    ,we             // << i
    ,w_addr         // << i
    ,r_addr         // << i
    ,w_data         // << i
    ,r_data         // >> o
    ,seg0           // >> o
    ,seg1           // >> o
    ,seg2           // >> o
    ,seg3           // >> o
    ,seg4           // >> o
    ,seg5           // >> o
    ,seg6           // >> o
    ,seg7           // >> o
);
    input                        clk,we,w_addr,r_addr,w_data;
    output                                            r_data;
    output   [7:0]   seg0,seg1,seg2,seg3,seg4,seg5,seg6,seg7;
                      
    wire                 [ADDR_WIDTH-1:0]             w_addr;
    wire                 [DATA_WIDTH-1:0]             w_data;
    wire                 [ADDR_WIDTH-1:0]             r_addr;
    wire                 [DATA_WIDTH-1:0]             r_data;  // 中间连接seg_show的线,并接入led输出

    ram my_ram(
         .clk       (clk    ) 
        ,.we        (we     )
        ,.w_addr    (w_addr )
        ,.r_addr    (r_addr )
        ,.w_data    (w_data )
        ,.r_data    (r_data )
    );

    seg_show my_seg_show(
         .seg_in    (r_data)
        ,.seg0      (seg0  )
        ,.seg1      (seg1  )
        ,.seg2      (seg2  )
        ,.seg3      (seg3  )
        ,.seg4      (seg4  )
        ,.seg5      (seg5  )
        ,.seg6      (seg6  )
        ,.seg7      (seg7  )
    );
endmodule
