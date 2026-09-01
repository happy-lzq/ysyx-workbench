`include "ctrl_defs.vh"
module ex_stage (
    pc 
    ,ex_valid
    ,rs1_rdata          
    ,rs2_rdata
    ,rs1_addr
    ,rs2_addr                 
    ,imm_out    
    ,csr_rdata
    ,ex_ctrl
    ,csr_ctrl
    ,pc_ctrl
    // forwarding 输入
    ,ex_mem_rd_addr
    ,ex_mem_alu_result
    ,ex_mem_valid
    ,ex_mem_wb_ctrl
    ,mem_wb_rd_addr
    ,mem_wb_wb_ctrl
    ,wb_rd_wdata
    ,mem_wb_valid
    // 输出
    ,alu_result                   
    ,csr_wdata   
    ,ex_rs2_value
    ,redirect_valid
    ,redirect_pc
);
    // === 数据流 ===
    input  wire [31:0] pc;
    input  wire        ex_valid;
    input  wire [31:0] rs1_rdata;
    input  wire [31:0] rs2_rdata;
    input  wire [4 :0] rs1_addr;
    input  wire [4 :0] rs2_addr;
    input  wire [31:0] imm_out;
    input  wire [31:0] csr_rdata;

    // === 控制流 ===
    input  wire [`EX_CTRL_WIDTH-1:0]  ex_ctrl;
    input  wire [`CSR_CTRL_WIDTH-1:0] csr_ctrl;
    input  wire [`PC_CTRL_WIDTH-1:0]  pc_ctrl;

    // === forwarding 源 ===
    // EX/MEM
    input  wire [4 :0]                 ex_mem_rd_addr;
    input  wire [31:0]                 ex_mem_alu_result;
    input  wire                        ex_mem_valid;
    input  wire [`WB_CTRL_WIDTH-1:0]   ex_mem_wb_ctrl;
    // MEM/WB
    input  wire [4 :0]                 mem_wb_rd_addr;
    input  wire [`WB_CTRL_WIDTH-1:0]   mem_wb_wb_ctrl;
    input  wire [31:0]                 wb_rd_wdata;
    input  wire                        mem_wb_valid;

    // === 输出 ===
    output wire [31:0] alu_result;
    output reg  [31:0] csr_wdata;
    output wire [31:0] ex_rs2_value;
    output wire        redirect_valid;
    output wire [31:0] redirect_pc;

    // ============================================================
    //  拆包
    // ============================================================
    wire [4:0] alu_op    = ex_ctrl[`EX_CTRL_ALU_OP_MSB:`EX_CTRL_ALU_OP_LSB];
    wire       alu_src_a = ex_ctrl[`EX_CTRL_ALU_SRC_A];
    wire [1:0] alu_src_b = ex_ctrl[`EX_CTRL_ALU_SRC_B_MSB:`EX_CTRL_ALU_SRC_B_LSB];

    wire [1:0]  csr_op   = csr_ctrl[`CSR_CTRL_OP_MSB:`CSR_CTRL_OP_LSB];
    wire        csr_imm  = csr_ctrl[`CSR_CTRL_IMM];
    wire [31:0] csr_zimm = csr_ctrl[`CSR_CTRL_ZIMM_MSB:`CSR_CTRL_ZIMM_LSB];
    wire [1:0]  pc_sel   = pc_ctrl[`PC_SEL_MSB:`PC_SEL_LSB];
    wire [2:0]  br_type  = pc_ctrl[`BR_TYPE_MSB:`BR_TYPE_LSB];
    wire        is_jalr  = pc_ctrl[`IS_JALR];
    wire [31:0] src1,src2 ;

    // ===========================================================
    // forwarding ALU + STORE（rs2_rdata）  三类RAW数据冒险
    // ===========================================================
    // 生产者 ALU结果 
    // forwarding EX/MEM 转发
    wire [1:0] ex_mem_wdata_src = ex_mem_wb_ctrl[`WB_CTRL_WDATA_SRC_MSB:`WB_CTRL_WDATA_SRC_LSB];
    wire ex_mem_reg_write = ex_mem_valid && ex_mem_wb_ctrl[`WB_CTRL_REG_WRITE] && (ex_mem_rd_addr != 5'b0);
    wire ex_mem_alu_ready = (ex_mem_wdata_src == 2'b00);
    wire ex_mem_can_forwading = ex_mem_reg_write && ex_mem_alu_ready;
    wire rs1_from_ex  = ex_mem_can_forwading && (rs1_addr == ex_mem_rd_addr);
    wire rs2_from_ex  = ex_mem_can_forwading && (rs2_addr == ex_mem_rd_addr);

    // forwarding MEM/WB 转发
    wire mem_wb_reg_write = mem_wb_valid && mem_wb_wb_ctrl[`WB_CTRL_REG_WRITE] && (mem_wb_rd_addr != 5'b0);
    wire rs1_from_mem = mem_wb_reg_write && (rs1_addr == mem_wb_rd_addr) && !rs1_from_ex;
    wire rs2_from_mem = mem_wb_reg_write && (rs2_addr == mem_wb_rd_addr) && !rs2_from_ex;
    // 最终旁路选择
    wire [31:0] rs1_real = rs1_from_ex   ? ex_mem_alu_result :
                           rs1_from_mem  ? wb_rd_wdata   : rs1_rdata;
                           
    wire [31:0] rs2_real = rs2_from_ex   ? ex_mem_alu_result :
                           rs2_from_mem  ? wb_rd_wdata   : rs2_rdata;

    // Branch/JALR 在 EX 消费经过 forwarding 选择后的真实寄存器值。
    wire branch_taken;
    wire is_branch = (pc_sel == 2'b11);
    wire [31:0] redirect_sum = (is_jalr ? rs1_real : pc) + imm_out;
    assign redirect_pc = is_jalr ? {redirect_sum[31:1], 1'b0} : redirect_sum;
    assign redirect_valid = ex_valid &&
                            (is_jalr || (is_branch && branch_taken));
    
    assign src1 = (alu_src_a == 1'b0 ) ? rs1_real : pc;
    assign src2 = (alu_src_b == 2'b00) ? rs2_real :
                  (alu_src_b == 2'b01) ? imm_out  : 32'd4;
    assign ex_rs2_value = rs2_real;

    wire [31:0] csr_src = csr_imm ? rs1_real : csr_zimm;
    always @(*) begin
        case (csr_op)
           2'b00 : csr_wdata = csr_src;               // csrrw csrrwi
           2'b01 : csr_wdata = csr_rdata |  csr_src;  // csrrs csrrsi
           2'b10 : csr_wdata = csr_rdata & ~csr_src;  // csrrc csrrci
            default: csr_wdata = 32'b0;
    endcase
    end


    br_cond u_br_cond (
        .rs1_data    (rs1_real),
        .rs2_data    (rs2_real),
        .br_type     (br_type),
        .br_taken    (branch_taken)
    );

    alu alu_pic(
        .src1           (src1      )
        ,.src2          (src2      )
        ,.alu_op        (alu_op    )
        ,.result        (alu_result)
    );
endmodule
