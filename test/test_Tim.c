#include "unity.h"
#include "Tim.h"


//For Test Driver Development
#include "Host.h"
#include "Registers.h"
#include "peripherals.h"
//Included library by Following Tim.h 
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "malloc.h"


void setUp(void)
{
  HostTim2= malloc((sizeof(TIM_TypeDef)));
  
}

void tearDown(void)
{
  free(HostTim2);
  HostTim2 = NULL;
}


void test_TIM_ClearITPendingBit_clear_the_IT_update_flag(void){
  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
  TEST_ASSERT_EQUAL(0xfffe,TIM2->SR);
}

