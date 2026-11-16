# RT-Thread-AM：构建与启动验收

## 构建入口与当前依赖

BSP 位于 `am-kernels/rt-thread-am/bsp/abstract-machine`。当前 `files.mk` 列出既有 RT 配置的源文件和 include 目录；原来引用旧电脑 `/home/luo/ysyx/...`，现使用 `AM_RTT_ROOT := $(abspath ../..)` 计算仓库根目录。63 个引用均核对实际存在，未变更 RT 配置或源文件集合。

NPC 容器已安装 `libnewlib-dev`，提供 bare-metal GCC 缺少的 C 库头文件。当前由公共 `abstract-machine/scripts/isa/riscv.mk` 在 CROSS_COMPILE 包含 `unknown-elf` 时添加 `-isystem $(NEWLIB_INCLUDE)`（默认 `/usr/include/newlib`），覆盖 AM、子应用及 C++。BSP 仅保留 `_POSIX_C_SOURCE=200809L`；它避免 Newlib 的 BSD sys/types 经 RT-Thread sys/select/sys/time 回到 time.h 时 clock_t 尚未声明的包含循环。没有新增 Newlib 库链接，运行时仍使用 AM/RT-Thread 实现；保留 -Werror。头文件原来仅在 BSP 添加，导致独立构建的 snake/FCEUX 缺少 stdlib.h/inttypes.h；修正与完整应用镜像验收见 [2026-10-11 记录](rtthread-app-build-acceptance.md)。

现有容器已安装该包，Dockerfile 也记录依赖。没有重建/替换容器，没有安装宿主机软件或修改 nanhu-dfz 环境。容器没有 SCons，当前通过已有清单构建；将来变更 RT 配置时仍需准备 SCons 并更新清单。

首轮使用空 AM 应用清单，仅构建内核/控制台，未整合 MicroBench、游戏或 FCEUX。可复现入口（在 ysyx-npc 内）：

```bash
source /workspace/ysyx-workbench/npc/container/env.sh
task_dir="$NPC_HOME/build/rtthread-boot-gWwDvc"
cd /workspace/ysyx-workbench/am-kernels/rt-thread-am/bsp/abstract-machine
make ARCH=riscv32I-npc insert-arg \
  AM_APPS_MK="$task_dir/empty-am-apps.mk" git_commit=
cd "$NPC_HOME"
make sim git_commit=
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy timeout -k 2s 30s \
  ./build/sim --log-limit 1000 \
  -i "$task_dir/rtthread.bin" -e "$task_dir/rtthread.elf" \
  -d "$task_dir/ref.so"
```

上述运行使用证据目录中的镜像副本；重新构建后的镜像在 BSP/build 中，复现新版本时应另建产物目录并复制新镜像，避免覆盖本次证据。

## Difftest 初始 RAM 合同

开始执行程序前，DUT/REF 的整个 RAM 区间必须一致，不能只复制二进制范围。当前 NPC 清零 128 MiB RAM；NEMU 配置启用 MEM_RANDOM，初始化时填充随机字节；原 init_difftest 只复制 img_size 字节。

`tests/difftest_memory_init.cpp` 使用生产 monitor/Difftest 初始化，在执行任何指令前读取 REF 的镜像起点、镜像末尾之外、RAM 中部和末端，对照 DUT RAM。修改前观察到后三个位置 DUT=0、REF=0x67676767，测试退出码 1。修正 init_difftest 为从 PMEM_BASE 一次复制 PMEM_SIZE 字节后，测试 PASS、退出码 0。这是一次性的初态同步，不是运行时掩盖失配；每个提交事件仍比较 GPR/CSR。

## 2026-10-04 实际观察

证据目录：`npc/build/rtthread-boot-gWwDvc`，保留安装、构建、初始化测试与启动日志，以及镜像/ELF、REF 副本、配置、散列和 C++ 临时审计驱动。

1. 原清单构建退出码 2，无法找到 rtdevice.h；路径修正后推进至缺 sys/types.h；安装头文件并设置 POSIX 可见性后镜像构建退出码 0，bin 为 164600 字节。
2. 首次启动打印 RT-Thread 5.0.1 横幅，随后在 PC 0x80019b14（`lw s0,32(sp)`）失配。DUT x8=0、REF x8=0x67676767，两侧 sp=0x800282d8。load 地址为 0x800282f8，恰好是镜像已复制范围之外的首地址。
3. 全 RAM 初态同步后，初始化测试 PASS；生产模拟器越过该失配点，运行 30 秒无 mismatch，按预设超时结束，退出码 124。
4. 独立 C++ 临时审计驱动运行 500 万拍，记录最近有效 WB 提交及 irq_taken，退出码 0、无 mismatch。后期最近提交持续为 PC 0x80019b78 的 MRET，mepc 也为 0x80019b78；本次驱动观察到 35 次 irq_taken。未观察到 Hello RISC-V 或 shell 提示。早期 raw 审计含 bubble 槽，不用其 PC/inst 作为有效提交证据；以 boot-audit-valid.log 为准。
5. `bash tests/run_diff_log.sh` 回归退出码 0：开启/关闭日志两种配置均检出人为 x31 失配，正常短程序 GOOD TRAP。证据为 `npc/build/diff-log-test-2NWN0T`。表明内存初态修正没有关闭架构状态检查。
6. 生产 RTL 20 个文件与本轮前 SHA-256 一致，没有修改 RTL；没有采集 VCD，也没有进行新的插桩波形资格验证。

