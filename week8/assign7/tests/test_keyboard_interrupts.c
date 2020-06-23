#include <timer.h>
#include <keyboard.h>
#include <printf.h>
#include <uart.h>

#define ESC_SCANCODE 0x76

void
main(void)
{
  uart_init();
  keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);

  while(1) {
    char c = keyboard_read_next();

    if(c >= '\t' && c <= 0x80) {
      printf("%c", c);
    } else {
      printf("[%02x]", c);
    }

    if(c == ps2_keys[ESC_SCANCODE].ch)
      break;

    timer_delay(1);
  }

  uart_putchar(EOT);
}
