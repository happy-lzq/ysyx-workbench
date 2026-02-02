# FPGA 设计范式：低速异步串行接口接收逻辑

**日期**: 2025-01
**主题**: 串行转并行 (SIPO) 与 跨时钟域处理 (CDC)
**案例**: PS/2 键盘驱动

---

## 1. 核心设计思想概述

在 FPGA 设计中，处理外部输入的低速串行信号（如键盘、串口、传感器数据）时，有一套标准化的处理流程。其核心目标是将 **异步、串行** 的外部物理信号，安全地转换为 **同步、并行** 的内部逻辑数据。

**通用设计公式：**
> **打拍同步** $\rightarrow$ **边沿检测** $\rightarrow$ **采样移位** $\rightarrow$ **计数控制** $\rightarrow$ **数据校验** $\rightarrow$ **握手输出**

---

## 2. 详细设计步骤分解

### 步骤一：物理层与约束 (Physical Layer)
信号源于物理设备（如键盘），通过 PCB 走线进入 FPGA 引脚。
*   **输入**：通常包含 `Clock` (随路时钟) 和 `Data` (数据线)。
*   **关键**：必须在 `.xdc` 或 `.cst` 文件中将代码端口绑定到正确的物理引脚。

### 步骤二：跨时钟域处理 (CDC & Synchronization)
外部信号相对于 FPGA 系统主时钟（如 50MHz）是**异步**的。直接使用会导致**亚稳态 (Metastability)**，可能导致逻辑错误甚至电路故障。

*   **处理方法**：**“打两拍” (2-Stage Flip-Flop Synchronizer)**
*   **代码范式**：
    ```verilog
    reg [2:0] sync_r;
    always @(posedge sys_clk) begin
        // 将外部信号 rx_in 移入寄存器，利用 D触发器特性消除亚稳态
        sync_r <= {sync_r[1:0], rx_in}; 
    end
    ```

### 步骤三：边沿检测 (Edge Detection)
FPGA 内部逻辑应统一由系统主时钟驱动，不建议在模块内部使用 `always @(negedge external_clk)`（多时钟域是 FPGA 设计大忌）。我们需要把外部信号的“边沿”转换为内部的一个“脉冲信号”。

*   **原理**：比较“当前拍”和“前一拍”的状态。
*   **下降沿检测**：前一拍是 1，当前拍是 0。
    ```verilog
    // sync_r[2] 是老状态(1)，sync_r[1] 是新状态(0)
    wire sampling_pulse = sync_r[2] & ~sync_r[1]; 
    ```
*   **意义**：`sampling_pulse` 只维持一个系统时钟周期，作为后续逻辑的**使能信号 (Enable)**。

### 步骤四：串转并移位 (Serial In Parallel Out - SIPO)
利用移位寄存器或索引寻址，将一位一位到来的数据拼凑成完整的字节。

*   **操作**：当检测到采样脉冲时，将数据锁存。
    ```verilog
    if (sampling_pulse) begin
        buffer[count] <= data_in; // 或 buffer <= {buffer[6:0], data_in};
    end
    ```

### 步骤五：计数与帧结构控制 (Counter & FSM)
通过计数器判断当前传输到了协议帧的哪一部分（开始位？数据位？校验位？停止位？）。

*   **PS/2 帧结构**：
    `[Start(0)] + [Data(8bit)] + [Parity(odd)] + [Stop(1)]` = 11 bits
*   **逻辑**：
    *   `count == 0`: 接收开始位
    *   `count == 1~8`: 接收数据
    *   ...

### 步骤六：数据完整性校验 (Validation)
**这是保证系统健壮性的关键。** 外部线路可能存在干扰，或者发生插拔，未经验证的数据是不可信的。

*   **校验点**：
    1.  **帧头帧尾**：Start 是否为 0？Stop 是否为 1？
    2.  **内容校验**：奇偶校验 (Parity Check) 或 CRC。
        *   *Verilog 技巧*：`^buffer[9:1]` (缩减异或运算) 可快速计算奇偶性。

### 步骤七：握手输出 (Handshake Output)
当数据校验通过后，通知下游模块。

*   **机制**：**数据 (`data`) + 有效标志 (`valid`)**
*   **操作**：
    *   平时 `valid` 为 0。
    *   仅在校验成功的那个时钟周期，将 `valid` 拉高，同时更新 `data`。
    *   下游模块逻辑：`if (valid) process(data);`

---

## 3. 代码逻辑摘要 (Template)

```verilog
always @(posedge clk) begin
    // 1. 同步打拍
    sync <= {sync[1:0], rx_in}; 
end

// 2. 边沿检测
wire sample_en = sync[2] & ~sync[1]; 

always @(posedge clk) begin
    if (reset) begin ... end
    else begin
        valid_out <= 0; // 默认无效（脉冲生成）
        
        if (sample_en) begin
            // 3. 接收数据 & 4. 计数
            buffer[count] <= rx_data;
            count <= count + 1;
            
            // 5. 帧结束判断
            if (count == FRAME_LEN) begin
                count <= 0;
                // 6. 校验与输出
                if (check_start_stop && check_parity) begin
                    data_out  <= buffer[8:1];
                    valid_out <= 1; // 拉高 Valid
                end
            end
        end
    end
end
```

## 4. 适用条件与限制 (Constraint)

此设计范式基于 **过采样 (Oversampling)** 原理。

*   **奈奎斯特采样定理**：为了准确捕捉外部信号的变化，FPGA 的系统时钟频率必须 **远大于** 外部信号频率。
*   **工程经验公式**：
    > `F_sys > 10 * F_ext`
    > *(即：系统时钟频率至少应该是外部信号频率的 10 倍以上)*

### ✅ 适用场景 (低速接口)
*   **PS/2** (10 - 16 kHz)
*   **UART** (9600 - 115200 bps)
*   **I2C** (100 - 400 kHz)
*   **SPI** (作为从机，且频率较低时)

### ❌ 不适用场景 (高速接口)
对于高速接口，简单的 GPIO 采样逻辑无法满足时序要求，必须使用 FPGA 内部专用的硬件资源：
*   **DDR 内存接口** (几百 MHz 到 GHz) $\rightarrow$ 需用专用 **PHY / Memory Controller**。
*   **高速 MIPI / LVDS / PCIe** $\rightarrow$ 需用专用 **SerDes** 或 **硬件原语 (Primitives)**。
*   **千兆以太网 (RGMII/SGMII)** $\rightarrow$ 需用 **IDDR/ODDR** 或 **GTP/GTX Transceivers**。