以上是首次审计的历史观察；后续定位与验收如下。

## Context 与中断返回的约束

当前 RV32 Context 为 144 字节：GPR 位于偏移 0～124，mcause/mstatus/mepc 分别位于 128/132/136，最后一个字为 pdir。`rt_hw_stack_init` 构造初始 Context；`rt_hw_context_switch_to` 取 `*(Context **)to` 放入 rt_next_ctx，再通过 yield/ECALL 进入 AM。EVENT_YIELD handler 返回下一线程的 Context；trap.S 用 `mv sp,a0` 切换恢复源，读取 Context 的 mstatus/mepc、恢复 GPR，最后 MRET。

MIE 控制当前 M-mode 是否允许中断；MPIE 是 MRET 要恢复到 MIE 的位。初始 Context 应保持 MIE=0、MPIE=1：恢复寄存器期间屏蔽中断，由 MRET 在返回线程时打开中断。不能把“线程开始运行后需要中断”理解为“恢复线程 Context 时就打开 MIE”。语义依据见 [RISC-V privileged specification](https://docs.riscv.org/reference/isa/v20260120/priv/machine.html)，Privilege and Global Interrupt-Enable Stack 小节。

## 2026-10-04 返回路径定位与修正

### 最早的功能偏差

证据目录：`npc/build/rtthread-context-FQRvAG`，包含修正前镜像、符号/反汇编、配置、REF、审计驱动及 `pre-fix-qualified.vcd/.csv/.log`。修正前受控用例运行 200 万拍，退出码 5（没有进入 main/shell），仅注入一次 timer 请求；观察到 55894 次 MRET，未出现 Difftest mismatch。

首次切换目标实际上为优先级较高的 `_timer_thread_entry`，不是 main：to=0x80028154、Context=0x80028098、Context.mepc=0x80019630。handler 返回该 Context；`mv sp,a0` 正确切换到 0x80028098。最早偏差出现在该 Context 的 mstatus=0x1888 被恢复之后：

| 审计周期 n（零起点） | 修正前的有效事件 |
| --- | --- |
| 43272 | PC 0x80019af4，`csrw mstatus,t1` 提交，MIE 提前变为 1 |
| 43273 | PC 0x80019af8，`csrw mepc,t2` 提交，mepc=0x80019630；受控 timer 已 pending，WB CSR 写优先，本拍不接受 IRQ |
| 43274 | PC 0x80019afc 的 load 提交，同时接受 IRQ；最老被 kill 指令为 PC 0x80019b00，mepc 被更新为该地址 |
| 43715 | 内层 timer handler 的 MRET 返回 0x80019b00；外层原本准备的线程入口 mepc 已丢失 |
| 后续 | 外层返回继续使用 mepc=0x80019b00，反复恢复寄存器、调整 sp、执行 MRET，无法进入线程入口 |

沿 RTL 核对了 WB CSR 写仲裁、irq_resume_pc、年轻槽 valid 清除、CSR trap 状态及 IF 重定向：上述保存 PC 和 MRET 目标符合当前设计。DUT/REF 共同执行错误的软件返回路径，故没有失配；不能因此判定 RT 启动功能正常。

### 实际修改

- `am-kernels/rt-thread-am/bsp/abstract-machine/src/context.c`：INIT_MSTATUS 从 0x1888 改为 0x1880。
- `abstract-machine/am/src/riscv/npc/cte.c`：AM kcontext 的初始 MSTATUS 同步改为 0x1880，避免创建同类不安全初态。RT 使用自己的 rt_hw_stack_init，本次 RT 验收直接覆盖前者；通用 kcontext 的独立切换测试尚未另行执行。
- 新增 `npc/tests/rtthread_context_test.cpp`：复用生产 single_cycle/Difftest，观察有效 WB、Context handoff、MRET、IRQ、main/shell 入口及 rt_tick；支持关闭 timer、固定恢复边界注入一次 timer、宿主 timer 三种模式。
- 新增 `npc/tests/check_rtthread_context_vcd.py`：核对审计字段与原生 RTL 波形及采样周期，并检查 MRET 目标、MIE/MPIE、IRQ 保存最老被 kill PC、mtvec 跳转和四组年轻槽 valid。
- 新增 `npc/tests/run_rtthread.sh`：容器内构建当前 BSP/NPC，以新证据目录保存输入与日志，执行三组有界测试及 probe 反例。

没有修改生产 RTL、DPI 事件接口或 trap.S。硬件流水级数、握手、reset 均未改变；软件的中断接受时点从恢复中途推迟到 MRET 之后，pending 事件仍会被接受。没有关闭生产 timer 或 Difftest。

### 修正后的受控时序

同一周期 43273 注入请求：MIE=0，CSR 写正常提交，pending 保留。周期 43305 的 MRET 返回 0x80019630 并恢复 MIE；周期 43306 无 WB 提交，IRQ 保存同一线程入口 PC，再跳转 mtvec。周期 43747 中断返回该线程入口；之后 main、shell 均运行。

## 当前验收记录

最终可复现测试目录为 `npc/build/rtthread-test-pe6tCr`，命令（ysyx-npc 内）：

```bash
source /workspace/ysyx-workbench/npc/container/env.sh
cd "$NPC_HOME"
bash tests/run_rtthread.sh
```

该脚本使用已有 RT 配置和空 AM 应用清单，不整合游戏等应用；每次使用新目录。当前配置 DIFFTEST/HAS_TIMER 开启，DIFF_LOG_ENABLE/ITRACE 关闭；timeout 30 秒、每组 200 万拍，无随机 seed，受控注入点为首次恢复 mstatus 的提交之后。host 模式保留原 ITIMER_VIRTUAL 约 60 Hz timer，其事件数依赖宿主耗时，不是可重复 seed。

| 模式 | 提交事件数（idx） | 接受 IRQ | rt_tick_increase 入口 / rt_tick | 功能与退出 |
| --- | ---: | ---: | ---: | --- |
| off | 1498963 | 0 | 0 / 0 | main、Hello、shell 命令运行，退出 0 |
| restore | 1498971 | 1 | 1 / 1 | pending 越过恢复区间，main、Hello、shell 命令运行，退出 0 |
| host | 1497909 | 57 | 57 / 57 | 周期 timer 保持运行，main、Hello、shell 命令运行，退出 0 |

三组无 Difftest mismatch，脚本总退出码 0。idx 在此表示 WB/IRQ Difftest 事件次数，同拍可能包含两者，不等于严格的 retired instruction 计数。shell 使用 BSP UART 的固定输入，执行 help/date/version/free/ps 等命令；utest 框架初始化但 testcase 数为 0，不称为 utest 回归通过。

本次实际采集了原生 VCD 与审计 CSV：每组核对 1976 个恢复区间周期；restore/host 的该窗口各覆盖 3 次 MRET 和 1 次 IRQ，off 覆盖 2 次 MRET。每组完整 200 万拍另核对原生 pre-edge WB 与 DPI 缓存身份。故本次波形资格只覆盖该窗口，不能称为全部 200 万拍的波形验证。修改一条 CSV wb_inst 的反例被离线检查器拒绝（退出 1）；正常资格检查退出 0。这证明采样/字段检查能发现该类不一致，不是独立 RTL 正确性证明。

审计期间发现并修正两个测试工具问题，证据保留：初版用 IF 地址触发 trace，捕获了 RET 杀死路径的同址取指，未覆盖 MRET，资格检查正确拒绝；改为有效 WB 动态身份触发。随后串口和审计同写 stderr 导致 `msh />ps` 被诊断文本打断，runner 的字符串检查失败；现在审计独立写 `.audit.log`，串口输出独立保留。两者均未修改 RTL。

另用生产 `build/sim`（没有审计驱动）执行当前修正镜像 12 秒，配置与 REF 开启不变；日志 `npc/build/rtthread-test-pB3O6H/production.log`。观察到 Hello、shell 命令输出，未观察到 mismatch/ABORT；按有界 OS 运行计划超时退出 124，不算程序正常结束/GOOD TRAP。

生产 RTL 20 文件与本轮开始散列逐项一致，见 `rtl-after-check.log`。工具/编译命令可在 `sim-build.log` 复查，镜像、REF、驱动和脚本摘要保存在 inputs.sha256。

## 适用边界与下一步

当前已验收指定配置的 RT 启动、初始线程切换、timer 接受/返回、tick 推进和固定输入 shell。未发现此次启动故障由 RTL 造成；这不等于整个 RTL 或全部特权 ISA 已验证。

后续两个线程的 delay/wakeup 与 timer 抢占验收已完成，记录见 [延时与抢占验收](rtthread-delay-acceptance.md)。本轮发现 BSP 在同一 ISR 多次调度时覆写原 Context 保存来源，已作软件修正；受控 timer、宿主 timer、Difftest、原生 VCD 资格及普通启动回归通过。未修改生产 RTL。

仍有宿主约 60 Hz 用户态 CPU 时间 timer 与 RT_TICK_PER_SECOND=100 的时间尺度差异；UART 输入是固定字符串，尚非交互输入/接收中断；设备 MEIP 来源仍未接入。date 的 RTC 提示来自 RT BSP 尚未注册 RTC 设备，不表示 NPC RTC MMIO 不存在。设备中断、通用 kcontext、完整 RT API 和同优先级时间片需各自验收。如果后续发现 RTL bug，先给证据与修改意义，不直接修改生产 RTL。
