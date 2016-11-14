#include "unity.h"
#include "Dma.h"


//For Test Driver Development
#include "Host.h"
#include "Registers.h"
#include "peripherals.h"
//Included library by Following DMA.h 
#include <stdint.h>
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"

#include "Spi.h"
#include "HardwareConfig.h"
#include "Motor.h"



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
  HostNVIC = NULL;
  HostDma1_Channel3 = NULL;
  HostDma1 = NULL;
}

void test_sendSetStep_send_the_Execute_instruction_to_the_motorDriveBuffer(void){

  sendSetStep();
  TEST_ASSERT_EQUAL(motorInfoBuffer[X_MOTOR_SET],motorDriveBuffer[X_MOTOR]);
	TEST_ASSERT_EQUAL(motorInfoBuffer[Y_MOTOR_SET],motorDriveBuffer[Y_MOTOR]);
  TEST_ASSERT_EQUAL(motorInfoBuffer[Z_MOTOR_SET],motorDriveBuffer[Z_MOTOR]);
  TEST_ASSERT_EQUAL(3,DMA1_Channel3->CNDTR);
  
  
}


void test_sendResetStep_send_the_Execute_instruction_to_the_motorDriveBuffer(void){

  sendResetStep();
  TEST_ASSERT_EQUAL(motorInfoBuffer[X_MOTOR_RESET],motorDriveBuffer[X_MOTOR]);
	TEST_ASSERT_EQUAL(motorInfoBuffer[Y_MOTOR_RESET],motorDriveBuffer[Y_MOTOR]);
  TEST_ASSERT_EQUAL(motorInfoBuffer[Z_MOTOR_RESET],motorDriveBuffer[Z_MOTOR]);
  TEST_ASSERT_EQUAL(3,DMA1_Channel3->CNDTR);
  
  
  
}
