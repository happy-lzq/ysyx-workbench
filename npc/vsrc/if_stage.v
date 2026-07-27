`include "ctrl_defs.vh"
`include "dpi_imports.vh"

module if_stage (
    clk
    ,rst
    ,pc_enable
    ,pc_ctrl
    ,sys_ctrl
    ,interrupt_valid
    ,trap_target
    ,redirect_pc
    ,pc
    ,pc_plus4                       
    ,inst
);
    // 控制流
    input   wire [0 :0]            clk          ;
    input   wire [0 :0]            rst          ;
    input   wire [0 :0]            pc_enable    ;
    input   wire [`PC_CTRL_WIDTH-1:0] pc_ctrl   ;
    input   wire [`SYS_CTRL_WIDTH-1:0] sys_ctrl ;
    input   wire [0 :0]          interrupt_valid;
    input   wire [31:0]              redirect_pc;
    input   wire [31:0]            trap_target  ;
    // 输出
    output  reg  [31:0]            pc           ;
    output  wire [31:0]            pc_plus4     ;
    output  wire [31:0]            inst         ;
    wire [31:0] pc_next;

   // === 从 pc_ctrl 拆包 ===
    wire [1:0] pc_sel   = pc_ctrl[`PC_SEL_MSB:`PC_SEL_LSB];
    wire       br_taken = pc_ctrl[`BR_TAKEN];
    wire       is_jal   = pc_ctrl[`IS_JAL];
    wire       is_jalr  = pc_ctrl[`IS_JALR];
    
    // 从 sys_ctrl 拆包
    wire trap_enter_raw = sys_ctrl[`SYS_CTRL_TRAP_ENTER];
    wire mret_raw       = sys_ctrl[`SYS_CTRL_MRET];

    // interrupt 门控
    wire trap_enter = trap_enter_raw | interrupt_valid;
    wire mret       = interrupt_valid ? 1'b0 : mret_raw;

    // === redirect 判定 ===
    wire redirect_valid = is_jal | is_jalr | ((pc_sel == 2'b11) & br_taken);
    assign pc_next = (trap_enter | mret) ? trap_target :
                     redirect_valid      ? redirect_pc : pc + 32'd4;

    assign pc_plus4 = pc + 32'd4;

    // 取指：通过 DPI-C 从 C++ 统一内存读取；不作为 data load 触发 MMIO skip
    assign inst = dpi_mem_read(pc, 32'b0);

    always @(posedge clk ) begin
        if (rst) 
            pc <= `RESET_VECTOR;
        else if(pc_enable)
            pc <= pc_next;
    end
endmodule
    