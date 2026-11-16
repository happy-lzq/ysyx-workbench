#!/usr/bin/env bash
set -euo pipefail

npc_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd -P)"
cd "$npc_dir"
source "$npc_dir/scripts/source_paths.sh"

verilator --cc --exe --build --trace \
    --top-module core_top \
    --Mdir obj_dir \
    -j "${NPC_BUILD_JOBS:-2}" \
    -I"$npc_rtl_include_dir" -Iinclude \
    -CFLAGS "-g -O2 -I$npc_cpp_include_dir -I$npc_dir/include -I$npc_dir/obj_dir -I$npc_dir/include/generated -I$npc_dir/tools/capstone/repo/include $(pkg-config --cflags sdl2)" \
    -LDFLAGS "-ldl -lreadline -lSDL2" \
    "${npc_rtl_sources[@]}" "${npc_cpp_sources[@]}"

mkdir -p build
install -m 755 obj_dir/Vcore_top build/sim
echo "NPC simulator: $npc_dir/build/sim"
