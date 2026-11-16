# 非法指令异常闭环验收计划

范围：现有 RV32 M-only、对齐 Direct mtvec、正常访存响应下的非法指令异常。保留 ECALL/MRET/EBREAK 现有兼容路径。本轮不修改生产 RTL；若出现 RTL 错误，保存最小触发、最早可观测偏差及修正意义，交由用户修改。

1. 记录当前 RTL/配置/输入摘要，检查 lint。
2. 新增原生 core 定向测试：现场保存、故障指令不普通退休、老指令完成、年轻 Store/GPR 被取消、错误路径非法指令不陷入、handler 显式修改 mepc 后 MRET、WB 异常与 IRQ 竞争。自然指令流难以覆盖的年轻阶段碰撞用测试台注入，单独标记。
3. 对本轮监测字段进行原生 VCD/日志采样核对，使用字段损坏和旁路许可突变作为负对照；资格只覆盖本轮采样合同及实际场景。
4. 构建独立 AM 非法指令客户程序，复用真实 trap.S/cte.c，验证 EVENT_ERROR/cause=2、现场检查和显式恢复。非法指令用例不声称已获 REF Difftest 支持。
5. 运行 35 项 CPU Difftest 回归，以及有界 RT-Thread 启动/命令/timer 检查。关闭全程 trace/diff 文件，保留有界诊断波形。
6. 检查生产 RTL 摘要未变，将真实结果、命令、输入、延迟和 seed 写入验收记录。保留同步异常 DPI 事件和 REF 支持的边界，不将普通回归通过称为完整精确异常通过。

本轮证据目录：`npc/build/x-illegal-ZpDVN0/`。所有编译和仿真只在现有 `ysyx-npc` 容器内执行。

执行结果以 [非法指令验收记录](../cpu-fuzzing/x-illegal-acceptance.md) 为准。生产 RTL 摘要未变；CPU/RT 的既有 Difftest 通过，非法指令 REF 能力检查失败已单独记录，未被普通状态同步掩盖。
