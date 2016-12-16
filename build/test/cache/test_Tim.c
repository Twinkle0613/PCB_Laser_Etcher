#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "peripherals.h"
#include "Registers.h"
#include "Host.h"
#include "Tim.h"
#include "unity.h"




void setUp(void)

{

  HostTim2= malloc((sizeof(TIM_TypeDef)));



}



void tearDown(void)

{

  free(HostTim2);

  HostTim2 = ((void *)0);

}





void test_TIM_ClearITPendingBit_clear_the_IT_update_flag(void){

  TIM_ClearITPendingBit(((TIM_TypeDef*) ((uint32_t)HostTim2)),((uint16_t)0x0001));

  UnityAssertEqualNumber((_U_SINT)((0xfffe)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->SR)), (((void *)0)), (_U_UINT)(30), UNITY_DISPLAY_STYLE_INT);

}
