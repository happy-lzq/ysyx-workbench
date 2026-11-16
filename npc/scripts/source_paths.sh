#!/usr/bin/env bash
# Source this file from NPC build/test scripts; paths are absolute and recursive.
npc_source_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd -P)"
npc_rtl_dir="$npc_source_root/src/vsrc/src"
npc_rtl_include_dir="$npc_source_root/src/vsrc/include"
npc_cpp_dir="$npc_source_root/src/csrc/src"
npc_cpp_include_dir="$npc_source_root/src/csrc/include"

mapfile -t npc_rtl_sources < <(find "$npc_rtl_dir" -type f -name '*.v' | LC_ALL=C sort)
mapfile -t npc_rtl_headers < <(find "$npc_rtl_include_dir" -type f -name '*.vh' | LC_ALL=C sort)
mapfile -t npc_cpp_sources < <(find "$npc_cpp_dir" -type f -name '*.cpp' | LC_ALL=C sort)

if (( ${#npc_rtl_sources[@]} == 0 || ${#npc_cpp_sources[@]} == 0 )); then
    echo "NPC source tree is incomplete: $npc_source_root/src" >&2
    return 1
fi
