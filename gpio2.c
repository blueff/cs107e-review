#include "gpio.h"

#define base 0x3F200000

struct gpio{
  unsigned int FSEL0;
  unsigned int FSEL1;
  unsigned int FSEL2;
  unsigned int FSEL3;
  unsigned int FSEL4;
  unsigned int FSEL5;
  unsigned int res1;
  unsigned int SET0;
  unsigned int SET1;
  unsigned int res2;
  unsigned int CLR0;
  unsigned int CLR1;
  unsigned int res3;
  unsigned int LEV0;
  unsigned int LEV1;
  unsigned int res4;
  unsigned int EDS0;
  unsigned int EDS1;
  unsigned int res5;
  unsigned int REN0;
  unsigned int REN1;
  unsigned int res6;
  unsigned int FEN0;
  unsigned int FEN1;
};

static volatile struct gpio* gpio_register;

static volatile unsigned int* calculate_FSEL_register(unsigned int pin){
  unsigned volatile int* reg = 0;
    unsigned int val = (pin/10);
    switch(val){
        case 0:
            reg = &(gpio_register->FSEL0);
            break;
        case 1:
            reg = &(gpio_register->FSEL1);
            break;
        case 2:
            reg = &(gpio_register->FSEL2);
            break;
        case 3:
            reg = &(gpio_register->FSEL3);
            break;
        case 4:
            reg = &(gpio_register->FSEL4);
            break;
        case 5:
            reg = &(gpio_register->FSEL5);
            break;
        default:
            break;
    }
//    if (pin >= GPIO_PIN_FIRST && pin <= GPIO_PIN_LAST){
//      if (pin < 10 ){
//        reg = &(gpio_register->FSEL0);
//      }
//      else if (pin < 20){
//        reg = &(gpio_register->FSEL1);
//      }
//      else if (pin < 30){
//        reg = &(gpio_register->FSEL2);
//      }
//      else if (pin < 40){
//        reg = &(gpio_register->FSEL3);
//      }
//      else if (pin < 50){
//        reg = &(gpio_register->FSEL4);
//      }
//      else{
//        reg = &(gpio_register->FSEL5);
//     }
//  }
  return reg;
}

static unsigned int pin_to_bit_map(unsigned int pin){
  return pin % 32;
}

static volatile unsigned int* pin_to_set_register_map(unsigned int pin){
//  return (pin < 32) ? &gpio_register->SET0 : &gpio_register->SET1;
  if(pin < 32){
    return &(gpio_register->SET0);
  }
  return &(gpio_register->SET1);
}

static volatile unsigned int* pin_to_clear_register_map(unsigned int pin){
  //return (pin < 32) ? &gpio_register->CLR0 : &gpio_register->CLR1;
  if(pin < 32){
    return &(gpio_register->CLR0);
  }
  return &(gpio_register->CLR1);

}

static volatile unsigned int* pin_to_lev_register_map(unsigned int pin){
  //return (pin < 32) ? &gpio_register->LEV0 : &gpio_register->LEV1;
  if(pin < 32){
    return &(gpio_register->LEV0);
  }
  return &(gpio_register->LEV1);

}

void gpio_init(void){
  gpio_register = (struct gpio*)base;
}


void gpio_set_function(unsigned int pin, unsigned int function){
  unsigned int offset = 0;
  unsigned volatile int *reg;
  if (pin >= GPIO_PIN_FIRST && pin <= GPIO_PIN_LAST && function >=0 && function <= 7){
    if ( (reg = calculate_FSEL_register(pin)) ){
      offset = pin % 10;
      *reg = (*reg | ( function << ( offset * 3) ));
    }
  }
}


unsigned int gpio_get_function(unsigned int pin){
  unsigned int offset = 0;
  unsigned volatile int* reg = calculate_FSEL_register(pin);
  if(reg){
    offset = pin % 10;
    return (*reg >> (offset * 3)) & 7;
  }
  return GPIO_INVALID_REQUEST;
}



void gpio_set_input(unsigned int pin){
  gpio_set_function(pin, GPIO_FUNC_INPUT);
}

void gpio_set_output(unsigned int pin){
  gpio_set_function(pin,GPIO_FUNC_OUTPUT);
}

void gpio_write(unsigned int pin, unsigned int val){
  unsigned volatile int* reg;
  unsigned int offset = pin_to_bit_map(pin);
  if(val){
    reg = pin_to_set_register_map(pin);
  }else{
    reg = pin_to_clear_register_map(pin);
  }
  *reg = 1 << offset;
}

unsigned int gpio_read(unsigned int pin){
  unsigned volatile int* reg = pin_to_lev_register_map(pin);
  unsigned offset = pin_to_bit_map(pin);
  return (*reg >> offset)& 1;
}
