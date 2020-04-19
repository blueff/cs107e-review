#include <timer.h>

#define GPIO_FSEL3 ((unsigned int *)0x2020000c)
#define GPIO_SET1 ((unsigned int *)0x20200020)
#define GPIO_CLR1 ((unsigned int *)0x2020002c)

// Red power LED (on Pi board) is GPIO 35.
#define ABORT_OUTPUT (1 << (3 * 5))
#define ABORT_BIT (1 << (35 - 32))

void
abort(void) {
  *GPIO_FSEL3 = ABORT_OUTPUT;
  while(1) {
    *GPIO_SET1 = ABORT_BIT;
    timer_delay_ms(100);
    *GPIO_CLR1 = ABORT_BIT;
    timer_delay_ms(100);
  }
}
