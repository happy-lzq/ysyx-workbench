# NPC 设计笔记

本文基于当前 `npc/csrc` 和 `npc/vsrc` 源码整理，目标是建立一套可以用于阅读、调试和扩展 NPC 的整体设计模型。

当前 NPC 是一个用 Verilog 描述的 RV32 单周期 CPU 核，外层用 Verilator 生成 C++ 仿真程序。RTL 只保留 CPU 数据通路和控制逻辑；内存、MMIO 设备、difftest、SDB 调试器、itrace、波形等基础设施都在 C++ 侧实现。

## 1. 总体架构

### 1.1 一句话理解

NPC 的运行方式可以概括为：

```text
RISC-V binary
  -> C++ monitor 加载到 npc_pmem[128MB]
  -> Verilated core_top 每个周期执行一条 RTL 指令
  -> RTL 通过 DPI-C 访问 C++ 统一内存和 MMIO
  -> C++ 每周期做 difftest / watchpoint / itrace / halt 检查
```

### 1.2 代码分层

```text
npc/
  Makefile                     Verilator 构建入口
  include/
    npc.h                      全局状态、CSR 常量、Verilator 内部信号访问器
    memory.h                   PMEM 常量、DPI 内存接口
    mmio.h                     MMIO 设备注册表接口
    difftest.h                 NEMU difftest API 声明
    interrupt.h                中断查询和注入接口
    sdb.h                      SDB/表达式/监视点接口
    trace.h                    itrace/disasm 接口
    generated/autoconf.h       Kconfig 生成配置
  csrc/
    main.cpp                   仿真主循环和单周期执行
    monitor/monitor.cpp        参数解析、初始化、复位、状态检查
    memory/memory.cpp          C++ 统一物理内存和 DPI-C 实现
    device/mmio.cpp            MMIO 地址映射和设备注册
    device/serial.cpp          串口输出
    device/rtc.cpp             RTC 读和 host timer interrupt 源
    monitor/interrupt.cpp      中断 pending 查询、interrupt_valid 注入
    difftest/dut.cpp           difftest 初始化、状态采样和比较
    monitor/sdb/*              简易调试器、表达式、监视点
    monitor/trace/*            itrace 和 Capstone 反汇编
  vsrc/
    core_top.v                 CPU 顶层连线
    if_stage.v                 PC 更新和取指
    id_stage.v                 指令字段拆解、控制、立即数
    control.v                  指令译码和控制信号生成
    ex_stage.v                 ALU、分支判断、CSR 写数据生成
    mem_stage.v                DPI load/store 时序封装
    lsu.v                      load 扩展、store 数据和 wmask
    wb_stage.v                 写回数据选择
    regfile.v                  32 个 GPR
    csr.v                      M-mode CSR、trap、mret、mcycle
    halt.v                     ebreak 转仿真结束
    dpi_imports.vh             Verilog 侧 DPI-C 声明
```

### 1.3 当前配置重点

来自 `include/generated/autoconf.h`：

```text
ISA                 riscv32
RESET_VECTOR        0x80000000
PMEM_SIZE           0x8000000, 即 128MB
DEVICE              enabled
SERIAL_MMIO         0xa00003f8
RTC_MMIO            0xa0000048
DIFFTEST            enabled
ITRACE              enabled
WATCHPOINT          enabled
WAVE                enabled
BATCH               enabled
```

注意：RTL 中 `if_stage.v` 和 `halt.v` 的 `RESET_VECTOR` 参数硬编码为 `32'h8000_0000`，当前与 Kconfig 一致。如果以后修改 Kconfig 的 reset vector，需要同步处理 RTL 参数或通过 Verilator define 传入。

## 2. 构建和运行入口

### 2.1 Verilator 构建

`Makefile` 中：

```makefile
VSRCS=$(shell find vsrc/ -name "*.v")
CSRCS=$(shell find csrc/ -name "*.cpp")
TOP=core_top
```

`make sim` 会执行：

```text
verilator --cc --exe --build --trace \
  --top-module core_top \
  vsrc/*.v csrc/*.cpp \
  -I vsrc -I include \
  -CFLAGS "... include paths ..." \
  -LDFLAGS "-ldl -lreadline"
```

然后把 `obj_dir/Vcore_top` 移到 `build/sim`。

### 2.2 运行参数

`monitor.cpp` 支持：

```text
-i, --bin   binary image 路径，必需
-e, --elf   ELF 路径，主要给 trace/ftrace 预留
-d, --diff  NEMU difftest 动态库路径
-w, --wave  当前只解析，不直接改变 CONFIG_WAVE
-h, --help
```

