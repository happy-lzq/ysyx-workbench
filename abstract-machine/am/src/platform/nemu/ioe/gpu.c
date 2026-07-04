#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)
#define _16_bits 0xffff


// 硬件初始化 + 早期测试，确保 GPU 子系统正常工作
void __am_gpu_init() {
  int i;
  // 宽高数据存放在硬件中定义在外设的设备寄存器：VGACTL_ADDR
  // 先从设备寄存器中读出来然后进行高地16位拆分
  uint32_t cfg = inl(VGACTL_ADDR);
  int w = (cfg >> 16) & _16_bits;
  int h = cfg & _16_bits;

  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = 0;
  outl(SYNC_ADDR, 1);
}

// 上层应用提供当前 GPU 的真实参数，实现硬件抽象
void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {

  uint32_t cfg_val = inl(VGACTL_ADDR);
  *cfg = (AM_GPU_CONFIG_T) {
    .present   = true, 
    .has_accel = false,
    .width  = (cfg_val >> 16) & _16_bits, 
    .height = cfg_val & _16_bits,
    .vmemsz = cfg->width * cfg->height * sizeof(uint32_t)
    // Vmemsz 属于像素的显存总大小，按字节存储 
    // 宽高代表像素点个数，每个像素点4字节=32位
  };
}


// 软件层的图像帧数据写入到抽象寄存器中vga帧缓冲区寄存器，当sync同步信号为ture 供给硬件实现帧缓冲区刷新到屏幕

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  uint32_t *pixels = (uint32_t *)ctl->pixels;
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;

  uint32_t cfg = inl(VGACTL_ADDR);
  int W = (cfg >> 16) & _16_bits;
  int H = cfg & _16_bits;
  

  // 将源像素块裁剪到屏幕可见区域，再按行写入 framebuffer。
  if (fb != NULL && w >0 && x < W && h > 0 && y < H ){
    int copy_h = (y + h) < H ? h : H - y;
    int copy_w = (x + w) < W ? w : W - x ;
    for ( int j = 0; j < copy_h; j++){
      uint32_t *dst = fb + (j + y)*W + x;
      uint32_t *src = pixels + j*w ;
      for (int i = 0; i < copy_w; i++)
      {
        dst[i] = src[i];
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
