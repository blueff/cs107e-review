#include "basic.h"
#include "timer.h"

#define TIMER_BASE ((u32 *)0x20003000)

void timer_init(void) {
}

u32 timer_get_ticks(void) {
  volatile u32 *reg = TIMER_BASE + 1;
  return *reg;
}

void timer_delay_us(unsigned int usec) {
  u32 current = timer_get_ticks();
  while((timer_get_ticks() - current) < usec) {}
}

void timer_delay_ms(unsigned int msec) {
  timer_delay_us(msec * 1000);
}

void timer_delay(unsigned int sec) {
  timer_delay_us(sec * 1000 * 1000);
}

