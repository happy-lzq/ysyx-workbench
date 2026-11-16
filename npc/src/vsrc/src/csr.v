`include "ctrl_defs.vh"
`include "exception.vh"

module csr (
    clk
    ,rst
    ,csr_commit_valid
    ,csr_read_ctrl
    ,csr_write_ctrl
    ,sys_commit_ctrl
    ,interrupt_valid
    ,interrupt_cause
    ,csr_wdata
    ,trap_pc
    ,wb_exc_valid
    ,wb_exc_cause
    ,wb_exc_tval
    ,csr_rdata
    ,trap_target
);
    // 控制流
    input  wire [0:0]                 clk;
    input  wire [0:0]                 rst;
    input  wire [0:0]                 csr_commit_valid;
    input  wire [`CSR_CTRL_WIDTH-1:0] csr_read_ctrl;
    input  wire [`CSR_CTRL_WIDTH-1:0] csr_write_ctrl;
    input  wire [`SYS_CTRL_WIDTH-1:0] sys_commit_ctrl;
    input  wire [0:0]                 interrupt_valid;
    input  wire [31:0]                interrupt_cause;
    // 数据流
    input  wire [31:0]                csr_wdata;
    input  wire [31:0]                trap_pc;

    // 异常控制 
    input wire        wb_exc_valid;
    input wire [31:0] wb_exc_cause;
    input wire [31:0] wb_exc_tval;

    // 输出
    output wire [31:0]                csr_rdata;
    output wire [31:0]                trap_target;

    // === csr_ctrl 拆包 ===
    // csr_rdata
    wire [11:0] csr_read_addr  = csr_read_ctrl[`CSR_CTRL_ADDR_MSB:`CSR_CTRL_ADDR_LSB];
    wire        csr_read       = csr_read_ctrl[`CSR_CTRL_READ];
    // csr_wdata
    wire [11:0] csr_write_addr = csr_write_ctrl[`CSR_CTRL_ADDR_MSB:`CSR_CTRL_ADDR_LSB];
    wire        csr_write_raw  = csr_write_ctrl[`CSR_CTRL_WRITE];

    // === WB trap/MRET 跳转目标 ===
    wire wb_trap = csr_commit_valid &&
                   sys_commit_ctrl[`SYS_CTRL_TRAP_ENTER];

    wire wb_mret = csr_commit_valid &&
                   sys_commit_ctrl[`SYS_CTRL_MRET];

    assign trap_target = (wb_exc_valid || wb_trap || interrupt_valid) ? csr_mtvec :
                          wb_mret                                     ? csr_mepc  : 32'b0;

    // === WB系统提交控制：只负责修改CSR状态 ===
    wire commit_trap_raw = sys_commit_ctrl[`SYS_CTRL_TRAP_ENTER];
    wire commit_mret_raw = sys_commit_ctrl[`SYS_CTRL_MRET];
    wire [31:0] commit_trap_code_raw = sys_commit_ctrl[`SYS_CTRL_TRAP_CODE_MSB:`SYS_CTRL_TRAP_CODE_LSB];

    // === WB提交许可 ===
    wire csr_write = csr_commit_valid &&
                     !trap_enter &&
                     !mret &&
                     csr_write_raw;
    // 异常有效入口
    wire trap_enter = wb_exc_valid      ||                      // WB携带同步异常
                      interrupt_valid   ||                      // 外部中断
                      (csr_commit_valid && commit_trap_raw);    // 现有的异常控制指令-ECALL
    // 异常返回
    wire mret       = csr_commit_valid  && 
                      !interrupt_valid  && 
                      commit_mret_raw;
    // 异常原因同步
    wire [31:0] trap_code = wb_exc_valid    ? wb_exc_cause    :
                            interrupt_valid ? interrupt_cause : commit_trap_code_raw;

    wire [31:0] trap_tval = wb_exc_valid ? wb_exc_tval : 32'b0; 

    parameter mstatus   = 12'h300;
    parameter mtvec     = 12'h305;
    parameter mepc      = 12'h341;
    parameter mcause    = 12'h342;
    parameter mcyclel   = 12'hB00;
    parameter mcycleh   = 12'hB80;
    parameter mip       = 12'h344;
    parameter mie       = 12'h304;
    parameter mtval     = 12'h343;
    parameter mscratch  = 12'h340; 

    // 当前实现：RV32IM/Zicsr、仅 M、无 F/V/S/U/H 或自定义状态扩展。
    // mstatus 只实现 MIE[3]、MPIE[7]、固定 MPP[12:11]=11。
    // 其余未实现/保留字段只读零；未来增加模式时须连同使用逻辑一起扩展。
    localparam [31:0] MSTATUS_SW_WRITE_MASK = 32'h0000_0088;
    localparam [31:0] MSTATUS_M_ONLY        = 32'h0000_1800;
    // 仅实现机器定时器和外部中断；无 MSIP、自定义本地中断或 S 模式源。
    localparam [31:0] MACHINE_IRQ_MASK      = 32'h0000_0880;

    reg [31:0] csr_mstatus;    // 机器状态寄存器
    reg [31:0] csr_mtvec;      // 机器陷阱向量基址
    reg [31:0] csr_mepc;       // 机器异常PC
    reg [31:0] csr_mcause;     // 机器异常编号
    reg [31:0] csr_mip;        // 机器中断挂起
    reg [31:0] csr_mie ;       // 机器中断使能
    reg [31:0] csr_mtval  ;    // 机器陷阱值
    reg [31:0] csr_mscratch ;  // 机器暂存寄存器
    reg [63:0] mcycle_64 ;
    reg [63:0] mcycle_next;

    // RV32 写入只改变所寻址半部。有效软件写优先于该拍整个计数器自增，
    // 另一半保持原值，避免整体自增与切片非阻塞赋值发生重叠。
    always @(*) begin
        mcycle_next = mcycle_64 + 64'd1;
        if (csr_write && (csr_write_addr == mcyclel))
            mcycle_next = {mcycle_64[63:32], csr_wdata};
        else if (csr_write && (csr_write_addr == mcycleh))
            mcycle_next = {csr_wdata, mcycle_64[31:0]};
    end

    assign csr_rdata = csr_read ? ((csr_read_addr == mstatus) ? csr_mstatus      : 
                                   (csr_read_addr == mtvec)   ? csr_mtvec        : 
                                   (csr_read_addr == mepc)    ? csr_mepc         : 
                                   (csr_read_addr == mcause)  ? csr_mcause       : 
                                   (csr_read_addr == mip)     ? csr_mip          :
                                   (csr_read_addr == mie)     ? csr_mie          :
                                   (csr_read_addr == mtval)   ? csr_mtval        :
                                   (csr_read_addr == mscratch)? csr_mscratch     :
                                   (csr_read_addr == mcyclel) ? mcycle_64[31:0]  :
                                   (csr_read_addr == mcycleh) ? mcycle_64[63:32] : 32'h0 ) : 32'h0;

    always @(posedge clk ) begin
        if (rst) begin
            csr_mstatus     <= 32'h0000_1800;       //RISC-V 规定复位后 CPU 处于 M-mode
            csr_mtvec       <= 32'h8000_0000;
            csr_mepc        <= 32'h0;
            csr_mcause      <= 32'h0;
            csr_mip         <= 32'h0;
            csr_mie         <= 32'h0;
            csr_mtval       <= 32'h0;
            csr_mscratch    <= 32'h0;
            mcycle_64       <= 64'b0;
        end else begin
            mcycle_64       <= mcycle_next;
            if (trap_enter) begin
    // 
                case (trap_code)
                    `IRQ_M_TIMER : csr_mip <= {csr_mip[31:8],  1'b0, csr_mip[6:0]};     // clear MTIP (bit 7)
                    `IRQ_M_EXT   : csr_mip <= {csr_mip[31:12], 1'b0, csr_mip[10:0]};    // clear MEIP (bit 11)
                    default: ;
                endcase
    // 异常处理的共同现场 CPU进入trap的异常处理现场
                csr_mepc    <= trap_pc               ;           // 保存异常返回地址
                csr_mcause  <= trap_code             ;           // 11=ecall, 3=ebreak
                csr_mtval   <= trap_tval             ;           // 更多处理信息
                csr_mstatus <= {csr_mstatus[31:13]   , 
                                2'b11                ,           // MPP ← 3  M特权级
                                csr_mstatus[10:8]    ,
                                csr_mstatus[3]       ,           // MPIE ← MIE
                                csr_mstatus[6:4]     ,
                                1'b0                 ,           // MIE ← 0
                                csr_mstatus[2:0]}    ;

             end else if (mret) begin
                 csr_mstatus <= {csr_mstatus[31:13]   , 
                                 2'b11                ,           // MPP ← 11               保存进入 M 模式 trap 前的特权级，供 MRET 返回
                                 csr_mstatus[10:8]    ,
                                 1'b1                 ,           // MPIE ← 1               保存进入 trap 前的 MIE
                                 csr_mstatus[6:4]     ,
                                 csr_mstatus[7]       ,           // MIE ← MPIE(bit7→bit3)  M 模式全局中断使能
                                 csr_mstatus[2:0]}    ;
             end else if (csr_write) begin
                    case (csr_write_addr)               // 软件执行 CSR 指令写入时，对不同模式下的只读/写特权控制 
                    // 1. 核只实现了 M 模式”，还没有实现随 M/S/U 切换的字段权限逻辑
                    // 2. 普通可写位接收数据，只读位保持规定行为，受约束的可写位保存支持的合法值。
                        mstatus : csr_mstatus       <= (csr_wdata & MSTATUS_SW_WRITE_MASK) | MSTATUS_M_ONLY;
                        mtvec   : csr_mtvec         <= {csr_wdata[31:2], 2'b00};        // 当前trap入口模式都是direct
                        mepc    : csr_mepc          <= {csr_wdata[31:2], 2'b00};        // 只支持 IALIGN=32 的实现，mepc[1:0] 始终为 0。RISC-V 特权架构规范：mepc        
                        mcause  : csr_mcause        <= csr_wdata;                       // WLRL 原因字段；支持完整编码存储
                        mip     : csr_mip           <= csr_mip & MACHINE_IRQ_MASK;      // 保持设备 pending，忽略软件数据
                        mie     : csr_mie           <= csr_wdata & MACHINE_IRQ_MASK;    // MEIE[11]、MTIE[7] 可写
                        mtval   : csr_mtval         <= csr_wdata;                       // 地址/非法指令编码，保留全部位
                        mscratch: csr_mscratch      <= csr_wdata;
                        mcyclel, mcycleh: ;                                             // 统一由 mcycle_next 更新
                        default : ;
                    endcase
            end 
        end
    end
endmodule


/*
        CSR部分的同步精准异常处理
    1、CSR地址是否正确？
        指令经过opcode+functs3 得到真确指令，还需要正确的访问地址，非则也算一条错误的非法指令，也就是CSR指令访问的真确性。
    2、当前执行的特权级是否允许访问？
        不同模式特权级对应到CSR指令的访问权限也不一样。
    3、如果有写行为，CSR是否允许写？
        CSR指令读写权限也不一致

*/
