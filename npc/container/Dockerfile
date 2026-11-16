ARG BASE_IMAGE=xiangshan-nanhu:local
FROM ${BASE_IMAGE}

# Keep the NPC-only bare-metal cross toolchain in this derived image. This does
# not modify the XiangShan base image or any container created from that image.
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
      gcc-riscv64-unknown-elf \
      binutils-riscv64-unknown-elf \
      libnewlib-dev && \
    rm -rf /var/lib/apt/lists/*

# Reuse the XiangShan Verilator/C++ toolchain. Mount source at runtime.
WORKDIR /workspace/ysyx-workbench/npc
CMD ["/bin/bash"]
