#include <gpio.h>
#include <gpioextra.h>
#include <keyboard.h>
#include <ps2.h>
#include <timer.h>

static unsigned int CLK, DATA;

static int
set_bit_count(int value)
{
  int result = 0;

  while(value) {
    result += value & 1;
    value >>= 1;
  }

  return result;
}

void
keyboard_init(unsigned int clock_gpio, unsigned int data_gpio)
{
  CLK = clock_gpio;
  gpio_set_input(CLK);
  gpio_set_pullup(CLK);

  DATA = data_gpio;
  gpio_set_input(DATA);
  gpio_set_pullup(DATA);
}

#define READ_BIT_TIMEOUT 3000 // 3ms
static int read_bit_timeout = 0;

static int
read_bit(void)
{
  static unsigned int last_tick = 0;

  // falling edge on clock triggers read
  // wait until clock reads high, then wait until clock reads low
  // now read data
  while(gpio_read(CLK) == 0) {}

  while(gpio_read(CLK) == 1) {}

  if(last_tick != 0) {
    read_bit_timeout = (timer_get_ticks() - last_tick) > READ_BIT_TIMEOUT;
  }

  last_tick = timer_get_ticks();

  return gpio_read(DATA);
}

// If the current clock edge occurred more than 3ms after the previous one,
// reset the state and assume the current clock edge is for a start bit.
unsigned char
keyboard_read_scancode(void)
{
  unsigned char result = 0;

  while(1) {
    // Start bit must be low
    while(read_bit() != 0) {}
  reset:
    result = 0;

    for(int i = 0; i < 8; i++) {
      result |= (read_bit() << i);
      if(read_bit_timeout) {
        goto reset;
      }
    }

    int parity = read_bit();
    if(read_bit_timeout) {
      goto reset;
    }

    int stop = read_bit();
    if(read_bit_timeout) {
      goto reset;
    }

    // Must be odd parity
    if((set_bit_count(result) + parity) % 2 != 1) {
      continue;
    }

    if(stop != 1) {
      continue;
    }

    break;
  }

  return result;
}

key_action_t
keyboard_read_sequence(void)
{
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

#define LEFT_SHIFT_SCANCODE 0x12
#define RIGHT_SHIFT_SCANCODE 0x59
#define ALT_SCANCODE 0x11
#define CTRL_SCANCODE 0x14
#define CAPS_LOCK_SCANCODE 0x58
#define SCROLL_LOCK_SCANCODE 0x7e
#define NUM_LOCK_SCANCODE 0x77

key_event_t
keyboard_read_event(void)
{
  static keyboard_modifiers_t modifiers = 0;
  key_event_t result = {};
  key_action_t action = keyboard_read_sequence();

  // Shift
  if(action.keycode == LEFT_SHIFT_SCANCODE
    || action.keycode == RIGHT_SHIFT_SCANCODE) {
    if(action.what == KEY_PRESS) {
      modifiers |= KEYBOARD_MOD_SHIFT;
    } else {
      modifiers &= ~KEYBOARD_MOD_SHIFT;
    }
  }

  // Alt
  if(action.keycode == ALT_SCANCODE) {
    if(action.what == KEY_PRESS) {
      modifiers |= KEYBOARD_MOD_ALT;
    } else {
      modifiers &= ~KEYBOARD_MOD_ALT;
    }
  }

  // Ctrl
  if(action.keycode == CTRL_SCANCODE) {
    if(action.what == KEY_PRESS) {
      modifiers |= KEYBOARD_MOD_CTRL;
    } else {
      modifiers &= ~KEYBOARD_MOD_CTRL;
    }
  }

  // Caps Lock
  if(action.keycode == CAPS_LOCK_SCANCODE) {
    if(action.what == KEY_PRESS) {
      if(modifiers & KEYBOARD_MOD_CAPS_LOCK) {
        modifiers &= ~KEYBOARD_MOD_CAPS_LOCK;
      } else {
        modifiers |= KEYBOARD_MOD_CAPS_LOCK;
      }
    }
  }

  // Scroll Lock
  if(action.keycode == SCROLL_LOCK_SCANCODE) {
    if(action.what == KEY_PRESS) {
      if(modifiers & KEYBOARD_MOD_SCROLL_LOCK) {
        modifiers &= ~KEYBOARD_MOD_SCROLL_LOCK;
      } else {
        modifiers |= KEYBOARD_MOD_SCROLL_LOCK;
      }
    }
  }

  // Num Lock
  if(action.keycode == NUM_LOCK_SCANCODE) {
    if(action.what == KEY_PRESS) {
      if(modifiers & KEYBOARD_MOD_NUM_LOCK) {
        modifiers &= ~KEYBOARD_MOD_NUM_LOCK;
      } else {
        modifiers |= KEYBOARD_MOD_NUM_LOCK;
      }
    }
  }

  result.action = action;
  result.key = ps2_keys[action.keycode];
  result.modifiers = modifiers;

  return result;
}

unsigned char
keyboard_read_next(void)
{
  unsigned char result = 0;
  key_event_t event;

  while(1) {
    event = keyboard_read_event();

    if(event.action.what == KEY_RELEASE)
      continue;

    if(event.key.ch == PS2_KEY_SHIFT || event.key.ch == PS2_KEY_ALT
      || event.key.ch == PS2_KEY_CTRL || event.key.ch == PS2_KEY_CAPS_LOCK)
      continue;

    // Ctrl-a, Ctrl-e, Ctrl-u
    // Only has control modifier
    if(event.modifiers == KEYBOARD_MOD_CTRL) {
      switch(event.key.ch) {
      case 'a':
        return 0x01;

      case 'e':
        return 0x05;

      case 'u':
        return 0x15;
      }
    }

    result = event.key.ch;

    if(event.key.other_ch != 0) {
      if(event.modifiers & KEYBOARD_MOD_SHIFT) {
        result = event.key.other_ch;
      }

      if(event.modifiers & KEYBOARD_MOD_CAPS_LOCK
        && (event.key.ch >= 'a' && event.key.ch <= 'z')) {
        result = event.key.other_ch;
      }
    }

    break;
  }

  return result;
}
