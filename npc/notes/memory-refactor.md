# NPC 内存架构重构：哈佛 → 冯诺依曼 (DPI-C 统一内存)

> 日期：2026-06-02  
> 目标：从 Verilog reg 大数组的哈佛架构，迁移到 DPI-C + C++ 数组的冯诺依曼统一内存

---

## 一、架构对比

### 改造前：哈佛架构 + Verilog 内存 + 外部信号 MMIO

```
┌────────────────────────────────────────────────────────────┐
│                      Verilog RTL                           │
│                                                            │
│  ┌──────────┐     ┌──────────────────┐                     │
│  │ if_stage │────→│ imem[33M]        │ 128MB Verilog reg   │
│  │          │     │ (指令专用)         │ 数组，Verilator 慢   │
│  └──────────┘     └──────────────────┘                     │
│                                                            │
│  ┌──────────┐     ┌──────────────────┐                     │
│  │mem_stage │────→│ dmem[33M]        │ 又一个 128MB reg    │
│  │          │     │ (数据专用)         │ 数组                │
│  │          │     └──────────────────┘                     │
│  │          │     ┌──────────────────┐                     │
│  │          │────→│ dev_req/dev_addr │ 设备访问信号         │
│  │          │     │ dev_wdata/wmask  │ → C++ 处理           │
│  └──────────┘     └──────────────────┘                     │
│                                                            │
│  问题:                                                     │
│  ① IMEM/DMEM 重复加载同一份 .bin                            │
│  ② .bss 段处理困难（需要从 ELF 分段加载）                     │
│  ③ MMIO 需要单独的 dev_* 信号通道                           │
│  ④ Verilator 大数组仿真慢                                   │
│  ⑤ 不利于后续接总线                                         │
└────────────────────────────────────────────────────────────┘
```

### 改造后：冯诺依曼 + DPI-C 统一内存

```
┌──────────────────────┐        ┌────────────────────────────┐
│     Verilog RTL      │  DPI-C │      C++ Testbench         │
│     (只有 CPU 核)     │═══════→│                            │
│                      │        │  npc_pmem[128MB]           │
│  ┌──────────┐        │  read  │  ┌──────────────────────┐  │
│  │ if_stage │────────┼───────→│  │ 物理内存              │  │
│  │          │ instr  │        │  │ [0x80000000-0x88000000)│ │
│  └──────────┘        │        │  │                      │  │
│                      │        │  ├──────────────────────┤  │
│  ┌──────────┐        │  read  │  │ MMIO 路由             │  │
│  │mem_stage │────────┼───────→│  │ 0x10000000 → UART    │  │
│  │          │        │  write │  │ 0xa0000048 → RTC     │  │
│  └──────────┘        │═══════→│  └──────────────────────┘  │
│                      │        │                            │
│  Verilog 中:          │        │  加载: fread 一条语句       │
│  零个大数组！          │        │  读写: 地址范围内判断       │
│  零个 dev_* 端口！     │        │  调试: printf + gdb        │
└──────────────────────┘        └────────────────────────────┘

优势:
① 统一内存，指令和数据走同一入口
② .bin 直接 fread，天然包含所有段
③ MMIO 在同一个读写函数内 if/else 分支处理
④ C++ 原生数组，速度快
⑤ 后续接总线模型天然适配
```

---

## 二、核心设计决策

### 2.1 DPI-C 通信模型

| 方向 | 函数 | Verilog 调用位置 | C++ 实现 |
|------|------|-----------------|----------|
| RTL→C++ 读 | `dpi_mem_read(addr)` | 组合逻辑 `assign` | `memory.cpp` |
| RTL→C++ 写 | `dpi_mem_write(addr, data, wmask)` | 时序逻辑 `always @(posedge)` | `memory.cpp` |

### 2.2 为什么读用 combinational、写用 sequential？

- **读**：`if_stage` 取指和 `mem_stage` 加载都是组合逻辑链的一部分，必须在同一 `eval()` 内返回数据给 WB 阶段。DPI-C 函数在 `eval()` 求解过程中被 Verilator 调用，数据即时可用。
- **写**：写入是 `posedge clk` 触发，这是有副作用的操作（修改 C++ 数组、printf 输出字符），放在时序逻辑中时机明确、不会在 delta 循环中重复调用。

### 2.3 文件组织

```
npc/
├── vsrc/
│   ├── dpi_imports.vh          ← DPI-C 声明（Verilog 侧）
│   ├── if_stage.v              ← 删除了 imem 数组
│   ├── mem_stage.v             ← 删除了 dmem 数组 + dev_* 端口
│   └── core_top.v              ← 删除了 dev_* 端口
├── include/
│   ├── memory.h                ← [NEW] 内存 API + 常量
│   ├── dpi_export.h            ← DPI-C 声明（C++ 侧）
│   ├── device.h                ← 仅保留 npc_serial_putc
│   └── npc.h                   ← 删除 imem/dmem 访问器
└── csrc/
    ├── memory/
    │   └── memory.cpp           ← [NEW] 统一内存 + MMIO 路由
    ├── device/
    │   ├── serial.cpp           ← 不变
    │   ├── dpi_device.cpp       ← 清理（定时器预留）
    │   └── mmio.cpp             ← 清理（路由已移入 memory.cpp）
    ├── main.cpp                 ← 删除 npc_pmem、MMIO 拦截
    ├── monitor/
    │   ├── monitor.cpp          ← load_bin 简化
    │   └── sdb/npc_isa.cpp      ← pmem_read/write
    ├── difftest/dut.cpp         ← img_size → npc_img_size
    └── monitor/trace/itrace.cpp ← 加 memory.h
```

---

## 三、逐文件代码对比

### 3.1 `vsrc/dpi_imports.vh` — DPI-C 声明集中管理

