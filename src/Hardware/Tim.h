#ifndef Tim_H
#define Tim_H
#include <stdint.h>
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

void TimerConfig(TIM_TypeDef* TIMx,
                uint16_t Prescaler,
                uint16_t CounterMode,
                uint16_t Period,
                uint16_t priority
              );
#endif // Tim_H
