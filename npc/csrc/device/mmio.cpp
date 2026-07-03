#include <mmio.h>
#include <device.h>
#include <memory.h>
#include <inttypes.h>

#define FMT_PADDR "0x%08" PRIx32
#define NPC_IO_SPACE_MAX (64 * 1024)                // 64k
#define NPC_PAGE_SIZE  4096
#define NPC_PAGE_MASK  (NPC_PAGE_SIZE - 1)
#define NR_MAP 16

static MMIODevice maps[NR_MAP] = {};
static int nr_map = 0;

static uint8_t *io_space = NULL;   // 预分配池
static uint8_t *p_space = NULL;    // 当前分配指针

void init_map() {
    io_space = (uint8_t*)malloc(NPC_IO_SPACE_MAX);
    assert(io_space);
    p_space = io_space;
}

uint8_t* new_space(int size) {
    uint8_t *p = p_space;

    // 页对齐：把 size 向上取整到 4KB 的整数倍
    size = (size + NPC_PAGE_MASK) & ~NPC_PAGE_MASK;

    p_space += size;
    assert(p_space - io_space <= NPC_IO_SPACE_MAX);

    memset(p, 0, size);    // 清零整页
    return p;
}

static void report_mmio_overlap(const char *name1, paddr_t l1, paddr_t r1,
                                const char *name2, paddr_t l2, paddr_t r2) {
  panic("MMIO region %s@[" FMT_PADDR ", " FMT_PADDR "] is overlapped "
               "with %s@[" FMT_PADDR ", " FMT_PADDR "]", name1, l1, r1, name2, l2, r2);
}
void add_mmio_device(const char *name,void *space, uint32_t addr, uint32_t len,
                    uint8_t wmask, mmio_read_cb read, mmio_write_cb write){
    paddr_t left = addr, right = addr + len - 1;
    // 检查 addr 范围不与PMEM重叠
    Assert(!(in_pmem(left) || in_pmem(right)),"the device addr=0x%08x of %s belong to PMEM",addr,name);
    // 检查 addr 范围不与已注册设备重叠
    for (int i = 0; i < nr_map; i++) {
        if (left <= maps[i].addr_end && right >= maps[i].addr_start) {
          report_mmio_overlap(name, left, right, maps[i].name, maps[i].addr_start, maps[i].addr_end);
        }
    }
    // device mmio 绑定
    maps[nr_map]= { .name       = name,
                    .space      = (uint8_t*)space,
                    .addr_start = left,
                    .addr_end   = right,
                    .wmask      = wmask,
                    .read       = read,
                    .write      = write,
    };
    nr_map++;

}
uint32_t mmio_space_read(MMIODevice *dev, uint32_t offset){
    uint32_t val = 0;
    // 数组存放数据，高位在数组下标更大的地址，需要拼接
    for (int i = 0; i < 4; i++){
        val |= (uint32_t)dev->space[offset + i] << (i * 8);
    }
    return val;
}
uint32_t mmio_read(uint32_t addr){
    int i = 0;
    for ( i ; i < nr_map; i++){
        if (addr >= maps[i].addr_start && addr <=maps[i].addr_end){
            uint32_t offset = addr - maps[i].addr_start ;
            if (maps[i].read){
                return maps[i].read(&maps[i],offset);
            }else {
                return mmio_space_read(&maps[i],offset);
            }
        }
    }
    Assert(!(i==nr_map),"mmio_read addr[0x%08x] is illegal !",addr);
    return 0;
}
void mmio_space_write(MMIODevice *dev,uint32_t offset,uint32_t data,uint8_t wmask){
    for (int i = 0; i < 4; i++){
        if (wmask &(1<<i)){
           dev->space[offset + i] = (data >> (i * 8)) & 0xFF;
        }
    }
    
}
void mmio_write(uint32_t addr, uint32_t data,uint8_t wmask){
    int i = 0;
    for ( i; i < nr_map; i++){
        if (addr >= maps[i].addr_start && addr <=maps[i].addr_end){
            uint32_t offset = addr - maps[i].addr_start ;
            mmio_space_write(&maps[i], offset, data, wmask);
            if (maps[i].write){
                maps[i].write(&maps[i],offset,data,wmask);
            }
            return ;
        }
    }
    Assert(!(i==nr_map),"mmio_write addr[0x%08x] is illegal !",addr);
}


void init_mmio(){
    init_map();
    // 1. 串口：8 字节 space (8250 UART 8 个寄存器)
    #ifdef CONFIG_HAS_SERIAL
    add_mmio_device("serial",new_space(8),CONFIG_SERIAL_MMIO,8,
                    0x1,NULL,serial_write_handler);  // 读无回调(space直读)，写有回调
    #endif
    // 2. RTC：8 字节 space
    #ifdef CONFIG_HAS_TIMER
    add_mmio_device("rtc", new_space(8), CONFIG_RTC_MMIO, 8,
                    0xF, rtc_read_handler, NULL);   // 读有回调，写无回调(space直写)
    #endif
    // 3. VGA：控制寄存器 8 字节 + 帧缓冲区
    #ifdef CONFIG_HAS_VGA
    init_vga();
    #endif
    // 4. 键盘：4 字节 space（读回调返回键码）
    #ifdef CONFIG_HAS_KEYBOARD
    init_i8042();
    #endif
    // 5. 声卡：控制寄存器 + 流缓冲区
    #ifdef CONFIG_HAS_AUDIO
    init_audio();
    #endif
}

