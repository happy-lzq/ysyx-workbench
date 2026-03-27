#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

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

    if (*fmt == 's') {
      const char *s = va_arg(ap, const char *);
      if (s == NULL) s = "(null)";
      while (*s != '\0') {
        if (n > 0 && written + 1 < n) {
          *str++ = *s;
        }
        written++;
        s++;
      }
    }
    else if (*fmt == 'd' || *fmt == 'x' || *fmt == 'p') {
      uintptr_t value = 0;
      int base = 10;
      char buf[32];
      int i = 0;

      if (*fmt == 'd') {
        int num = va_arg(ap, int);
        if (num < 0) {
          if (n > 0 && written + 1 < n) {
            *str++ = '-';
          }
          written++;
          value = 0u - (unsigned int)num;
        } else {
          value = (unsigned int)num;
        }
      } else if (*fmt == 'x') {
        value = va_arg(ap, unsigned int);
        base = 16;
      } else {
        if (n > 0 && written + 1 < n) {
          *str++ = '0';
        }
        written++;
        if (n > 0 && written + 1 < n) {
          *str++ = 'x';
        }
        written++;
        value = (uintptr_t)va_arg(ap, void *);
        base = 16;
      }

      if (value == 0) {
        if (n > 0 && written + 1 < n) {
          *str++ = '0';
        }
        written++;
      } else {
        while (value > 0) {
          int digit = value % base;
          buf[i++] = (digit < 10 ? '0' + digit : 'a' + digit - 10);
          value /= base;
        }
        while (i > 0) {
          char ch = buf[--i];
          if (n > 0 && written + 1 < n) {
            *str++ = ch;
          }
          written++;
        }
      }
    }
    else if (*fmt == 'c' || *fmt == '%') {
      char ch = (*fmt == '%') ? '%' : (char)va_arg(ap, int);
      if (n > 0 && written + 1 < n) {
        *str++ = ch;
      }
      written++;
    }
    else {
      if (n > 0 && written + 1 < n) {
        *str++ = '%';
      }
      written++;
      if (n > 0 && written + 1 < n) {
        *str++ = *fmt;
      }
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
