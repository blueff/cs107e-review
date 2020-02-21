#include "../basic.h"
#include "../gpio.h"
#include "../timer.h"

// Bit 0 (the least significant) will represent segment A, bit 1 segment B...
// Bit 7 DP
static u8 bit_patterns[] = {
  0x3f, // 0 0b00111111
  0x30, // 1 0b00110000
  0x5b, // 2 0b01011011
  0x4f, // 3 0b01001111
  0x66, // 4 0b01100110
  0x6d, // 5 0b01101101
  0x7d, // 6 0b01111101
  0x07, // 7 0b00000111
  0x7f, // 8 0b01111111
  0x6f, // 9 0b01101111
  0x77, // A 0b01110111
  0x7f, // B 0b01111111
  0x39, // C 0b00111001
  0x3f, // D 0b00111111
  0x79, // E 0b01111001
  0x71, // F 0b01110001
  0x40, // - 0b01000000
};

static void clear() {
  for(int i = 20; i <= 27; i++) {
    gpio_write(i, 0);
  }

  for(int i = 10; i <= 13; i++) {
    gpio_write(i, 0);
  }
}

static void display_char(int digit_index, char c) {
  clear();

  gpio_write(10 + digit_index, 1);

  u8 pattern = 0;

  if(c == '0') {
    pattern = 0x3f;
  } else if(c == '1') {
    pattern = 0x30;
  } else if(c == '2') {
    pattern = 0x5b;
  } else if(c == '3') {
    pattern = 0x4f;
  } else if(c == '4') {
    pattern = 0x66;
  } else if(c == '5') {
    pattern = 0x6d;
  } else if(c == '6') {
    pattern = 0x7d;
  } else if(c == '7') {
    pattern = 0x07;
  } else if(c == '8') {
    pattern = 0x7f;
  } else if(c == '9') {
    pattern = 0x6f;
  } else if(c == '-') {
    pattern = 0x40;
  }

  /* switch(pattern_index) { */
  /*   case 0: { */
  /*     pattern = 0x3f; */
  /*   } break; */

  /*   case 1: { */
  /*     pattern = 0x30; */
  /*   } break; */

  /*   case 2: { */
  /*     pattern = 0x5b; */
  /*   } break; */

  /*   case 3: { */
  /*     pattern = 0x4f; */
  /*   } break; */

  /*   case 4: { */
  /*     pattern = 0x66; */
  /*   } break; */

  /*   case 5: { */
  /*     pattern = 0x6d; */
  /*   } break; */

  /*   case 6: { */
  /*     pattern = 0x7d; */
  /*   } break; */

  /*   case 7: { */
  /*     pattern = 0x07; */
  /*   } break; */

  /*   case 8: { */
  /*     pattern = 0x7f; */
  /*   } break; */

  /*   case 9: { */
  /*     pattern = 0x6f; */
  /*   } break; */
  /* } */

  for(int i = 0; i < 8; i++) {
    if(pattern & (1 << i)) {
      gpio_write(20 + i , 1);
    }
  }
}

static void display_number(int digit_index, int num) {
  display_char(digit_index, '0' + num);
}

void main(void) {
  // set GPIO 20 ~ 27 to output
  for(int i = 20; i <= 27; i++) {
    gpio_set_output(i);
  }

  // set GPIO 10 ~ 13 to output
  for(int i = 10; i <= 13; i++) {
    gpio_set_output(i);
  }

  u32 ticks = timer_get_ticks();

  // wait 1 second
  while(timer_get_ticks() - ticks < 1000000) {
    for(int digit_index = 0; digit_index < 4; digit_index++) {
      display_char(digit_index, '-');
      timer_delay_us(2500);
    }
  }

  u32 total_seconds = 0;

  while(1) {
    for(int i = 0; i < 100; i++) {
      u32 minutes = total_seconds / 60;
      u32 seconds = total_seconds % 60;

      display_number(0, minutes / 10);
      timer_delay_us(2500);

      display_number(1, minutes % 10);
      timer_delay_us(2500);

      display_number(2, seconds / 10);
      timer_delay_us(2500);

      display_number(3, seconds % 10);
      timer_delay_us(2500);
    }

    total_seconds += 1;
  }
}
