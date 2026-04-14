#include <am.h>
#include <nemu.h>
#include <klib.h>

#define AUDIO_FREQ_ADDR      (AUDIO_ADDR + 0x00)  // reg_freq,      
#define AUDIO_CHANNELS_ADDR  (AUDIO_ADDR + 0x04)  // reg_channels,  
#define AUDIO_SAMPLES_ADDR   (AUDIO_ADDR + 0x08)  // reg_samples,   
#define AUDIO_SBUF_SIZE_ADDR (AUDIO_ADDR + 0x0c)  // reg_sbuf_size  
#define AUDIO_INIT_ADDR      (AUDIO_ADDR + 0x10)  // reg_init,      
#define AUDIO_COUNT_ADDR     (AUDIO_ADDR + 0x14)  // reg_count,   

/*
抽象寄存器             上层软件操作                     AM职责                          NEMU硬件指针
AM_AUDIO_CONFIG     读present,busize            返回true以及真实硬件缓冲区大小      初始化提供设备是否存在以及缓冲区大小
AM_AUDIO_CRTL       写freq,channels,samples     三个软件层参数写入外设音频控制寄存器  重新配置SDL音频规格 
AM_AUDIO_STATUS     读count                     已缓冲待播放字节数                 流控处理
AM_AUDIO_PLAY       写Aera buf                  将buf数据拷贝外设音频流缓冲区       维护计数器

抽象寄存器 AM_AUDIO_CTRL	
一次配置完成：io_write(AM_AUDIO_CTRL, freq, channels, samples)	上层认为这是原子的单次操作
硬件寄存器 reg_freq/channels/samples/init	
三个参数分别存放在独立寄存器中，通过写 reg_init 触发硬件动作	底层实际是多次 MMIO 写 + 一次触发

*/

static size_t write_pos = 0;  //AM 在 sbuf 中的写偏移



void __am_audio_init() {
  outl(AUDIO_COUNT_ADDR, 0); // 告诉硬件，把当前未播放的声音数据清零（防止开机爆音）
  outl(AUDIO_INIT_ADDR, 0);  // 初始化声卡状态机的状态
  write_pos = 0;
}

// 读取设备存在性和缓冲区大小
void __am_audio_config(AM_AUDIO_CONFIG_T *cfg) {
  cfg->present = true;
  cfg->bufsize = inl(AUDIO_SBUF_SIZE_ADDR);
}

// 配置音频参数并启动设备
void __am_audio_ctrl(AM_AUDIO_CTRL_T *ctrl) {
  outl(AUDIO_FREQ_ADDR,ctrl->freq);
  outl(AUDIO_CHANNELS_ADDR,ctrl->channels);
  outl(AUDIO_SAMPLES_ADDR,ctrl->samples);
  outl(AUDIO_INIT_ADDR,1);      // 解决写同步屏障，参数配置成功后执行设备硬件初始化
}

void __am_audio_status(AM_AUDIO_STATUS_T *stat) {
  stat->count = inl(AUDIO_COUNT_ADDR);    // 实际缓冲区剩余未被SDL读取容量

}

void __am_audio_play(AM_AUDIO_PLAY_T *ctl) {
  uint8_t *src = ctl->buf.start; 
  size_t len = ctl->buf.end -ctl->buf.start;    // 软件层真实待播放音频数据总量，起始地址和初始地址

  uint32_t sbuf_size = inl(AUDIO_SBUF_SIZE_ADDR);
  while ( len>0 )
  {
    uint32_t free_space = sbuf_size - inl(AUDIO_COUNT_ADDR);
    // 写入流控逻辑根据缓冲区free_space决定写入
    // 1、free_space = 0  重复读取硬件缓冲区空闲量直到存在容量可写
    if (free_space == 0) continue;
    // 2、free_space !=0   
    size_t able_copy = len < free_space ? len : free_space;
    uint8_t *sbuf = (uint8_t *)AUDIO_SBUF_ADDR;    // 音频硬件缓冲区起始地址
    // 3、写入量未超过缓冲区末尾段，不做夸段饶回处理
    if (write_pos + able_copy <= sbuf_size){
      memcpy(sbuf + write_pos,src,able_copy);
    } else{
    // 4、数据跨越缓冲区末尾，分两段拷贝
      size_t fisrt_part  = sbuf_size - write_pos;
      size_t second_part = able_copy - fisrt_part;
      memcpy(sbuf + write_pos,src,fisrt_part);
      memcpy(sbuf,src + fisrt_part,second_part); 
    }
    // 5、维护软件写指针(环形移动),下一次写入的起始位置在缓冲区中的偏移量
    write_pos = (write_pos + able_copy) % sbuf_size;
    // 6、抽象控制寄存器更新信息内容进行对话协议交互，reg_count 写入本次硬件音频缓冲区真实的写入量
    outl(AUDIO_COUNT_ADDR, able_copy);
    // 7、维护软件层待写入数据的指针
    src += able_copy;
    len -=able_copy;
  }
  
  
}
