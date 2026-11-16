`include "mem_if_defs.vh"

module mem_port (
    clk
    ,rst
    ,if_req
    ,if_addr
    ,mem_req
    ,mem_read
    ,mem_write
    ,mem_addr
    ,mem_wdata
    ,mem_wmask
    ,mem_size
    ,mem_device
    ,if_resp_ready
    ,mem_resp_ready
    ,req_ready
    ,resp_valid
    ,resp_rdata
    ,resp_err
    ,if_resp_valid
    ,mem_resp_valid
    ,if_resp_pc
    ,if_rdata
    ,mem_rdata_raw
    ,if_resp_err
    ,mem_resp_err
    ,if_req_fire
    ,mem_req_fire
    ,req_valid
    ,req_addr
    ,req_kind
    ,req_size
    ,req_priv
    ,req_mem_attr
    ,req_wdata
    ,req_wmask
    ,resp_ready
);
    input  wire                    clk;
    input  wire                    rst;
    input  wire                    if_req;
    input  wire [31:0]             if_addr;
    input  wire                    mem_req;
    input  wire                    mem_read;
    input  wire                    mem_write;
    input  wire [31:0]             mem_addr;
    input  wire [31:0]             mem_wdata;
    input  wire [3:0]              mem_wmask;
    input  wire [1:0]              mem_size;
    input  wire                    mem_device;
    input  wire                    if_resp_ready;
    input  wire                    mem_resp_ready;
    input  wire                    req_ready;
    input  wire                    resp_valid;
    input  wire [31:0]             resp_rdata;
    input  wire                    resp_err;

    output wire                    if_resp_valid;
    output wire                    mem_resp_valid;
    output wire [31:0]             if_resp_pc;
    output wire [31:0]             if_rdata;
    output wire [31:0]             mem_rdata_raw;
    output wire                    if_resp_err;
    output wire                    mem_resp_err;
    output wire                    if_req_fire;
    output wire                    mem_req_fire;
    output wire                    req_valid;
    output reg  [`MEM_PADDR_W-1:0] req_addr;
    output reg  [1:0]              req_kind;
    output reg  [1:0]              req_size;
    output wire [1:0]              req_priv;
    output reg                     req_mem_attr;
    output reg  [31:0]             req_wdata;
    output reg  [3:0]              req_wmask;
    output wire                    resp_ready;

    localparam [1:0] S_IDLE=0, S_OFFER=1, S_WAIT=2;
    reg [1:0] state;
    reg access_if_mem;
    wire select_mem         = mem_req;  // 0-IF  1-MEM
    wire select_fire        = !rst && state == S_IDLE && (mem_req || if_req);
    assign mem_req_fire     = select_fire && select_mem;
    assign if_req_fire      = select_fire && !select_mem;
    assign req_valid        = !rst && state == S_OFFER;
    assign req_priv         = 2'b11; // M-only core; future effective privilege is latched here.
    assign if_resp_valid    = !rst && state == S_WAIT && resp_valid && !access_if_mem;
    assign mem_resp_valid   = !rst && state == S_WAIT && resp_valid && access_if_mem;
    assign resp_ready       = !rst && state == S_WAIT &&
                              (access_if_mem ? mem_resp_ready : if_resp_ready);
    assign if_resp_pc       = req_addr[31:0];
    assign if_rdata         = resp_rdata;
    assign mem_rdata_raw    = resp_rdata;
    assign if_resp_err      = resp_err;
    assign mem_resp_err     = resp_err;
    always @(posedge clk) begin
        if (rst) begin
            state           <= S_IDLE;
            access_if_mem   <= 0;
            req_addr        <= 0; 
            req_kind        <= 0; 
            req_size        <= 0;
            req_mem_attr    <= 0; 
            req_wdata       <= 0; 
            req_wmask       <= 0;
        end else case (state)
            S_IDLE: if (select_fire) begin
                access_if_mem   <= select_mem;
                req_addr        <= {2'b0, select_mem ? mem_addr : if_addr};
                req_kind        <= select_mem ? (mem_write ? `MEM_STORE : `MEM_LOAD) : `MEM_IFETCH;
                req_size        <= select_mem ? mem_size : 2'd2;
                req_mem_attr    <= select_mem && mem_device;
                req_wdata       <= select_mem ? mem_wdata : 32'b0;
                req_wmask       <= select_mem ? mem_wmask : 4'b0;
                state           <= S_OFFER;
            end
            S_OFFER: if (req_valid && req_ready) state <= S_WAIT;
            S_WAIT : if (resp_valid && resp_ready) state <= S_IDLE;
            default: state <= S_IDLE;
        endcase
    end
endmodule
