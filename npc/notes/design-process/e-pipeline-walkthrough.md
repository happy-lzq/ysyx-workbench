# E 首版：从槽位到握手，按当前代码学习

实现日期：2026-10-18。对应 `npc/vsrc/core_top.v`、四组级间寄存器、`mem_port.v`、`dpi_mem.v`。结果见 [E 验收记录](../cpu-fuzzing/e-acceptance.md)。本文解释当前单笔在途、WB 始终可接收实现，不假设已经有 AXI 或精确访存异常。

## 1. 先区分三个问题

| 问题 | 当前信号 | 含义 |
| --- | --- | --- |
| 槽里有没有指令？ | `if_id_valid` 等 | 状态，可能连续很多拍为 1 |
| 这个槽能不能接下一条？ | `id_allowin` 等 | 当前空，或者旧指令本拍能离开 |
| 旧指令本拍真的离开吗？ | `id_fire` 等 | 有指令、已就绪、下游能接、没有被 kill |

`allowin=1` 不代表一定装入指令；上游可能没有有效输入。此时槽位下一拍变空。

`valid=1` 不代表本拍一定完成，更不代表每拍都要提交一次。

## 2. 从最后一级反推，别先猜 IF 的 ready

### 2.1 WB 是递推起点

本版 WB 每拍可处理一条指令，没有外部 `wb_ready` 阻塞：

```verilog
wire wb_fire = mem_wb_valid && !rst && !faulted && !halt;
```

MEM/WB 寄存器 enable 固定为 1：旧 WB 在本沿提交，本沿同时装入 MEM 的新指令；如果没有新指令，就装入 valid=0。因此旧 WB 不会在 MEM 等待期间反复提交。

**练习：**WB 有指令 A，MEM 的 Load B 还没返回。本沿 A 提交，下一拍 WB 为空，B 仍留在 MEM。

### 2.2 MEM：先判断完成，再判断有没有容量

```verilog
wire mem_complete = !mem_operation ||
                    (mem_issued && mem_resp_valid && !mem_resp_err);
wire mem_allowin = !ex_mem_valid || mem_complete;
```

- MEM 空：可以装入。
- MEM 是普通 ALU/CSR 等非访存指令：结果已经具备，可以送 WB 并装下一条。
- MEM 是 Load/Store：请求已经锁定、成功响应回来，才能送 WB。
- 尚未响应：MEM 保持，不能被下一条覆盖。

这里没有再乘一个 `wb_allowin`，因为本版 WB 始终可接收；也没有再乘 `mem_resp_ready`，因为它在非复位时恒为 1。将来改变这两个前提，必须同步改公式。

实际移交还需要有效、正常运行且不被更老事件 kill：

```verilog
wire mem_fire = !rst && !faulted && !halt && ex_mem_valid &&
                mem_complete && !mem_wb_flush;
```

### 2.3 EX：自己计算完成，也得等 MEM 空位

```verilog
wire ex_allowin = !id_ex_valid || mem_allowin;
```

EX 空时可接 ID；EX 有指令时，只有 MEM 可接，旧 EX 才能离开并给新 ID 腾位置。

**额外问题：**旧 EX 正在用 WB 旁路值，本拍被 MEM 阻塞。下一拍 WB 已经走了，再重算会不会用回旧寄存器值？

所以顶层增加 `ex_saved` 和 `held_*`：第一次受阻时保存已算好的 ALU、Store 数据、CSR 读写值、分支决定与目标。释放前使用保存结果。它是 EX 内部保持状态，没有增加一个流水级。

### 2.4 ID：既要解开依赖，又要 EX 有容量

```verilog
wire id_allowin = !if_id_valid || (!id_stall_req && ex_allowin);
```

这里对应你最初的三种判断：

| IF/ID 状态 | ID 冒险 | EX 可接 | `id_allowin` |
| --- | --- | --- | --- |
| 空 | 任意 | 任意 | 1 |
| 有旧指令 | 无 | 是 | 1，旧指令离开，本沿可换新指令 |
| 有旧指令 | 有，或 EX 不能接 | — | 0，完整保留旧槽 |

之前只写 `!if_id_valid || !id_stall_req`，漏掉的是 **EX 可能因为 MEM 等待而不能接收**。

`id_fire` 还排除复位、故障、停机和年龄相关 kill。JAL 不能杀死自己进入 EX 的行为；更老 EX/WB redirect 可以杀死 ID，具体由 `id_ex_flush` 区分。

## 3. 级间寄存器如何实现上述决定

以 IF/ID 为例，实际连接是：

```verilog
.enable(id_allowin),
.flush(if_id_flush),
.if_id_valid_in(if_capture_fire)
```

更新优先级：

