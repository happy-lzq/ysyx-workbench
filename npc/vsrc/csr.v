module csr (
    clk
    ,rst
    ,csr_op
    ,csr_imm
    ,csr_addr
    ,csr_write
    ,csr_wdata
    ,csr_read
    ,csr_rs1
    ,trap_enter
    ,trap_pc
    ,trap_code
    ,mret
    ,csr_rdata
    ,trap_target
);
    parameter mstatus = 12'h300;
    parameter mtvec   = 12'h305;
    parameter mepc    = 12'h341;
    parameter mcause  = 12'h342;
    reg [31:0] csr_mstatus;   // 0x300
    reg [31:0] csr_mtvec;     // 0x305
    reg [31:0] csr_mepc;      // 0x341
    reg [31:0] csr_mcause;    // 0x342

    input  wire [1 :0] csr_op; 
    input  wire [4 :0] csr_rs1;
    input  wire [11:0] csr_addr;
    input  wire [31:0] trap_pc,trap_code,csr_wdata;
    input  wire [0: 0] clk,rst,csr_imm,csr_write,csr_read,mret,trap_enter;
    output wire [31:0] csr_rdata,trap_target;

    
    
    assign csr_rdata = csr_read ? ((csr_addr == mstatus) ? csr_mstatus : 
                                   (csr_addr == mtvec)   ? csr_mtvec   : 
                                   (csr_addr == mepc)    ? csr_mepc    : 
                                   (csr_addr == mcause)  ? csr_mcause  : 32'h0 ) : 32'h0;

    always @(posedge clk ) begin
        if (rst) begin
            csr_mstatus <= 32'h0000_1800;
            csr_mtvec   <= 32'h8000_0000;
            csr_mepc    <= 32'h0;
            csr_mcause  <= 32'h0;
        end else if (csr_write) begin
            if (csr_addr == mstatus) begin
                csr_mstatus <= csr_wdata;
            end       
             if (csr_addr == mtvec) begin
                csr_mtvec <= csr_wdata;
            end
            if (csr_addr == mepc) begin
                csr_mepc <= csr_wdata;
            end       
             if (csr_addr == mcause) begin
                csr_mcause <= csr_wdata;
            end         
        end
    end
endmodule
