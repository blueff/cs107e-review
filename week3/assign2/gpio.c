#include "basic.h"
#include "gpio.h"

#define GPIO_BASE (u32 *)0x20200000

static volatile u32* get_fsel_register(uint pin){
  return GPIO_BASE + (pin / 10);
}

static volatile u32* get_set_register(uint pin){
  return GPIO_BASE + (0x1c / 4) +  (pin / 32);
}

static volatile u32* get_clear_register(uint pin){
  return GPIO_BASE + (0x28 / 4) +  (pin / 32);
}

static volatile u32* get_level_register(uint pin){
  return GPIO_BASE + (0x34 / 4) +  (pin / 32);
}

static bool is_pin_valid(uint pin) {
  return (pin >= GPIO_PIN_FIRST && pin <= GPIO_PIN_LAST);
}

static bool is_function_valid(uint function) {
  return (function >= GPIO_FUNC_FIRST && function <= GPIO_FUNC_LAST);
}

void gpio_init(void) {
}

// If `pin` or `function` is invalid, does nothing.
void gpio_set_function(uint pin, unsigned int function) {
  if(!is_pin_valid(pin) || !is_function_valid(function)) {
    return;
  }

  volatile u32 *reg = get_fsel_register(pin);
  int offset = (pin % 10) * 3;
  u32 value = *reg;
  value = value & (~(0x111 << offset));
  *reg = value | (function << offset);
}

int gpio_get_function(uint pin) {
  if(!is_pin_valid(pin)) {
    return -1;
  }

  volatile u32 *reg = get_fsel_register(pin);
  int offset = pin % 10;
  return (*reg >> offset) & 0x111;
}

void gpio_set_input(uint pin) {
  gpio_set_function(pin, GPIO_FUNC_INPUT);
}

void gpio_set_output(uint pin) {
  gpio_set_function(pin, GPIO_FUNC_OUTPUT);
}

void gpio_write(uint pin, uint val) {
  if(!is_pin_valid(pin) || !(val == 0 || val == 1)) {
    return;
  }

  volatile u32 *reg = val == 1 ? get_set_register(pin) : get_clear_register(pin);
  int offset = pin % 32;
  *reg = (1 << offset);
}

int gpio_read(uint pin) {
  if(!is_pin_valid(pin)) {
    return -1;
  }

  volatile u32 *reg = get_level_register(pin);
  int offset = pin % 32;
  return (*reg >> offset) & 0x1;
}
