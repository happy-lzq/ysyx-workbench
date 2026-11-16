# D4 CSR 冒险验收记录

> 最新状态：D4.1 六项转发场景、D4.2 七项 CSR 状态场景、D4.3 指定的 CSR/System 邻接与中断边界场景、D3 三个测试台和顶层 lint 在当前工作区通过。D4.2 采用同地址 stall/bubble，并对 CSRRW/CSRRWI rd=x0 关闭无需的 CSR 读取。此前的 rd=x0 失败记录保留为缺陷证据。

## 2026-09-24：D4.1 CSR 旧值到 GPR 的 EX/MEM forwarding

基线提交：e6349ff；验收时工作区有未提交 RTL 和测试改动，因此本记录对应此工作区快照，而非单独的 Git 提交。环境：项目指定的 ysyx-npc 容器，Verilator 5.048。

### 修改与范围

- 测试台 redirect_req_tb.sv 补齐 ex_stage 新增的 ex_mem_csr_rdata 与 ex_mem_pc_plus4 输入及初始化。
- 新增 csr_forward_tb.sv 和 run_d4_forward.sh；从 core_top 加载真实指令，经 IF/ID/EX/MEM/WB 检查 GPR、CSR 和 Store 的最终结果。
- 本轮没有修改生产 RTL；EX/MEM 的 CSR、PC+4 旁路由用户此前实现。
- 测试台通过层级赋值初始化 GPR 和 mscratch，指令本身仍走整核流水线；该初始化方法不验证软件设置 CSR 的路径。

### 运行命令与观察

在项目根目录执行：

    docker exec -i --workdir /workspace/ysyx-workbench/npc ysyx-npc bash tests/run_d3.sh
    docker exec -i --workdir /workspace/ysyx-workbench/npc ysyx-npc bash tests/run_d4_forward.sh

结果：

- D3：redirect_req_tb、redirect_kill_tb、system_interrupt_tb 均 PASS。
- D4.1：CSR 旧值→紧邻 ADDI、CSR 旧值→紧邻 Store、EX/MEM CSR 与更老 MEM/WB 同名写入的优先级、相隔一条时 MEM/WB→EX、相隔两条时 WB→ID，五例均 PASS。后两例还检查了对应流水线位置与旁路命中。
- 用容器内临时 RTL 副本禁用 CSR EX/MEM 旁路后，第一例按预期失败：x5=0x23、x6=0x01、mscratch=0x55；正常 RTL 期待 x6=0x24。该检查确认测试能观察到缺失旁路。
- 顶层 Verilator lint 已通过；本轮未采集波形，未做 Difftest 或随机回归。

### 结论与剩余范围

当前工作区的 D4.1 指定定向场景通过。MEM/WB→EX 与 WB→ID 复用已有的 wb_rd_wdata 数据源与地址比较逻辑，本轮只新增验收测试，没有修改这两条 RTL 路径。此结论限于上面列出的指令序列和仿真配置；没有验证所有 CSR/GPR 组合。D4.2 同地址 CSR 写后读 interlock、D4.3 CSR/System 邻接边界仍待独立定向验收。PC+4 的 EX/MEM 数据分支虽然在 RTL 选择器中存在，本次 D4.1 测试没有专门激活并检查该分支。

## 2026-09-24：MEM/WB CSR 专用选择候选的失败记录

用户在 ex_stage.v 中新增了 mem_wb_csr_wdata 和 mem_wb_forwarding_data。为检查目标寄存器为 x0 的边界，在 csr_forward_tb.sv 增加 csrrw x0, mscratch, x2；相隔一条指令后执行 addi x6, x0, 1 的整核用例。

同一容器入口 tests/run_d4_forward.sh 的本次结果：此前五项继续通过，第六项失败；x6=0x24，预期 0x01，且 rs1_from_mem=1。顶层 Verilator lint 通过，因此问题是功能逻辑而非端口/语法错误。