**改造前：**
```verilog
// dpi_imports.vh — NPC 所有 DPI-C 导入集中管理

// ========== MMIO 设备读 ==========
import "DPI-C" function int dpi_mmio_read(input int addr);

// ========== 预留：定时器中断 ==========
// import "DPI-C" function int dpi_timer_irq();

// ========== 预留：UART 输出（可选，也可用 top-level 信号）==========
// import "DPI-C" function void dpi_serial_putc(input int ch);
```

> 问题：`dpi_mmio_read` 只管 MMIO 设备读，物理内存读写还在 Verilog 的 imem/dmem 数组中。

**改造后：**
```verilog
// dpi_imports.vh — NPC 所有 DPI-C 导入集中管理

// ========== 统一内存读写 ==========
import "DPI-C" function int  dpi_mem_read(input int addr);
import "DPI-C" function void dpi_mem_write(input int addr, input int data, input int wmask);

// ========== 预留：定时器中断 ==========
// import "DPI-C" function int dpi_timer_irq();
```

> 变化：`dpi_mmio_read` → `dpi_mem_read` + `dpi_mem_write`。两个函数覆盖全部地址空间（物理内存 + MMIO），不需要区分。

---

### 3.2 `vsrc/if_stage.v` — 取指：从 Verilog IMEM → DPI-C

**改造前（哈佛架构，IMEM 在 Verilog 内）：**
```verilog
module if_stage (
    clk ,rst ,trap_enter ,trap_target ,mret
    ,pc_sel ,br_taken ,jump_jalr ,imm_jal ,imm_br
    ,pc ,pc_plus4 ,instr
);
    input   wire                  clk,rst,br_taken,trap_enter,mret;
    input   wire [1:0]                                      pc_sel;
    input   wire [31:0]       jump_jalr,imm_jal,imm_br,trap_target;
    output  reg  [31:0]                                         pc;
    output  wire [31:0]                             pc_plus4,instr;

    parameter RESET_VECTOR = 32'h8000_0000;
    parameter IMEM_SIZE    = 33554432     ;  // 128MB / 4B = 33,554,432 words

    wire [31:0] pc_next;
    reg  [31:0] imem [0:IMEM_SIZE - 1];      // ← 128MB 的 Verilog reg 数组

    assign pc_next = (trap_enter | mret ) ? trap_target           :
                     ((pc_sel == 2'b00 ) ? pc + 32'd4             :
                      (pc_sel == 2'b01 ) ? pc + imm_jal            :
                      (pc_sel == 2'b10 ) ? {jump_jalr[31:1],1'b0}  :
                      (pc_sel == 2'b11 ) ? (br_taken ? pc + imm_br : pc + 32'd4)
                                          : pc + 32'd4 );

    assign pc_plus4 = pc + 32'd4;
    wire [31:0] pc_off = pc - RESET_VECTOR;     // ← 手动地址译码
    wire [24:0] imem_idx = pc_off[26:2];

    assign instr = imem[imem_idx];              // ← 从 Verilog 数组读

    always @(posedge clk ) begin
        if (rst) 
            pc <= RESET_VECTOR;
        else
            pc <= pc_next;
    end
endmodule
```

> 问题：`imem[33M]` 是 Verilog 的 `reg` 数组，Verilator 将其转换为 C++ 大数组，但仍在 Verilator 的符号表内管理。加载时需要逐字写入。

**改造后（冯诺依曼，DPI-C 取指）：**
```verilog
`include "dpi_imports.vh"

