#ifndef Usart_H
#define Usart_H
#include "stdint.h"
#include "String.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_usart.h"




#define USART1_CK GPIO_Pin_8
#define USART1_TX GPIO_Pin_9
#define USART1_RX GPIO_Pin_10
#define USART1_CTS GPIO_Pin_11
#define USART1_RTS GPIO_Pin_12

#define RX_BUFFER_IS_EMPTY 0xFF
#define RX_BUFFER_SIZE 256

#define setRXNEflag USART1->SR |= (1<<5)
#define resetSR USART1->SR = 0
extern uint8_t rxBuffer[RX_BUFFER_SIZE];
extern uint16_t rxBufferHead;
extern uint16_t rxBufferTail;

void insertDataIntoRxBuffer(uint8_t data);
void printString(char *string);
void usartConfig(USART_TypeDef* USARTx, uint32_t baudRate);
uint8_t readRxBuffer(void);
void resetRxBuffer(void);
#endif // Usart_H