典型运行：

```bash
./build/sim -i path/to/test.bin -d path/to/riscv32-nemu-interpreter-so
```

## 3. C++ 仿真框架

### 3.1 全局状态

`include/npc.h` 定义了仿真状态：

```c
enum { NPC_STOP, NPC_RUNNING, NPC_END, NPC_ABORT, NPC_QUIT };

typedef struct {
  int state;
  vaddr_t halt_pc;
  uint32_t halt_ret;
} NPCSIM_State;

typedef struct {
  uint32_t gpr[32];
  uint32_t pc;
  uint32_t csr[8];
} NPC_state;
```

`NPC_state` 是 difftest 使用的 DUT/REF 快照，包含 32 个 GPR、PC 和 8 个机器态 CSR。CSR 顺序在 `difftest/dut.cpp::npc_state_data()` 中固定：

```text
0 mstatus
1 mip
2 mie
3 mcause
4 mtvec
5 mtval
6 mepc
7 mscratch
```

### 3.2 初始化流程

入口是 `main.cpp::main()`：

```text
main()
  -> Verilated::commandArgs()
  -> top = new Vcore_top
  -> 如果 CONFIG_WAVE，打开 build/wave.vcd
  -> monitor_init(argc, argv)
  -> npc_exec()
  -> 关闭波形并释放 top
  -> 根据 NPC_END 且 halt_ret==0 返回 0，否则返回 1
```

`monitor_init()` 的顺序：

```text
parse_args()
pmem_init()
init_mmio()              如果 CONFIG_DEVICE
pmem_load_bin(img_file)
npc_init()               RTL reset
init_timer_alarm()       如果 CONFIG_HAS_TIMER
difftest_init()          如果 CONFIG_DIFFTEST
init_disasm()
itarce_log_file()
init_sdb()
```

这个顺序很重要：PMEM/MMIO 先准备好，再复位 RTL，最后让 difftest REF 拿到同一份镜像和初始寄存器状态。

### 3.3 复位流程

`npc_init()` 手动驱动 reset：

```text
clk=0, rst=1, eval()
dump wave
clk=1, rst=1, eval()     上升沿执行 RTL reset
npc_sim_state.state = NPC_STOP
dump wave
clk=0, rst=0, eval()
dump wave
```

复位后 CPU PC 为 `0x80000000`，CSR 初始化在 `csr.v` 中完成：

```text
mstatus = 0x00001800
mtvec   = 0x80000000
mepc/mcause/mip/mie/mtval/mscratch = 0
mcycle  = 0
```

### 3.4 主执行循环

`npc_exec()` 是状态机：

```text
NPC_RUNNING  -> single_cycle(); cycle++
NPC_STOP     -> sdb_mainloop()
NPC_END      -> return
NPC_ABORT    -> return
NPC_QUIT     -> exit loop
```

由于当前 `CONFIG_BATCH=1`，`sdb_mainloop()` 进入后会直接执行 `cmd_c()`，把状态改成 `NPC_RUNNING`，也就是默认批处理运行，不停在交互调试器。

### 3.5 一个周期的精确顺序

核心函数是 `main.cpp::single_cycle()`：

```text
1. interrupt_check()
   - 根据 host timer 和 CSR(mstatus/mie/mip) 判断是否有中断
   - 如果有，设置 top->interrupt_valid/cause

2. 记录当前 this_pc 和 this_inst
   - this_pc 用于 difftest/itrace 的“刚执行指令 PC”
   - this_inst 用于 itrace 反汇编

3. clk=1; top->eval()
   - RTL 在上升沿提交 PC/GPR/CSR/store/halt

4. 清 top->interrupt_valid/cause

5. halt_check()
   - 如果 RTL halt 输出为 1，设置 NPC_END、halt_pc、halt_ret

6. difftest
   - 普通指令：ref exec 1 条，然后比较 PC/GPR/CSR
   - MMIO/异步中断：不同步逐条比较，改为把 DUT 状态同步到 REF

7. watchpoint
   - 如果表达式值变化，进入 NPC_STOP

8. itrace
   - 记录 this_pc 和 this_inst

9. npc_state_check()
   - 打印 good/bad trap 或 abort 原因

10. dump 上升沿波形

11. clk=0; top->eval()

12. dump 下降沿波形
```

这个顺序体现了当前设计的核心假设：一个 `single_cycle()` 对应一条 RTL 指令的提交。difftest 在上升沿之后读取 DUT 状态。

## 4. RTL 顶层设计

### 4.1 `core_top` 的模块关系

