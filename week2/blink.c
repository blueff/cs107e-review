#include <stdint.h>

#define DELAY 0x3F0000

static volatile uint32_t *FSEL2 = (uint32_t *)0x20200008;
static volatile uint32_t *SET0 = (uint32_t *)0x2020001c;
static volatile uint32_t *CLR0 = (uint32_t *)0x20200028;

int main() {
  volatile int  i;

  // configure GPIO 20 for output
  *FSEL2 = 1;

  while(1) {
    // set GPIO 20 high
    *SET0 = 1 << 20;

    for(i = 0; i < DELAY; i++); // sleep

    // set GPIO 20 low
    *CLR0 = 1 << 20;

    for(i = 0; i < DELAY; i++); // sleep
  }
}

