`include "mem_if_defs.vh"
`include "dpi_imports.vh"
// Simulation backend. Replace this module with SRAM/cache/AXI bridge, keeping
// the req/resp contract. Delays are cycle counts, never host wall-clock sleeps.
module dpi_mem (
    clk
    ,rst
    ,req_valid
    ,resp_ready
    ,req_addr
    ,req_kind
    ,req_size
    ,req_priv
    ,req_mem_attr
    ,req_wdata
    ,req_wmask
    ,resp_valid
    ,req_ready
    ,resp_rdata
    ,resp_err
);
    input  wire                    clk;
    input  wire                    rst;
    input  wire                    req_valid;
    input  wire                    resp_ready;
    input  wire [`MEM_PADDR_W-1:0] req_addr;
    input  wire [1:0]              req_kind;
    input  wire [1:0]              req_size;
    input  wire [1:0]              req_priv;
    input  wire                    req_mem_attr;
    input  wire [31:0]             req_wdata;
    input  wire [3:0]              req_wmask;

    output wire                    resp_valid;
    output wire                    req_ready;
    output reg  [31:0]             resp_rdata;
    output reg                     resp_err;

    localparam [1:0] IDLE=0, EXEC=1, RESP=2;
    reg [1:0] state;
    reg [33:0] saved_addr;
    reg [1:0] saved_kind, saved_size;
    reg saved_attr;
    reg [31:0] saved_wdata;
    reg [3:0] saved_wmask;
    integer req_delay, resp_delay, random_max;
    integer offered, remaining;
    reg [31:0] rng;
    integer seed;
    integer result, error;
    initial begin
        req_delay=0; resp_delay=1; random_max=0; seed=1;
        if ($value$plusargs("mem_req_delay=%d", req_delay)) begin end
        if ($value$plusargs("mem_resp_delay=%d", resp_delay)) begin end
        if ($value$plusargs("mem_random=%d", random_max)) begin end
        if ($value$plusargs("mem_seed=%d", seed)) begin end
        if (req_delay<0 || resp_delay<1 || random_max<0 || random_max>10000)
            $fatal(1,"invalid memory delay configuration");
    end
    assign req_ready = !rst && state == IDLE && offered >= req_delay;
    assign resp_valid = !rst && state == RESP;
    always @(posedge clk) begin
        if (rst) begin
            state       <=IDLE; 
            offered     <=0; 
            remaining   <=0; 
            rng         <=32'(seed);
            saved_addr  <=0; 
            saved_kind  <=0; 
            saved_size  <=0; 
            saved_attr  <=0;
            saved_wdata <=0; 
            saved_wmask <=0; 
            resp_rdata  <=0; 
            resp_err    <=0;
        end else case(state)
            IDLE: if (req_valid) begin
                if (req_ready) begin
                    saved_addr  <=req_addr; 
                    saved_kind  <=req_kind;
                    saved_size  <=req_size; 
                    saved_attr  <=req_mem_attr;
                    saved_wdata <=req_wdata; 
                    saved_wmask <=req_wmask;
                    remaining   <=resp_delay-1 + ((random_max!=0) ? int'(rng % (32'(random_max)+1)) : 0);
                    rng         <=rng*32'd1664525+32'd1013904223;
                    offered     <=0; 
                    state       <=EXEC;
                end else offered<=offered+1;
            end
            EXEC: if (remaining!=0) remaining<=remaining-1;
                  else begin
                    error = dpi_mem_access(
                        {30'b0,saved_addr}, 
                        {30'b0,saved_kind},
                        {30'b0,saved_size}, 
                        {31'b0,saved_attr}, 
                        saved_wdata,
                        {28'b0,saved_wmask}, 
                        result
                        );
                    resp_rdata  <=32'(result); 
                    resp_err    <=(error!=0); 
                    state       <=RESP;
                  end
            RESP: if(resp_ready) state<=IDLE;
            default: state<=IDLE;
        endcase
    end
endmodule