```text
                 +------------------+
                 |      if_stage    |
                 | PC / fetch / NPC |
                 +--------+---------+
                          |
                          v instr
                 +------------------+
                 |      id_stage    |
                 | control + imm    |
                 +--------+---------+
                          |
            +-------------+--------------+
            |                            |
            v                            v
     +-------------+              +-------------+
     |   regfile   |              |     csr     |
     +------+------+              +------+------+
            |                            |
            v                            v
                 +------------------+
                 |     ex_stage     |
                 | ALU/branch/CSR W |
                 +--------+---------+
                          |
                          v
                 +------------------+
                 |    mem_stage     |
                 | DPI load/store   |
                 +--------+---------+
                          |
                          v
                 +------------------+
                 |    wb_stage      |
                 | writeback mux    |
                 +--------+---------+
                          |
                          v
                       regfile
```

它不是流水线，而是按阶段拆分的单周期数据通路。IF/ID/EX/MEM/WB 名字用于组织组合逻辑；真正的时序状态主要在：

```text
if_stage.pc
regfile.rf[0..31]
csr 内部寄存器
halt 寄存器
C++ npc_pmem, store 在 posedge 通过 DPI 写入
```

### 4.2 中断 kill 信号

`core_top` 中有一组 `_eff` 信号：

```verilog
assign mem_read_eff   = interrupt_valid ? 1'b0 : mem_read;
assign mem_write_eff  = interrupt_valid ? 1'b0 : mem_write;
assign reg_write_eff  = interrupt_valid ? 1'b0 : reg_write;
assign csr_write_eff  = interrupt_valid ? 1'b0 : csr_write;
assign mret_eff       = interrupt_valid ? 1'b0 : mret;
assign trap_enter_eff = trap_enter | interrupt_valid;
assign trap_code_eff  = interrupt_valid ? interrupt_cause : trap_code;
assign is_ebreak_eff  = interrupt_valid ? 1'b0 : is_ebreak;
```

含义：

- 如果 C++ 在周期开始前发现中断，当前 PC 对应的普通指令被 kill。
- 被 kill 的指令不能写 GPR、不能写 CSR、不能访存、不能执行 mret、不能触发 ebreak halt。
- 同一个周期转为 trap：`trap_enter_eff=1`，`trap_code_eff=interrupt_cause`。
- CSR 在上升沿记录 `mepc=pc`、`mcause=interrupt_cause`，IF 在同一上升沿把下一拍 PC 设为 `mtvec`。

这是一种“中断发生在当前指令执行前”的精确中断模型。mret 后会回到 `mepc`，重新执行被 kill 的那条指令。

## 5. 指令执行数据通路

### 5.1 IF: PC 和取指

`if_stage.v` 负责 PC 选择和取指。

PC 优先级：

```verilog
pc_next = (trap_enter | mret) ? trap_target :
          pc_sel==00 ? pc + 4 :
          pc_sel==01 ? pc + imm_jal :
          pc_sel==10 ? {jump_jalr[31:1], 1'b0} :
          pc_sel==11 ? (br_taken ? pc + imm_br : pc + 4) :
                       pc + 4;
```

取指走 DPI-C：

```verilog
assign instr = dpi_mem_read(pc, 32'b0);
```

第二个参数 `is_load=0` 的意义是：取指虽然会组合读内存，但不应该被 C++ 侧当作 data load MMIO，也不应该触发 difftest 的 MMIO skip。

### 5.2 ID: 字段拆解、立即数、控制

`id_stage.v` 拆出：

```text
opcode   instr[6:0]
rd       instr[11:7]
funct3   instr[14:12]
rs1      instr[19:15]
rs2      instr[24:20]
funct7   instr[31:25]
```

`imm_gen.v` 根据 opcode 生成 I/S/B/U/J 立即数：

```text
I: {{20{instr[31]}}, instr[31:20]}
S: {{20{instr[31]}}, instr[31:25], instr[11:7]}
B: {{19{instr[31]}}, instr[31], instr[7], instr[30:25], instr[11:8], 1'b0}
U: {instr[31:12], 12'b0}
J: {{11{instr[31]}}, instr[31], instr[19:12], instr[20], instr[30:21], 1'b0}
```

`control.v` 是纯组合译码，默认所有副作用关闭：

```text
pc_sel        = PC+4
reg_write     = 0
mem_read      = 0
mem_write     = 0
csr_read      = 0
csr_write     = 0
trap_enter    = 0
mret          = 0
is_ebreak     = 0
```

然后按 opcode/funct3/funct7 覆盖需要的控制信号。

### 5.3 控制信号速查

