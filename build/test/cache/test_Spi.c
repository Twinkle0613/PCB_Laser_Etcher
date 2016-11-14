#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "peripherals.h"
#include "Registers.h"
#include "Host.h"
#include "Spi.h"
#include "unity.h"
void setUp(void)

{

  HostGpioA = malloc((sizeof(GPIO_TypeDef)));

}



void tearDown(void)

{

  free(HostGpioA);

  HostGpioA = ((void *)0);

}



void test_triggerOutputData_control_the_NSS_PIN(void){

   ((GPIO_TypeDef *) ((uint32_t)HostGpioA))->BSRR = 0x0000;

   ((GPIO_TypeDef *) ((uint32_t)HostGpioA))->BRR = 0x0000;

   triggerOutputData();

   UnityAssertEqualNumber((_U_SINT)((0x0010)), (_U_SINT)((((GPIO_TypeDef *) ((uint32_t)HostGpioA))->BRR)), (((void *)0)), (_U_UINT)(29), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((0x0010)), (_U_SINT)((((GPIO_TypeDef *) ((uint32_t)HostGpioA))->BSRR)), (((void *)0)), (_U_UINT)(30), UNITY_DISPLAY_STYLE_INT);

}
