#include <math.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"
#include "Gpio.h"
#include "Tim.h"
#include "Dma.h"
#include "Spi.h"
#include "Motor.h"
#include "Planner.h"
#include "Stepper.h"
#include "System.h"
#include "Usart.h"
#include "Protocol.h"
#include "String.h"
#include "Setting.h"
//uint8_t motorDriveBuffer[NUMBER_OF_MOTOR];
//uint8_t motorInfoBuffer[NUMBER_OF_MOTOR*2];
void usartInit(void);
void dmaInit(void);
void timInit(void);
void spiInit(void);
void gpioInit(void);
void motorInit(void);
void motorDriveBufferInit(void);
void motorInfoBufferInit(void);

MotorInfo* motor2;
MotorInfo* motor1;
MotorInfo* motor0;
uint32_t signal;


int main(void)
{
   resetRxBuffer();
   gpioInit();
   spiInit();
   dmaInit();
   motorInit();
   planInit();
   motorInfoBufferInit();
   motorDriveBufferInit();
   timInit();
   usartInit();
   availableInsertBlock = 2;

   while(1)
   {
    dataProcess();
   }
}


void usartInit(void){
	GpioConfig(GPIOA,GPIO_Pin_9,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);   //SPI1_NSS
	GpioConfig(GPIOA,GPIO_Pin_10,Input_Mode,GPIO_Mode_IPU);   //SPI1_SCK
	usartConfig(USART1,9600);
}
void motorInfoBufferInit(void){
	motorInfoBuffer[X_MOTOR_RESET] = getMotorInfo(motor0,MOTOR_LEFT,MOTOR_STEP_LOW);
	motorInfoBuffer[Y_MOTOR_RESET] = getMotorInfo(motor1,MOTOR_LEFT,MOTOR_STEP_LOW);
	motorInfoBuffer[Z_MOTOR_RESET] = getMotorInfo(motor2,MOTOR_LEFT,MOTOR_STEP_LOW);
	motorInfoBuffer[X_MOTOR_SET] = getMotorInfo(motor0,MOTOR_LEFT,MOTOR_STEP_HIGH);
	motorInfoBuffer[Y_MOTOR_SET] = getMotorInfo(motor1,MOTOR_LEFT,MOTOR_STEP_HIGH);
	motorInfoBuffer[Z_MOTOR_SET] = getMotorInfo(motor2,MOTOR_LEFT,MOTOR_STEP_HIGH);
}

void motorDriveBufferInit(void){
	motorDriveBuffer[X_MOTOR] = motorInfoBuffer[3];
	motorDriveBuffer[Y_MOTOR] = motorInfoBuffer[4];
	motorDriveBuffer[Z_MOTOR] = motorInfoBuffer[5];
}
void dmaInit(void){
	SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
	DmaConfig(DMA1_Channel3,(uint32_t)&(SPI1->DR),(uint32_t)&motorDriveBuffer,DMA_DIR_PeripheralDST,3,
	DMA_PeripheralDataSize_Byte,DMA_MemoryDataSize_Byte,DMA_Mode_Normal,0);
}

void timInit(void){
	TimerConfig(TIM2,1500-1,TIM_CounterMode_Up,(48)-1,0);
	TIM_Cmd(TIM2,ENABLE);
}

void spiInit(void){
	GpioConfig(GPIOA,NSS_PIN,GPIO_Speed_50MHz,GPIO_Mode_Out_PP);   //SPI1_NSS
	GpioConfig(GPIOA,SCLK_PIN,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);   //SPI1_SCK
    GpioConfig(GPIOA,MISO_PIN,Input_Mode,GPIO_Mode_IPU);           //SPI1_MISO
	GpioConfig(GPIOA,MOSI_PIN,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);   //SPI1_MOSI
	SpiConfig(SPI1,SPI_Mode_Master,SPI_DataSize_8b,SPI_BaudRatePrescaler_2,SPI_FirstBit_LSB,idle0_1Edge);
}

void gpioInit(void){
	GpioConfig(GPIOC, GPIO_Pin_13,GPIO_Speed_50MHz,GPIO_Mode_Out_OD);
}

