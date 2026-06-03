`include "../include/dpi_imports.vh"
module if_stage (
    clk
    ,rst
    ,trap_enter
    ,trap_target
    ,mret
    ,pc_sel
    ,br_taken
    ,jump_jalr
    ,imm_jal 
    ,imm_br
    ,pc
    ,pc_plus4                       
    ,instr
);
    input   wire                  clk,rst,br_taken,trap_enter,mret;
    input   wire [1:0]                                      pc_sel;
    input   wire [31:0]       jump_jalr,imm_jal,imm_br,trap_target;
    output  reg  [31:0]                                         pc;
    output  wire [31:0]                             pc_plus4,instr;

    parameter RESET_VECTOR = 32'h8000_0000;

    wire [31:0] pc_next;

    // PC 选择逻辑
    assign pc_next = (trap_enter | mret ) ? trap_target           :
                     ((pc_sel == 2'b00 ) ? pc + 32'd4             :
                      (pc_sel == 2'b01 ) ? pc + imm_jal            :
                      (pc_sel == 2'b10 ) ? {jump_jalr[31:1],1'b0}  :
                      (pc_sel == 2'b11 ) ? (br_taken ? pc + imm_br : pc + 32'd4) : pc + 32'd4 );

    assign pc_plus4 = pc + 32'd4;

    // 取指：通过 DPI-C 从 C++ 统一内存读取
    assign instr = dpi_mem_read(pc);

    always @(posedge clk ) begin
        if (rst) 
            pc <= RESET_VECTOR;
        else
            pc <= pc_next;
    end
endmodule
