#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len =0;
  while (s[len] != '\0'){
    len++;
  }
  return len;
}

char *strcpy(char *dst, const char *src) {
  char *pre_dst = dst;
  while (*src != '\0')
  {
    *dst = *src;
    dst++;
    src++;
  }
  *dst = '\0';
  return pre_dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  char *pre_dst = dst;
  size_t i = 0;
  while ((i < n) && src[i] !='\0' )
  {
    dst[i] = src[i];
    i++;
  }
  for ( ; i < n; i++)
  {
    dst[i] = '\0';
  }
  return pre_dst;
}

char *strcat(char *dst, const char *src) {
  char *pre_dst = dst;
  size_t i = 0;
  while (dst[i] != '\0')
  {
    i++;
  }
  size_t y = 0;
  while (src[y] !='\0')
  {
    dst[i] = src[y];
    i++;
    y++;
  }
  dst[i] = '\0';
  return pre_dst;
}

int strcmp(const char *s1, const char *s2) {
  while (*s1 == *s2)
  {
    if (*s1=='\0')
    {
      return 0;
    }
    s1++;
    s2++;
  }
  /*
  在 x86 架构的 GCC 编译器里，光写 char 通常等同于 signed char（有符号，范围是 -128 到 127）。
  在 ARM 架构里，默认有时是 unsigned char（无符号，范围是 0 到 255）。

  */
  return (*(unsigned char*)s1 - *(unsigned char*)s2);
}

int strncmp(const char *s1, const char *s2, size_t n) {
  if (n == 0) return 0;
  size_t i = 0;
  while (*s1 == *s2 && i < n - 1)
  {
    if (*s1 == '\0')
    {
      return 0;
    }
    i++;
    s1++;
    s2++;
  }
  return (*(unsigned char*)s1 - *(unsigned char*)s2);   //n=1时直接计算第一个字符的ASIC码
}

void *memset(void *s, int c, size_t n) {
  //内存填充，指针指向前n个字节用int c 来填充
  unsigned char* pre_s =  (unsigned char *) s;
  size_t i = 0;
  while (i < n)
  {
    pre_s[i] = (unsigned char) c ;
    i++; 
  }
  return pre_s;
}

// void *memmove(void *dst, const void *src, size_t n) {
//   // 将源指针指向的内存位置的num字节 值复制到目标指针指向的内存块。复制过程如同使用中间缓冲区，允许源指针和目标指针重叠
//   unsigned char *pre_dst = (unsigned char *)dst;
//   const unsigned char *pre_src = (const unsigned char *)src;
//   size_t i=0;
//   // dst > src,从后往前移动
//   if (pre_dst > pre_src)
//   {
//      for ( i = n; i > 0; i--)
//      {
//   // 保证数组d[0]开始，并且循环成立，而且移动个数保证为n。
//       pre_dst[i-1] = pre_src[i-1];
//      }
//   } else {
//     for ( i = 0; i < n; i++)
//     {
//       pre_dst[i] = pre_src[i];
//     }
    
//   }
//   return pre_dst;
// }

void *memcpy(void *out, const void *in, size_t n) {
  // 将源指针指向的内存位置的num字节 值直接复制到目标指针指向的内存块。dst 和 src 这两块内存绝对、绝对不可能有任何重合的部分！”
  unsigned char *pre_out = (unsigned char *)out;
  const unsigned char *pre_in = (const unsigned char *)in;
  size_t i =0;
  while (i < n)
  {
    pre_out[i] = pre_in[i];
    i++;
  }
  return pre_out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  const unsigned char * pre_s1 = (const unsigned char *)s1;
  const unsigned char * pre_s2 = (const unsigned char *)s2;
  for (size_t i = 0; i < n; i++) {
    if (pre_s1[i] != pre_s2[i]) {
      return pre_s1[i] - pre_s2[i];
    }
  }
  return 0;
}

#endif
