#include <am.h>
#include <klib.h>
#include <riscv/riscv.h>

#define AUDIO_FREQ_ADDR      (NPC_AUDIO_ADDR + 0x00)
#define AUDIO_CHANNELS_ADDR  (NPC_AUDIO_ADDR + 0x04)
#define AUDIO_SAMPLES_ADDR   (NPC_AUDIO_ADDR + 0x08)
#define AUDIO_SBUF_SIZE_ADDR (NPC_AUDIO_ADDR + 0x0c)
#define AUDIO_INIT_ADDR      (NPC_AUDIO_ADDR + 0x10)
#define AUDIO_COUNT_ADDR     (NPC_AUDIO_ADDR + 0x14)

static size_t write_pos = 0;

void __am_audio_config(AM_AUDIO_CONFIG_T *cfg) {
  cfg->present = true;
  cfg->bufsize = inl(AUDIO_SBUF_SIZE_ADDR);
}

void __am_audio_ctrl(AM_AUDIO_CTRL_T *ctrl) {
  outl(AUDIO_FREQ_ADDR,    ctrl->freq);
  outl(AUDIO_CHANNELS_ADDR, ctrl->channels);
  outl(AUDIO_SAMPLES_ADDR,  ctrl->samples);
  outl(AUDIO_INIT_ADDR, 1);
}

void __am_audio_status(AM_AUDIO_STATUS_T *stat) {
  stat->count = inl(AUDIO_COUNT_ADDR);
}

void __am_audio_play(AM_AUDIO_PLAY_T *ctl) {
  uint8_t *src = ctl->buf.start;
  size_t len = ctl->buf.end - ctl->buf.start;
  uint32_t sbuf_size = inl(AUDIO_SBUF_SIZE_ADDR);

  while (len > 0) {
    uint32_t free_space = sbuf_size - inl(AUDIO_COUNT_ADDR);
    if (free_space == 0) continue;

    size_t able_copy = len < free_space ? len : free_space;
    uint8_t *sbuf = (uint8_t *)(uintptr_t)NPC_AUDIO_SBUF_ADDR;

    if (write_pos + able_copy <= sbuf_size) {
      memcpy(sbuf + write_pos, src, able_copy);
    } else {
      size_t first  = sbuf_size - write_pos;
      size_t second = able_copy - first;
      memcpy(sbuf + write_pos, src, first);
      memcpy(sbuf, src + first, second);
    }
    write_pos = (write_pos + able_copy) % sbuf_size;
    outl(AUDIO_COUNT_ADDR, able_copy);
    src += able_copy;
    len -= able_copy;
  }
}
