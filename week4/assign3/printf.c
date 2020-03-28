#include "printf_internal.h"
#include "printf.h"
#include "strings.h"
#include <stdarg.h>

#define MAX(x, y) ((x) <= (y) ? y: x)

void panic() {
  while(1) {
  }
}

int number_to_base(
  char *buf,
  size_t bufsize,
  unsigned int val,
  int base,
  int min_width,
  int negative
) {
  int result = 0;
  char output[MAX(sizeof(int), min_width)];
  int cursor = 0;

  if(val == 0) {
    output[cursor++] = '0';
  }
  while(val != 0) {
    int v = val % base;

    if(v < 10) {
      output[cursor++] = '0' + v;
    } else {
      output[cursor++] = 'a' + v - 10;
    }

    val = val / base;
  }

  int limit = min_width - cursor;
  if(negative) {
    limit--;
  }
  for(int i = 0; i < limit; i++) {
    output[cursor++] = '0';
  }

  if(negative) {
    output[cursor++] = '-';
  }

  result = cursor;

  for(int i = 0; i + 1 < bufsize && cursor > 0; i++) {
    *buf++ = output[--cursor];
  }

  if(bufsize > 0) {
    *buf = 0;
  }

  return result;
}

int unsigned_to_base(
  char *buf,
  size_t bufsize,
  unsigned int val,
  int base,
  int min_width
) {
  return number_to_base(
    buf, bufsize,
    val, base, min_width,
    0
  );
}

int signed_to_base(
  char *buf,
  size_t bufsize,
  int val,
  int base,
  int min_width
) {
  return number_to_base(
    buf, bufsize,
    val < 0 ? -val : val, base, min_width,
    val < 0 ? 1 : 0
  );
}

// return how many bytes actually are written
size_t memncpy(char *dst, char *src, size_t dst_size, size_t src_size) {
  if(src_size <= dst_size) {
    memcpy(dst, src, src_size);
    return src_size;
  }

  memcpy(dst, src, dst_size);
  return dst_size;
}

// return 0 or 1
size_t putchar(char *dst, char c, size_t dst_size) {
  if(dst_size > 0) {
    *dst = c;
    return 1;
  }

  return 0;
}

inline int
is_number_char(char c) {
  return c >= '0' && c <= '9';
}

int snprintf(
  char *buf,
  size_t bufsize,
  const char *format,
  ...
) {
  va_list ap;
  va_start(ap, buf);
  return vsnprintf(buf, bufsize, format, ap);
}

// for invaid format conversions
// will copy it as is
int vsnprintf(
  char *buf,
  size_t bufsize,
  const char *format,
  va_list ap
) {
  if(bufsize == 0) return;

  int result = 0;
  size_t written = 0;
  char *start = (char *)format;
  char *end = (char *)format;
  char cur;

  while((cur = *end) != 0) {
    if(cur != '%') {
      end++;
      continue;
    };

    if(start != end) {
      result += end - start;
      written += memncpy(buf + written, start, bufsize - written - 1, end - start);
    }

    end++;
    cur = *end;

    // %%, %c
    if(cur == '%' || cur == 'c') {
      // NOTE: default argument promotions
      // https://stackoverflow.com/questions/1255775/default-argument-promotions-in-c-function-calls
      char c = cur == '%' ? '%' : (char)va_arg(ap, int);
      written += putchar(buf + written, c, bufsize - written - 1);
      result += 1;
    }

    // %s
    else if(cur == 's') {
      char *str = va_arg(ap, char *);
      size_t len = strlen(str);
      size_t w = memncpy(buf + written, str, bufsize - written - 1, len);
      written += w;
      result += len;
      if(written == bufsize - 1) {
        buf[bufsize - 1] = 0;
        return result;
      }
    }

    // %d, %x with optional width
    else if(cur == 'd' || cur == 'x' || is_number_char(cur)) {
      unsigned int width = 0;
      int is_valid = 1;

      if(is_number_char(cur)) {
        // NOTE: assume this is enough
        char tmp[64];
        char *tp = tmp;

        while(is_number_char(*end)) {
          *tp++ = *end++;
        }
        *tp = 0;
        cur = *end;

        if(cur == 'd' || cur == 'x') {
          char *endptr;
          width = strtonum(tmp, &endptr);

          // should never happen
          if(endptr != tp) {
            panic();
          }
        } else {
          is_valid = 0;

          *tp++ = cur;
          *tp = 0;

          result += 1;
          written += putchar(buf + written, '%', bufsize - written - 1);

          int tmp_length = tp - tmp;
          written += memncpy(buf + written, tmp, bufsize - written - 1, tmp_length);
          result += tmp_length;
        }
      }

      if(is_valid) {
        int value = va_arg(ap, int);
        size_t capacity = bufsize - written;
        int c = signed_to_base(buf + written, capacity, value, cur == 'd' ? 10 : 16, width);
        result += c;

        if(c >= capacity - 1) {
          return result;
        } else {
          written += c;
        }
      }
    }

    // %p
    // outputs an address as a width-8 hexadecimal string prefixed with 0x, e.g. 0x20200004
    else if (cur == 'p') {
      size_t capacity = bufsize - written;

      result += 2;
      if(capacity >= 3) {
        memcpy(buf + written, "0x", 2);
        written += 2;
        capacity -= 2;
      }

      unsigned int addr = va_arg(ap, unsigned int);
      int c = unsigned_to_base(buf + written, capacity, addr, 16, 8);
      result += c;
      if(c >= capacity - 1) {
        return result;
      } else {
        written += c;
      }
    }

    // invalid format conversions
    // copy it as is
    else {
      result += 2;
      written += putchar(buf + written, '%', bufsize - written - 1);
      written += putchar(buf + written, cur, bufsize - written - 1);
    }

    end++;
    start = end;
  }

  if(start != end) {
    result += end - start;
    written += memncpy(buf + written, start, bufsize - written - 1, end - start);
  }

  buf[written] = 0;

  return result;
}

int printf(const char *format, ...) {
  char buf[1024];
  va_list ap;
  va_start(ap, buf);
  int result = vsnprintf(buf, sizeof(buf), format, ap);
  uart_putstring(buf);
}
