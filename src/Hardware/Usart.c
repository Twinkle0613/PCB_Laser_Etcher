#include "Usart.h"
uint8_t rxBuffer[RX_BUFFER_SIZE];
uint16_t rxBufferHead;
uint16_t rxBufferTail;



void USART1_IRQHandler(void){
  uint8_t data;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
	  data = USART_ReceiveData(USART1);
    if(data == '$'){
     memset(rxBuffer,0,sizeof(rxBuffer));
     printString("RESET Success!");
     
    }else if(data == '!'){
     printString("OK");
    }else{
     insertDataIntoRxBuffer(data);
    }
  }
  USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}

void usartConfig(USART_TypeDef* USARTx, uint32_t baudRate){
  
   if(USARTx == USART1){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 |RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,DISABLE);
  }else if(USARTx == USART2){
    RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  }
  USART_InitTypeDef usart;
  usart.USART_BaudRate            = baudRate;	  
	usart.USART_WordLength          = USART_WordLength_8b; 
	usart.USART_StopBits            = USART_StopBits_1;	 
	usart.USART_Parity              = USART_Parity_No ;	 
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1,&usart);
  USART_Cmd(USARTx, ENABLE);
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
  int USARTx_IRQn;
  if(USARTx == USART1){
   USARTx_IRQn = USART1_IRQn;
  }else if(USARTx == USART2){
   USARTx_IRQn = USART2_IRQn;
 }
  NVIC_SetPriority(USARTx_IRQn,0);
  NVIC_EnableIRQ(USARTx_IRQn);
}


void insertDataIntoRxBuffer(uint8_t data){
  uint16_t nextHead = rxBufferHead;
  nextHead += 1;
  if(nextHead == RX_BUFFER_SIZE){
    nextHead = 0;
  }
  
  if(nextHead != rxBufferTail){
   rxBuffer[rxBufferHead] = data;
   rxBufferHead = nextHead;
  }
}

void resetRxBuffer(void){
  rxBufferHead = 0;
  rxBufferTail = rxBufferHead;
}

uint8_t readRxBuffer(void){
  uint16_t tail = rxBufferTail;  
  if(rxBufferHead == tail){
    return RX_BUFFER_IS_EMPTY;
  }else{
     uint8_t data = rxBuffer[tail];
     tail++;
     if(tail == RX_BUFFER_SIZE){
       tail = 0;
     }
     rxBufferTail = tail;
    return data;
  }
}

void printString(char *string){
  while(*string){
    while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));
    USART_SendData(USART1,*string++);
  }
}
