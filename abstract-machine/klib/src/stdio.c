#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>
#include <stdbool.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
// native 未被定义,显示要求native使用当前klib 
int printf(const char *fmt, ...) {
  char buf[1024];
  va_list ap;
  va_start(ap, fmt);
  int ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  int limit = ret;
  if (limit > (int)sizeof(buf) - 1) {
    limit = sizeof(buf) - 1;
  }
  for (int i = 0; i < limit; i++) {
    putch(buf[i]);
  }
  return ret;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  // Reuse the fully featured formatter so sprintf/vsprintf and
  // snprintf/vsnprintf stay behaviorally aligned.
  return vsnprintf(out, (size_t)-1, fmt, ap);
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  //va_start(va_list ap, last_fixed_arg) 
  //last_fixed_arg  ... 前面的最后一个确定已知的参数。在 sprintf 中，它就是那个 fmt。
  va_start(ap, fmt);
  int ret = vsprintf(out, fmt, ap);
  va_end(ap);
  //使 ap 失效：它会将 ap 强行置为空指针（NULL）
  return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int ret = vsnprintf(out, n, fmt, ap);
  va_end(ap);
  return ret;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  char *str = out;
  size_t written = 0;

  while (*fmt != '\0') {
    if (*fmt != '%') {
      if (n > 0 && written + 1 < n) {
        *str++ = *fmt;
      }
      written++;
      fmt++;
      continue;
    }

    fmt++;
    if (*fmt == '\0') break;

    char pad_char = ' ';
    if (*fmt == '0') {
      pad_char = '0';
      fmt++;
    }

    int width = 0;
    while (*fmt >= '0' && *fmt <= '9') {
      width = width * 10 + (*fmt - '0');
      fmt++;
    }

    if (*fmt == 's') {
      const char *s = va_arg(ap, const char *);
      if (s == NULL) s = "(null)";
      int s_len = 0;
      while (s[s_len] != '\0') s_len++;
      int pad_len = width - s_len;
      if (pad_len < 0) pad_len = 0;
      
      while (pad_len-- > 0) {
        if (n > 0 && written + 1 < n) *str++ = ' ';
        written++;
      }
      while (*s != '\0') {
        if (n > 0 && written + 1 < n) *str++ = *s;
        written++;
        s++;
      }
    }
    else if (*fmt == 'd' || *fmt == 'x' || *fmt == 'p') {
      uintptr_t value = 0;
      int base = 10;
      char buf[32];
      int i = 0;
      bool is_neg = false;

      if (*fmt == 'd') {
        int num = va_arg(ap, int);
        if (num < 0) {
          is_neg = true;
          value = 0u - (unsigned int)num;
        } else {
          value = (unsigned int)num;
        }
      } else if (*fmt == 'x') {
        value = va_arg(ap, unsigned int);
        base = 16;
      } else {
        value = (uintptr_t)va_arg(ap, void *);
        base = 16;
      }

      if (value == 0) {
        buf[i++] = '0';
      } else {
        while (value > 0) {
          int digit = value % base;
          buf[i++] = (digit < 10 ? '0' + digit : 'a' + digit - 10);
          value /= base;
        }
      }

      int prefix_len = is_neg ? 1 : (*fmt == 'p' ? 2 : 0);
      int num_len = i;
      int pad_len = width - num_len - prefix_len;
      if (pad_len < 0) pad_len = 0;

      // Handle space padding (goes before prefix)
      if (pad_char == ' ') {
        while (pad_len-- > 0) {
          if (n > 0 && written + 1 < n) *str++ = ' ';
          written++;
        }
      }

      // Output prefix
      if (is_neg) {
        if (n > 0 && written + 1 < n) *str++ = '-';
        written++;
      } else if (*fmt == 'p') {
        if (n > 0 && written + 1 < n) *str++ = '0';
        written++;
        if (n > 0 && written + 1 < n) *str++ = 'x';
        written++;
      }

      // Handle zero padding (goes after prefix)
      if (pad_char == '0') {
        while (pad_len-- > 0) {
          if (n > 0 && written + 1 < n) *str++ = '0';
          written++;
        }
      }

      // Output digits reversed
      while (i > 0) {
        char ch = buf[--i];
        if (n > 0 && written + 1 < n) *str++ = ch;
        written++;
      }
    }
    else if (*fmt == 'c' || *fmt == '%') {
      char ch = (*fmt == '%') ? '%' : (char)va_arg(ap, int);
      if (n > 0 && written + 1 < n) *str++ = ch;
      written++;
    }
    else {
      // Revert unrecognized formatter handling cleanly
      if (n > 0 && written + 1 < n) *str++ = '%';
      written++;
      if (pad_char == '0') {
        if (n > 0 && written + 1 < n) *str++ = '0';
        written++;
      }
      if (width > 0) { // Just a tiny hack to approximate original fallback loop
         // In reality, this is fine to just drop width digits for unrecognized specifiers 
      }
      if (n > 0 && written + 1 < n) *str++ = *fmt;
      written++;
    }

    fmt++;
  }

  if (n > 0) {
    if (written < n) *str = '\0';
    else out[n - 1] = '\0';
  }

  return (int)written;
}

#endif
