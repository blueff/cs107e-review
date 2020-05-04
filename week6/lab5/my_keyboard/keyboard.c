#include "gpio.h"
#include "gpioextra.h"
#include "keyboard.h"
#include "ps2.h"

static unsigned int CLK, DATA;

void
keyboard_init(unsigned int clock_gpio, unsigned int data_gpio) {
  CLK = clock_gpio;
  gpio_set_input(CLK);
  gpio_set_pullup(CLK);

  DATA = data_gpio;
  gpio_set_input(DATA);
  gpio_set_pullup(DATA);
}

static int
read_bit(void) {
  // falling edge on clock triggers read
  // wait until clock reads high, then wait until clock reads low
  // now read data
  while(gpio_read(CLK) == 0) {}

  while(gpio_read(CLK) == 1) {}

  return gpio_read(DATA);
}

unsigned char
keyboard_read_scancode(void) {
  unsigned char result = 0;

  while(1) {
    // Start bit must be low
    while(read_bit() != 0) {}

    for(int i = 0; i < 8; i++) { result |= (read_bit() << i); }

    // Discard parity and stop bit
    read_bit();
    read_bit();

    break;
  }

  return result;
}

key_action_t
keyboard_read_sequence(void) {
  key_action_t action;

  unsigned char keycode = keyboard_read_scancode();
  if(keycode == PS2_CODE_EXTENDED) {
    keycode = keyboard_read_scancode();
  }

  if(keycode == PS2_CODE_RELEASE) {
    action.what = KEY_RELEASE;
    action.keycode = keyboard_read_scancode();
  } else {
    action.what = KEY_PRESS;
    action.keycode = keycode;
  }

  return action;
}
