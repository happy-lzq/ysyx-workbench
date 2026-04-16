/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <common.h>
#include <device/map.h>
#include <SDL2/SDL.h>
/*  SDL音频库的运行逻辑
1、SDL_AudioSpec 结构体和回调函数Callback 为核心，一个是生产者 一个是消费者
2、SDL音频线程独立于CPU模拟线程由回调函数触发：从音频环形缓冲区sbuf取出数据填充到SDL流宿主声卡播放

*/



// 将声卡的三个抽象寄存器--软件协议构建对应的独立硬件寄存器   
enum {
  reg_freq,      // W  只存值，无副作用
  reg_channels,  // W  只存值，无副作用
  reg_samples,   // W  只存值，无副作用
  reg_sbuf_size, // RD 读出时：返回固定容量 CONFIG_SB_SIZE。
  reg_init,      // w  有副作用。若写非 0，执行“应用配置并启动/重启 SDL 设备”
  reg_count,     // RD 写入时：如果你采用“delta 通知”模型，则把写入值当新增待播字节，读出时：返回当前待播字节数（动态）。
  nr_reg         //   上述6寄存器= 6 来开辟寄存器空间
};
static SDL_AudioDeviceID audio_dev = 0; // 
static uint8_t *sbuf = NULL;            // 流缓冲区指针
static uint32_t *audio_base = NULL;     // 寄存器数组基址
static size_t read_pos = 0;             // SDL 线程下一次读取sbuf的起始位置（硬件内部维护，软件不可见）
static uint32_t pending_count = 0;      // 真正被SDL读取给宿主声卡播放的数据大小
// 全局设备真实写入缓冲区待SDL读写字节,要与设备寄存器reg_count分离处理，保证缓冲区待读字节的正确性
// 原因：MMIO 写操作：先 host_write(已经更改了reg_count当前值)，再 invoke_callback。

// SDL 在独立的音频线程中周期调用    len 必须填满否则音爆/静音
static void sdl_audio_callback(void *userdata,Uint8 *stream,int len){
// 1、计算实际可拷贝的数据量
  size_t able_copy = pending_count < len ? pending_count : len;
// 2、将外界音频缓冲区拷贝数据到stream 
  if (read_pos + able_copy <= audio_base[reg_sbuf_size]){
    memcpy(stream,sbuf + read_pos, able_copy); 
  } else{
    size_t first_part = audio_base[reg_sbuf_size] - read_pos ;
    memcpy(stream,sbuf + read_pos,first_part);                  // 缓冲区末尾部分写满
    memcpy(stream + first_part,sbuf,able_copy - first_part);    // 再将剩余部分绕回处理
  }
// 3、如果可拷贝数据不够，剩余部分填0，防止音爆
  if (able_copy < len){
    memset(stream + able_copy, 0, len - able_copy);
  }
// 4、更新读指针和缓冲计数
  read_pos = (read_pos + able_copy) %audio_base[reg_sbuf_size];
  pending_count -= able_copy;
// 5、同步到reg_count抽象寄存器更新
  audio_base[reg_count] = pending_count;
  
}



static void audio_open_device(){
  // 1、如果之前打开过，先关闭
  if (audio_dev !=0){
    SDL_CloseAudioDevice(audio_dev);
  }
  // 2、设置音频参数   音频协议
  SDL_AudioSpec want,have;
  want.freq     = audio_base[reg_freq];
  want.format   = AUDIO_S16SYS;
  want.channels = audio_base[reg_channels];
  want.samples  = audio_base[reg_samples];
  want.callback = sdl_audio_callback;
  want.userdata = NULL;

  // 3、打开外设获得实际参数
  audio_dev = SDL_OpenAudioDevice(NULL,0,&want,&have,0);
  if (audio_dev == 0){
    printf("SDL_OpenAudioDevice failed: %s\n", SDL_GetError());
    return;
  }
  // 4、播放
    SDL_PauseAudioDevice(audio_dev,0);
}



// 基于offset/4 读写数据动作在总线层已经完成，case进需要进行对应的副作用
// MMIO 写操作：先 host_write，再 invoke_callback。
// MMIO 读操作：先 先invoke_callback, 再 host_read
// 核心：数据读写通路由 MMIO完成，case属于控制通路触发副作用
static void audio_io_handler(uint32_t offset, int len, bool is_write) {
  int reg_idx = offset / 4;
  if (is_write)  {
    switch (reg_idx){
    case reg_freq:
    case reg_channels:
    case reg_samples:
      break;
    case reg_init:
      if (audio_base[reg_init] != 0){
        audio_open_device();
    // 重置逻辑的本质是“丢弃旧格式脏数据，为新格式建立干净起点”
        audio_base[reg_init] = 0;
        read_pos = 0;
        pending_count = 0;
        audio_base[reg_count] = 0;
      }
      break;
    case reg_count:
      uint32_t delta = audio_base[reg_count];
      // reg_count 完善被SDL读走之后剩余容量的动态保护
      if (delta > audio_base[reg_sbuf_size] - pending_count){
        delta = audio_base[reg_sbuf_size] - pending_count;      // 防止溢出
      }
      pending_count += delta; 
      // audio_base[reg_count] = pending_count;                    
      break;
    default:
      break;
    }
  } else {
    switch (reg_idx){
      case reg_count:
        audio_base[reg_count] = pending_count;    // 软件层读取时，反正实际已缓冲未SDL读取容量，用于流控处理
        break;
      case reg_sbuf_size:
        audio_base[reg_sbuf_size] = CONFIG_SB_SIZE;
      break;
      default:
        break;
    }
  }
}



/*
硬件初始化：
1、在模拟器启动时，造出“寄存器内存”和“数据缓冲区”，
2、把它们挂到约定好的物理地址上，同时初始化好宿主端的音频库，
3、把真正启动音频流的开关交给 AM 通过写 reg_init 来触发
*/

void init_audio() {
  uint32_t space_size = sizeof(uint32_t) * nr_reg;
  audio_base = (uint32_t *)new_space(space_size);
  // audio_base 是控制寄存器组的存储空间
#ifdef CONFIG_HAS_PORT_IO
  add_pio_map ("audio", CONFIG_AUDIO_CTL_PORT, audio_base, space_size, audio_io_handler);
#else
  add_mmio_map("audio", CONFIG_AUDIO_CTL_MMIO, audio_base, space_size, audio_io_handler);
#endif
// 
  audio_base[reg_sbuf_size] = CONFIG_SB_SIZE;    // 提供音频硬件缓冲区大小
  audio_base[reg_count] = 0;                     // 当前已缓冲但SDL未读取的字节大小

  sbuf = (uint8_t *)new_space(CONFIG_SB_SIZE);   // 音频数据缓冲区，里面存放的是实际的 PCM 采样数据
  add_mmio_map("audio-sbuf", CONFIG_SB_ADDR, sbuf, CONFIG_SB_SIZE, NULL);
  // SDL_INIT_AUDIO SDL音频库初始化音频子系统
  SDL_InitSubSystem(SDL_INIT_AUDIO);
}