```text
alu_src_a:
  0 = rs1_rdata
  1 = pc

alu_src_b:
  00 = rs2_rdata
  01 = imm_out
  10 = 4

reg_wdata_src:
  00 = alu_result
  01 = mem_rdata
  10 = pc_plus4
  11 = csr_rdata

pc_sel:
  00 = pc + 4
  01 = jal, pc + imm_j
  10 = jalr, (rs1 + imm_i) & ~1
  11 = branch, br_taken ? pc + imm_b : pc + 4

br_type:
  000 = beq
  001 = bne
  010 = blt
  011 = bge
  100 = bltu
  101 = bgeu

lsu_type:
  000 = lw
  001 = lh
  010 = lb
  011 = lhu
  100 = lbu
  101 = sw
  110 = sh
  111 = sb
```

### 5.4 EX: ALU、分支、CSR 写数据

`ex_stage.v` 选择 ALU 输入：

```text
src1 = alu_src_a ? pc : rs1_rdata
src2 = alu_src_b==00 ? rs2_rdata :
       alu_src_b==01 ? imm_out :
                       32'd4
```

ALU 支持：

```text
RV32I:
  add/sub/sll/slt/sltu/xor/or/and/srl/sra/lui

RV32M:
  mul/mulh/mulhsu/mulhu/div/divu/rem/remu
```

除法和取余函数显式处理了 RISC-V 边界语义：

```text
divisor == 0:
  div/divu -> 0xffffffff
  rem/remu -> dividend

signed overflow:
  0x80000000 / -1 -> 0x80000000
  0x80000000 % -1 -> 0
```

分支判断在 `br_cond.v` 中完成，输出 `br_taken` 反馈给 IF。

CSR 写数据在 EX 阶段生成：

```text
csr_src = csr_imm ? rs1_rdata : csr_zimm
csrrw/csrrwi: csr_wdata = csr_src
csrrs/csrrsi: csr_wdata = csr_rdata | csr_src
csrrc/csrrci: csr_wdata = csr_rdata & ~csr_src
```

这里变量名 `csr_imm` 的含义有些反直觉：当前代码里 `csr_imm=1` 表示使用 `rs1_rdata`，`csr_imm=0` 表示使用 `zimm`。

### 5.5 MEM: load/store 和 DPI 内存

`mem_stage.v` 中：

```verilog
assign mem_rdata_raw = dpi_mem_read(mem_addr, {31'b0, mem_read});

always @(posedge clk) begin
  if (mem_write) begin
    dpi_mem_write(mem_addr, mem_wdata, {28'b0, mem_wmask});
  end
end
```

设计意图：

- load 是组合读，数据在同一周期传给 WB。
- store 是时序写，上升沿提交副作用。
- `mem_read` 作为 `is_load` 传给 C++，只有真正 load 到 MMIO 才触发 difftest skip。

`lsu.v` 根据地址低位选择字节/半字，并生成写掩码：

```text
load:
  lw  -> 原始 32 bit
  lh  -> 选低/高半字并符号扩展
  lhu -> 选低/高半字并零扩展
  lb  -> 选四个 byte 之一并符号扩展
  lbu -> 选四个 byte 之一并零扩展

store:
  sw -> wmask 1111, wdata = rs2
  sh -> wmask 0011 或 1100, wdata = {2{rs2[15:0]}}
  sb -> wmask 0001 << addr[1:0], wdata = {4{rs2[7:0]}}
```

当前没有实现 misaligned load/store exception。未对齐访问会按现有 LSU 选择和 C++ 字对齐读写行为执行，不会进入 trap。

### 5.6 WB 和寄存器堆

`wb_stage.v` 只做写回 MUX：

```text
00 -> alu_result
01 -> mem_rdata
10 -> pc_plus4
11 -> csr_rdata
```

`regfile.v`：

- 32 个 32-bit GPR。
- x0 读恒为 0。
- 写在 posedge。
- `rd_addr != 0` 才写，保证 x0 不被修改。
- 额外输出 `rs_a0=rf[10]` 给 halt 模块作为程序返回值。

### 5.7 ebreak 和仿真结束

`control.v` 对 `ebreak` 只设置 `is_ebreak=1`，不作为架构异常进入 CSR trap。`halt.v` 在上升沿看到 `is_ebreak` 后：

```text
halt_reg = 1
halt_pc  = pc
halt_ret = x10/a0
```

C++ 侧 `halt_check()` 看到 `top->halt` 后进入 `NPC_END`。

所以当前 NPC 里：

- `ecall` 是架构 trap，会写 `mepc/mcause/mstatus` 并跳到 `mtvec`。
- `mret` 是架构 trap return，会跳到 `mepc`。
- `ebreak` 是仿真结束信号，用于 AM 测试的 good/bad trap。

