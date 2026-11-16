# AM 中断测试：构建问题与短时验收

## 资源符号声明

`am-kernels/tests/am-tests` 会编译所有测试的 C 文件，再通过 `mainargs` 选择运行入口。选择 `i` 仍会编译 `mp4.c`。

MP4 汇编资源通过 `test_video_payload` / `test_video_payload_end` 等标签表示起止地址。原 C 声明为单个 `uint8_t`，编译器据此推断视频资源大小只有 1 字节，GCC 在帧读取循环报 `-Werror=array-bounds`。修正为未指定长度的外部字节数组，资源长度使用 `uintptr_t` 地址差计算。没有关闭警告，没有修改资源布局、播放算法或生产 RTL。

## 2026-10-04 验收记录

容器：`ysyx-npc`；工作区：`/workspace/ysyx-workbench`。

证据目录：`npc/build/am-intr-acceptance-EhXzqM`。保留构建前后日志、退出码、输入镜像/ELF、NPC 配置、输入与模拟器/REF 的 SHA-256，以及运行前 RTL SHA-256。

修改前执行 `make ARCH=riscv32I-npc image mainargs=i git_commit=`，复现 `mp4.c` 三处数组越界编译错误，退出码 2。

修改后：

```bash
source /workspace/ysyx-workbench/npc/container/env.sh
cd /workspace/ysyx-workbench/am-kernels/tests/am-tests
make ARCH=riscv32I-npc insert-arg mainargs=i git_commit=
cd "$NPC_HOME"
make sim git_commit=
```

镜像构建与模拟器构建均退出码 0。复制 AM 镜像到证据目录后执行：

```bash
cd /workspace/ysyx-workbench/npc
task_dir=build/am-intr-acceptance-EhXzqM
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy timeout -k 2s 20s \
  ./build/sim --log-limit 1000 \
  -i "$task_dir/intr.bin" -e "$task_dir/intr.elf" \
  -d "$NEMU_HOME/build/riscv32-nemu-interpreter-so"
```

运行配置为 `CONFIG_DIFFTEST=y`、`CONFIG_HAS_TIMER=y`；DIFF_LOG_ENABLE、ITRACE、WAVE 关闭。观察到启动说明及连续的 `y`、`t` 输出，未报告 Difftest mismatch。该程序无限循环，预设 20 秒超时结束，运行退出码为 124；不能将其称为完整程序正常退出或全量中断回归 PASS。RTL 20 个文件的运行前后 SHA-256 均一致。

`y` 来自 AM 的 EVENT_YIELD 处理，`t` 来自 EVENT_IRQ_TIMER 处理。观察支持本次窗口内 yield、timer trap/返回与提交级 Difftest 路径能够共同运行。宿主 timer 使用用户态 CPU 时间，测试结果不能作为真实硬件中断周期的性能证据。

边界：未运行 MP4 实际音视频播放、RT-Thread、设备外部中断 MEIP、所有 MMIO kill 场景或在途 CSR 读与异步 pending 更新的定向冲突；本次没有采集 VCD，也没有进行新的插桩波形资格验证。
