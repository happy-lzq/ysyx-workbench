#!/usr/bin/env bash
# Export container-only headers for VS Code C++ IntelliSense in an SSH window.
set -euo pipefail

npc_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd -P)"
repo_dir="$(cd "$npc_dir/.." && pwd -P)"
cache_dir="$npc_dir/.vscode-includes"
image=ysyx-npc:local

if ! docker image inspect "$image" >/dev/null 2>&1; then
    echo "NPC image is missing: $image" >&2
    exit 1
fi
mkdir -p "$cache_dir"
docker run --rm --user "$(id -u):$(id -g)" \
    --mount "type=bind,src=$repo_dir,dst=$repo_dir" \
    --workdir "$repo_dir" \
    "$image" bash -c '
        set -euo pipefail
        cache_dir="$1"
        mkdir -p "$cache_dir/verilator/include" "$cache_dir/SDL2" \
                 "$cache_dir/arch/SDL2" "$cache_dir/readline"
        cp -a /usr/local/share/verilator/include/. "$cache_dir/verilator/include/"
        cp -a /usr/include/SDL2/. "$cache_dir/SDL2/"
        cp -a /usr/include/x86_64-linux-gnu/SDL2/. "$cache_dir/arch/SDL2/"
        cp -a /usr/include/readline/. "$cache_dir/readline/"
    ' _ "$cache_dir"
printf 'VS Code C++ header cache: %s\n' "$cache_dir"
