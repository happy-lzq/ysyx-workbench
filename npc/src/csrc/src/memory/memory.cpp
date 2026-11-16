#include <dpi_export.h>
#include <memory.h>
#include <device.h>
#include <chrono>

// ==================== 128MB 统一物理内存 ====================
uint8_t npc_pmem[PMEM_SIZE];
long npc_img_size = 0;
static bool mmio_accessed = false;   // difftest 跳过标志


// ==================== C++ 辅助函数 ====================

static bool pmem_access_ok(uint32_t addr, int len) {
    if (len <= 0 || len > 4) return false;
    if (addr < PMEM_BASE || addr >= PMEM_END) return false;

    uint32_t offset = addr - PMEM_BASE;
    return offset <= PMEM_SIZE - (uint32_t)len;
}

void pmem_init() {
    memset(npc_pmem, 0, PMEM_SIZE);
}

void pmem_load_bin(const char *path) {
    Assert(path, "load path is NULL");
    FILE *fp = fopen(path, "rb");
    Assert(fp, "cannot open file %s", path);

    fseek(fp, 0, SEEK_END);
    npc_img_size = ftell(fp);
    Assert(npc_img_size >= 0 && (uint64_t)npc_img_size <= PMEM_SIZE,
           "image too large: %ld > PMEM_SIZE(%u)", npc_img_size, PMEM_SIZE);

    fseek(fp, 0, SEEK_SET);
    size_t n = fread(npc_pmem, 1, npc_img_size, fp);
    Assert(n == (size_t)npc_img_size, "fread failed: read %zu, expected %ld", n, npc_img_size);
    fclose(fp);

    printf("Loaded %ld bytes from %s into pmem [0x%08x, 0x%08x)\n",
           npc_img_size, path, PMEM_BASE, PMEM_BASE + (uint32_t)npc_img_size);
}

uint32_t pmem_read(uint32_t addr, int len) {
    Assert(pmem_access_ok(addr,len),"ADDR(0x%08x) IS ERROR!",addr);

    uint32_t offset = addr - PMEM_BASE;
    uint32_t val = 0;
    for (int i = 0; i < len; i++)
        val |= (uint32_t)npc_pmem[offset + i] << (i * 8);
    return val;
}

void pmem_write(uint32_t addr, int len, uint32_t data) {
    Assert(pmem_access_ok(addr,len),"ADDR(0x%08x) IS ERROR!",addr);

    uint32_t offset = addr - PMEM_BASE;
    for (int i = 0; i < len; i++)
        npc_pmem[offset + i] = (data >> (i * 8)) & 0xFF;
}

// ==================== DPI-C: 统一内存读写 ====================

extern "C" {

int dpi_mem_read(int addr, int is_load) {
    uint32_t paddr = (uint32_t)addr;

    // 物理内存 — 字对齐读取（匹配 DMEM 的 word-indexed 行为）
    if (paddr >= PMEM_BASE && paddr < PMEM_END) {
        uint32_t word_addr = paddr & ~3U;  // ~向下对齐4字节边界，
        return (int)pmem_read(word_addr, 4);
    }

    // 组合读路径会被取指和非 load 指令访问；只有真实 data load 才能触发 MMIO skip
    if (!is_load) return 0;

    // MMIO 设备读
    mmio_accessed = true;
    return mmio_read(paddr);
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
                npc_pmem[offset + i] = (wdata >> (i * 8)) & 0xFF;
        }
        return;
    }

    // MMIO 设备写
    mmio_accessed = true;
    mmio_write(paddr,wdata,wmask);
}

int dpi_is_mmio(int addr){
    return mmio_contains(static_cast<uint32_t>(addr)) ? 1 : 0;
}


} 

// ==================== difftest 跳过检测 ====================
bool pmem_mmio_accessed() {
    bool v = mmio_accessed;
    mmio_accessed = false;    // 读取后自动清零，下周期重新检测
    return v;
}

// E transactions execute exactly once in dpi_mem.EXEC. Never truncate a 34-bit
// address into the RV32 memory map. Old RAM word-alignment semantics are kept.
extern "C" int dpi_mem_access(unsigned long long addr, int kind, int size,
                              int device_attr, int data, int mask, int *rdata) {
    *rdata = 0;
    if (addr > UINT32_MAX || kind < 0 || kind > 2 || size < 0 || size > 2) return 1;
    const uint32_t a = static_cast<uint32_t>(addr);
    if (in_pmem(a)) {
        if (device_attr || !pmem_access_ok(a & ~3u, 4)) return 1;
        if (kind == 0 && (size != 2 || (a & 3))) return 1;
        if (kind == 2) dpi_mem_write(static_cast<int>(a), data, mask);
        else *rdata = dpi_mem_read(static_cast<int>(a), kind == 1);
        return 0;
    }
    if (kind == 0 || !device_attr) return 1; // Device is never executable.
    uint32_t value = 0;
    if (!mmio_access(a, unsigned(size), kind == 2, uint32_t(data), uint8_t(mask), &value))
        return 1;
    *rdata = static_cast<int>(value);
    return 0;
}

extern "C" void dpi_mem_fault(int pc, int addr, int kind) {
    fprintf(stderr, "MEMORY ERROR pc=0x%08x addr=0x%08x kind=%d (precise trap requires X)\n",
            unsigned(pc), unsigned(addr), kind);
    npc_sim_state.state = NPC_ABORT;
    npc_sim_state.halt_pc = uint32_t(pc);
    npc_sim_state.halt_ret = -2;
}
