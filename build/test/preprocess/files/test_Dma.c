#include "Motor.h"
#include "HardwareConfig.h"
#include "Spi.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
#include "peripherals.h"
#include "Registers.h"
#include "Host.h"
#include "Dma.h"
#include "unity.h"






void initMotorDriveBuffer(void){

  int i;

  for(i = 0; i <= 2 ; i++){

    motorDriveBuffer[i] = 0;

  }

}



void initMotorInfoBuffer(void){

  int i;

  for(i = 0; i <= 5 ; i++){

    motorInfoBuffer[i] = i+1;

  }

}



void setUp(void)

{

  HostDma1_Channel3 = malloc((sizeof(DMA_Channel_TypeDef)));

  HostDma1 = malloc((sizeof(DMA_TypeDef)));

  HostNVIC = malloc((sizeof(NVIC_Type)));

  initMotorDriveBuffer();

  initMotorInfoBuffer();

}



void tearDown(void)

{

  free(HostDma1_Channel3);

  free(HostDma1);

  free(HostNVIC);

  HostNVIC = ((void *)0);

  HostDma1_Channel3 = ((void *)0);

  HostDma1 = ((void *)0);

}



void test_sendSetStep_send_the_Execute_instruction_to_the_motorDriveBuffer(void){



  sendSetStep();

  UnityAssertEqualNumber((_U_SINT)((motorInfoBuffer[3])), (_U_SINT)((motorDriveBuffer[0])), (((void *)0)), (_U_UINT)(58), UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((motorInfoBuffer[4])), (_U_SINT)((motorDriveBuffer[1])), (((void *)0)), (_U_UINT)(59), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((motorInfoBuffer[5])), (_U_SINT)((motorDriveBuffer[2])), (((void *)0)), (_U_UINT)(60), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((3)), (_U_SINT)((((DMA_Channel_TypeDef*) ((uint32_t)HostDma1_Channel3))->CNDTR)), (((void *)0)), (_U_UINT)(61), UNITY_DISPLAY_STYLE_INT);





}





void test_sendResetStep_send_the_Execute_instruction_to_the_motorDriveBuffer(void){



  sendResetStep();

  UnityAssertEqualNumber((_U_SINT)((motorInfoBuffer[0])), (_U_SINT)((motorDriveBuffer[0])), (((void *)0)), (_U_UINT)(70), UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((motorInfoBuffer[1])), (_U_SINT)((motorDriveBuffer[1])), (((void *)0)), (_U_UINT)(71), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((motorInfoBuffer[2])), (_U_SINT)((motorDriveBuffer[2])), (((void *)0)), (_U_UINT)(72), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((3)), (_U_SINT)((((DMA_Channel_TypeDef*) ((uint32_t)HostDma1_Channel3))->CNDTR)), (((void *)0)), (_U_UINT)(73), UNITY_DISPLAY_STYLE_INT);







}
