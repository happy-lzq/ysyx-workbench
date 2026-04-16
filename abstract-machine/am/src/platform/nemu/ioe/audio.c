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
抽象寄存器	            上层软件操作	             AM 驱动层职责	      NEMU 硬件层职责
AM_AUDIO_CONFIG	
  1、读 present, bufsize	
  2、返回 true 和通过读 reg_sbuf_size 获取的真实硬件缓冲区字节数	
  3、初始化时设定 reg_sbuf_size 为 CONFIG_SB_SIZE，暴露给 AM
AM_AUDIO_CTRL	
  1、写 freq, channels, samples	
  2、将三个参数依次写入硬件寄存器 reg_freq, reg_channels, reg_samples 最后写 reg_init=1 触发设备初始化	
  3、收到 reg_init 写操作后，调用 SDL_OpenAudioDevice  配置并启动音频设备；重置 read_pos 和 pending_count
AM_AUDIO_STATUS	
  1、读 count	
  2、读取硬件寄存器 reg_count 的值并返回，用于上层或自身的流控决策	
  3、在读 reg_count 时返回当前内部维护的 pending_count（已缓冲未播放字节数）
AM_AUDIO_PLAY	
  1、写 Area buf（数据指针和长度）	
  2、数据通路：将 buf 描述的音频数据分段拷贝到 sbuf 中，处理环形写入；
    控制通路：每段拷贝后写 reg_count 增量通知硬件。
    流控：拷贝前检查 free_space = sbuf_size - pending_count，确保不覆盖未播放数据	
  3、数据消费：SDL 音频回调独立运行，从 sbuf 中取数据填充宿主声卡，并实时更新 pending_count 和 read_pos；状态暴露：通过读 reg_count 提供 pending_count 值

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
