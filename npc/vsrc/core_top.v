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
    wire [31:0] if_pc;
    wire [31:0] if_pc_plus4;
    wire [31:0] reg_rs1_rdata;
    wire [31:0] reg_rs2_rdata;
    wire [31:0] wb_rd_wdata;
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


    // ========== 控制总线（id_stage → 各模块）==========
    wire [`EX_CTRL_WIDTH-1:0]  ex_ctrl;
    wire [`MEM_CTRL_WIDTH-1:0] mem_ctrl;
    wire [`WB_CTRL_WIDTH-1:0]  wb_ctrl;
    wire [`CSR_CTRL_WIDTH-1:0] csr_ctrl;
    wire [`SYS_CTRL_WIDTH-1:0] sys_ctrl;
    wire [`PC_CTRL_WIDTH-1:0]  pc_ctrl;

    // id译码地址属于组合逻辑，跟IF/ID输出属于同一周期
    wire [4:0] id_rd_addr ;
    wire [4:0] id_rs1_addr;
    wire [4:0] id_rs2_addr;

    // ========== 从总线提取 + 门控（简单模块用）==========
    
    wire reg_write_raw  = mem_wb_wb_ctrl[`WB_CTRL_REG_WRITE];
    wire is_ebreak_raw  = mem_wb_sys_ctrl[`SYS_CTRL_IS_EBREAK];
    
    // 寄存器写回信号必须由mem/wb当前有效作为总控来决定
    wire reg_write_eff  = mem_wb_valid && !interrupt_valid && reg_write_raw;
    wire is_ebreak_eff  = mem_wb_valid && !interrupt_valid && is_ebreak_raw;
    wire [31:0] redirect_pc ;

    wire id_uses_rs1;
    wire id_uses_rs2;

    // ============= 数据冒险：ALU 生产者 -> ID 阶段 branch/jalr =============
    // ALU-ID 冒险检查
    wire id_is_branch = (if_id_inst[6:0] == 7'b1100011);
    wire id_is_jalr   = (if_id_inst[6:0] == 7'b1100111);
    wire id_early_gpr_consumer = if_id_valid && (id_is_branch || id_is_jalr);
    wire [1:0] id_ex_wdata_src = id_ex_wb_ctrl[`WB_CTRL_WDATA_SRC_MSB:`WB_CTRL_WDATA_SRC_LSB];
    wire id_ex_alu_producer = id_ex_valid &&
                              id_ex_wb_ctrl[`WB_CTRL_REG_WRITE] &&
                              (id_ex_rd_addr != 5'b0) &&
                              (id_ex_wdata_src == 2'b00);
    // 消费者与生产者匹配，产生 ALU -> ID 冒险请求。
    wire alu_to_id_hazard = id_early_gpr_consumer &&
                            id_ex_alu_producer &&
                            ((id_uses_rs1 && (id_rs1_addr == id_ex_rd_addr)) ||
                             (id_uses_rs2 && (id_rs2_addr == id_ex_rd_addr)));

    // ============= 数据冒险 load-use load -> EX 冒险请求 ==============
    wire load_use_hazard = if_id_valid &&
                           id_ex_valid &&
                           id_ex_mem_ctrl[`MEM_CTRL_READ] &&
                           (id_ex_rd_addr != 5'b0) &&
                           ((id_uses_rs1 && (id_rs1_addr == id_ex_rd_addr)) ||
                            (id_uses_rs2 && (id_rs2_addr == id_ex_rd_addr)));

    // 后续可在这里追加 csr_to_id_hazard 和 other_id_hazard。
    wire id_stall_req = alu_to_id_hazard || load_use_hazard;

    // if/id stall 设计
    wire if_id_valid_sel            = id_stall_req ? if_id_valid : 1'b1;
    wire [31:0] if_id_pc_sel        = id_stall_req ? if_id_pc         : if_pc;
    wire [31:0] if_id_pc_plus4_sel  = id_stall_req ? if_id_pc_plus4   : if_pc_plus4;
    wire [31:0] if_id_inst_sel      = id_stall_req ? if_id_inst       : instr;

    // ============= stall 时 ID/EX 插入bubble ========
    wire [0 :0]  id_ex_valid_sel = id_stall_req ? 1'b0 : if_id_valid;

    // ============= stall 时 IF阶段插入pc_enable 规避pc跳跃 ========
    wire [0 :0]  pc_enable = !id_stall_req;

    // 暂停后的下一拍，生产者位于 EX/MEM，ALU 结果可直接前递给 ID。
    wire [1:0] ex_mem_wdata_src = ex_mem_wb_ctrl[`WB_CTRL_WDATA_SRC_MSB:`WB_CTRL_WDATA_SRC_LSB];
    wire ex_mem_alu_producer = ex_mem_valid &&
                               ex_mem_wb_ctrl[`WB_CTRL_REG_WRITE] &&
                               (ex_mem_rd_addr != 5'b0) &&
                               (ex_mem_wdata_src == 2'b00);

    wire ex_mem_id_rs1_hit = id_early_gpr_consumer &&
                             id_uses_rs1 &&
                             ex_mem_alu_producer &&
                             (id_rs1_addr == ex_mem_rd_addr);

    wire ex_mem_id_rs2_hit = id_early_gpr_consumer &&
                             id_uses_rs2 &&
                             ex_mem_alu_producer &&
                             (id_rs2_addr == ex_mem_rd_addr);



    // ============= 数据冒险：WB -> ID 同周期 bypass =============
    // 生产者在 WB 写 regfile，消费者在 ID 读 regfile。
    wire wb_id_rs1_hit = if_id_valid &&
                         id_uses_rs1 &&
                         reg_write_eff &&
                         (mem_wb_rd_addr != 5'b0) &&
                         (id_rs1_addr == mem_wb_rd_addr);

    wire wb_id_rs2_hit = if_id_valid &&
                         id_uses_rs2 &&
                         reg_write_eff &&
                         (mem_wb_rd_addr != 5'b0) &&
                         (id_rs2_addr == mem_wb_rd_addr);

    // 同时命中时，距离消费者更近的 EX/MEM 生产者拥有最高优先级。
    wire [31:0] id_rs1_rdata = ex_mem_id_rs1_hit ? ex_mem_alu_result : 
                               wb_id_rs1_hit     ? wb_rd_wdata       : reg_rs1_rdata;
    wire [31:0] id_rs2_rdata = ex_mem_id_rs2_hit ? ex_mem_alu_result : 
                               wb_id_rs2_hit     ? wb_rd_wdata       : reg_rs2_rdata;

    // ================= 数据冒险 store RAW 紧挨着/相隔一条指令 =============
    wire [31:0] ex_rs2_value;

    // ============ 流水线寄存变量 ====================== 
    //if/id
    wire  [0 :0]  if_id_valid ;
    wire  [31:0]  if_id_pc;
    wire  [31:0]  if_id_pc_plus4;
    wire  [31:0]  if_id_inst;

    // id/ex
    wire                          id_ex_valid;
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

if_stage u_if_stage (
    .clk                (clk            ),
    .rst                (rst            ),
    .pc_enable          (pc_enable      ),
    .pc_ctrl            (pc_ctrl        ),
    .sys_ctrl           (sys_ctrl       ),
    .interrupt_valid    (interrupt_valid),
    .trap_target        (trap_target    ),
    .redirect_pc        (redirect_pc    ), 
    .pc                 (if_pc          ),
    .pc_plus4           (if_pc_plus4    ),
    .inst               (instr          )
);

if_id_reg u_if_id_reg (
    .clk                  (clk                ),
    .rst                  (rst                ),
    .if_id_valid_in       (if_id_valid_sel    ),
    .if_id_pc_in          (if_id_pc_sel       ),
    .if_id_pc_plus4_in    (if_id_pc_plus4_sel ),
    .if_id_inst_in        (if_id_inst_sel     ),
    .if_id_valid          (if_id_valid   ),
    .if_id_pc             (if_id_pc      ),
    .if_id_pc_plus4       (if_id_pc_plus4),
    .if_id_inst           (if_id_inst    )
);
id_stage u_id_stage (
    .inst           (if_id_inst ),
    .if_id_pc       (if_id_pc   ),
    .rs1_data       (id_rs1_rdata),
    .rs2_data       (id_rs2_rdata),
    .rs1_addr       (id_rs1_addr),
    .rs2_addr       (id_rs2_addr),
    .rd_addr        (id_rd_addr ),
    .id_uses_rs1    (id_uses_rs1),
    .id_uses_rs2    (id_uses_rs2),
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
    .id_ex_valid_in        (id_ex_valid_sel ),
    .id_ex_pc_in           (if_id_pc        ),
    .id_ex_pc_plus4_in     (if_id_pc_plus4  ),
    .id_ex_inst_in         (if_id_inst      ),
    .id_ex_rs1_addr_in     (id_rs1_addr     ),
    .id_ex_rs2_addr_in     (id_rs2_addr     ),
    .id_ex_rd_addr_in      (id_rd_addr      ),
    .id_ex_rs1_rdata_in    (id_rs1_rdata    ),
    .id_ex_rs2_rdata_in    (id_rs2_rdata    ),
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
    .pc                   (id_ex_pc         ),
    .rs1_rdata            (id_ex_rs1_rdata  ),
    .rs2_rdata            (id_ex_rs2_rdata  ),
    .rs1_addr             (id_ex_rs1_addr   ),
    .rs2_addr             (id_ex_rs2_addr   ),
    .imm_out              (id_ex_imm_out    ),
    .csr_rdata            (csr_rdata        ),
    .ex_ctrl              (id_ex_ex_ctrl    ),
    .csr_ctrl             (id_ex_csr_ctrl   ),
    // forwarding 输入
    .ex_mem_rd_addr       (ex_mem_rd_addr   ),
    .ex_mem_alu_result    (ex_mem_alu_result),
    .ex_mem_valid         (ex_mem_valid     ),
    .ex_mem_wb_ctrl       (ex_mem_wb_ctrl   ),
    .mem_wb_rd_addr       (mem_wb_rd_addr   ),
    .mem_wb_wb_ctrl       (mem_wb_wb_ctrl   ),
    .wb_rd_wdata          (wb_rd_wdata      ),
    // 输出
    .mem_wb_valid         (mem_wb_valid     ),
    .alu_result           (alu_result       ),
    .csr_wdata            (csr_wdata        ),
    .ex_rs2_value         (ex_rs2_value     )
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
    .ex_mem_rs2_rdata_in     (ex_rs2_value      ),
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
    .clk                (clk                    ),                
    .mem_ctrl           (ex_mem_mem_ctrl        ),
    .interrupt_valid    (interrupt_valid        ),
    .mem_valid          (ex_mem_valid           ),          
    .mem_addr           (ex_mem_alu_result      ),
    .mem_wdata_raw      (ex_mem_rs2_rdata       ),
    .mem_rdata          (mem_rdata              )             
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
    .rd_wdata      (wb_rd_wdata        )                  
);
regfile u_regfile (
    .clk          (clk               ),  
    .rs1_addr     (id_rs1_addr       ),
    .rs2_addr     (id_rs2_addr       ),
    .wb_rd_addr   (mem_wb_rd_addr    ),
    .wb_rd_wdata  (wb_rd_wdata       ),
    .reg_write    (reg_write_eff     ),
    .rs1_rdata    (reg_rs1_rdata     ),                  
    .rs2_rdata    (reg_rs2_rdata     ),
    .rs_a0        (rs_a0             )
);

csr u_csr (
    .clk                 (clk               ),
    .rst                 (rst               ),
    .csr_commit_valid    (mem_wb_valid      ),
    .csr_read_ctrl       (id_ex_csr_ctrl    ),
    .csr_write_ctrl      (mem_wb_csr_ctrl   ),
    .csr_wdata           (mem_wb_csr_wdata  ),
    .sys_redirect_ctrl   (sys_ctrl          ),
    .sys_commit_ctrl     (mem_wb_sys_ctrl   ),
    .interrupt_valid     (interrupt_valid   ),
    .interrupt_cause     (interrupt_cause   ),
    .trap_pc             (mem_wb_pc         ),
    .csr_rdata           (csr_rdata         ),
    .trap_target         (trap_target       )
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
