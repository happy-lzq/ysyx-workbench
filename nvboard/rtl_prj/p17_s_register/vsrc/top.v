module top (
     clk                // << i
    ,rst                // << i    
    ,data_in            // << i
    ,data_s             // << i
    ,mode               // << i
    ,data_out           // >> o
);
    input      clk,rst,data_in,mode,data_s;
    output                        data_out;


    wire                    clk,rst,data_s;  // BTNC BTNU
    wire              [2:0]           mode;  // sw[2 :0]
    wire              [7:0]        data_in;  // sw[15:8]
    reg               [7:0]       data_out;
    reg               [31:0]         count;

    localparam LOAD       = 3'b001;  // 置数
    localparam SRL        = 3'b010;  // 逻辑右移
    localparam SLL        = 3'b011;  // 逻辑左移
    localparam SRA        = 3'b100;  // 算术右移
    localparam SERIAL_IN  = 3'b101;  // 左端串行输入
    localparam ROR        = 3'b110;  // 循环右移
    localparam ROL        = 3'b111;  // 循环左移



always @(posedge clk or posedge rst ) begin
    if (rst)
        data_out <= 8'h0;
    else begin
        if(count == 0) begin
        case (mode)
            LOAD      : data_out <= data_in                     ;
            SRL       : data_out <= {1'b0,data_out[7:1]}        ;
            SLL       : data_out <= {data_out[6:0],1'b0}        ;
            SRA       : data_out <= {data_out[7],data_out[7:1]} ;   // 右移 ：最高位保持不变，最低位剔除
            SERIAL_IN : data_out <= {data_s,data_out[7:1]}      ;
            ROR       : data_out <= {data_out[0],data_out[7:1]} ;
            ROL       : data_out <= {data_out[6:0],data_out[7]} ;
            default   : data_out <= data_out                    ; 
        endcase
        end
        count <= (count >= 5000000 ? 32'b0 : count + 1);
    end
end

endmodule

