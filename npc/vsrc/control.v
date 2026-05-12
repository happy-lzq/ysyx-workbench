module control (
    opcode
    ,funct3
    ,funct7
    ,alu_op
    ,alu_src_a
    ,alu_src_b
    ,br_type
    ,mem_read
    ,mem_write
    ,lsu_type
    ,reg_write
    ,reg_wdata_src
    ,pc_sel
);
    input [6:0] opcode;
    input [2:0] funct3;
    input [6:0] funct7;

    output reg [4:0] alu_op;
    output reg [0:0] alu_src_a;
    output reg [1:0] alu_src_b;
    output reg [2:0] br_type;
    output reg [0:0] mem_read,mem_write,reg_write;
    output reg [2:0] lsu_type;
    output reg [1:0] reg_wdata_src;
    output reg [1:0] pc_sel;

    always@(*)begin
    // 第一步：给所有信号设默认值（非分支、不访存、不写回）
        alu_op        = 5'b0;  //SUB
        alu_src_a     = 0;
        alu_src_b     = 2'b00;
        br_type       = 3'b000;
        mem_read      = 0;
        mem_write     = 0;
        lsu_type      = 3'b000;
        reg_write     = 0;
        reg_wdata_src = 2'b00;
        pc_sel        = 2'b00;

        case (opcode)
        // ==== R =====
          7'b0110011 : begin
            reg_write = 1'b1;
            case (funct3)
                3'b000 : alu_op = funct7[5] ? 5'b0_0000 : 5'b0_0001 ; // SUB/ADD
                3'b001 : alu_op = 5'b0_0010                         ; // SLL
                3'b010 : alu_op = 5'b0_0011                         ; // SLT
                3'b011 : alu_op = 5'b0_0100                         ; // SLTU
                3'b100 : alu_op = 5'b0_0101                         ; // XOR
                3'b101 : alu_op = funct7[5] ? 5'b0_1001 : 5'b0_1000 ; // SRA/SRL
                3'b110 : alu_op = 5'b0_0110                         ; // OR
                3'b111 : alu_op = 5'b0_0111                         ; // AND
                default: alu_op = 5'b0;
            endcase            
          end
        // ==== I-算术立即数 ====
          7'b0010011 : begin
            reg_write = 1'b1 ;
            alu_src_b = 2'b01;
            case (funct3)
                3'b000 : alu_op = 5'b0_0001 ;
                3'b001 : alu_op = 5'b0_0010 ;
                3'b010 : alu_op = 5'b0_0011 ;
                3'b011 : alu_op = 5'b0_0100 ;
                3'b100 : alu_op = 5'b0_0101 ;
                3'b101 : alu_op = funct7[5] ? 5'b0_1001 : 5'b0_1000 ;
                3'b110 : alu_op = 5'b0_0110 ;
                3'b111 : alu_op = 5'b0_0111 ;
                default: alu_op = 5'b0;
            endcase
          end
          // ==== I-Load ====
          7'b0000011 : begin
            alu_src_b     = 2'b01;
            reg_write     = 1'b1;
            mem_read      = 1'b1; 
            reg_wdata_src = 2'b01;
            alu_op        = 5'b0_0001;
            case (funct3)
                3'b000 : lsu_type = 3'b010 ;    // lb 
                3'b001 : lsu_type = 3'b001 ;    // lh
                3'b010 : lsu_type = 3'b000 ;    // lw
                3'b100 : lsu_type = 3'b100 ;    // lbu
                3'b101 : lsu_type = 3'b011 ;    // lhu
                default: ;
            endcase 
          end
          // ==== I-跳转 jalr ====
          7'b1100111 : begin
            alu_src_b     = 2'b01;
            alu_op        = 5'b0_0001;
            reg_write     = 1'b1;
            reg_wdata_src = 2'b10;
            pc_sel        = 2'b10;
          end
          // ==== S-Store ====
          7'b0100011 : begin
            alu_src_b     = 2'b01;
            mem_write     = 1'b1;
            alu_op        = 5'b0_0001;
            case (funct3)
              3'b010 : lsu_type = 3'b101;   // sw
              3'b001 : lsu_type = 3'b110;   // sh
              3'b000 : lsu_type = 3'b111;   // sb
              default: ;
            endcase
          end
          7'b1100011 : begin
            br_type       = 3'b000;
            pc_sel        = 2'b11 ;
            case (funct3)
              3'b000 : br_type = 3'b000;  // beq
              3'b001 : br_type = 3'b001;  // bne
              3'b100 : br_type = 3'b010;  // blt
              3'b101 : br_type = 3'b011;  // bge
              3'b110 : br_type = 3'b100;  // bltu
              3'b111 : br_type = 3'b101;  // bgeu
              default: ;
            endcase
          end
          // U型指令
          7'b0110111 : begin // lui
            alu_op        = 5'b0_1010;
            reg_write     = 1'b1 ; 
            alu_src_b     = 2'b01;
          end 
          7'b0010111 : begin // auipc
            alu_op        = 5'b0_0001;
            reg_write     = 1'b1;
            alu_src_a     = 1'b1;
            alu_src_b     = 2'b01;
          end
          7'b1101111 : begin // jal
            reg_write     = 1'b1;
            reg_wdata_src = 2'b10;
            pc_sel        = 2'b01;
          end
          default: ;
        endcase
    end
endmodule
