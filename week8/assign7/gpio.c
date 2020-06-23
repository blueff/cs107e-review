#include <gpio.h>

#define GPIO_BASE (unsigned int *)0x20200000
#define GPIO_FUNC_FIRST 0
#define GPIO_FUNC_LAST 7

static volatile unsigned int *
get_fsel_register(unsigned int pin)
{
  return GPIO_BASE + (pin / 10);
}

static volatile unsigned int *
get_set_register(unsigned int pin)
{
  return GPIO_BASE + (0x1c / 4) + (pin / 32);
}

static volatile unsigned int *
get_clear_register(unsigned int pin)
{
  return GPIO_BASE + (0x28 / 4) + (pin / 32);
}

static volatile unsigned int *
get_level_register(unsigned int pin)
{
  return GPIO_BASE + (0x34 / 4) + (pin / 32);
}

static int
is_pin_valid(unsigned int pin)
{
  return (pin >= GPIO_PIN_FIRST && pin <= GPIO_PIN_LAST);
}

static int
is_function_valid(unsigned int function)
{
  return (function >= GPIO_FUNC_FIRST && function <= GPIO_FUNC_LAST);
}

void
gpio_init(void)
{
}

// If `pin` or `function` is invalid, does nothing.
// 0bxxx is a GCC extension
void
gpio_set_function(unsigned int pin, unsigned int function)
{
  if(!is_pin_valid(pin) || !is_function_valid(function))
    return;

  volatile unsigned int *reg = get_fsel_register(pin);
  int offset = (pin % 10) * 3;
  unsigned int value = *reg;
  value = value & (~(0b111 << offset));
  *reg = value | (function << offset);
}

unsigned int
gpio_get_function(unsigned int pin)
{
  if(!is_pin_valid(pin))
    return -1;

  volatile unsigned int *reg = get_fsel_register(pin);
  int offset = (pin % 10) * 3;
  return (*reg >> offset) & 0b111;
}

void
gpio_set_input(unsigned int pin)
{
  gpio_set_function(pin, GPIO_FUNC_INPUT);
}

void
gpio_set_output(unsigned int pin)
{
  gpio_set_function(pin, GPIO_FUNC_OUTPUT);
}

void
gpio_write(unsigned int pin, unsigned int val)
{
  if(!is_pin_valid(pin) || !(val == 0 || val == 1)) {
    return;
  }

  volatile unsigned int *reg
    = val == 1 ? get_set_register(pin) : get_clear_register(pin);
  int offset = pin % 32;
  *reg = (1 << offset);
}

unsigned int
gpio_read(unsigned int pin)
{
  if(!is_pin_valid(pin)) {
    return -1;
  }

  volatile unsigned int *reg = get_level_register(pin);
  int offset = pin % 32;
  return (*reg >> offset) & 0x1;
}
