#include <npc.h>
#include <device.h>
#include <SDL2/SDL.h>

enum {
  reg_freq,
  reg_channels,
  reg_samples,
  reg_sbuf_size,
  reg_init,
  reg_count,
  nr_reg
};

static SDL_AudioDeviceID audio_dev = 0;
static uint8_t *sbuf = NULL;
static uint32_t *audio_base = NULL;
static size_t read_pos = 0;
static uint32_t pending_count = 0;

static void sdl_audio_callback(void *userdata, Uint8 *stream, int len) {
  size_t sz = (size_t)audio_base[reg_sbuf_size];
  size_t able_copy = pending_count < (uint32_t)len ? pending_count : (uint32_t)len;

  if (read_pos + able_copy <= sz) {
    memcpy(stream, sbuf + read_pos, able_copy);
  } else {
    size_t first = sz - read_pos;
    memcpy(stream, sbuf + read_pos, first);
    memcpy(stream + first, sbuf, able_copy - first);
  }
  if (able_copy < (size_t)len) {
    memset(stream + able_copy, 0, len - able_copy);
  }
  read_pos = (read_pos + able_copy) % sz;
  pending_count -= able_copy;
  audio_base[reg_count] = pending_count;
}

static void audio_open_device() {
  if (audio_dev != 0) {
    SDL_CloseAudioDevice(audio_dev);
  }
  SDL_AudioSpec want, have;
  want.freq     = audio_base[reg_freq];
  want.format   = AUDIO_S16SYS;
  want.channels = audio_base[reg_channels];
  want.samples  = audio_base[reg_samples];
  want.callback = sdl_audio_callback;
  want.userdata = NULL;

  audio_dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
  if (audio_dev == 0) {
    printf("SDL_OpenAudioDevice failed: %s\n", SDL_GetError());
    return;
  }
  SDL_PauseAudioDevice(audio_dev, 0);
}

static uint32_t audio_read_handler(MMIODevice *dev, uint32_t offset) {
  int reg_idx = offset / 4;
  switch (reg_idx) {
    case reg_sbuf_size:
      audio_base[reg_sbuf_size] = CONFIG_SB_SIZE;
      return CONFIG_SB_SIZE;
    case reg_count:
      audio_base[reg_count] = pending_count;
      return pending_count;
    default:
      return audio_base[reg_idx];
  }
}

static void audio_write_handler(MMIODevice *dev, uint32_t offset,
                                 uint32_t data, uint8_t wmask) {
  // data has already been written to dev->space by mmio_space_write
  int reg_idx = offset / 4;
  audio_base[reg_idx] = mmio_space_read(dev, offset & ~3u);

  switch (reg_idx) {
    case reg_init:
      if (audio_base[reg_init] != 0) {
        audio_open_device();
        audio_base[reg_init] = 0;
        read_pos = 0;
        pending_count = 0;
        audio_base[reg_count] = 0;
      }
      break;
    case reg_count: {
      uint32_t delta = audio_base[reg_count];
      uint32_t sz = audio_base[reg_sbuf_size];
      if (delta > sz - pending_count) {
        delta = sz - pending_count;
      }
      pending_count += delta;
      break;
    }
    default: break;
  }
}

void init_audio() {
  uint32_t space_size = sizeof(uint32_t) * nr_reg;
  audio_base = (uint32_t *)new_space(space_size);
  add_mmio_device("audio", audio_base, CONFIG_AUDIO_CTL_MMIO, space_size, 0xF,
                  audio_read_handler, audio_write_handler);

  audio_base[reg_sbuf_size] = CONFIG_SB_SIZE;
  audio_base[reg_count] = 0;

  sbuf = (uint8_t *)malloc(CONFIG_SB_SIZE);
  assert(sbuf);
  add_mmio_device("audio-sbuf", sbuf, CONFIG_SB_ADDR, CONFIG_SB_SIZE, 0xF, NULL, NULL);

  SDL_InitSubSystem(SDL_INIT_AUDIO);
}
