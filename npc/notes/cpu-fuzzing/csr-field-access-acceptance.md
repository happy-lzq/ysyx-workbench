# CSR 字段及非法访问验收记录（2026-11-01）

> 本记录引用的临时测试源码和生成产物已按用户要求删除；以下是历史失败证据的文字记录，见 [清理记录](test-cleanup.md)。

本文件保留较早失败版本的证据。后续经授权完成的软件写字段修改与新验收，见 [M-only 十 CSR 验收](m-only-csr-acceptance.md)。

结论：当前版本未通过全部 CSR 字段验收。范围为 M-mode 单独实现假设；本次未修改生产 RTL 或 NEMU。

环境：ysyx-npc 容器，Verilator 5.048，容器执行用户 UID/GID 1006:1006。受测 SHA-256：

- `npc/src/vsrc/src/csr.v`：`82598076cbe4b7fced8695cf922cde0f7f819ae6edfd21bad7983f463f0b4007`
- `npc/src/vsrc/src/control.v`：`4cc0076eb31dcf4176437071aa65bb6585287d1dc0a7eda4dddabdc6d0dd0b32`
- `npc/tests/csr_fields_review_tb.sv`：`f9e9d42ab8e4ef9b07c793cfa7b5982d87aed6209ff35c6faf3c5cbba2bd8351`

## CSR 和译码单元测试

运行：

```bash
cd /workspace/ysyx-workbench/npc
set -o pipefail
run=$(mktemp -d build/csr-fields-review-XXXXXX)
verilator --binary --timing -Wno-TIMESCALEMOD \
  --top-module csr_fields_review_tb -Isrc/vsrc/include \
  --Mdir "$run/model" tests/csr_fields_review_tb.sv \
  src/vsrc/src/csr.v src/vsrc/src/control.v > "$run/build.log" 2>&1
ulimit -c 0
"$run/model/Vcsr_fields_review_tb" 2>&1 | tee "$run/result.log"
```

运行于 `build/csr-fields-review-TkG2d5`，27 项检查中 24 通过、3 失败；测试以非零退出。通过包含 mtvec/mepc 各一组低位写入、mie 全 1 写、mtval/mscratch/mcause 一组写、mip 的 MTIP/MEIP 保留、同步异常优先保存现场，以及 6 类 CSR 指令各一组合法/不支持地址译码。mip 置 1 使用 testbench 层次赋值，只验证 CSR 写入保留，不证明真实中断源行为。

失败：

1. `mstatus` 写入全 1 后 `SUM[18]` 为 1，M-only 配置应为 0。
2. `mstatus` 写入全 1 后 `SIE[1]/SPIE[5]` 为 1，M-only 配置应为 0。
3. `mip` 写入全 1 后 `SEIP[9]/STIP[5]/SSIP[1]` 为 1，M-only 配置应为 0。

测试没有采集波形；单元测试不能证明真实流水线 CSR 提交或外设中断事件。未实现的 CSR、读写意图、所有字段、计数器写入与自增竞争、实际外设置位/清除及真实 IRQ 同拍交互尚未覆盖完整。

## 现有原生 RTL 非法指令回归

```bash
cd /workspace/ysyx-workbench/npc
bash tests/run_x_illegal.sh
```

产物 `build/x-illegal-94rG5M`。7 个固定场景、2 个随机等待场景及 3 项负向检查通过；脚本生成原生 RTL VCD、CSV，并运行其离线检查。该回归以普通非法指令为主，不应当作全部 CSR 非法访问或 CSR 字段 Difftest 通过。

## 尚需处理

- 保持 M-only 假设，分别修正 mstatus 和 mip 的无 S 模式字段；不要把设备控制的 MTIP/MEIP 硬接为 0。设备响应和 `mtimecmp` 是后续真实 SoC 契约。
- NEMU 当前 CSR 地址映射后直接写存储，未复现 mtvec/mepc/mstatus/mip/mie 的 RTL 字段约束；代码审阅可推断部分写入将导致不一致，本次未做新的 Difftest 复现。修正 RTL 后应独立定义规范行为并同步 REF。
- Difftest 当前只采集比较前 8 个 CSR，未对 mcycle/mcycleh 的周期行为形成验收。
- 译码器仅支持固定 10 项 M 模式 CSR；尚未实现实际独立只读 CSR 的地址读写意图验收，也无 S/U 特权级切换。
- IF/MEM 错误目前仍采用 faulted 停机，不是完整精确异常。
