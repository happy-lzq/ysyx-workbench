`include "ctrl_defs.vh"

module core_top (
    clk
    ,rst
    ,interrupt_valid
    ,interrupt_cause
    ,instr
    ,halt
    ,halt_pc
    ,halt_ret
);
    input  wire [0 :0]                 clk,rst,interrupt_valid;
    input  wire [31:0]                         interrupt_cause;
    output wire [0 :0]                                    halt;
    output wire [31:0]                   instr,halt_pc,halt_ret;

    // ========== 数据流 ==========
    wire [31:0] pc;
    wire [31:0] pc_plus4;
    wire [31:0] rs1_rdata;
    wire [31:0] rs2_rdata;
    wire [31:0] rd_wdata;
    wire [31:0] alu_result;
    wire [31:0] imm_out;
    wire [31:0] mem_addr;
    wire [31:0] mem_wdata_raw;
    wire [31:0] mem_rdata;
    wire [31:0] jump_jalr;
    wire [31:0] imm_jal;
    wire [31:0] imm_br;
    wire [31:0] csr_rdata;
    wire [31:0] csr_wdata;
    wire [31:0] trap_target;
    wire [31:0] trap_pc;
    wire [31:0] rs_a0;
    wire [4 :0] rs1_addr;
    wire [4 :0] rs2_addr;

    // ========== 控制总线（id_stage → 各模块）==========
    wire [`EX_CTRL_WIDTH-1:0]  ex_ctrl;
    wire [`MEM_CTRL_WIDTH-1:0] mem_ctrl;
    wire [`WB_CTRL_WIDTH-1:0]  wb_ctrl;
    wire [`CSR_CTRL_WIDTH-1:0] csr_ctrl;
    wire [`SYS_CTRL_WIDTH-1:0] sys_ctrl;
    wire [`PC_CTRL_WIDTH-1:0]  pc_ctrl;
    wire [4:0] rd_addr;
    // ========== 从总线提取 + 门控（简单模块用）==========
    
    wire reg_write_raw  = mem_wb_wb_ctrl[`WB_CTRL_REG_WRITE];
    wire is_ebreak_raw  = mem_wb_sys_ctrl[`SYS_CTRL_IS_EBREAK];
    
    // 寄存器写回信号必须由mem/wb当前有效作为总控来决定
    wire reg_write_eff  = mem_wb_valid && !interrupt_valid && reg_write_raw;
    wire is_ebreak_eff  = mem_wb_valid && !interrupt_valid && is_ebreak_raw;

    wire if_valid = 1'b1;          // valid 链的唯一源头
    wire [31:0]     redirect_pc ;
    // ============ 流水线寄存变量 ====================== 
    //if/id
    wire          if_id_valid;
    wire  [31:0]  if_id_pc;
    wire  [31:0]  if_id_pc_plus4;
    wire  [31:0]  if_id_inst;

    // id/ex
    wire                          id_ex_valid     ;
    wire  [31:0]                  id_ex_pc ;
    wire  [31:0]                  id_ex_pc_plus4 ;
    wire  [31:0]                  id_ex_inst  ;
    wire  [4 :0]                  id_ex_rs1_addr ;
    wire  [4 :0]                  id_ex_rs2_addr ;
    wire  [4 :0]                  id_ex_rd_addr ;
    wire  [31:0]                  id_ex_rs1_rdata ;
    wire  [31:0]                  id_ex_rs2_rdata ;
    wire  [31:0]                  id_ex_imm_out ;
    wire  [`EX_CTRL_WIDTH-1 :0]   id_ex_ex_ctrl;
    wire  [`MEM_CTRL_WIDTH-1:0]   id_ex_mem_ctrl;
    wire  [`WB_CTRL_WIDTH-1 :0]   id_ex_wb_ctrl;
    wire  [`CSR_CTRL_WIDTH-1:0]   id_ex_csr_ctrl;
    wire  [`SYS_CTRL_WIDTH-1:0]   id_ex_sys_ctrl;
    // ex/mem
    wire                          ex_mem_valid     ;
    wire  [31:0]                  ex_mem_pc ;
    wire  [31:0]                  ex_mem_pc_plus4 ;
    wire  [31:0]                  ex_mem_inst  ;
    wire  [4 :0]                  ex_mem_rd_addr ;
    wire  [31:0]                  ex_mem_alu_result ;
    wire  [31:0]                  ex_mem_rs2_rdata ;
    wire  [31:0]                  ex_mem_csr_rdata ;
    wire  [31:0]                  ex_mem_csr_wdata ;
    wire  [`MEM_CTRL_WIDTH-1:0]   ex_mem_mem_ctrl;
    wire  [`WB_CTRL_WIDTH-1 :0]   ex_mem_wb_ctrl;
    wire  [`CSR_CTRL_WIDTH-1:0]   ex_mem_csr_ctrl;
    wire  [`SYS_CTRL_WIDTH-1:0]   ex_mem_sys_ctrl;
    // mem/wb

    wire                          mem_wb_valid     ;
    wire  [31:0]                  mem_wb_pc ;
    wire  [31:0]                  mem_wb_pc_plus4 ;
    wire  [31:0]                  mem_wb_inst  ;
    wire  [4 :0]                  mem_wb_rd_addr ;
    wire  [31:0]                  mem_wb_alu_result ;
    wire  [31:0]                  mem_wb_mem_rdata ;
    wire  [31:0]                  mem_wb_csr_rdata ;
    wire  [31:0]                  mem_wb_csr_wdata ;
    wire  [`WB_CTRL_WIDTH-1 :0]   mem_wb_wb_ctrl;
    wire  [`CSR_CTRL_WIDTH-1:0]   mem_wb_csr_ctrl;
    wire  [`SYS_CTRL_WIDTH-1:0]   mem_wb_sys_ctrl;

    assign jump_jalr     = alu_result;
    assign imm_jal       = imm_out;
    assign imm_br        = imm_out;
    // assign mem_addr      = alu_result;
    // assign mem_wdata_raw = rs2_rdata;
    // assign trap_pc       = pc;



if_stage u_if_stage (
    .clk                (clk            ),
    .rst                (rst            ),
    .pc_ctrl            (pc_ctrl        ),
    .sys_ctrl           (sys_ctrl       ),
    .interrupt_valid    (interrupt_valid),
    .trap_target        (trap_target    ),
    .redirect_pc        (redirect_pc    ), 
    .pc                 (pc             ),
    .pc_plus4           (pc_plus4       ),
    .inst               (instr          )
);

if_id_reg u_if_id_reg (
    .clk                  (clk           ),
    .rst                  (rst           ),
    .if_id_valid_in       (if_valid      ),
    .if_id_pc_in          (pc            ),
    .if_id_pc_plus4_in    (pc_plus4      ),
    .if_id_inst_in        (instr         ),
    .if_id_valid          (if_id_valid   ),
    .if_id_pc             (if_id_pc      ),
    .if_id_pc_plus4       (if_id_pc_plus4),
    .if_id_inst           (if_id_inst    )
);
id_stage u_id_stage (
    .inst           (if_id_inst ),
    .if_id_pc       (if_id_pc   ),
    .rs1_data       (rs1_rdata  ),
    .rs2_data       (rs2_rdata  ),
    .rs1_addr       (rs1_addr   ),
    .rs2_addr       (rs2_addr   ),
    .rd_addr        (rd_addr    ),
    .imm_out        (imm_out    ),
    .redirect_pc    (redirect_pc),
    .ex_ctrl        (ex_ctrl    ),
    .mem_ctrl       (mem_ctrl   ),
    .wb_ctrl        (wb_ctrl    ),
    .csr_ctrl       (csr_ctrl   ),
    .sys_ctrl       (sys_ctrl   ),
    .pc_ctrl        (pc_ctrl    )
);
id_ex_reg u_id_ex_reg (
    .clk                   (clk),
    .rst                   (rst),
    .id_ex_valid_in        (if_id_valid     ),
    .id_ex_pc_in           (if_id_pc        ),
    .id_ex_pc_plus4_in     (if_id_pc_plus4  ),
    .id_ex_inst_in         (if_id_inst      ),
    .id_ex_rs1_addr_in     (rs1_addr        ),
    .id_ex_rs2_addr_in     (rs2_addr        ),
    .id_ex_rd_addr_in      (rd_addr         ),
    .id_ex_rs1_rdata_in    (rs1_rdata       ),
    .id_ex_rs2_rdata_in    (rs2_rdata       ),
    .id_ex_imm_out_in      (imm_out         ),
    .id_ex_ex_ctrl_in      (ex_ctrl         ),
    .id_ex_mem_ctrl_in     (mem_ctrl        ),
    .id_ex_wb_ctrl_in      (wb_ctrl         ),
    .id_ex_csr_ctrl_in     (csr_ctrl        ),
    .id_ex_sys_ctrl_in     (sys_ctrl        ),
    .id_ex_valid           (id_ex_valid     ),
    .id_ex_pc              (id_ex_pc        ),
    .id_ex_pc_plus4        (id_ex_pc_plus4  ),
    .id_ex_inst            (id_ex_inst      ),
    .id_ex_rs1_addr        (id_ex_rs1_addr  ),
    .id_ex_rs2_addr        (id_ex_rs2_addr  ),
    .id_ex_rd_addr         (id_ex_rd_addr   ),
    .id_ex_rs1_rdata       (id_ex_rs1_rdata ),
    .id_ex_rs2_rdata       (id_ex_rs2_rdata ),
    .id_ex_imm_out         (id_ex_imm_out   ),
    .id_ex_ex_ctrl         (id_ex_ex_ctrl   ),
    .id_ex_mem_ctrl        (id_ex_mem_ctrl  ),
    .id_ex_wb_ctrl         (id_ex_wb_ctrl   ),
    .id_ex_csr_ctrl        (id_ex_csr_ctrl  ),
    .id_ex_sys_ctrl        (id_ex_sys_ctrl  )
);
ex_stage u_ex_stage (
    .pc            (id_ex_pc                      ),                        
    .rs1_rdata     (id_ex_rs1_rdata               ),
    .rs2_rdata     (id_ex_rs2_rdata               ),
    .imm_out       (id_ex_imm_out                 ),
    .csr_rdata     (csr_rdata                     ),
    .ex_ctrl       (id_ex_ex_ctrl                 ),
    .csr_ctrl      (id_ex_csr_ctrl                ),
    .alu_result    (alu_result                    ),                
    .csr_wdata     (csr_wdata                     )
);

ex_mem_reg u_ex_mem_reg (
    .clk                     (clk               ),
    .rst                     (rst               ),
    .ex_mem_valid_in         (id_ex_valid       ),
    .ex_mem_pc_in            (id_ex_pc          ),
    .ex_mem_pc_plus4_in      (id_ex_pc_plus4    ),
    .ex_mem_inst_in          (id_ex_inst        ),
    .ex_mem_rd_addr_in       (id_ex_rd_addr     ),
    .ex_mem_alu_result_in    (alu_result        ),
    .ex_mem_rs2_rdata_in     (id_ex_rs2_rdata   ),
    .ex_mem_mem_ctrl_in      (id_ex_mem_ctrl    ),
    .ex_mem_wb_ctrl_in       (id_ex_wb_ctrl     ),
    .ex_mem_csr_rdata_in     (csr_rdata         ),
    .ex_mem_csr_wdata_in     (csr_wdata         ),
    .ex_mem_csr_ctrl_in      (id_ex_csr_ctrl    ),
    .ex_mem_sys_ctrl_in      (id_ex_sys_ctrl    ),
    .ex_mem_valid            (ex_mem_valid      ),
    .ex_mem_pc               (ex_mem_pc         ),
    .ex_mem_pc_plus4         (ex_mem_pc_plus4   ),
    .ex_mem_inst             (ex_mem_inst       ),
    .ex_mem_rd_addr          (ex_mem_rd_addr    ),
    .ex_mem_alu_result       (ex_mem_alu_result ),
    .ex_mem_rs2_rdata        (ex_mem_rs2_rdata  ),
    .ex_mem_mem_ctrl         (ex_mem_mem_ctrl   ),
    .ex_mem_wb_ctrl          (ex_mem_wb_ctrl    ),
    .ex_mem_csr_rdata        (ex_mem_csr_rdata  ),
    .ex_mem_csr_wdata        (ex_mem_csr_wdata  ),
    .ex_mem_csr_ctrl         (ex_mem_csr_ctrl   ),
    .ex_mem_sys_ctrl         (ex_mem_sys_ctrl   )
);

mem_stage u_mem_stage (
    .clk                (clk                    ),                  // input
    .mem_ctrl           (ex_mem_mem_ctrl        ),
    .interrupt_valid    (interrupt_valid        ),
    .mem_valid          (ex_mem_valid           ),          
    .mem_addr           (ex_mem_alu_result      ),
    .mem_wdata_raw      (ex_mem_rs2_rdata       ),
    .mem_rdata          (mem_rdata              )             // output
);

mem_wb_reg u_mem_wb_reg (
    .clk                     (clk               ),
    .rst                     (rst               ),
    .mem_wb_valid_in         (ex_mem_valid      ),
    .mem_wb_pc_in            (ex_mem_pc         ),
    .mem_wb_pc_plus4_in      (ex_mem_pc_plus4   ),
    .mem_wb_inst_in          (ex_mem_inst       ),
    .mem_wb_rd_addr_in       (ex_mem_rd_addr    ),
    .mem_wb_alu_result_in    (ex_mem_alu_result ),
    .mem_wb_mem_rdata_in     (mem_rdata         ),
    .mem_wb_wb_ctrl_in       (ex_mem_wb_ctrl    ),
    .mem_wb_csr_rdata_in     (ex_mem_csr_rdata  ),
    .mem_wb_csr_wdata_in     (ex_mem_csr_wdata  ),
    .mem_wb_csr_ctrl_in      (ex_mem_csr_ctrl   ),
    .mem_wb_sys_ctrl_in      (ex_mem_sys_ctrl   ),
    .mem_wb_valid            (mem_wb_valid      ),
    .mem_wb_pc               (mem_wb_pc         ),
    .mem_wb_pc_plus4         (mem_wb_pc_plus4   ),
    .mem_wb_inst             (mem_wb_inst       ),
    .mem_wb_rd_addr          (mem_wb_rd_addr    ),
    .mem_wb_alu_result       (mem_wb_alu_result ),
    .mem_wb_mem_rdata        (mem_wb_mem_rdata  ),
    .mem_wb_wb_ctrl          (mem_wb_wb_ctrl    ),
    .mem_wb_csr_rdata        (mem_wb_csr_rdata  ),
    .mem_wb_csr_wdata        (mem_wb_csr_wdata  ),
    .mem_wb_csr_ctrl         (mem_wb_csr_ctrl   ),
    .mem_wb_sys_ctrl         (mem_wb_sys_ctrl   )
);

wb_stage u_wb_stage (
    .wb_ctrl       (mem_wb_wb_ctrl     ),                  
    .mem_rdata     (mem_wb_mem_rdata   ),
    .alu_result    (mem_wb_alu_result  ),
    .pc_plus4      (mem_wb_pc_plus4    ),
    .csr_rdata     (mem_wb_csr_rdata   ),
    .rd_wdata      (rd_wdata           )                  
);
regfile u_regfile (
    .clk          (clk                  ),  
    .rs1_addr     (rs1_addr             ),
    .rs2_addr     (rs2_addr             ),
    .rd_addr      (mem_wb_rd_addr       ),
    .rd_wdata     (rd_wdata     ),
    .reg_write    (reg_write_eff),
    .rs1_rdata    (rs1_rdata    ),                  
    .rs2_rdata    (rs2_rdata    ),
    .rs_a0        (rs_a0        )
);

csr u_csr (
    .clk                 (clk),
    .rst                 (rst),
    .csr_commit_valid    (mem_wb_valid),
    .csr_read_ctrl       (id_ex_csr_ctrl),
    .csr_write_ctrl      (mem_wb_csr_ctrl),
    .csr_wdata           (mem_wb_csr_wdata),
    .sys_redirect_ctrl   (sys_ctrl),
    .sys_commit_ctrl     (mem_wb_sys_ctrl),
    .interrupt_valid     (interrupt_valid),
    .interrupt_cause     (interrupt_cause),
    .trap_pc             (mem_wb_pc),
    .csr_rdata           (csr_rdata),
    .trap_target         (trap_target)
);

halt u_halt (
    .clk          (clk),
    .rst          (rst),
    .is_ebreak    (is_ebreak_eff),
    .pc           (mem_wb_pc),
    .rs_a0        (rs_a0),
    .halt_reg     (halt),
    .halt_pc      (halt_pc),
    .halt_ret     (halt_ret)
);

endmodule
