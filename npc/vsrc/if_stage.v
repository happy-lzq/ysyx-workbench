`include "dpi_imports.vh"

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
    input   wire [0 :0]            clk          ;
    input   wire [0 :0]            rst          ;
    input   wire [0 :0]            br_taken     ;
    input   wire [0 :0]            trap_enter   ;
    input   wire [0 :0]            mret         ;
    input   wire [1 :0]            pc_sel       ;
    input   wire [31:0]            jump_jalr    ;
    input   wire [31:0]            imm_jal      ;
    input   wire [31:0]            imm_br       ;
    input   wire [31:0]            trap_target  ;
    output  reg  [31:0]            pc           ;
    output  wire [31:0]            pc_plus4     ;
    output  wire [31:0]            instr        ;

    parameter RESET_VECTOR = 32'h8000_0000;
    wire [31:0] pc_next;

    // PC 选择逻辑
    assign pc_next = (trap_enter | mret ) ? trap_target             :
                     ((pc_sel == 2'b00 )  ? pc + 32'd4              :
                      (pc_sel == 2'b01 )  ? pc + imm_jal            :
                      (pc_sel == 2'b10 )  ? {jump_jalr[31:1],1'b0}  :
                      (pc_sel == 2'b11 )  ? (br_taken ? pc + imm_br : pc + 32'd4) : pc + 32'd4 );

    assign pc_plus4 = pc + 32'd4;

    // 取指：通过 DPI-C 从 C++ 统一内存读取；不作为 data load 触发 MMIO skip
    assign instr = dpi_mem_read(pc, 32'b0);

    always @(posedge clk ) begin
        if (rst) 
            pc <= RESET_VECTOR;
        else
            pc <= pc_next;
    end
endmodule
