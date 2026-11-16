# 十个 CSR 的 M-only 软件写入验收（2026-11-01）

> 后续按用户要求已删除本文件引用的临时测试源码和生成产物。以下保留当时验收记录，命令不再能直接复现，见 [清理记录](test-cleanup.md)。

适用合同见 [M-only CSR 字段设计](../design-process/m-only-csr-fields.md)。本轮经用户明确授权修改生产 RTL；不表示完整特权 ISA、S/U 模式或精确访存异常已经实现。

## 修改和时序

- `csr.v`：mstatus 仅 MIE/MPIE 可写、MPP=M，其他未实现或保留字段为零；mip 软件写保留设备控制的 MTIP/MEIP、其余零；mie 仅 MEIE/MTIE 可写。mtvec/mepc 的低位约束保持；mcause/mtval/mscratch 全宽保留。
- mcycle/mcycleh：以统一 64 位 next 值更新，软件写被寻址半部时保持另一半，并优先于该拍自增。阻止提交的写不抑制正常自增。原整体加一与切片写入在溢出边界同时改变另一半的问题已由测试复现后修正。
- NEMU：新增 `csr_write()`，六类 CSR 指令调用它，保留各自的 rd/rs1/zimm 抑制与非法地址 trap。按规范及本项目合同独立处理字段，没有复制 DUT 执行结果。
- RTL 端口、流水线时延、状态机、ready/valid、异常和 IRQ 仲裁未改变。计数器写入同拍的行为发生上述明确变化；组合路径增加软件写优先的 64 位选择。

## 环境和结果

ysyx-npc 容器，UID/GID 1006:1006，Verilator 5.048，并行度 2。所有构建和仿真在容器完成。未替换用户的 `npc/build/sim` 或 NEMU 默认 REF 动态库，使用隔离产物。

| 检查 | 结果 | 产物 |
| --- | --- | --- |
| 修改前扩展 RTL 单元测试 | 140 项，87 通过、53 失败，包含保留字段和计数器边界 | `build/m-only-csr-Tfoi73/red/result.log` |
| 修改前独立 REF 指令测试 | 209/267 通过，写入约束缺失得到复现 | `build/m-only-csr-Tfoi73/red/ref-result.log` |
| 修改后 CSR/译码/计数器单元测试 | 140/140 通过 | `build/csr-fields-MCn8Yg/unit.log` |
| 修改后独立 REF 指令测试 | 267/267 通过，覆盖十地址、六操作、x0/立即数、别名和不支持地址 | `build/csr-fields-MCn8Yg/ref.log` |
| 自检字段程序 + 生产仿真器 Difftest | GOOD TRAP，336 个程序内比较，八个非周期 CSR | `build/csr-fields-MCn8Yg/fields.log` 与 `fields.json` |
| CPU35 + Difftest | 35/35 通过，req_delay=3、resp_delay=5、random=7、seed=73 | `build/csr-fields-MCn8Yg/cpu-summary.log` |
| 原生非法指令回归 | 7 固定场景、2 随机等待及 3 负向检查通过 | `build/x-illegal-oWdxKB/native-summary.log` |
| RT-Thread 延时/抢占/上下文 + Difftest | diagnostic、tick、host 通过；关闭 timer 的负例按预期不完成；既有负向检查被拒绝 | `build/rtthread-delay-ixrcIi/` |
| 普通 RT-Thread 启动/命令及 timer | off/restore/host 有界检查通过 | `build/rtthread-delay-ixrcIi/boot-e7KCsx/` |

字段测试首次在 `build/csr-fields-bXPZ6G/fields.log` 假设 mip 恒为零，运行中宿主 timer 置位 MTIP，DUT/REF 同时 BAD TRAP，并非 Difftest mismatch。相同二进制仅屏蔽 SIGVTALRM 后通过（`fields-timer-isolated.log`）。测试入口因此只对字段专项通过 `run_without_host_timer.py` 控制该外部输入；生产 timer 机制未修改，CPU35/RT host 回归保留实际 timer。

CSR 单元未采集波形；原生异常和 RT 复用既有 VCD/日志合同，本次脚本实际采集并完成离线检查；没有新增生产 probe 或改变事件 ABI。这些检查与 DUT/REF 独立期望配合使用，不把同源信号一致当成独立 ISA 正确性证明。

生产 C++ 构建仍有原有 `itrace.cpp:103` 的 uint64_t 与 `%d` 格式警告，本轮未修改该无关源码。RTL/REF 构建及上述测试通过。

最后清理了 `nemu/csr.c` 文件末尾的额外空行；该格式调整后重新构建 REF，267 项仍全部通过，见 `ref-final-build.log`、`ref-final.log` 与 `final-production.sha256`。原始 `input.sha256` 保留综合回归开始时的字节摘要，RTL 没有后续变化。

## 复现

容器内、项目根目录：

```bash
cd /workspace/ysyx-workbench/npc
bash tests/run_csr_fields.sh
bash tests/run_x_illegal.sh
NPC_RTT_REF_SO="$PWD/build/csr-fields-MCn8Yg/ref-build/riscv32-nemu-interpreter-so" \
  bash tests/run_rtthread_delay.sh
```

重新运行第一条会打印新的独立产物目录；RT 的 `NPC_RTT_REF_SO` 应替换成对应新目录。每次测试保存输入 SHA-256，并检查运行期间输入未变。

## 边界

- 这是十个既有 CSR 的软件写入合同，不增加 S/U、misa、minstret、PMP 或 MMU，也不完成整个 Machine-Level ISA。
- mcause 的 Exception Code 是 WLRL，全宽存储不表示 CPU 会生成所有编号；不支持的值不增加额外 trap。mtval 保留非法指令编码，IF/D 错误仍是 faulted，后续精确访问异常另做。
- mcycle 的周期、溢出和半部写入在 RTL 单元验收；REF 没有核心周期概念。现有生产 Difftest 仍只直接比较八个 CSR，没有把真实 mcycle 读出结果形成周期对齐合同，字段程序有意不执行计数器读。
- MTIP/MEIP 当前由仿真路径置位并在现有 trap 路径清除，尚不是标准 mtime/mtimecmp 或真实外部中断控制器的完整 SoC 协议。
- 某些单元边界由 testbench 层次赋值设置初始状态（计数器溢出、pending=1）；实际软件写入和响应检查使用 DUT 端口。这不能替代真实外设的全部行为验收。
- 默认仿真器/REF 二进制未覆盖；后续日常运行需通过既有构建入口重新构建，或直接使用本验收目录内的 model/Vcore_top 和 REF。
