# CPU 微结构长期学习与实现路线

> **2026-11-01 验证文件清理：** 按用户要求，`npc/tests/` 及对应测试生成目录已删除，生产源码和常规仿真器保留。下文测试脚本、原始产物路径及通过数字按历史记录理解；当前可复现边界见 [清理记录](test-cleanup.md)。

> 源码目录说明：当前实现位于 `npc/src/vsrc/src/` 和 `npc/src/csrc/src/`，对应头文件位于各自 `include/`；配置生成目录仍为 `npc/include/`。构建入口见 [源码目录与构建说明](../design-process/source-layout.md)。下文历史源码路径按 [迁移验收记录](source-layout-acceptance.md) 中的映射阅读。

> 当前面向 SoC、Linux、顺序双发射及流片的目标与验收关口，见 [NPC 到 SoC、Linux、顺序双发射与流片的目标路线](roadmap-soc-linux-dualissue-tapeout.md)。本文件保留已有阶段的实现笔记；其中历史待办描述以当前 RTL 和独立验收记录核对。

> **当前同步异常专项进度入口：** [M 模式五级流水线精确同步异常进展路线](../design-process/m-mode-precise-exceptions-progress.md)。固定十个 M 模式 CSR 的软件写字段和 REF 已有专项验收；完整 CSR 合规仍按该路线的边界推进，随后处理对齐和 IF/D 错误。

> 历史状态核对日期：2026-10-07。本次更新依据当前源码与已有验收产物，没有重新编译或仿真。当前状态以“0.1 当前进展总表”和“2. 当前阶段定位”为入口；带日期的历史推导保留当时结论。

> 历史记录（2026-10-12）：根据当前源码和 [E 到 Linux/顺序双发射设计路线](../design-process/cpu-design-roadmap-e-to-linux-dualissue.md) 整理 E0～E8 执行与验收计划。本次仅更新文档，没有修改 RTL/C++、编译或仿真；E 仍未实现，工作包和测试矩阵中的未勾选项均为待办。

> **2026-10-18 最新更新：E 首版已实现并完成适用范围闭环验收。** 本次实际修改 RTL/C++ 并在 ysyx-npc 编译/仿真，35 个 cpu-tests、CoreMark 1000 迭代、RT 定时/抢占及 E 专项通过；以 [E 验收记录](e-acceptance.md) 为准。旧阶段文字保留推导背景，当前接线以 E 节和源码为准。

> **2026-10-26 更新：非法指令同步异常的指定原生/AM 场景验收通过。** 用户当前 RTL 的异常载荷、普通写回抑制、WB CSR 现场保存、redirect/kill 和 IRQ 仲裁已做定向仿真及有界原生波形核对；35 项 CPU Difftest、RT-Thread 启动/timer/抢占复验通过。本轮新增测试与记录，没有修改生产 RTL。非法指令用例开启当前 REF 后仍在 `mcause` 失配，不代表异常 Difftest 已完成；完整 X 仍未完成，见 [非法指令验收记录](x-illegal-acceptance.md)。

> **2026-10-27 ISA 复核：异常进入通过，完整 trap 返回仍有规范差异。** 原编码与规范明确非法的全零编码各重新验证 9 个原生配置、4 种 AM handler 模式，共核对 5144 行 VCD/CSV。独立 REF 单步确认两个编码均未生成 cause=2；另在 DUT 的 6 个 MRET 返回实例中观察到 MPP=0，而当前仅 M 模式应为 MPP=3，REF 有相同问题。本轮没有修改生产源码，未重跑 CPU35/RT。保留编码不能一概称为基础 ISA 强制非法；范围、失败证据和后续最小修改方向见 [非法指令 ISA 复核](x-illegal-isa-review-20261027.md)。

> **2026-10-27 后续补齐：指定非法指令 Difftest 已闭环。** 用户已自行修正 DUT/NEMU 的 MRET MPP；本任务未改 RTL，只补 NEMU 非法译码兜底、合法 FENCE 与公共 trap 的 mtval 清理。独立 REF 从 12/29 到 29/29，通过原编码/全零 AM Difftest、CPU35、原生异常和 RT 启动/timer/抢占回归；旧 REF 对同一镜像仍触发原 mcause 失配。当前结论以 [NEMU 非法指令 Difftest 验收](nemu-illegal-difftest-acceptance.md) 为准；完整 X 及 CSR WARL/权限、精确访问异常仍未完成。

> 目标：先形成可验证、可综合、可接入 SoC 的单发射五级流水线基线，再分别推进五级核流片工程，以及单发射 Linux、分支预测、顺序双发射路线。乱序设计保留为更长期研究方向。

> **2026-11-01 更新：十个 CSR 的 M-only 软件写合同已实现并完成指定场景验收。** mstatus 的 S/U/未实现扩展字段、mip 设备状态保留、mie 使能范围和计数器半部写优先级已处理，NEMU 六类 CSR 操作统一应用相同规范合同。RTL 单元 140、独立 REF 267、字段自检 Difftest、CPU35、原生非法指令和 RT 启动/定时抢占回归通过。周期计数器读的 Difftest 对齐、真实 SoC 中断源和完整精确异常仍待完成，见 [M-only CSR 验收记录](m-only-csr-acceptance.md)。

> **2026-11-14 更新：MEM 精确异常已通过指定 DUT 定向验收，完整异常 Difftest 尚未闭环。** 本地对齐/范围错误及指定设备后端错误可进入 WB trap；当前 REF 对未对齐 Load/Store 仍直接读写，首次在 LH 失配。正常访问 Difftest 与 cause=0 短回归通过。本轮未改生产源码，未重跑 CPU35/RT，见 [MEM 异常验收](../exception_interrupt/exception-4_6-load-store-acceptance.md)。

> **2026-11-16 更新：NEMU RAM 未对齐异常缺口已修复。** 当前 RTL 不变，独立 REF 205/205、完整异常序列默认/随机等待 Difftest、CPU35 既有镜像及 cause=0 短回归通过，默认 REF 已重新构建。设备错误仍有无设备 REF 的语义边界，完整 IF/设备/IRQ 碰撞验收未完成，见 [REF 修复与 ISA 对照](../exception_interrupt/nemu-mem-exception-fix-acceptance.md)。

---

## 目录

