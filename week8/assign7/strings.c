#include <stddef.h>
#include <strings.h>

void *
memset(void *s, int c, size_t n)
{
  unsigned char *p = (unsigned char *)s;
  for(size_t i = 0; i < n; i++)
    *p++ = c;
  return s;
}

void *
memcpy(void *dst, const void *src, size_t n)
{
  unsigned char *p = (unsigned char *)dst;
  unsigned char *s = (unsigned char *)src;
  for(size_t i = 0; i < n; i++)
    *p++ = *s++;
  return dst;
}

size_t
strlen(const char *s)
{
  size_t result = 0;
  while(*s++ != 0)
    result++;
  return result;
}

int
strcmp(const char *s1, const char *s2)
{
  unsigned char *p1 = (unsigned char *)s1;
  unsigned char *p2 = (unsigned char *)s2;
  unsigned char v1, v2;
  while(1) {
    v1 = *p1++;
    v2 = *p2++;

    if(v1 > v2)
      return 1;

    if(v1 < v2)
      return -1;

    // v1 == v2 == 0
    if(v1 == 0)
      return 0;
  }
}

unsigned int
strtonum(const char *str, const char **endptr)
{
  unsigned int result = 0;
  char *p = (char *)str;
  int base = 10;

  // check if 0x
  if(str[0] == '0' && str[1] == 'x') {
    base = 16;
    p += 2;
  }

  char c;
  while(1) {
    c = *p;

    if(base == 10) {
      if(c >= '0' && c <= '9') {
        result = result * base + c - '0';
      } else {
        break;
      }
    }

    if(base == 16) {
      if(c >= '0' && c <= '9') {
        result = result * base + c - '0';
      } else if(c >= 'a' && c <= 'f') {
        result = result * base + c - 'a' + 10;
      } else {
        break;
      }
    }

    p++;
  }

  if(endptr != NULL) {
    *endptr = p;
  }

  return result;
}

size_t
strlcat(char *dst, const char *src, size_t maxsize)
{
  size_t dst_length = strlen(dst);
  size_t src_length = strlen(src);
  size_t result = dst_length + src_length;

  if(maxsize <= dst_length + 1)
    return result;

  char *p = dst + dst_length;
  char *s = (char *)src;

  size_t limit = maxsize - dst_length - 1;
  if(limit > src_length) {
    limit = src_length;
  }

  for(size_t i = 0; i < limit; i++) {
    *p++ = *s++;
  }

  *p = 0;

  return result;
}
