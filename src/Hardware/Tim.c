#include "Tim.h"

void TimerConfig(TIM_TypeDef* TIMx,
                uint16_t Prescaler,
                uint16_t CounterMode,
                uint16_t Period,
                uint16_t priority
              ){

	if(TIMx==TIM1)     RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	else if(TIMx==TIM2)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	else if(TIMx==TIM3)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	else if(TIMx==TIM4)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	else if(TIMx==TIM5)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	else if(TIMx==TIM6)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	else if(TIMx==TIM7)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	else if(TIMx==TIM8)RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	int TIMx_IRQn;
	if(TIMx==TIM2){TIMx_IRQn = TIM2_IRQn;}
	else if(TIMx==TIM3){TIMx_IRQn = TIM3_IRQn;}
	else if(TIMx==TIM4){TIMx_IRQn = TIM4_IRQn;}
	else if(TIMx==TIM5){TIMx_IRQn = TIM5_IRQn;}


	NVIC_SetPriority(TIMx_IRQn,priority);
	NVIC_EnableIRQ(TIMx_IRQn);
    TIM_TimeBaseInitTypeDef timer;
    timer.TIM_Prescaler = Prescaler;
    timer.TIM_CounterMode = CounterMode;
    timer.TIM_Period = Period;
    timer.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIMx,&timer);
    TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Regular);
    TIM_ARRPreloadConfig(TIMx,DISABLE);
    TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
    TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);
}
