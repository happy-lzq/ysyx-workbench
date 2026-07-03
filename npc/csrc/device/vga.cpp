
#include <npc.h>
#include <device.h>

#ifdef CONFIG_VGA_SIZE_800x600
#define SCREEN_W 800
#define SCREEN_H 600
#else
#define SCREEN_W 400
#define SCREEN_H 300
#endif

static uint32_t screen_width()  { return SCREEN_W; }
static uint32_t screen_height() { return SCREEN_H; }

static uint32_t screen_size() {
  return screen_width() * screen_height() * sizeof(uint32_t);
}

static void *vmem = NULL;
static uint32_t *vgactl_port_base = NULL;

#ifdef CONFIG_VGA_SHOW_SCREEN
#ifndef CONFIG_TARGET_AM
#include <SDL2/SDL.h>

static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

static void init_screen() {
  SDL_Window *window = NULL;
  char title[128];
  sprintf(title, "NPC");
  SDL_Init(SDL_INIT_VIDEO);
#ifdef CONFIG_VGA_SIZE_400x300
  SDL_CreateWindowAndRenderer(SCREEN_W * 2, SCREEN_H * 2, 0, &window, &renderer);
#else
  SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_H, 0, &window, &renderer);
#endif
  SDL_SetWindowTitle(window, title);
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STATIC, SCREEN_W, SCREEN_H);
  SDL_RenderPresent(renderer);
}

static inline void update_screen() {
  SDL_UpdateTexture(texture, NULL, vmem, SCREEN_W * sizeof(uint32_t));
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}
#else
static void init_screen() {}

static inline void update_screen() {
  io_write(AM_GPU_FBDRAW, 0, 0, vmem, screen_width(), screen_height(), true);
}
#endif
#endif

void vga_update_screen() {
  if (vgactl_port_base[1] !=0){
    update_screen();
    vgactl_port_base[1] = 0;
  }
  return;
}

void init_vga() {
  vgactl_port_base = (uint32_t *)new_space(8);
  vgactl_port_base[0] = (screen_width() << 16) | screen_height();

  add_mmio_device("vgactl", vgactl_port_base, CONFIG_VGA_CTL_MMIO, 8, 0xF, NULL, NULL);

  vmem = new_space(screen_size());
  add_mmio_device("vmem", vmem, CONFIG_FB_ADDR, screen_size(), 0xF, NULL, NULL);

  #ifdef CONFIG_VGA_SHOW_SCREEN
    init_screen();
    memset(vmem, 0, screen_size());
  #endif
}
