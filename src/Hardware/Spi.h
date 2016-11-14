#ifndef Spi_H
#define Spi_H
#include<stdint.h>
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define Input_Mode 0x0

#define  idle0_1Edge  0
#define  idle0_2Edge  1
#define  idle1_1Edge  2
#define  idle1_2Edge  3

//For SPI1
#define NSS_PIN  GPIO_Pin_4
#define SCLK_PIN GPIO_Pin_5
#define MISO_PIN GPIO_Pin_6
#define MOSI_PIN GPIO_Pin_7

void SpiConfig(SPI_TypeDef* SPIx,
              uint16_t Mode,
              uint16_t DataSize,
              uint16_t BaudRatePrescaler,
              uint16_t FirstBit,
              uint16_t spi_clock
              );

void triggerOutputData();

#endif // Spi_H
