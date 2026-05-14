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

#include <isa.h>
#include <cpu/cpu.h>
#include <memory/paddr.h>
#include <utils.h>
#include <difftest-def.h>
// 调用参考模型ref的内存拷贝接口——>用来把 DUT 的镜像同步到 REF
void (*ref_difftest_memcpy)(paddr_t addr, void *buf, size_t n, bool direction) = NULL;
// 调用参考模型 ref的寄存器拷贝接口——>既能 DUT -> REF，也能 REF -> DUT
void (*ref_difftest_regcpy)(void *dut, bool direction) = NULL;
// 让参考模型执行 n 条指令
void (*ref_difftest_exec)(uint64_t n) = NULL;
// 让参考模型注入异常中断
void (*ref_difftest_raise_intr)(uint64_t NO) = NULL;

#ifdef CONFIG_DIFFTEST

static bool is_skip_ref = false;   //标记“本条指令是否要让 REF 正常执行并比较”
static int skip_dut_nr_inst = 0;   //允许 DUT 继续跑若干条，直到 PC 追上 REF

void difftest_skip_ref() {
  is_skip_ref = true;
  skip_dut_nr_inst = 0;
}

void difftest_skip_dut(int nr_ref, int nr_dut) {
  skip_dut_nr_inst += nr_dut;

  while (nr_ref -- > 0) {
    ref_difftest_exec(1);
  }
}

void init_difftest(char *ref_so_file, long img_size, int port) {
  assert(ref_so_file != NULL);

  void *handle;
  handle = dlopen(ref_so_file, RTLD_LAZY);                   //打开参考模型动态库
  assert(handle);

  ref_difftest_memcpy = dlsym(handle, "difftest_memcpy");    //取出 
  assert(ref_difftest_memcpy);

  ref_difftest_regcpy = dlsym(handle, "difftest_regcpy");
  assert(ref_difftest_regcpy);

  ref_difftest_exec = dlsym(handle, "difftest_exec");
  assert(ref_difftest_exec);

  ref_difftest_raise_intr = dlsym(handle, "difftest_raise_intr");
  assert(ref_difftest_raise_intr);

  void (*ref_difftest_init)(int) = dlsym(handle, "difftest_init");
  assert(ref_difftest_init);

  Log("Differential testing: %s", ANSI_FMT("ON", ANSI_FG_GREEN));
  Log("The result of every instruction will be compared with %s. "
      "This will help you a lot for debugging, but also significantly reduce the performance. "
      "If it is not necessary, you can turn it off in menuconfig.", ref_so_file);

  ref_difftest_init(port);     //初始化参考模型
  //镜像从 RESET_VECTOR 开始同步到 REF
  ref_difftest_memcpy(RESET_VECTOR, guest_to_host(RESET_VECTOR), img_size, DIFFTEST_TO_REF);
  //把 DUT 当前寄存器同步到 REF
  ref_difftest_regcpy(&cpu, DIFFTEST_TO_REF);
}

static void checkregs(CPU_state *ref, vaddr_t pc) {
  if (!isa_difftest_checkregs(ref, pc)) {
    nemu_state.state = NEMU_ABORT;
    nemu_state.halt_pc = pc;
    isa_reg_display();
  }
}
/*
1、负责“加载参考模型、驱动参考模型执行、取回状态并触发比较”
*/
void difftest_step(vaddr_t pc, vaddr_t npc) {
  CPU_state ref_r;
  // 
  if (skip_dut_nr_inst > 0) {
    ref_difftest_regcpy(&ref_r, DIFFTEST_TO_DUT);
    if (ref_r.pc == npc) {
      skip_dut_nr_inst = 0;
      checkregs(&ref_r, npc);
      return;
    }
    skip_dut_nr_inst --;
    if (skip_dut_nr_inst == 0)
      panic("can not catch up with ref.pc = " FMT_WORD " at pc = " FMT_WORD, ref_r.pc, pc);
    return;
  }

  if (is_skip_ref) {
    ref_difftest_regcpy(&cpu, DIFFTEST_TO_REF);
    is_skip_ref = false;
    return;
  }

  ref_difftest_exec(1);
  ref_difftest_regcpy(&ref_r, DIFFTEST_TO_DUT);

  checkregs(&ref_r, pc);
}
#else
void init_difftest(char *ref_so_file, long img_size, int port) { }
#endif
