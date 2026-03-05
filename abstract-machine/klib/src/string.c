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
  panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  panic("Not implemented");
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  panic("Not implemented");
}

#endif
