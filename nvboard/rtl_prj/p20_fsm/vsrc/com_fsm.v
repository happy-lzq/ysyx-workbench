module com_fsm (
     clk
    ,rst_n
    ,one
    ,half
    ,pi_quit
    ,po_money
    ,po_cola
    ,quit_clk
);
    input      clk, rst_n, one,half,pi_quit;
    output  reg   po_cola,po_money,quit_clk;

    parameter s0 = 3'b000, s1 = 3'b001, s2 = 3'b010, s3 = 3'b011, s4 = 3'b100;
    reg       [2:0]                      state;
    wire      [1:0]                   pi_money;
    assign               pi_money = {one,half};         // 不投币（00） 投half(01) 投one(10)

    //  退钱脉冲  利用寄存器打一拍，将pi_quit电平保持一个时钟周期
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            quit_clk <= 1'b0;
        end else begin
            quit_clk <= pi_quit;                // 打一拍  
        end
    end

//第一段状态机，描述当前状态state如何根据输入跳转到下一状态
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state <= s0;
        end else if ((( pi_quit && !quit_clk ) && (state != s0))) begin
            state <= s0;
        end else begin
                case (state)
                    s0 : case (pi_money)                    // 0
                            2'b01  :state <= s1;
                            2'b10  :state <= s2;
                            default:state <= s0;
                         endcase
                    s1 : case (pi_money)                    // 0.5
                            2'b01  :state <= s2;
                            2'b10  :state <= s3;
                            default:state <= s1;
                         endcase     
                    s2 : case (pi_money)                    // 1
                            2'b01  :state <= s3;
                            2'b10  :state <= s4;
                            default:state <= s2;
                         endcase
                    s3 : case (pi_money)                    // 1.5
                            2'b01  :state <= s4;
                            2'b10  :state <= s0;
                            default:state <= s3;
                         endcase                      
                    s4 : case (pi_money)                    // 2
                            2'b01  :state <= s0;
                            2'b10  :state <= s0;
                            default:state <= s4;
                         endcase                      
                    default :state <= s0;
                endcase
            end
    end

//第二段状态机，描述当前状态state和输入pi_money如何影响po_cola输出
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            po_cola <=1'b0;
        end else begin
            if(((state == s3) && (pi_money == 2'b10))  || 
               ((state == s4) && (pi_money == 2'b10))  ||
               ((state == s4) && (pi_money == 2'b01)))       begin
                po_cola <= 1'b1;
            end
            else po_cola <= 1'b0;
        end
    end

//第三段状态机，描述当前状态state和输入pi_money如何影响po_money输出
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            po_money <= 1'b0;
        end else begin
            if (((pi_quit && !quit_clk)  && (state != s0))) begin
                po_money <= 1'b1;
            end else if((state == s4) && (pi_money == 2'b10)) begin
                po_money <= 1'b1;
            end else 
                po_money <= 1'b0;
        end 
    end
endmodule

/*
通过pi_qiut和quit_clk在同一时钟周期内，上升沿采样时
"在当前周期同步电路内的上升沿来临时属于quit_clk的上升沿"
更精确的说法是：我们检测到的是"在时钟采样时刻，信号从0变为1"

*/
