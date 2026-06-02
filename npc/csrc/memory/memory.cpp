#include <cstdio>
#include <cstring>
#include <cassert>
#include <memory.h>
#include <device.h>

// ==================== 128MB 统一物理内存 ====================
uint8_t npc_pmem[PMEM_SIZE];
long npc_img_size = 0;

// ==================== C++ 辅助函数 ====================

void pmem_init() {
    memset(npc_pmem, 0, PMEM_SIZE);
}

void pmem_load_bin(const char *path) {
    Assert(!path,"load addr is NULL ");
    FILE *fp = fopen(path, "rb");
    assert(fp);

    fseek(fp, 0, SEEK_END);
    npc_img_size = ftell(fp);
    assert(npc_img_size <= PMEM_SIZE);

    fseek(fp, 0, SEEK_SET);
    size_t n = fread(npc_pmem, 1, npc_img_size, fp);
    assert(n == (size_t)npc_img_size);
    fclose(fp);

    printf("Loaded %ld bytes from %s into pmem [0x%08x, 0x%08lx)\n",
           npc_img_size, path, PMEM_BASE, PMEM_BASE + npc_img_size);
}

long pmem_img_size() {
    return npc_img_size;
}

uint32_t pmem_read(uint32_t addr, int len) {
    uint32_t offset = addr - PMEM_BASE;
    if (offset >= PMEM_SIZE) return 0;

    uint32_t val = 0;
    for (int i = 0; i < len; i++)
        val |= (uint32_t)npc_pmem[offset + i] << (i * 8);
    return val;
}

void pmem_write(uint32_t addr, int len, uint32_t data) {
    uint32_t offset = addr - PMEM_BASE;
    if (offset >= PMEM_SIZE) return;

    for (int i = 0; i < len; i++)
        npc_pmem[offset + i] = (data >> (i * 8)) & 0xFF;
}

// ==================== DPI-C: 统一内存读写 ====================

extern "C" {

int dpi_mem_read(int addr) {
    uint32_t paddr = (uint32_t)addr;

    // 物理内存 — 字对齐读取（匹配 DMEM 的 word-indexed 行为）
    if (paddr >= PMEM_BASE && paddr < PMEM_END) {
        uint32_t word_addr = paddr & ~3U;         // ← 字对齐！
        uint32_t offset = word_addr - PMEM_BASE;
        return *(uint32_t *)(npc_pmem + offset);
    }

    // MMIO 设备读 — 精确地址（设备寄存器对齐）
    switch (paddr) {
        case 0xa00003f8: return 0;           // UART 只写设备
        // case 0xa0000048: return rtc_lo(); // RTC（后续扩展）
        default: return 0;
    }
}

void dpi_mem_write(int addr, int data, int wmask) {
    uint32_t paddr = (uint32_t)addr;
    uint32_t wdata = (uint32_t)data;

    // 物理内存 — 字对齐写入（wmask 相对于字对齐基址）
    if (paddr >= PMEM_BASE && paddr < PMEM_END) {
        uint32_t word_addr = paddr & ~3U;         // ← 字对齐！与 dmem 行为一致
        uint32_t offset = word_addr - PMEM_BASE;
        for (int i = 0; i < 4; i++) {
            if (wmask & (1 << i))
                npc_pmem[offset + i] = (data >> (i * 8)) & 0xFF;
        }
        return;
    }

    // MMIO 设备写
    switch (paddr) {
        case 0xa00003f8:  // UART (NEMU 兼容地址)
            if (wmask & 0x1)
                npc_serial_putc(wdata & 0xFF);
            break;
        // case 0xa0000048: ... // RTC（后续扩展）
        default: break;
    }
}

}  // extern "C"
