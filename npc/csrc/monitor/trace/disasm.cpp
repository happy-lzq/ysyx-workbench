/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <dlfcn.h>
#include <inttypes.h>
#include <capstone/capstone.h>
#include <npc.h>

static size_t (*cs_disasm_dl)(csh handle, const uint8_t *code,
    size_t code_size, uint64_t address, size_t count, cs_insn **insn);
static void (*cs_free_dl)(cs_insn *insn, size_t count);

static csh handle;

void init_disasm() {
  void *dl_handle;
  dl_handle = dlopen("tools/capstone/repo/libcapstone.so.5", RTLD_LAZY);
  Assert(dl_handle, "can not open capstone: %s", dlerror());

  cs_err (*cs_open_dl)(cs_arch arch, cs_mode mode, csh *handle) = NULL;
  cs_open_dl = (cs_err (*)(cs_arch, cs_mode, csh *))dlsym(dl_handle, "cs_open");
  Assert(cs_open_dl, "can not find capstone symbol cs_open");

  cs_disasm_dl = (size_t (*)(csh, const uint8_t *, size_t, uint64_t, size_t, cs_insn **))dlsym(dl_handle, "cs_disasm");
  Assert(cs_disasm_dl, "can not find capstone symbol cs_disasm");

  cs_free_dl = (void (*)(cs_insn *, size_t))dlsym(dl_handle, "cs_free");
  Assert(cs_free_dl, "can not find capstone symbol cs_free");

  // NPC 仅支持 RISC-V，无需 NEMU 的 MUXDEF 多 ISA 分支
#ifdef CONFIG_ISA_riscv64
  cs_arch arch = CS_ARCH_RISCV;
  cs_mode mode = (cs_mode)(CS_MODE_RISCV64 | CS_MODE_RISCVC);
#else
  cs_arch arch = CS_ARCH_RISCV;
  cs_mode mode = (cs_mode)(CS_MODE_RISCV32 | CS_MODE_RISCVC);
#endif
  int ret = cs_open_dl(arch, mode, &handle);
  Assert(ret == CS_ERR_OK, "capstone open failed");
}

void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte) {
  cs_insn *insn;
  size_t count = cs_disasm_dl(handle, code, nbyte, pc, 0, &insn);
  Assert(count == 1,
      "failed to disassemble at pc=0x%08" PRIx64
      ", bytes=%02x %02x %02x %02x",
      pc, code[0], code[1], code[2], code[3]);
  int ret = snprintf(str, size, "%s", insn->mnemonic);
  if (insn->op_str[0] != '\0') {
    snprintf(str + ret, size - ret, "\t%s", insn->op_str);
  }
  cs_free_dl(insn, count);
}
