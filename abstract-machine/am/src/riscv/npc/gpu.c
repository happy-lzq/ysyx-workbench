#include <am.h>
#include <riscv/riscv.h>

#define SYNC_ADDR (NPC_VGA_CTL_ADDR + 4)
#define _16_bits 0xffff

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint32_t cfg_val = inl(NPC_VGA_CTL_ADDR);
  int w = (cfg_val >> 16) & _16_bits;
  int h = cfg_val & _16_bits;
  *cfg = (AM_GPU_CONFIG_T) {
    .present   = true,
    .has_accel = false,
    .width     = w,
    .height    = h,
    .vmemsz    = w * h * sizeof(uint32_t)
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  uint32_t *pixels = (uint32_t *)ctl->pixels;
  uint32_t *fb = (uint32_t *)(uintptr_t)NPC_FB_ADDR;

  uint32_t cfg = inl(NPC_VGA_CTL_ADDR);
  int W = (cfg >> 16) & _16_bits;
  int H = cfg & _16_bits;

  if (fb != NULL && w > 0 && x < W && h > 0 && y < H) {
    int copy_h = (y + h) < H ? h : H - y;
    int copy_w = (x + w) < W ? w : W - x;
    for (int j = 0; j < copy_h; j++) {
      uint32_t *dst = fb + (j + y) * W + x;
      uint32_t *src = pixels + j * w;
      for (int i = 0; i < copy_w; i++) {
        dst[i] = src[i] | 0xFF000000;  // 强制 alpha=0xFF，兼容 ARGB8888
      }
    }
  }

  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