## 6. CSR、异常和中断

### 6.1 CSR 模块

`csr.v` 实现：

```text
mstatus   0x300
mie       0x304
mtvec     0x305
mscratch  0x340
mepc      0x341
mcause    0x342
mtval     0x343
mip       0x344
mcycle    0xB00/0xB80
```

CSR 读是组合读：

```text
csr_read=1 时根据 csr_addr 返回对应 CSR，否则返回 0
```

CSR 写、trap、mret 都在 posedge 提交，优先级：

```text
reset
  > trap_enter
  > mret
  > csr_write
```

### 6.2 trap 进入

`trap_enter=1` 时：

```text
如果 trap_code 是 timer/ext interrupt，清 mip 对应 pending bit
mepc    <- trap_pc
mcause  <- trap_code
mstatus.MPIE <- mstatus.MIE
mstatus.MIE  <- 0
mstatus.MPP  <- 3
trap_target = mtvec
```

`trap_pc` 在 `core_top.v` 中接当前 `pc`。

对于 `ecall`，`control.v` 设置：

```text
trap_enter = 1
trap_code  = 11
```

对于外部注入的中断，`trap_code_eff = interrupt_cause`。

### 6.3 mret 返回

`mret=1` 时：

```text
trap_target = mepc
mstatus.MIE  <- mstatus.MPIE
mstatus.MPIE <- 1
mstatus.MPP  <- 0
```

当前代码注释写“MPP <- 0 S特权级”，但 RISC-V 编码里 MPP=00 表示 U-mode，S-mode 是 01。这个注释需要以后修正；代码行为是把 MPP 清零。

### 6.4 C++ 中断源

中断入口在 `monitor/interrupt.cpp`。

`rtc.cpp::init_timer_alarm()` 使用 `setitimer(ITIMER_VIRTUAL)`，约 60Hz 触发 `SIGVTALRM`。信号处理函数只做一件事：

```c
alarm_fired = 1;
```

每个 `single_cycle()` 开始时调用 `interrupt_check()`：

```text
top->interrupt_valid = 0
top->interrupt_cause = 0
difftest_sync_needed = false

如果当前不是 NPC_RUNNING，直接返回

如果 alarm_fired:
  alarm_fired = 0
  mip.MTIP = 1
  difftest_sync_needed = true

isa_query_intr():
  如果 mstatus.MIE=0，无中断
  如果 mip.MTIP && mie.MTIE，返回 0x80000007
  如果 mip.MEIP && mie.MEIE，返回 0x8000000b

如果有中断:
  top->interrupt_valid = 1
  top->interrupt_cause = trap_cause
  difftest_sync_needed = true
```

### 6.5 中断和 difftest 的关系

定时器中断由宿主机信号触发，不是 NEMU 和 NPC 可以天然同步复现的确定性指令行为。因此当前策略是：

- 发生 timer pending 或中断响应时，设置 `difftest_sync_needed`。
- 本周期不做普通 difftest compare。
- 上升沿后读取 DUT 状态，并用 `ref_difftest_regcpy(..., DIFFTEST_TO_REF)` 把 REF 同步成 DUT。

这牺牲中断附近的逐条比较，但避免了异步事件导致的误报。

## 7. 统一内存和 MMIO

### 7.1 PMEM

`memory.cpp` 中：

```c
uint8_t npc_pmem[PMEM_SIZE];
```

地址范围：

```text
PMEM_BASE = 0x80000000
PMEM_SIZE = 128MB
PMEM_END  = 0x88000000
```

`pmem_load_bin()` 直接把 binary 从 offset 0 加载到 `npc_pmem`，也就是 guest 地址 `0x80000000` 起始处。

### 7.2 DPI-C 内存读

```c
int dpi_mem_read(int addr, int is_load)
```

行为：

```text
如果 addr 在 PMEM:
  word_addr = addr & ~3
  返回 pmem_read(word_addr, 4)

否则如果 is_load==0:
  返回 0

否则:
  mmio_accessed = true
  返回 mmio_read(addr)
```

这里的字对齐读取非常关键：RTL LSU 根据 `mem_addr[1:0]` 自己选择 byte/half，所以 C++ 每次只需要返回包含目标 byte 的 32-bit word。

`is_load==0` 时访问非 PMEM 返回 0，是为了防止组合路径中的取指或非 load 指令误触发 MMIO。

### 7.3 DPI-C 内存写

```c
void dpi_mem_write(int addr, int data, int wmask)
```

行为：

