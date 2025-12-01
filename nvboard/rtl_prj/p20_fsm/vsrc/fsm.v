module fsm (
     clk
    ,rst_n
    ,din
    ,dout
);
    input           clk,rst_n,din;
    output   reg             dout;

    parameter s0 = 4'b0000, s1 = 4'b0001, s2 = 4'b0010, s3 = 4'b0011;
    parameter s4 = 4'b0100, s5 = 4'b0101, s6 = 4'b0110, s7 = 4'b0111, s8 = 4'b1000;
    reg     [3:0]           state;

    //第一段状态机，描述当前状态state如何根据输入跳转到下一状态
    always @(posedge clk  or negedge rst_n) begin
        if (!rst_n) begin
            state <= s0;
        end else begin
            case (state)
                s0 : state <= (din == 1'b0) ? s1 : s5;
                s1 : state <= (din == 1'b0) ? s2 : s5;
                s2 : state <= (din == 1'b0) ? s3 : s5;
                s3 : state <= (din == 1'b0) ? s4 : s5;
                s4 : state <= (din == 1'b0) ? s4 : s5;
                s5 : state <= (din == 1'b1) ? s6 : s1;
                s6 : state <= (din == 1'b1) ? s7 : s1;
                s7 : state <= (din == 1'b1) ? s8 : s1;              
                s8 : state <= (din == 1'b1) ? s8 : s1;
                default : state <= s0;
            endcase
        end
    end

    //第二段状态机，描述当前状态state和输入pi_money如何影响po_cola输出
    
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            dout <= 1'b0;
        end else 
            dout <= ((state == s4) || (state == s8));       // 逻辑或，两者之一任意成立结果都为1  并且dout延迟一拍输出
    end
endmodule

/*
状态机核心：
    1：状态转移图确定每一种状态如何产生
    2：输出结果0/1状态在何时产生？ 在什么状态下产生输出1状态，其余都为0状态
*/
