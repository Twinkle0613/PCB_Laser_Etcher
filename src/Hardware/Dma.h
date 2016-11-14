#ifndef Dma_H
#define Dma_H
#include <stdint.h>
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "Spi.h"
#include "HardwareConfig.h"
#include "Motor.h"

extern uint8_t motorDriveBuffer[];
extern uint8_t motorInfoBuffer[];


#define startDMA(x) (DMA_Cmd(x,ENABLE))
#define stopDMA(x) (DMA_Cmd(x,DISABLE))

#define X_MOTOR_RESET 0
#define Y_MOTOR_RESET 1
#define Z_MOTOR_RESET 2

#define X_MOTOR_SET 3
#define Y_MOTOR_SET 4
#define Z_MOTOR_SET 5

void DmaConfig(DMA_Channel_TypeDef* DMAy_Channelx,
              uint32_t periAddress,
              uint32_t memoryAddress,
              uint32_t dataDirection,
              uint32_t bufferSize,
              uint32_t periDataSize,
              uint32_t memoryDataSize,
              uint32_t dmaMode,
              int priority
              );
void setDataNumber(DMA_Channel_TypeDef* DMAy_Channelx,uint32_t dataNumber);
void sendSetStep(void);
void sendResetStep(void);



#endif // Dma_H
