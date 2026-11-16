# Interactive NPC shell environment, matching nanhu-dfz/env.sh.
export WORKSPACE=/workspace/ysyx-workbench
export AM_HOME=$WORKSPACE/abstract-machine
export NPC_HOME=$WORKSPACE/npc
export NEMU_HOME=$WORKSPACE/nemu
export CROSS_COMPILE=riscv64-unknown-elf-

force_color_prompt=yes
if [ -x /usr/bin/tput ] && tput setaf 1 >&/dev/null; then
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
fi
alias ..='cd ../'
