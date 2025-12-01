module register #(
    parameter WIDTH = 8
) (
    clk         // << I <<
    ,rst        // << I <<
    ,set        // << I <<
    ,en         // << I <<
    ,d          // << I <<
    ,q          // >> O >>
);
    input        clk,rst,set,en,d;
    output                       q;
    wire           [WIDTH-1:0]   d;
    reg            [WIDTH-1:0]   q;

    // 异步复位，使能控制的寄存器
    
    always @(posedge clk or posedge rst or posedge set) begin
        if (rst) begin
            q <= {WIDTH{1'b0}};
        end else if (set) begin
            q <= {WIDTH{1'b1}};
        end else if (en) begin
            q <= d;
        end
    end
endmodule 
