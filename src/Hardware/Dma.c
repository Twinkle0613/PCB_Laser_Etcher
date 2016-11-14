#include "Dma.h"



uint8_t motorDriveBuffer[NUMBER_OF_MOTOR];
uint8_t motorInfoBuffer[NUMBER_OF_MOTOR*2];

void DMA1_Channel3_IRQHandler(void){
	DMA_ClearITPendingBit(DMA1_IT_TC3);
	triggerOutputData();
  sendSetStep();
}


void sendSetStep(void){
  motorDriveBuffer[X_MOTOR] = motorInfoBuffer[X_MOTOR_SET];
	motorDriveBuffer[Y_MOTOR] = motorInfoBuffer[Y_MOTOR_SET];
	motorDriveBuffer[Z_MOTOR] = motorInfoBuffer[Z_MOTOR_SET];
  NVIC_DisableIRQ(DMA1_Channel3_IRQn);
  setDataNumber(DMA1_Channel3,NUMBER_OF_MOTOR);
}

void sendResetStep(void){
   motorDriveBuffer[X_MOTOR] = motorInfoBuffer[X_MOTOR_RESET];
	 motorDriveBuffer[Y_MOTOR] = motorInfoBuffer[Y_MOTOR_RESET];
	 motorDriveBuffer[Z_MOTOR] = motorInfoBuffer[Z_MOTOR_RESET];
	 NVIC_EnableIRQ(DMA1_Channel3_IRQn);
   setDataNumber(DMA1_Channel3,NUMBER_OF_MOTOR);
}


void setDataNumber(DMA_Channel_TypeDef* DMAy_Channelx,uint32_t dataNumber){
	stopDMA(DMAy_Channelx);
	DMAy_Channelx->CNDTR = dataNumber;
	startDMA(DMAy_Channelx);
}

void DmaConfig(DMA_Channel_TypeDef* DMAy_Channelx,
              uint32_t periAddress,
              uint32_t memoryAddress,
              uint32_t dataDirection,
              uint32_t bufferSize,
              uint32_t periDataSize,
              uint32_t memoryDataSize,
              uint32_t dmaMode,
              int priority
              ){
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

  DMA_DeInit(DMAy_Channelx);
  DMA_InitTypeDef dma;
  dma.DMA_PeripheralBaseAddr = periAddress;
  dma.DMA_MemoryBaseAddr = memoryAddress;
  dma.DMA_DIR = dataDirection;
  dma.DMA_BufferSize = bufferSize;
  dma.DMA_PeripheralDataSize = periDataSize;
  dma.DMA_MemoryDataSize = memoryDataSize;

  dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dma.DMA_Mode = dmaMode;
  dma.DMA_Priority = DMA_Priority_High;
  dma.DMA_M2M = DMA_M2M_Disable;

  DMA_Init(DMAy_Channelx,&dma);
  DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,ENABLE);
  int DMAy_Channelx_IRQn;


  if(DMAy_Channelx == DMA1_Channel1){DMAy_Channelx_IRQn = DMA1_Channel2_IRQn;}
  else if(DMAy_Channelx == DMA1_Channel2){DMAy_Channelx_IRQn = DMA1_Channel2_IRQn;}
  else if(DMAy_Channelx == DMA1_Channel3){DMAy_Channelx_IRQn = DMA1_Channel3_IRQn;}
  else if(DMAy_Channelx == DMA1_Channel4){DMAy_Channelx_IRQn = DMA1_Channel4_IRQn;}
  else if(DMAy_Channelx == DMA1_Channel5){DMAy_Channelx_IRQn = DMA1_Channel5_IRQn;}
  else if(DMAy_Channelx == DMA1_Channel6){DMAy_Channelx_IRQn = DMA1_Channel6_IRQn;}
  else if(DMAy_Channelx == DMA1_Channel7){DMAy_Channelx_IRQn = DMA1_Channel7_IRQn;}
  else if(DMAy_Channelx == DMA2_Channel1){DMAy_Channelx_IRQn = DMA2_Channel1_IRQn;}
  else if(DMAy_Channelx == DMA2_Channel2){DMAy_Channelx_IRQn = DMA2_Channel2_IRQn;}
  else if(DMAy_Channelx == DMA2_Channel3){DMAy_Channelx_IRQn = DMA2_Channel3_IRQn;}

  NVIC_SetPriority(DMAy_Channelx_IRQn,priority);
  NVIC_EnableIRQ(DMAy_Channelx_IRQn);
}



