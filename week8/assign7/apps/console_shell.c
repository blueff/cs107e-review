#include <gpio.h>
#include <keyboard.h>
#include <console.h>
#include <interrupts.h>
#include <shell.h>

#define NUM_ROWS 40
#define NUM_COLS 80

void
main(void)
{
  gpio_init();
  interrupts_init();
  keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
  keyboard_use_interrupts();
  console_init(NUM_ROWS, NUM_COLS);
  interrupts_global_enable();

  shell_init(console_printf);
  shell_run();
}
