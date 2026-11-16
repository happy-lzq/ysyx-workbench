# NEMU 非法指令异常与 Difftest 补齐计划

日期：2026-10-27。授权目标：完善当前 REF 缺失的非法指令同步异常，使指定客户程序 Difftest 通过；不修改 CPU RTL。

## 范围和决定

采用前述已讨论的独立 REF 方案：REF 从自己的指令存储取指、判断是否合法、计算 trap 状态，再由已有 Difftest 比较。没有合法匹配时使用 cause=2，mepc 为本条 PC，mtval 选择返回指令位。既有 MPP=M 的用户修改保留。

保留现有 DPI/提交 ABI，不复制 DUT 的异常结果给 REF，不扩大到访问错误、CSR 权限、未对齐、完整 WARL、Vectored mtvec 或 Linux 特权支持。不重构旧译码注释块。既有 FENCE 需要显式合法匹配，因为此前无匹配编码顺序前进的行为会被替换。

## 执行顺序

1. 保存当前 RTL/头文件、软件输入、配置、旧 REF 二进制和将修改文件的副本。在 NPC 容器中按当前用户源码构建 REF 基线。
2. 新增独立 REF 原生 API 测试：合法算术/跳转/CSR/FENCE 对照，多个非法编码、MIE=0 时同步异常、ECALL/IRQ 的 mtval 清理、M-only MRET。修改前执行，确认失败来自实际行为。
3. 最小修改 NEMU `inst.c` 的合法 FENCE 和活动兜底，`system/intr.c` 的公共 mtval 清理；重建后运行同一个测试。
4. 让 AM 测试支持可选 REF 和全零输入，用生产 C++ 对原编码/全零两组客户执行 Difftest。用基线 REF 负对照确认检查器确实捕获旧失配。
5. 构建当前 DUT 模型，运行原生异常测试、CPU35 和 RT timer/抢占回归；核对 RTL 摘要未变化，更新验收记录并给出软件源码讲解。

## 验证合同

- 全部构建/仿真位于现有 `ysyx-npc` 容器，工作区 `/workspace/ysyx-workbench`，并行度 2。
- 使用唯一 build 目录，不覆盖已有验收证据，不清理用户产物、不提交或推送。
- 原生测试只承诺实际覆盖项；独立 REF 使用手工期望状态，不读取 DUT 的结果。
- 独立 REF 正例及 AM Difftest 正例必须真实退出 0；retry/default 等负例必须匹配预期原因，不能只检查非零退出。
- 若回归暴露 RTL 问题，先定位、记录并解释，不直接修改 RTL。
- 最终由一次只读评审检查本任务相对初始副本的改动和证据。

## 执行记录

- Task 1：完成；当前 REF 基线从用户已有 MPP 修改构建并保存为独立库，RTL/软件摘要和改动文件副本已归档。
- Task 2：完成；同一组独立 REF API 测试在修改前 12/29 通过、17 项失败，退出 1；失败包含非法编码不 trap 和旧 mtval 残留。
- Task 3：完成；最小修改后 29/29 通过。生产修改仅为 NEMU 两个 C 文件，用户 MPP 修改保留。
- Task 4：完成；两种编码共 8 个 AM handler 模式带 REF 符合预期，旧 REF 负对照重现原 mcause 失配。修正 ASFLAGS 后的最终结果使用 reserved-final/、zero-final/。
- Task 5：完成；CPU35/35、当前原生异常 9 组、用户 MPP 后状态、RT timer/抢占/普通启动回归均通过；只读最终评审未发现本任务新增的阻塞问题，最后核对摘要并交付。

Ruling：直接在用户指定的当前工作区实现，不新建 worktree、不提交；这是用户明确要求直接完善当前 NEMU 的工作方式，保留既有修改并用任务初始副本区分改动。

Task 4 调试记录：原编码的四种 AM handler 模式已带 REF 通过。首次全零用例的 CFLAGS 已传宏，但 AM 对 `.S` 使用独立 ASFLAGS，导致探针仍为旧编码；已从反汇编定位测试输入错误，补齐 ASFLAGS，在新目录重跑，不调整生产 REF/RTL 来规避断言。
