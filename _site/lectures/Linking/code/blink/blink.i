# 1 "blink.c"
# 1 "/Users/cj/Documents/cs107e-home/cs107e.github.io/lectures/Linking/code/blink//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "blink.c"
# 1 "gpio.h" 1
# 24 "gpio.h"
enum {
    GPIO_PIN_FIRST = 0,
    GPIO_PIN0 = 0,
    GPIO_PIN1 = 1,
    GPIO_PIN2 = 2,
    GPIO_PIN3 = 3,
    GPIO_PIN4 = 4,
    GPIO_PIN5 = 5,
    GPIO_PIN6 = 6,
    GPIO_PIN7 = 7,
    GPIO_PIN8 = 8,
    GPIO_PIN9 = 9,
    GPIO_PIN10 = 10,
    GPIO_PIN11 = 11,
    GPIO_PIN12 = 12,
    GPIO_PIN13 = 13,
    GPIO_PIN14 = 14,
    GPIO_PIN15 = 15,
    GPIO_PIN16 = 16,
    GPIO_PIN17 = 17,
    GPIO_PIN18 = 18,
    GPIO_PIN19 = 19,
    GPIO_PIN20 = 20,
    GPIO_PIN21 = 21,
    GPIO_PIN22 = 22,
    GPIO_PIN23 = 23,
    GPIO_PIN24 = 24,
    GPIO_PIN25 = 25,
    GPIO_PIN26 = 26,
    GPIO_PIN27 = 27,
    GPIO_PIN28 = 28,
    GPIO_PIN29 = 29,
    GPIO_PIN30 = 30,
    GPIO_PIN31 = 31,
    GPIO_PIN32 = 32,
    GPIO_PIN33 = 33,
    GPIO_PIN34 = 34,
    GPIO_PIN35 = 35,
    GPIO_PIN36 = 36,
    GPIO_PIN37 = 37,
    GPIO_PIN38 = 38,
    GPIO_PIN39 = 39,
    GPIO_PIN40 = 40,
    GPIO_PIN41 = 41,
    GPIO_PIN42 = 42,
    GPIO_PIN43 = 43,
    GPIO_PIN44 = 44,
    GPIO_PIN45 = 45,
    GPIO_PIN46 = 46,
    GPIO_PIN47 = 47,
    GPIO_PIN48 = 48,
    GPIO_PIN49 = 49,
    GPIO_PIN50 = 50,
    GPIO_PIN51 = 51,
    GPIO_PIN52 = 52,
    GPIO_PIN53 = 53,
    GPIO_PIN_LAST = 53
};
# 91 "gpio.h"
enum {
    GPIO_FUNC_INPUT = 0,
    GPIO_FUNC_OUTPUT = 1,
    GPIO_FUNC_ALT0 = 4,
    GPIO_FUNC_ALT1 = 5,
    GPIO_FUNC_ALT2 = 6,
    GPIO_FUNC_ALT3 = 7,
    GPIO_FUNC_ALT4 = 3,
    GPIO_FUNC_ALT5 = 2,
};
# 110 "gpio.h"
void gpio_init(void);
# 122 "gpio.h"
void gpio_set_function(unsigned int pin, unsigned int function);
# 133 "gpio.h"
unsigned int gpio_get_function(unsigned int pin);







void gpio_set_input(unsigned int pin);
void gpio_set_output(unsigned int pin);
# 155 "gpio.h"
void gpio_write(unsigned int pin, unsigned int val);
# 166 "gpio.h"
unsigned int gpio_read(unsigned int pin);
# 2 "blink.c" 2
# 1 "timer.h" 1
# 18 "timer.h"
void timer_init(void);
# 27 "timer.h"
unsigned int timer_get_ticks(void);






void timer_delay_us(unsigned int usec);






void timer_delay_ms(unsigned int msec);






void timer_delay(unsigned int sec);
# 3 "blink.c" 2

void main() {

  gpio_init();
  timer_init();

  gpio_set_function(GPIO_PIN20, GPIO_FUNC_OUTPUT);
  while(1) {
      gpio_write(GPIO_PIN20, 1);
      timer_delay(1);
      gpio_write(GPIO_PIN20, 0);
      timer_delay(1);
  }
}
