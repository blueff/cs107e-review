#include <timer.h>
#include <keyboard.h>
#include <printf.h>
#include <uart.h>
#include <gpio.h>
#include <interrupts.h>

#define ESC_SCANCODE 0x76

void
main(void)
{
  gpio_init();
  uart_init();

#if 1
  interrupts_init();
  keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
  keyboard_use_interrupts();
  interrupts_global_enable();
#else
  keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
#endif

  while(1) {
    char c = keyboard_read_next();

    if(c >= '\t' && c <= 0x80) {
      printf("%c", c);
    } else {
      printf("[%02x]", c);
    }

    if(c == ps2_keys[ESC_SCANCODE].ch)
      break;

    timer_delay_ms(300);
  }

  uart_putchar(EOT);
}
