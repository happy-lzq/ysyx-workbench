#include <npc.h>
#include <device.h>


#define KEYDOWN_MASK 0x8000

#ifndef CONFIG_TARGET_AM
#include <SDL2/SDL.h>
// Note that this is not the standard
#define NEMU_KEYS(f) \
  f(ESCAPE) f(F1) f(F2) f(F3) f(F4) f(F5) f(F6) f(F7) f(F8) f(F9) f(F10) f(F11) f(F12) \
f(GRAVE) f(1) f(2) f(3) f(4) f(5) f(6) f(7) f(8) f(9) f(0) f(MINUS) f(EQUALS) f(BACKSPACE) \
f(TAB) f(Q) f(W) f(E) f(R) f(T) f(Y) f(U) f(I) f(O) f(P) f(LEFTBRACKET) f(RIGHTBRACKET) f(BACKSLASH) \
f(CAPSLOCK) f(A) f(S) f(D) f(F) f(G) f(H) f(J) f(K) f(L) f(SEMICOLON) f(APOSTROPHE) f(RETURN) \
f(LSHIFT) f(Z) f(X) f(C) f(V) f(B) f(N) f(M) f(COMMA) f(PERIOD) f(SLASH) f(RSHIFT) \
f(LCTRL) f(APPLICATION) f(LALT) f(SPACE) f(RALT) f(RCTRL) \
f(UP) f(DOWN) f(LEFT) f(RIGHT) f(INSERT) f(DELETE) f(HOME) f(END) f(PAGEUP) f(PAGEDOWN)

#define NEMU_KEY_NAME(k) NEMU_KEY_ ## k,

enum {
  NEMU_KEY_NONE = 0,
  MAP(NEMU_KEYS, NEMU_KEY_NAME)
  // 将宏函数作为变量：NEMU_KEY_NAME 传入宏函数：NEMU_KEYS
};

#define SDL_KEYMAP(k) keymap[SDL_SCANCODE_ ## k] = NEMU_KEY_ ## k;
static uint32_t keymap[256] = {};

static void init_keymap() {
  MAP(NEMU_KEYS, SDL_KEYMAP)
}

#define KEY_QUEUE_LEN 1024
static int key_queue[KEY_QUEUE_LEN] = {};
static int key_f = 0, key_r = 0;  // key_f (读取位置） key_r (写入位置)

// 写入环形缓冲区
static void key_enqueue(uint32_t am_scancode) {
  key_queue[key_r] = am_scancode;
  key_r = (key_r + 1) % KEY_QUEUE_LEN;
  Assert(key_r != key_f, "key queue overflow!");
}
// 环形缓冲区读取
static uint32_t key_dequeue() {
  uint32_t key = NEMU_KEY_NONE;
  if (key_f != key_r) {
    key = key_queue[key_f];
    key_f = (key_f + 1) % KEY_QUEUE_LEN;
  }
  return key;
}
// SDL获取的原始按键扫描码转化为nemu可识别的键码
// 按下： 原始码按位与掩码   释放： 原始码按位与0(保持不变)
void send_key(uint8_t scancode, bool is_keydown) {
  if (npc_sim_state.state == NPC_RUNNING && keymap[scancode] != NEMU_KEY_NONE) {
    uint32_t am_scancode = keymap[scancode] | (is_keydown ? KEYDOWN_MASK : 0);
    key_enqueue(am_scancode);
  }
}
#endif

static uint32_t *i8042_data_port_base = NULL;

static uint32_t keyboard_read_handler(MMIODevice *dev, uint32_t offset) {
  assert(offset == 0);
  return key_dequeue();  // 返回 32 位键码（含 KEYDOWN_MASK）
}

void init_i8042() {
  i8042_data_port_base = (uint32_t *)new_space(4);
  i8042_data_port_base[0] = NEMU_KEY_NONE;
  add_mmio_device("keyboard", new_space(4), CONFIG_KEYBOARD_MMIO, 4, 0xF, keyboard_read_handler, NULL);
  init_keymap();
  printf("[keyboard] init_i8042 done, addr=0x%08x\n", CONFIG_KEYBOARD_MMIO);
}

void sdl_clear_event_queue() {
  SDL_Event event;
  while (SDL_PollEvent(&event));
}
