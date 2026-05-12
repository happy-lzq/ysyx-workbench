module if_stage (
    clk
    ,rst
    ,pc_sel
    ,br_taken
    ,jump_jalr
    ,imm_jal 
    ,imm_br
    ,pc
    ,pc_plus4                       // 用于regfile 写回
    ,instr
);
    input   wire                      clk,rst,br_taken;
    input   wire [1:0]                          pc_sel;
    input   wire [31:0]       jump_jalr,imm_jal,imm_br;
    output  reg  [31:0]                             pc;
    output  wire [31:0]                 pc_plus4,instr;

    parameter RESET_VECTOR = 32'h8000_0000;
    parameter IMEM_SIZE    = 65536        ;  // imem dmem 统一 2^16=65526

    wire [31:0] pc_next;
    reg  [31:0] imem [0:IMEM_SIZE - 1];
    initial begin
        $readmemh("../am-kernels/tests/cpu-tests/build/add-riscv32I-npc.bin",imem);
    end
    assign pc_next = (pc_sel == 2'b00 ) ? pc + 32'd4              :
                     (pc_sel == 2'b01 ) ? pc + imm_jal            :
                     (pc_sel == 2'b10 ) ? {jump_jalr[31:1],1'b0}  :
                     (pc_sel == 2'b11 ) ? (br_taken ? pc + imm_br : pc + 32'd4) : pc + 32'd4 ;

    assign pc_plus4 = pc + 32'd4;
    assign instr    = imem[pc[17:2]];

    always @(posedge clk ) begin
        if (rst) 
            pc <= RESET_VECTOR;
        else
            pc <= pc_next;
    end
endmodule