- [0. 总目标](#0-总目标)
- [0.1 当前进展总表](#01-当前进展总表)
- [0.2 本次补齐的记录与证据边界](#02-本次补齐的记录与证据边界)
- [1. 路线总览](#1-路线总览)
  - [阶段 A：单周期基线固化](#阶段-a单周期基线固化)
  - [阶段 B：控制包化单周期核](#阶段-b控制包化单周期核)
  - [阶段 C：五级流水线骨架](#阶段-c五级流水线骨架)
  - [阶段 D：流水线正确性](#阶段-d流水线正确性)
    - [D.1 流水线冒险总览](#d1-流水线冒险总览)
    - [D.2 已完成：GPR RAW、Store 与 load-use](#d2-已完成gpr-rawstore-与-load-use)
    - [D.3 定向验收完成：控制冒险](#d3-定向验收完成控制冒险)
    - [D.4 定向验收完成：CSR 数据冒险](#d4-定向验收完成csr-数据冒险)
    - [D.5 定向验收完成：统一存储端口结构冒险](#d5-定向验收完成统一存储端口结构冒险)
  - [阶段 D 正确性待办](#阶段-d-正确性待办)
  - [阶段 E：同步访存接口](#阶段-e同步访存接口)
    - [E.0 当前起点与设计范围](#e0-当前起点与设计范围)
    - [E.1 内部端口与事件合同](#e1-内部端口与事件合同)
    - [E.2 模块职责与等待状态](#e2-模块职责与等待状态)
    - [E.3 E0～E8 执行工作包](#e3-e0e8-完成记录)
    - [E.4 定向验收与负例矩阵](#e4-验收范围与未完成项)
    - [E.5 验收记录与退出关口](#e5-验收记录与退出关口)
  - [后续：精确异常与不可撤销副作用](#后续精确异常与不可撤销副作用)
  - [阶段 F：AXI4 bridge](#阶段-faxi4-bridge)
  - [阶段 G：SoC 接入](#阶段-gsoc-接入)
  - [阶段 H：cache 与性能优化](#阶段-hcache-与性能优化)
  - [阶段 I：CPU fuzzing 基础设施](#阶段-icpu-fuzzing-基础设施)
  - [阶段 J：顺序双发射与后续乱序研究](#阶段-j顺序双发射与后续乱序研究)
- [2. 当前阶段定位](#2-当前阶段定位)
  - [2.1 下一步执行顺序](#21-下一步执行顺序)
  - [2.2 Difftest 基线的复用与冻结范围](#22-difftest-基线的复用与冻结范围)

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
  -> 单事务 AXI4 bridge（MMIO 按需适配 AXI4-Lite）
  -> 可接入 SoC 的处理器核
  -> I-cache / D-cache
  -> 性能计数与微结构 trace
  -> 指令/微结构 fuzzing 框架
  -> 单发射 Linux 基线
  -> 分支预测与顺序双发射
  -> 更长期的 scoreboard / Tomasulo / ROB / 乱序研究
```

总线、SoC、cache 和综合/FPGA 构成两个近期目标的共同基础。流片线从边界明确的单发射版本另做 DFT、物理实现与签核；不要求先实现双发射。上述图是能力演进概览，实际依赖和验收顺序见第 2 节及目标路线。

核心原则：

| 原则 | 说明 |
|------|------|
| **先正确，后性能** | 每个阶段先保证可运行、可 difftest、可回退，再考虑优化 |
| **先简单协议，后复杂总线** | 先做 SRAM-like req/resp，再桥接 AXI，避免 CPU 内部一开始被 AXI 细节污染 |
| **先五级流水，后乱序** | 五级流水中的 stall、flush、forward、精确异常是乱序核的前置基础 |
| **可观测性优先** | 每次引入新微结构，都同步建立 trace、事件日志和检查点 |
| **验证驱动设计** | cpu-tests、fceux、rt-thread、difftest、随机测试和 fuzzing 逐层推进 |

## 0.1 当前进展总表

状态分为：**已实现并有指定场景验收**、**源码已有但专项待验收**、**用户反馈通过待归档/复验**、**未实现**。任何 PASS 都限定于记录中的版本、配置、输入与检查项；源码审阅不构成形式证明。

| 内容 | 当前状态 | 依据与适用边界 |
| --- | --- | --- |
| A/B 单周期与控制包化 | 已完成的历史基线 | 保留原有设计和回归记录，不能移作当前五级流水线的回归结果 |
| C 五级骨架、四组级间寄存器与 valid | 已实现 | 当前 `vsrc/*_reg.v` 与 `core_top.v`；尚无完整访存 backpressure |
| D2 GPR RAW、Store、固定访存延迟下的 load-use | 已实现并有定向验收 | EX/MEM、MEM/WB→EX 与 WB→ID；一拍 load-use 结论依赖当前内存模型 |
| D3 redirect/kill | 已实现并有定向验收 | JAL 在 ID，Branch/JALR 在 EX，ECALL/MRET 在 WB；见 [D3 验收](d3-acceptance.md) |
| D4.1 CSR 旧值→GPR | 六项定向场景已验收 | 包含紧邻、间隔一/两条、优先级、rd=x0；见 [D4 验收](d4-acceptance.md) |
| D4.2 CSR 状态 RAW | 七项定向场景已验收 | 同地址 ID stall/bubble；CSRRW/CSRRWI rd=x0 不读旧值 |
| D4.3 CSR/System 与 IRQ 边界 | 指定场景已验收 | WB 查询 mtvec/mepc，CSR 写优先于 IRQ；持续请求测试不等于 SoC 脉冲接口验收 |
| 提交级 DPI 事件与 Difftest | 已实现并用于程序验收 | `dpi_wb_event`→`CycleEvent`→`single_cycle`→`difftest_step`；包括仅 IRQ 无 WB 的事件 |
| MMIO 访问身份与 REF 同步 | 已实现 | MEM 判定后随指令进入 WB；Load 只同步 rd，Store 不让 REF 重复访问设备；设备值本身需独立检查 |
| 键盘读出与消费分离 | 指定专项已验收 | 组合 peek 不出队，MEM 接受后 dequeue；重复求值、rd=x0、kill/恢复及 WB 数据已检查，见 [D5 闭环记录](d5-closure-acceptance.md) |
| timer 与 Difftest 协同 | 指定场景已验收 | DUT/REF 同步 pending，再按实际 irq_take 向 REF 注入中断；宿主 timer 不是 RTL timer |
| ITRACE、si、watchpoint 的提交触发 | 源码已迁移 | ITRACE 用 WB PC/inst，si 按 WB 数计数；调试表达式 `$pc` 仍读 IF PC，专项调试回归未独立归档 |
| Difftest 日志开关/记录上限 | 已实现并有负例验收 | 关闭文件日志仍检出失配；见 [CoreMark 记录的日志配置章节](coremark-acceptance.md) |
| CoreMark 1000 次迭代 + Difftest | 已有 2026-10-04 验收记录 | CRC、NPC/NEMU GOOD TRAP；本次未复跑，原始产物目录的可用性见 0.2 |
| RT-Thread 启动与 timer/tick | 指定配置已验收，2026-10-27 使用补齐后的 REF 复验通过 | main、固定输入 shell、Context 恢复；见 [原记录](rtthread-acceptance.md)与[本轮复验](nemu-illegal-difftest-acceptance.md) |
| RT 两线程 delay/wakeup 与 timer 抢占 | 指定配置已验收，2026-10-27 使用补齐后的 REF 复验通过 | A8/B4、寄存器/栈校验、Difftest、窗口内原生波形资格；见 [原记录](rtthread-delay-acceptance.md)与[本轮复验](nemu-illegal-difftest-acceptance.md) |
| 五级流水线 cpu-tests 35/35 | 2026-10-27 使用补齐后的 REF 独立 Difftest 复验通过 | 复用并摘要绑定 35 个既有镜像、当前 RTL/配置和 REF；见 [本轮记录](nemu-illegal-difftest-acceptance.md) |
| MicroBench | 用户反馈通过，本轮未复跑 | 保留历史用户输出，不将其作为当前非法指令版本的实测结果 |
| NPC 容器与编译工具链 | 已配置并用于上述验收 | `npc/container/`；编译/仿真继续在 ysyx-npc 内执行 |
| D5 共享单端口仲裁 | 固定时延指定场景验收闭环完成 | 独立 `mem_port`、MEM 优先、PC/valid、MMIO 消费/kill 与控制碰撞均有定向证据，见 [基础记录](d5-acceptance.md)及 [专项闭环](d5-closure-acceptance.md) |
| E 同步 req/resp 与可变延迟 | 首版实现及指定验收闭环完成 | 单笔在途、WB 始终可接收、反压/owner/IF 排空/D 许可；见 [E 验收记录](e-acceptance.md)，精确异常留 X |
| X 非法指令同步异常 | 指定原生/AM/REF Difftest 场景验收通过 | 独立 REF 识别非法编码并保存现场；原编码/全零客户及软件恢复有 Difftest，原生测试含三项注入碰撞；见 [最新验收](nemu-illegal-difftest-acceptance.md)，不代表完整 X 完成 |
| 完整精确异常与在途副作用规则 | 未完成 | 非法指令、固定 CSR 合同、cause=0 和 MEM 4/5/6/7 已有指定 DUT 验收；RAM 异常已含 REF/Difftest 验收；IF 错误仍 fail-stop，独立设备模型与完整碰撞/副作用合同待补，见 [REF 修复](../exception_interrupt/nemu-mem-exception-fix-acceptance.md) |
| 总线、可综合最小 SoC、cache | 未实现 | 现有虚拟 MMIO 设备不等于 RTL 外设或 SoC |
| 随机验证、覆盖率、综合/FPGA/流片签核 | 待建立 | 已有定向测试和有界波形工具可复用，尚未形成这些验收闭环 |
| Linux、分支预测、顺序双发射 | 未实现 | 属于共同基础完成后的目标；当前为 M 模式 CSR 子集，尚无 S/U、MMU/TLB 等 Linux 平台基础 |

## 0.2 本次补齐的记录与证据边界

本次补齐了此前分散在验收笔记和对话中的仿真迁移、MMIO/timer、日志配置、RT 调度与 BSP 修正记录；修正了正文中仍称“commit-level Difftest 未完成”“CSR 结果不能转发”的旧状态。Branch-in-ID 是历史方案，当前采用 EX 判定；不会把历史方案的 ID forwarding 待办列为当前必做项。

2026-10-07 只进行源码、文件摘要和记录核对：

- `build/rtthread-delay-f59PA0/rtl-before.sha256` 的 20 项，以及 `inputs.sha256` 的 15 项均与对应文件一致；后者包含镜像、ELF、REF、测试模型、三个生产 C++ 文件及选定测试输入。该清单未覆盖所有软件依赖，不外推为整个仓库的版本证明。
- 当前 BSP `context.c`、NPC `.config`、当前 REF `.so` 分别与该目录的保存副本一致。正例 diagnostic/tick/host 的保存退出码均为 0；off 为预期未完成的 5，不能当作普通程序 PASS。
- 普通 RT 启动三模式回归及波形资格的结果仍见原记录。本次未重新运行仿真，也未采集新的 VCD。
- [CoreMark 验收记录](coremark-acceptance.md)保留了 1000 次迭代通过结果，但其引用的 `build/coremark-full-20261004-gr77Ra` 当前未找到；D3 记录引用的 `build/dpi-regression-20261004-1pNnZW` 也未找到。保留这些已做工作的历史结论；需要重新绑定当前版本或取证时，应在新目录复验，不能声称本次核对了这些原始运行产物。
- cpu-tests/MicroBench 的通过输出属于用户反馈，尚待补齐源码、配置、输入、REF、命令及结果归档。fceux 的单周期历史结果不能代替当前五级核的独立验收。

RT 验收曾定位两处 BSP 软件问题：初始 Context 的 MIE 提前打开，以及同一 timer ISR 多次调度覆盖首次 Context 保存来源。两者的修改和动态证据在 RT 两份验收记录中；该过程没有修改生产 RTL。DUT/REF 同时执行错误软件仍可能保持架构状态一致，因此还需要应用功能检查。

---

## 1. 路线总览

### 阶段 A：单周期基线固化

单周期阶段已有基础（历史记录）：

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

当时状态：

```text
已完成：集中控制散线 -> 分阶段控制包
已验证：cpu-tests 全 PASS，fceux VGA 可运行，字符模式退出路径 GOOD TRAP，rt-thread 可运行
```

### 阶段 B：控制包化单周期核

这是已经完成的历史阶段，也是后续流水线化的准备工作。

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

> 本节保留骨架搭建时的阶段性推导。当前 redirect、CSR 冒险和中断仲裁以阶段 D 的更新及独立验收记录为准。

本阶段已经把 CPU 从“阶段化命名的单周期”推进到具有四组级间寄存器的五级流水线骨架；后续 D2/D3/D4 的指定场景已验收，E 首版同步访存现已实现并验收，下一步理解该实现并补 X。AXI 接入仍在内部访存接口固化之后。

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
  ex_ctrl, mem_ctrl, wb_ctrl, csr_ctrl, sys_ctrl, pc_ctrl

  （当前：JAL 在 ID；Branch/JALR 在 EX，pc_ctrl 经 ID/EX 传入 EX。
   ex_ctrl 为 8bit，br_type 位于 pc_ctrl；2026-07-22 的全 ID 判定方案已被替代。）

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

例如当前 Branch 的 `branch_taken` 由 EX 中的 `br_cond` 根据 forwarding 后的 rs1/rs2 产生，与 JALR 一起形成 EX redirect 请求；顶层按 `id_ex_valid` 授权并清除年轻槽。`pc_ctrl` 的译码字段经过 ID/EX，实际 redirect/flush 则在当前周期反馈控制寄存器更新。JAL 的 ID 请求和 ECALL/MRET 的 WB 请求具有各自的 valid 与优先级。

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
  ex_ctrl, mem_ctrl, wb_ctrl, csr_ctrl, sys_ctrl, pc_ctrl

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

这说明“顺向流水线骨架”已经完成，但还不能等同于“可正确执行任意程序的五级流水线”。刚完成骨架时，最小 `dummy` 曾因 `auipc/addi` RAW 把 Store 地址算成 `0x00000008`；该问题后来通过 EX forwarding、WB→ID bypass 和 load-use stall 解决。后续 D3/D4 已补齐当前 EX 分支方案的 redirect/flush、CSR 冒险及指定中断边界，仿真也已迁移到提交级。D5 固定时延的指定验收现已闭环；E 首版现已补齐单笔等待与副作用许可，当前缺口是完整精确异常、总线/SoC 与尚未覆盖的验证组合。

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
    mem_wb_wb_ctrl[`WB_CTRL_REG_WRITE];
```

为什么不能继续使用 ID 的 `wb_ctrl/rd_addr`？因为某个周期中 ID 和 WB 同时存在两条不同指令。若写使能来自 ID、写地址来自 WB，或者写地址来自 ID、写数据来自 WB，就会把指令 B 的控制和指令 X 的结果拼在一起，破坏体系结构状态。

REGFILE 模块本身不一定需要接收整条 `wb_ctrl`。当前做法是在 `core_top` 从 MEM/WB 控制包拆出并用 `mem_wb_valid` 门控，再把最终写脉冲传给 REGFILE，这种职责划分是合理的。

当前 REGFILE 数据冒险处理状态：

```text
已实现并定向验收：
  EX/MEM、MEM/WB -> EX forwarding；WB -> ID 同周期 bypass
  Store 地址与写数据复用 EX forwarding
  Load -> EX 消费者一拍 stall
  Branch/JALR 在 EX 使用 forwarding 后判定
  CSR 旧值 -> GPR 的 EX/MEM、MEM/WB 旁路
```

旧的 Branch-in-ID 方案及其 ID forwarding/interlock 待办已由 EX 判定方案取代，详见 D3 验收记录。

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

相邻 CSR 指令访问同一地址时存在状态写后读冒险；当前 D4.2 已在 ID 检测 ID/EX、EX/MEM 中的较老 CSR 写，并用 stall/bubble 等待 WB 提交。

#### SYS_CTRL：ECALL/MRET 在 WB 查询目标并提交

`sys_ctrl` 包含 `trap_enter`、`mret`、`is_ebreak` 和 `trap_code`。ID 负责译码，系统指令并不在 ID 修改 PC：

```text
ID.sys_ctrl -> ID/EX -> EX/MEM -> MEM/WB
                             -> core_top.wb_sys_redirect_fire（需 mem_wb_valid）
                             -> CSR.sys_commit_ctrl / trap_target
                             -> IF.redirect_target / redirect_fire
```

ECALL 在 WB 组合读取 `mtvec`，MRET 在 WB 组合读取 `mepc`。同一周期末，IF PC 接收目标；ECALL 保存自身 PC 到 `mepc` 并更新 `mcause/mstatus`，MRET 恢复 `mstatus`。EBREAK 由 WB `mem_wb_valid` 授权后触发 HALT，不产生这条系统 redirect。较老 CSR 写若在前一周期 WB 提交，系统指令本周期会读取更新后的 CSR 值。

WB 系统 redirect 清除全部年轻流水线槽的下一拍 `valid`，并门控年轻 MEM Store。这保证系统指令在 WB 修改 CSR 状态后，处理程序或返回目标才进入取指路径。当前 CSR 模块只接收 `mem_wb_sys_ctrl` 作为系统提交控制；旧方案中的 `sys_redirect_ctrl = ID.sys_ctrl` 端口已不存在。时序及验收见 D4.3 和 [D4 验收记录](d4-acceptance.md)。

#### VALID：载荷存在不代表指令有效

级间寄存器在 `valid=0` 时可以保留旧的 pc、inst 和控制包。真正决定副作用能否发生的是对应阶段的 `valid`：

```text
MEM访存   -> ex_mem_valid && !mem_wb_flush
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

当前 JAL 的 ID redirect 由 `if_id_valid && !id_stall_req` 授权；Branch/JALR 的 EX redirect 由 `id_ex_valid` 授权；ECALL/MRET 的 WB redirect 由 `mem_wb_valid` 授权。无效槽中残留的编码或控制包不能单独触发跳转。

#### 控制冒险：按 redirect 所在阶段清除年轻指令

| redirect 来源 | 决策阶段 | 同周期末清除的年轻槽 |
|---|---|---|
| JAL | ID | IF/ID |
| Branch/JALR | EX | IF/ID、ID/EX |
| ECALL/MRET、已接受中断 | WB/提交边界 | IF/ID、ID/EX、EX/MEM、MEM/WB 的下一拍 valid；同时门控年轻 MEM 副作用 |

顶层 `redirect_target` 的优先级为 WB system/IRQ、EX Branch/JALR、ID JAL；`redirect_fire` 使 IF 在时钟沿更新 PC，即使 ID 正因数据冒险 stall。这里的 kill 通过清除流水线寄存器的 `valid` 实现，数据载荷本身可以保留旧值。

#### HALT：ebreak 必须在 WB 提交

单周期版本可以在 ID 识别 `ebreak` 后立即停止；流水线中这样会让更老的 EX/MEM/WB 指令来不及完成，并且记录到的是 IF 的 PC。

当前已经改为：

```text
is_ebreak_raw = mem_wb_sys_ctrl.IS_EBREAK
is_ebreak_eff = mem_wb_valid && is_ebreak_raw
halt.pc       = mem_wb_pc
halt.ret      = regfile.x10
```

`halt.v` 本身只需要在 `is_ebreak_eff` 有效时锁存 `halt_pc/halt_ret`，无需接收整条 `sys_ctrl`。核心修改是保证它的输入都来自同一条 WB 指令。

#### 仿真环境：已从 cycle-level 迁移到 commit-level

五级流水线打破了“一个周期完成一条指令”的假设。复位填充、bubble、stall 和 flush 周期都可能没有指令提交。当前已通过 DPI-C 导出：

```text
commit_valid = mem_wb_valid && !rst
commit_pc    = mem_wb_pc
commit_inst  = mem_wb_inst
irq_taken    = irq_take
irq_cause    = interrupt_cause
wb_is_mmio   = mem_wb_valid && !rst && mem_wb_is_mmio
```

实际连接与采样：

```text
core_top 上升沿调用 dpi_wb_event，记录该沿 WB/IRQ 事件
  -> csrc/monitor/cycle_event.cpp 缓存为 CycleEvent
  -> main.cpp::single_cycle 在 eval 后取出事件
  -> 使用写回后的 GPR/CSR 调用 difftest_step
```

`single_cycle()` 每拍清空缓存、检查中断请求、驱动上升沿、处理 MMIO 接受事件，再执行 Difftest/提交观察，最后驱动下降沿。只有 `event.mem_wb_valid || event.irq_taken` 才进入 Difftest；普通等待周期不推进 REF。

| WB 有效 | MMIO | 接受 IRQ | REF 的处理顺序 |
| --- | --- | --- | --- |
| 1 | 0 | 0 | 执行一条普通指令，再比较 |
| 1 | 1 | 0 | 同步该条 MMIO 指令的结果，再比较 |
| 1 | 0 | 1 | 执行普通指令 → 注入 IRQ → 比较 |
| 1 | 1 | 1 | 同步 MMIO 结果 → 注入 IRQ → 比较 |
| 0 | 无有效 WB 身份 | 1 | 不执行普通指令，直接注入 IRQ 后比较 |

当前 `dut.cpp` 在 WB 有效时先检查提交 PC 与 REF 执行前 PC；普通指令调用 `ref_difftest_exec(1)`。MMIO Load 仅把 DUT 已写回的 rd 值同步给 REF，MMIO Store 不执行 REF 设备访问，两者把 REF PC 前进 4；随后仍比较其他 GPR/CSR。若同拍接受 IRQ，必须在上述 WB 处理之后调用 `ref_difftest_raise_intr()`。

已完成的配套迁移：

- REF 初始化时同步完整 RAM 初态，避免镜像之外的栈/堆初值不一致。
- ITRACE 使用 `event.wb_pc/wb_inst`；`cmd_si` 累计有效 WB 提交，bubble 和仅 IRQ 不计为一条指令；watchpoint 在有效 WB 后检查。
- `cycle` 在 `single_cycle()` 尾部递增。`idx` 是发生 WB/IRQ 时的 Difftest 调用次数，同拍可包含二者，不等于 retired instruction 数。
- `CONFIG_DIFFTEST` 与 `CONFIG_DIFF_LOG_ENABLE` 分离，关闭日志仍执行对比；`--log-limit` 限制常规文件记录，失配仍保留错误路径。

比较边界：当前显式读取 32 个 GPR 和 8 个 CSR。普通指令的后继控制流主要通过下一条提交 PC 对齐检查；不能用 IF PC 代表每条指令的架构下一 PC。调试表达式 `$pc` 当前仍读取 IF PC。计数器 CSR/时间源、访存副作用、设备数据与无提交死锁需要各自的合同和专项检查。

#### 外部中断：顶层接受、提交边界与请求保持

`interrupt_valid` 是输入请求，`irq_take` 才是本拍真正接受。顶层在 WB 系统指令、WB CSR 写或 WB EBREAK 存在时延后中断；接受时允许当前 WB 中更老的普通指令完成，清理 EX/MEM 及更年轻槽，`mepc` 保存最老被 kill 指令的 PC。CSR 模块收到的是 `irq_take`，不是原始 `interrupt_valid`。

当前仿真器从 `mip/mie/mstatus` 每周期重新计算请求；若以后接入只持续一拍的原始 SoC 中断脉冲，需要先在中断控制器或 pending 位保存事件，再用已提交的 CSR 使能重新判断，不应简单锁存已经按旧使能判定的请求。接口合同、时序例子及验收清单见 D4.3 的“中断请求保持问题”；这项 SoC 接口工作不阻塞当前 D4 的 CSR/System 定向验收结论。

当前 timer 仿真与 REF 已接通：

```text
宿主 ITIMER_VIRTUAL -> alarm_fired
  -> interrupt_check 置 DUT mip.MTIP，并同步 REF pending
  -> isa_query_intr 根据 MIE/MTIE/pending 形成请求
  -> RTL 顶层产生 irq_take，CSR 更新 mepc/mcause/mstatus 并 redirect/kill
  -> dpi_wb_event 把实际接受事件交给 C++
  -> 处理更老 WB 后向 REF 注入同一个 IRQ
```

`monitor_init` 在 `CONFIG_HAS_TIMER` 下启动 timer，带 Difftest 运行时也保留该来源；以前临时禁用 timer 的做法已不代表当前源码。RT 验收包含受控与宿主 timer 两种来源。仍未接入真正的设备 MEIP 来源、交互 UART 接收中断或可综合 RTL timer；宿主约 60 Hz 用户态 CPU 时间 timer 与 RT tick 配置的时间尺度也仍需另行统一。

#### 骨架阶段检查结论（2026-07-24 历史快照）

> 以下通过与未完成项是骨架阶段当时的记录；当前状态见阶段 D 正确性待办和 D3/D4 验收记录。

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

因此骨架阶段当时的状态应描述为（当前状态见 0.1）：

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
| JAL/JALR 的 `pc+4`，`wdata_src=10` | 指令自身已有 `pc_plus4` | EX/MEM 数据选择分支、MEM/WB 最终写回路径已有；旁路分支的专项覆盖不能由源码存在代替 |
| CSR 旧值，`wdata_src=11` | CSR 在 EX 组合读出 | EX/MEM→EX、MEM/WB→EX、WB→ID 已有指定场景验收，见 D4.1 |

| 消费位置 | 典型用途 | 当前处理状态 |
|------|------|------|
| ID | REGFILE 读出并锁存；JAL 使用 PC/立即数 | WB→ID 同沿 bypass 已实现；当前不在 ID 比较 Branch 或计算 JALR 寄存器目标 |
| EX | ALU、Load/Store 地址/数据修正、CSR 源、Branch/JALR | 共用 EX forwarding；Load 未就绪时由当前一拍 interlock 等待 |
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

ex_mem_forwarding_sel =
    (ex_mem_wdata_src == 2'b00) ||  // ALU
    (ex_mem_wdata_src == 2'b10) ||  // PC+4
    (ex_mem_wdata_src == 2'b11);    // CSR旧值；Load 01 不在此转发

ex_mem_can_forwading =
    ex_mem_reg_write && ex_mem_forwarding_sel;

ex_mem_forwarding_data =
    (ex_mem_wdata_src == 2'b00) ? ex_mem_alu_result :
    (ex_mem_wdata_src == 2'b10) ? ex_mem_pc_plus4   :
    (ex_mem_wdata_src == 2'b11) ? ex_mem_csr_rdata  : 32'b0;

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

rs1_real = rs1_from_ex   ? ex_mem_forwarding_data :
           rs1_from_mem ? wb_rd_wdata             : rs1_rdata;

rs2_real = rs2_from_ex   ? ex_mem_forwarding_data :
           rs2_from_mem ? wb_rd_wdata             : rs2_rdata;
```

| 条件 | 为什么必须存在 |
|------|------|
| `valid` | bubble 中可能残留地址和控制位，不能成为生产者 |
| `reg_write` | 只有真正写 GPR 的指令才能产生依赖 |
| `rd != x0` | x0 永远为零，不能被 forwarding 覆盖 |
| `rd == rs1/rs2` | 判断生产者和消费者是否访问同一个寄存器 |
| EX/MEM 的 `wdata_src` 属于 00/10/11 | 只允许已可用的 ALU、PC+4、CSR 旧值，防止把 Load 的地址误当作加载数据 |
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
load_use_hazard =
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
  Load 在 EX，消费者在 ID，load_use_hazard=1

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

这些信号当前用于 load-use 检测和 WB→ID bypass。当前 Branch/JALR 在 EX 使用修正后的操作数；若以后研究 Branch-in-ID，应作为新的设计变更单独定义其 interlock 与验收范围。

### D.2.7 已完成设计中的关键纠偏

| 设计过程中出现的疑问 | 正确认识 | 最终代码位置 |
|------|------|------|
| Store 的 rs2 是地址 | rs1 是基地址，rs2 是写入数据 | `ex_stage`、`mem_stage` |
| forwarding 全部放在 ID/EX 输入 | 紧邻和间隔一条统一在 EX 选择；只有同沿 WB→ID 在 ID 输入旁路 | `ex_stage`、`core_top` |
| `wb_rd_wdata` 已经晚一拍 | WB 是 MEM/WB 后的组合选择，与 MEM/WB 属于同周期 | `wb_stage` |
| Store 需要 MEM 阶段再做一套 forwarding | EX 已生成 `rs2_real`，通过 `ex_rs2_value` 随 Store 传到 MEM | `ex_stage`、`ex_mem_reg` |
| EX/MEM 的 `reg_write` 足以证明数据可转发 | 还必须检查数据来源；Load 的 ALU 结果只是地址 | `ex_mem_wdata_src` |
| Load 可以直接 EX/MEM→EX | EX/MEM 中没有最终 Load 数据 | `load_use_hazard` |
| 任意 rs 位段相等都应 stall | 必须结合 `id_uses_rs1/id_uses_rs2` | `id_stage` |

### D.2.8 当前已完成范围

```text
已完成：
  ALU -> ALU/地址/CSR源：紧邻、间隔一条、间隔两条
  ALU -> Store 地址和写数据：紧邻、间隔一条、间隔两条
  Load -> 普通 EX 消费者：立即依赖的一拍 stall + forwarding
  Load -> Store 地址/写数据：立即依赖的一拍 stall + forwarding
  x0 排除、valid 门控、EX/MEM 最新值优先、假 stall 分类

其他章节或后续范围：
  Branch/JALR 当前在 EX 消费数据；redirect/flush 已在 D3 定向验收
  CSR 结果写 GPR、CSR 状态写后读已在 D4 定向验收
  Branch-in-ID 是已替代的历史方案，不是当前必做待办
  D5 固定时延共享端口仲裁与 MMIO/控制碰撞指定专项已完成验收
  E 首版可变延迟已验收；AXI 反压仍待 F
```

---

## D.3 定向验收完成：控制冒险

> 当前状态：JAL 在 ID、Branch/JALR 在 EX、ECALL/MRET 在 WB 重定向；有效中断在顶层接受。D3 指定场景已定向验收，见 [D3 验收记录](d3-acceptance.md)。以下 D.3.1～D.3.3 的 Branch-in-ID 论证、失败现象和待办矩阵是设计过程中的历史方案，不代表当前 RTL。

当前实现路径：

```text
ID JAL 请求 + if_id_valid + !id_stall_req
  -> id_jal_redirect_fire
EX Branch/JALR 使用 rs1_real/rs2_real + id_ex_valid
  -> ex_branch_redirect_fire
WB ECALL/MRET + mem_wb_valid
  -> wb_sys_redirect_fire
输入中断请求经顶层仲裁
  -> irq_take
  -> 顶层统一 redirect_fire/redirect_target
  -> IF 更新 PC + 按来源清除年轻槽 valid
```

顶层优先级为 WB system/IRQ → EX Branch/JALR → ID JAL。CSR 写、系统指令及 EBREAK 对 IRQ 的具体仲裁见 D4.3。当前固定时延模型下的 flush 不能直接代表未来未完成总线请求已被撤销；等待与在途请求属于 E 及后续总线验收。

以下保留历史 Branch-in-ID 的两个问题：

| 问题 | 本质 | 需要的机制 |
|------|------|------|
| 分支/JALR 操作数数据冒险 | ID 当场使用 rs1/rs2，但新值仍在 ID/EX 或 EX/MEM | ID forwarding + interlock/stall |
| redirect 后错误路径冒险 | redirect 时顺序 PC 指令已经在 IF | IF/ID flush |

### D.3.1 历史方案：Branch/JALR 的 ID 数据冒险

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

### D.3.2 历史方案：redirect 与 IF/ID flush

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

### D.3.3 历史方案：Branch-in-ID 待实现矩阵

| ID 消费者依赖的生产者位置 | ALU结果 | Load结果 | 第一版处理 |
|------|------|------|------|
| ID/EX | 当前 EX 正在产生 | 尚未访存 | stall |
| EX/MEM | 可用 | MEM 周期才产生 | ALU→ID forwarding；Load 继续 stall |
| MEM/WB | `wb_rd_wdata` 可用 | `wb_rd_wdata` 可用 | 复用当前 WB→ID bypass |

---

## D.4 定向验收完成：CSR 数据冒险

> D4 指定定向场景已验收：D4.1 六项、D4.2 七项、D4.3 CSR/System 邻接及中断边界，见 [D4 验收记录](d4-acceptance.md)。2026-10-04 的 D3 记录还补充了当前 DPI 接口下五个测试台的复验。后续 CoreMark/RT 已使用提交级 Difftest，RT 有窗口内原生波形资格；这些结果不等于所有 CSR 地址、随机回归或完整特权 ISA 已覆盖。

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

消费者进入 EX 时，生产者位于 EX/MEM。最终 GPR 结果位于 EX/MEM 中的 CSR 旧值。此前 EX/MEM forwarding 只允许 ALU 结果；当前实现已加入 CSR 旧值来源，上述紧邻场景的定向测试通过。

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

当前采用方案 A，使 forwarding 网络按“最终写回来源”统一扩展；Load 仍需按 load-use 规则等待。

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
beq   x1, x0, target     # 当前 Branch 在 EX 消费；CSR→Branch 专项未单独验收
```

### D.4.2 CSR 状态写后读 RAW

> 2026-09-24 当前实现：core_top.v 已加入同地址 CSR RAW interlock，复用 ID stall/bubble；定向验收见 d4-acceptance.md。

> 2026-09-25 补充：CSRRW/CSRRWI 若 rd=x0，则不读取 CSR，但仍写 CSR；只有它们作为年轻指令时无需因旧 CSR 值等待。CSRRS/CSRRC 即使 rd=x0 仍需按其读改写语义检查。

```asm
csrrw x0, mstatus, x2
csrrs x3, mstatus, x0
```

若没有 interlock，第一条在 EX 计算 `csr_wdata`，但要到 WB 才真正修改 `csr_mstatus`；第二条下一周期就在 EX 组合读取 `csr_mstatus`，因此读到旧状态。

```text
老 CSR 写：ID/EX -> EX/MEM -> MEM/WB -> WB 时序提交
新 CSR 读：                    ID/EX -> EX 组合读取
```

当前实现使用 interlock：

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
csrrw x0, mscratch, x2
csrrc x3, mscratch, x4
```

### D.4.3 CSR 与系统指令之间的依赖

CSR 写在 WB 周期末的上升沿更新状态；ECALL/MRET 的 `sys_ctrl` 从 ID 经 ID/EX、EX/MEM、MEM/WB 传到 WB。顶层仅在 `mem_wb_valid` 有效时产生 `wb_sys_redirect_fire`；CSR 模块在该 WB 周期组合读取 `mtvec/mepc` 作为 `trap_target`。信号路径见 `core_top.v` 的 `wb_sys_redirect_fire`、`csr u_csr`，以及 `csr.v` 的 `trap_target`。

| 紧邻指令 | WB 周期 N | 周期 N 末 | 周期 N+1 |
|---|---|---|---|
| 写 `mtvec` → ECALL | 老 CSR 在 WB，ECALL 在 MEM | `mtvec` 更新，ECALL 进入 WB | ECALL 读取新 `mtvec` 并跳转 |
| 写 `mepc` → MRET | 老 CSR 在 WB，MRET 在 MEM | `mepc` 更新，MRET 进入 WB | MRET 读取新 `mepc` 并返回 |
| 写 `mstatus` → MRET | 老 CSR 在 WB，MRET 在 MEM | `mstatus` 更新 | MRET 用新状态恢复 `mstatus` |

因此，**当前 WB 查询方案不需要把 ECALL/MRET 加入 ID 阶段的 CSR 同地址 interlock**。普通年轻 CSR 指令在 EX 读旧值，仍由 D4.2 的 ID stall/bubble 处理；两类冒险的消费时点不同。

中断来自外部，不是一条排在 CSR 指令后面的流水线指令。当前顶层仅当 `interrupt_valid && !rst && !wb_sys_redirect_fire && !wb_csr_write_fire && !is_ebreak_eff` 时产生 `irq_take`：

- 老 CSR 写处于 WB：本拍先提交写入，不接受中断；若请求继续有效，下拍按更新后的 CSR 状态重新决定中断。
- CSR 写还在 MEM/EX 等年轻槽：若本拍接受中断，该未提交指令被 kill；`mepc` 保存最老被 kill 指令的 PC，MRET 后从该 PC 重执行。
- 接受中断或 WB 系统指令 redirect 时，IF/ID、ID/EX、EX/MEM、MEM/WB 的下一拍 `valid` 被清除；年轻 MEM Store 的副作用也被门控。当前 WB 中较老普通指令仍可提交。

`tests/system_interrupt_tb.sv` 已覆盖三条紧邻 CSR→系统指令、WB CSR 写与持续中断相撞、IRQ kill 年轻 CSR 后经 MRET 重执行；命令及结果见 [D4 验收记录](d4-acceptance.md)。这属于指定场景的仿真验收，不等于全部 CSR 地址、所有中断时序或完整特权架构已验证。

#### 与 CSR 冒险分开的中断请求保持问题

当前 `core_top.v` 没有中断 pending/ack 寄存器。若外部只给一个周期的 `interrupt_valid`，而该周期因 WB CSR 写被拒绝，则下周期 `interrupt_valid=0`，`irq_take` 不会再发生。这是**输入接口的请求保持问题**，不是“ECALL/MRET 读旧 CSR”的 D4 冒险。

当前仿真入口 `csrc/monitor/interrupt.cpp` 每周期根据 `mstatus.MIE`、`mip` 与 `mie` 重新计算 `interrupt_valid`；定时器事件先置位 `mip.MTIP`。所以保持 pending 的事件在 CSR 写提交后会重新判断：若写 `mtvec`，下一拍可用新入口；若写 `mstatus/mie` 关闭使能，下一拍不应错误地接受原请求。

接入 SoC 时，建议明确采用下列接口合同：

1. 原始设备脉冲先进入中断控制器或 `mip` 等待处理位；按来源分别保存 pending，不能只保存一个已仲裁的 `interrupt_cause`。
2. 使用**已提交**的 `mstatus/mie` 与 pending 状态形成对核的有效中断请求；在核给出 `irq_take` 前，保持请求及选定 cause 稳定，或在被拒绝后重新仲裁。接入控制器时需将顶层内部的 `irq_take` 明确导出为接受/应答事件。
3. WB CSR 写、ECALL/MRET、EBREAK 仍按当前顶层优先级处理；被拒绝的请求留在 pending。接受时再确认/清理事件，具体清理规则需区分脉冲与电平源。
4. 仿真侧应观察实际 `irq_take`，不能把输入的 `interrupt_valid` 直接当作已接受事件。当前已由 `dpi_wb_event` 导出并在 `event.irq_taken` 上处理；`main.cpp` 注释中的旧单周期 `is_trap = top->interrupt_valid` 已不在执行路径。

不能简单地在 `core_top` 锁存一次已经按旧 `mstatus/mie` 判定的 `interrupt_valid`：同拍 WB 可能关闭中断使能，锁存后的请求若不重新判断，就可能在下一拍错误地被接受。

后续接口验收至少覆盖：单拍原始脉冲撞上 WB CSR 写后仍 pending；WB 写 `mtvec` 后使用新入口；WB 写 `mstatus/mie` 关闭使能后不误接收；ECALL/MRET 优先级、MRET 恢复 PC，以及同一事件不重复响应。当前 RTL 尚未实现上述 SoC 原始脉冲接口，不能把这些待测项记为 D4 已通过的结果。

---

## D.5 定向验收完成：统一存储端口结构冒险

> 状态：2026-10-11 **D5 在当前固定时延合同与指定场景内完成验收闭环**。基础实现见 [原验收记录](d5-acceptance.md)；MMIO 消费/kill、控制碰撞和证据资格见 [专项闭环记录](d5-closure-acceptance.md)。15 个场景通过生产 Difftest，原生波形与事件核对通过，生产源码未改动。这是 D5 当时状态；2026-10-18 已完成 E 首版，以下 D5 接线描述作为历史基线。

### D.5.1 改造前的 DPI 模型与当前模块边界

改造前 IF 和 MEM 分别调用 `dpi_mem_read()`。虽然底层使用同一片软件内存，RTL 接口允许同周期发起两次组合调用，相当于提供了两个逻辑访问端口。

因此改造前不存在“两个阶段抢一个物理端口但无法同时完成”的结构冒险，更准确的说法是：**旧模型没有施加单端口资源约束。**

当前 `if_stage` 保存 PC 并接收指令数据；`mem_stage` 保留读写控制拆包、MMIO 地址查询与 LSU；顶层提供请求条件并消费授权；`mem_port` 统一完成 MEM 优先仲裁、地址选择、DPI 读写和读结果分配。DPI 读写调用只位于 `mem_port.v`，原 `mem_prot.v` 已统一命名。没有增加流水线寄存器或状态机：读为组合逻辑，Store 在上升沿执行；仿真器一次周期内可多次求值组合读，不能把原始 DPI 调用次数当作事务数。

`if_req = !rst && !id_stall_req && !redirect_fire` 申请读取当前 PC。redirect 本拍允许独立更新 PC、丢弃旧取指，目标 PC 生效后再申请取指。`pc_enable = redirect_fire || if_grant`；IF/ID 的优先级为 flush → ID stall 保留 → 按 `if_grant` 接收有效新指令。MEM 未访问时的非访存指令仍正常进入 MEM/WB，不能用 `mem_grant` 代替 MEM 指令有效性。

### D.5.2 接入共享单端口资源后的冲突

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

是否共享端口取决于实际存储组织和 bridge；使用 AXI 本身不意味着 IF/MEM 必须共享唯一端口。D5 为学习和验收结构冒险，先显式建立一个共享资源约束。

第一版设计与后续扩展边界：

| 情况 | 处理要求 | 阶段 |
| --- | --- | --- |
| MEM 无请求 | IF 在有接收空间时可获得取指许可 | D5 |
| IF/MEM 同时请求 | 第一版 MEM 优先；IF 未取得指令时不得伪造新的 IF/ID 有效指令 | D5 |
| ID 已消费旧 IF/ID，但本拍无新取指 | 下一拍 IF/ID 应为空；不能保持旧 valid 导致同一条指令再次进入 ID/EX | D5 |
| ID 不能接收旧 IF/ID | 保持原槽位和载荷，PC 更新/redirect 另按合同决定 | D5 |
| 请求未 ready | 保持已呈现请求的字段稳定，按约定处理取消；相关指令不得提前完成 | E |
| 响应未 valid | 保留在途指令与响应归属；上游按空间反压，较老 WB 可以排空 | E |

### D.5.3 实施与验收清单

- [x] 定义固定时延共享逻辑访问端口、IF/MEM 请求与访问许可；DPI 暂留在仿真后端。
- [x] 固定时延下定义槽位保留、消费、新接收与空槽；新测试检查未授权时 PC 保持和 IF/ID 无效。
- [x] 仲裁结果控制取指有效性及实际访存；唯一 DPI 读写位置有静态检查，动态检查授权互斥及 MEM 优先。
- [x] 指定 RAM 程序覆盖 Load/Store、Load-use、CSR stall、taken branch，以及 6 次 IF/MEM 请求争用；这些场景均出现不等于它们的所有同拍组合均已覆盖。
- [x] 独立检查该程序的 15 条 WB PC/inst 顺序、4 次 Store 与最终字节掩码结果；错误路径 Store 不执行，无额外提交。
- [x] 接入共享端口后重跑 D3/D4 五组定向测试；`load-store`、`unalign`、`hello-str` 的集成 Difftest 通过。详见验收记录中的原始日志与版本边界。
- [x] 当前小程序有 100 周期完成上限，运行脚本有 30 秒超时并拒绝 fatal/error 日志；已保存源码哈希、工具版本、命令及结果。这只检查该程序的有界完成，不能外推为所有程序均无死锁。
- [x] 连续访存、Load-use/CSR stall 与 MEM 同拍、ID 保留、JAL/JALR/Branch 与较老 MEM 共存、ECALL/MRET/IRQ 与年轻 MEM 相撞均设置必需覆盖条件并命中。
- [x] 真实键盘后端的重复 peek、有效接受/消费、rd=x0 和 WB 数据对应通过；计数型 MMIO 设备的连续 Store 次数及最终读取值通过。
- [x] ECALL/MRET/IRQ kill 的年轻 MMIO Load/Store 无消费/写副作用；IRQ→MRET 后重执行仅访问一次。

### D.5.4 专项闭环结果与进入 E 的条件

以下固定时延小规模专项现已完成，流水线时延及生产源码保持原样，具体输入与证据见 [专项闭环记录](d5-closure-acceptance.md)：

1. **设备访问闭环**：以有消费行为的键盘队列为例，区分组合 peek、MEM 接受事件和 WB 提交。重复求值不消费；每次有效接受消费一次；kill 的访问不消费。串口 `hello-str` 通过尚不能代替这项检查。
2. **控制碰撞闭环**：已显式检查 ID stall 时旧 IF/ID 保留，MEM 争用但 ID 已消费时 IF/ID 置空；EX redirect 与较老有效 MEM 访问共存时分别执行各自许可；WB system/IRQ 时年轻 MEM 请求被取消，PC 仍可更新到 redirect 目标。当前 `if_req` 在 redirect 本拍为 0，不能把“redirect 本拍同时有 `if_req=1`”设为覆盖目标。
3. **证据闭环**：已归档实际命中、访问/消费/提交计数、有界完成、Difftest 与 VCD；三个行为故障反例及离线重复事件均被拒绝，观察开关前后 15 组原生波形一致。专项使用生产键盘/MMIO 后端，补足原 RAM-only 测试的范围。

以上三部分现已通过指定正例、负例及原生波形/观察核对，D5 可按此固定时延范围标为完成。下一步进入 E 的 req/resp 与等待设计；这不是 AXI、cache 或跨周期响应的完成证据。

D5 当时 `core_top` 的 `mem_wb_valid_sel = ex_mem_valid && !mem_wb_flush` 没有跨周期响应完成条件，`mem_port` 在获授权的 Store 上升沿调用 `dpi_mem_write`。固定时延下 MEM 总能获授权且流水线继续推进；后续 hold 若沿用这些条件，可能提前传递 Load 或重复 Store，进入 E 时必须修正。

---

### 阶段 D 正确性待办

```text
当前 RTL 与定向仿真已覆盖：
  [x] ALU/CSR 结果 -> EX forwarding，MEM/WB -> EX forwarding，WB -> ID bypass
  [x] Store 地址/数据 RAW 与普通 Load-use 一拍 stall
  [x] JAL(ID)、Branch/JALR(EX) redirect 与相应年轻槽 kill
  [x] ECALL/MRET(WB)、已接受中断的 redirect、年轻槽 kill 和 MEM 副作用门控
  [x] D4.1 CSR 旧值 -> GPR forwarding
  [x] D4.2 同地址 CSR 状态 RAW stall/bubble
  [x] D4.3 CSR -> ECALL/MRET 及 CSR/IRQ 边界定向场景

下一阶段与集成验收：
  [x] commit-level Difftest、MMIO WB 标记与实际 IRQ 接受事件接通
  [x] Difftest 文件日志可独立关闭，失配检测与退出仍有效
  [x] 指定 CoreMark 程序验收；RT 启动、delay/wakeup 和抢占验收
  [x] 键盘/MMIO 的结果捕获、消费次数与 kill 指定专项验收
  [ ] cpu-tests/MicroBench 用户反馈结果的独立归档或当前版本复验
  [x] D5 基础：独立固定时延共享端口、取指有效性与指定流水线推进场景
  [x] D5 扩展：MMIO 消费次数/kill 与指定争用组合专项验收
  [x] E 首版：同步 req/resp、可变延迟、跨级 backpressure 与在途响应处理（范围见 E 节）
  [ ] 精确异常、已接受 Store/MMIO 的执行许可/排空与错误恢复
  [ ] SoC 原始中断脉冲转 pending、资格判断与 irq_take 应答接口
  [ ] 随机指令/随机等待、覆盖率与统一回归
```

D4 的“完成”指 [D4 验收记录](d4-acceptance.md)列出的定向场景；不表示上面未勾选的集成任务已经通过。

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

上面是拟补齐的全流水线 trace 字段，不代表当前已有完整日志。当前已有提交事件，以及 RT 测试中的有界原生波形/CSV；后续 trace 可服务 fuzzing 和双发射/乱序调试。

### 阶段 E：同步访存接口

> 2026-10-18：E 首版已实现，单笔在途、五级反压、IF 作废响应排空与 D 副作用许可已接通。具体测试结果及版本绑定见 [E 验收记录](e-acceptance.md)。完整精确异常仍属于 X；AXI/SoC/Cache 尚未实现。

**学习入口：**[E 流水线逐拍导读](../design-process/e-pipeline-walkthrough.md)。先理解 WB→MEM→EX→ID 的容量传播，再看 IF 的响应接收。不要把请求、响应和指令提交当作同一事件。

#### E.0 实际实现与范围

| 位置 | 已实现的责任 |
| --- | --- |
| `vsrc/mem_if_defs.vh` | 34 位字节地址，kind IF/LOAD/STORE=0/1/2，size byte/half/word=0/1/2，Normal/Device 属性 |
| `vsrc/mem_port.v` | IDLE 仲裁（MEM 优先）→OFFER 保持请求→WAIT 路由响应；锁住 owner、地址、大小、写数据与掩码 |
| `vsrc/dpi_mem.v` | 独立仿真后端，IDLE 接收→EXEC 等待并执行一次 DPI→RESP 保持结果/错误；后续可替换 SRAM/AXI bridge |
| `vsrc/core_top.v` | `*_allowin`/`*_fire`、IF busy/live、MEM issued、EX 保留结果、年龄相关 flush、IRQ 安全边界 |
| 四组 `*_reg.v` | reset/flush/enable/hold，载荷与 valid 一起保持；WB 首版始终可接收 |
| `vsrc/ex_stage.v` | 最近未就绪 Load 阻止使用更老 WB 的匹配值；ID 配合等待真实 Load 结果 |
| `vsrc/mem_stage.v` | 访存译码、LSU 大小/字节 lane 与固定 NPC PMA 分类；不执行 DPI 访存 |
| `csrc/memory/memory.cpp`、`device/mmio.cpp` | 地址/类型/设备宽度检查，一次事务一次访问，错误返回，不重复求值消费设备 |
| WB/DPI/Difftest | 以实际 WB/IRQ 事件推进；等待周期无虚构退休，MMIO 消费与提交分开 |

当前约束：全局最多一笔在途，纯 M 模式（`req_priv=3`），WB 不反压。IF 使用预留的 IF/ID 槽作返回缓冲；没有新增第六流水级或独立多项请求队列。所有 D 请求保守地等 WB 空后才交给端口，不只限制 Store/MMIO。代价是较低吞吐，后续优化需要重新验证许可边界。

#### E.1 两层接受事件与端口合同

```text
IF/MEM 访问意图 ─ mem_port 锁存 ─ 保持型 req/resp ─ dpi_mem
                   ↑                     ↑
       if_req_fire / mem_req_fire     req_valid && req_ready
```

- 顶层 `if_req`/`mem_req` 是尚未锁定的访问意图，不是对后端已作出的不可撤回承诺。
- `if_req_fire`/`mem_req_fire`：mem_port 在 IDLE 选中并保存该请求；此后不重新仲裁。
- 后端 `req_valid && req_ready`：dpi_mem 接受已保存的请求。OFFER 中 VALID 和所有载荷保持，与当前流水线控制脱离。
- `resp_valid && resp_ready`：结果或错误被原 owner 接收；Store 也必须等响应。
- `mem_fire`：MEM 指令真正移交 WB；`wb_fire`：WB 真正提交。它们都不等于请求接受。

后端字段为 `req_addr[33:0]`、`req_kind[1:0]`、`req_size[1:0]`、`req_priv[1:0]`、`req_mem_attr`、`req_wdata[31:0]`、`req_wmask[3:0]`；响应为 `resp_rdata[31:0]`、`resp_err`。窄访问数据采用 32 位字的 byte lane，LSU 负责提取/扩展。

请求和响应被阻塞时保持；协调复位以外，每笔后端接受必须恰好一个响应。不支持同沿零延迟响应、多 ID、乱序返回或单边运行中复位。34 位高地址由 DPI 检查拒绝，不能截断成低地址别名。

#### E.2 推进、kill 与副作用

1. **WB** 每拍可提交一个旧槽，下一拍可换入新指令或变空。等待 MEM 不重复提交 WB。
2. **MEM** 非访存指令直接完成；Load/Store 等已发请求的成功响应。未完成时 EX/MEM 槽保持；WB 可先排空。
3. **EX** 下游能接才移交。第一次受阻时保存旁路解析后的 ALU、Store 数据、CSR 读写值及分支结果，生产者离开 WB 后不回退到旧值。
4. **ID** 无 RAW/CSR 冒险且 EX 可接才移交；阻塞传播到 IF/ID。
5. **IF** 有空位或旧 ID 本拍能离开时才发请求，预留接收位置；成功且仍存活的响应进入 IF/ID，PC 顺序推进。redirect 可独立更新 PC。
6. **旧 IF 响应**：redirect 把 `if_live` 清零，但不撤回已锁定请求；响应仍握手排空，既不填 ID，也不触发旧路径错误。连续 redirect 保留最新目标。
7. **D 访问**：首次锁定前等 WB 空，已锁定后保留该指令直到退休。`mem_issued` 和 WB 的 memory 标志使 IRQ 等到不可撤销操作退休之后；未发出的年轻访问可被更老 WB redirect 取消。

当前合法路径没有“已发出的年轻 D 请求被更老 WB kill”的窗口，这是保守许可政策的结果；不能用它声称一般投机 D 访问排空已实现。

#### E.3 E0～E8 完成记录

| 工作包 | 当前交付 | 验收入口 |
| --- | --- | --- |
| E0 | 保存用户未提交基线；复现首条提交 PC/inst 错配起点 | `build/e-20261018-baseline` |
| E1/E2 | 字段合同、锁定 owner、独立延迟后端、事务账本与负例 | `tests/run_e_protocol.sh` |
| E3/E4 | 四级 hold/flush、反压、Load 就绪、EX 快照和 CSR 等待 | `tests/run_e_pipeline.sh` |
| E5 | IF 预留容量、busy/live、OFFER/WAIT/同拍响应及连续 redirect | 同上，`e_redirect_tb.sv` |
| E6/E7 | 单次 WB、MMIO 宽度/消费、D 许可、IRQ 恢复 PC/MRET | `tests/run_e_device.sh`、pipeline、RT 测试 |
| E8 | 固定/随机响应延迟、正负例、35 个 cpu-tests、CoreMark、RT 与原生波形资格 | [验收记录](e-acceptance.md) |

原 D3/D4/D5 脚本含固定时延/旧接口假设，本次没有宣称它们在新 RTL 上通过；其关键语义由 E 的 pipeline/stages/redirect/control 和 RT 用例覆盖。历史 D5 记录保留为历史基线。

#### E.4 验收范围与未完成项

- 后端请求等待 1/3/17、响应延迟 1/5/31、响应反压及固定 seed 随机**响应**延迟；生产 WB 不反压。
- E-15 原拟“WB 被阻塞的 CSR 写”在本首版不适用；已检查真实 CSR 提交与同址依赖。将来增加 WB ready 时必须补此窗口，不能照搬当前放行公式。
- E 首版的历史边界：有效 IF/D 错误进入 `faulted` 停机。2026-11-14 当前 MEM 错误已接入精确 WB trap，指定场景通过；IF 错误仍停机。2026-11-16 RAM 异常 REF 已对齐并通过指定 Difftest；独立设备模型仍待完成，见 [REF 修复](../exception_interrupt/nemu-mem-exception-fix-acceptance.md)。
- RAM 保留旧模型的向下对齐行为，未对齐语义尚待 X 冻结。MMIO 必须自然对齐、完整落在已注册设备范围内，不能扩大窄访问；现有不带宽度的设备 read callback 仅接受对齐 word，窄读直接返回错误。无 callback 的映射支持精确 byte/half/word。
- NPC Device PMA 暂为 `[0xa0000000, 0xa2000000)`；注册设备仍由后端逐次核对。SoC 地址图、权限、外设中断控制器、复位域在 F/SoC 阶段完善。
- `dpi_mem` 是仿真后端，不是可综合 SRAM/AXI 控制器。总线错误精确处理、AXI 通道、Cache、MMU/Linux 不能由 E 的 PASS 外推。

实际命令均在 **ysyx-npc 容器内**：

```bash
source /workspace/ysyx-workbench/npc/container/env.sh
cd "$NPC_HOME"
bash tests/run_e_protocol.sh
bash tests/run_e_pipeline.sh
bash tests/run_e_device.sh
bash tests/run_e_regression.sh
bash tests/run_rtthread_delay.sh
```

脚本分别创建独立证据目录；程序回归复用已有 guest bin/REF 并记录摘要。测试前确认 DIFFTEST/TIMER 开启，逐条 DIFF_LOG、ITRACE 和全程 WAVE 关闭；专项脚本单独采集有界波形。

### 后续：精确异常与不可撤销副作用

> 状态：部分基础已有，完整机制未完成。ECALL/MRET/IRQ 的指定边界已验收，不能等同于任意异常/总线错误下的精确性。

2026-10-26 历史验收覆盖非法指令的一个明确 RV32 编码、正常访存响应及 M-only/Direct mtvec 下的现场保存、普通写回抑制、WB redirect/kill、IRQ 冲突、错误路径取消和软件恢复。真实 AM `trap.S → __am_irq_handle → handler` 的停止/恢复及默认 `panic` 出口均已运行；当时尚未包括 REF 独立处理，能力检查在 `mcause` 首次失配。细节见 [历史验收记录](x-illegal-acceptance.md)。2026-10-27 后续已补齐 NEMU 对未匹配编码的 trap，原编码/全零 AM Difftest 和 CPU/RT 回归通过，见 [当前 REF 闭环记录](nemu-illegal-difftest-acceptance.md)。完整异常/提交事件 ABI、访问错误和权限等范围仍待完成。

在同步访存与总线集成过程中，需要补齐：

- 扩大非法/不支持编码的译码覆盖，补齐 CSR 访问合法性、取指/数据访问错误，以及明确的未对齐访问策略。
- 异常有效、cause、故障地址和指令 PC 随动态指令传递，较老异常优先处理，故障指令不进行普通写回。
- Store/MMIO 的不可撤销许可边界；尚未呈现、已呈现未接受、已接受、已响应的请求分别定义 kill、排空与重执行规则，首次呈现 VALID 前完成许可。
- 中断不能把已经产生外部副作用的指令简单 kill 后重新执行；恢复 PC 必须与真实完成边界一致。
- 异常/IRQ、访存响应、redirect 同拍的定向测试；REF trap 对齐和独立副作用计数同步完善。

这部分应在可综合最小 SoC 验收前形成闭环；接口与异常设计有关联，可分步实现，但不能只完成错误响应传输就标为异常处理完成。

### 阶段 F：AXI4 bridge

> 状态：未实现。按当前详细路线，E/X 之后主存路径先做单 beat、单 outstanding、固定 ID 的 AXI4 bridge；AXI4-Lite 可用于 MMIO 适配，不再作为必须先做的一整套阶段。目标平台的宽度、地址/属性映射和复位域在 F 实施前冻结。

不要让 CPU 核内部一开始直接暴露完整 AXI。更推荐：

```text
core 内部：简单 req/resp
core 外部：bridge 转 AXI
```

推荐顺序：

```text
1. SRAM-like memory model
2. E/X 的内部 req/resp、不可撤销访问许可与错误合同验收
3. 单 beat、单 outstanding AXI4 bridge：AR/R 与 AW/W/B 独立握手
4. 最小 SoC 地址译码、默认错误从设备、RAM/UART/timer；按需适配 AXI4-Lite MMIO
5. H 的 cache refill 再扩展 burst，并检查宽度转换、MMIO bypass 与维护
6. 多 ID、多 outstanding 仅在后续需求明确时另行设计和验收
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

> 状态：未实现。当前 DPI RAM、serial/RTC/keyboard/VGA/audio 是仿真设备模型；可综合 ROM/RAM、UART、RTL timer/IRQ 与启动链仍待构建。

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

> 状态：cache、分支预测及完整性能计数体系未实现。当前已有宿主侧 cycle/事件计数和 CSR `mcycle`，不能据此宣称已有硬件 instret、cache 统计或可比的硬件性能分数。

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

> 状态：已有 NEMU 提交级对比、定向 IRQ 注入、失败附近提交记录与有界波形工具可复用；随机生成、覆盖反馈、失败最小化及批量回归闭环尚未建立。

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

### 阶段 J：顺序双发射与后续乱序研究

> 状态：未实现。近期第二条目标是带分支预测的顺序双发射并保持 Linux 回归；ROB/Tomasulo/重命名属于更长期的乱序研究，不是顺序双发射的必需前置。

共同基础完成后，近期推进顺序：

```text
单发射 SoC 基线
  -> 按固定固件/内核配置补齐特权、Sv32/MMU、原子操作与平台功能
  -> 单发射 Linux 运行用户态程序
  -> 分支预测，验证更新与误预测恢复
  -> 保守顺序双发射，再逐步放宽配对限制
```

双发射需要明确槽间 RAW/WAW、单 LSU/控制流限制、写回仲裁、顺序提交与精确异常，并同步扩展提交事件和 REF 推进数量。具体关口以 [目标路线](roadmap-soc-linux-dualissue-tapeout.md) 的 P3/P4/P5 为准；该路线文件早期写的 D4 待验收状态已由本文件 0.1 的当前状态更新。

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

以下保留更长期的乱序学习顺序，不作为近期顺序双发射的实施清单：

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

2026-10-18 更新：在原 D5 基线基础上完成 E 首版实现与新版本验收：

```text
阶段 A：单周期基线              ✅ 历史基线
阶段 B：控制包化单周期          ✅ 历史基线
阶段 C：五级流水线骨架          ✅ 四组级间寄存器与 valid
阶段 D2：普通数据冒险           ✅ 指定定向测试
阶段 D3：控制冒险/redirect      ✅ 指定定向测试
阶段 D4：CSR 冒险              ✅ 指定定向测试
提交级 Difftest / timer / MMIO   ✅ 已接通；范围与缺口见 0.1
CoreMark / RT 指定场景          ✅ 已有验收记录；证据可用性见 0.2
cpu-tests                      ✅ 本次 E 版本 35/35 生产 Difftest
MicroBench                     📝 保留此前用户反馈；本次 E 未重跑
阶段 D5：存储端口结构冒险       ✅ 固定时延基础与 MMIO/控制碰撞指定验收闭环
阶段 E：同步访存接口            ✅ 首版指定范围实现与验收闭环
完整精确异常                    ⏳ X 待完善；E 已有保守 D 许可与 IRQ 边界
阶段 F/G/H：总线、SoC、cache    ⏳ 未实现
随机验证 / 综合 / FPGA         ⏳ 待建立相应验收
Linux / 分支预测 / 顺序双发射   ⏳ 后续目标
```

当前定位是：**已形成单笔可变延迟访存的单发射五级核与提交级仿真基线，E 首版指定验收闭环；尚未形成完整精确异常及可综合 SoC 核。** RT 启动及指定调度用例通过增加了架构和功能证据，不能确定全部 RTL 正确，也不能外推到尚未实现的总线/cache 场景。

### 2.1 下一步执行顺序

| 顺序 | 工作 | 验收关口 |
| --- | --- | --- |
| 1. D5（指定验收完成） | 固定时延共享端口仲裁；正确消费/保留 IF/ID，控制实际访存许可 | 已完成指定争用、访问/消费/提交计数、有界进度、Difftest 与波形核对；后续作为回归 |
| 2. E（首版已完成） | 按 [逐拍导读](../design-process/e-pipeline-walkthrough.md) 学习握手、反压和许可；保留新回归入口 | [本次 E 记录](e-acceptance.md)：适用场景、负例、波形资格与程序回归 |
| 3. 精确异常与副作用 | 非法指令、访存错误、异常载荷和 Store/MMIO 不可撤销边界 | 更老异常/IRQ 与年轻请求相撞、恢复 PC 正确、无错误副作用 |
| 4. F/G 总线与最小 SoC | bridge、地址图、ROM/RAM、UART、RTL timer/IRQ、启动链 | 可综合顶层运行裸机与 RT，协议检查和必要 Difftest |
| 5. H Cache | 先 blocking I-cache，再简单 D-cache，MMIO bypass 与维护语义 | miss/refill、写入/回写、维护、flush/中断碰撞与已有程序回归 |
| 6. 综合/FPGA 基线 | 时钟约束、存储器 wrapper、启动与调试通道 | 面积/关键路径报告和板级运行证据 |

D5 提供访问意图和固定时延基线，E 新建完整内部接口合同；每步先做小程序与协议检查，再按改动风险回归 CoreMark/RT 等已有用例。新的等待或仲裁逻辑不能只靠长程序最后输出 PASS 验收。主存 bridge 计划采用 AXI4，平台参数在 F 实施前确认，core 内部协议先固定。

### 2.2 Difftest 基线的复用与冻结范围

已经可以把当前提交级框架作为后续 RTL 修改的回归基线。现有验收目录保存指定输入、配置和摘要，但尚未创建覆盖完整工作区的独立冻结快照或 Git tag；当前工作区仍含用户未提交修改，本次文档更新不执行提交或打 tag。

| 保留的合同 | D5/E 及后续适配要求 |
| --- | --- |
| 每次实际普通提交只让 REF 执行一次；无提交等待周期不执行普通指令 | WB 如可 hold，提交事件及状态写入需由实际提交 fire 授权，不能逐拍重复报告槽位 valid |
| WB 先于同拍 IRQ 处理；IRQ 按实际接受事件注入 | 已接受 Store/MMIO 出现后重新验证 IRQ 接受边界、排空和恢复 PC |
| MMIO 身份与动态指令一起到 WB，限定同步该指令结果 | 设备读取/消费边界按新握手适配，独立验证设备值、次数和 kill 行为 |
| 比较 GPR/已覆盖 CSR、提交 PC 对齐；失配进入 ABORT 并失败退出 | 新异常、特权、计数器等需扩展状态与 REF 合同；普通分支下一 PC 的检查边界要明确 |
| 日志配置与检查功能独立 | 限制全程日志/波形体积，失败保留必要取证，正常回归复用已具资格的日志 |

后续验收组合固定为：**提交级 Difftest + 本阶段协议/副作用检查 + 有界进度检查 + 必要旧用例回归**。源码、采样周期或事件合同变化时，对受影响的 DPI/probe 重新做原生 RTL 波形资格；不能把旧资格自动扩展到新 ready/valid 语义。

历史入口如下；E 当前入口及本次实际运行结果见 E 节和验收记录。旧固定时延测试不能直接当作新 RTL 回归通过：

| 范围 | 入口 | 记录 |
| --- | --- | --- |
| D3 与 CSR 定向 | `bash tests/run_d3.sh`、`bash tests/run_d4_forward.sh`、`bash tests/run_d4_state.sh` | [D3](d3-acceptance.md)、[D4](d4-acceptance.md) |
| 日志开关及失配负例 | `bash tests/run_diff_log.sh` | [日志验收](coremark-acceptance.md) |
| 生产 CoreMark + Difftest | `bash tests/run_coremark.sh` | [CoreMark](coremark-acceptance.md) |
| RT 启动与 timer 恢复 | `bash tests/run_rtthread.sh` | [RT 启动](rtthread-acceptance.md) |
| RT delay/抢占与启动回归 | `bash tests/run_rtthread_delay.sh` | [RT 延时与抢占](rtthread-delay-acceptance.md) |

本次 E 已获得用户直接修改授权并完成实现；后续讨论/教学与修改任务继续按用户当次授权执行。总线/cache 接入后继续保留该基线，并增加对应配置的独立验收记录。

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
> pc_sel 独立端口取消并入 pc_ctrl。
> 2026-10-07 补充：后续又改为 JAL(ID)、Branch/JALR(EX)、system(WB) 的当前方案；本章与末尾 C-Stage 章节均是历史推导，当前接线以阶段 D 和 0.1 为准。

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

> 本节为 B-Stage 单周期历史设计。当前五级核由顶层仲裁 `irq_take`；MEM 接收门控后的 `mem_stage_valid`，IF 接收统一 redirect，CSR 接收实际接受的 IRQ。不要按本节旧端口表恢复当前 RTL。

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

### 当前五级流水线状态索引

2026-10-07 已将当前状态统一到 [0.1 当前进展总表](#01-当前进展总表)，证据可用性见 [0.2](#02-本次补齐的记录与证据边界)，后续顺序和 Difftest 基线范围见 [第 2 节](#2-当前阶段定位)。本历史章不再维护第二份当前检查表，避免与正文状态冲突。

上方“控制包化单周期基线测试结果”仅属于加入流水线之前的历史基线，不能移作当前流水线的通过证据。


---

# C-Stage 前置：PC_CTRL 整包 + 分支/跳转解析前移 ID

> 历史状态：当时 RTL 重构与 core_top 接线完成，普通 EX forwarding、WB→ID bypass、load-use stall 已完成，Branch-in-ID、CSR 冒险和 flush 尚待实现。
> 日期：2026-07-22
> 前置：B-Stage 控制包化单周期核
>
> 当前说明（2026-10-07）：本章保留全 ID 判定的历史推导。当前 Branch/JALR 已移到 EX，pc_ctrl 经 ID/EX 传递，D3/D4 指定场景已验收；下方旧待办不属于当前必做清单。

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

以下是 2026-07-22 历史方案当时的验证待办；当前方案的定向验收与程序验收见 0.1 和独立记录，本次不改写历史勾选状态。

- [ ] `verilator --lint-only --top-module core_top -Ivsrc vsrc/*.v` 清零
- [ ] cpu-tests 全 PASS（纯重构，功能应逐周期等价，重点覆盖 branch/jal/jalr 用例）
