#ifndef _PRINTF_INTERNAL_H
#define _PRINTF_INTERNAL_H

#include <stddef.h>

int unsigned_to_base(char *buf,
  size_t bufsize,
  unsigned int val,
  int base,
  int min_width);

int signed_to_base(char *buf, size_t bufsize, int val, int base, int min_width);

#endif
