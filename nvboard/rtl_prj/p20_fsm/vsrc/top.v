module top(
    input clk,
    input rst_n,
    input ps2_clk,
    input ps2_data,
    output [15:0] led,
    output [7:0] seg0,
    output [7:0] seg1,
    output [7:0] seg2,
    output [7:0] seg3
);

    wire [7:0] scancode;
    wire valid;
    
    ps2_keyboard u_ps2(
        .clk(clk),
        .resetn(rst_n),
        .ps2_clk(ps2_clk),
        .ps2_data(ps2_data),
        .scancode(scancode),
        .valid(valid)
    );
/*================= 按键数据传输设计：可乐机使能与复位、投币，与退钱处理 ========================*/
    reg [7:0] released_key;
    reg key_release_pulse;  // 按键有效释放标志位
    reg f0_seen;            // 1,代表F0，存在按键松开，具体哪个键不清楚，需要等通码验证
// 数据发送逻辑：按下发送：通码(传输1次)；释放：发送断码=FO+通码(传输2次)；本设计抓释放时刻即可。
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            f0_seen <= 0;
            key_release_pulse <= 0;
            released_key <= 0;
        end else begin
            key_release_pulse <= 0;
            if (valid) begin
                if (scancode == 8'hF0) begin
                    f0_seen <= 1;
                end else begin
                    if (f0_seen) begin
                        key_release_pulse <= 1;
                        released_key <= scancode;
                        f0_seen <= 0;
                    end
                end
            end
        end
    end

    reg system_enable;      // 可乐机使能标志位:enter/esc
    initial system_enable = 0; 
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            system_enable <= 0;
        end else begin
            if (key_release_pulse) begin
                if (released_key == 8'h5A) // Enter
                    system_enable <= 1;
                else if (released_key == 8'h76) // Esc
                    system_enable <= 0;
            end
        end
    end

    reg one_pulse;   // 投币1
    reg half_pulse;  // 投币0.5
    reg pi_quit;     // 退钱按钮
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            one_pulse <= 0;
            half_pulse <= 0;
            pi_quit <= 0;
        end else begin
            one_pulse <= 0;
            half_pulse <= 0;
            pi_quit <= 0;
            if (system_enable && key_release_pulse) begin
                if (released_key == 8'h3A) // M
                    one_pulse <= 1;
                else if (released_key == 8'h31) // N
                    half_pulse <= 1;
                else if (released_key == 8'h29)
                    pi_quit <=1;
            end
        end
    end

/*===================== 可乐状态机购买输出逻辑设计：根据标志位设计LED显示时长 ========================*/
    wire po_money, po_cola;
    wire [2:0] state_out;
    wire [2:0] quit_out;
    com_fsm u_com_fsm(
        .clk        (clk),
        .rst_n      (rst_n && system_enable), 
        .one        (one_pulse),      // 1
        .half       (half_pulse),     // 0.5
        .pi_quit    (pi_quit),        // 退钱输入  
        .po_money   (po_money),       // 退钱输出标志
        .po_cola    (po_cola),        // 可乐输出标志-LD0  
        .state_out  (state_out),      // 投币状态输出
        .quit_out   (quit_out)        // 退钱状态输出
    );
    
    reg [23:0] cola_timer;    // 可乐出货LED0显示计数
    reg [23:0] refund_timer;  // 退钱LED1显示计算          
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            cola_timer <= 0;
            refund_timer <= 0;
        end else begin
            // Cola Timer
            if (po_cola) 
                cola_timer <= 24'd10000000; 
            else if (cola_timer > 0)
                cola_timer <= cola_timer - 1;
            
            // Refund Timer
            if (po_money)
                refund_timer <= 24'd10000000;
            else if (refund_timer > 0)
                refund_timer <= refund_timer - 1;
        end
    end
    
    // LED[0]: 可乐出货指示 (状态为2.0 或 脉冲延时)
    assign led[0] = (state_out == 3'd4) || (cola_timer > 0);
    assign led[1] = (refund_timer > 0);
    assign led[14:2] = 0;
    assign led[15] = rst_n;


/*===================== 投币数码管根据状态显示逻辑设计：根据输出状态设计数码管状态显示 ========================*/

    reg [3:0] digit_int;
    reg [3:0] digit_dec;
    always @(*) begin
        if (system_enable) begin
            case(state_out)
                3'd0: begin digit_int = 0; digit_dec = 0; end   // 0.0
                3'd1: begin digit_int = 0; digit_dec = 5; end   // 0.5
                3'd2: begin digit_int = 1; digit_dec = 0; end   // 1.0
                3'd3: begin digit_int = 1; digit_dec = 5; end   // 1.5
                3'd4: begin digit_int = 2; digit_dec = 0; end   // 2.0
                default: begin digit_int = 0; digit_dec = 0; end
            endcase
        end else begin
            digit_int = 4'd10; // --
            digit_dec = 4'd10; // --
        end
    end

    wire [6:0] h_int, h_dec;    //  bcd7seg模块输出线
    bcd7seg u_seg_int(
        .in (digit_int), 
        .bcd7seg_out (h_int)
    );
    bcd7seg u_seg_dec(
        .in (digit_dec), 
        .bcd7seg_out (h_dec)
    );

/*===================== 退钱数码管根据状态显示逻辑设计：根据输出状态设计数码管状态显示 ========================*/
    reg [3:0] change_int;
    reg [3:0] change_dec;
    wire [6:0] h_ch_int, h_ch_dec;
    
    always @(*) begin
        if (system_enable) begin
            case(quit_out)
                3'd0: begin change_int = 0; change_dec = 0; end
                3'd1: begin change_int = 0; change_dec = 5; end // 0.5
                3'd2: begin change_int = 1; change_dec = 0; end // 1.0
                3'd3: begin change_int = 1; change_dec = 5; end // 1.5
                3'd4: begin change_int = 2; change_dec = 0; end // 2.0
                default: begin change_int = 0; change_dec = 0; end
            endcase

        end else begin
            change_int = 4'd10; // --
            change_dec = 4'd10; // --
        end
    end

    bcd7seg u_seg_ch_int(
        .in (change_int), 
        .bcd7seg_out (h_ch_int)
    );

    bcd7seg u_seg_ch_dec(
        .in (change_dec), 
        .bcd7seg_out (h_ch_dec)
    );
/*===================== 数码管根据显示逻辑设计连接开发板输出端口 ========================*/
    wire dp1 = system_enable; 
    wire dp0 = 0;
    assign seg1 = ~{h_int, dp1}; 
    assign seg0 = ~{h_dec, dp0}; 
    assign seg3 = ~{h_ch_int, dp1}; 
    assign seg2 = ~{h_ch_dec, dp0};

endmodule
