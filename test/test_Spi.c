#include "unity.h"
#include "Spi.h"

//For Test Driver Development
#include "Host.h"
#include "Registers.h"
#include "peripherals.h"
//Included library by Following Spi.h 
#include<stdint.h>
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "malloc.h"
void setUp(void)
{
  HostGpioA = malloc((sizeof(GPIO_TypeDef)));
}

void tearDown(void)
{
  free(HostGpioA);
  HostGpioA = NULL;
}

void test_triggerOutputData_control_the_NSS_PIN(void){
   GPIOA->BSRR = 0x0000; // Reset
   GPIOA->BRR = 0x0000;  // Reset
   triggerOutputData();
   TEST_ASSERT_EQUAL(0x0010,GPIOA->BRR);
   TEST_ASSERT_EQUAL(0x0010,GPIOA->BSRR);
}
