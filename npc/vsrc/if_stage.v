`include "ctrl_defs.vh"
`include "dpi_imports.vh"

module if_stage (
    clk
    ,rst
    ,sys_ctrl
    ,interrupt_valid
    ,trap_target
    ,pc_sel
    ,br_taken
    ,jump_jalr
    ,imm_jal 
    ,imm_br
    ,pc
    ,pc_plus4                       
    ,inst
);
    // 控制流
    input   wire [0 :0]            clk          ;
    input   wire [0 :0]            rst          ;
    input   wire [`SYS_CTRL_WIDTH-1:0] sys_ctrl ;
    input   wire [0 :0]            interrupt_valid;
    input   wire [0 :0]            br_taken     ;
    input   wire [1 :0]            pc_sel       ;
    // 数据流
    input   wire [31:0]            jump_jalr    ;
    input   wire [31:0]            imm_jal      ;
    input   wire [31:0]            imm_br       ;
    input   wire [31:0]            trap_target  ;
    output  reg  [31:0]            pc           ;
    output  wire [31:0]            pc_plus4     ;
    output  wire [31:0]            inst         ;
    // 从 sys_ctrl 拆包
    wire trap_enter_raw = sys_ctrl[`SYS_CTRL_TRAP_ENTER];
    wire mret_raw       = sys_ctrl[`SYS_CTRL_MRET];

    // interrupt 门控
    wire trap_enter = trap_enter_raw | interrupt_valid;
    wire mret       = interrupt_valid ? 1'b0 : mret_raw;

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
    assign inst = dpi_mem_read(pc, 32'b0);

    always @(posedge clk ) begin
        if (rst) 
            pc <= RESET_VECTOR;
        else
            pc <= pc_next;
    end
endmodule
    