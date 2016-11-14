
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
uint8_t motorDriveBuffer[NUMBER_OF_MOTOR];
uint8_t motorInfoBuffer[NUMBER_OF_MOTOR*2];

void dmaInit(void);
void timInit(void);
void spiInit(void);
void gpioInit(void);
void motorInit(void);
void motorDriveBufferInit(void);

MotorInfo* motor2;
MotorInfo* motor1;
MotorInfo* motor0;

uint32_t signal;

int main(void)
{

	gpioInit();
	spiInit();
	dmaInit();
	motorInit();
	motorDriveBufferInit();

	//timInit();

	while(1)
    {

    }
}

void motorInit(void){
	motor0  = motorConfig(Motor_Anti_Clockwise,MOTOR_FULL_STEP);
	motor1  = motorConfig(Motor_Clockwise,MOTOR_FULL_STEP);
	motor2  = motorConfig(Motor_Anti_Clockwise,MOTOR_FULL_STEP);
	motorInfoBuffer[X_MOTOR_RESET] = getMotorInfo(motor0,Motor_Left,Motor_Step_Low);
	motorInfoBuffer[Y_MOTOR_RESET] = getMotorInfo(motor1,Motor_Left,Motor_Step_Low);
	motorInfoBuffer[Z_MOTOR_RESET] = getMotorInfo(motor2,Motor_Left,Motor_Step_Low);
	motorInfoBuffer[X_MOTOR_SET] = getMotorInfo(motor0,Motor_Left,Motor_Step_High);
	motorInfoBuffer[Y_MOTOR_SET] = getMotorInfo(motor1,Motor_Left,Motor_Step_High);
	motorInfoBuffer[Z_MOTOR_SET] = getMotorInfo(motor2,Motor_Left,Motor_Step_High);
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
	TimerConfig(TIM2,1500,TIM_CounterMode_Up,480,0);
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

