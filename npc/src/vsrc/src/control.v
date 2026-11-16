`include "ctrl_defs.vh"
module control (
    inst
    ,opcode
    ,funct3
    ,funct7
    ,ex_ctrl
    ,mem_ctrl
    ,wb_ctrl
    ,csr_ctrl
    ,sys_ctrl
    ,pc_ctrl
    ,illegal_inst
);
    input  wire [31:0]                inst;
    input  wire [6:0]                 opcode;
    input  wire [2:0]                 funct3;
    input  wire [6:0]                 funct7;

    // == 打包总线端口（wire assign 驱动）==
    output wire [`EX_CTRL_WIDTH-1:0]  ex_ctrl;
    output wire [`MEM_CTRL_WIDTH-1:0] mem_ctrl;
    output wire [`WB_CTRL_WIDTH-1:0]  wb_ctrl;
    output wire [`CSR_CTRL_WIDTH-1:0] csr_ctrl;
    output wire [`SYS_CTRL_WIDTH-1:0] sys_ctrl;
    output wire [`PC_CTRL_WIDTH-1:0]  pc_ctrl;
    output reg illegal_inst;

  // === 内部 reg：always 块里赋值，末尾打包进总线 ===
    reg [4:0]  alu_op;
    reg [0:0]  alu_src_a;
    reg [1:0]  alu_src_b;
    reg [2:0]  br_type;
    reg [0:0]  mem_read, mem_write;
    reg [2:0]  lsu_type;
    reg [0:0]  reg_write;
    reg [1:0]  reg_wdata_src;
    reg [0:0]  csr_read, csr_write, csr_imm;
    reg [1:0]  csr_op;
    reg [1:0]  pc_sel;

    // 用 _int 后缀避免与端口 冲突
    reg [0 :0]  trap_enter_int, is_ebreak_int, mret_int,is_jal_int,is_jalr_int;
    reg [31:0]  trap_code_int;

    wire [4 :0] rs1_addr = inst[19:15] ;
    wire [4 :0] rd_addr  = inst[11: 7] ;
    wire [11:0] funct12  = inst[31:20] ;
    wire [11:0] csr_addr = inst[31:20] ;
    wire [31:0] csr_zimm = {{27{1'b0}},inst[19:15]};

    // ============== CSR访问地址是否正确 ===============
    wire csr_addr_supported = (csr_addr == 12'h300) ||  // mstatus
                              (csr_addr == 12'h304) ||  // mie
                              (csr_addr == 12'h305) ||  // mtvec
                              (csr_addr == 12'h340) ||  // mscratch
                              (csr_addr == 12'h341) ||  // mepc
                              (csr_addr == 12'h342) ||  // mcause
                              (csr_addr == 12'h343) ||  // mtval
                              (csr_addr == 12'h344) ||  // mip
                              (csr_addr == 12'hB00) ||  // mcycle 低 32 位
                              (csr_addr == 12'hB80);    // mcycle 高 32 位

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
        is_ebreak_int     = 1'b0;
        trap_enter_int    = 1'b0;
        trap_code_int     = 32'b0;
        mret_int          = 1'b0;
        // br_type
        is_jal_int  = 1'b0;
        is_jalr_int = 1'b0;

        // 非法指令
        illegal_inst = 1'b0;

        case (opcode)
// ================  system inst  ======================== //
          7'b1110011 : begin
            case (funct3) 
              3'b000 : begin
                case (inst)
                    32'h0000_0073: begin // ECALL
                        trap_enter_int = 1'b1;
                        trap_code_int  = 32'd11;
                    end             
                    32'h0010_0073: begin // EBREAK：暂时保留现有仿真结束行为
                        is_ebreak_int = 1'b1;
                    end             
                    32'h3020_0073: begin // MRET
                        mret_int = 1'b1;
                    end             
                    default: begin
                        illegal_inst = 1'b1;
                    end
                endcase
              end 
  /*
  指令	            rd=x0 时是否读 CSR	        是否写 CSR
  CSRRW  / CSRRWI	  不读 CSR 旧值	              仍然写 CSR
  CSRRS  / CSRRC	  仍然读 CSR	                看 rs1 编号是否为 x0
  CSRRSI / CSRRCI	  仍然读 CSR	                看 zimm 是否为 0
  */
              3'b001 : begin                              // csrrw
                csr_op        = 2'b00;                    // 写
                csr_read      = rd_addr == 5'b0 ? 1'b0 : 1'b1; // csrrw rd=x0,不需要执行csr_read
                csr_write     = 1'b1;                     // store csr
                reg_write     = 1'b1;                     // store regfile
                csr_imm       = 1'b1;                     // 取rs1_data
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
                csr_read      =rd_addr == 5'b0 ? 1'b0 : 1'b1;   
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
              default: begin
                illegal_inst = 1'b1;
              end
            endcase
          end

// =========================== R =====================================
          7'b0110011 : begin
            reg_write = 1'b1;
          
            if (funct7 == 7'b0000001) begin
              case (funct3)
                3'b000 : alu_op = 5'b1_0000;  // MUL
                3'b001 : alu_op = 5'b1_0001;  // MULH
                3'b010 : alu_op = 5'b1_0010;  // MULHSU
                3'b011 : alu_op = 5'b1_0011;  // MULHU
                3'b100 : alu_op = 5'b1_0100;  // DIV
                3'b101 : alu_op = 5'b1_0101;  // DIVU
                3'b110 : alu_op = 5'b1_0110;  // REM
                3'b111 : alu_op = 5'b1_0111;  // REMU
              endcase
            end else if (funct7 == 7'b0000000) begin
                case (funct3)
                    3'b000 : alu_op = 5'b0_0001; // ADD
                    3'b001 : alu_op = 5'b0_0010; // SLL
                    3'b010 : alu_op = 5'b0_0011; // SLT
                    3'b011 : alu_op = 5'b0_0100; // SLTU
                    3'b100 : alu_op = 5'b0_0101; // XOR
                    3'b101 : alu_op = 5'b0_1000; // SRL
                    3'b110 : alu_op = 5'b0_0110; // OR
                    3'b111 : alu_op = 5'b0_0111; // AND
                endcase
            end else if (funct7 == 7'b0100000) begin
                case (funct3)
                    3'b000 : alu_op = 5'b0_0000; // SUB
                    3'b101 : alu_op = 5'b0_1001; // SRA

                    default: begin
                        illegal_inst = 1'b1;
                        reg_write    = 1'b0;
                    end
                endcase
            end else begin
                illegal_inst = 1'b1;
                reg_write    = 1'b0;
            end
          end

// ========================= I-算术立即数 ================================
          7'b0010011 : begin
            reg_write = 1'b1 ;
            alu_src_b = 2'b01;
            case (funct3)
                3'b000 : alu_op = 5'b0_0001 ;     // ADDI
                3'b010 : alu_op = 5'b0_0011 ;     // SLTI
                3'b011 : alu_op = 5'b0_0100 ;     // SLTIU
                3'b100 : alu_op = 5'b0_0101 ;     // XORI
                3'b110 : alu_op = 5'b0_0110 ;     // ORI
                3'b111 : alu_op = 5'b0_0111 ;     // ANDI
                3'b001 : begin                    // SLLI
                  if (funct7 == 7'b0) begin
                    alu_op = 5'b0_0010 ;
                  end else begin
                    illegal_inst = 1'b1;
                    reg_write    = 1'b0;
                  end
                end
                3'b101 : begin                    
                  case (funct7)
                   7'b0000000 : alu_op = 5'b0_1000;     // SRLI
                   7'b0100000 : alu_op = 5'b0_1001;     // SRAI
                    default: begin
                      illegal_inst = 1'b1;
                      reg_write    = 1'b0;
                    end
                  endcase
                end
                default: begin
                  alu_op = 5'b0; 
                  reg_write = 1'b0 ;  
                end
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
                default: begin
                  illegal_inst = 1'b1;
                  mem_read     = 1'b0;
                  reg_write    = 1'b0;
                end

            endcase
          end

// ========================= I-跳转 jalr ===========================
          7'b1100111 : begin
            alu_src_b     = 2'b01;
            alu_op        = 5'b0_0001;
            reg_write     = 1'b1;
            reg_wdata_src = 2'b10;
            pc_sel        = 2'b10;
            is_jalr_int   = 1'b1;
            if(funct3 != 3'b000) begin
              illegal_inst = 1'b1;
              reg_write    = 1'b0;
              pc_sel       = 2'b00;
              is_jalr_int  = 1'b0;
            end
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
              default: begin
                illegal_inst = 1'b1;
                mem_write    = 1'b0;
              end
            endcase
          end
// ========================== branch  ==============================
          7'b1100011 : begin
            br_type   = 3'b000;
            pc_sel    = 2'b11 ;
            case (funct3)
              3'b000 : br_type = 3'b000;  // beq
              3'b001 : br_type = 3'b001;  // bne
              3'b100 : br_type = 3'b010;  // blt
              3'b101 : br_type = 3'b011;  // bge
              3'b110 : br_type = 3'b100;  // bltu
              3'b111 : br_type = 3'b101;  // bgeu
              default: begin
                illegal_inst = 1'b1;
                pc_sel       = 2'b00;
              end
            endcase
          end
// =======================  fence ============================
          7'b0001111 : begin
            case (funct3)
              3'b000 : begin
                illegal_inst = 1'b0;
              end 
              default: begin
                illegal_inst = 1'b1;
              end
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
// ======================   J型指令 ===============================
          7'b1101111 : begin // jal
            reg_write     = 1'b1;
            reg_wdata_src = 2'b10;
            pc_sel        = 2'b01;
            is_jal_int    = 1'b1;
          end
        default : begin
          illegal_inst    = 1'b1;
        end
      endcase
      if ((csr_read || csr_write) && !csr_addr_supported) begin
        illegal_inst = 1'b1;
      end
    end

    // 精准异常同步


    // === 打包：内部 reg → 总线端口 ===
    assign ex_ctrl  = {alu_op, alu_src_a, alu_src_b};
    assign mem_ctrl = {mem_read, mem_write, lsu_type};
    assign wb_ctrl  = {reg_write, reg_wdata_src};
    assign csr_ctrl = {csr_read, csr_write, csr_op, csr_imm, csr_addr, csr_zimm};
    assign sys_ctrl = {trap_enter_int, mret_int, is_ebreak_int, trap_code_int};
    assign pc_ctrl  = {pc_sel, 1'b0, br_type, is_jal_int, is_jalr_int, trap_code_int};



endmodule
