# E 阶段 Implementation Plan

**Goal:** 单笔同步访存、五级反压、一次性副作用和提交闭环。
**Spec:** ../cpu-fuzzing/cpu.md，E0～E8。
**Architecture:** IF 槽预留 IF/ID 容量；mem_port 锁存 owner 与请求；独立 dpi_mem 后端实现保持型 req/resp。WB 始终可接收，MEM 等返回，上游按 allowin 保留。所有 D 请求保守地等 WB 空后发出，IRQ 等 D 事务退休，先保证精确副作用。
**Tech Stack:** Verilog/SystemVerilog、DPI C++、Verilator、NEMU。

## 约束与裁定
- 按用户授权直接修改当前工作区；保存未提交基线，不建丢失用户修改的 checkout，不提交。
- 全部构建/仿真在 ysyx-npc；证据用独立目录。
- 内部字节地址 34 位，kind IF/LOAD/STORE=0/1/2，size byte/half/word=0/1/2，priv=M(3)，attr Normal/Device=0/1。
- E 保持旧 RAM 向下对齐行为；设备不允许越界/扩大访问，错误返回而不重试。有效错误有界 fail-stop，精确异常留 X，不能宣称完整异常支持。
- 首版所有 D 访问均在最老时授权，代价是保守性能；IRQ 直到其 WB 完成后才接收。普通 D 读不投机，因此无在途年轻 D 被 WB kill 的合法窗口。
- IF/ID 是 IF 返回缓冲：仅在有空位/本拍旧 ID 离开时发起 IF，预留空位直至响应；防止共享端口被 IF 响应反压堵住老 MEM。

## 任务和验收
- [x] E0：快照、工具/配置；运行当前代码最小提交序列，记录失败起点。
- [x] E1/E2：mem_if_defs、mem_port 锁定请求、dpi_mem 时钟延迟模型；req/resp 负例、固定/随机等待、错误和响应反压。
- [x] E3/E4：四级寄存器 enable/flush；WB→MEM→EX→ID allowin；Load 就绪、CSR 依赖、EX hold 保存计算结果；小程序提交和内存账本。
- [x] E5：IF busy/live 与预留容量，redirect 更新 PC、旧响应排空；OFFER/WAIT/同拍响应/连续 redirect 测试。
- [x] E6/E7：一次性 WB/IRQ、D 不可撤销许可、MMIO 精确宽度与消费；原生波形和 DPI 事件比对。
- [x] E8：定向/随机/负例与 cpu-tests、CoreMark、RT 按风险回归；真实覆盖、适用边界写验收记录，更新 cpu.md。

## Review Focus
- 最近 Load 遮挡旧 WB，EX 等待时旁路消失。
- 已呈现 IF 被 redirect 作废但通信不能撤回。
- Store/MMIO 直到退休不可 kill，IRQ 恢复点正确。
- Device byte lane 和端点越界，无重复消费。
- WB 不因 MEM 等待重复提交，错误不提前退休。

## 执行记录
- 已读取实际源码；现版本 PC 在请求边沿推进、MEM 无等待、reg 无 hold。基线保存在 build/e-20261018-baseline。

- 2026-10-18：实现与定向/负例、设备、程序回归结果见 `../cpu-fuzzing/e-acceptance.md`。最终产物清单与范围由该记录管理。
- 独立代码审阅发现 callback MMIO 窄读会扩大访问/触发键盘断言；已用真实键盘负例复现，再修为宽度不支持时错误返回。另补 IRQ 恢复 PC/MRET 和有效 IF/D 错误停止定向用例。
- E-15 的 stallable WB 变体不适用本版；X 精确异常、F AXI 与后续性能优化仍未完成。
