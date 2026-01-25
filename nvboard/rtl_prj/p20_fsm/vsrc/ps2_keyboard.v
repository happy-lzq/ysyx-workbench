module ps2_keyboard(
    clk
    ,resetn
    ,ps2_clk     // PS2，有效时钟
    ,ps2_data    // 传入有效数据
    ,scancode    // 有效数据
    ,valid       // 有效数据标志位   
);
    input clk,resetn,ps2_clk,ps2_data;
    output reg [7:0] scancode;
    output reg valid;

    reg [9:0] buffer;        // ps2_data bits
    reg [3:0] count;         // count ps2_data bits
    reg [2:0] ps2_clk_sync;

/* 奈奎斯特采样，打三拍通过处理上上拍与上一拍的变化情况来决定采样时刻，经典的下降沿采样处理
    上升沿 = (上一次是0) 并且 (这一次是1)
          = ~ps2_clk_sync[2] &  ps2_clk_sync[1]
*/
    always @(posedge clk) begin
        ps2_clk_sync <=  {ps2_clk_sync[1:0],ps2_clk};           // 拼接移位操作，低两位左移，拼接ps2_clk构成三位
    end
    wire sampling = ps2_clk_sync[2] & ~ps2_clk_sync[1];         // 下降沿采样信号
    
    always @(posedge clk or negedge resetn) begin
        if (resetn == 0) begin // reset
            count <= 0;
            valid <= 0;
            scancode <= 0;
        end
        else begin
            valid <= 0;
            if (sampling) begin
              if (count == 4'd10) begin
                if ((buffer[0] == 0) &&         // start bit = 0
                    (ps2_data)       &&         // stop  bit = 1
                    (^buffer[9:1]))             // 奇数个1，异或结果为1，偶数个1，异或结果0
                    begin       
                        scancode <= buffer[8:1];
                        valid <= 1;
                end
                count <= 0;                     // for next
              end else begin
                buffer[count] <= ps2_data;      // store ps2_data
                count <= count + 3'b1;
              end
            end
        end
    end

endmodule

/*
    1、奈奎斯特采样定律：只要采样评率远大于信号频率，采用“过采样”捕捉信号处理
    2、PS/2 协议的一帧包含 11 个位：
       [开始位(0)] + [8位数据] + [奇偶校验位] + [停止位(1)]
    3、ps2_data属于串行传输，并行输出。
    4、^buffer[9:1] (奇偶校验)：
        ^ 是 缩减异或（Reduction XOR） 运算符。意思是把这些位全部异或起来。
        范围 buffer[9:1] 包含了：8位数据 ([8:1]) + 1位校验位 ([9])。
        奇校验（Odd Parity）规则：数据位和校验位中，"1" 的总个数必须是奇数。
        异或的特性：
        如果有奇数个 1，异或结果为 1。
        如果有偶数个 1，异或结果为 0。
        结论：如果 ^buffer[9:1] 结果为 1，说明满足奇校验规则，数据传输大概率是正确的。

*/
