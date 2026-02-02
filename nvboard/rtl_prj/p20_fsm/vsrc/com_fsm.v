module com_fsm (
     clk
    ,rst_n
    ,one
    ,half
    ,pi_quit
    ,po_money
    ,po_cola
    ,state_out
    ,quit_out
);
    input      clk, rst_n, one,half,pi_quit;
    output      [2:0]             state_out;
    output  reg            po_cola,po_money;
    output  reg [2:0]              quit_out;

    parameter s0 = 3'b000, s1 = 3'b001, s2 = 3'b010, s3 = 3'b011, s4 = 3'b100;
    reg       [2:0]                      state;
    reg                               quit_clk;
    assign state_out = state;
    wire      [1:0]                   pi_money;
    
    // Auto-reset timer for State 4 (2.0)
    // 50MHz clock * 2s = 100,000,000 ticks. 
    reg [31:0] wait_timer;
    parameter WAIT_TIME = 20000000; // 2 seconds at 50MHz

    assign               pi_money = {one,half};         // 不投币（00） 投half(01) 投one(10)

    //  退钱脉冲  利用寄存器打一拍，将pi_quit电平保持一个时钟周期
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            quit_clk <= 1'b0;
        end else begin
            quit_clk <= pi_quit;     // 寄存器打一拍，即是存上时钟周期的电平，后续利用当前pi_quit与quit_clk判断边沿
        end
    end

//第一段状态机，描述当前状态state如何根据输入跳转到下一状态
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state <= s0;
            wait_timer <= 0;
        end else if ((( pi_quit && !quit_clk ) && (state != s0))) begin
            state <= s0;
            wait_timer <= 0;
        end else begin
                if (state != s4) 
                    wait_timer <= 0;
                else 
                    wait_timer <= wait_timer + 1;

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
                            default: begin
                                // If timeout, auto reset to s0
                                if (wait_timer >= WAIT_TIME) 
                                    state <= s0;
                                else 
                                    state <= s4;
                            end
                         endcase                      
                    default :state <= s0;
                endcase
            end
    end

//第二段状态机，描述当前状态state和输入pi_money如何影响po_cola输出-对应LD0
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            po_cola <=1'b0;
        end else begin
            if(((state == s3) && (pi_money == 2'b10))  || 
               ((state == s4) && (pi_money == 2'b10))  ||
               ((state == s4) && (pi_money == 2'b01))  ||
               // Added: Trigger Cola pulse when reaching 2.0 exactly (1.5 + 0.5 or 1.0 + 1.0)
               ((state == s3) && (pi_money == 2'b01))  ||
               ((state == s2) && (pi_money == 2'b10)) 
              ) begin
                po_cola <= 1'b1;
            end
            else po_cola <= 1'b0;
        end
    end

//第三段状态机，描述当前状态state和输入pi_money如何影响po_money输出-对应-LD2
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            po_money <= 1'b0;
        end else begin
            if (((pi_quit && !quit_clk)  && (state != s0))) begin      // Refund button
                po_money <= 1'b1;
            end else if(((state == s3) && (pi_money == 2'b10))) begin  // 1.5 + 1.0 (Return 0.5)
                po_money <= 1'b1;
            end else 
                po_money <= 1'b0;
        end 
    end

// 被动退币逻辑以及主动退币逻辑 
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            quit_out <= 0;
        end else begin
            if (pi_quit && !quit_clk && state != s0) begin
                case(state)
                    s1: quit_out <= 1; // 0.5
                    s2: quit_out <= 2; // 1.0
                    s3: quit_out <= 3; // 1.5
                    default: quit_out <= 0;
                endcase
            end else if (state == s3 && pi_money == 2'b10) begin 
                // 1.5 + 1.0 = 2.5 -> Change 0.5
                quit_out <= 1;
            end else if (state == s4 && pi_money == 2'b01) begin 
                // 2.0 + 0.5 = 2.5 -> Change 0.5
                quit_out <= 1;
            end else if (state == s4 && pi_money == 2'b10) begin 
                // 2.0 + 1.0 = 3.0 -> Change 1.0
                quit_out <= 2; 
            end else if (state == s2 && pi_money == 2'b01) begin
                // 1.0 + 0.5 = 1.5 < 2.0, No change (Logic check)
            end
            if (state == s0 && pi_money != 0) begin
                quit_out <= 0;
            end
        end
    end
endmodule

/*
通过pi_qiut和quit_clk在同一时钟周期内，上升沿采样时
"在当前周期同步电路内的上升沿来临时属于quit_clk的上升沿"
更精确的说法是：我们检测到的是"在时钟采样时刻，信号从0变为1"
换算参考表 (50MHz下)
期望时间	需要的计数值 (WAIT_TIME)
0.2 秒	10,000,000
0.5 秒	25,000,000
1.0 秒	50,000,000
2.0 秒	100,000,000
10.0 秒	500,000,000

*/
