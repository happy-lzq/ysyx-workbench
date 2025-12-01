module vga_ctrl(
    vga_clk
    ,rst_n
    ,pix_data
    ,pix_x
    ,pix_y
    ,hsync
    ,vsync
    ,rgb
);
    input                      vga_clk,rst_n;    // vga_clk = 25Mhz
    input  wire   [15:0]            pix_data;    // R[15:11]  G[10:5]  B[4:0]
    output wire                  hsync,vsync;
    output wire   [9:0]          pix_x,pix_y;    // 有效显示区域像素点x，y坐标 ：x相对于vga时序计数的坐标映射
    output wire   [15:0]                 rgb;

    // 参数信号
    parameter 
    H_SYNC   = 10'd96      , // 行时序
    H_BACK  = 10'd40       , // 行时序后沿
    H_LEFT   = 10'd8       , // 行时序左边框
    H_VALID  = 10'd640     , // 行有效数据
    H_RIGHT  = 10'd8       , // 行时序右边框
    H_FRONT  = 10'd8       , // 行时序前沿
    H_TOTAL  = 10'd800     ; // 行扫描周期
    parameter 
    V_SYNC   = 10'd2       , // 场同步
    V_BACK   = 10'd25      , // 场时序后沿
    V_TOP    = 10'd8       , // 场时序上边框
    V_VALID  = 10'd480     , // 场有效数据
    V_BOTTOM = 10'd8       , // 场时序下边框
    V_FRONT  = 10'd2       , // 场时序前沿
    V_TOTAL  = 10'd525     ; // 场扫描周期

    // 无效坐标地址
    parameter no_valid_addr = 10'h3ff;

    // 计数器
    reg [9:0] cnt_h,cnt_v  ;    // 行场时钟周期计数器
    wire      rgb_valid    ;
    wire      pix_data_req ;

    // hsync 计数器
    always @(posedge vga_clk or negedge rst_n) begin
        if (!rst_n) begin
            cnt_h <= 10'd0;
        end else if (cnt_h == H_TOTAL - 1'd1)begin
            cnt_h <= 10'd0;
        end else
            cnt_h <= cnt_h +1;
    end
    assign hsync = (cnt_h <= H_SYNC - 1'd1) ? 1'b1 : 1'b0;

    // vsync 计数器
    always @(posedge vga_clk or negedge rst_n) begin
        if (!rst_n) begin
            cnt_v <= 10'd0;
        end else if ((cnt_h == H_TOTAL - 1'b1) && (cnt_v == V_TOTAL - 1'b1)) begin
                cnt_v <= 0;
        end else if (cnt_h == H_TOTAL - 1'b1)
            cnt_v <= cnt_v +1;
    end
    assign vsync = (cnt_v <= V_SYNC - 1'd1) ? 1'b1 : 1'b0;

    // vga 显示有效区域时钟标志位
    assign rgb_valid = (cnt_h >= H_SYNC + H_BACK + H_LEFT)            && 
                       (cnt_h <  H_SYNC + H_BACK + H_LEFT + H_VALID ) &&
                       (cnt_v >= V_SYNC + V_BACK + V_TOP)             && 
                       (cnt_v <  V_SYNC + V_BACK + V_TOP + V_VALID )  ?  1'b1 : 1'b0;

    // vga显示有效区域超前一个时钟周期请求标志
    assign pix_data_req = (cnt_h >= H_SYNC + H_BACK + H_LEFT -1'b1)            && 
                          (cnt_h <  H_SYNC + H_BACK + H_LEFT + H_VALID - 1'b1) &&
                          (cnt_v >= V_SYNC + V_BACK + V_TOP )                  && 
                          (cnt_v <  V_SYNC + V_BACK + V_TOP + V_VALID)         ?  1'b1 : 1'b0;

    // vga有效显示区域像素点坐标
    assign pix_x = (pix_data_req == 1'b1) ? (cnt_h - (H_SYNC + H_BACK + H_LEFT - 1'b1)) : no_valid_addr ;
    assign pix_y = (pix_data_req == 1'b1) ? (cnt_v - (V_SYNC + V_BACK + V_TOP        )) : no_valid_addr ;

    // rgb：vga时序控制有效显示区域时钟内，将外部处理好的有效图像输出，消隐期显示黑色（0黑色，vga标准）
    assign rgb = (rgb_valid == 1'b1 ) ? pix_data : 16'b0;    

endmodule
/*
1. 为什么要设置 VGA 显示有效区域请求信号？
    vga控制模块即将进行有效显示区域处理并传输数据时，需要提前通知数据源（图像处理模块）准备像素数据
    工作流程（一个像素的生命周期）
    T周期（请求阶段）：
        VGA控制器计算出"下一周期需要显示的像素坐标"
        通过pix_x/pix_y发送给图像处理模块
        例如：当扫描到(143,34)时，请求(0,0)位置的数据
    T+1周期（显示阶段）：
        图像处理模块已将(0,0)位置的像素数据准备好放在pix_data上
        VGA控制器进入有效显示区域，将pix_data直接输出到显示器
        同时计算并请求下一个像素(1,0)的坐标
    为什么需要这个设计
        25MHz时钟下，一个周期只有40纳秒
        外部模块需要时间：访问显存→处理数据→稳定输出
        提前请求确保了在需要显示的精确时刻，数据已经准备就绪

2. 为什么请求信号行减少一个周期，场不减少？
    行方向：像素数据是高速连续输出的，每个时钟周期输出一个像素，结合1问数据准备需要时间，，所以提前一个周期发出请求
    场方向：场扫描很慢，属于每一个行扫描周期结束后才增加一场计数，相对于整行的数据准备时间很充足，不需要提前一个周期请求。

3. 输出的有效区域像素点的作用是什么？
    通过vga时序控制得到精准的有效显示区域内的坐标位置，为外部图像处理模块提供精准的像素位置信息
    当不处于有效显示区域时（消隐期），返回无效地址即可

4. 对于pix_x,pix_y而言，当cnt_h=143,cnt_v=34时，(pix_x,pix_y)=(0,0)这个坐标点的意义是啥
    在时钟处于：cnt_h=143,cnt_v=34时刻，系统在请求有效坐标点为(pix_x,pix_y)=(0,0)的数据
    真正显示（0，0）位置像素是在下一时钟周期：cnt_h=144,cnt_v=35时刻，也刚好对应vga有效显示图像区域的时钟周期。完美与vga实现同步匹配显示。
    确保当VGA扫描到有效显示区域开始位置时，第一个像素的数据已经准备好。与第一问呼应

5. 为什么有效区域时钟计数时开始点为 >= 结束点<
    有效区域定义为从起始点 ——> 起始点+有效长度-1的区间。
    (cnt_h >= 144) && (cnt_h < 784) : 144, 145, 146, ..., 783（共640个像素）
    场逻辑一致
*/