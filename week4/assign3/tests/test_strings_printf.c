#include <timer.h>
#include "../strings.h"
#include "../printf.h"
#include "../printf_internal.h"

#define assert(x) if(!(x)) abort()

#define GPIO_FSEL3  ((unsigned int *)0x2020000c)
#define GPIO_SET1   ((unsigned int *)0x20200020)
#define GPIO_CLR1   ((unsigned int *)0x2020002c)

// Red power LED (on Pi board) is GPIO 35.
#define ABORT_OUTPUT (1 << (3*5))
#define ABORT_BIT    (1 << (35-32))

void abort(void) {
  *GPIO_FSEL3 = ABORT_OUTPUT;
  while (1) {
    *GPIO_SET1 = ABORT_BIT;
    timer_delay_ms(100);
    *GPIO_CLR1 = ABORT_BIT;
    timer_delay_ms(100);
  }
}

void test_strings() {
  // memset
  {
    char buf[10];
    void *result = memset(buf, 'a', 5);
    assert(result == buf);
    for(int i = 0; i < 5; i++) {
      assert(buf[i] == 'a');
    }
  }

  // memcpy
  {
    char dst[10];
    char src[10] = "abcdefg";

    {
      void *result = memcpy(dst, src, 1);
      assert(result == dst);
      assert(dst[0] = 'a');
    }

    {
      void *result = memcpy(dst + 1, src + 1, 3);
      assert(result == dst + 1);
      assert(dst[1] = 'b');
      assert(dst[2] = 'c');
      assert(dst[3] = 'd');
    }
  }

  // strlen
  {
    assert(strlen("") == 0);
    assert(strlen("abc") == 3);
  }

  // strcmp
  {
    assert(strcmp("", "") == 0);
    assert(strcmp("a", "a") == 0);
    assert(strcmp("abcd", "abcd") == 0);

    assert(strcmp("a", "b") < 0);
    assert(strcmp("hello a", "hello b") < 0);
    assert(strcmp("b", "a") > 0);
    assert(strcmp("hello b", "hello a") > 0);

    char buf1[10] = {};
    char buf2[10] = {};

    buf1[0] = 10;
    buf2[0] = 20;
    assert(strcmp(buf1, buf2) < 0);

    buf1[0] = 30;
    buf2[0] = 20;
    assert(strcmp(buf1, buf2) > 0);

    buf1[0] = -1;
    buf2[0] = 20;
    assert(strcmp(buf1, buf2) > 0);

    buf1[0] = 1;
    buf2[0] = -2;
    assert(strcmp(buf1, buf2) < 0);

    buf1[0] = -1;
    buf2[0] = -2;
    assert(strcmp(buf1, buf2) > 0);
  }

  // strtonum
  {
    const char *endptr;
    unsigned int value;

    char *src = "123";
    value = strtonum(src, &endptr);
    assert(value == 123 && endptr == src + 3);

    src = "0x60";
    value = strtonum(src, &endptr);
    assert(value == 96 && endptr == src + 4);

    src = "0xabc";
    value = strtonum(src, &endptr);
    assert(value == 2748 && endptr == src + 5);

    src = "abcd";
    value = strtonum(src, &endptr);
    assert(value == 0 && endptr == src + 0);

    src = " 123";
    value = strtonum(src, &endptr);
    assert(value == 0 && endptr == src + 0);

    src = "0X60";
    value = strtonum(src, &endptr);
    assert(value == 0 && endptr == src + 1);

    src = "0xAB";
    value = strtonum(src, &endptr);
    assert(value == 0 && endptr == src + 2);

    src = "+100";
    value = strtonum(src, &endptr);
    assert(value == 0 && endptr == src);

    src = "100";
    value = strtonum(src, NULL);
    assert(value == 100);
  }

  // strlcat
  {
    size_t result;

    {
      char dst[10] = {};
      dst[0] = 'x';
      result = strlcat(dst, "hello", 10);
      assert(result == 6);
      assert(strcmp(dst, "xhello") == 0);
    }

    {
      char dst[10] = {};
      dst[0] = 'a';
      dst[1] = 'b';
      result = strlcat(dst, "xy", 10);
      assert(result == 4);
      assert(strcmp(dst, "abxy") == 0);
    }

    {
      char dst[10] = {};
      dst[0] = 'x';
      char *src = "too much string";
      result = strlcat(dst, src, 10);
      assert(result == strlen(src) + 1);
      assert(strcmp(dst, "xtoo much") == 0);
    }

    {
      char dst[10] = {};
      dst[0] = 'x';
      char *src = "a string";
      result = strlcat(dst, src, 0);
      assert(result == strlen(src) + 1);
      assert(strcmp(dst, "x") == 0);
    }
  }
}

