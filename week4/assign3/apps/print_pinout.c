#include "uart.h"
#include "timer.h"
#include "gpio.h"
#include "printf.h"

typedef unsigned int u32;

volatile u32 *FSEL2 = (u32 *)0x20200008;

void main(void) {
  uart_init();

  printf("Hello there!\n");
  timer_delay(1);

  printf("FSEL2 value: 0x%x\n", *FSEL2);

  gpio_set_output(20);
  gpio_set_output(21);

  printf("FSEL2 value: 0x%x\n", *FSEL2);
  uart_putchar(EOT);
}
