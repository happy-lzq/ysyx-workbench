module imm_gen (
    instr
    ,imm_out
);
    input wire [31:0] instr;
    output reg [31:0] imm_out;

    wire [31:0] imm_i,imm_s,imm_b,imm_u,imm_j;
    assign imm_i = {{20{instr[31]}}, instr[31:20]};
    assign imm_s = {{20{instr[31]}}, instr[31:25], instr[11:7] };
    assign imm_b = {{19{instr[31]}}, instr[31], instr[7], instr[30:25], instr[11:8], 1'b0};
    assign imm_u = {instr[31:12], 12'b0};
    assign imm_j = {{11{instr[31]}}, instr[31], instr[19:12], instr[20],instr[30:21], 1'b0};
 
    always @(*) begin
        case (instr[6:0])
        //    7'b0110011 : R类型: 无立即数
        // I 
           7'b0010011 , // 算术
           7'b0000011 , // 加载
           7'b1100111 , // jalr
           7'b1110011 : // ecall ,csr*
            imm_out = imm_i;
        // S 
           7'b0100011 :
            imm_out = imm_s;
        // B
           7'b1100011 :
            imm_out = imm_b;
        // U 
           7'b0110111 , // lui
           7'b0010111 : // auipc
            imm_out = imm_u;
        // J
           7'b1101111 :
            imm_out = imm_j; 
            default: 
            imm_out = 32'd0;
        endcase
    end
endmodule