void test_helpers(void) {
  // unsigned_to_base
  {
    char buf[20];
    int result;

    result = unsigned_to_base(buf, sizeof(buf), 64, 10, 2);
    assert(result = 4);
    assert(strcmp(buf, "64") == 0);

    result = unsigned_to_base(buf, sizeof(buf), 64, 10, 4);
    assert(result = 4);
    assert(strcmp(buf, "0064") == 0);

    result = unsigned_to_base(buf, sizeof(buf), 0, 10, 0);
    assert(result = 1);
    assert(strcmp(buf, "0") == 0);

    result = unsigned_to_base(buf, sizeof(buf), 64, 16, 4);
    assert(result = 4);
    assert(strcmp(buf, "0040") == 0);

    result = unsigned_to_base(buf, sizeof(buf), 64, 16, 1);
    assert(result = 2);
    assert(strcmp(buf, "40") == 0);

    result = unsigned_to_base(buf, 2, 64, 10, 4);
    assert(result = 4);
    assert(strcmp(buf, "0") == 0);

    result = unsigned_to_base(buf, 4, 64, 10, 4);
    assert(result = 4);
    assert(strcmp(buf, "006") == 0);

    result = unsigned_to_base(buf, 1, 64, 10, 4);
    assert(result = 4);
    assert(strcmp(buf, "") == 0);

    buf[0] = 'a';
    buf[1] = 0;
    result = unsigned_to_base(buf, 0, 64, 10, 4);
    assert(result = 4);
    assert(strcmp(buf, "a") == 0);
  }

  // signed_to_base
  {
    char buf[20];
    int result;

    result = signed_to_base(buf, sizeof(buf), 35, 10, 0);
    assert(result == 2);
    assert(strcmp(buf, "35") == 0);

    result = signed_to_base(buf, sizeof(buf), 35, 10, 3);
    assert(result == 3);
    assert(strcmp(buf, "035") == 0);

    result = signed_to_base(buf, sizeof(buf), 160, 16, 1);
    assert(result == 2);
    assert(strcmp(buf, "a0") == 0);

    result = signed_to_base(buf, sizeof(buf), 100, 16, 5);
    assert(result == 5);
    assert(strcmp(buf, "00064") == 0);

    result = signed_to_base(buf, sizeof(buf), -200, 16, 5);
    assert(result == 5);
    assert(strcmp(buf, "-00c8") == 0);

    result = signed_to_base(buf, sizeof(buf), -1, 10, 0);
    assert(result == 2);
    assert(strcmp(buf, "-1") == 0);

    result = signed_to_base(buf, 3, 1234, 10, 2);
    assert(result == 4);
    assert(strcmp(buf, "12") == 0);

    result = signed_to_base(buf, 5, -9999, 10, 6);
    assert(result == 6);
    assert(strcmp(buf, "-099") == 0);

    buf[0] = '1';
    buf[1] = 0;
    result = signed_to_base(buf, 0, 12345, 10, 10);
    assert(result == 10);
    assert(strcmp(buf, "1") == 0);
  }
}

