#include <uart.h>
#include "../printf.h"
#include "../disassemble.h"

void
main(void) {
  uint32_t addr = 0x8000;
  uint32_t *p = (uint32_t *)addr;
  uint32_t value;

  char buf[1024*1024];

  for(int i = 0; i < 100; i++) {
    value = *p++;

    disassemble(buf, sizeof(buf), value, addr);

    printf("%x: %8x %s\n", addr, value, buf);

    addr += 4;
  }

  uart_putchar(EOT);
}