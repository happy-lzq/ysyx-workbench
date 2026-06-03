module control (
    inst
    ,opcode
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
    ,csr_op          
    ,csr_read        
    ,csr_write       
    ,csr_addr        
    ,csr_zimm        
    ,csr_imm
    ,trap_enter
    ,trap_code    
    ,is_ebreak     
    ,mret  
);


    input       [6 :0] opcode;
    input       [2 :0] funct3;
    input       [6 :0] funct7;
    input       [31:0] inst  ;
    output reg  [4 :0] alu_op;
    output reg  [0 :0] alu_src_a;
    output reg  [1 :0] alu_src_b;
    output reg  [2 :0] br_type;
    output reg  [0 :0] mem_read;
    output reg  [0 :0] mem_write;
    output reg  [0 :0] reg_write;
    output reg  [2 :0] lsu_type;
    output reg  [1 :0] reg_wdata_src;
    output reg  [1 :0] pc_sel;
  // CSR output data stream
    output reg  [0 :0] csr_read;
    output reg  [0 :0] csr_write;
    output reg  [0 :0] csr_imm;
    output reg  [0 :0] is_ebreak;
    output reg  [1 :0] csr_op;
    output wire [11:0] csr_addr;
    output wire [31:0] csr_zimm;
  // system output data ctream
    output reg  [0 :0] trap_enter;
    output reg  [0 :0] mret;    
    output reg  [31:0] trap_code;     // 11=ecall, 3=ebreak
  
    wire [4 :0] rs1_addr = inst[19:15];
    wire [11:0] funct12  = inst[31:20] ;
    assign csr_addr      = inst[31:20] ;
    assign csr_zimm      = {{27{1'b0}},inst[19:15]};
    

// ============== 常规指令 ========================//
    always@(*)begin
    // 第一步：给所有信号设默认值（非分支、不访存、不写回）
        alu_op        = 5'b0;  //SUB
        alu_src_a     = 1'b0;
        alu_src_b     = 2'b00;
        br_type       = 3'b000;
        mem_read      = 1'b0;
        mem_write     = 1'b0;
        lsu_type      = 3'b000;
        reg_write     = 1'b0;
        reg_wdata_src = 2'b00;
        pc_sel        = 2'b00;
        // csr 
        csr_op        = 2'b00;
        csr_read      = 1'b0;
        csr_write     = 1'b0;
        csr_imm       = 1'b0;
        // system
        is_ebreak     = 1'b0;
        trap_enter    = 1'b0;
        trap_code     = 32'b0;
        mret          = 1'b0;

        case (opcode)
// ================  system inst  ======================== //
          7'b1110011 : begin
            case (funct3) 
              3'b000 : begin
              case (funct12) 
              12'b0000_0000_0001 : begin                  // ebreak
                // trap_enter = 1'b1;        后续进入trap使用
                // trap_code  = 32'b1011;          
                is_ebreak  = 1'b1;                 
              end
              12'b0000_0000_0000 : begin                  // ecall
                trap_enter = 1'b1;
                trap_code  = 32'd11; 
              end
              12'b0011_0000_0010 : begin                  // mret
                mret = 1'b1;
              end
              default : ;
              endcase
            end
              3'b001 : begin                                  // csrrw
                csr_op        = 2'b00;                        // 写
                csr_read      = 1'b1;                         // load  csr
                csr_write     = 1'b1;                         // store csr
                reg_write     = 1'b1;                         // store regfile
                csr_imm       = 1'b1;                         // 取rs1_data
                reg_wdata_src = 2'b11;
              end
              3'b010 : begin                              // csrrs
                csr_op        = 2'b01;  
                csr_read      = 1'b1; 
                csr_write     = (rs1_addr !=5'b0 ) ? 1 : 0; 
                reg_write     = 1'b1;
                csr_imm       = 1'b1; 
                reg_wdata_src = 2'b11;                        
              end
              3'b011 : begin                              // csrrc
                csr_op        = 2'b10;  
                csr_read      = 1'b1; 
                csr_write     = (rs1_addr !=5'b0 ) ? 1 : 0; 
                reg_write     = 1'b1; 
                csr_imm       = 1'b1;
                reg_wdata_src = 2'b11;
              end 
              3'b101 : begin                              // csrrwi
                csr_op        = 2'b00;  
                csr_read      = 1'b1;   
                csr_write     = 1'b1; 
                reg_write     = 1'b1;   
                csr_imm       = 1'b0;
                reg_wdata_src = 2'b11;
              end
              3'b110 : begin                              // csrrsi
                csr_op        = 2'b01;  
                csr_read      = 1'b1; 
                csr_write     = (csr_zimm !=32'b0 ) ? 1 : 0;  
                reg_write     = 1'b1;
                csr_imm       = 1'b0; 
                reg_wdata_src = 2'b11;   
              end
              3'b111 : begin                              // csrrci
                csr_op         = 2'b10;  
                csr_read       = 1'b1; 
                csr_write      = (csr_zimm !=32'b0 ) ? 1 : 0;  
                reg_write      = 1'b1; 
                csr_imm        = 1'b0;
                reg_wdata_src = 2'b11;
              end
              default:;
            endcase
          end

// =========================== R =====================================
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

// ========================= I-算术立即数 ================================
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

// ========================== I-Load =================================
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

// ========================= I-跳转 jalr ===========================
          7'b1100111 : begin
            alu_src_b     = 2'b01;
            alu_op        = 5'b0_0001;
            reg_write     = 1'b1;
            reg_wdata_src = 2'b10;
            pc_sel        = 2'b10;
          end

// ============================ S-Store ===========================
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

// ========================  U型指令 =============================
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
        default : ;
      endcase
    end
endmodule
