# D3 定向验收记录（2026-09-23）

## 输入与命令

工作区当前源码；镜像 `ysyx-npc:local`；容器内 Verilator 5.048。未修改生产 RTL。

```bash
docker exec ysyx-npc bash -lc 'cd /workspace/ysyx-workbench/npc && bash tests/run_d3.sh'
```

`run_d3.sh` 在容器内构建并运行以下三个顶层测试：

| 测试 | 覆盖的行为 | 本次结果 |
|---|---|---|
| `redirect_req_tb` | EX Branch taken/not taken、JALR 目标 bit 0 清零 | PASS |
| `redirect_kill_tb` | ID JAL、EX Branch/JALR 的 redirect 和年轻槽 kill、redirect 优先级、Load→Branch stall/forwarding | PASS |
| `system_interrupt_tb` | WB ECALL/MRET 的 PC/CSR 更新和年轻 MEM Store kill；中断的 WB 保留、恢复 PC、MRET 后重执行；ECALL/EBREAK 与中断优先级；WB CSR 写先于持续请求的中断；错误路径 EBREAK 不 halt | PASS |

本次修复了 `redirect_kill_tb.sv` 对旧信号 `ex_redirect_fire` 的引用，改为顶层现有的 `ex_branch_redirect_fire`。原测试在修复前因该信号不存在而无法编译。

## 验收边界

这是定向 RTL 仿真，不等同于 Difftest、长程序回归或形式证明。`interrupt_valid` 在测试中按持续到被接受的电平请求驱动；顶层没有 `ready/ack` 或挂起寄存器，单拍脉冲与 WB CSR/system 提交冲突时可能丢失。仿真程序的 `interrupt_check()` 每拍从 CSR pending 状态重新查询，但该行为不构成任意外部脉冲的接口保证。

D4 的普通 CSR→GPR forwarding 和 CSR 同地址写后读 interlock 不在本次 D3 验收范围；另见 `cpu.md` D.4。

## 2026-10-04：当前 DPI 接口下的 D3/D4 基线复验

本轮只修改 `tests/redirect_kill_dpi.cpp`，补齐与 `include/dpi_export.h` 一致的三个 C++ 定义：`dpi_is_mmio`、`dpi_wb_event`、`dpi_mmio_read_event`。修改前 `redirect_kill_tb` 已完成生成和编译，但链接因这三个符号 undefined reference 失败；证据保存在 `build/dpi-baseline-20261004-SaD4uT/build.log`。

共享测试桩仅提供普通内存模型，地址查询返回非 MMIO；两个事件回调不缓存生产事件，若收到有效 MMIO 标记则报错退出。因此本次恢复测试接口，不构成真实设备消费或生产 `CycleEvent` 的验收。没有修改 `vsrc` 或 SystemVerilog 测试台；修改前后 `vsrc/*` SHA-256 一致。

环境：`ysyx-npc` 容器，Verilator 5.048。当前运行标识 `dpi-regression-20261004-1pNnZW`；构建日志、运行日志、工具版本及源码 SHA-256 保存在对应 `build/` 子目录。可从项目根目录复现，使用新目录避免覆盖证据：

```bash
docker exec ysyx-npc bash -lc '
set -euo pipefail
source /workspace/ysyx-workbench/npc/container/env.sh
cd /workspace/ysyx-workbench/npc
run_dir=$(mktemp -d build/dpi-regression-XXXXXX)
for tb in redirect_req redirect_kill system_interrupt csr_forward csr_state; do
    verilator --binary --timing -Wno-TIMESCALEMOD \
        --top-module "${tb}_tb" -Ivsrc -Iinclude \
        --Mdir "$run_dir/$tb" \
        "tests/${tb}_tb.sv" tests/redirect_kill_dpi.cpp vsrc/*.v \
        > "$run_dir/$tb.build.log" 2>&1
    timeout -k 2s 30s "$run_dir/$tb/V${tb}_tb" \
        > "$run_dir/$tb.run.log" 2>&1
    cat "$run_dir/$tb.run.log"
done
'
```

观察：五个测试台均构建成功、运行输出 PASS，整组命令退出码 0。其中 `system_interrupt_tb` 运行 11 项场景、`csr_forward_tb` 6 项、`csr_state_tb` 7 项。结论限于这些定向场景；没有运行生产 Difftest、真实 MMIO/键盘路径或宿主 timer，也没有采集本次 VCD。下一步需要独立检查 MEM load 被接受时的数据捕获与设备消费，以及 redirect kill 时不消费；MMIO skip 的状态同步本身不能作为设备数据正确性的独立依据。
