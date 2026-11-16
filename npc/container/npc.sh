# Source this file from an interactive Bash startup file to get `npc`.
npc() {
    local repo_dir
    repo_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd -P)" || return

    local container_name=ysyx-npc
    local image=ysyx-npc:local
    local workspace=/workspace/ysyx-workbench
    local persistent_home="$HOME/.cache/ysyx-npc-home"
    local identity="$(id -u):$(id -g)"
    local -a npc_env=(
        --env "AM_HOME=$workspace/abstract-machine"
        --env "NPC_HOME=$workspace/npc"
        --env "NEMU_HOME=$workspace/nemu"
        --env "CROSS_COMPILE=riscv64-unknown-elf-"
    )
    local actual expected

    if [[ "$(id -un)" != lzq ]]; then
        echo "The NPC container entry is configured for the lzq account." >&2
        return 1
    fi
    if [[ ! -f "$repo_dir/npc/Makefile" ||
          ! -d "$repo_dir/npc/src/vsrc/src" ||
          ! -d "$repo_dir/npc/src/csrc/src" ]]; then
        echo "NPC workspace is incomplete: $repo_dir" >&2
        return 1
    fi
    if ! docker image inspect "$image" >/dev/null 2>&1; then
        echo "NPC image is missing. Build it with:" >&2
        echo "docker build -t $image -f $repo_dir/npc/container/Dockerfile $repo_dir/npc/container" >&2
        return 1
    fi

    if docker container inspect "$container_name" >/dev/null 2>&1; then
        actual="$(docker inspect --format '{{.Config.User}}|{{.Config.Image}}|{{range .Mounts}}{{if eq .Destination "/workspace/ysyx-workbench"}}{{.Source}}{{end}}{{end}}|{{range .Mounts}}{{if eq .Destination "/home/lzq"}}{{.Source}}{{end}}{{end}}' "$container_name")" || return
        expected="$identity|$image|$repo_dir|$persistent_home"
        if [[ "$actual" != "$expected" ]]; then
            echo "Existing $container_name configuration differs; refusing to replace it: $actual" >&2
            return 1
        fi
        if [[ "$(docker inspect --format '{{.State.Running}}' "$container_name")" != true ]]; then
            docker start "$container_name" >/dev/null || return
        fi
    else
        mkdir -p "$persistent_home" || return
        docker run -d --user "$identity" --env HOME=/home/lzq \
            "${npc_env[@]}" \
            --volume /etc/passwd:/etc/passwd:ro \
            --volume /etc/group:/etc/group:ro \
            --volume "$persistent_home:/home/lzq" \
            --volume "$repo_dir:$workspace" \
            --workdir "$workspace/npc" \
            --hostname ysyx-npc --name "$container_name" \
            "$image" sleep infinity >/dev/null || return
    fi

    if (( $# == 0 )); then
        docker exec -it --user "$identity" "${npc_env[@]}" \
            --workdir "$workspace/npc" \
            "$container_name" /bin/bash --rcfile "$workspace/npc/container/env.sh"
    else
        docker exec -i --user "$identity" "${npc_env[@]}" \
            --workdir "$workspace/npc" \
            "$container_name" "$@"
    fi
}