1. reset：清空。
2. flush：清 valid，原载荷不必全清零。
3. enable：装入上游 valid 和完整载荷。
4. 否则：valid 和载荷一起保持。

因此不需要用很多数据选择器让每个字段各自“选自己”。保持行为由寄存器 enable 统一表达。

**例子：**旧 ID 本拍进入 EX，但 IF 没返回，enable=1、输入 valid=0，IF/ID 正确变空。不能保留旧 valid，否则旧指令下一拍又执行一次。

## 4. 再回来看 `if_resp_ready`

正常活响应时，它等于 IF/ID 的容量判断；作废响应时，它代表“允许丢弃并结束通信”：

```verilog
wire if_resp_ready = !rst &&
    (!if_live || redirect_fire || id_allowin || faulted || halt);
```

真正装入 IF/ID 必须另外判断：

```verilog
wire if_capture_fire = if_resp_fire && if_live &&
                       !redirect_fire && !if_resp_err && !faulted && !halt;
```

所以 **ready 可以为 1，但不装入指令**。这不是矛盾：旧路径的响应仍必须被接收端结束掉，否则共享端口永远占着。`if_live` 记录请求所属路径是否还有效，不能用当前 PC 是否相同代替。

### 为什么发 IF 请求也看容量？

本版没有额外的取指返回队列：

```verilog
if_req = !rst && !halt && !faulted && !if_busy &&
         id_allowin && !redirect_fire;
```

发请求时就预留 IF/ID 空位。如果当时有旧 ID，它在同沿离开；之后没有第二个 IF 请求可填这个槽，直到当前响应回来。这样避免“IF 响应占住共享端口，但 ID 被等待同一端口的老 MEM 阻塞”的环形等待。

## 5. 一笔取指经过哪些沿

下表以默认请求等待 0、响应延迟 1 为例；每行是上升沿发生的事件，不是源码语句执行顺序。

| 沿 | mem_port / dpi_mem | 流水线行为 |
| --- | --- | --- |
| A | IDLE 选 IF，锁存地址，进入 OFFER | `if_req_fire=1`；busy/live 置 1，预留 IF/ID |
| B | OFFER 的请求被后端接受；port 进 WAIT，后端进 EXEC | PC 仍是这次请求的地址，没有指令进入 ID |
| C | EXEC 调用一次 DPI，锁存结果，后端进 RESP | 结果在此沿之后有效 |
| D | 响应握手，port/后端释放事务 | 活响应进入 IF/ID；PC 顺序加 4；busy 清零 |

若 C 前出现 redirect，PC 立即转目标、live 清零；D 只排空旧结果。之后再从新 PC 发请求。不是取消内存里已经接收的操作。

## 6. Load 的区别

EX/MEM 槽保存 Load 的 PC、rd、地址和控制。`mem_req_fire` 后置 `mem_issued`，防止同一槽的 `mem_read=1` 连续造成多次请求。

等待中：MEM 保持；EX/ID 逐级反压；更老 WB 排空。

响应成功沿：`mem_resp_fire` 与 `mem_fire` 成立，Load 结果移入 MEM/WB；下一沿由 WB 写回。Load-use 检查覆盖 ID/EX、EX/MEM 中尚未离开的 Load，不能再固定只插一拍 bubble。

错误响应：当前实现停止仿真，不把错误数据退休。异常处理程序跳转留给 X。

## 7. 为什么 Store/设备需要等最老

D 请求在 WB 空时才许可。锁定之后，`mem_issued` 保护到移交 WB；`wb_memory` 再保护 WB 退休沿。后到 IRQ 等保护结束再接收，恢复点不重执行已写设备的指令。

设备读数据在后端 EXEC 取样，键盘出队在 MEM 接收事件消费一次，WB 的 MMIO 标志随后用于 Difftest 同步。请求、取样、消费、退休是不同沿上的事件。

## 8. 建议的学习与实验顺序

1. 看 `core_top.v` 的 `wb_fire` 到 `id_fire`，手画“一条 Load 等待、老 WB 排空”。
2. 看 `if_id_reg.v` 的 enable/flush；区分保持、变空、换入。
3. 看 `mem_port.v`：只在 IDLE 仲裁，之后使用保存字段。
4. 看 `dpi_mem.v`：延迟只是 EXEC 多等几拍；DPI 只调用一次。
5. 看 `if_busy/if_live/if_capture_fire`，画响应与 redirect 同拍。
6. 看 `ex_saved`，理解旁路源离开后的保持。
7. 最后看 IRQ/D 副作用许可和独立验收，不从复杂碰撞开始学。

可从 `bash tests/run_e_protocol.sh` 开始复现，再运行 `run_e_pipeline.sh`。先对照测试里的确定性输入、期望和计数，再看波形；程序 PASS 本身不是所有微架构行为的证明。