```text
如果 addr 在 PMEM:
  word_addr = addr & ~3
  对 wmask[0..3] 为 1 的 byte 写入 npc_pmem

否则:
  mmio_accessed = true
  mmio_write(addr, data, wmask)
```

### 7.4 MMIO 注册表

`device/mmio.cpp` 管理一个最多 16 项的 MMIO map：

```text
name
space
addr_start
addr_end
wmask
read callback
write callback
```

`init_mmio()` 当前注册：

```text
serial:
  addr = CONFIG_SERIAL_MMIO = 0xa00003f8
  len  = 8
  read = NULL
  write = serial_write_handler

rtc:
  addr = CONFIG_RTC_MMIO = 0xa0000048
  len  = 8
  read = rtc_read_handler
  write = NULL
```

注册时会检查：

- 设备地址不能落在 PMEM 内。
- 设备地址范围不能和已有 MMIO 设备重叠。

### 7.5 串口

`serial_write_handler()` 在 offset 0 且 `wmask & 0x1` 时输出 `space[0]` 到 `stderr`：

```text
guest store byte/word -> mmio_space_write 更新 dev->space
                      -> serial_write_handler 从 dev->space[0] 取字符
                      -> fputc(ch, stderr)
```

### 7.6 RTC

`rtc_read_handler()` 提供 64-bit 微秒时间：

```text
offset 0: 返回低 32 位
offset 4: 更新 latch 并返回高 32 位
```

当前实现里 low 读在 `rtc_latched_us==0` 时才初始化，high 读会重新锁存 host 时间。这个接口可用于 AM 中读取时钟。

## 8. difftest 设计

### 8.1 初始化

`difftest_init()` 在传入 `-d diff_so` 时生效。

`init_difftest()`：

```text
dlopen(diff_so)
dlsym:
  difftest_memcpy
  difftest_regcpy
  difftest_exec
  difftest_raise_intr
  difftest_init

ref_difftest_init(1234)
ref_difftest_memcpy(RESET_VECTOR, npc_pmem, img_size, TO_REF)
npc_state_data(top)
ref_difftest_regcpy(&npc_s, TO_REF)
```

也就是先把 NPC 加载的 binary 复制到 REF，再把 NPC 初始寄存器/CSR 状态复制到 REF。

### 8.2 普通单步比较

`difftest_step(top, cycle, this_pc)`：

```text
1. ref_difftest_regcpy(&ref_s, TO_DUT)
   获取 REF 执行前 PC，记为 ref_exec_pc

2. ref_difftest_exec(1)
   REF 执行一条

3. ref_difftest_regcpy(&ref_s, TO_DUT)
   获取 REF 执行后状态

4. npc_state_data(top)
   获取 DUT 执行后状态

5. 把 npc_s.pc/ref_s.pc 覆盖成执行前 PC:
   npc_s.pc = npc_exec_pc
   ref_s.pc = ref_exec_pc

6. diff_log_write()

7. difftest_compare()
   比较 PC/GPR/CSR
```

第 5 步的意义是：log 和 mismatch 定位的是“刚执行的那条指令 PC”，而不是执行后的 next PC。这对调试更直观。

### 8.3 同步代替比较的情况

`single_cycle()` 中：

```text
mmio_accessed = pmem_mmio_accessed()
post_intr_pending = !is_trap && isa_query_intr()!=INTR_EMPTY

如果 mmio_accessed || difftest_sync_needed || post_intr_pending:
  可选 ref_difftest_exec(1)
  npc_state_data(top)
  ref_difftest_regcpy(&npc_s, TO_REF)
  difftest_sync_needed = false
否则:
  difftest_step()
```

原因：

- MMIO 外设行为不一定能被 REF 以同样时序复现。
- host timer 中断是异步事件。
- pending interrupt 在 NPC/REF 的响应边界可能不完全一致。

这些情况下直接同步 REF 状态比硬比较更稳。

## 9. SDB、表达式、监视点和 trace

### 9.1 SDB 命令

`sdb.cpp` 支持：

```text
help           显示命令
c              continue
q              quit
si [n]         单步 n 条，默认 1
info r         打印寄存器
info w         打印监视点
x N EXPR       从地址读 N 个 word
p EXPR         表达式求值，或从文件读取测试表达式
w EXPR         添加监视点
d NO           删除监视点
```

在 `CONFIG_BATCH=1` 下，默认不会停在交互界面，而是自动 continue。

### 9.2 表达式

表达式求值在 `expr.cpp`，通过 POSIX regex 做 tokenizer。寄存器名解析在 `npc_isa.cpp::isa_reg_str2val()`：

