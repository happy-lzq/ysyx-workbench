# NEMU RAM 未对齐异常修复与 RTL/ISA 对照

验收日期：2026-11-16（Asia/Shanghai）。本任务于 11 月 14 日建立修改前证据，中断后于 11 月 16 日核对进程、产物和源码完整性并继续。运行目录：`npc/build/ref-mem-fix-20261114-v39df72c/`。

**原先 `PC=0x800009c8` 的未对齐 LH 失配已修复，并通过当前范围的独立 REF 测试与 Difftest。** 生产源码仅修改 `nemu/src/isa/riscv32/inst.c`，并重新构建常规运行加载的默认 REF；没有修改 RTL、生产 NPC C++、公共 trap 函数或设备同步协议。该结论不扩展为完整设备属性模型、完整精确异常或 ISA 合规认证。

## 1. 最早差异与根因

上轮首个失配是 `LH t2,0(t0)`，有效地址 `0x80010001`，指令 PC `0x800009c8`。DUT 按已选策略保存 cause=4 并保留 x7，旧 REF 则将未对齐地址直接交给 `Mr`，读出 `0xffffff7f` 并写 x7。

本轮修改前重新构建 REF，并实际复现失败：

- 原始 8 例：[before-ref-probe.json](../../build/ref-mem-fix-20261114-v39df72c/before-ref-probe.json)，5 个未对齐实例失败，3 个正常对照通过。
- 扩展 205 例：[before-memory-cases.json](../../build/ref-mem-fix-20261114-v39df72c/before-memory-cases.json)，111 通过、94 失败。非法区间在有界子进程执行，旧 REF 的宿主断言/异常退出也记录为失败，没有通过读取不安全地址伪造通过。

实际链路为：

```text
Load/Store INSTPAT
    → Mr/Mw
    → vaddr_read/write
    → paddr_read/write
    → 宿主内存读写，或未映射地址的 panic
```

这条路径既没有指令级 RAM 未对齐异常判断，也没有在本条指令执行前验证完整访问区间。仅修改最终 Difftest 比较或复制 DUT CSR，不能解决这个根因。

## 2. 实际修改

[inst.c](../../../nemu/src/isa/riscv32/inst.c:82) 新增三个局部函数，8 条 Load/Store 均调用它们：

| 函数 | 职责 |
| --- | --- |
| `data_access_ok(s,addr,len,is_store)` | 判断完整 RAM 区间与自然对齐；失败时进入公共 trap，写 mtval，返回 false |
| `execute_load(...)` | 检查成功才真实读取；按指令符号/零扩展；最后决定是否写 rd |
| `execute_store(...)` | 检查成功才真实写入 |

当前流程：

```text
译码捕获 rs1/rs2 和符号扩展立即数
    → 按 RV32 有效地址语义形成 addr
    → 检查 addr..addr+len-1 的完整区间
        ├─ RAM 完整区间内且自然对齐：正常访问
        ├─ RAM 完整区间内但未对齐：Load=4 / Store=6
        └─ 越过 RAM 结束边界、或当前无设备 REF 的未映射地址：Load=5 / Store=7
    → 故障时：isa_raise_intr(cause,s->pc)
              mtval=原有效地址
              直接返回，禁止 Mr/Mw 和普通 rd 写入
```

`last` 使用 `uint64_t`，保留末字节加法溢出；地址本身仍为本项目 RV32 的 `vaddr_t`。使用 NEMU 自己的 `CONFIG_MBASE/CONFIG_MSIZE`，没有引用 DUT 输出或读取 NPC 的异常结果。

本次没有改动 [isa_raise_intr](../../../nemu/src/isa/riscv32/system/intr.c)：它继续处理 mepc/mcause 和 MIE/MPIE/MPP，返回 mtvec；指令局部逻辑在公共入口清理 mtval 后写入本条访问的故障地址。也没有改 MRET 或通用取指函数，因而 cause=1 并未随本次完成。

正常 Load 的读取位于 `if (rd != 0)` 之前，所以 `rd=x0` 仅丢弃值，不能跳过访存与异常。译码阶段已先捕获 src1，故 `rd=rs1` 也不会影响本条访问的有效地址。

影响：REF 的架构执行路径改变；DUT 的周期、流水级、状态机、组合路径、ready/valid、reset 和综合逻辑均未改变。完整补丁相对于用户原文件保存在 [ref-fix.patch](../../build/ref-mem-fix-20261114-v39df72c/ref-fix.patch)。

## 3. ISA 与项目策略的区别

