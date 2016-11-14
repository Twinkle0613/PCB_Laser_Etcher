#ifndef Gpio_H
#define Gpio_H
#include <stdint.h>
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void GpioConfig(GPIO_TypeDef* GPIOx, uint16_t Pin, int Speed, int Mode);

#endif // Gpio_H