void test_snprintf(void) {
  int result;

  // no formatting codes
  {
    char buf[10];
    char *str = "hello";
    result = snprintf(buf, sizeof(buf), str);
    assert(result == strlen(str));
    assert(strcmp(buf, str) == 0);

    str = "hellohellohello";
    result = snprintf(buf, sizeof(buf), str);
    assert(result == strlen(str));
    assert(strcmp(buf, "hellohell") == 0);
  }

  // %%, %c
  {
    char buf[10];
    result = snprintf(buf, sizeof(buf), "hello%%");
    assert(result == 6);
    assert(strcmp(buf, "hello%") == 0);

    result = snprintf(buf, sizeof(buf), "hello%c%c", 'A', 'B');
    assert(result == 7);
    assert(strcmp(buf, "helloAB") == 0);

    result = snprintf(buf, sizeof(buf), "%c%c = 100%% fresh", 'C', 'S');
    assert(result == 15);
    assert(strcmp(buf, "CS = 100%") == 0);
  }

  // %s
  {
    char buf[20];
    result = snprintf(buf, sizeof(buf), "hello:%s", "world");
    assert(result == 11);
    assert(strcmp(buf, "hello:world") == 0);

    result = snprintf(buf, sizeof(buf), "hello, %s, %s", "world", "good morning!");
    assert(result == 27);
    assert(strcmp(buf, "hello, world, good ") == 0);

    {
      char buf[11];
      result = snprintf(buf, sizeof(buf), "hello%s", "world");
      assert(result == 10);
      assert(strcmp(buf, "helloworld") == 0);
    }
  }

  // plain %d, %x
  {
    char buf[5];

    result = snprintf(buf, sizeof(buf), "%d", 100);
    assert(result == 3);
    assert(strcmp(buf, "100") == 0);

    result = snprintf(buf, sizeof(buf), "n:%d", 123);
    assert(result == 5);
    assert(strcmp(buf, "n:12") == 0);

    result = snprintf(buf, sizeof(buf), "%x", 64);
    assert(result == 2);
    assert(strcmp(buf, "40") == 0);

    result = snprintf(buf, sizeof(buf), "n:%x", 2048);
    assert(result == 5);
    assert(strcmp(buf, "n:80") == 0);
  }

  // %d, %x with width
  {
    char buf[10];

    result = snprintf(buf, sizeof(buf), "%1d", 100);
    assert(result == 3);
    assert(strcmp(buf, "100") == 0);

    result = snprintf(buf, sizeof(buf), "%5d", 100);
    assert(result == 5);
    assert(strcmp(buf, "00100") == 0);

    result = snprintf(buf, sizeof(buf), "%1x", 64);
    assert(result == 2);
    assert(strcmp(buf, "40") == 0);

    result = snprintf(buf, sizeof(buf), "%5x", 128);
    assert(result == 5);
    assert(strcmp(buf, "00080") == 0);
  }

  // %d, %x with invalid numbers
  {
    char buf[10];

    result = snprintf(buf, sizeof(buf), "%1yzd");
    assert(result == 5);
    assert(strcmp(buf, "%1yzd") == 0);

    result = snprintf(buf, sizeof(buf), "%20mx");
    assert(result == 5);
    assert(strcmp(buf, "%20mx") == 0);
  }

  // %p
  {
    char buf[15];

    result = snprintf(buf, sizeof(buf), "%p", (void *)0xa);
    assert(result == 10);
    assert(strcmp(buf, "0x0000000a") == 0);

    result = snprintf(buf, sizeof(buf), "%p", (void *)0xabcd);
    assert(result == 10);
    assert(strcmp(buf, "0x0000abcd") == 0);

    result = snprintf(buf, sizeof(buf), "addr:%p", (void *)0x12345);
    assert(result == 15);
    assert(strcmp(buf, "addr:0x0001234") == 0);
  }

  // invalid conversions
  {
    char buf[10];
    result = snprintf(buf, sizeof(buf), "%a");
    assert(result == 2);
    assert(strcmp(buf, "%a") == 0);
  }

  // overall
  {
    char buf[1024];
    int result;

    snprintf(buf, sizeof(buf), "hello, this is a char: %c, a string: %s, an integer: %4d, a hex: %6x, a pointer: %p, and %%", 'X', "hello", 100, 200, (void *)0x123);
    assert(strcmp(buf, "hello, this is a char: X, a string: hello, an integer: 0100, a hex: 0000c8, a pointer: 0x00000123, and %") == 0);

    buf[0] = 'x';
    result = snprintf(buf, 0, "abcde");
    assert(result == 5);
    assert(buf[0] == 'x');
  }

  // %b
  {
    char buf[5];

    // normal
    result = snprintf(buf, sizeof(buf), "%b", 7);
    assert(result == 3);
    assert(strcmp(buf, "111") == 0);

    result = snprintf(buf, sizeof(buf), "n:%b", 10);
    assert(result == 6);
    assert(strcmp(buf, "n:10") == 0);

    // with width
    result = snprintf(buf, sizeof(buf), "%4b", 2);
    assert(result == 4);
    assert(strcmp(buf, "0010") == 0);

    // invalid width number
    result = snprintf(buf, sizeof(buf), "%1mb");
    assert(result == 4);
    assert(strcmp(buf, "%1mb") == 0);
  }
}

void main(void) {
  test_strings();
  test_helpers();
  test_snprintf();
}
