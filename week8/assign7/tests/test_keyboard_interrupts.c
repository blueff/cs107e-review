#include <timer.h>
#include <keyboard.h>
#include <printf.h>
#include <uart.h>
#include <gpio.h>
#include <interrupts.h>
#include <console.h>

#define ESC_SCANCODE 0x76

void
test_keyboard_interrupts(void)
{
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

void
test_console(void)
{
  console_init(20, 40);

  {
    console_printf("Hello World!");
  }

  {
    timer_delay(1);
    console_printf("\nOther line!");
  }

  {
    timer_delay(1);
    console_printf("\nLone long long long long long long long long long line!");
  }

  // Scroll
  {
    timer_delay(1);
    for(int i = 0; i < 16; i++) {
      console_printf("\nLine %d", i);
    }

    timer_delay(1);
    console_printf("\nScroll down");

    // `\b`, `\r`
    {
      timer_delay(1);
      console_printf("\nMove cursor back and carriage return");

      for(int i = 0; i < 5; i++) {
        timer_delay_ms(300);
        console_printf("\b");
      }

      timer_delay(1);
      console_printf("\r");
    }

    // `\f`
    {
      timer_delay(1);
      console_printf("\nform feed to clear all contents");
      console_printf("\f");
    }
  }
}

void
main(void)
{
  gpio_init();
  uart_init();

  // test_keyboard_interrupts();

  test_console();
}