依据官方 [RV32I 2.1，Load and Store Instructions](https://docs.riscv.org/reference/isa/v20260120/unpriv/rv32.html#_load_and_store_instructions)：有效地址来自 rs1 加符号扩展的偏移；各指令按宽度读写及扩展；`rd=x0` 不取消异常与副作用。未对齐访问是否支持由执行环境选择，不能将“必须报告 4/6”说成所有实现的统一要求。

本项目选择 RAM 不透明拆分，Device 不透明重复消费。官方 [Machine-Level ISA 1.13 源文](https://github.com/riscv/riscv-isa-manual/blob/main/src/priv/machine.adoc) 的 mcause、同步异常优先级、mepc/mtval 和状态栈规则用于核对 trap。访问错误与地址未对齐之间存在实现可选的优先关系；本次采用完整区间失败优先。mtval 对具体异常的信息报告有平台约定，不能泛化为所有 trap 都无条件写相同数据。

### 3.1 当前代码对照

| 检查点 | RTL producer → consumer | 修复后 REF | 本次依据与边界 |
| --- | --- | --- | --- |
| 有效地址 | `imm_gen` → `ex_stage` ALU → `ex_alu_final` → EX/MEM `ex_mem_alu_result` → MEM | `src1+imm` → `vaddr_t addr` | 正常、负立即数和 rd=rs1 REF 用例；RTL 请求/异常绑定同一有效地址 |
| 数据宽度 | control 的 LSU 编码 → `mem_stage`/LSU | 5 类 Load/3 类 Store 的 len=1/2/4 | 避免直接把 funct3 当内部 LSU 编码；本轮各类均有正反例 |
| 自然对齐 | `mem_stage.addr_misaligned`：半字 bit0，字低两位；字节无额外限制 | `addr & (len-1)` | 同一字节偏移在不同宽度下可能合法或非法 |
| RAM 对齐异常 | 完整 RAM 区间内的 `ram_align_fault` → 顶层 cause=4/6 | 完整 RAM 区间内判断对齐，报告 4/6 | 这是本项目选定策略；不能据此宣称支持未对齐硬件拆分 |
| 区间与优先级 | 33 位 `access_last`、`local_range_fault`；顶层访问错误分支优先 | 64 位 last，完整区间失败优先于对齐，报告 5/7 | RAM/Device 末端跨界测试有实测；不是按 cause 数值仲裁 |
| 符号扩展与 Store 数据 | `lsu` 的 byte/half 选择、符号扩展和 mask | 读取后按 LB/LH 扩展，Store 交付低 len 字节 | 当前 little-endian；RAM 四个 lane 的数据/相邻字检查通过 |
| 故障槽与正常退休分开 | `mem_exc_*` → MEM/WB → `wb_trap_fire`，`reg_write_eff` 关闭 | 进入 trap 后不执行 Mr/Mw，不写 rd | trap 边沿全部 GPR 保持，未执行的 Store 不改内存 |
| 精确现场 | 该指令 PC → `trap_pc` → csr.mepc；cause/tval → csr | `s->pc` → mepc，cause → mcause，addr → mtval | mstatus 初值包含 MIE 开/关两组；公共入口和恢复自检通过 |
| `rd=x0` | 访存请求与消费不由 rd 决定，WB 抑制 x0 写入 | 先检查/读取，再决定写 rd | REF 故障 Load x0；DUT 键盘 x0 读取仍消费的定向观察 |

这里的“代码对照”是源码审阅；仿真结论来自第 4 节，均不是形式证明。当前跨界访问在发请求前整笔拒绝，不执行字节拆分；以后若实现分段访问或总线返回子访问错误，必须重新按实际故障部分核对 mtval，不能机械复用原起始地址。

### 3.2 MMIO 的重要边界

当前 NEMU 配置未启用 `CONFIG_DEVICE`。因此 REF 直接执行的非 RAM Load/Store 被视为其地址图中的访问错误；**合法 NPC MMIO 仍由既有 WB 协议同步结果**。本次没有改变该协议，也没有让 fault slot 冒充成功 MMIO 来跳过异常。

所以 73 例 Difftest 通过不表示 REF 已独立判断 NPC 的设备宽度/权限。若 DUT 将一条本应成功的 MMIO 误判为访问错误，仅靠这个无设备 REF 可能无法识别；本次已有明确正常期望的设备夹具和真实消费计数补充检查。通用验证仍需要独立设备属性模型或独立的事务期望，不能只依赖寄存器一致。

`CONFIG_DEVICE` 开启时保留既有非 RAM 设备路径；该配置的设备特定错误、宽度和边界处理没有由本补丁补齐，也没有在本轮构建/验收。当前结论限定于现有 M-only、无 MMU、无 C、无设备 REF 的配置。

## 4. 实测结果

| 检查 | 修改前/负例 | 修改后 |
| --- | --- | --- |
| 独立 REF 205 例 | 111/205，94 失败 | **205/205** |
| 原失败的完整 73 场景 | 旧 REF 在 `0x800009c8` 改写 x7 | **73/73，43 次 trap，20703 周期** |
| 同一完整输入，随机等待 | — | **73/73，43 次 trap，85073 周期** |
| 用户默认路径 REF | 已保存旧文件 | **完整 73 场景通过** |
| CPU35 | 本次不以旧结果作比较 | **35/35 既有镜像通过** |
| 既有 cause=0 自检 | 原程序保留 | **48 场景/19 trap，默认/延迟 Difftest 均通过** |
| 检查器负对照 | 修改一条夹具的预期 cause | 按预期拒绝，RTL 不变 |

73 例中正常 30、cause=4 共 9、cause=6 共 5、cause=5 共 17、cause=7 共 12。本次完整序列 REF 执行 4455 条指令事件。请求与副作用计数仍满足：36 个本地错误零 D 请求，7 个后端错误各一次请求/执行/错误响应/trap，错误设备读不消费，年轻 Store 未生效。

- [独立 REF 修改后结果](../../build/ref-mem-fix-20261114-v39df72c/after-memory-cases.json)：包括所有 GPR/CSR、RAM 数据保持、rd=x0/rs1 别名、正常边界及故障边界。
- [完整 Difftest](../../build/ref-mem-fix-20261114-v39df72c/full-difftest-summary.json)、[延迟配置](../../build/ref-mem-fix-20261114-v39df72c/delayed-difftest-summary.json)。
- [相同输入换回旧 REF](../../build/ref-mem-fix-20261114-v39df72c/old-ref-negative.log)：仍出现原 GPR[7] 失配，退出码 1；不是额外的 RTL 失败。
- [CPU35 逐项结果、输入哈希和命令](../../build/ref-mem-fix-20261114-v39df72c/cpu35-results.json)：生产 main/时钟循环、当前默认 REF、35 个既有二进制镜像；未重新编译这些 C 输入。
- `unalign` 通过不表示硬件支持未对齐 LW/SW。其 [反汇编](../../build/ref-mem-fix-20261114-v39df72c/cpu35-unalign.disasm) 在 `0x8000007c..0x800000a8` 实际使用 LBU/SB，再用移位/OR 组合数据。

### 4.1 波形资格与证据完整性

本轮 `native-default` 实际使用用户默认 REF，采集原生 VCD 并核对日志：[qualification.json](../../build/ref-mem-fix-20261114-v39df72c/qualification.json)，20703 行、5174 次真实 DPI 执行、967853 次字段比较，0 不一致。其余配置采日志，未声称分别采集并通过了 VCD。

监视字段与原生信号一致仅说明本次身份/字段/采样合同一致；正确性预期另来自夹具与规范对照。生产源码完整性及默认 REF 的实际哈希见运行目录 `source-before.json`、`source-after.json`、`source-integrity.json`。绑定的生产源码仅 inst.c 改变，默认 REF 二进制相应更新；RTL/NPC C++ 不变。

## 5. 构建与复现

仅在 `ysyx-npc` 容器、UID/GID `1006:1006`、`/workspace/ysyx-workbench` 构建和仿真；Verilator 5.048，RISC-V GCC 13.2.0，构建并行度 4。定向同步异常测试不启动宿主 alarm，CPU35 使用原生产初始化流程。没有修改宿主或其他容器的软件环境。

独立 REF 构建与检查：

```bash
# 容器内，工作区根目录；已保存的证据应复制到新运行目录再复现。
RUN=npc/build/ref-mem-fix-20261114-v39df72c
python3 "$RUN/build.py" ref
python3 "$RUN/ref_memory_cases.py" \
  "$PWD/$RUN/ref-build/riscv32-nemu-interpreter-so" "$RUN/recheck-memory.json"
python3 "$RUN/run.py" recheck-full mem_cases \
  "$PWD/$RUN/ref-build/riscv32-nemu-interpreter-so"
python3 "$RUN/run.py" recheck-delayed mem_cases \
  "$PWD/$RUN/ref-build/riscv32-nemu-interpreter-so" \
  +mem_req_delay=3 +mem_resp_delay=5 +mem_random=11 +mem_seed=17
```

常规默认 REF 已使用 `make -j4 SHARE=1 git_commit= app` 及 `-W` 强制重建所选 C 源文件，命令与返回码见 `build-default-ref.json`；原默认文件保存为 `default-ref-before.so`，原独立 REF 保存为 `ref-before.so`。无需把测试目录的 so 路径永久改进平台 Makefile。

本次没有跑 RT-Thread、CoreMark/MicroBench，也未完成 IF cause=1、IRQ/较老异常/复位与错误响应的全面碰撞测试。当前修复的是 REF 数据访问策略与指定异常 Difftest 的缺口；设备模型及其独立错误条件仍是后续工作。
