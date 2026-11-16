`include "ctrl_defs.vh"
`include "mem_if_defs.vh"
`include "dpi_imports.vh"
`include "exception.vh"

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
    input  wire [0:0]  clk;
    input  wire [0:0]  rst;
    input  wire [0:0]  interrupt_valid;
    input  wire [31:0] interrupt_cause;

    output wire [31:0] instr;
    output wire [0:0]  halt;
    output wire [31:0] halt_pc;
    output wire [31:0] halt_ret;

    // ========== 数据流 ==========
    wire [31:0] if_pc;
    wire [31:0] if_pc_plus4;
    wire [31:0] reg_rs1_rdata;
    wire [31:0] reg_rs2_rdata;
    wire [31:0] wb_rd_wdata;
    wire [31:0] alu_result;
    wire [31:0] imm_out;
    wire [31:0] mem_rdata;
    wire [31:0] csr_rdata;
    wire [31:0] csr_wdata;
    wire [31:0] trap_target;
    wire [31:0] trap_pc;
    wire [31:0] rs_a0;
    wire [31:0] id_jal_redirect_target;
    wire [31:0] ex_branch_redirect_target;
    wire [31:0] redirect_target;
    wire [0 :0] mem_is_mmio    ;
    wire [0 :0] mem_ram_fault;        
    wire [0 :0] mem_device_fault;     
    wire [0 :0] mem_local_range_fault;
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

    wire id_uses_rs1;
    wire id_uses_rs2;

    // ============= 数据冒险 load-use load -> EX 冒险请求 ==============
    wire load_use_hazard = if_id_valid && (
        (id_ex_valid && id_ex_mem_ctrl[`MEM_CTRL_READ] && id_ex_rd_addr != 0 &&
         ((id_uses_rs1 && id_rs1_addr == id_ex_rd_addr) ||
          (id_uses_rs2 && id_rs2_addr == id_ex_rd_addr))) ||
        (ex_mem_valid && ex_mem_mem_ctrl[`MEM_CTRL_READ] && ex_mem_rd_addr != 0 &&
         ((id_uses_rs1 && id_rs1_addr == ex_mem_rd_addr) ||
          (id_uses_rs2 && id_rs2_addr == ex_mem_rd_addr))));

    // CSR 年轻指令读，年老指令写，规则：年老指令优先wb提交，年轻指令stall等待。
    wire [11:0] id_csr_addr     = csr_ctrl[`CSR_CTRL_ADDR_MSB:`CSR_CTRL_ADDR_LSB];
    wire [11:0] id_ex_csr_addr  = id_ex_csr_ctrl[`CSR_CTRL_ADDR_MSB:`CSR_CTRL_ADDR_LSB];
    wire [11:0] ex_mem_csr_addr = ex_mem_csr_ctrl[`CSR_CTRL_ADDR_MSB:`CSR_CTRL_ADDR_LSB];
    wire id_csr_read            = csr_ctrl[`CSR_CTRL_READ];
    wire id_ex_csr_write        = id_ex_valid  && id_ex_csr_ctrl[`CSR_CTRL_WRITE]; 
    wire ex_mem_csr_write       = ex_mem_valid && ex_mem_csr_ctrl[`CSR_CTRL_WRITE];

    wire csr_state_raw_hazard = (if_id_valid && id_csr_read) &&
                                ((id_ex_csr_write  && (id_csr_addr == id_ex_csr_addr)) ||
                                 (ex_mem_csr_write && (id_csr_addr == ex_mem_csr_addr))) ;
    wire id_stall_req = load_use_hazard || csr_state_raw_hazard;

    // ============= jal / jalr + branch redirect ==========================
    // 各 stage 只产生 redirect request/target，顶层统一结合 valid/stall 生成 fire。
    wire id_jal_redirect_fire = if_id_valid     &&
                                !id_exc_valid   &&
                                !id_stall_req   && 
                                ex_allowin      && 
                                !sys_or_irq_redirect_fire   &&
                                !ex_branch_redirect_fire    && 
                                pc_ctrl[`IS_JAL];        
                                
    wire ex_redirect_req;
    wire ex_branch_redirect_fire = ex_fire && 
                                   !ex_exc_valid &&
                                   ex_redirect_final;
    // ecall mret wb_sys 重定向请求

    wire wb_sys_redirect_req  = mem_wb_sys_ctrl[`SYS_CTRL_TRAP_ENTER] ||
                                mem_wb_sys_ctrl[`SYS_CTRL_MRET];
    wire wb_sys_redirect_fire = wb_retire_fire && wb_sys_redirect_req;
    // interrupt 重定向
    wire wb_csr_write_fire    = wb_retire_fire && mem_wb_csr_ctrl[`CSR_CTRL_WRITE];

    wire irq_take = !wb_trap_fire &&
                    interrupt_valid && 
                    !rst            &&
                    !wb_sys_redirect_fire &&      // ecall mret 优先中断执行
                    !wb_csr_write_fire &&         // csr 写优先中断执行
                    !is_ebreak_eff && !halt && !faulted &&
                    !mem_issued && 
                    !(mem_wb_valid && wb_memory);

    wire [31:0] irq_resume_pc = ex_mem_valid ? ex_mem_pc :
                                id_ex_valid  ? id_ex_pc  :
                                if_id_valid  ? if_id_pc  : if_pc;
    // 没有中断请求，正常输入mem_wb_pc 也就是ecall/mret
    // 有中断请求，csr保留最老的被kill的指令pc用于恢复时候
    assign trap_pc = irq_take ? irq_resume_pc : mem_wb_pc;         

    // branch + jal + ecall + mret + interrupt  重定向决策
    wire sys_or_irq_redirect_fire = wb_trap_fire || wb_sys_redirect_fire || irq_take;
    wire redirect_fire            = sys_or_irq_redirect_fire ||
                                    ex_branch_redirect_fire  || 
                                    id_jal_redirect_fire;

    assign redirect_target        = sys_or_irq_redirect_fire ? trap_target     :
                                    ex_branch_redirect_fire  ? ex_target_final : id_jal_redirect_target;

    // ID redirect 清除 IF 中一条年轻指令；EX redirect 清除 IF、ID 两条年轻指令。
    
    wire if_id_flush     = redirect_fire;
    wire id_ex_flush     = ex_branch_redirect_fire || sys_or_irq_redirect_fire;
    wire ex_mem_flush    = sys_or_irq_redirect_fire;
    wire mem_wb_flush    = sys_or_irq_redirect_fire;
   
    // Reception capacity is computed backwards. WB drains every cycle.
    // A valid slot is not a transfer: only *_fire moves its dynamic instruction.
    // wb_fire 代表有没有 有效指令存在  
    wire wb_fire         = mem_wb_valid && !rst && !faulted && !halt;       
    wire mem_operation   = ex_mem_mem_ctrl[`MEM_CTRL_READ] || ex_mem_mem_ctrl[`MEM_CTRL_WRITE];
    reg mem_issued;
    reg wb_memory;
    reg faulted;
    // 请求已经交出，成功/错误响应已经准备好，本级具备完成条件。
    wire mem_complete    = mem_exc_valid  ||                //携带异常       → 本级已有处理结论，可以向 WB 运输异常。
                           !mem_operation ||                //非访存指令     → 不需要等待内存。
                           (mem_issued && mem_resp_valid);  //访存响应已返回 → 本次事务结束，成功或失败分别携带结果。
    // 允许进入mem访存阶段：对应级流水线有效的 && 当前拍访存完成
    wire mem_allowin     = !ex_mem_valid  || mem_complete;
    // mem访问 当前访存指令完成并进入 WB
    wire mem_fire        = !rst && !faulted && !halt && ex_mem_valid && mem_complete && !mem_wb_flush;
    wire ex_allowin      = !id_ex_valid || mem_allowin;
    wire ex_fire         = !rst && !faulted && !halt && id_ex_valid && mem_allowin && !ex_mem_flush;
    wire id_allowin      = !if_id_valid || (!id_stall_req && ex_allowin);
    // if访问 当前取指指令完成并进入 ID
    wire id_fire         = !rst && !faulted && !halt && if_id_valid && !id_stall_req && ex_allowin && !id_ex_flush;
    wire if_capture_fire = if_resp_fire && if_live && !redirect_fire && !if_resp_err && !faulted && !halt;

    wire if_id_valid_sel            = if_capture_fire;
    wire [31:0] if_id_pc_sel        = if_resp_pc;
    wire [31:0] if_id_pc_plus4_sel  = if_resp_pc + 32'd4;
    wire [31:0] if_id_inst_sel      = if_rdata;
    wire id_ex_valid_sel            = id_fire;
    wire ex_mem_valid_sel           = ex_fire;
    wire mem_wb_valid_sel           = mem_fire;
    wire mem_stage_valid            = ex_mem_valid  && 
                                      !mem_wb_flush && 
                                      !faulted && !halt;
    wire mem_mmio_load_fire         = mem_fire      && 
                                      mem_operation && 
                                      mem_read_req  && 
                                      mem_is_mmio   &&
                                      !mem_exc_valid;
    wire mem_mmio_access            = mem_is_mmio && !mem_exc_valid ;
    wire pc_enable                  = redirect_fire || if_capture_fire;

    // Once D is issued it is irrevocable through WB. A later IRQ waits.
    always @(posedge clk) begin
        if (rst) begin 
            mem_issued<=0; 
            wb_memory<=0; 
        end else begin
            if (mem_req_fire) mem_issued<=1; //1：请求已经交出，正在等这条指令完成
            if (mem_fire)     mem_issued<=0; //0：当前 MEM 指令的请求还没交出
            //对齐当前WB指令是不是当前的L/S指令
            wb_memory <= mem_fire && mem_operation;
        end
    end

    // Freeze resolved EX results before an older MEM wait outlives its bypass.
    reg         ex_saved;
    reg [31:0]  held_alu, 
                held_rs2, 
                held_csr_read, 
                held_csr_write, 
                held_target;
    reg         held_redirect;
    wire [31:0] ex_alu_final        = ex_saved ? held_alu : alu_result;
    wire [31:0] ex_rs2_final        = ex_saved ? held_rs2 : ex_rs2_value;
    wire [31:0] ex_csr_read_final   = ex_saved ? held_csr_read : csr_rdata;
    wire [31:0] ex_csr_write_final  = ex_saved ? held_csr_write : csr_wdata;
    wire [31:0] ex_target_final     = ex_saved ? held_target : ex_branch_redirect_target;
    wire ex_redirect_final          = ex_saved ? held_redirect : ex_redirect_req;

    always @(posedge clk) begin
        if (rst || id_ex_flush || ex_allowin) ex_saved <= 0;
        else if (id_ex_valid && !ex_saved) begin
            ex_saved        <=1; 
            held_alu        <=alu_result; 
            held_rs2        <=ex_rs2_value;
            held_csr_read   <=csr_rdata; 
            held_csr_write  <=csr_wdata;
            held_target     <=ex_branch_redirect_target; 
            held_redirect   <=ex_redirect_req;
        end
    end
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

    wire [31:0] id_rs1_rdata = wb_id_rs1_hit ? wb_rd_wdata : reg_rs1_rdata;
    wire [31:0] id_rs2_rdata = wb_id_rs2_hit ? wb_rd_wdata : reg_rs2_rdata;
    wire [31:0] ex_rs2_value;

    // ================= IF/MEM 共享端口连接 =================
    // MEM 保留控制拆包和 LSU；共享端口只接收读写需求。
    wire mem_read_req;
    wire mem_write_req;

    // IF reserves its eventual IF/ID slot before offering a request. This
    // prevents a blocked IF response from monopolizing the port needed by MEM.
    reg if_busy,     // 锁存响应返回被接受的繁忙状态
        if_live;     // 表示返回指令是否还需要被执行？redirect已经改变了对应的生命状态
    wire if_req = !rst && !halt && !faulted && !if_busy && id_allowin && !redirect_fire;
    // Conservative oldest-instruction permission, obtained BEFORE presentation.
    wire mem_req = !mem_exc_valid &&
                   !rst && 
                   !halt && 
                   !faulted && 
                   !mem_issued && 
                   !mem_wb_valid &&
                   (mem_read_req || mem_write_req);
                   
    wire if_resp_ready  = !rst && (!if_live || redirect_fire || id_allowin || faulted || halt);
    wire mem_resp_ready = !rst;     // 每一个上升沿都接受来自MEM阶段访存的数据
    wire if_resp_fire   = if_resp_valid && if_resp_ready;
    wire mem_resp_fire  = mem_resp_valid && mem_resp_ready;
    wire [31:0] if_rdata, 
                mem_wdata, 
                mem_rdata_raw, 
                if_resp_pc;
    wire [3:0] mem_wmask;
    wire if_resp_valid,
         if_req_fire, 
         mem_resp_valid, 
         mem_req_fire;
    wire if_resp_err, 
         mem_resp_err;
    wire [2:0] mem_lsu      = ex_mem_mem_ctrl[`MEM_CTRL_LSU_MSB:`MEM_CTRL_LSU_LSB];
    wire [1:0] mem_size     = (mem_lsu == 3'd0 || mem_lsu == 3'd5) ? 2'd2 :
                              (mem_lsu == 3'd1 || mem_lsu == 3'd3 || mem_lsu == 3'd6) ? 2'd1 : 2'd0;
    always @(posedge clk) begin
        if (rst) begin 
            if_busy <=0; 
            if_live <=0; 
        end else begin
            if (if_req_fire) begin 
                if_busy <=1; 
                if_live <=1; 
                end
            if (redirect_fire) 
                if_live <=0;
            if (if_resp_fire) begin 
                if_busy <=0; 
                if_live <=0; 
                end
        end
    end
    // E transports errors and stops with dynamic identity. Precise error traps
    // (mtval/mcause and privilege checks) are the subsequent X work package.

    wire if_fault  = if_resp_fire && if_live && !redirect_fire && if_resp_err;
    always @(posedge clk) begin
        if (rst)
            faulted <= 1'b0;
        else if (!faulted && if_fault) begin
            faulted <= 1'b1;
            dpi_mem_fault(if_resp_pc, if_resp_pc, 0);
        end
    end

    wire bus_req_valid, bus_req_ready, bus_resp_valid, bus_resp_ready, bus_resp_err;
    wire [33:0] bus_req_addr;
    wire [1 :0] bus_req_kind, 
                bus_req_size, 
                bus_req_priv;
    wire        bus_req_attr;
    wire [31:0] bus_req_wdata, bus_resp_rdata;
    wire [3 :0] bus_req_wmask;

// ===================== 精准异常同步处理 ==============================
    wire        if_id_exc_valid_in;
    wire [31:0] if_id_exc_cause_in;
    wire [31:0] if_id_exc_tval_in; 

    assign if_id_exc_valid_in = 1'b0;
    assign if_id_exc_cause_in = 32'b0;
    assign if_id_exc_tval_in  = 32'b0;

    wire        if_id_exc_valid;        
    wire [31:0] if_id_exc_cause;
    wire [31:0] if_id_exc_tval; 

    wire        id_ex_exc_valid;
    wire [31:0] id_ex_exc_cause;
    wire [31:0] id_ex_exc_tval;

    wire        ex_mem_exc_valid;
    wire [31:0] ex_mem_exc_cause;
    wire [31:0] ex_mem_exc_tval;

    wire        mem_wb_exc_valid;
    wire [31:0] mem_wb_exc_cause;
    wire [31:0] mem_wb_exc_tval;

    
    wire illegal_inst;                      // exception_code = 2
    wire jal_target_misaligned ;            // exception_code = 0 
    
    // ex阶段最后的跳转指令就是jalr + branch 跳转地址，直接顶层检查
    wire ex_target_misaligned_final = ex_redirect_final &&
                                     (ex_target_final[1:0] != 2'b00);   
    /*
    本级输出异常有效 = 本级指令有效 &&
                     (指令已携带异常 || 本级新检测到异常);
    非法指令到达 WB
    ↓
    wb_trap_fire=1，wb_retire_fire=0
        ├─ 禁止普通 GPR/CSR 写入
        ├─ CSR 保存异常现场
        ├─ redirect 到 mtvec
        ├─ kill 年轻指令
        └─ 本拍不接受 IRQ
    */
    // id阶段的同步异常有效处理
    wire id_exc_valid = if_id_valid &&
                       (if_id_exc_valid || 
                        illegal_inst    || 
                        jal_target_misaligned);

    wire [31:0] id_exc_cause = !id_exc_valid         ? 32'b0 :
                               if_id_exc_valid       ? if_id_exc_cause :
                               illegal_inst          ? `EXC_ILLEGAL_INST :
                               jal_target_misaligned ? `EXC_INST_ADDR_MISALIGNED : 32'b0;
    wire [31:0] id_exc_tval  = !id_exc_valid         ? 32'b0 :
                               if_id_exc_valid       ? if_id_exc_tval :
                               illegal_inst          ? if_id_inst :
                               jal_target_misaligned ? id_jal_redirect_target :
                                                       32'b0;
    // ex阶段的同步异常有效处理+并且需要进行ex
    wire ex_exc_valid = id_ex_valid &&
                        (id_ex_exc_valid || ex_target_misaligned_final);

    wire [31:0] ex_exc_cause =  !ex_exc_valid              ? 32'b0 :
                                id_ex_exc_valid            ? id_ex_exc_cause :
                                ex_target_misaligned_final ? `EXC_INST_ADDR_MISALIGNED :
                                     32'b0;

    wire [31:0] ex_exc_tval =   !ex_exc_valid              ? 32'b0 :
                                id_ex_exc_valid            ? id_ex_exc_tval :
                                ex_target_misaligned_final ? ex_target_final :
                                     32'b0;
                               
    // wb 异常处理   有效指令存在的同时检查是否携带异常
    wire wb_retire_fire = wb_fire && !mem_wb_exc_valid;
    wire wb_trap_fire   = wb_fire &&  mem_wb_exc_valid;

    // 1、禁止普通GPR写
    wire reg_write_eff  = wb_retire_fire && reg_write_raw;
    wire is_ebreak_eff  = wb_retire_fire && is_ebreak_raw;

    // mem 阶段 load /store 精准异常对齐处理
    wire mem_resp_fault   = mem_resp_fire && mem_issued && mem_resp_err;
    wire mem_access_fault = mem_local_range_fault ||
                            mem_device_fault ||
                            mem_resp_fault;

    wire mem_fault        =   mem_ram_fault || mem_access_fault ;
    wire mem_exc_valid          = ex_mem_valid && (ex_mem_exc_valid || mem_fault);
    wire [31:0] mem_exc_cause   = !mem_exc_valid    ? 32'b0 :
                                  ex_mem_exc_valid  ? ex_mem_exc_cause :
                                  mem_access_fault  ? 
                                  (ex_mem_mem_ctrl[`MEM_CTRL_WRITE] ? `EXC_STORE_ACCESS_FAULT 
                                                                    :  `EXC_LOAD_ACCESS_FAULT) :
                                  mem_ram_fault     ?
                                  (ex_mem_mem_ctrl[`MEM_CTRL_WRITE] ? `EXC_STORE_ADDR_MISALIGNED 
                                                                    : `EXC_LOAD_ADDR_MISALIGNED) :32'b0 ;
    wire [31:0] mem_exc_tval    = !mem_exc_valid    ? 32'b0 :
                                  ex_mem_exc_valid  ? ex_mem_exc_tval  : ex_mem_alu_result ;

    
















    mem_port u_mem_port (
        .clk            (clk              ),    // << i
        .rst            (rst              ),    // << i
        .if_req         (if_req           ),    // << i
        .if_addr        (if_pc            ),    // << i
        .mem_req        (mem_req          ),    // << i
        .mem_read       (mem_read_req     ),    // << i
        .mem_write      (mem_write_req    ),    // << i
        .mem_addr       (ex_mem_alu_result),    // << i
        .mem_wdata      (mem_wdata        ),    // << i
        .mem_wmask      (mem_wmask        ),    // << i
        .mem_size       (mem_size         ),    // << i
        .mem_device     (mem_is_mmio      ),    // << i
        .if_resp_ready  (if_resp_ready    ),    // << i
        .mem_resp_ready (mem_resp_ready   ),    // << i
        .req_ready      (bus_req_ready    ),    // << i
        .resp_valid     (bus_resp_valid   ),    // << i
        .resp_rdata     (bus_resp_rdata   ),    // << i
        .resp_err       (bus_resp_err     ),    // << i

        .if_resp_valid  (if_resp_valid    ),    // >> o
        .mem_resp_valid (mem_resp_valid   ),    // >> o
        .if_resp_pc     (if_resp_pc       ),    // >> o
        .if_rdata       (if_rdata         ),    // >> o
        .mem_rdata_raw  (mem_rdata_raw    ),    // >> o
        .if_resp_err    (if_resp_err      ),    // >> o
        .mem_resp_err   (mem_resp_err     ),    // >> o
        .if_req_fire    (if_req_fire      ),    // >> o
        .mem_req_fire   (mem_req_fire     ),    // >> o
        .req_valid      (bus_req_valid    ),    // >> o
        .req_addr       (bus_req_addr     ),    // >> o
        .req_kind       (bus_req_kind     ),    // >> o
        .req_size       (bus_req_size     ),    // >> o
        .req_priv       (bus_req_priv     ),    // >> o
        .req_mem_attr   (bus_req_attr     ),    // >> o
        .req_wdata      (bus_req_wdata    ),    // >> o
        .req_wmask      (bus_req_wmask    ),    // >> o
        .resp_ready     (bus_resp_ready   )     // >> o
    );
    dpi_mem u_dpi_mem (
        .clk          (clk           ),    // << i
        .rst          (rst           ),    // << i
        .req_valid    (bus_req_valid ),    // << i
        .resp_ready   (bus_resp_ready),    // << i
        .req_addr     (bus_req_addr  ),    // << i
        .req_kind     (bus_req_kind  ),    // << i
        .req_size     (bus_req_size  ),    // << i
        .req_priv     (bus_req_priv  ),    // << i
        .req_mem_attr (bus_req_attr  ),    // << i
        .req_wdata    (bus_req_wdata ),    // << i
        .req_wmask    (bus_req_wmask ),    // << i

        .resp_valid   (bus_resp_valid),    // >> o
        .req_ready    (bus_req_ready ),    // >> o
        .resp_rdata   (bus_resp_rdata),    // >> o
        .resp_err     (bus_resp_err  )     // >> o
    );

    // ============ 流水线寄存变量 ======================
    //if/id
    wire  [0 :0]  if_id_valid ;
    wire  [31:0]  if_id_pc;
    wire  [31:0]  if_id_pc_plus4;
    wire  [31:0]  if_id_inst;

    // id/ex
    wire                          id_ex_valid       ;
    wire  [31:0]                  id_ex_pc          ;
    wire  [31:0]                  id_ex_pc_plus4    ;
    wire  [31:0]                  id_ex_inst        ;
    wire  [4 :0]                  id_ex_rs1_addr    ;
    wire  [4 :0]                  id_ex_rs2_addr    ;
    wire  [4 :0]                  id_ex_rd_addr     ;
    wire  [31:0]                  id_ex_rs1_rdata   ;
    wire  [31:0]                  id_ex_rs2_rdata   ;
    wire  [31:0]                  id_ex_imm_out     ;
    wire  [`EX_CTRL_WIDTH-1 :0]   id_ex_ex_ctrl     ;
    wire  [`MEM_CTRL_WIDTH-1:0]   id_ex_mem_ctrl    ;
    wire  [`WB_CTRL_WIDTH-1 :0]   id_ex_wb_ctrl     ;
    wire  [`CSR_CTRL_WIDTH-1:0]   id_ex_csr_ctrl    ;
    wire  [`SYS_CTRL_WIDTH-1:0]   id_ex_sys_ctrl    ;
    wire  [`PC_CTRL_WIDTH-1:0]    id_ex_pc_ctrl     ;
    // ex/mem
    wire                          ex_mem_valid      ;
    wire  [31:0]                  ex_mem_pc         ;
    wire  [31:0]                  ex_mem_pc_plus4   ;
    wire  [31:0]                  ex_mem_inst       ;
    wire  [4 :0]                  ex_mem_rd_addr    ;
    wire  [31:0]                  ex_mem_alu_result ;
    wire  [31:0]                  ex_mem_rs2_rdata  ;
    wire  [31:0]                  ex_mem_csr_rdata  ;
    wire  [31:0]                  ex_mem_csr_wdata  ;
    wire  [`MEM_CTRL_WIDTH-1:0]   ex_mem_mem_ctrl   ;
    wire  [`WB_CTRL_WIDTH-1 :0]   ex_mem_wb_ctrl    ;
    wire  [`CSR_CTRL_WIDTH-1:0]   ex_mem_csr_ctrl   ;
    wire  [`SYS_CTRL_WIDTH-1:0]   ex_mem_sys_ctrl   ;
    
    // mem/wb

    wire                          mem_wb_valid      ;
    wire  [31:0]                  mem_wb_pc         ;
    wire  [31:0]                  mem_wb_pc_plus4   ;
    wire  [31:0]                  mem_wb_inst       ;
    wire  [4 :0]                  mem_wb_rd_addr    ;
    wire  [31:0]                  mem_wb_alu_result ;
    wire  [31:0]                  mem_wb_mem_rdata  ;
    wire  [31:0]                  mem_wb_csr_rdata  ;
    wire  [31:0]                  mem_wb_csr_wdata  ;
    wire  [`WB_CTRL_WIDTH-1 :0]   mem_wb_wb_ctrl    ;
    wire  [`CSR_CTRL_WIDTH-1:0]   mem_wb_csr_ctrl   ;
    wire  [`SYS_CTRL_WIDTH-1:0]   mem_wb_sys_ctrl   ;
    wire                          mem_wb_is_mmio    ;


    if_stage u_if_stage (
        .clk             (clk            ),    // << i
        .rst             (rst            ),    // << i
        .pc_enable       (pc_enable      ),    // << i
        .redirect_fire   (redirect_fire  ),    // << i
        .redirect_target (redirect_target),    // << i
        .inst_rdata      (if_rdata       ),    // << i

        .pc              (if_pc          ),    // >> o
        .pc_plus4        (if_pc_plus4    ),    // >> o
        .inst            (instr          )     // >> o
    );


    if_id_reg u_if_id_reg (
        .clk                    (clk               ),    // << i
        .rst                    (rst               ),    // << i
        .enable                 (id_allowin        ),    // << i
        .flush                  (if_id_flush       ),    // << i
        .if_id_valid_in         (if_id_valid_sel   ),    // << i
        .if_id_pc_in            (if_id_pc_sel      ),    // << i
        .if_id_pc_plus4_in      (if_id_pc_plus4_sel),    // << i
        .if_id_inst_in          (if_id_inst_sel    ),    // << i
        .if_id_exc_valid_in     (if_id_exc_valid_in),    // << i
        .if_id_exc_cause_in     (if_id_exc_cause_in),    // << i
        .if_id_exc_tval_in      (if_id_exc_tval_in ),    // << i

        .if_id_valid            (if_id_valid       ),    // >> o
        .if_id_pc               (if_id_pc          ),    // >> o
        .if_id_pc_plus4         (if_id_pc_plus4    ),    // >> o
        .if_id_inst             (if_id_inst        ),    // >> o
        .if_id_exc_valid        (if_id_exc_valid   ),    // >> o
        .if_id_exc_cause        (if_id_exc_cause   ),    // >> o
        .if_id_exc_tval         (if_id_exc_tval    )     // >> o
    );



    id_stage u_id_stage (
        .inst                       (if_id_inst            ),    // << i
        .if_id_pc                   (if_id_pc              ),    // << i

        .rs1_addr                   (id_rs1_addr           ),    // >> o
        .rs2_addr                   (id_rs2_addr           ),    // >> o
        .rd_addr                    (id_rd_addr            ),    // >> o
        .id_uses_rs1                (id_uses_rs1           ),    // >> o
        .id_uses_rs2                (id_uses_rs2           ),    // >> o
        .imm_out                    (imm_out               ),    // >> o
        .jal_redirect_pc            (id_jal_redirect_target),    // >> o
        .ex_ctrl                    (ex_ctrl               ),    // >> o
        .mem_ctrl                   (mem_ctrl              ),    // >> o
        .wb_ctrl                    (wb_ctrl               ),    // >> o
        .csr_ctrl                   (csr_ctrl              ),    // >> o
        .sys_ctrl                   (sys_ctrl              ),    // >> o
        .pc_ctrl                    (pc_ctrl               ),    // >> o
        .illegal_inst               (illegal_inst          ),    // >> o     
        .jal_target_misaligned      (jal_target_misaligned )    // >> o     
    );
    id_ex_reg u_id_ex_reg (
        .clk                (clk            ),    // << i
        .rst                (rst            ),    // << i
        .enable             (ex_allowin     ),    // << i
        .flush              (id_ex_flush    ),    // << i
        .id_ex_valid_in     (id_ex_valid_sel),    // << i
        .id_ex_pc_in        (if_id_pc       ),    // << i
        .id_ex_pc_plus4_in  (if_id_pc_plus4 ),    // << i
        .id_ex_inst_in      (if_id_inst     ),    // << i
        .id_ex_exc_valid_in (id_exc_valid   ),    // << i
        .id_ex_exc_cause_in (id_exc_cause   ),    // << i
        .id_ex_exc_tval_in  (id_exc_tval    ),    // << i
        .id_ex_rs1_addr_in  (id_rs1_addr    ),    // << i
        .id_ex_rs2_addr_in  (id_rs2_addr    ),    // << i
        .id_ex_rd_addr_in   (id_rd_addr     ),    // << i
        .id_ex_rs1_rdata_in (id_rs1_rdata   ),    // << i
        .id_ex_rs2_rdata_in (id_rs2_rdata   ),    // << i
        .id_ex_imm_out_in   (imm_out        ),    // << i
        .id_ex_ex_ctrl_in   (ex_ctrl        ),    // << i
        .id_ex_mem_ctrl_in  (mem_ctrl       ),    // << i
        .id_ex_wb_ctrl_in   (wb_ctrl        ),    // << i
        .id_ex_csr_ctrl_in  (csr_ctrl       ),    // << i
        .id_ex_sys_ctrl_in  (sys_ctrl       ),    // << i
        .id_ex_pc_ctrl_in   (pc_ctrl        ),    // << i

        .id_ex_valid        (id_ex_valid    ),    // >> o
        .id_ex_pc           (id_ex_pc       ),    // >> o
        .id_ex_pc_plus4     (id_ex_pc_plus4 ),    // >> o
        .id_ex_inst         (id_ex_inst     ),    // >> o
        .id_ex_exc_valid    (id_ex_exc_valid),    // >> o
        .id_ex_exc_cause    (id_ex_exc_cause),    // >> o
        .id_ex_exc_tval     (id_ex_exc_tval ),    // >> o
        .id_ex_rs1_addr     (id_ex_rs1_addr ),    // >> o
        .id_ex_rs2_addr     (id_ex_rs2_addr ),    // >> o
        .id_ex_rd_addr      (id_ex_rd_addr  ),    // >> o
        .id_ex_rs1_rdata    (id_ex_rs1_rdata),    // >> o
        .id_ex_rs2_rdata    (id_ex_rs2_rdata),    // >> o
        .id_ex_imm_out      (id_ex_imm_out  ),    // >> o
        .id_ex_ex_ctrl      (id_ex_ex_ctrl  ),    // >> o
        .id_ex_mem_ctrl     (id_ex_mem_ctrl ),    // >> o
        .id_ex_wb_ctrl      (id_ex_wb_ctrl  ),    // >> o
        .id_ex_csr_ctrl     (id_ex_csr_ctrl ),    // >> o
        .id_ex_sys_ctrl     (id_ex_sys_ctrl ),    // >> o
        .id_ex_pc_ctrl      (id_ex_pc_ctrl  )     // >> o
    );

    ex_stage u_ex_stage (
        .pc                              (id_ex_pc                      ),    // << i
        .rs1_rdata                       (id_ex_rs1_rdata               ),    // << i
        .rs2_rdata                       (id_ex_rs2_rdata               ),    // << i
        .rs1_addr                        (id_ex_rs1_addr                ),    // << i
        .rs2_addr                        (id_ex_rs2_addr                ),    // << i
        .imm_out                         (id_ex_imm_out                 ),    // << i
        .csr_rdata                       (csr_rdata                     ),    // << i
        .ex_ctrl                         (id_ex_ex_ctrl                 ),    // << i
        .csr_ctrl                        (id_ex_csr_ctrl                ),    // << i
        .pc_ctrl                         (id_ex_pc_ctrl                 ),    // << i
        // forwarding 输入               
        .ex_mem_rd_addr                  (ex_mem_rd_addr                ),    // << i
        .ex_mem_alu_result               (ex_mem_alu_result             ),    // << i
        .ex_mem_csr_rdata                (ex_mem_csr_rdata              ),    // << i
        .ex_mem_pc_plus4                 (ex_mem_pc_plus4               ),    // << i
        .ex_mem_valid                    (ex_mem_valid                  ),    // << i
        .ex_mem_wb_ctrl                  (ex_mem_wb_ctrl                ),    // << i
        .mem_wb_rd_addr                  (mem_wb_rd_addr                ),    // << i
        .mem_wb_wb_ctrl                  (mem_wb_wb_ctrl                ),    // << i
        .wb_rd_wdata                     (wb_rd_wdata                   ),    // << i
        .mem_wb_valid                    (mem_wb_valid                  ),    // << i
        
        .alu_result                      (alu_result                    ),    // >> o
        .csr_wdata                       (csr_wdata                     ),    // >> o
        .ex_rs2_value                    (ex_rs2_value                  ),    // >> o
        .redirect_req                    (ex_redirect_req               ),    // >> o
        .redirect_target                 (ex_branch_redirect_target     )    // >> o


    );

    ex_mem_reg u_ex_mem_reg (
        .clk                  (clk               ),    // << i
        .rst                  (rst               ),    // << i
        .enable               (mem_allowin       ),    // << i
        .flush                (ex_mem_flush      ),    // << i
        .ex_mem_valid_in      (ex_mem_valid_sel  ),    // << i
        .ex_mem_pc_in         (id_ex_pc          ),    // << i
        .ex_mem_pc_plus4_in   (id_ex_pc_plus4    ),    // << i
        .ex_mem_inst_in       (id_ex_inst        ),    // << i
        .ex_mem_exc_valid_in  (ex_exc_valid      ),    // << i
        .ex_mem_exc_cause_in  (ex_exc_cause      ),    // << i
        .ex_mem_exc_tval_in   (ex_exc_tval       ),    // << i
        .ex_mem_rd_addr_in    (id_ex_rd_addr     ),    // << i
        .ex_mem_alu_result_in (ex_alu_final      ),    // << i
        .ex_mem_rs2_rdata_in  (ex_rs2_final      ),    // << i
        .ex_mem_mem_ctrl_in   (id_ex_mem_ctrl    ),    // << i
        .ex_mem_wb_ctrl_in    (id_ex_wb_ctrl     ),    // << i
        .ex_mem_csr_rdata_in  (ex_csr_read_final ),    // << i
        .ex_mem_csr_wdata_in  (ex_csr_write_final),    // << i
        .ex_mem_csr_ctrl_in   (id_ex_csr_ctrl    ),    // << i
        .ex_mem_sys_ctrl_in   (id_ex_sys_ctrl    ),    // << i

        .ex_mem_valid         (ex_mem_valid      ),    // >> o
        .ex_mem_pc            (ex_mem_pc         ),    // >> o
        .ex_mem_pc_plus4      (ex_mem_pc_plus4   ),    // >> o
        .ex_mem_inst          (ex_mem_inst       ),    // >> o
        .ex_mem_exc_valid     (ex_mem_exc_valid  ),    // >> o
        .ex_mem_exc_cause     (ex_mem_exc_cause  ),    // >> o
        .ex_mem_exc_tval      (ex_mem_exc_tval   ),    // >> o
        .ex_mem_rd_addr       (ex_mem_rd_addr    ),    // >> o
        .ex_mem_alu_result    (ex_mem_alu_result ),    // >> o
        .ex_mem_rs2_rdata     (ex_mem_rs2_rdata  ),    // >> o
        .ex_mem_mem_ctrl      (ex_mem_mem_ctrl   ),    // >> o
        .ex_mem_wb_ctrl       (ex_mem_wb_ctrl    ),    // >> o
        .ex_mem_csr_rdata     (ex_mem_csr_rdata  ),    // >> o
        .ex_mem_csr_wdata     (ex_mem_csr_wdata  ),    // >> o
        .ex_mem_csr_ctrl      (ex_mem_csr_ctrl   ),    // >> o
        .ex_mem_sys_ctrl      (ex_mem_sys_ctrl   )     // >> o
    );
 
    mem_stage u_mem_stage (
        .mem_ctrl             (ex_mem_mem_ctrl  ),    // << i
        .mem_valid            (mem_stage_valid  ),    // << i
        .mem_addr             (ex_mem_alu_result),    // << i
        .mem_wdata_raw        (ex_mem_rs2_rdata ),    // << i
        .mem_rdata_raw        (mem_rdata_raw    ),    // << i
    
        .mem_rdata            (mem_rdata        ),    // >> o
        .mem_wdata            (mem_wdata        ),    // >> o
        .mem_wmask            (mem_wmask        ),    // >> o
        .mem_is_mmio          (mem_is_mmio      ),    // >> o
        .mem_read             (mem_read_req     ),    // >> o
        .mem_write            (mem_write_req    ),    // >> o
        .ram_fault            (mem_ram_fault    ),    // >> o  
        .device_fault         (mem_device_fault ),    // >> o  
        .local_range_fault    (mem_local_range_fault) // >> o 
    );

    mem_wb_reg u_mem_wb_reg (
        .clk                  (clk              ),    // << i
        .rst                  (rst              ),    // << i
        .enable               (1'b1             ),    // << i
        .flush                (1'b0             ),    // << i
        .mem_wb_is_mmio_in    (mem_mmio_access  ),    // << i
        .mem_wb_valid_in      (mem_wb_valid_sel ),    // << i
        .mem_wb_pc_in         (ex_mem_pc        ),    // << i
        .mem_wb_pc_plus4_in   (ex_mem_pc_plus4  ),    // << i
        .mem_wb_inst_in       (ex_mem_inst      ),    // << i
        .mem_wb_exc_valid_in  (mem_exc_valid    ),    // << i
        .mem_wb_exc_cause_in  (mem_exc_cause    ),    // << i
        .mem_wb_exc_tval_in   (mem_exc_tval     ),    // << i
        .mem_wb_rd_addr_in    (ex_mem_rd_addr   ),    // << i
        .mem_wb_alu_result_in (ex_mem_alu_result),    // << i
        .mem_wb_mem_rdata_in  (mem_rdata        ),    // << i
        .mem_wb_wb_ctrl_in    (ex_mem_wb_ctrl   ),    // << i
        .mem_wb_csr_rdata_in  (ex_mem_csr_rdata ),    // << i
        .mem_wb_csr_wdata_in  (ex_mem_csr_wdata ),    // << i
        .mem_wb_csr_ctrl_in   (ex_mem_csr_ctrl  ),    // << i
        .mem_wb_sys_ctrl_in   (ex_mem_sys_ctrl  ),    // << i

        .mem_wb_valid         (mem_wb_valid     ),    // >> o
        .mem_wb_pc            (mem_wb_pc        ),    // >> o
        .mem_wb_pc_plus4      (mem_wb_pc_plus4  ),    // >> o
        .mem_wb_inst          (mem_wb_inst      ),    // >> o
        .mem_wb_exc_valid     (mem_wb_exc_valid ),    // >> o
        .mem_wb_exc_cause     (mem_wb_exc_cause ),    // >> o
        .mem_wb_exc_tval      (mem_wb_exc_tval  ),    // >> o
        .mem_wb_is_mmio       (mem_wb_is_mmio   ),    // >> o
        .mem_wb_rd_addr       (mem_wb_rd_addr   ),    // >> o
        .mem_wb_alu_result    (mem_wb_alu_result),    // >> o
        .mem_wb_mem_rdata     (mem_wb_mem_rdata ),    // >> o
        .mem_wb_wb_ctrl       (mem_wb_wb_ctrl   ),    // >> o
        .mem_wb_csr_rdata     (mem_wb_csr_rdata ),    // >> o
        .mem_wb_csr_wdata     (mem_wb_csr_wdata ),    // >> o
        .mem_wb_csr_ctrl      (mem_wb_csr_ctrl  ),    // >> o
        .mem_wb_sys_ctrl      (mem_wb_sys_ctrl  )     // >> o
    );

// c++ 调用 写回事件
always @(posedge clk) begin
    dpi_wb_event({31'b0, wb_fire},
                 {mem_wb_pc},
                 {mem_wb_inst},
                 {31'b0, irq_take},
                 {interrupt_cause},
                 {31'b0, wb_fire && mem_wb_is_mmio}
                );
    dpi_mmio_read_event(
        {31'b0, mem_mmio_load_fire},
        {ex_mem_pc},
        {ex_mem_alu_result},
        {mem_rdata}
    );
end

    wb_stage u_wb_stage (
        .wb_ctrl    (mem_wb_wb_ctrl   ),    // << i
        .mem_rdata  (mem_wb_mem_rdata ),    // << i
        .alu_result (mem_wb_alu_result),    // << i
        .pc_plus4   (mem_wb_pc_plus4  ),    // << i
        .csr_rdata  (mem_wb_csr_rdata ),    // << i

        .rd_wdata   (wb_rd_wdata      )     // >> o
    );
    regfile u_regfile (
        .clk         (clk           ),    // << i
        .rs1_addr    (id_rs1_addr   ),    // << i
        .rs2_addr    (id_rs2_addr   ),    // << i
        .wb_rd_addr  (mem_wb_rd_addr),    // << i
        .wb_rd_wdata (wb_rd_wdata   ),    // << i
        .reg_write   (reg_write_eff ),    // << i

        .rs1_rdata   (reg_rs1_rdata ),    // >> o
        .rs2_rdata   (reg_rs2_rdata ),    // >> o
        .rs_a0       (rs_a0         )     // >> o
    );

    csr u_csr (
        .clk              (clk             ),    // << i
        .rst              (rst             ),    // << i
        .csr_commit_valid (wb_retire_fire  ),    // << i
        .csr_read_ctrl    (id_ex_csr_ctrl  ),    // << i
        .csr_write_ctrl   (mem_wb_csr_ctrl ),    // << i
        .sys_commit_ctrl  (mem_wb_sys_ctrl ),    // << i
        .interrupt_valid  (irq_take        ),    // << i
        .interrupt_cause  (interrupt_cause ),    // << i
        .csr_wdata        (mem_wb_csr_wdata),    // << i
        .trap_pc          (trap_pc         ),    // << i
        .wb_exc_valid     (wb_trap_fire    ),    // << i
        .wb_exc_cause     (mem_wb_exc_cause),    // << i
        .wb_exc_tval      (mem_wb_exc_tval ),     // << i

        .csr_rdata        (csr_rdata       ),    // >> o
        .trap_target      (trap_target     )     // >> o
    );

    halt u_halt (
        .clk       (clk          ),    // << i
        .rst       (rst          ),    // << i
        .is_ebreak (is_ebreak_eff),    // << i
        .pc        (mem_wb_pc    ),    // << i
        .rs_a0     (rs_a0        ),    // << i

        .halt_reg  (halt         ),    // >> o
        .halt_pc   (halt_pc      ),    // >> o
        .halt_ret  (halt_ret     )     // >> o
    );
endmodule
