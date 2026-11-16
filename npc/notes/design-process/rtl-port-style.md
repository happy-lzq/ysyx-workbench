# NPC RTL 端口与实例连接格式

适用范围：`npc/vsrc/*.v` 的模块接口与实例连接。现有模块及后续新增模块沿用此格式。

## 模块接口

- 使用非 ANSI 写法：模块头仅列端口名，在模块内声明方向、类型与位宽。
- 模块头每行一个端口，第一个端口前无逗号，其余行使用前置逗号。
- 输入在前，输出在后；如有 inout，放在输入与输出之间。
- 每条 input/output 声明只声明一个信号；方向、wire/reg、位宽和名称按列对齐。
- clk、rst、enable、flush 都单独声明，不再合并为一行。
- 保留端口原有名称、类型、位宽；函数参数、内部状态及控制表达式不因接口排版而重写。

```verilog
module example (
    clk
    ,rst
    ,valid_in
    ,valid_out
);
    input  wire clk;
    input  wire rst;
    input  wire valid_in;

    output reg  valid_out;
```

## 实例连接

- 使用命名连接，每行一个端口；顺序与对应模块头一致。
- 输入连接在前，输出连接在后，组间留一空行。
- 端口名、左括号、连接表达式、右括号及方向注释按列对齐。
- 输入注释为 `// << i`，输出为 `// >> o`，inout 为 `// <> io`。
- 方向以被实例化模块的端口声明为准，例如 `req_ready` 在 mem_port 是输入，在 dpi_mem 是输出。

```verilog
    example u_example (
        .clk       (clk       ),    // << i
        .rst       (rst       ),    // << i
        .valid_in  (valid_in  ),    // << i

        .valid_out (valid_out )     // >> o
    );
```

端口重排时核对外部使用的是命名连接还是位置连接；本次 vsrc 与现有 tests 使用命名连接。格式检查不能替代 RTL 功能验证。纯格式修改至少核对接口合同、连接表达式与模块主体，并执行工程 lint/elaboration 或编译；不以旧长程序结果冒充新版本回归。
