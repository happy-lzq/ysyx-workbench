# 当前十个 CSR 的 M-only 字段合同

适用配置：RV32IM/Zicsr 子集、仅 M 模式、IALIGN=32、无 F/V/S/U/H、无自定义状态扩展、无机器软件中断源和本地自定义中断源。依据 Machine-Level ISA 1.13 与 Zicsr 2.0，约束当前已实现的十个地址，不表示已完成整个特权架构。

采用软件写入时约束。CSR 读、trap 目标和 MRET 使用同一已约束的存储值。未实现模式不通过软件写值伪造。

| 地址 | 名称 | 软件写入行为 |
| --- | --- | --- |
| 300 | mstatus | MIE[3]/MPIE[7] 可写；MPP[12:11]=11；其他位在本配置为零。无 S/U、F/V、自定义状态或相关可选扩展 |
| 304 | mie | MTIE[7]/MEIE[11] 可写；其他位为零 |
| 305 | mtvec | 仅 Direct；保留 BASE[31:2]，MODE[1:0]=00。未支持 MODE 映射到 Direct，是本实现的 WARL 选择 |
| 340 | mscratch | 完整 32 位读写 |
| 341 | mepc | 保留 [31:2]，低两位为零；只适用于固定 IALIGN=32 |
| 342 | mcause | 完整 32 位存储，保存支持的完整原因编码；Exception Code 是 WLRL，不增加软件写原因值白名单或新异常 |
| 343 | mtval | 完整 32 位存储；保留非法指令编码和未来故障地址低位，不按 PC 对齐 |
| 344 | mip | MTIP[7]/MEIP[11] 由当前仿真设备路径控制；CSR 软件写保持这两位，其他位零。CSR 地址仍可访问，字段只读不产生整 CSR 只读异常 |
| B00 | mcycle（本地名 mcyclel） | 写 64 位计数器低 32 位，保持高 32 位 |
| B80 | mcycleh | 写 64 位计数器高 32 位，保持低 32 位 |

计数器正常每核心周期加一；本实现选择有效的计数器软件写入优先于本周期整个逻辑计数器的自增，未被寻址的另一半保持写入前值。reset 优先，已有 trap/MRET 优先于普通 CSR 写。代码使用单个 64 位 next 值，避免整体与切片的重叠非阻塞赋值。阻止提交的写不能抑制正常计数。

NEMU 六类 CSR 指令统一调用地址对应的写函数，独立实现以上字段规则，保留 rd/rs1/zimm 的读写抑制语义。REF 无 DUT 周期概念，不把每条 REF 指令当成一个 DUT 周期；周期计数器的时钟及 GPR 读出 Difftest 对齐不在本次软件写合同内，单独记录限制。

访存接口、流水级数量、ready/valid、redirect、实际中断源协议不改变。本项目当前 trap 自动清 pending 和 C++ backdoor 不等于真实 mtime/mtimecmp、PLIC；本次不顺便改写这些机制。

验证：先扩展 CSR/译码单元测试并观察失败，再实现规则；独立 REF 指令测试覆盖六类操作、全部十个地址、x0/zimm 抑制和非法地址；生产仿真器做定向字段程序 Difftest、CPU35 和已有异常回归。计数器以原生模块时序测试覆盖两个半部、溢出、写/自增优先级。每次使用独立产物目录；保存输入摘要和结果。

参考：[Machine-Level ISA](https://docs.riscv.org/reference/isa/v20260120/priv/machine.html)、[CSR 字段规则](https://docs.riscv.org/reference/isa/v20260120/priv/priv-csrs.html)、[Zicsr](https://docs.riscv.org/reference/isa/v20260120/unpriv/zicsr.html)。
