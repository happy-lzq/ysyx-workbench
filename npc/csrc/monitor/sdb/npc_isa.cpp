#include <sdb.h>
#include <memory.h>

// 寄存器名映射
static const char* regs[] = {
    "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
    "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
    "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
    for (int i = 0; i < 32; i++) {
        printf("%-4s: 0x%08x  ", regs[i], npc_gpr(top, i, 0, READ));
        if ((i+1) % 4 == 0) printf("\n");
    }
    // printf("pc : 0x%08x\n", npc_pc(top, 0, READ));
}

word_t isa_reg_str2val(const char *s, bool *success) {

    if (strcmp(s, "$pc") == 0) {
        *success = true;
        return npc_pc(top, 0, READ);
    }
    for (int i = 0; i < 32; i++) {
        if (strcmp(s, regs[i]) == 0) {
            *success = true;
            return npc_gpr(top, i, 0, READ);
        }
        if (s[0] == '$' && s[1] >= '0' && s[1] <= '9' && atoi(s+1) == i) {
            *success = true;
            return npc_gpr(top, i, 0, READ);
        }
    }
    *success = false;
    return 0;
}

word_t vaddr_read(vaddr_t addr, int len) {
    return pmem_read(addr, len);
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
    pmem_write(addr, len, data);
}