# CPU 微结构长期学习与实现路线

> 目标：以当前 NPC 为起点，逐步完成一个可验证、可扩展、可接入 SoC 的五级流水线核；在此基础上继续补齐总线、cache、性能计数、微结构 trace 和 fuzzing 基础设施，为后续乱序/超标量处理器设计打基础。

---

## 目录

- [0. 总目标](#0-总目标)
- [1. 路线总览](#1-路线总览)
  - [阶段 A：单周期基线固化](#阶段-a单周期基线固化)
  - [阶段 B：控制包化单周期核](#阶段-b控制包化单周期核)
  - [阶段 C：五级流水线骨架](#阶段-c五级流水线骨架)
  - [阶段 D：流水线正确性](#阶段-d流水线正确性)
    - [D.1 流水线冒险总览](#d1-流水线冒险总览)
    - [D.2 已完成：GPR RAW、Store 与 load-use](#d2-已完成gpr-rawstore-与-load-use)
    - [D.3 待完成：Branch-in-ID 与控制冒险](#d3-待完成branch-in-id-与控制冒险)
    - [D.4 待完成：CSR 数据冒险](#d4-待完成csr-数据冒险)
    - [D.5 待完成：统一存储端口结构冒险](#d5-待完成统一存储端口结构冒险)
  - [阶段 D 正确性待办](#阶段-d-正确性待办)
  - [阶段 E：同步访存接口](#阶段-e同步访存接口)
  - [阶段 F：AXI4-Lite / AXI 总线](#阶段-faxi4-lite--axi-总线)
  - [阶段 G：SoC 接入](#阶段-gsoc-接入)
  - [阶段 H：cache 与性能优化](#阶段-hcache-与性能优化)
  - [阶段 I：CPU fuzzing 基础设施](#阶段-icpu-fuzzing-基础设施)
  - [阶段 J：乱序 / 超标量路线](#阶段-j乱序--超标量路线)
- [2. 当前阶段定位](#2-当前阶段定位)

---

## 0. 总目标

这条路线不以完成一生一芯考核为最终目的，而是把一生一芯 B 阶段中的总线、SoC、时序优化和流水线内容作为参考材料，建立一条更偏长期工程和研究的 CPU 微结构路线。

最终希望形成：

```text
单周期 NPC
  -> 控制包化单周期核
  -> 五级流水线核
  -> 支持 stall/flush/forward/精确异常
  -> SRAM-like 同步访存接口
  -> AXI4-Lite / AXI bridge
  -> 可接入 SoC 的处理器核
  -> I-cache / D-cache
  -> 性能计数与微结构 trace
  -> 指令/微结构 fuzzing 框架
  -> scoreboard / Tomasulo / ROB
  -> 乱序超标量核心
```

核心原则：

| 原则 | 说明 |
|------|------|
| **先正确，后性能** | 每个阶段先保证可运行、可 difftest、可回退，再考虑优化 |
| **先简单协议，后复杂总线** | 先做 SRAM-like req/resp，再桥接 AXI，避免 CPU 内部一开始被 AXI 细节污染 |
| **先五级流水，后乱序** | 五级流水中的 stall、flush、forward、精确异常是乱序核的前置基础 |
| **可观测性优先** | 每次引入新微结构，都同步建立 trace、事件日志和检查点 |
| **验证驱动设计** | cpu-tests、fceux、rt-thread、difftest、随机测试和 fuzzing 逐层推进 |

---

## 1. 路线总览

### 阶段 A：单周期基线固化

当前已有基础：

```text
RV32I/M 基础指令
CSR / ecall / mret / ebreak
timer interrupt
DPI-C 统一内存
MMIO serial / rtc / keyboard / vga / audio
difftest / itrace / watchpoint
```

本阶段目标：

```text
保持当前单周期核作为黄金基线
整理设计笔记
保证 cpu-tests / fceux / rt-thread 可作为回归测试
```

当前状态：

```text
已完成：集中控制散线 -> 分阶段控制包
已验证：cpu-tests 全 PASS，fceux VGA 可运行，字符模式退出路径 GOOD TRAP，rt-thread 可运行
```

### 阶段 B：控制包化单周期核

这是当前已经完成的阶段，也是后续流水线化的准备工作。

核心思想：

```text
数据通路暂时不变
控制信号不再从 control.v 散线广播
control.v 按阶段职责打包：
  ex_ctrl
  mem_ctrl
  wb_ctrl
  csr_ctrl
  sys_ctrl
  pc_sel
各 stage wrapper 内部拆包使用
```

这个阶段的意义不是提升性能，而是让控制信号具备“随指令流动”的形态。后续一旦加入级间寄存器，就可以直接保存：

```verilog
id_ex_ex_ctrl  <= ex_ctrl;
id_ex_mem_ctrl <= mem_ctrl;
id_ex_wb_ctrl  <= wb_ctrl;
```

### 阶段 C：五级流水线骨架

先不要急着接 AXI。本阶段已经把 CPU 从“阶段化命名的单周期”推进到具有四组级间寄存器的五级流水线骨架；当前工作已经进入阶段 D 的正确性补全。

目标结构：

```text
IF  ->  ID  ->  EX  ->  MEM  ->  WB
 |       |       |       |       |
IF/ID  ID/EX  EX/MEM  MEM/WB  commit
```

第一版流水线目标：

```text
引入 IF/ID、ID/EX、EX/MEM、MEM/WB 寄存器
每个级间寄存器先引入 valid
第一版每周期无条件向后流动，不立即加入 allowin/ready_go
暂时保持 DPI-C 组合取指/load 结构
先用无相关或手工插入 nop 的直线程序验证逐拍流动
```

这一版只建立顺向流水线骨架，暂不同时解决：

```text
数据 forwarding / load-use stall
branch redirect / flush
AXI ready-valid 反压
精确异常和中断提交
```

#### 级间寄存器内容

```text
IF/ID:
  valid
  pc, pc_plus4, inst

ID/EX:
  valid
  pc, pc_plus4, inst
  rs1_addr, rs2_addr, rd_addr
  rs1_rdata, rs2_rdata, imm_out
  ex_ctrl, mem_ctrl, wb_ctrl, csr_ctrl, sys_ctrl

  （2026-07-22 修订：分支/跳转解析前移至 ID，pc_sel 随 PC_CTRL 整包
   同周期前馈 IF，不再进入 ID/EX；ex_ctrl 摘除 br_type 瘦身为 8bit）

EX/MEM:
  valid
  pc, pc_plus4, inst
  rd_addr, alu_result, rs2_rdata
  csr_rdata, csr_wdata,
  mem_ctrl, wb_ctrl, csr_ctrl, sys_ctrl

MEM/WB:
  valid
  pc, pc_plus4, inst
  rd_addr, alu_result, mem_rdata
  csr_rdata, csr_wdata, 
  wb_ctrl,csr_ctrl, sys_ctrl

  （2026-07-22 修订：csr_write/csr_addr 不单独拆传，csr_ctrl 整包携带到 WB，
   执行字段在包内自然死亡，由 csr 提交写口自行拆包）
```

`inst` 和部分 `pc` 信息并非每个执行阶段都需要，但为了 commit-level difftest、itrace、halt 和后续 fuzzing，第一版保守地随指令携带到 WB。

#### 确定锁存信号的全局方法

不能只抄某个模块的输入/输出端口，而要从全局追踪信号生命周期：

```text
1. 信号在哪个阶段产生？
2. 最后在哪个阶段使用？
3. 从产生点到最后使用点需要跨过哪些流水线边界？
```

只要后续阶段还要使用，就继续锁存；最后一个使用阶段消费后，后续立即丢弃：

| 信号 | 产生阶段 | 最后使用阶段 | 需要穿越的级间寄存器 |
|------|----------|--------------|----------------------|
| `ex_ctrl` | ID | EX | `ID/EX` |
| `mem_ctrl` | ID | MEM | `ID/EX -> EX/MEM` |
| `wb_ctrl` | ID | WB | `ID/EX -> EX/MEM -> MEM/WB` |
| `imm_out` | ID | EX | `ID/EX` |
| `rs2_rdata` | ID | MEM（store data） | `ID/EX -> EX/MEM` |
| `rd_addr` | ID | WB | `ID/EX -> EX/MEM -> MEM/WB` |
| `alu_result` | EX | WB | `EX/MEM -> MEM/WB` |
| `mem_rdata` | MEM | WB | `MEM/WB` |
| `pc_plus4` | IF | WB（jal/jalr） | 四个级间寄存器 |

因此控制包不是全部无条件传到 WB，而是逐级消费、逐级缩减：

```text
ID/EX  : EX控制 + MEM控制 + WB控制 + 提交控制
EX消费 : ex_ctrl、CSR执行字段
EX/MEM : MEM控制 + WB控制 + CSR/系统提交字段
MEM消费: mem_ctrl
MEM/WB : WB控制 + CSR/系统提交字段
WB消费 : 完成体系结构状态提交
```

#### 顺向载荷与周期内反馈

当前先完成顺向载荷，但必须保留下面的概念边界：

| 类型 | 示例 | 处理方式 |
|------|------|----------|
| 随指令向后流动 | `pc`、`rd_addr`、`alu_result`、各阶段控制 | 进入必要的级间寄存器 |
| 当前周期反馈 | `br_taken`、`redirect`、`flush`、`stall`、`ready` | 通常不进入顺向级间寄存器，在当前周期末控制寄存器更新 |
| 流水线状态 | `valid` | 保存在级间寄存器中 |
| 全局/外部事件 | `rst`、`interrupt_valid` | 专门同步、挂起或在提交边界处理 |

例如 `br_taken` 现在由 ID 产生（br_cond 前移），随 PC_CTRL 同周期反馈 IF 修改下一个 PC；它不进入任何顺向级间寄存器。当前已经完成重定向，尚未实现配套 flush，因此错误路径年轻指令仍可能进入流水线。

#### 当前流水线骨架：已经完成什么

截至 2026-07-24，当前 RTL 已经加入四组级间寄存器：

```text
IF -> [IF/ID] -> ID -> [ID/EX] -> EX -> [EX/MEM] -> MEM -> [MEM/WB] -> WB
```

每组级间寄存器都保存 `valid` 和该指令后续仍需使用的载荷。当前实现的基本流向如下：

```text
IF/ID:
  if_id_valid
  if_id_pc, if_id_pc_plus4, if_id_inst

ID/EX:
  id_ex_valid
  pc, pc_plus4, inst
  rs1_addr, rs2_addr, rd_addr
  rs1_rdata, rs2_rdata, imm_out
  ex_ctrl, mem_ctrl, wb_ctrl, csr_ctrl, sys_ctrl

EX/MEM:
  ex_mem_valid
  pc, pc_plus4, inst
  rd_addr, alu_result, rs2_rdata
  csr_rdata, csr_wdata, 
  mem_ctrl, wb_ctrl,csr_ctrl, sys_ctrl

MEM/WB:
  mem_wb_valid
  pc, pc_plus4, inst
  rd_addr, alu_result, mem_rdata
  csr_rdata, csr_wdata, 
  wb_ctrl,csr_ctrl, sys_ctrl
```

这说明“顺向流水线骨架”已经完成，但还不能等同于“可正确执行任意程序的五级流水线”。刚完成骨架时，最小 `dummy` 曾因 `auipc/addi` RAW 把 Store 地址算成 `0x00000008`；该问题后来通过 EX forwarding、WB→ID bypass 和 load-use stall 解决。当前仍缺少 Branch-in-ID、flush、CSR 冒险、commit-level difftest 和精确中断。

#### 理解读写周期的本质：模块不等于流水级

这次最容易混淆的问题，是把“模块输入输出”误认为“每经过一个模块就增加一个周期”。真正划分周期的是级间寄存器，而不是组合逻辑模块：

```text
上升沿①
  -> ID/EX 的 Q 端更新
  -> CSR读多路器 / ALU / ex_stage 等组合逻辑传播
  -> EX/MEM 的 D 端稳定
上升沿②
  -> EX/MEM 锁存结果
```

只要读取逻辑使用 `assign` 或 `always @(*)`，它就在当前周期内完成，不额外消耗一个时钟。只有 `always @(posedge clk)` 中保存的状态才跨越周期。

流水线稳定运行时，同一个物理模块的不同端口可以在同一周期服务不同指令：

```text
ID : 指令 B 正在读 REGFILE
EX : 指令 A 正在读 CSR、计算 ALU/CSR 结果
WB : 更老的指令 X 正在写 REGFILE 或 CSR
```

因此必须按“当前端口服务哪个阶段的指令”选择信号，不能继续使用单周期时期的一份原始控制信号广播给所有端口。

#### REGFILE：ID 组合读，WB 时序写

REGFILE 是全核共享的体系结构状态，不是额外流水级。它的读写端口属于两个不同阶段：

```text
读端口：当前 ID 指令
写端口：当前 WB 指令
```

ID 读路径：

```text
if_id_inst
  -> rs1_addr / rs2_addr
  -> REGFILE 组合读
  -> rs1_rdata / rs2_rdata
  -> ID/EX 锁存
```

WB 写路径：

```text
mem_wb_wb_ctrl  -> reg_write_raw
mem_wb_rd_addr  -> rd_addr
WB选择器输出     -> rd_wdata
mem_wb_valid     -> 提交许可
```

正确写使能为：

```verilog
reg_write_eff =
    mem_wb_valid &&
    !interrupt_valid &&
    mem_wb_wb_ctrl[`WB_CTRL_REG_WRITE];
```

为什么不能继续使用 ID 的 `wb_ctrl/rd_addr`？因为某个周期中 ID 和 WB 同时存在两条不同指令。若写使能来自 ID、写地址来自 WB，或者写地址来自 ID、写数据来自 WB，就会把指令 B 的控制和指令 X 的结果拼在一起，破坏体系结构状态。

REGFILE 模块本身不一定需要接收整条 `wb_ctrl`。当前做法是在 `core_top` 从 MEM/WB 控制包拆出并用 `mem_wb_valid` 门控，再把最终写脉冲传给 REGFILE，这种职责划分是合理的。

当前 REGFILE 数据冒险处理状态：

```text
已经解决：
  EX/MEM/MEM-WB -> 普通 EX 消费者 forwarding
  Load -> 普通 EX 消费者的一拍 stall
  Store 地址和写数据 RAW
  WB -> ID 同周期 bypass

尚未解决：
  branch/jalr 在 ID 解析时对 ID/EX、EX/MEM 生产者的依赖
  CSR 结果紧邻写 GPR
```

因此下一步仍需要：

```text
ID/EX 生产者 -> Branch/JALR interlock
EX/MEM -> ID forwarding
EX/MEM Load -> ID 继续 stall
```

#### CSR：EX 组合读，WB 时序写

CSR 同样是全核共享状态，但当前选择的阶段划分与 REGFILE 不同：

```text
CSR读端口：当前 EX 指令
CSR写端口：当前 WB 指令
```

当前选择 EX 读取，而不是 ID 读取，原因是 `ex_stage` 需要在 EX 周期根据旧 CSR 值计算新值：

```text
id_ex_csr_ctrl(A)
  -> CSR组合读
  -> csr_rdata(A)
  -> ex_stage计算
  -> csr_wdata(A)
  -> EX/MEM锁存
```

这里没有周期冲突。`id_ex_csr_ctrl` 在上升沿后稳定，CSR 的组合读多路器在同一个 EX 周期输出 `csr_rdata`，随后 `ex_stage` 计算 `csr_wdata`，下一上升沿才由 EX/MEM 锁存。

因此当前方案中不应增加 `id_ex_csr_rdata`：

```text
正确：id_ex_csr_ctrl -> CSR -> csr_rdata/csr_wdata -> EX/MEM
错误：id_ex_csr_ctrl -> CSR -> csr_rdata -> 再返回 ID/EX
```

如果另选“ID 读取 CSR”的架构，才应使用原始 `csr_ctrl` 直接读 CSR，并把 `csr_rdata` 锁存进 ID/EX。两种方案都能实现，但不能混用。当前源码已经选择 EX 组合读方案。

CSR 指令存在两个不同数据结果：

```text
csr_rdata：CSR旧值，最终写入通用寄存器 rd
csr_wdata：根据 csr_op 计算出的CSR新值，最终写入CSR状态
```

它们都从 EX/MEM 继续传到 MEM/WB：

```text
mem_wb_csr_rdata -> wb_stage -> rd_wdata -> REGFILE
mem_wb_csr_wdata -> CSR写端口
```

所以 CSR 模块必须拆分读控制和写提交控制：

```text
csr_read_ctrl  = id_ex_csr_ctrl
csr_write_ctrl = mem_wb_csr_ctrl
csr_wdata      = mem_wb_csr_wdata
commit_valid   = mem_wb_valid
```

普通 CSR 写提交条件为：

```verilog
csr_write =
    csr_commit_valid &&
    !interrupt_valid &&
    csr_write_raw;
```

为什么不能只保留一份 `csr_ctrl`？同一个周期内 EX 指令 A 需要读 CSR，而 WB 指令 X 可能正在写另一个 CSR：

```text
id_ex_csr_ctrl  属于指令 A
mem_wb_csr_ctrl 属于指令 X
```

若控制来自 MEM/WB、数据却使用 EX 的 `csr_wdata`，就会把 X 的写地址与 A 的写数据组合；若控制全部来自 ID，则会与 EX/WB 两级的指令同时错位。

当前 CSR 仍有后续数据冒险：相邻 CSR 指令访问同一地址时，年轻指令可能在老指令提交前读到旧值。后续需要 CSR forwarding 或 CSR hazard stall。

#### SYS_CTRL：ID 重定向与 WB 提交是两条路径

`sys_ctrl` 包含：

```text
trap_enter
mret
is_ebreak
trap_code
```

这些字段有两类用途：

```text
ID直接反馈路径：尽早决定 IF 的下一 PC
WB顺向提交路径：修改 CSR 状态或结束仿真
```

完整流向：

```text
                            -> IF：判断 ecall/mret 重定向
ID产生 sys_ctrl -----------+
                            -> CSR.sys_redirect_ctrl：选择 mtvec/mepc

ID产生 sys_ctrl
  -> ID/EX -> EX/MEM -> MEM/WB
  -> CSR.sys_commit_ctrl：提交 ecall/mret 状态
  -> HALT：提交 ebreak
```

CSR 因此还需要拆分两份系统控制：

```text
sys_redirect_ctrl = ID 的 sys_ctrl
sys_commit_ctrl   = mem_wb_sys_ctrl
```

如果只把 `mem_wb_sys_ctrl` 交给 CSR 选择 `trap_target`，会发生周期错位。例如 ID 正在译码 ecall，而 WB 是普通指令：

```text
IF 看到 ID.trap_enter = 1，要求跳转
CSR 看到 WB.trap_enter = 0，输出 trap_target = 0
```

等 ecall 到达 WB 时，CSR 才看到 `trap_enter=1`，但 IF 的 ID 指令已经不是 ecall。两边永远不在同一周期。因此 ID 控制只负责目标查询，WB 控制只负责状态提交。

各系统指令的职责：

```text
ecall:
  ID -> 请求跳到 mtvec
  WB -> mepc=mem_wb_pc，mcause=trap_code，更新 mstatus

mret:
  ID -> 请求跳到 mepc
  WB -> 恢复 mstatus

ebreak:
  ID -> 只产生并携带 is_ebreak
  WB -> mem_wb_valid 确认后触发 HALT
```

当前使用 ID 提前重定向、WB 延后提交，因此还需要 flush/序列化。否则异常处理程序可能在 `mepc/mcause/mstatus` 更新前进入流水线。后续可以选择：

```text
方案一：系统指令到 WB 时统一重定向并 flush 年轻指令，逻辑简单且精确
方案二：保留 ID 提前重定向，但暂停异常入口指令直到系统状态提交
```

#### VALID：载荷存在不代表指令有效

级间寄存器在 `valid=0` 时可以保留旧的 pc、inst 和控制包。真正决定副作用能否发生的是对应阶段的 `valid`：

```text
MEM访存   -> ex_mem_valid
GPR写回   -> mem_wb_valid
CSR写提交 -> mem_wb_valid
HALT提交  -> mem_wb_valid
commit    -> mem_wb_valid
```

当前已完成：

```text
mem_stage：mem_valid 门控 mem_read/mem_write
REGFILE  ：mem_wb_valid 门控 reg_write
CSR      ：csr_commit_valid 门控普通CSR写、ecall、mret
HALT     ：mem_wb_valid 门控 ebreak
```

后续加入 flush 后，ID 到 IF 的 `pc_ctrl/sys_ctrl` 反馈也必须由 `if_id_valid` 授权，防止无效槽中残留的 branch/ecall 再次改变 PC。

#### 控制冒险：重定向已经存在，flush 尚未存在

当前 branch、jal、jalr、ecall、mret 都可以在 ID 形成重定向请求，IF 会在当前周期末把 PC 更新为目标地址。但是同一个上升沿中，IF/ID 仍会锁存重定向前已经取出的顺序路径指令：

```text
周期 N：
  ID 判断跳转成立
  IF 已经取到 pc+4 的错误路径指令

周期 N 末上升沿：
  IF.pc    <- redirect_pc
  IF/ID    <- 仍然锁存旧顺序路径指令
```

若没有 flush，这条错误路径指令会像合法指令一样继续执行，表现为并不存在的“延迟槽”。因此后续需要在 `redirect_valid` 时清除 IF/ID 的有效位：

```text
redirect_valid = 1
  -> IF 更新 PC
  -> if_id_valid 清零，向 ID 注入 bubble
```

因为分支解析位于 ID，branch/jal/jalr 一般只需要清除比它年轻的 IF/ID 指令。系统指令是否只清 IF/ID，取决于最终选择“ID 提前重定向”还是“WB 精确提交重定向”；后者需要冲刷所有比提交指令年轻的流水级。

#### HALT：ebreak 必须在 WB 提交

单周期版本可以在 ID 识别 `ebreak` 后立即停止；流水线中这样会让更老的 EX/MEM/WB 指令来不及完成，并且记录到的是 IF 的 PC。

当前已经改为：

```text
is_ebreak_raw = mem_wb_sys_ctrl.IS_EBREAK
is_ebreak_eff = mem_wb_valid && !interrupt_valid && is_ebreak_raw
halt.pc       = mem_wb_pc
halt.ret      = regfile.x10
```

`halt.v` 本身只需要在 `is_ebreak_eff` 有效时锁存 `halt_pc/halt_ret`，无需接收整条 `sys_ctrl`。核心修改是保证它的输入都来自同一条 WB 指令。

#### 仿真环境必须从 cycle-level 改为 commit-level

五级流水线打破了“一个周期完成一条指令”的假设。复位填充、bubble、stall 和 flush 周期都可能没有指令提交，因此需要增加：

```text
commit_valid = mem_wb_valid
commit_pc    = mem_wb_pc
commit_inst  = mem_wb_inst
```

C++ 仿真环境只能在 `commit_valid` 时：

```text
让参考模型执行一条指令
执行 difftest
记录 itrace
增加 guest instruction 计数
检查提交级事件
```

当前 `main.cpp` 仍读取 IF 的 `pc/instr` 并每周期推进参考模型，所以 commit 接口尚未完成。

#### 外部中断：当前门控只是过渡方案

当前 `interrupt_valid` 在同一周期直接作用于 IF、MEM、REGFILE 和 CSR：

```text
IF       -> 立即跳到 mtvec
MEM      -> 禁止 mem_read/mem_write
REGFILE  -> 禁止 reg_write
CSR      -> 注入 trap，禁止普通 CSR 写
```

这种做法继承自单周期核。在流水线中，不同阶段是不同年龄的指令；中断到来时，MEM/WB 中可能是必须完成的老指令。全局屏蔽会把这些老指令的合法副作用一并取消，破坏精确中断语义。同时使用任意周期的 `mem_wb_pc` 保存 `mepc`，也不一定代表正确的恢复边界。

后续精确中断应遵守：

```text
选择一个提交边界接收中断
允许边界之前的老指令完成
阻止 faulting/interrupt 边界之后的年轻指令提交
冲刷流水线中的年轻指令
mepc 保存中断返回后应继续执行的 PC
```

所以当前 `!interrupt_valid` 对 MEM/GPR/CSR/HALT 的门控只能视为骨架阶段的临时保护，不能作为最终五级流水线中断设计。

#### 当前骨架检查结论

```text
已通过：
  Verilator RTL lint（仅剩未使用信号、文件末尾换行等告警）
  ALU rs1/rs2：紧邻、间隔一条、间隔两条 RAW 定向测试
  Store 写数据：紧邻、间隔一条、间隔两条 RAW 定向测试
  Load -> ALU、Load -> Store 的立即 load-use 定向测试

尚未完成：
  Branch/JALR 在 ID 阶段读取操作数时的 forwarding/stall
  redirect 后的 IF/ID flush
  CSR 结果写 GPR 与 CSR 状态写后读冒险
  接入单端口存储器/AXI 后的结构冒险和反压
```

因此当前状态应准确描述为：

```text
五级流水线顺向骨架：完成
阶段读写端口对齐：REGFILE / CSR / SYS / HALT 已完成基础改造
普通 GPR/ALU RAW、Store RAW、普通 EX 消费者 load-use：完成
Branch-in-ID、控制 flush、CSR 冒险、精确异常/中断：未完成
```

### 阶段 D：流水线正确性

顺向五级骨架完成之后，再逐步处理真实流水线问题。

---

## D.1 流水线冒险总览

流水线的根本矛盾是：**多条指令重叠执行，但年轻指令可能依赖老指令尚未完成的结果，也可能与其他阶段争用资源。**

| 冒险类型 | 本质 | 核心解法 |
|------|------|------|
| 数据冒险 | 生产者的新值还没有到达，消费者已经需要使用 | 数据可用时 forwarding；尚不可用时 stall |
| 控制冒险 | IF 尚不知道下一条应取顺序 PC 还是跳转目标 | redirect + flush |
| 结构冒险 | IF、MEM 等阶段同时请求同一个物理硬件端口 | 仲裁 + stall/backpressure，或增加物理端口 |

当前核是顺序、定长五级流水线。GPR 的读发生在前部，写回按程序顺序发生，因此当前重点是 RAW；WAR 和 WAW 在当前结构下不会出现。以后引入乱序执行、多周期执行单元或非顺序写回后，才需要重新处理 WAR/WAW。

分析一个冒险时必须同时回答两个问题：

| 观察维度 | 需要回答的问题 |
|------|------|
| 生产者 | 最终结果是什么？在哪个阶段产生？当前阶段是否已经可用？ |
| 消费者 | 在 ID、EX 还是 MEM 阶段真正需要该值？ |

不能只按指令名称分类。更准确的分类方法是“**写回数据来源 × 消费位置**”：

| 生产者最终结果 | 最早可用位置 | 当前处理状态 |
|------|------|------|
| ALU/LUI/AUIPC 结果，`wdata_src=00` | EX 结束，进入 EX/MEM | EX/MEM、MEM/WB forwarding 已完成 |
| Load 数据，`wdata_src=01` | MEM 结束，进入 MEM/WB | 普通 EX 消费者采用 stall 1 拍 + MEM/WB forwarding |
| JAL/JALR 的 `pc+4`，`wdata_src=10` | 指令自身已有 `pc_plus4` | MEM/WB 最终写回路径已存在；仍依赖正确的控制 flush |
| CSR 旧值，`wdata_src=11` | CSR 在 EX 组合读出 | 相邻 CSR→GPR forwarding 尚未完成 |

| 消费位置 | 典型用途 | 当前处理状态 |
|------|------|------|
| ID | Branch 比较、JALR 目标计算 | 只有 WB→ID，ID/EX 与 EX/MEM 依赖尚未完成 |
| EX | ALU 两个操作数、Load/Store 地址、CSR 寄存器源 | 普通 GPR RAW 已完成 |
| MEM | Store 写入内存的数据 | 在 EX 先修正 rs2，再通过 EX/MEM 携带，已完成 |

> Store 不是寄存器结果生产者，而是 rs1/rs2 的消费者。它可以和 ALU 共用 EX forwarding 网络，但不应与 ALU、Load、CSR 并列为“写回数据来源”。

---

## D.2 已完成：GPR RAW、Store 与 load-use

### D.2.1 为什么会产生 GPR RAW

ID 从 Regfile 组合读出的 `rs1_rdata/rs2_rdata` 会进入 ID/EX。生产者尚未到 WB 时，Regfile 中仍是旧值。如果 EX 直接使用 ID/EX 中的旧值，计算、地址或 Store 数据都会错误。

```text
生产者新值：ALU/访存已经得到，但尚未写回 Regfile
消费者旧值：Regfile -> ID/EX.rs1_rdata/rs2_rdata -> EX
```

解决原则不是等待所有生产者写回，而是：

```text
结果已经可用  -> 从最近的流水线级 forwarding
结果尚不可用  -> stall，等它到达可转发位置
```

### D.2.2 EX 统一 forwarding 网络

`ex_stage.v` 在真正产生 `src1/src2` 之前，先生成 `rs1_real/rs2_real`：

```verilog
ex_mem_reg_write =
    ex_mem_valid &&
    ex_mem_wb_ctrl[`WB_CTRL_REG_WRITE] &&
    (ex_mem_rd_addr != 5'b0);

ex_mem_alu_ready =
    ex_mem_wdata_src == 2'b00;

ex_mem_can_forwading =
    ex_mem_reg_write && ex_mem_alu_ready;

mem_wb_reg_write =
    mem_wb_valid &&
    mem_wb_wb_ctrl[`WB_CTRL_REG_WRITE] &&
    (mem_wb_rd_addr != 5'b0);

rs1_from_ex =
    ex_mem_can_forwading &&
    (rs1_addr == ex_mem_rd_addr);

rs2_from_ex =
    ex_mem_can_forwading &&
    (rs2_addr == ex_mem_rd_addr);

rs1_from_mem =
    mem_wb_reg_write &&
    (rs1_addr == mem_wb_rd_addr) &&
    !rs1_from_ex;

rs2_from_mem =
    mem_wb_reg_write &&
    (rs2_addr == mem_wb_rd_addr) &&
    !rs2_from_ex;

rs1_real = rs1_from_ex  ? ex_mem_alu_result :
           rs1_from_mem ? wb_rd_wdata        : rs1_rdata;

rs2_real = rs2_from_ex  ? ex_mem_alu_result :
           rs2_from_mem ? wb_rd_wdata        : rs2_rdata;
```

| 条件 | 为什么必须存在 |
|------|------|
| `valid` | bubble 中可能残留地址和控制位，不能成为生产者 |
| `reg_write` | 只有真正写 GPR 的指令才能产生依赖 |
| `rd != x0` | x0 永远为零，不能被 forwarding 覆盖 |
| `rd == rs1/rs2` | 判断生产者和消费者是否访问同一个寄存器 |
| EX/MEM 的 `wdata_src==00` | 防止把 Load 的地址误当作加载数据 |
| EX/MEM 优先于 MEM/WB | 两级同时命中时，EX/MEM 是更新、更年轻的生产者 |

MEM/WB 使用 `wb_rd_wdata`，而不是固定使用 `mem_wb_alu_result`。因为 `wb_stage` 已经组合选择出 ALU、Load、PC+4、CSR 中的最终写回值，它与 MEM/WB 属于同一个周期，并没有额外延迟一拍。

### D.2.3 ALU 结果 RAW：三种指令距离

#### 类型 A：生产者与消费者紧邻

```asm
add  x1, x2, x3       # 生产者
sub  x5, x1, x4       # 消费者通过 rs1 使用 x1
```

也可能通过 rs2 使用：

```asm
add  x1, x2, x3
sub  x5, x4, x1       # 消费者通过 rs2 使用 x1
```

消费者进入 EX 时，生产者位于 EX/MEM：

```text
ex_mem_valid + ex_mem_reg_write + rd匹配 + ALU结果就绪
        -> ex_mem_alu_result
        -> EX.rs1_real/rs2_real
        -> ALU.src1/src2
```

#### 类型 B：中间间隔一条指令

```asm
add  x1, x2, x3
addi x6, x0, 1
sub  x5, x1, x4
```

消费者进入 EX 时，生产者位于 MEM/WB：

```text
mem_wb_valid + mem_wb_reg_write + rd匹配
        -> wb_stage 组合选择 wb_rd_wdata
        -> EX.rs1_real/rs2_real
```

#### 类型 C：中间间隔两条指令

```asm
add  x1, x2, x3
addi x6, x0, 1
addi x7, x0, 2
sub  x5, x1, x4
```

此时生产者在 WB，消费者在 ID。Regfile 写回和 ID/EX 锁存发生在同一个上升沿：

```text
生产者：Regfile[x1] <= wb_rd_wdata
消费者：ID/EX.rs1_rdata <= Regfile[x1] 的沿前旧值
```

因此在 `core_top.v` 增加 WB→ID bypass：

```text
mem_wb_valid + reg_write_eff + rd匹配 + id_uses_rs1/rs2
        -> id_rs1_rdata/id_rs2_rdata 选择 wb_rd_wdata
        -> 上升沿锁存进 ID/EX
        -> 后续 EX 使用正确值
```

| 指令距离 | 消费者在 EX 时生产者位置 | 转发路径 | 状态 |
|------|------|------|------|
| 紧邻 | EX/MEM | `ex_mem_alu_result -> EX` | 已完成 |
| 间隔一条 | MEM/WB | `wb_rd_wdata -> EX` | 已完成 |
| 间隔两条 | 生产者在 WB、消费者在 ID | `wb_rd_wdata -> ID -> ID/EX` | 已完成 |

相同网络还覆盖其他 EX 阶段消费者：

| 消费类型 | 指令示例 | 用途 |
|------|------|------|
| ALU→Load 地址 | `addi x1, x2, 16`；`lw x3, 0(x1)` | rs1 参与地址计算 |
| ALU→Store 地址 | `addi x1, x2, 16`；`sw x3, 0(x1)` | rs1 参与地址计算 |
| ALU→Store 数据 | `addi x1, x2, 5`；`sw x1, 0(x3)` | rs2 作为写入数据 |
| ALU→CSR 源 | `addi x1, x0, 8`；`csrrw x0, mie, x1` | rs1 参与 CSR 新值计算 |

### D.2.4 ALU 与 Store 共用 EX forwarding

Store 不产生 GPR 结果，但它需要两个源寄存器：

| Store 源 | 用途 | 消费位置 | 当前数据链 |
|------|------|------|------|
| rs1 | 基地址，计算 `rs1 + imm` | EX | `rs1_real -> ALU -> ex_mem_alu_result` |
| rs2 | 实际写入内存的数据 | MEM | `rs2_real -> ex_rs2_value -> EX/MEM.rs2_rdata -> MEM` |

因此 ALU 与 Store 的统一点不是“它们都是生产者”，而是它们都在 EX 使用修正后的 `rs1_real/rs2_real`。

#### Store 基地址 rs1 冒险

```asm
addi x3, x2, 16       # 产生新的基地址 x3
sw   x1, 0(x3)        # EX 需要新 x3 计算地址
```

这与普通 ALU 的 rs1 RAW 完全相同：

```text
ex_mem_alu_result/wb_rd_wdata
        -> EX.rs1_real
        -> ALU 计算 Store 地址
        -> EX/MEM.alu_result
        -> mem_stage.mem_addr
```

#### Store 写数据 rs2：紧邻

```asm
addi x1, x2, 5
sw   x1, 0(x3)
```

```text
ex_mem_alu_result
        -> EX.rs2_real
        -> ex_rs2_value
        -> EX/MEM.ex_mem_rs2_rdata
        -> mem_stage.mem_wdata_raw
```

#### Store 写数据 rs2：间隔一条

```asm
addi x1, x2, 5
addi x6, x0, 1
sw   x1, 0(x3)
```

生产者已经位于 MEM/WB，使用：

```text
wb_rd_wdata -> EX.rs2_real -> ex_rs2_value -> EX/MEM -> MEM
```

#### Store 写数据 rs2：间隔两条

```asm
addi x1, x2, 5
addi x6, x0, 1
addi x7, x0, 2
sw   x1, 0(x3)
```

生产者 WB 与 Store ID 同周期，先通过 WB→ID bypass 把正确值锁入 ID/EX。Store 到 EX 后，`rs2_real` 默认选择的 `id_ex_rs2_rdata` 已经正确。

> 最终采用的方案是在 `ex_stage` 统一修正 rs2，再输出 `ex_rs2_value`。不是在 ID/EX 输入端为 Store 单独建立三路 MUX，也不是等到 MEM 再修正。

### D.2.5 Load-use：stall 1 拍 + forwarding

#### 为什么 forwarding 不能单独解决

```asm
lw   x1, 0(x2)
addi x3, x1, 7
```

消费者即将进入 EX 时，Load 正在 EX，`alu_result` 只是访存地址。下一周期 Load 在 MEM 才得到 `mem_rdata`，与消费者原本需要该值的 EX 周期重合，尚未进入 MEM/WB。

#### 当前检测条件

```verilog
load_use_stall =
    if_id_valid &&
    id_ex_valid &&
    id_ex_mem_ctrl[`MEM_CTRL_READ] &&
    (id_ex_rd_addr != 5'b0) &&
    ((id_uses_rs1 && id_rs1_addr == id_ex_rd_addr) ||
     (id_uses_rs2 && id_rs2_addr == id_ex_rd_addr));
```

#### stall 的三项同步动作

| 控制动作 | 当前信号 | 目的 |
|------|------|------|
| 冻结 IF 的 PC | `pc_enable = 0` | 防止取指地址继续前进 |
| 保持 IF/ID | IF/ID 输入选择自身旧值 | 消费者继续停留在 ID |
| ID/EX 插入 bubble | `id_ex_valid_in = 0` | 防止消费者带旧值进入 EX |

时序链：

```text
周期 N：
  Load 在 EX，消费者在 ID，load_use_stall=1

上升沿 N：
  Load 正常进入 EX/MEM
  ID/EX 锁存 valid=0
  IF/ID 保持消费者

周期 N+1：
  Load 在 MEM，消费者仍在 ID，EX 是 bubble

上升沿 N+1：
  Load 数据进入 MEM/WB
  消费者进入 ID/EX

周期 N+2：
  消费者进入 EX
  wb_rd_wdata -> rs1_real/rs2_real
```

#### Load-use 指令示例

| 消费类型 | 指令示例 | 使用的源 |
|------|------|------|
| Load→ALU rs1 | `lw x1, 0(x2)`；`addi x3, x1, 1` | rs1 |
| Load→ALU rs2 | `lw x1, 0(x2)`；`add x3, x4, x1` | rs2 |
| Load→Load 地址 | `lw x1, 0(x2)`；`lw x3, 0(x1)` | Load.rs1 |
| Load→Store 地址 | `lw x3, 0(x2)`；`sw x1, 0(x3)` | Store.rs1 |
| Load→Store 数据 | `lw x1, 0(x2)`；`sw x1, 0(x3)` | Store.rs2 |
| Load→CSR 寄存器源 | `lw x1, 0(x2)`；`csrrw x0, mscratch, x1` | CSR.rs1 |

上述消费者都在 EX 使用寄存器，因此当前的一拍 stall + MEM/WB forwarding 可以覆盖。

有间隔时不再属于“必须 stall 的立即 load-use”：

```asm
lw   x1, 0(x2)
addi x6, x0, 1
add  x3, x1, x4       # MEM/WB -> EX
```

```asm
lw   x1, 0(x2)
addi x6, x0, 1
addi x7, x0, 2
add  x3, x1, x4       # WB -> ID bypass
```

### D.2.6 `id_uses_rs1/id_uses_rs2` 与假 stall

指令编码中的 `[19:15]`、`[24:20]` 不一定真的是源寄存器。如果只比较位段，JAL、LUI、CSR 立即数等可能产生假命中。因此 `id_stage` 已经输出真实使用分类：

| 指令类型 | opcode | funct3 | uses_rs1 | uses_rs2 |
|------|------|------|:--:|:--:|
| R 型 ALU / M | `0110011` | 任意 | 是 | 是 |
| I 型 ALU | `0010011` | 任意 | 是 | 否 |
| Load | `0000011` | 任意 | 是 | 否 |
| Store | `0100011` | 任意 | 是 | 是 |
| Branch | `1100011` | 任意 | 是 | 是 |
| JALR | `1100111` | 任意 | 是 | 否 |
| JAL | `1101111` | 任意 | 否 | 否 |
| LUI | `0110111` | 任意 | 否 | 否 |
| AUIPC | `0010111` | 任意 | 否 | 否 |
| CSR 寄存器型 | `1110011` | `001/010/011` | 是 | 否 |
| CSR 立即数型 | `1110011` | `101/110/111` | 否 | 否 |
| ECALL/MRET/EBREAK | `1110011` | `000` | 否 | 否 |

这些信号当前用于 load-use 检测和 WB→ID bypass。以后实现 Branch-in-ID interlock 时也必须复用它们。

### D.2.7 已完成设计中的关键纠偏

| 设计过程中出现的疑问 | 正确认识 | 最终代码位置 |
|------|------|------|
| Store 的 rs2 是地址 | rs1 是基地址，rs2 是写入数据 | `ex_stage`、`mem_stage` |
| forwarding 全部放在 ID/EX 输入 | 紧邻和间隔一条统一在 EX 选择；只有同沿 WB→ID 在 ID 输入旁路 | `ex_stage`、`core_top` |
| `wb_rd_wdata` 已经晚一拍 | WB 是 MEM/WB 后的组合选择，与 MEM/WB 属于同周期 | `wb_stage` |
| Store 需要 MEM 阶段再做一套 forwarding | EX 已生成 `rs2_real`，通过 `ex_rs2_value` 随 Store 传到 MEM | `ex_stage`、`ex_mem_reg` |
| EX/MEM 的 `reg_write` 足以证明数据可转发 | 还必须检查数据来源；Load 的 ALU 结果只是地址 | `ex_mem_wdata_src` |
| Load 可以直接 EX/MEM→EX | EX/MEM 中没有最终 Load 数据 | `load_use_stall` |
| 任意 rs 位段相等都应 stall | 必须结合 `id_uses_rs1/id_uses_rs2` | `id_stage` |

### D.2.8 当前已完成范围

```text
已完成：
  ALU -> ALU/地址/CSR源：紧邻、间隔一条、间隔两条
  ALU -> Store 地址和写数据：紧邻、间隔一条、间隔两条
  Load -> 普通 EX 消费者：立即依赖的一拍 stall + forwarding
  Load -> Store 地址/写数据：立即依赖的一拍 stall + forwarding
  x0 排除、valid 门控、EX/MEM 最新值优先、假 stall 分类

不属于本节“已完成”范围：
  Branch/JALR 在 ID 阶段消费数据
  CSR 结果紧邻写 GPR、CSR 状态写后读
  redirect/flush
  真实单端口存储器或 AXI 反压
```

---

## D.3 待完成：Branch-in-ID 与控制冒险

Branch-in-ID 必须拆成两个独立问题：

| 问题 | 本质 | 需要的机制 |
|------|------|------|
| 分支/JALR 操作数数据冒险 | ID 当场使用 rs1/rs2，但新值仍在 ID/EX 或 EX/MEM | ID forwarding + interlock/stall |
| redirect 后错误路径冒险 | redirect 时顺序 PC 指令已经在 IF | IF/ID flush |

### D.3.1 Branch/JALR 的 ID 数据冒险

`id_stage` 中：

```text
Branch：br_cond(rs1_data, rs2_data)
JALR：redirect_pc = rs1_data + imm
```

它们不等到 EX，因此现有 EX forwarding 无法帮助它们。

#### 紧邻 ALU→Branch

```asm
addi x1, x0, 1
beq  x1, x0, target
```

Branch 在 ID 时，生产者仍在 EX/ID-EX，`ex_mem_alu_result` 还没有越过上升沿。推荐先 stall Branch 一拍，再从 EX/MEM→ID forwarding；也可以建立 EX→ID 组合旁路，但会拉长组合路径。

#### 间隔一条 ALU→Branch

```asm
addi x1, x0, 1
addi x6, x0, 2
bne  x1, x0, target
```

生产者位于 EX/MEM，数据已经可用，应增加：

```text
ex_mem_alu_result -> ID.rs1_data/rs2_data -> br_cond
```

#### 间隔两条 ALU→Branch

```asm
addi x1, x0, 1
addi x6, x0, 2
addi x7, x0, 3
bne  x1, x0, target
```

生产者位于 WB，当前 `wb_rd_wdata -> ID` bypass 已能提供正确值。

#### Load→Branch

```asm
lw   x1, 0(x2)
beq  x1, x0, target
```

当前普通 load-use stall 只暂停一拍。第二周期 Branch 仍停在 ID，而 Load 位于 MEM，结果尚未进入 MEM/WB。因为 Branch 在 ID 当场消费，所以还需要二选一：

```text
方案 1：Load 位于 EX/MEM 时继续 stall，等 WB→ID
方案 2：增加 mem_rdata→ID 旁路
```

第一版推荐方案 1，控制更清楚，也更接近未来同步存储器/AXI 的数据就绪语义。

#### ALU/Load→JALR

```asm
addi x1, x0, 32
jalr x5, 0(x1)
```

```asm
lw   x1, 0(x2)
jalr x5, 0(x1)
```

JALR 只使用 rs1，但与 Branch 具有相同的 ID 消费时点，必须复用同一套 ID forwarding/stall。

### D.3.2 redirect 与 IF/ID flush

当前已经存在以下 redirect 来源，其中前五项由 ID 译码/判断产生，interrupt 来自外部：

```text
JAL
JALR
taken Branch
ECALL
MRET
interrupt
```

但 redirect 发生时，IF 已经取到了顺序下一条指令：

```asm
jal  x0, target
addi x10, x0, 1       # 错误路径，必须被 flush
target:
ebreak
```

当前定向测试中，这条错误路径 `addi` 仍然提交，最终得到 BAD TRAP。

正确时序应为：

```text
周期 N：
  ID = redirect 指令
  IF = 顺序路径指令

上升沿 N：
  PC 接收 redirect_pc
  redirect 指令正常进入 ID/EX
  IF/ID.valid 写入 0，丢弃顺序路径指令

周期 N+1：
  IF 从正确目标取指
  ID 是 bubble
```

最终触发条件不能只看指令编码：

```text
redirect_fire =
    if_id_valid &&
    !id_operand_stall &&
    redirect_request;
```

这是因为未来 flush 后 IF/ID 的数据位可以保留旧指令，只有 `valid=0` 才表示它不再具有语义。如果不使用 `if_id_valid` 门控，残留的 Branch/ECALL/MRET 编码可能再次触发跳转。

### D.3.3 Branch-in-ID 待实现矩阵

| ID 消费者依赖的生产者位置 | ALU结果 | Load结果 | 第一版处理 |
|------|------|------|------|
| ID/EX | 当前 EX 正在产生 | 尚未访存 | stall |
| EX/MEM | 可用 | MEM 周期才产生 | ALU→ID forwarding；Load 继续 stall |
| MEM/WB | `wb_rd_wdata` 可用 | `wb_rd_wdata` 可用 | 复用当前 WB→ID bypass |

---

## D.4 待完成：CSR 数据冒险

CSR 指令同时涉及两套状态：

```text
GPR 结果：
  CSR 旧值 -> csr_rdata -> EX/MEM -> MEM/WB -> wb_stage -> rd

CSR 状态更新：
  rs1/zimm 与 CSR 旧值运算 -> csr_wdata -> EX/MEM -> MEM/WB -> CSR 在 WB 提交
```

所以必须把 CSR 冒险拆成两类，不能统称为一个“CSR forwarding”问题。

### D.4.1 CSR 结果→GPR RAW

#### 紧邻 CSR→ALU

```asm
csrrw x1, mscratch, x2   # x1 得到 mscratch 的旧值
addi  x3, x1, 1          # 紧接着使用 x1
```

消费者进入 EX 时，生产者位于 EX/MEM。最终 GPR 结果位于 `ex_mem_csr_rdata`，但当前 EX/MEM forwarding 只允许 `wdata_src==00` 的 ALU 结果，所以该序列当前会失败。

可选方案：

```text
方案 A：
  根据 ex_mem_wdata_src 建立 EX/MEM 最终结果 MUX
  00 -> ex_mem_alu_result
  10 -> ex_mem_pc_plus4
  11 -> ex_mem_csr_rdata
  01 -> Load 未就绪，禁止 EX/MEM forwarding

方案 B：
  CSR→GPR 紧邻依赖 stall 一拍
  然后复用 MEM/WB.wb_rd_wdata
```

推荐方案 A，使 forwarding 网络按“最终写回来源”统一扩展。

#### 间隔一条 CSR→ALU

```asm
csrrw x1, mscratch, x2
addi  x6, x0, 1
addi  x3, x1, 1
```

生产者位于 MEM/WB，`wb_rd_wdata` 已经选择 CSR 旧值，当前 MEM/WB→EX forwarding 可以覆盖。

#### 间隔两条 CSR→ALU

```asm
csrrw x1, mscratch, x2
addi  x6, x0, 1
addi  x7, x0, 2
addi  x3, x1, 1
```

当前 WB→ID bypass 可以覆盖。

同类消费者还包括：

```asm
csrrw x1, mscratch, x2
sw    x1, 0(x3)          # CSR结果→Store数据
```

```asm
csrrw x1, mscratch, x2
beq   x1, x0, target     # CSR结果→Branch-in-ID，同时受两类未完成机制影响
```

### D.4.2 CSR 状态写后读 RAW

```asm
csrrw x0, mstatus, x2
csrrs x3, mstatus, x0
```

第一条在 EX 计算 `csr_wdata`，但要到 WB 才真正修改 `csr_mstatus`；第二条下一周期就在 EX 组合读取 `csr_mstatus`，因此读到旧状态。

```text
老 CSR 写：ID/EX -> EX/MEM -> MEM/WB -> WB 时序提交
新 CSR 读：                    ID/EX -> EX 组合读取
```

第一版推荐使用 interlock：

```text
年轻指令要读 CSR
&& 流水线中存在更老的 CSR 写
&& csr_read_addr == older_csr_write_addr
    -> 冻结 PC
    -> 保持 IF/ID
    -> ID/EX 插入 bubble
    -> 等老写到 WB 提交后再放行
```

需要检查的老写位置主要是 ID/EX 和 EX/MEM。老写已经处于 MEM/WB 时，会在消费者进入 EX 的上升沿提交；下一周期组合读可以看到新值，通常可以放行。

更多 CSR 状态 RAW 示例：

```asm
csrrwi x0, mie, 8
csrrs  x3, mie, x0
```

```asm
csrrw x0, mcycle, x2
csrrc x3, mcycle, x4
```

### D.4.3 CSR 与系统指令之间的依赖

系统跳转目标也来自 CSR 状态，因此必须纳入 CSR interlock：

```asm
csrrw x0, mtvec, x2
ecall                       # 必须使用新的 mtvec
```

```asm
csrrw x0, mepc, x2
mret                        # 必须跳到新的 mepc
```

```asm
csrrw x0, mstatus, x2
mret                        # mret 修改/恢复 mstatus，必须保持顺序
```

CSR/System 最终至少要保证：

```text
同一 CSR 地址写后读不越过
mtvec/mepc/mstatus 等系统依赖不读取旧状态
CSR 普通写、trap、mret、interrupt 在 WB 提交时有明确优先级
无效槽和错误路径不能提交 CSR/System 副作用
```

---

## D.5 待完成：统一存储端口结构冒险

### D.5.1 当前 DPI 模型为什么没有实际冲突

当前 IF 和 MEM 分别调用 `dpi_mem_read()`。虽然底层使用同一片软件内存，RTL 接口允许同周期发起两次组合调用，相当于提供了两个逻辑访问端口。

因此当前不存在“两个阶段抢一个物理端口但无法同时完成”的结构冒险。不能简单表述为“因为 DPI 不阻塞所以隐藏了冲突”，更准确的说法是：**当前模型没有施加单端口资源约束。**

### D.5.2 接入单端口 SRAM/AXI 后的冲突

任意 Load/Store 位于 MEM 时，IF 仍要取下一条指令：

```asm
lw   x1, 0(x2)       # 到 MEM 后发起数据读
addi x3, x4, 1       # 同周期 IF 还要取后续指令
```

```asm
sw   x1, 0(x2)       # 到 MEM 后发起数据写
addi x3, x4, 1       # IF 同样请求取指
```

若只有一个物理请求端口：

```text
IF 取指请求 ----\
                >---- 仲裁器 ---- 单端口 SRAM / AXI master
MEM 数据请求 ---/
```

第一版通常采用：

| 情况 | 处理 |
|------|------|
| MEM 无请求 | 允许 IF 发起取指 |
| MEM 有 Load/Store 请求 | MEM 优先，冻结 PC 和 IF/ID |
| 请求未 ready | 保持请求字段稳定，相关流水线级不得前进 |
| 响应未 valid | Load 及其后续消费者继续等待 |

接入 AXI 后，问题不只是经典结构冒险，还包括可变延迟和 backpressure。届时需要把当前固定前进的流水线寄存器改造成具有 `valid/ready` 或 `allowin/ready_go` 语义的级间接口。

---

### 阶段 D 正确性待办

```text
已经完成：
  [x] ALU -> EX forwarding（EX/MEM、MEM/WB）
  [x] WB -> ID 同周期 bypass
  [x] Store 地址和写数据复用 EX forwarding
  [x] ex_rs2_value -> EX/MEM -> MEM Store 数据链
  [x] 普通 EX 消费者的 load-use detection + stall
  [x] id_uses_rs1/id_uses_rs2，排除假 stall
  [x] EX/MEM 仅允许已就绪 ALU 结果 forwarding

下一步必须完成：
  [ ] Branch/JALR 的 ID forwarding + interlock
  [ ] branch/jal/jalr/ecall/mret redirect + IF/ID flush
  [ ] CSR 结果 -> GPR 的相邻 forwarding
  [ ] CSR 状态同地址写后读 interlock
  [ ] CSR 与 ecall/mret 的状态依赖
  [ ] ecall/mret/ebreak 精确提交 + flush

可选后延：
  [ ] interrupt 精确注入
  [ ] commit-level difftest
  [ ] 单端口存储器仲裁和 AXI backpressure
```

```text
cycle
IF.valid  IF.pc  IF.instr
ID.valid  ID.pc  ID.instr
EX.valid  EX.pc  EX.alu_op
MEM.valid MEM.pc MEM.addr MEM.read/write
WB.valid  WB.pc  WB.rd WB.wdata WB.wen
stall_reason
flush_reason
forward_select
```

这套 trace 后续会直接服务 fuzzing 和乱序核调试。

### 阶段 E：同步访存接口

在接 AXI 前，先把内部访存接口从 DPI-C 组合读改成同步 req/resp。

内部接口建议先做成 SRAM-like：

```text
req_valid
req_ready
req_addr
req_wdata
req_wmask
req_write

resp_valid
resp_ready
resp_rdata
```

目标：

```text
IF 取指可能等待
MEM load/store 可能等待
流水线支持 IF stall 和 MEM stall
ready_go 不再恒为 1
```

这一阶段的意义：

```text
先掌握“访存等待如何反压流水线”
再接 AXI 会自然很多
```

### 阶段 F：AXI4-Lite / AXI 总线

不要让 CPU 核内部一开始直接暴露完整 AXI。更推荐：

```text
core 内部：简单 req/resp
core 外部：bridge 转 AXI
```

推荐顺序：

```text
1. SRAM-like memory model
2. SRAM-like -> AXI4-Lite bridge
3. AXI4-Lite 接 MMIO / 简单 RAM
4. AXI4 接主存
5. 支持 burst
6. 支持多个 slave 的地址译码
```

需要理解：

```text
AXI read address / read data
AXI write address / write data / write response
valid-ready 握手
outstanding transaction
burst length / size / response
AXI-Lite 与 AXI4 的差异
```

### 阶段 G：SoC 接入

目标是让 CPU 核可以接入一生一芯 SoC 或自建 SoC。

需要明确：

```text
reset vector
地址空间
AXI master 接口
UART
timer
keyboard / vga / framebuffer
中断源
boot ROM / SRAM / Flash / DDR model
```

建议保持 CPU core 与 SoC wrapper 分离：

```text
core_top:
  只描述 CPU 核内部微结构

core_axi_wrap:
  把 core req/resp 转成 AXI

soc_top:
  挂 CPU、RAM、UART、timer、VGA 等设备
```

### 阶段 H：cache 与性能优化

先做简单 cache，不要一开始追求复杂。

推荐顺序：

```text
I-cache direct-mapped
D-cache direct-mapped
uncached MMIO 区分
cache miss stall
cache refill
write-through D-cache
再考虑 write-back / set-associative
```

性能优化逐步加入：

```text
分支预测
更早分支判断
乘除法多周期化
简单 prefetch
性能计数器
```

必须同步建立性能计数：

```text
cycle
instret
icache_miss
dcache_miss
branch_total
branch_mispredict
load_use_stall
mem_stall
flush_count
```

### 阶段 I：CPU fuzzing 基础设施

fuzzing 不是最后才做，而是从五级流水线开始就逐步建立。

基础组件：

```text
随机指令生成器
约束随机程序生成
参考模型 NEMU / Spike
difftest commit 比较
异常/中断可控注入
最小化失败用例
波形/trace 自动保存
失败分类
```

五级流水 fuzzing 关注：

```text
数据冒险组合
load-use
branch flush
CSR 读写
中断打断普通指令
MMIO 与普通访存混合
```

总线/cache fuzzing 关注：

```text
随机 wait state
AXI backpressure
乱序返回约束
cache miss/refill
uncached MMIO
边界地址
非对齐访问
```

乱序核 fuzzing 关注：

```text
ROB 提交顺序
rename 回收
分支错误恢复
load-store ordering
异常精确性
多发射冲突
```

### 阶段 J：乱序 / 超标量路线

进入乱序前，必须已经掌握：

```text
五级流水线
valid-ready
stall/flush
forwarding
精确异常
cache miss stall
总线等待
commit-level difftest
```

推荐学习实现顺序：

```text
1. scoreboard in-order issue
2. 多周期执行单元
3. reservation station
4. Tomasulo wakeup/select
5. ROB 精确提交
6. register renaming
7. free list / physical register file
8. load-store queue
9. branch checkpoint / recovery
10. dual issue
11. 多执行单元
12. 超标量前端
```

乱序核的核心不只是“并行执行”，而是：

```text
如何保持架构状态按顺序提交
如何在错误预测/异常/中断时恢复
如何保证 memory ordering
如何让验证工具看到确定的 commit 序列
```

---

## 2. 当前阶段定位

当前 NPC 所处位置：

```text
阶段 A：单周期基线              ✅
阶段 B：控制包化单周期          ✅
阶段 C 前置：PC_CTRL 整包重构   ✅（2026-07-22，分支解析前移 ID）
阶段 C：五级流水线骨架          ✅（四组级间寄存器 + valid 已接入）
阶段 D：流水线正确性            ⏳ 当前（forward/stall/flush/commit）
```

下一步建议：

```text
1. 保留当前四组级间寄存器和 valid 骨架
2. 增加 forwarding、WB->ID bypass 和 load-use stall
3. 增加 redirect flush，消除 branch/jal/jalr/system 延迟槽
4. 增加 commit_valid/commit_pc/commit_inst
5. 把 difftest、itrace、指令计数改为 commit-level
6. 再处理系统指令序列化、精确中断和 allowin/ready_go
```

这条路线的核心判断：

```text
AXI 很重要，但不应该早于流水线 stall/allowin 机制。
否则总线等待无法自然反压流水线，后续会很痛苦。
```

---

# B-Stage: 控制信号分组——集中控制 → 分布式控制

> 状态：✅ 已完成并通过测试  
> 日期：2026-07-15 ~ 2026-07-16  
> 分支：`b-stage`
>
> ⚠️ **2026-07-22 修订提示**：本章记录的是 B-Stage 完成时的历史快照。
> 其中 `EX_CTRL`（11 bit）、`PC_CTRL`（37 bit 预留）、`pc_sel` 独立端口三处
> 已被后续"C-Stage 前置：PC_CTRL 整包 + 分支解析前移 ID"章节取代：
> EX_CTRL 瘦身为 8 bit（br_type 移入 PC_CTRL）、PC_CTRL 扩为 40 bit 并启用、
> pc_sel 独立端口取消并入 pc_ctrl。阅读时以新章节为准。

---

## 0. 分布式控制核心总架构

```
                    core_top（接线员）
                ┌──────────────────────────────────────┐
control.v       │                                      │
┌──────┐        │  ex_ctrl[10:0]  ────────────────────→ ex_stage
│ 译码 │        │     拆：alu_op, alu_src_a/b, br_type   │
│ 打包 │──bus──→│                                      │
└──────┘        │  mem_ctrl[4:0]  ────────────────────→ mem_stage
                │     拆：mem_read, mem_write, lsu_type  │
                │     内部 interrupt 门控                │
                │                                      │
                │  wb_ctrl[2:0]  ──┬──→ wb_stage        │
                │     拆：reg_wdata_src                  │
                │                  └──→ regfile（写端口） │
                │     core_top代拆：reg_write             │
                │  rd_addr[4:0]：ID独立数据流 → regfile   │
                │                                      │
                │  csr_ctrl[48:0] ──┬──→ ex_stage       │
                │     拆：csr_op, csr_imm, csr_zimm      │
                │     生成 csr_wdata                     │
                │                   └──→ csr             │
                │     拆：csr_read, csr_write, csr_addr  │
                │                                      │
                │  sys_ctrl[34:0] ──┬──→ csr             │
                │                   ├──→ if_stage        │
                │                   └──→ halt            │
                │     csr拆：trap_enter, mret, trap_code  │
                │     if_stage拆：trap_enter, mret        │
                │     core_top代拆：is_ebreak给halt       │
                │     csr/if内部 interrupt 门控           │
                │                                      │
                │  pc_sel[1:0]  ──────────────────────→ if_stage
                │     （独立前馈，不走流水线）             │
                │                                      │
                │  数据流（不打包，照旧直连）：            │
                │  pc, pc_plus4, rs1_rdata, rs2_rdata,  │
                │  imm_out, alu_result, mem_rdata,      │
                │  csr_rdata, rd_wdata ...              │
                └──────────────────────────────────────┘
```

### 核心设计原则

| 原则 | 说明 |
|------|------|
| **数据与控制分离** | 数据通路（pc, rs_data, alu_result...）不走总线，照旧直连 |
| **控制按功能域打包** | 译码信号按目标阶段/功能域分组，5 组总线 + 1 独立信号 |
| **一根总线多消费者** | `csr_ctrl` → ex_stage + csr；`wb_ctrl` → wb_stage + regfile 写使能；`sys_ctrl` → csr + if_stage + halt |
| **谁用谁拆包** | 复杂模块收总线自行拆包；简单模块（regfile/halt）由 core_top 代拆 |
| **门控下放** | `interrupt_valid` 传入 mem_stage/csr/if_stage，各模块内部自行门控 |
| **core_top 只接线** | 顶层不译码：声明总线、连接生产者消费者、为简单模块代拆少量信号 |

---

## 1. 设计动机

### 1.1 改前：单周期集中控制

```
control.v ──alu_op──┬──→ id_stage ──→ core_top ──→ ex_stage
           ──mem_read─┤
           ──reg_write┘  20+ 根散线，"谁要谁拿走"
```

**痛点**：
- 新增控制信号需要改 3+ 个文件的端口声明和连线
- 引入流水线后 ID/EX 寄存器需逐信号声明，代码膨胀
- 控制信号从 ID "广播"到所有阶段，各阶段之间的依赖关系不清晰

### 1.2 改后：分布式控制

```
control.v ──ex_ctrl──→ id_stage ──→ core_top ──→ ex_stage（拆包）
           ──mem_ctrl─→           ──→           ──→ mem_stage（拆包）
           ──wb_ctrl──→           ──→           ──→ wb_stage（拆包）
           ──csr_ctrl─→           ──→           ──→ ex_stage + csr
           ──sys_ctrl─→           ──→           ──→ csr + if_stage + halt
           ──pc_sel───→           ──→           ──→ if_stage

           5 根集装箱 + 1 根快递单，"你的包裹请签收"
```

**优势**：
- 控制信号按目标阶段打包，依赖关系一目了然
- 流水线寄存器"整存整取"：`reg [EX_CTRL_WIDTH-1:0] ex_ctrl_q;` 一行搞定
- 新增信号只改 `.vh` 和 `control.v` 内部拼接，接口不变

---

## 2. 控制信号分组方案

### 2.1 组定义与位宽映射（`ctrl_defs.vh`）

#### EX_CTRL（11 bits）— 执行阶段

> ⚠️ 2026-07-22 修订：br_type 已移入 PC_CTRL，EX_CTRL 瘦身为 8 bit：`{alu_op[7:3], alu_src_a[2], alu_src_b[1:0]}`。以下为历史记录。

```
assign ex_ctrl = {alu_op[4:0], alu_src_a, alu_src_b[1:0], br_type[2:0]};
//                [10:6]       [5]        [4:3]           [2:0]
```

| 信号 | 位宽 | 位段 | `define 宏 |
|------|:----:|------|------------|
| `alu_op` | 5 | `[10:6]` | `` `EX_CTRL_ALU_OP_MSB `` / `` `EX_CTRL_ALU_OP_LSB `` |
| `alu_src_a` | 1 | `[5]` | `` `EX_CTRL_ALU_SRC_A `` |
| `alu_src_b` | 2 | `[4:3]` | `` `EX_CTRL_ALU_SRC_B_MSB `` / `` `EX_CTRL_ALU_SRC_B_LSB `` |
| `br_type` | 3 | `[2:0]` | `` `EX_CTRL_BR_TYPE_MSB `` / `` `EX_CTRL_BR_TYPE_LSB `` |

#### MEM_CTRL（5 bits）— 访存阶段

```
assign mem_ctrl = {mem_read, mem_write, lsu_type[2:0]};
//                  [4]        [3]         [2:0]
```

| 信号 | 位宽 | 位段 | `define 宏 |
|------|:----:|------|------------|
| `mem_read` | 1 | `[4]` | `` `MEM_CTRL_READ `` |
| `mem_write` | 1 | `[3]` | `` `MEM_CTRL_WRITE `` |
| `lsu_type` | 3 | `[2:0]` | `` `MEM_CTRL_LSU_MSB `` / `` `MEM_CTRL_LSU_LSB `` |

#### WB_CTRL（3 bits）— 写回阶段

```
assign wb_ctrl = {reg_write, reg_wdata_src[1:0]};
//                  [2]              [1:0]
```

| 信号 | 位宽 | 位段 | `define 宏 |
|------|:----:|------|------------|
| `reg_write` | 1 | `[2]` | `` `WB_CTRL_REG_WRITE `` |
| `reg_wdata_src` | 2 | `[1:0]` | `` `WB_CTRL_WDATA_SRC_MSB `` / `` `WB_CTRL_WDATA_SRC_LSB `` |

`rd_addr` 已从 `wb_ctrl` 中移除。它由 `id_stage` 从 `inst[11:7]` 独立产生，属于指令数据/目的寄存器元信息；流水化后必须与该指令一起穿过 `ID/EX -> EX/MEM -> MEM/WB`，最终连接 REGFILE 写地址。

#### CSR_CTRL（49 bits）— CSR 控制 + 数据

```
assign csr_ctrl = {csr_read, csr_write, csr_op[1:0], csr_imm, csr_addr[11:0], csr_zimm[31:0]};
//                  [48]       [47]        [46:45]      [44]     [43:32]          [31:0]
```

| 信号 | 位宽 | 位段 | `define 宏 |
|------|:----:|------|------------|
| `csr_read` | 1 | `[48]` | `` `CSR_CTRL_READ `` |
| `csr_write` | 1 | `[47]` | `` `CSR_CTRL_WRITE `` |
| `csr_op` | 2 | `[46:45]` | `` `CSR_CTRL_OP_MSB `` / `` `CSR_CTRL_OP_LSB `` |
| `csr_imm` | 1 | `[44]` | `` `CSR_CTRL_IMM `` |
| `csr_addr` | 12 | `[43:32]` | `` `CSR_CTRL_ADDR_MSB `` / `` `CSR_CTRL_ADDR_LSB `` |
| `csr_zimm` | 32 | `[31:0]` | `` `CSR_CTRL_ZIMM_MSB `` / `` `CSR_CTRL_ZIMM_LSB `` |

> `csr_zimm` 放最低 32 位的好处：EX 阶段取用它时直接 `csr_ctrl[31:0]`，无需移位。

#### SYS_CTRL（35 bits）— 系统 / 异常控制

```
assign sys_ctrl = {trap_enter, mret, is_ebreak, trap_code[31:0]};
//                  [34]        [33]   [32]        [31:0]
```

| 信号 | 位宽 | 位段 | `define 宏 |
|------|:----:|------|------------|
| `trap_enter` | 1 | `[34]` | `` `SYS_CTRL_TRAP_ENTER `` |
| `mret` | 1 | `[33]` | `` `SYS_CTRL_MRET `` |
| `is_ebreak` | 1 | `[32]` | `` `SYS_CTRL_IS_EBREAK `` |
| `trap_code` | 32 | `[31:0]` | `` `SYS_CTRL_TRAP_CODE_MSB `` / `` `SYS_CTRL_TRAP_CODE_LSB `` |

> 流水线冲刷时只需清空 `sys_ctrl_q` 一个寄存器，不用逐信号清零。

#### PC_CTRL（37 bits）— PC 控制（预留）

> ⚠️ 2026-07-22 修订：PC_CTRL 已启用并扩为 40 bit，br_type 入包、pc_sel 独立端口取消；br_taken 与其他字段已同阶段（ID）同方向（→IF），下方 ⚠️ 所述混包问题已消除。以下为历史记录。

| 信号 | 位宽 | 位段 | `define 宏 | 说明 |
|------|:----:|------|-----------|------|
| `pc_sel` | 2 | `[36:35]` | `` `PC_SEL_MSB `` / `` `PC_SEL_LSB `` | ID 发出 |
| `br_taken` | 1 | `[34]` | `` `BR_TAKEN `` | ⚠️ EX 阶段产生，不应在 ID 控制包中 |
| `is_jal` | 1 | `[33]` | `` `IS_JAL `` | ID 发出 |
| `is_jalr` | 1 | `[32]` | `` `IS_JALR `` | ID 发出 |
| `trap_code` | 32 | `[31:0]` | `` `TRAP_CODE_MSB `` / `` `TRAP_CODE_LSB `` | ID 发出 |

> ⚠️ `PC_CTRL` 中的 `br_taken` 是 EX 阶段**产生**的信号，与其他 ID 阶段**发出**的信号混在同一组中。流水线化后需拆分：ID 发出的 PC 控制请求（前馈）vs EX 反馈的跳转结果（反馈），走不同的路径。

#### 汇总

| 总线 | 总宽 | 拼接式子 |
|------|:----:|----------|
| `ex_ctrl` | 11 | `{alu_op(5), alu_src_a(1), alu_src_b(2), br_type(3)}` |
| `mem_ctrl` | 5 | `{mem_read(1), mem_write(1), lsu_type(3)}` |
| `wb_ctrl` | 3 | `{reg_write(1), reg_wdata_src(2)}` |
| `csr_ctrl` | 49 | `{csr_read(1), csr_write(1), csr_op(2), csr_imm(1), csr_addr(12), csr_zimm(32)}` |
| `sys_ctrl` | 35 | `{trap_enter(1), mret(1), is_ebreak(1), trap_code(32)}` |
| `pc_ctrl` | 37 | `{pc_sel(2), br_taken(1), is_jal(1), is_jalr(1), trap_code(32)}` （预留，未启用） |

### 2.2 位段宏的必要性

```verilog
// ❌ 无宏：魔法数字，改一处需改 N 处
assign alu_op = ex_ctrl[10:6];

// ✅ 有宏：单一真相源，自文档化
`define EX_CTRL_ALU_OP_MSB 10
`define EX_CTRL_ALU_OP_LSB  6
assign alu_op = ex_ctrl[`EX_CTRL_ALU_OP_MSB:`EX_CTRL_ALU_OP_LSB];
```

**三大好处**：
- **单一真相源**：位宽/位位置只在 `.vh` 定义一次
- **防错**：编译器检查宏名拼写，数字写错不报错
- **易重构**：增删信号只改 `.vh`，所有引用自动跟随

### 2.3 唯一独立保留的信号：`pc_sel`

> ⚠️ 2026-07-22 修订：pc_sel 已并入 pc_ctrl 总线（`pc_ctrl[39:38]`），独立端口取消，控制信号 100% 总线化。以下为历史记录。

控制信号已按功能打包成 5 根总线，`pc_sel[1:0]` 保留为独立控制端口；`rd_addr` 则作为独立数据/元信息端口输出。

| 信号 | 原因 |
|------|------|
| `pc_sel` | 当前单周期版本中为 ID → IF 前馈；流水线版本中需进入 `ID/EX`，在 EX 形成 redirect 后反馈 IF |
| `rd_addr` | ID 独立产生；不属于控制选择，但必须随指令一直传到 WB |

> 其他原本独立的信号归宿：
> - `csr_zimm` → 合入 `csr_ctrl[31:0]`
> - `trap_enter`, `mret`, `is_ebreak`, `trap_code` → 合入 `sys_ctrl[34:0]`

---

## 3. 核心改造技巧：端口降级 + 末尾打包

### 3.1 问题

原来的 `control.v` 中信号既是端口又是 reg：

```verilog
output reg [4:0] alu_op;   // 不能同时保留这个又新增 bus 端口
```

### 3.2 解决方案

```
原来：output reg 信号  →  直接当端口用
现在：内部 reg 信号    →  assign 拼接  →  output wire 总线端口
```

**三步操作**：

1. **端口声明改总线**：`output wire [WIDTH-1:0] ex_ctrl;`
2. **原端口降级为内部 reg**：`reg [4:0] alu_op;`（去掉 `output`）
3. **末尾打包**：`assign ex_ctrl = {alu_op, alu_src_a, alu_src_b, br_type};`

### 3.3 端口与内部 reg 命名冲突

保留独立端口的信号（如 `pc_sel`），其端口名与内部 reg 冲突。解决：内部 reg 加 `_int` 后缀。

```verilog
// 端口
output wire [1:0] pc_sel;

// 内部 reg（always 块里赋值）
reg [1:0] pc_sel_int;

// 末尾桥接
assign pc_sel = pc_sel_int;
```

---

## 4. interrupt 门控分发策略

### 4.1 设计决策：分布式门控

`interrupt_valid` 从 DPI-C 仿真环境传入，作用是"取消当前指令副作用 + 注入 trap"。选择**分布式门控**：各模块自行处理，而非 core_top 集中门控。

| 模块 | 收什么 | 内部门控逻辑 |
|------|--------|-------------|
| `mem_stage` | `mem_ctrl` + `interrupt_valid` | `mem_read`/`mem_write` 清零（阻止访存） |
| `csr` | `csr_ctrl` + `sys_ctrl` + `interrupt_valid` + `interrupt_cause` | `csr_write`/`mret` 清零，`trap_enter = raw \| interrupt`，`trap_code` 替换为 `interrupt_cause` |
| `if_stage` | `sys_ctrl` + `interrupt_valid` | `trap_enter = raw \| interrupt`，`mret` 清零 |
| **core_top 代劳** | `wb_ctrl` / `sys_ctrl` | `reg_write_eff`（给 regfile）、`is_ebreak_eff`（给 halt） |

### 4.2 两种门控模式

| 模式 | 含义 | 示例 |
|------|------|------|
| **清零** | 中断时取消操作 | `mem_write = interrupt_valid ? 1'b0 : raw` |
| **OR** | 中断时强制注入 | `trap_enter = raw | interrupt_valid` |

### 4.3 `csr.v` 需额外接收 `interrupt_cause`

中断时 `csr_mcause` 应写入 `interrupt_cause`（如 `0x80000007` 定时器中断），而非指令自身的 `trap_code`。因此 `csr` 需额外 `interrupt_cause` 输入端口。

---

## 5. 核心改造技巧

### 5.1 "端口降级 + 末尾打包"（`control.v`）

```
原来：output reg [4:0] alu_op;   ← 既是端口又是 reg
现在：reg [4:0] alu_op;          ← 降级为内部 reg
     assign ex_ctrl = {alu_op, ...};  ← 末尾拼接打包成总线端口
```

### 5.2 端口与内部 reg 命名冲突

`pc_sel` 等保留独立端口的信号，内部 reg 加 `_int` 后缀：

```verilog
output wire [1:0] pc_sel;       // 端口
reg [1:0] pc_sel_int;           // 内部 reg（always 块赋值）
assign pc_sel = pc_sel_int;     // 末尾桥接
```

涉及信号：`pc_sel_int`, `trap_enter_int`, `mret_int`, `is_ebreak_int`, `trap_code_int`

### 5.3 译码逻辑：一行不动

`always @(*)` 块内 case(opcode) 逻辑完全不变。核心原则：**控制信号的产生逻辑与传递方式解耦**。

### 5.4 `csr_zimm` 的陷阱

```verilog
// ❌ 错误：reg 初始化只在 t=0 执行一次，inst 变化时不更新
reg [31:0] csr_zimm = {{27{1'b0}}, inst[19:15]};

// ✅ 正确：wire + assign 持续赋值
wire [31:0] csr_zimm = {{27{1'b0}}, inst[19:15]};
```

---

## 6. 文件修改全记录

---

### 6.1 控制总线定义

| 文件 | 改动 | 复杂度 |
|------|------|:--:|
| `ctrl_defs.vh` | 定义 3-bit `WB_CTRL`；`rd_addr` 独立于控制总线；`CSR_CTRL` 扩至 49 bits（+csr_zimm）；新增 `SYS_CTRL` 35 bits | ⭐ |

### 6.2 译码 + 打包（信号生产者）

| 文件 | 改动 | 复杂度 |
|------|------|:--:|
| `control.v` | 输出从 ~20 根散线 → 5 总线 + pc_sel；内部 reg 降级；末尾打包；csr_zimm 改为 wire+assign | ⭐⭐⭐ |

### 6.3 透传层

| 文件 | 改动 | 复杂度 |
|------|------|:--:|
| `id_stage.v` | 端口从散线改总线；control 例化对应修改；独立输出 `rs1_addr/rs2_addr/rd_addr` | ⭐ |

### 6.4 阶段模块——收总线、拆包、门控

| 顺序 | 文件 | 收什么总线 | 内部拆包 | 门控 | 复杂度 |
|:--:|------|-----------|----------|:--:|:--:|
| ① | `wb_stage.v` | `wb_ctrl` | `rd_wdata_src` | — | ⭐ |
| ② | `mem_stage.v` | `mem_ctrl` | `mem_read/write/lsu_type` | 清零 | ⭐⭐ |
| ③ | `if_stage.v` | `sys_ctrl` | `trap_enter/mret` | 清零+OR | ⭐⭐ |
| ④ | `ex_stage.v` | `ex_ctrl` + `csr_ctrl` | 7 信号（4 EX + 3 CSR） | — | ⭐⭐⭐ |
| ⑤ | `csr.v` | `csr_ctrl` + `sys_ctrl` | 6 信号（3 CSR + 3 SYS） | 清零+OR+替换 | ⭐⭐⭐ |

### 6.5 顶层总装

| 文件 | 改动 | 复杂度 |
|------|------|:--:|
| `core_top.v` | 清理所有旧散线声明；声明 5 总线 + pc_sel；各模块例化从散线改总线；补 6 条数据通路 assign；为 regfile/halt 代拆门控 | ⭐⭐ |

### 6.6 不需改动的模块

| 模块 | 原因 |
|------|------|
| `alu.v` | 收 `src1/src2/alu_op`，由 ex_stage 拆包传入 |
| `br_cond.v` | 收 `rs1/rs2/br_type`，由 ex_stage 拆包传入 |
| `halt.v` | 收 `is_ebreak`，由 core_top 代拆+门控传入 |
| `lsu.v` | 收 `lsu_type/mem_read/mem_write`，由 mem_stage 拆包传入 |
| `regfile.v` | 模块本身保持独立；`reg_write` 由 core_top 从 `wb_ctrl` 代拆+门控，`rd_addr` 来自 ID 独立数据流 |
| `imm_gen.v` | 收 `inst`，由 id_stage 直传 |

---

## 7. 常见错误与教训

| 错误 | 原因 | 教训 |
|------|------|------|
| `` `WB_CTRL_WIDTH `` 未定义 | 忘记 `include "ctrl_defs.vh"` | 每个用到总线宏的文件都要 include |
| `csr_zimm` 声明为 `reg` | reg 初始化只在 t=0 生效 | 需持续赋值的必须 `wire` + `assign` |
| `pc_sel` 在 always 块直接赋值 | 端口改 output wire 后不能 always 赋值 | 内部 reg 加 `_int` 后缀 + 末尾 assign 桥接 |
| `rs1_addr`/`rs2_addr` 未声明 | id_stage 新增输出但 core_top 忘声明 wire | 改模块端口时同步检查顶层连线 |
| `rd_addr` 混入 `wb_ctrl` | 目的寄存器地址是指令元信息，不是写回选择控制 | 从 `wb_ctrl` 移除并作为独立数据流传递 |
| `mem_addr`/`mem_wdata_raw` 未赋值 | 清理旧散线时误删数据通路 assign | 数据通路 assign 不是控制信号，不能删 |
| `.instr(inst)` 端口名不匹配 | id_stage/if_stage 改名 `inst` 但顶层未同步 | 改端口名要全局搜索引用 |

---

## 8. 设计思想总结

### 从"广播"到"传递"

```
单周期（广播）：
  ID 控制信号 ──┬──→ EX
               ├──→ MEM
               └──→ WB
  所有阶段同时看到控制信号

流水线（传递）：
  ID → [ID/EX] → EX → [EX/MEM] → MEM → [MEM/WB] → WB
        ex_ctrl      mem_ctrl      wb_ctrl
        存入          存入          存入
```

### 打包 = 流水线寄存器的天然数据类型

```verilog
// 有了打包，流水线寄存器声明只需：
reg [`EX_CTRL_WIDTH-1:0]  ex_ctrl_id_ex;   // 一行替代 5 个 reg
reg [`MEM_CTRL_WIDTH-1:0] mem_ctrl_id_ex;
reg [`WB_CTRL_WIDTH-1:0]  wb_ctrl_id_ex;
```

### 每阶段只看到自己需要的控制

> 当前已经引入 IF/ID、ID/EX、EX/MEM、MEM/WB 四组级间寄存器。ID 产生的控制包按最后消费阶段逐级携带；PC/SYS 的 ID 重定向路径属于周期内反馈，WB 写回与 CSR/SYS/HALT 属于提交路径。

| 阶段 | 使用的总线 | 穿过（不丢弃） | 丢弃的总线 |
|------|-----------|---------------|-----------|
| EX | `ex_ctrl`, `pc_sel`, `csr_ctrl` 的执行字段 | `mem_ctrl`, `wb_ctrl`, CSR/系统提交字段 | `ex_ctrl`, `pc_sel`, 已消费的 CSR 执行字段 |
| MEM | `mem_ctrl` | `wb_ctrl`, CSR/系统提交字段 | `mem_ctrl` |
| WB | `wb_ctrl`, CSR/系统提交字段 | — | 提交后全部消费 |

> 是否携带控制信号的判断标准是“后续是否还会使用”，不是机械地把 ID 输出的全部总线复制到每一级。当前第一版设计中，`sys_ctrl` 保守地携带到 WB，以便后续统一处理 ecall/mret/ebreak 和精确提交。

---

## 9. 当前进度与测试结果

### 控制包化单周期基线：已完成并通过回归

- [x] `ctrl_defs.vh` — 控制总线位段宏定义
- [x] `control.v` — 译码输出从散线改 5 总线 + pc_sel
- [x] `ctrl_defs.vh/control.v` — `rd_addr` 从 `wb_ctrl` 移除，`WB_CTRL_WIDTH=3`
- [x] `id_stage.v` — 端口透传从散线改总线，独立输出 `rd_addr`
- [x] `wb_stage.v` — 接收 `wb_ctrl` 总线，内部拆包
- [x] `mem_stage.v` — 接收 `mem_ctrl` + `interrupt_valid`，内部拆包 + 门控
- [x] `if_stage.v` — 接收 `sys_ctrl` + `interrupt_valid`，内部拆包 + 门控
- [x] `ex_stage.v` — 接收 `ex_ctrl` + `csr_ctrl` 双总线，内部拆包
- [x] `csr.v` — 普通 CSR 组合读/时序写和系统控制基础功能
- [x] `core_top.v` — 清理旧散线 + 总线直连各模块

### 控制包化单周期基线测试结果

以下结果属于加入级间寄存器之前的单周期基线：

- ✅ `verilator --lint-only --top-module core_top -Ivsrc vsrc/*.v` 通过
- ✅ `am-kernels/tests/cpu-tests` 全部 PASS
- ✅ `fceux-am` VGA 图形模式可运行
- ✅ `fceux-am` 字符模式键盘退出路径可触发 GOOD TRAP；字符显示观感问题属于终端尺寸/字符渲染适配，不属于本次 RTL 控制包化问题

### 当前五级流水线骨架状态

- [x] 引入 IF/ID、ID/EX、EX/MEM、MEM/WB
- [x] 四组级间寄存器引入 `valid`
- [x] MEM、REGFILE、CSR、HALT 的提交副作用完成基础 valid 门控
- [x] CSR 拆分 `csr_read_ctrl/csr_write_ctrl`
- [x] SYS 拆分 `sys_redirect_ctrl/sys_commit_ctrl`
- [x] 当前 RTL lint 通过
- [x] ALU/Store 三种距离 RAW 与普通 load-use 定向测试通过
- [ ] 正式 C++ 构建接口同步（`npc.h` 仍引用顶层旧信号名 `pc`，RTL 已改为 `if_pc`）
- [ ] 当前流水线 cpu-tests 通过
- [ ] 当前流水线完整 `dummy` 回归（原 `0x00000008` 数据冒险已经修复，仍需先完成 flush）

### 下一步展望

- [ ] 增加 commit_valid/commit_pc/commit_inst，改造 difftest 为提交级比较
- [x] 普通 EX 消费者数据冒险：forwarding / load-use stall
- [ ] Branch/JALR ID 数据冒险：ID forwarding / interlock
- [ ] 控制冒险：redirect / IF-ID flush
- [ ] CSR 结果与 CSR 状态冒险
- [ ] 精确异常：kill younger instruction
- [ ] 流水线正确后再引入 allowin/ready_go


---

# C-Stage 前置：PC_CTRL 整包 + 分支/跳转解析前移 ID

> 状态：RTL 重构与 core_top 接线完成；普通 EX forwarding、WB→ID bypass、load-use stall 已完成，Branch-in-ID、CSR 冒险和 flush 尚待实现
> 日期：2026-07-22
> 前置：B-Stage 控制包化单周期核

---

## 0. 一句话

所有控制流判断——branch 判跳、jal/jalr/branch 的目标地址计算——前移到 **ID 一个周期内完成**；
结果以 **PC_CTRL 整包（40 bit，控制）+ redirect_pc（32 bit，数据）** 同周期前馈 IF；
IF 退化为纯选择器：`trap > redirect > pc+4`。

## 1. 设计动机

| 动机 | 说明 |
|------|------|
| **分支惩罚 2 → 1** | EX 解析时跳转需清 IF、ID 两条年轻指令；ID 解析只清 IF/ID 一条 |
| **PC_CTRL 整包自洽** | br_cond 前移后，br_taken 与 pc_sel/br_type/is_jal/is_jalr 全部在 ID 同周期同方向产生，原 ⚠️"跨阶段混包"问题消除，整包启用成立 |
| **控制 100% 总线化** | pc_sel 独立端口取消，B-Stage 遗留的最后一根控制散线消失 |
| **流水化天然正确** | 目标加法器在 ID 用本条指令的 `if_id_pc` 计算；EX 解析方案中"pc_sel 必须锁存进 ID/EX 否则张冠李戴"的问题整体消失 |
| **预测器接口预留** | redirect 收敛为 IF 处唯一挂点：`trap > 误预测纠正(ID) > 预测目标(IF) > pc+4` |

代价（已确认接受）：分支数据冒险变贵——ALU→branch 需停 1 拍、load→branch 需停 2 拍（EX 解析为 0/1）；forwarding 需拉到 ID 级（EX/MEM→ID、MEM/WB→ID）。该代价在阶段 D 兑现。

## 2. 总线位段重定义（`ctrl_defs.vh`）

### EX_CTRL：11 → 8 bit（摘除 br_type）

```text
ex_ctrl = {alu_op(5), alu_src_a(1), alu_src_b(2)}
//         [7:3]      [2]          [1:0]
```

br_type 的唯一消费者是 br_cond；br_cond 前移 ID 后，EX 不再消费 br_type。

### PC_CTRL：37（预留）→ 40 bit（启用，br_type 入包）

```text
pc_ctrl = {pc_sel(2), br_taken(1), br_type(3), is_jal(1), is_jalr(1), trap_code(32)}
//         [39:38]    [37]        [36:34]     [33]      [32]       [31:0]
```

| 字段 | 生产者 | 消费者 | 用途 |
|------|--------|--------|------|
| `pc_sel` | control | if_stage | 标识 branch（==2'b11） |
| `br_taken` | br_cond（ID） | if_stage | 条件跳成立 |
| `br_type` | control | br_cond（ID） | 分支比较类型 |
| `is_jal` | control | if_stage | 无条件跳（redirect_valid） |
| `is_jalr` | control | id_stage 加法器 + if_stage | 选加法器基址 rs1_data；无条件跳 |
| `trap_code` | control | （占位，与 sys_ctrl 重复，后续瘦身候选） | — |

## 3. ID 内部数据流：打包分两步（br_taken 回填）

```text
control ──打包（br_taken 位占位 1'b0）──→ pc_ctrl_int
pc_ctrl_int ──拆 br_type──→ br_cond(rs1_data, rs2_data) ──br_taken──┐
pc_ctrl_int ──拆 is_jalr──→ 目标加法器                               │
                                                                  ▼
pc_ctrl = {pc_ctrl_int[39:38], br_taken, pc_ctrl_int[36:0]}   （回填合成）
```

纪律：**ID 内部一律从 `pc_ctrl_int`（原始包）拆字段**，只有 if_stage 消费回填后的 `pc_ctrl`。

## 4. 目标地址计算：全部移入 ID

```verilog
// id_stage 内
wire [31:0] target_sum = (is_jalr ? rs1_data : if_id_pc) + imm_out;
assign redirect_pc = is_jalr ? {target_sum[31:1], 1'b0} : target_sum;
```

- 一口加法器覆盖旧设计三条路径：`pc+imm_jal`、`pc+imm_br`、`rs1+imm(jump_jalr)`
- jalr 清 bit0 是 RISC-V 规范要求（旧 if_stage `{jump_jalr[31:1],1'b0}` 语义平移）
- ID 只需 `is_jalr` 一位做选择：jal/branch 算法相同（pc+imm），无需区分
- `redirect_pc` 是**数据**，按"数据与控制分离"原则独立走线，不打包

## 5. 模块改造清单

| 文件 | 改动 |
|------|------|
| `ctrl_defs.vh` | EX_CTRL 宏组重定义（8 bit，删 BR_TYPE）；PC_CTRL 宏组重定义（40 bit，加 BR_TYPE 位段）；新增 `RESET_VECTOR` 宏 |
| `control.v` | pc_sel 独立端口删除，pc_ctrl 总线上位；新增 is_jal/is_jalr 内部 reg（默认 0，jal/jalr 分支置位）；末尾打包 br_taken 位占位 0 |
| `id_stage.v` | 新增输入 `if_id_pc`、`rs1_data/rs2_data`；br_cond 移入；目标加法器；br_taken 回填输出 pc_ctrl；新增输出 `redirect_pc` |
| `if_stage.v` | 删 jump_jalr/imm_jal/imm_br/pc_sel 端口；收 pc_ctrl + redirect_pc；`redirect_valid = is_jal \| is_jalr \| (pc_sel==2'b11 & br_taken)`；pc_next 三级化简；sys_ctrl 拆包与 interrupt 门控不变 |
| `ex_stage.v` | 删 br_cond 例化、br_taken 输出、br_type 拆包；ex_ctrl 按 8 bit 拆包；ALU/csr_wdata 逻辑不动 |
| `core_top.v` | 删 pc_sel/jump_jalr/imm_jal/imm_br 散线；新增 pc_ctrl 总线 + redirect_pc 线；id_stage 接 if_id_pc(pc)、redirect_pc |

分工一句话：**ID 负责"算"（判跳 + 算地址），IF 负责"选"（trap > redirect > pc+4）**。

## 6. 与原方案的逐案等价性

| 原 pc_sel 分支 | 原行为 | 新行为 | 等价依据 |
|---|---|---|---|
| `00` | pc+4 | redirect_valid=0 → pc+4 | 三个跳转位全 0 |
| `01` jal | pc+imm_jal | is_jal=1 → if_id_pc+imm_out | imm_jal 本是 imm_out 别名 |
| `10` jalr | {jump_jalr[31:1],1'b0} | is_jalr=1 → {sum[31:1],1'b0} | jump_jalr=alu_result=rs1+imm，同源 |
| `11` branch | br_taken ? pc+imm_br : pc+4 | pc_sel==11 & br_taken → if_id_pc+imm_out | 同上 |
| trap/mret | 最高优先 | 最高优先 | 优先级序不变 |

## 7. 对流水线寄存器清单的影响

- **ID/EX**：删 `id_ex_pc_sel`（pc_sel 不出 ID）；`ex_ctrl` 11→8 bit；合计 **313 → 308 bit**
- **IF/ID**：载荷不变（97 bit）
- **EX/MEM、MEM/WB**：`csr_write`/`csr_addr` 不单独拆传，`csr_ctrl` 整包携带到 WB（执行字段在包内自然死亡，WB 提交写口自行拆包）——**逐级缩减的粒度是"整束"，不是"字段"**；合计 **286 → 322 bit / 281 → 317 bit**
- **生命周期表**：pc_sel 行改为 PC_CTRL 整包行（ID→IF 同周期前馈，不穿越级间寄存器）；ex_ctrl 行删 br_type 说明；新增 redirect_pc 行（ID→IF 数据通路）
- **flush 语义**：ID 解析跳转时仅清 IF/ID 一级（1 条年轻指令），阶段 D 实现

## 8. 后续接口预留

- **ID forwarding 插入点**：`rs1_data/rs2_data` 进 id_stage 之后、br_cond/目标加法器之前插 mux（EX/MEM→ID、MEM/WB→ID 两路），端口不变
- **分支预测挂点**：pc_next 优先级扩为 `trap > 误预测纠正(ID) > 预测目标(IF) > pc+4`；IF/ID 届时加 pred 元数据字段（pred_taken/pred_target）
- **trap 重定向**：当前仍同周期生效（单周期语义）；精确异常落地时重定向点后移至 WB 提交边界，三级选择框架不变

## 9. 验证

- [ ] `verilator --lint-only --top-module core_top -Ivsrc vsrc/*.v` 清零
- [ ] cpu-tests 全 PASS（纯重构，功能应逐周期等价，重点覆盖 branch/jal/jalr 用例）