首个相关逻辑偏差：MEM/WB 中 CSR 的 rd=x0，mem_wb_reg_write 因 rd=x0 为 0，但新增 mem_wb_csr_wdata 仍为 1；后续指令的 rs1=x0 与该 rd 匹配，于是从 MEM/WB 误转发 CSR 旧值 0x23。WB 的 wb_rd_wdata 已经选择 CSR 旧值，原有 mem_wb_reg_write 门控可覆盖正常 CSR rd≠x0，不需要第二条 CSR 专用旁路。当前候选在修正和重新运行六项测试之前不予验收。本轮没有修改生产 RTL。

## 2026-09-24：恢复原路径后的集成检查

源码检查确认 ex_stage.v 已恢复 mem_wb_reg_write 地址匹配和 wb_rd_wdata 选择；WB 的 wb_stage.v 在 CSR 来源 11 时选择 mem_wb_csr_rdata。相隔一条指令的 MEM/WB→EX 数据流按此设计已具备。

但 core_top.v 的 ex_stage 实例仍含 .mem_wb_csr_rdata(mem_wb_csr_rdata)，而当前 ex_stage 模块已无该输入。容器内执行 verilator --lint-only --top-module core_top -Ivsrc vsrc/*.v 返回 PINNOTFOUND，当前快照不可 elaboration。需清理顶层多余连接后，重跑 lint、D3 和六项 D4.1 定向测试，才能更新验收结论。本次只检查并更新笔记，未修改生产 RTL。

## 2026-09-24：D4.1 六项当前快照复验

源码检查确认 core_top.v 已移除多余的 ex_stage.mem_wb_csr_rdata 连接。使用同一 ysyx-npc 容器重新执行顶层 Verilator lint、tests/run_d3.sh 和 tests/run_d4_forward.sh：lint 通过；D3 的 redirect_req、redirect_kill、system_interrupt 三个测试台通过；D4.1 的紧邻 CSR→ALU、紧邻 CSR→Store、较新 EX/MEM CSR 优先、相隔一条 MEM/WB→EX、相隔两条 WB→ID、CSR rd=x0 不得转发六项均通过。

相隔两条用例在生产者位于 WB 且消费者位于 ID 时，检查 wb_id_rs1_hit 与 id_rs1_rdata，再检查 x6 最终值。此结果只覆盖定向场景；未进行随机回归、Difftest 或本次波形采集。本轮没有修改生产 RTL。

## 2026-09-24：D4.2 同地址 CSR 状态 RAW interlock

本次修改 core_top.v：ID 的 CSR 读地址与 ID/EX、EX/MEM 中有效 CSR 写地址比较。命中时将 csr_state_raw_hazard 并入 id_stall_req，复用 PC 冻结、IF/ID 保持和 ID/EX valid 清零形成 bubble。老写已经处于 MEM/WB 时不再停顿：该上升沿写 CSR，同时年轻指令进入 EX，之后的组合读看到新状态。CSR 模块、EX 数据旁路和流水线寄存器本体没有改动。

新增 tests/csr_state_tb.sv 与 tests/run_d4_state.sh。先运行测试观察到修改前失败：相邻同地址场景应停两拍，实际为零拍。修改后在 ysyx-npc 容器运行：

    source npc/container/npc.sh && npc bash tests/run_d4_state.sh
    source npc/container/npc.sh && npc bash tests/run_d4_forward.sh
    source npc/container/npc.sh && npc bash tests/run_d3.sh
    source npc/container/npc.sh && npc bash -c 'verilator --lint-only --top-module core_top -Ivsrc vsrc/*.v'

结果：csr_state_tb 的相邻写后读两拍 stall、隔一条写后读一拍 stall、CSRRS→CSRRC 读改写链、不同 CSR 地址不 stall 四项均 PASS；csr_forward_tb 六项、D3 三个测试台均 PASS；顶层 lint 退出码 0。相邻用例还检查 stall 时 IF/ID 指令与 PC 保持、ID/EX valid 清零。

测试使用层级赋值初始化 mscratch/mtvec 和源 GPR；没有覆盖所有 CSR 地址、CSR 与系统指令邻接、重定向同时遇到 CSR stall、随机回归、Difftest 或波形取证。上述通过仅对应当前工作区和列出的定向场景。

## 2026-09-25：CSRRW/CSRRWI rd=x0 无读等待验收

用户已在 control.v 中让 CSRRW/CSRRWI 的 csr_read 随 rd 是否为 x0 决定，同时保持 csr_write=1。本轮只扩展 tests/csr_state_tb.sv，不修改生产 RTL：相邻两条同地址 CSRRW 且年轻 rd=x0、年轻 CSRRWI rd=x0 均检查 ID 不产生 stall、两条指令连续进入后续流水级以及最终 CSR 值；年轻 CSRRW rd≠x0 检查两拍 stall、GPR 收到老写的新 CSR 值和年轻指令最终写入。原有“老 CSRRW rd=x0 → 年轻 CSRRS”用例仍检查老指令的 CSR 写不能被忽略。

在项目根目录执行：

    source npc/container/npc.sh && npc bash tests/run_d4_state.sh

结果：七项定向场景均 PASS。为确认两条无 stall 用例能捕获旧行为，仅在忽略版本控制的 npc/build/d4-csr-state-mutations 下分别编译临时 control.v 副本：单独把 CSRRW 的 csr_read 恢复为恒 1 时，第一条无 stall 用例因“unexpectedly stalled in ID”失败；单独恢复 CSRRWI 时，第二条无 stall 用例因相同原因失败。两个负例都没有改动工作区生产 RTL。

本次测试没有重新运行 D3、D4.1、Difftest、随机回归或采集波形；D3、D4.1 和顶层 lint 的当前快照复验记录见此前章节。结论限于上述指令序列及 CSR 实现。

## 2026-09-25：D4.3 CSR/System 邻接与中断边界定向验收

本轮仅扩展 tests/system_interrupt_tb.sv，不修改生产 RTL。真实指令从 core_top 取指并经过流水线；测试以层级赋值设置初始 GPR、旧 mtvec/mepc，随后检查 WB 时的目标及提交后的 PC/CSR 状态：

- 紧邻的 CSRRW mtvec → ECALL：ECALL 使用新 mtvec，保存 ECALL 自身 PC 到 mepc，年轻错误路径不提交。
- 紧邻的 CSRRW mepc → MRET：MRET 跳到新 mepc，年轻错误路径不提交。
- 紧邻的 CSRRW mstatus → MRET：MRET 按新 mstatus 恢复状态。
- IRQ 在年轻 CSRRW mtvec 位于 MEM 时被接受：目标仍为旧 mtvec；年轻 CSR 写被 kill，mepc 指向该指令，MRET 后重新执行并写入新 mtvec。
- 原有 WB CSRRW mtvec 与保持有效的 IRQ 相撞测试继续检查：老 CSR 写先提交，下一周期 IRQ 使用新 mtvec。

在项目根目录、使用现有 ysyx-npc 容器执行：

    docker exec -i --workdir /workspace/ysyx-workbench/npc ysyx-npc bash tests/run_d3.sh
    docker exec -i --workdir /workspace/ysyx-workbench/npc ysyx-npc bash tests/run_d4_forward.sh
    docker exec -i --workdir /workspace/ysyx-workbench/npc ysyx-npc bash tests/run_d4_state.sh
    docker exec -i --workdir /workspace/ysyx-workbench/npc ysyx-npc bash -lc 'verilator --lint-only --top-module core_top -Ivsrc vsrc/*.v'

结果：D3 三个测试台通过，其中 system_interrupt_tb 的四个新增场景及原有场景通过；D4.1 六项、D4.2 七项通过；顶层 lint 退出码 0。本次定向仿真未观察到 CSR/System 时序 bug，因此没有修改 RTL。

结论限于当前工作区快照与上述场景。外部 interrupt_valid 若只在被拒绝周期脉冲一次，RTL 没有 pending/ack 保存机制；已有相撞测试的请求会保持到被接受。本次没有验证所有 CSR 地址、所有中断时序、Difftest、随机回归或波形。
