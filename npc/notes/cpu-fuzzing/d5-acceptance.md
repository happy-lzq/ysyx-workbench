# D5 固定时延共享端口基础验收

日期：2026-10-11。仅使用现有 `ysyx-npc` 容器，Verilator 5.048。未修改宿主机或 nanhu 环境。

## 实现范围

- 用户已有 IF 数据输入、MEM 原始读数据输入及 LSU 写数据输出，本次保留这些接口。
- 将用户创建的 `mem_prot.v` 统一命名为 `mem_port.v`，保留其 MEM 优先仲裁与 DPI 后端主体，并在 `core_top` 接入实例。
- `mem_stage` 输出有效 `mem_read/mem_write`；顶层生成请求、连接授权与数据。DPI 读写从顶层移到独立端口模块。
- 四组流水线寄存器未修改；端口内无新增状态寄存器，读组合返回，Store 在上升沿执行。
- 复用已有 MMIO 接受事件，`mem_port_is_load` 由端口的 `mem_load_fire` 输出驱动。没有新增提交事件字段或 RTL probe。

## 产物与命令

主运行目录：`npc/build/d5-mem-port-20261011-xvji8wb_/`。

目录内保存原始 RTL 快照和哈希、构建日志、五组旧定向测试日志、三个程序的镜像/ELF、仿真器、REF 动态库、配置及最终哈希 `final-sources.sha256`。

新增 D5 测试入口（容器内）：

```bash
cd /workspace/ysyx-workbench/npc
bash tests/run_d5.sh
```

最终独立 D5 产物：`npc/build/d5-acceptance-OC3gY5/`。

本次其他复现命令（容器内）：

```bash
bash build/d5-mem-port-20261011-xvji8wb_/regress.sh
bash build/d5-mem-port-20261011-xvji8wb_/build.sh
bash build/d5-mem-port-20261011-xvji8wb_/integration.sh
```

完整仿真构建使用项目 `container/build.sh`；REF 构建使用 `make SHARE=1 git_commit=`。编译各使用 2 个任务。集成程序使用当前 DIFFTEST 配置，关闭文件 Diff Log；SDL dummy 仅用于无显示运行。脚本不会提交代码。测试是确定性程序，无随机 seed。

## 观察结果

| 检查 | 结果与范围 |
|---|---|
| 改造前唯一访问位置检查 | 按预期失败：`core_top.v` 与未接入的 `mem_prot.v` 均有 DPI 读写位置；见 `before-access-check.log` |
| 最终唯一访问位置检查 | PASS：生产 RTL 的 DPI 读写调用各只有一个位置，均在 `mem_port.v` |
| 新增 `mem_port_core_tb` | PASS：15 条预期 WB 提交、4 次 Store、6 次 IF/MEM 争用、2 拍 Load-use、2 拍 CSR stall、1 次 taken branch |
| PC/IF-ID 与授权 | 未授权且无 redirect 时 PC 保持；ID 已消费且无新取指时 IF/ID 无效；授权互斥、MEM 优先 |
| 数据与提交 | 精确检查 WB PC/inst 序列；SW/SB/SH 数据和掩码结果正确，错误路径 Store 未执行，Load/forwarding/CSR 寄存器结果匹配 |
| D3 回归 | `redirect_req`、`redirect_kill`、`system_interrupt` 三组 PASS；包括 JAL/JALR/Branch、WB system/IRQ kill 与恢复 |
| D4 回归 | `csr_forward`、`csr_state` 两组 PASS |
| 完整 RTL+C++ / REF 构建 | PASS |
| 集成 Difftest | `load-store`、`unalign`、`hello-str` 均正常退出，NPC/NEMU GOOD TRAP；REF 分别执行 372、168、1309 条指令 |

首次动态测试出现测试收尾缺陷：`$finish` 后继续执行末尾 `$fatal`，即使已到达预期提交仍打印失败。修正测试退出循环，并使脚本拒绝 fatal/error 日志后重跑；上述 PASS 只引用最终 `OC3gY5` 运行。

## 基础验收时的边界与剩余工作

- 本次没有新增请求/响应等待机制、反压或在途响应状态，不代表 E 已完成。
- 新 D5 程序的内存来自 RAM-only DPI 测试后端；`hello-str` 增加实际串口 MMIO 集成证据，但没有独立计数全部 MMIO 消费或验证键盘 peek/dequeue 的 kill 场景。
- 分支、CSR、WB system/IRQ 的旧定向回归已重跑；完整争用碰撞矩阵仍未覆盖。
- 本次未采集 VCD，也未新增 probe；不能称为本次波形资格验证通过。
- 本次未重跑 CoreMark、RT-Thread、完整 35 项 cpu-tests 或 MicroBench。指定用例通过不能外推为全部 RTL 正确。

## D5 完成度复核（2026-10-11）

本次只检查源码、复跑已有 D5 测试并更新笔记，没有修改 RTL、C++ 或测试逻辑。

- 旧 `build/d5-acceptance-OC3gY5/sources.sha256` 对当前全部 `vsrc/*`、D5 testbench 与 DPI 测试后端校验均为 OK。
- 在 `ysyx-npc` 内执行 `bash tests/run_d5.sh`，退出码 0；新增产物目录为 `npc/build/d5-acceptance-USuAfZ/`，包含访问位置审计、源码哈希、工具版本、构建和运行日志。
- 本次结果：唯一 DPI 读写位置检查 PASS；`commits=15 stores=4 collisions=6 load_stalls=2 csr_stalls=2`，仿真在 306 ns 正常结束，无 fatal/error。
- 检查 testbench 可知中断输入固定为 0，DPI 后端的 `dpi_is_mmio` 固定返回 0。因此该测试不能验收真实 MMIO 消费或 IRQ 与端口争用碰撞。旧 D3/D4 与三个集成 Difftest 的结果是上次运行记录，本次未重跑这些组。

结论：**D5 固定时延基础实现与指定测试通过，整体专项验收仍未闭环。** 下一步补齐真实 MMIO 的接受/消费/WB 数据对应、kill 无副作用，以及明确命中的控制碰撞场景。跨周期响应等待属于 E，不能混入 D5 的完成判定。本次未采集 VCD。

## 后续专项闭环（2026-10-11）

上面的“专项尚未闭环”描述保留为基础验收及首次复核时的状态。后续在不修改生产源码的前提下，已完成 15 个真实设备/控制碰撞场景、生产 Difftest、计数/提交身份检查、三种故障负例与 VCD/观察资格核对。**D5 按当前固定时延指定范围完成闭环，下一步进入 E。** 新证据及适用边界见 [D5 专项闭环记录](d5-closure-acceptance.md)。
