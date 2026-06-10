module csr (
    clk
    ,rst
    ,csr_addr
    ,csr_write
    ,csr_wdata
    ,csr_read
    ,csr_rdata
    ,mret
    ,trap_pc
    ,trap_code         
    ,trap_enter
    ,trap_target
);
    input  wire [0 :0] clk;
    input  wire [0 :0] rst;
    input  wire [0 :0] csr_write;
    input  wire [0 :0] csr_read;
    input  wire [0 :0] mret;
    input  wire [0 :0] trap_enter;
    input  wire [11:0] csr_addr;
    input  wire [31:0] trap_pc;
    input  wire [31:0] csr_wdata;
    input  wire [31:0] trap_code;
    output wire [31:0] csr_rdata;
    output wire [31:0] trap_target;

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
    // 异常编号
    parameter IRQ_M_TIMER = 32'h80000007; 
    parameter IRQ_M_EXT   = 32'h8000000b; 

    reg [31:0] csr_mstatus;    // 机器状态寄存器
    reg [31:0] csr_mtvec;      // 机器陷阱向量基址
    reg [31:0] csr_mepc;       // 机器异常PC
    reg [31:0] csr_mcause;     // 机器异常编号
    reg [31:0] csr_mip;        // 机器中断挂起
    reg [31:0] csr_mie ;       // 机器中断使能
    reg [31:0] csr_mtval  ;    // 机器陷阱值
    reg [31:0] csr_mscratch ;  // 机器暂存寄存器
    reg [63:0] mcycle_64 ;

    assign csr_rdata = csr_read ? ((csr_addr == mstatus) ? csr_mstatus      : 
                                   (csr_addr == mtvec)   ? csr_mtvec        : 
                                   (csr_addr == mepc)    ? csr_mepc         : 
                                   (csr_addr == mcause)  ? csr_mcause       : 
                                   (csr_addr == mip)     ? csr_mip          :
                                   (csr_addr == mie)     ? csr_mie          :
                                   (csr_addr == mtval)   ? csr_mtval        :
                                   (csr_addr == mscratch)? csr_mscratch     :
                                   (csr_addr == mcyclel) ? mcycle_64[31:0]  :
                                   (csr_addr == mcycleh) ? mcycle_64[63:32] :32'h0 ) : 32'h0;


    assign trap_target = trap_enter ? csr_mtvec :               // ecall 保存返回地址
                         mret       ? csr_mepc  : 32'b0;        // mret  进入返回地址

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
            mcycle_64       <= mcycle_64 + 64'd1           ;     // 每个周期自增
            if (trap_enter) begin
            // 轮询之前优先清理
                case (trap_code)
                    IRQ_M_TIMER : csr_mip <= {csr_mip[31:8],  1'b0, csr_mip[6:0]};     // clear MTIP (bit 7)
                    IRQ_M_EXT   : csr_mip <= {csr_mip[31:12], 1'b0, csr_mip[10:0]};    // clear MEIP (bit 11)
                    default: ;
                endcase

                csr_mepc    <= trap_pc;
                csr_mcause  <= trap_code             ;           // 11=ecall, 3=ebreak
                csr_mstatus <= {csr_mstatus[31:13]   , 
                                2'b11                ,           // MPP ← 3  M特权级
                                csr_mstatus[10:8]    ,
                                csr_mstatus[3]       ,           // MPIE ← MIE
                                csr_mstatus[6:4]     ,
                                1'b0                 ,           // MIE ← 0
                                csr_mstatus[2:0]}    ;

             end else if (mret) begin
                 csr_mstatus <= {csr_mstatus[31:13]   , 
                                 2'b00                ,           // MPP ← 0   S特权级
                                 csr_mstatus[10:8]    ,
                                 1'b1                 ,           // MPIE ← 1
                                 csr_mstatus[6:4]     ,
                                 csr_mstatus[7]       ,           // MIE ← MPIE(bit7→bit3)
                                 csr_mstatus[2:0]}    ;
             end else if (csr_write) begin
                    case (csr_addr)
                        mstatus : csr_mstatus      <= csr_wdata;
                        mtvec   : csr_mtvec        <= csr_wdata;
                        mepc    : csr_mepc         <= csr_wdata;
                        mcause  : csr_mcause       <= csr_wdata;
                        mip     : csr_mip          <= csr_wdata;
                        mie     : csr_mie          <= csr_wdata;
                        mtval   : csr_mtval        <= csr_wdata;
                        mscratch: csr_mscratch     <= csr_wdata;
                        mcyclel : mcycle_64[31:0]  <= csr_wdata;
                        mcycleh : mcycle_64[63:32] <= csr_wdata;
                        default : ;
                    endcase
            end 
        end
    end

endmodule