```text
$pc
$0 或 $1..$31
ra/sp/gp/tp/t0/.../a0/.../t6
```

虚拟地址读写目前直接映射到 `pmem_read/pmem_write()`，没有地址翻译。

### 9.3 监视点

`watchpoint.cpp` 使用固定大小 `wp_pool[NR_WP]`，维护：

```text
free_list  空闲监视点链表
used_list  已使用监视点链表
```

每周期执行后，如果处于 `NPC_RUNNING`，调用 `check_watchpoint()`。只要任意表达式值变化：

```text
打印变化列表
npc_sim_state.state = NPC_STOP
```

### 9.4 itrace

`itrace.cpp`：

- 使用 Capstone 反汇编。
- 每周期记录 `cycle + pc + instruction bytes + disasm`。
- 同时维护环形缓冲 `irbuf`，用于 assert 失败时打印最近指令。

当前 `monitor.cpp::assert_fail_msg()` 在 `CONFIG_ITRACE` 下递归调用了自己，应该是想调用 `display_irbuf()`。这是一个需要修正的明显问题，否则断言失败时会递归崩溃。

## 10. 当前支持的 ISA 和边界

### 10.1 已支持

从 `control.v` 和 `alu.v` 看，当前支持：

```text
RV32I:
  R-type      add/sub/sll/slt/sltu/xor/srl/sra/or/and
  I-type      addi/slli/slti/sltiu/xori/srli/srai/ori/andi
  load        lb/lh/lw/lbu/lhu
  store       sb/sh/sw
  branch      beq/bne/blt/bge/bltu/bgeu
  jump        jal/jalr
  U-type      lui/auipc

RV32M:
  mul/mulh/mulhsu/mulhu/div/divu/rem/remu

Zicsr / machine mode subset:
  csrrw/csrrs/csrrc/csrrwi/csrrsi/csrrci
  ecall
  mret
  mstatus/mtvec/mepc/mcause/mip/mie/mtval/mscratch/mcycle

Simulation convention:
  ebreak -> halt
```

### 10.2 未完整实现或需要注意

```text
非法指令异常:
  control.v 默认无副作用，没有进入 illegal instruction trap。

访存非对齐异常:
  LSU/C++ 内存模型没有生成 misaligned load/store trap。

ebreak 架构语义:
  当前作为仿真结束，不写 mepc/mcause，也不跳 mtvec。

fence/fence.i:
  control.v 没有显式译码，默认当作无副作用 NOP。

特权级:
  只建模 M-mode 相关 CSR 行为，没有完整 privilege mode 状态机。

地址翻译:
  没有 MMU/SATP，vaddr_read 直接读 PMEM。
```

## 11. 调试时的思维模型

### 11.1 如果指令执行结果错

优先按数据通路定位：

```text
1. instr 是否正确
   - 看 if_stage 的 pc
   - 看 dpi_mem_read 是否从 PMEM 正确取到 word
   - 看 itrace 反汇编

2. 译码是否正确
   - control.v 是否匹配 opcode/funct3/funct7
   - imm_gen 是否生成正确立即数

3. 执行是否正确
   - alu_op 是否正确
   - alu_src_a/b 是否选对
   - br_type/br_taken 是否正确

4. 访存是否正确
   - mem_addr 是否正确
   - lsu_type 是否正确
   - byte/half 选择是否正确
   - wmask/wdata 是否正确
   - C++ dpi_mem_read/write 地址范围是否正确

5. 写回是否正确
   - reg_wdata_src 是否正确
   - rd_addr 是否正确
   - reg_write 是否被 interrupt_valid 屏蔽
```

### 11.2 如果 difftest mismatch

先判断 mismatch 周期是否靠近外部事件：

```text
MMIO load/store?
timer interrupt?
interrupt pending?
ecall/mret/CSR?
```

如果靠近外部事件，重点看 `single_cycle()` 里的同步策略是否进入了 `ref_difftest_regcpy(TO_REF)`。如果是普通指令 mismatch，再按 PC/GPR/CSR 分类：

```text
PC mismatch:
  分支、jal、jalr、trap、mret 优先查

GPR mismatch:
  control -> ex -> mem -> wb -> regfile

CSR mismatch:
  csr_read/csr_write/csr_op
  trap_enter/mret 优先级
  C++ 中断注入和 RTL CSR 更新是否重复/漏掉
```

### 11.3 如果中断行为错

按以下链路检查：

```text
host SIGVTALRM
  -> alarm_fired
  -> interrupt_check()
  -> mip.MTIP
  -> isa_query_intr()
  -> top->interrupt_valid/cause
  -> core_top _eff kill
  -> csr trap_enter
  -> if_stage pc_next=mtvec
  -> mret pc_next=mepc
```

