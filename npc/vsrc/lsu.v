module lsu (
    mem_addr                
    ,mem_rdata_raw          
    ,mem_wdata_raw          
    ,lsu_type               
    ,mem_read               
    ,mem_write              
    ,mem_rdata              
    ,mem_wdata              
    ,mem_wmask              
);
    input  wire [31:0] mem_addr;
    input  wire [31:0] mem_rdata_raw;
    input  wire [31:0] mem_wdata_raw;
    input  wire [2 :0] lsu_type;
    input  wire [0 :0] mem_read;
    input  wire [0 :0] mem_write; 
    output wire [31:0] mem_rdata;
    output wire [31:0] mem_wdata;
    output wire [3 :0] mem_wmask;
    // 取字/半字
    wire [1 :0] sel_byte     = mem_addr[1:0];
    wire [0 :0] sel_half     = mem_addr[1]  ;
    wire [7 :0] rdata_byte_0 = mem_rdata_raw[7 :0 ];
    wire [7 :0] rdata_byte_1 = mem_rdata_raw[15:8 ];
    wire [7 :0] rdata_byte_2 = mem_rdata_raw[23:16];
    wire [7 :0] rdata_byte_3 = mem_rdata_raw[31:24];

    wire [15:0] rdata_half_0 = mem_rdata_raw[15:0 ];
    wire [15:0] rdata_half_1 = mem_rdata_raw[31:16];

    wire [7 :0] byte_selected = (sel_byte == 2'b00) ? rdata_byte_0 : 
                                (sel_byte == 2'b01) ? rdata_byte_1 :
                                (sel_byte == 2'b10) ? rdata_byte_2 : rdata_byte_3;
    wire [15:0] half_selected = (sel_half == 1'b0) ? rdata_half_0 : rdata_half_1;

    // Load
    wire is_lw  = (lsu_type == 3'b000);
    wire is_lh  = (lsu_type == 3'b001);
    wire is_lb  = (lsu_type == 3'b010);
    wire is_lhu = (lsu_type == 3'b011);
    wire is_lbu = (lsu_type == 3'b100);
    wire is_sw  = (lsu_type == 3'b101);
    wire is_sh  = (lsu_type == 3'b110);
    wire is_sb  = (lsu_type == 3'b111);
    // 扩展 
    wire [31:0] lh_result  = {{16{half_selected[15]}}, half_selected};
    wire [31:0] lhu_result = {16'b0,half_selected};
    wire [31:0] lb_result  = {{24{byte_selected[7]}} , byte_selected};
    wire [31:0] lbu_result = {24'b0, byte_selected};
    assign mem_rdata = mem_read ? ( is_lw  ? mem_rdata_raw :
                                    is_lh  ? lh_result     :
                                    is_lb  ? lb_result     :
                                    is_lhu ? lhu_result    :
                                    is_lbu ? lbu_result    : 32'b0 ) : 32'b0;
    // Store
    wire [31:0] sw_wdata = mem_wdata_raw;
    wire [31:0] sh_wdata = {2{mem_wdata_raw[15:0]}};
    wire [31:0] sb_wdata = {4{mem_wdata_raw[ 7:0]}};

    wire [ 3:0] sw_mask = 4'b1111;
    wire [ 3:0] sh_mask = sel_half ? 4'b1100 : 4'b0011;
    wire [ 3:0] sb_mask = 4'b0001 << sel_byte; 

    assign mem_wmask = mem_write ? (is_sw ? sw_mask  :
                                    is_sh ? sh_mask  : sb_mask) : 4'b0000;
    assign mem_wdata = mem_write ? (is_sw ? sw_wdata : 
                                    is_sh ? sh_wdata : sb_wdata): 32'b0  ; 
 
endmodule 
