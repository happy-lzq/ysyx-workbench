#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char *str = out;
  /*
  1、循环检查结束符
  2、循环检查不等于字符串格式符%，str++
  3、遇见%：跳过检查后接的符号进行分类
  */
  while (*fmt != '\0') {
    if (*fmt != '%') {
      *str++ = *fmt++;
      continue;
    }
    fmt++; // skip '%'
    if (*fmt == '\0') {
      break;
    }
    switch (*fmt) {
      case 's': {
  // va_arg(va_list ap, type)：字符串格式决定type类型，进而决定va_arg()的返回类型，ap列表内存地址自动滑动相应数据类型内存宽度
        const char *s = va_arg(ap, const char *);
        if (!s) s = "(null)";
        while (*s != '\0') {
          *str++ = *s++;
        }
        break;
      }
      case 'd': {
        int d = va_arg(ap, int);
        if (d == 0) {
          *str++ = '0';
        } else {
          unsigned int ud;  // 接受转化后的无符号正整数
          if (d < 0) {
            *str++ = '-';   // 负数在无符号整数前添加 -
            ud = -d;
          } else {
            ud = d;
          }
// 把内存里的二进制正整数 ud，剥离出每一位的十进制位，转换为对应的 ASCII 字符，最后拼接到刚才的输出缓冲区里。
          char buf[32];
          int i = 0;
          while (ud > 0) {
            buf[i++] = (ud % 10) + '0'; // ASCII 字符'0' = 48；普通纯数字i(0-9)+字符'0' 刚好在ASCII码中表示对应的输出数字(0-9)
            ud /= 10;
          }
          while (i > 0) {
            *str++ = buf[--i];     // 将缓冲区的进制数调换成正确的位置()
          }
        }
        break;
      }
      default: {
        *str++ = '%';
        *str++ = *fmt;
        break;
      }
    }
    fmt++;
  }
  *str = '\0';
// printf、sprintf 等格式化输出家族函数的标准设定（也是 POSIX 标准规定）必须：
// 返回成功写入缓冲区的字符总数（不包括结尾的 \0 结束符）。
  return str - out;
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
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
