#ifndef __Registers_H__
#define __Registers_H__
#include <stdint.h>

#include "Host.h"

#define RCC_BASE_ADDRESS   ((uint32_t)HostRcc)
#define GPIOG_BASE_ADDRESS ((uint32_t)HostGpioG)
#define GPIOA_BASE_ADDRESS ((uint32_t)HostGpioA)
#define TIM2_BASE_ADDRESS  ((uint32_t)HostTim2)
#define DMA1_BASE_ADDRESS  ((uint32_t)HostDma1)
#define DMA1_Channel3_BASE_ADDRESS ((uint32_t)HostDma1_Channel3)
#define NVIC_BASE_ADDRESS ((uint32_t)HostNVIC)

#endif //__Registers_H__