需要特别确认：

- `mstatus.MIE` 是否为 1。
- `mie.MTIE` 是否为 1。
- `mip.MTIP` 是否被置位。
- `interrupt_valid` 只在一个周期有效，`single_cycle()` 上升沿后会清零。
- 被中断 kill 的指令是否没有写 GPR/CSR/内存。

## 12. 已发现的实现注意点

以下不是重新设计建议，只是阅读当前源码时应该知道的风险点：

1. `monitor.cpp::assert_fail_msg()` 在 `CONFIG_ITRACE` 下递归调用自身，应该改为 `display_irbuf()` 或类似函数。

2. `interrupt.cpp::isa_raise_intr()` 会直接写 CSR，但当前主流程实际使用的是 `interrupt_valid/cause` 注入 RTL，由 `csr.v` 写 CSR。`isa_raise_intr()` 目前更像遗留辅助函数，阅读时不要把它当作中断主路径。

3. `mmio.cpp` 的 `MMIODevice::wmask` 字段注册后没有参与 `mmio_space_write()` 的掩码计算，实际使用的是 RTL/LSU 传入的 `wmask`。

4. `if_stage.v` 和 `halt.v` 的 reset vector 是 RTL 局部参数，不直接来自 `CONFIG_RESET_VECTOR`。

5. `csr.v` 中 mret 注释把 MPP=00 写成 S 特权级，不符合 RISC-V 编码；代码行为是清零 MPP。

6. 当前非法指令、访存非对齐、ebreak 架构 trap 都没有完整异常建模。对 AM 测试来说可能足够，但跑更完整的软件时需要补。

## 13. 关键源码索引

```text
主循环:
  csrc/main.cpp::single_cycle
  csrc/main.cpp::npc_exec

初始化:
  csrc/monitor/monitor.cpp::monitor_init
  csrc/monitor/monitor.cpp::npc_init

内存:
  csrc/memory/memory.cpp::dpi_mem_read
  csrc/memory/memory.cpp::dpi_mem_write

MMIO:
  csrc/device/mmio.cpp::add_mmio_device
  csrc/device/mmio.cpp::mmio_read
  csrc/device/mmio.cpp::mmio_write
  csrc/device/serial.cpp::serial_write_handler
  csrc/device/rtc.cpp::rtc_read_handler

difftest:
  csrc/difftest/dut.cpp::init_difftest
  csrc/difftest/dut.cpp::difftest_step
  csrc/difftest/dut.cpp::difftest_compare

中断:
  csrc/monitor/interrupt.cpp::interrupt_check
  csrc/monitor/interrupt.cpp::isa_query_intr
  vsrc/core_top.v 的 _eff kill 逻辑
  vsrc/csr.v 的 trap_enter/mret 逻辑

RTL 数据通路:
  vsrc/core_top.v
  vsrc/if_stage.v
  vsrc/id_stage.v
  vsrc/control.v
  vsrc/ex_stage.v
  vsrc/mem_stage.v
  vsrc/lsu.v
  vsrc/wb_stage.v
  vsrc/regfile.v
  vsrc/csr.v
  vsrc/halt.v
```

## 14. 总结

NPC 当前设计的核心不是复杂流水线，而是“单周期 RTL CPU + 强 C++ 仿真基础设施”：

- RTL 负责确定性指令语义：PC、GPR、ALU、LSU、CSR、trap。
- C++ 负责仿真环境：binary 加载、统一内存、MMIO、异步 timer、difftest、SDB、trace、波形。
- DPI-C 是两者边界：RTL 把所有取指/load/store 统一交给 C++ 内存模型。
- difftest 的关键是区分“确定性普通指令”和“外部事件”。普通指令逐条比较，MMIO/异步中断同步 REF。
- 中断设计采用周期开始检查、当前指令 kill、CSR trap、PC 跳 mtvec 的模型，因此 `interrupt_valid` 是贯穿 C++ 和 RTL 的关键控制点。

理解 NPC 时，可以把它看成三条主线交织：

```text
指令数据通路: IF -> ID -> EX -> MEM -> WB
环境交互线:   RTL DPI -> C++ PMEM/MMIO/device
验证调试线:   single_cycle -> difftest/watchpoint/itrace/SDB
```

这三条线对齐后，基本就能定位大多数功能 bug：先看 RTL 的语义是否正确，再看 C++ 环境是否给了正确输入，最后看 difftest 是否在正确的事件边界上比较或同步。
