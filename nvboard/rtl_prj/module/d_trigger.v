// d_trigger.v 
// D触发器模块汇总
// 包含各类D触发器的实现
// 组合逻辑中的if-else 必须完整，否则容易产生锁存器；时序逻辑可以不完成出现
// ================================== 基本D触发器 ===================================
module d_ff_basic (
    clk     
    ,d      
    ,q      
);

    input      clk, d;
    output          q;

    wire          clk;
    wire    [0:0]   d;
    reg     [0:0]   q;

    always @(posedge clk ) begin
    q <= d;        
    end
    
endmodule

//================================= 同步复位D触发器 ==================================
module d_ff_sync (
     clk
    ,rstn
    ,d
    ,q
);
    input clk,rstn,d;
    output         q;

    wire         clk;
    wire  [0:0] rstn;
    wire  [0:0]    d;
    reg   [0:0]    q;

always @(posedge clk ) begin
    if (rstn) begin
        q <= 1'b0;
    end else begin
        q <= d;
    end
end
endmodule

//================================= 异步复位D触发器 ==================================
module d_ff_async (
     clk
    ,rstn
    ,d
    ,q
);
    input clk,rstn,d;
    output         q;

    wire         clk;
    wire  [0:0] rstn;
    wire  [0:0]    d;
    reg   [0:0]    q;

always @(posedge clk or posedge rstn) begin
    if (rstn) begin
        q <= 1'b0;
    end else begin
        q <= d;
    end
end
endmodule

//================================= 带使能端的异步复位D触发器 ==================================
module d_ff_sync_en (
     clk
    ,rstn
    ,en
    ,d
    ,q
);
    input clk,rstn,d,en;
    output            q;

    wire            clk;
    wire  [0:0]    rstn;
    wire  [0:0]      en;
    wire  [0:0]       d;
    reg   [0:0]       q;

always @(posedge clk or posedge rstn) begin
    if (rstn) begin
        q <= 1'b0;
    end else if (en) begin
        q <= d;
    end
end
endmodule

//================================= 异步置位D触发器 ==================================
module d_ff_async_set (
     clk
    ,set
    ,en
    ,d
    ,q
);
    input clk,set,d,en;
    output            q;

    wire            clk;
    wire  [0:0]    set;
    wire  [0:0]      en;
    wire  [0:0]       d;
    reg   [0:0]       q;

always @(posedge clk or posedge set) begin
    if (set) begin
        q <= 1'b0;    // 同步置位 数值
    end else if (en) begin
        q <= d;
    end 
end
endmodule

//================================= 异步使能、置位、复位D触发器 ==================================
module d_ff_async_en_rstn_set (
     clk
    ,rstn
    ,set
    ,en
    ,d
    ,q
);
    input clk,rstn,set,d,en;
    output                q;

    wire                clk;
    wire      [0:0]    rstn;
    wire      [0:0]     set;
    wire      [0:0]      en;
    wire      [0:0]       d;
    reg       [0:0]       q;

always @(posedge clk or posedge rstn or posedge set) begin
    // 复位 > 置位 
    if (rstn) begin
        q <= 1'b0;    // 同步置位 数值
    end else if (set) begin
        q <= 1'b1;
    end else if (en) begin
        q <= d;
    end 
end
endmodule
