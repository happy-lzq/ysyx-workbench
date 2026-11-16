#ifndef __NPC_MMIO_H__
#define __NPC_MMIO_H__

#include <stdbool.h>
#include <stdint.h>
#define SERIAL_ADDR 0xa00003f8
#define RTC_ADDR 0xa0000048
struct MMIODevice;

typedef uint32_t (*mmio_read_cb)(MMIODevice *dev, uint32_t offset);
typedef void (*mmio_write_cb)(MMIODevice *dev, uint32_t offset,
                              uint32_t data, uint8_t wmask);
typedef void (*mmio_read_accept_cb)(MMIODevice *dev, uint32_t offset);

struct MMIODevice {
    const char *name;
    uint8_t *space;
    uint32_t addr_start;
    uint32_t addr_end;
    uint32_t wmask;
    mmio_read_cb read;
    mmio_write_cb write;
    mmio_read_accept_cb read_accept;
};

// E: validates the entire byte range; payload uses word-relative lanes.
bool mmio_access(uint32_t addr, unsigned size, bool write,
                 uint32_t wdata, uint8_t wmask, uint32_t *rdata);
void init_mmio();
bool mmio_contains(uint32_t addr);
void mmio_read_accept(uint32_t addr);
void add_mmio_device(
    const char *name,
    void *space, 
    uint32_t addr, 
    uint32_t len,
    uint8_t wmask, 
    mmio_read_cb read, 
    mmio_write_cb write,
    mmio_read_accept_cb read_accept = nullptr
);

uint32_t mmio_read(uint32_t addr);
void mmio_write(uint32_t addr, uint32_t data, uint8_t wmask);

uint32_t mmio_space_read(MMIODevice *dev, uint32_t offset);
void mmio_space_write(MMIODevice *dev, uint32_t offset,
                      uint32_t data, uint8_t wmask);

#endif