module if_stage (
    clk ,rst ,trap_enter ,trap_target ,mret
    ,pc_sel ,br_taken ,jump_jalr ,imm_jal ,imm_br
    ,pc ,pc_plus4 ,instr
);
    input   wire                  clk,rst,br_taken,trap_enter,mret;
    input   wire [1:0]                                      pc_sel;
    input   wire [31:0]       jump_jalr,imm_jal,imm_br,trap_target;
    output  reg  [31:0]                                         pc;
    output  wire [31:0]                             pc_plus4,instr;

    parameter RESET_VECTOR = 32'h8000_0000;
    // 删除 IMEM_SIZE, imem 数组, imem_idx 译码

    wire [31:0] pc_next;

    // PC 选择逻辑（不变）
    assign pc_next = (trap_enter | mret ) ? trap_target           :
                     ((pc_sel == 2'b00 ) ? pc + 32'd4             :
                      (pc_sel == 2'b01 ) ? pc + imm_jal            :
                      (pc_sel == 2'b10 ) ? {jump_jalr[31:1],1'b0}  :
                      (pc_sel == 2'b11 ) ? (br_taken ? pc + imm_br : pc + 32'd4)
                                          : pc + 32'd4 );

    assign pc_plus4 = pc + 32'd4;

    // 取指：通过 DPI-C 从 C++ 统一内存读取
    // dpi_mem_read 内部判断 addr 范围，自动路由到物理内存或 MMIO
    assign instr = dpi_mem_read(pc);

    always @(posedge clk ) begin
        if (rst) 
            pc <= RESET_VECTOR;
        else
            pc <= pc_next;
    end
endmodule
```

> 变化：
> ① 删除 `parameter IMEM_SIZE`、`reg [31:0] imem`、`wire pc_off`、`wire imem_idx`
> ② `assign instr = imem[imem_idx]` → `assign instr = dpi_mem_read(pc)`
> ③ 添加 `` `include "dpi_imports.vh"``
> ④ PC 选择逻辑不变

---

### 3.3 `vsrc/mem_stage.v` — 访存：从 DMEM + dev_* → DPI-C

**改造前：**
```verilog
`include "dpi_imports.vh"

module mem_stage (
    clk ,lsu_type ,mem_read ,mem_write ,mem_addr
    ,mem_wdata_raw ,mem_rdata
    ,dev_req ,dev_r_w ,dev_addr ,dev_wdata ,dev_wmask   // ← 6 个设备端口
);
    input   wire [0 :0] clk,mem_read,mem_write;
    input   wire [2 :0] lsu_type;
    input   wire [31:0] mem_addr;        
    input   wire [31:0] mem_wdata_raw; 
    output  wire [31:0] mem_rdata ;

    // ======================= 外设分发信号 =================
    output wire [0 :0] dev_req ;        // 设备访问请求
    output wire [0 :0] dev_r_w ;        // 读/写方向
    output wire [31:0] dev_addr;        // 设备地址
    output wire [31:0] dev_wdata;       // 写入数据
    output wire [3 :0] dev_wmask;       // 字节使能

    parameter DATA_ADDR_START = 32'h80000000;
    parameter DATA_ADDR_END   = 32'h88000000;
    parameter PMEM_BASE = 32'h8000_0000;
    parameter MMEM_SIZE = 33554432;              // 128MB / 4B
    reg  [31:0] dmem [0:MMEM_SIZE-1];           // ← 128MB Verilog reg 数组
    wire [3 :0] mem_wmask ; 
    wire [31:0] mem_rdata_raw, mem_wdata, dev_rdata;
    wire [31:0] mem_off = mem_addr - PMEM_BASE; // ← 手动地址译码
    wire [24:0] mem_idx = mem_off[26:2];

    // 地址范围判断：在物理内存内？
    wire in_pmem = (mem_addr >= DATA_ADDR_START) && (mem_addr < DATA_ADDR_END);

    // 设备信号赋值（组合逻辑）
    assign dev_req   = (mem_read | mem_write) && !in_pmem;
    assign dev_r_w   = mem_write;
    assign dev_addr  = mem_addr;
    assign dev_wdata = mem_wdata_raw;
    assign dev_wmask = mem_wmask;

    // Load：物理内存走 dmem，设备走 DPI-C
    assign dev_rdata = dpi_mmio_read(mem_addr);         // ← 只读 MMIO
    assign mem_rdata_raw = in_pmem ? dmem[mem_idx] : dev_rdata;

    // Store：只在物理内存范围内写 DMEM
    always @(posedge clk ) begin
        if (mem_write && in_pmem) begin
            if(mem_wmask[0]) dmem[mem_idx][7 :0 ] <= mem_wdata[7 :0 ];
            if(mem_wmask[1]) dmem[mem_idx][15:8 ] <= mem_wdata[15:8 ];
            if(mem_wmask[2]) dmem[mem_idx][23:16] <= mem_wdata[23:16];
            if(mem_wmask[3]) dmem[mem_idx][31:24] <= mem_wdata[31:24];
        end 
    end

    lsu lsu_pic( ... );
endmodule
```

> 问题：
> ① `dmem[33M]` 第二个 128MB Verilog 大数组
> ② `in_pmem` 范围判断逻辑 + 译码逻辑全在 Verilog 中
> ③ 6 个设备信号端口，需要 `core_top` 层层透传
> ④ 物理内存和设备使用两条不同的数据路径（`dmem[mem_idx]` vs `dpi_mmio_read`）
> ⑤ MMIO 写还需 C++ 侧再拦截一次 `dev_req/dev_addr/dev_wdata`

**改造后：**
```verilog
`include "dpi_imports.vh"

module mem_stage (
    clk ,lsu_type ,mem_read ,mem_write
    ,mem_addr ,mem_wdata_raw ,mem_rdata
    // 所有设备端口删除！
);
    input   wire [0 :0] clk,mem_read,mem_write;
    input   wire [2 :0] lsu_type;
    input   wire [31:0] mem_addr;        
    input   wire [31:0] mem_wdata_raw; 
    output  wire [31:0] mem_rdata ;

    wire [3 :0] mem_wmask; 
    wire [31:0] mem_rdata_raw, mem_wdata;

    // 读：组合逻辑内通过 DPI-C 从 C++ 统一内存读取
    // dpi_mem_read 内部自动判断物理内存/MMIO
    assign mem_rdata_raw = dpi_mem_read(mem_addr);

    // 写：时序逻辑内通过 DPI-C 写入 C++ 统一内存
    // dpi_mem_write 内部自动判断物理内存/MMIO
    always @(posedge clk) begin
        if (mem_write) begin
            dpi_mem_write(mem_addr, mem_wdata, mem_wmask);
        end
    end

    lsu lsu_pic( ... );    // LSU 不变，仍负责字节拼接和 wmask 生成
endmodule
```

> 变化：
> ① 删除 `dmem` 数组、`DATA_ADDR_START/END`、`PMEM_BASE`、`MMEM_SIZE`、`mem_off`、`mem_idx`、`in_pmem`
> ② 删除 6 个 `dev_*` 输出端口
> ③ 删除 `dev_rdata`、`dpi_mmio_read` 调用
> ④ 读路径：`in_pmem ? dmem[idx] : dev_rdata` → `dpi_mem_read(mem_addr)` （一行）
> ⑤ 写路径：`if (mem_write && in_pmem) dmem[idx] <= ...` → `if (mem_write) dpi_mem_write(...)` （一行）
> ⑥ LSU 子模块完全保留不变

---

### 3.4 `vsrc/core_top.v` — 删除 dev_* 端口

**改造前：**
```verilog
module core_top (
    clk ,rst ,instr ,halt ,halt_pc ,halt_ret
    ,dev_req ,dev_r_w ,dev_addr ,dev_wdata ,dev_wmask  // ← 设备端口
);
    input  wire [0 :0]                            clk,rst;
    output wire [0 :0]                               halt;
    output wire [31:0]             instr,halt_pc,halt_ret;
    output wire [0 :0] dev_req, dev_r_w;              // ← 声明输出
    output wire [31:0] dev_addr, dev_wdata;
    output wire [3 :0] dev_wmask;
    // ... 内部 wires ...

mem_stage u_mem_stage (
    .clk              (clk),
    .lsu_type         (lsu_type),
    // ... 原有端口 ...
    .dev_req          (dev_req),     // ← 需逐线连接
    .dev_r_w          (dev_r_w),
    .dev_addr         (dev_addr),
    .dev_wdata        (dev_wdata),
    .dev_wmask        (dev_wmask)
);
```

**改造后：**
```verilog
module core_top (
    clk ,rst ,instr ,halt ,halt_pc ,halt_ret
    // 全部设备端口删除
);
    input  wire [0 :0]                            clk,rst;
    output wire [0 :0]                               halt;
    output wire [31:0]             instr,halt_pc,halt_ret;
    // dev_* wires 和 output 声明全部删除
    // ... 内部 wires 不变 ...

mem_stage u_mem_stage (
    .clk              (clk),
    .lsu_type         (lsu_type),
    .mem_read         (mem_read),
    .mem_write        (mem_write),
    .mem_addr         (mem_addr),
    .mem_wdata_raw    (mem_wdata_raw),
    .mem_rdata        (mem_rdata)
    // 5 个设备端口连接全部删除
);
```

---

### 3.5 `include/memory.h` — [NEW] 内存模块头文件

```c
#ifndef __NPC_MEMORY_H__
#define __NPC_MEMORY_H__

#include <stdint.h>

#define PMEM_BASE 0x80000000UL
#define PMEM_SIZE (128UL * 1024 * 1024)   // 128MB
#define PMEM_END  (PMEM_BASE + PMEM_SIZE)

#ifdef __cplusplus
extern "C" {
#endif

// ========== DPI-C 接口（Verilog 侧调用）==========
int  dpi_mem_read(int addr);
void dpi_mem_write(int addr, int data, int wmask);

#ifdef __cplusplus
}

// ========== C++ 侧辅助函数和全局变量 ==========
extern uint8_t npc_pmem[PMEM_SIZE];    // 128MB 统一物理内存
extern long npc_img_size;              // 加载镜像的字节数

void pmem_init();                      // 内存清零
void pmem_load_bin(const char *path);  // 从 .bin 文件加载
uint32_t pmem_read(uint32_t addr, int len);   // SDB 调试读
void     pmem_write(uint32_t addr, int len, uint32_t data); // SDB 调试写

#endif

#endif
```

> 关键设计：
> - `dpi_mem_read/write` 用 `extern "C"` 声明（DPI-C 必须是 C 链接）
> - 物理常量（PMEM_BASE/SIZE/END）集中定义，取代原来散落在各处 Kconfig/Verilog 的定义
> - `npc_pmem[]` 暴露给其他模块（itrace/difftest 需要）
> - `pmem_read/write` 是 SDB 调试器的辅助接口

---

### 3.6 `csrc/memory/memory.cpp` — [NEW] 统一内存实现（核心）

```cpp
#include <cstdio>
#include <cstring>
#include <cassert>
#include <memory.h>
#include <device.h>

// ==================== 128MB 统一物理内存 ====================
uint8_t npc_pmem[PMEM_SIZE];     // 全局数组，itrace/difftest 可访问
long npc_img_size = 0;           // 加载的镜像大小

// ==================== C++ 侧辅助 API ====================

void pmem_init() {
    memset(npc_pmem, 0, PMEM_SIZE);         // 全清零（含 .bss 段）
}

void pmem_load_bin(const char *path) {
    assert(path);
    FILE *fp = fopen(path, "rb");
    assert(fp);

    fseek(fp, 0, SEEK_END);
    npc_img_size = ftell(fp);              // 获取文件大小
    assert(npc_img_size <= PMEM_SIZE);

    fseek(fp, 0, SEEK_SET);
    size_t n = fread(npc_pmem, 1, npc_img_size, fp);  // ★ 一条语句加载
    assert(n == (size_t)npc_img_size);
    fclose(fp);

    printf("Loaded %ld bytes from %s into pmem [0x%08x, 0x%08lx)\n",
           npc_img_size, path, PMEM_BASE, PMEM_BASE + npc_img_size);
}
```

> 对比改造前的 `load_bin`（在 monitor.cpp 中）：
> ```cpp
> // 改造前：逐 4 字节循环，分别写 IMEM、DMEM、C++ 数组（三次写）
> __uint8_t buf[4];
> while (fread(buf,1,4,fp) == 4){
>     __uint32_t word = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
>     npc_imem(top,idx,word,WRITE);     // 写 Verilog IMEM
>     npc_dmem(top,idx,word,WRITE);     // 写 Verilog DMEM
>     memcpy(&npc_pmem[idx * 4], buf, 4); // 写 C++ 数组
>     idx++;
> }
> ```
> 改造后：`fread(npc_pmem, 1, npc_img_size, fp)` 一句搞定。不再需要逐字写入 Verilog 数组。

```cpp
// ==================== SDB 调试器接口 ====================

uint32_t pmem_read(uint32_t addr, int len) {
    uint32_t offset = addr - PMEM_BASE;
    if (offset >= PMEM_SIZE) return 0;          // 越界保护

    uint32_t val = 0;
    for (int i = 0; i < len; i++)
        val |= (uint32_t)npc_pmem[offset + i] << (i * 8);
    return val;
}

void pmem_write(uint32_t addr, int len, uint32_t data) {
    uint32_t offset = addr - PMEM_BASE;
    if (offset >= PMEM_SIZE) return;

    for (int i = 0; i < len; i++)
        npc_pmem[offset + i] = (data >> (i * 8)) & 0xFF;
}
```

> 对比改造前（sdb/npc_isa.cpp）：
> ```cpp
> // 改造前：直接访问 Verilog DMEM 数组
> word_t vaddr_read(vaddr_t addr, int len) {
>     uint32_t idx = (addr - RESET_VECTOR) >> 2;
>     return npc_dmem(top, idx, 0, READ);   // ← 读 Verilog 数组
> }
> ```
> 改造后：通过 C++ 原生数组访问，不再依赖 Verilator。

```cpp
// ==================== DPI-C：统一内存读写（Verilog 调用）====================

extern "C" {

int dpi_mem_read(int addr) {
    // ──── 物理内存 ────
    if (addr >= PMEM_BASE && addr < PMEM_END) {
        uint32_t offset = addr - PMEM_BASE;
        return *(uint32_t *)(npc_pmem + offset);
    }

    // ──── MMIO 设备读 ────
    switch (addr) {
        case 0x10000000: return 0;           // UART 只写设备
        // case 0xa0000048: return rtc_lo(); // RTC（后续扩展）
        default: return 0;
    }
}
```

> 核心逻辑：`if (addr 在物理内存范围)` → 直接数组访问；`else` → switch 查 MMIO 表。  
> 一个函数同时覆盖物理内存和所有外设。

```cpp
void dpi_mem_write(int addr, int data, int wmask) {
    // ──── 物理内存写入 ────
    if (addr >= PMEM_BASE && addr < PMEM_END) {
        uint32_t offset = addr - PMEM_BASE;
        for (int i = 0; i < 4; i++) {
            if (wmask & (1 << i))                   // 字节使能
                npc_pmem[offset + i] = (data >> (i * 8)) & 0xFF;
        }
        return;
    }

    // ──── MMIO 设备写 ────
    switch (addr) {
        case 0x10000000:  // UART 串口
            if (wmask & 0x1)
                npc_serial_putc(data & 0xFF);       // → 终端输出
            break;
        // case 0xa0000048: ... // RTC（后续扩展）
        default: break;
    }
}

}  // extern "C"
```

> **wmask（字节使能）的处理**：
> - `sb` 指令写入某个字节，wmask 中只有对应位为 1（如 `4'b0001`）
> - `sh` 指令写入 2 字节，wmask 为 `4'b0011` 或 `4'b1100`
> - `sw` 指令写入 4 字节，wmask 为 `4'b1111`
> - LSU 模块已经根据 `lsu_type` + `mem_addr[1:0]` 算好了 wmask，这里直接使用

---

### 3.7 `csrc/main.cpp` — 删除 MMIO 拦截和硬件内存引用

**改造前：**
```cpp
#include <difftest.h>
#include <sdb.h>
#include <trace.h>
#include <interrupt.h>
#include <device.h>                     // ← 需要 device.h
Vcore_top *top = NULL;
VerilatedVcdC* tfp = NULL;
NPC_state npc_s, ref_s;

int idx =0;
int cycle = 0;
uint8_t npc_pmem[PMEM_SIZE];           // ← main.cpp 拥有 npc_pmem

void single_cycle(){
    uint32_t this_pc = npc_pc(top, 0, READ);
    uint32_t this_inst = npc_imem(top, (this_pc - RESET_VECTOR) >> 2, 0, READ); // ← RTL IMEM
    top->clk = 1; top->eval();
    halt_check();

    // MMIO 设备写拦截 ← 需手动读取 dev_* 信号
    if (top->dev_req && top->dev_r_w) {
        mmio_write_handler(top->dev_addr, top->dev_wdata, top->dev_wmask);
    }

    #ifdef CONFIG_DIFFTEST
    // ...
```

**改造后：**
```cpp
#include <difftest.h>
#include <sdb.h>
#include <trace.h>
#include <interrupt.h>
// 删除 <device.h> — MMIO 路由已移入 dpi_mem_write

Vcore_top *top = NULL;
VerilatedVcdC* tfp = NULL;
NPC_state npc_s, ref_s;

int idx =0;
int cycle = 0;
// 删除 npc_pmem[PMEM_SIZE] — 已移入 memory.cpp

void single_cycle(){
    uint32_t this_pc = npc_pc(top, 0, READ);
    uint32_t this_inst = top->instr;      // ← 直接读 top 端口（DPI-C 组合输出）
    top->clk = 1; top->eval();
    halt_check();
    // 删除整个 MMIO 拦截块

    #ifdef CONFIG_DIFFTEST
    // ...
```

> 变化：
> ① 删除 `#include <device.h>`
> ② 删除 `uint8_t npc_pmem[PMEM_SIZE]`
> ③ `npc_imem(...)` → `top->instr`（直接读端口）
> ④ 删除 `if (top->dev_req && top->dev_r_w) { mmio_write_handler(...) }` —— 不再需要

---

### 3.8 `csrc/monitor/monitor.cpp` — 加载简化

**改造前：**
```cpp
void load_bin(Vcore_top* top, const char* path) {
    Assert(path, "IMG-BIN-FILE IS FATL!\n");
    FILE* fp = fopen(path, "rb");
    if (!fp) { fprintf(stderr, "Cannot open %s\n", path); exit(1); }

    __uint8_t buf[4];
    while (fread(buf, 1, 4, fp) == 4) {            // 逐 4 字节循环
        Assert((idx + 1) * 4 <= PMEM_SIZE, "image is too large for pmem\n");
        __uint32_t word = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
        npc_imem(top, idx, word, WRITE);             // ① 写 RTL IMEM
        npc_dmem(top, idx, word, WRITE);             // ② 写 RTL DMEM
        memcpy(&npc_pmem[idx * 4], buf, 4);          // ③ 写 C++ 数组
        idx++;
    }
    img_size = idx * 4;
    fclose(fp);
    printf("\nLoaded %d pc_addr to ref_mem and npc_mem form %s\n", idx, path);
}

void monitor_init(int argc, char* argv[]){
    parse_agrs(argc, argv);
    load_bin(top, img_file);       // ← 旧函数
    npc_init();
    // ...
```

**改造后：**
```cpp
#include <sdb.h>
#include <difftest.h>
#include <trace.h>
#include <memory.h>              // ← 新增

void monitor_init(int argc, char* argv[]){
    parse_agrs(argc, argv);
    pmem_init();                  // 清零 128MB 内存
    pmem_load_bin(img_file);      // ★ fread 一句加载
    npc_init();
    // ...
```

> 变化：
> ① `load_bin()` 函数完全删除（逻辑移入 memory.cpp）
> ② `pmem_init()` 清零整个 `npc_pmem[]` 数组（确保 `.bss` 为零）
> ③ `pmem_load_bin()` 用 `fread` 一次读入，不再需要逐字写

---

### 3.9 `include/npc.h` — 删除硬件内存访问器

**改造前：**
```verilog
// ==================== 基础常量 ====================
#define RESET_VECTOR CONFIG_RESET_VECTOR
#define PMEM_SIZE    CONFIG_PMEM_SIZE          // ← 从 Kconfig
#define PMEM_WORDS   (PMEM_SIZE / 4)
#define MAX_CYCLE 1000000

// ==================== 全局变量 ====================
extern uint8_t npc_pmem[PMEM_SIZE];            // ← main.cpp 定义

// ==================== Verilator RTL 访问器 ====================
static inline uint32_t npc_imem(Vcore_top *top, int idx, uint32_t val, int r_w) {
    // 直接读写 Verilog 的 imem 数组
    if (r_w == WRITE)
        return top->rootp->core_top__DOT__u_if_stage__DOT__imem[idx] = val;
    else
        return top->rootp->core_top__DOT__u_if_stage__DOT__imem[idx];
}

static inline uint32_t npc_dmem(Vcore_top *top, int idx, uint32_t val, int r_w) {
    // 直接读写 Verilog 的 dmem 数组
    if (r_w == WRITE)
        return top->rootp->core_top__DOT__u_mem_stage__DOT__dmem[idx] = val;
    else
        return top->rootp->core_top__DOT__u_mem_stage__DOT__dmem[idx];
}
```

**改造后：**
```cpp
#include <generated/autoconf.h>
#include <memory.h>                              // ← 统一内存 API

// ==================== 基础常量 ====================
#define RESET_VECTOR CONFIG_RESET_VECTOR         // 仅保留 Reset Vector
// 删除 PMEM_SIZE, PMEM_WORDS — 由 memory.h 提供
#define MAX_CYCLE 1000000

// 删除 extern uint8_t npc_pmem[PMEM_SIZE] — 由 memory.h 提供

// 删除 npc_imem() 函数 — Verilog 中已无 imem 数组
// 删除 npc_dmem() 函数 — Verilog 中已无 dmem 数组

// npc_csr() 不变 — CSR 仍在 Verilog 中
// npc_gpr(), npc_pc(), npc_npc(), npc_inst() 不变
```

---

### 3.10 其他文件清理

**`include/device.h`** — 删除 `mmio_write_handler` 声明：
```cpp
// 改造后只保留：
#ifndef __NPC_DEVICE_H__
#define __NPC_DEVICE_H__
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
void npc_serial_putc(unsigned char ch);
#ifdef __cplusplus
}
#endif
#endif
```

**`csrc/device/mmio.cpp`** — 改为说明注释：
```cpp
// mmio.cpp — MMIO 设备路由已整合至 csrc/memory/memory.cpp
// 见 dpi_mem_read / dpi_mem_write 函数
```

**`csrc/device/dpi_device.cpp`** — 清除旧实现，保留定时器预留：
```cpp
// dpi_device.cpp — 设备侧 DPI-C 实现（定时器等）
// 内存相关的 DPI-C 函数已移至 csrc/memory/memory.cpp
#include <dpi_export.h>
extern "C" {
// ========== 预留：定时器中断 ==========
// int dpi_timer_irq() { ... }
}
```

**`csrc/monitor/sdb/npc_isa.cpp`** — 改用 `pmem_read/write`：
```cpp
// 改造前：
word_t vaddr_read(vaddr_t addr, int len) {
    uint32_t idx = (addr - RESET_VECTOR) >> 2;
    return npc_dmem(top, idx, 0, READ);         // ← RTL DMEM
}
// 改造后：
word_t vaddr_read(vaddr_t addr, int len) {
    return pmem_read(addr, len);                // ← C++ 数组
}
```

**`csrc/difftest/dut.cpp`** — 使用 `npc_img_size`：
```cpp
// 改造前：自己维护一个 static long img_size
long img_size = 0;
void difftest_init() {
    init_difftest(diff_so_file, img_size);
}
// 改造后：
void difftest_init() {
    init_difftest(diff_so_file, npc_img_size);  // ← 来自 memory.cpp
}
```

**`csrc/monitor/trace/itrace.cpp`** — 添加 `memory.h` 引入：
```cpp
#include <trace.h>
#include <difftest.h>
#include <memory.h>    // ← 新增，guest_to_host 需要 npc_pmem
```

---

## 四、MMIO 新增设备只需加 case

后续添加 RTC、Timer 等外设时，只需要在 `memory.cpp` 的两个函数中各加一个 `case`：

```cpp
// dpi_mem_read 中加：
case 0xa0000048: return rtc_read_lo();
case 0xa000004c: return rtc_read_hi();

// dpi_mem_write 中加：
case 0xa0000048: rtc_write_lo(data, wmask); break;
case 0x20000000: timer_config(data, wmask); break;
```

不需要改任何 Verilog 代码，不需要新增任何端口。

---

## 五、改造效果对比

| 维度 | 改造前 | 改造后 |
|------|-------|--------|
| Verilog 大数组 | 2 个 (imem + dmem, 各 128MB) | 0 个 |
| `core_top` 端口数 | 6 基础 + 5 设备 = 11 | 6 基础 |
| 加载代码行数 | ~15 行（逐字循环） | 2 行（fread） |
| MMIO 新增设备 | 改 RTL + 改 C++ | 只改 C++ 的 switch |
| 内存调试 | 波形中翻找 | printf + gdb |
| 总线接口准备 | 从零开始 | 天然适配 |
| 编译依赖 | 依赖 Verilator 内部符号 | 只依赖 memory.h |

---

## 六、迁移过程中发现并修复的 Bug

### Bug #1：C/C++ 类型提升导致的符号扩展陷阱

**影响**：所有地址 `>= 0x80000000` 的判断全部失败，指令取指返回全零。  
**表现**：`cpu-tests` 全部测试失败（`gpr[2] mismatch at pc=0x80000008`，sp 不更新）。  
**根因**：DPI-C 接口传 `int`（32-bit signed），但 `PMEM_BASE` 定义为 `unsigned long`（64-bit）。比较时 `int` 先符号扩展到 64-bit 再比较。

#### 错误的代码（修复前）：

```cpp
// memory.h
#define PMEM_BASE 0x80000000UL          // unsigned long (64-bit)
#define PMEM_END  (PMEM_BASE + PMEM_SIZE)

// memory.cpp
int dpi_mem_read(int addr) {            // addr = int (32-bit signed)
    if (addr >= PMEM_BASE && addr < PMEM_END) {  // ← 这里出 bug！
        uint32_t offset = addr - PMEM_BASE;
        return *(uint32_t *)(npc_pmem + offset);
    }
    // 走到 MMIO default → return 0
}
```

#### Bug 的数值追踪：

```
addr 从 Verilog 传来:  0x80000004 (32-bit 位模式)
DPI-C → C int:         -2147483644  (signed 32-bit)
与 PMEM_BASE 比较时:
  int → unsigned long (64-bit): 0xFFFFFFFF80000004  ← 符号扩展！
  PMEM_BASE = 0x0000000080000000 (64-bit unsigned)
  比较: 0xFFFFFFFF80000004 >= 0x0000000080000000 → true ✅
  但是: 0xFFFFFFFF80000004 <  0x0000000088000000 → FALSE ❌
       ↑ 这个值远大于 PMEM_END！

结论: 0x80000004 被判断为"不在物理内存范围内" → 走入 MMIO default → return 0
      → CPU 拿到 0x00000000 指令 (effectively NOP) → 不写寄存器
```

#### 正确的代码（修复后）：

```cpp
// memory.h — 将常量改为 32-bit unsigned，避免类型提升到 64-bit
#define PMEM_BASE 0x80000000U           // unsigned int (32-bit)
#define PMEM_SIZE (128U * 1024 * 1024)
#define PMEM_END  (PMEM_BASE + PMEM_SIZE)

// memory.cpp — 显式将 addr 转为 uint32_t，禁止符号扩展
int dpi_mem_read(int addr) {
    uint32_t paddr = (uint32_t)addr;     // ← 关键！去掉符号位

    if (paddr >= PMEM_BASE && paddr < PMEM_END) {
        uint32_t word_addr = paddr & ~3U;
        uint32_t offset = word_addr - PMEM_BASE;
        return *(uint32_t *)(npc_pmem + offset);
    }
    // MMIO ...
}

void dpi_mem_write(int addr, int data, int wmask) {
    uint32_t paddr = (uint32_t)addr;     // ← 同样处理
    uint32_t wdata = (uint32_t)data;
    // ...
}
```

**教训**：DPI-C 的 Verilog `int` 对应 C `int`（signed 32-bit）。处理硬件地址（unsigned 语义）时，C++ 侧**必须**先 `(uint32_t)` 转换再参与运算和比较，否则会被 C 的整型提升规则"暗算"。

---

### Bug #2：`dpi_mem_read` 字节地址 vs 字对齐不匹配

**影响**：所有涉及内存读写（`lw/lh/lb/sw/sh/sb`）的测试失败。  
**表现**：`cpu-tests` 中 `load-store`、`unalign`、`string`、`crc32`、`hello-str`、`to-lower-case` 共 6 项 FAIL。  
**根因**：旧 DMEM 是 word-indexed（`dmem[(addr>>2)]`），新 `dpi_mem_read` 从精确字节地址读取 `*(uint32_t*)`，LSU 拿到错误的字。

#### 错误的代码（修复前）：

```cpp
int dpi_mem_read(int addr) {
    uint32_t paddr = (uint32_t)addr;
    if (paddr >= PMEM_BASE && paddr < PMEM_END) {
        uint32_t offset = paddr - PMEM_BASE;   // ← 精确字节偏移！
        return *(uint32_t *)(npc_pmem + offset); // ← 从未对齐地址读 4 字节！
    }
}
```

#### Bug 的数值追踪（以 `lb 0x80000001` 为例）：

```
旧 DMEM (word-indexed):
  mem_idx  = (0x80000001 - 0x80000000) >> 2 = 0
  dmem[0]  → 读取 0x80000000 处的整个字 (bytes 0-3)
  LSU 从字中提取 byte 1 ✅

新 dpi_mem_read (byte-indexed):
  offset   = 0x80000001 - 0x80000000 = 1
  *(uint32_t*)(npc_pmem + 1) → 读取 npc_pmem[1..4]
  ↑ 这是 bytes 1-4，不是 bytes 0-3！
  LSU 从错误的字中提取 byte → 得到错误数据 ❌

同样地:
  lh 0x80000002:
    旧: dmem[0] → LSU 提取 bytes 2-3 ✅
    新: npc_pmem[2..5] → bytes 2-5 ❌ (应该读 bytes 0-3 的 word)
```

#### 架构对比图示：

```
旧 DMEM 寻址（Word-Indexed）:
  地址 0x80000000 ─→ dmem[0] (一个 32-bit word，包含 bytes 0,1,2,3)
  地址 0x80000001 ─→ dmem[0] (同上，LSU 从 word 中提取 byte 1)
  地址 0x80000004 ─→ dmem[1] (包含 bytes 4,5,6,7)

新 dpi_mem_read 修复前（Byte-Indexed）:
  地址 0x80000000 ─→ *(uint32_t*)(npc_pmem + 0) = word at bytes 0-3 ✅
  地址 0x80000001 ─→ *(uint32_t*)(npc_pmem + 1) = word at bytes 1-4 ❌
  地址 0x80000004 ─→ *(uint32_t*)(npc_pmem + 4) = word at bytes 4-7 ✅
```

#### 正确的代码（修复后）：

```cpp
int dpi_mem_read(int addr) {
    uint32_t paddr = (uint32_t)addr;

    if (paddr >= PMEM_BASE && paddr < PMEM_END) {
        uint32_t word_addr = paddr & ~3U;         // ← 字对齐！
        uint32_t offset = word_addr - PMEM_BASE;
        return *(uint32_t *)(npc_pmem + offset);   // 总是从对齐地址读
    }
    // MMIO 设备读 — 使用精确地址（设备寄存器对齐）
    switch (paddr) { ... }
}
```

**为什么 `dpi_mem_write` 不需要同样修改？**  
因为写路径使用 `wmask`（字节使能），它天然编码了"写哪个字节"：

```cpp
// dpi_mem_write — 正确（无需修改）
void dpi_mem_write(int addr, int data, int wmask) {
    uint32_t paddr = (uint32_t)addr;
    // ...
    for (int i = 0; i < 4; i++) {
        if (wmask & (1 << i))                          // wmask 精确指示每字节
            npc_pmem[offset + i] = (wdata >> (i*8)) & 0xFF;
    }
}
```

旧 DMEM 写路径：`dmem[mem_idx][7:0] <= mem_wdata[7:0]`（`mem_idx` 字对齐，`[7:0]` 选中字节 0）  
新 C++ 写路径：`npc_pmem[mem_idx*4 + 0] = ...` — 物理上更新同一个字节 ✅

#### `dpi_mem_read` 的双重角色与对齐策略：

| 调用方 | 地址来源 | 是否需要字对齐？ | 当前行为 |
|--------|---------|:---:|------|
| `if_stage.v`：`instr = dpi_mem_read(pc)` | PC 寄存器（总是 4 字节对齐） | N/A | ✅ 自然对齐 |
| `mem_stage.v`：`mem_rdata_raw = dpi_mem_read(mem_addr)` | ALU 输出（任意字节地址） | **是** | 修复后 ✅ |
| MMIO 读路由 | 设备地址（如 0x10000000） | **否**（精确匹配） | ✅ 不走字对齐分支 |

**教训**：从 Verilog 的 word-indexed 大数组迁移到 C++ 的 byte-indexed 数组时，读路径需要手动做字对齐以匹配 LSU 的行为预期。写路径因为有 `wmask`，字节偏移隐含在 mask 中，天然正确。

---

### Bug #3：Verilator 编译 include 路径未配置

**影响**：`dpi_imports.vh` 找不到。  
**表现**：`%Error: Cannot find include file: 'dpi_imports.vh'`  
**根因**：Verilator 命令行缺少 `-I$(NPC_HOME)/vsrc`，Verilator 的预处理器找不到 include 文件。  
**注意**：`-CFLAGS` 只传给 C++ 编译器，不影响 Verilog include 搜索路径。

**修复**：在 Makefile 的 `verilator` 命令行添加 `-I$(NPC_HOME)/vsrc`：

```makefile
# Makefile
sim: $(VSRCS) $(CSRCS)
	verilator --cc --exe --build --trace \
	--top-module $(TOP) $(VSRCS) $(CSRCS) \
	-I$(NPC_HOME)/vsrc \                         # ← 新增
	-CFLAGS "$(SIM_CFLAGS)" -LDFLAGS "$(LDFLAGS)"
```

---

### Bug #4：`#ifdef` 块被意外嵌套导致编译错误

**影响**：`main.cpp` 编译失败。  
**表现**：`error: unterminated #ifdef`  
**根因**：替换 `load_bin` 相关代码时，注释掉的旧 `printf` 调试代码中包含一个未配对的 `#ifdef CONFIG_DIFFTEST`。

**修复前**（错误）：
```cpp
    halt_check();

    #ifdef CONFIG_DIFFTEST                              // ← 未配对！注释块内残留
    //    cycle,
    //    npc_pc(top, 0, READ),
    // isa_reg_display(); 

    #ifdef CONFIG_DIFFTEST                              // ← 第二个（正确配对）
    if (diff_so_file) {
        difftest_step(top, cycle);
    }
    #endif
```

**修复后**（正确）：
```cpp
    halt_check();

    #ifdef CONFIG_DIFFTEST
    if (diff_so_file) {
        difftest_step(top, cycle);
    }
    #endif
```

**教训**：删除大段包含预处理指令的代码时，要仔细检查 `#ifdef`/`#ifndef`/`#endif` 的配对数。

---

### Bug 修复总结

| Bug # | 类别 | 根因 | 影响范围 | 修复文件 |
|-------|------|------|---------|---------|
| #1 | C/C++ 类型系统 | `int`→`unsigned long` 符号扩展 | 全部指令（取指返回 0） | `memory.h`、`memory.cpp` |
| #2 | 内存寻址模型 | 字节地址 vs 字对齐不匹配 | `lb/lh/lw/sh/sb/sw` 全部 | `memory.cpp`（`dpi_mem_read`） |
| #3 | 构建系统 | Verilator include 路径缺失 | 编译失败 | `Makefile` |
| #4 | 预处理 | `#ifdef` 配对错误 | 编译失败 | `main.cpp` |

**测试验证**：全部 35 项 `cpu-tests` 通过。  
**测试命令**：`make ARCH=riscv32I-npc run ALL=*`
