#include "unity.h"
#include "Usart.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_usart.h"
#include "malloc.h"
//For Test Driver Development
#include "Host.h"
#include "string.h"
#include "Registers.h"
#include "peripherals.h"
#include "mock_MockFunction.h"


uint8_t* UsartDataPointer = NULL;

void setUsartData(uint8_t dataBuffer[]){
  UsartDataPointer = dataBuffer;  
}



uint16_t fake_USART_ReceiveData(USART_TypeDef* USARTx){
  uint8_t data = *UsartDataPointer;
  UsartDataPointer++;
  return data;
}


void setUp(void)
{
  memset(rxBuffer,0,sizeof(rxBuffer));
  UsartDataPointer = NULL;
  USART_ReceiveData_StubWithCallback(fake_USART_ReceiveData);
  HostNVIC = malloc((sizeof(NVIC_Type)));
  HostUSART1 = malloc((sizeof(USART_TypeDef)));
  HostRcc = malloc((sizeof(USART_TypeDef)));
  resetRxBuffer();
}

void tearDown(void)
{
 free(HostNVIC);
 free(HostUSART1);
 free(HostRcc);
 HostRcc = NULL;
 HostUSART1 = NULL;
 HostNVIC = NULL;
}


void test_USART_ReceiveData_set_A_B_C_D_data_to_USART_DATA(void){
  uint8_t dataBuffer[] = {'A','B','C','D'};
  setUsartData(dataBuffer);
  TEST_ASSERT_EQUAL('A',USART_ReceiveData(USART1));
  TEST_ASSERT_EQUAL('B',USART_ReceiveData(USART1));
  TEST_ASSERT_EQUAL('C',USART_ReceiveData(USART1));
  TEST_ASSERT_EQUAL('D',USART_ReceiveData(USART1));
  
  
}

void test_USART_ReceiveData_set_string_data_to_USART_DATA(void){
  char* dataBuffer = "ABCD";
  setUsartData(dataBuffer);
  TEST_ASSERT_EQUAL('A',USART_ReceiveData(USART1));
  TEST_ASSERT_EQUAL('B',USART_ReceiveData(USART1));
  TEST_ASSERT_EQUAL('C',USART_ReceiveData(USART1));
  TEST_ASSERT_EQUAL('D',USART_ReceiveData(USART1));
}


void resetUsart(USART_TypeDef* USARTx){
  USARTx->SR = 0;
  USARTx->DR = 0;
  USARTx->BRR = 0;
  USARTx->CR1 = 0;
  USARTx->CR2 = 0;
  USARTx->CR3 = 0;
  USARTx->GTPR = 0;
}

void test_reserUsart_(void){
  resetUsart(USART1);
  TEST_ASSERT_EQUAL(USART1->SR,0);
  TEST_ASSERT_EQUAL(USART1->DR,0);
  TEST_ASSERT_EQUAL(USART1->BRR,0);
  TEST_ASSERT_EQUAL(USART1->CR1,0);
  TEST_ASSERT_EQUAL(USART1->CR2,0);
  TEST_ASSERT_EQUAL(USART1->CR3,0);
  TEST_ASSERT_EQUAL(USART1->GTPR,0);
}
void test_usartConfig_(void){
  resetUsart(USART1);
  usartConfig(USART1,9600);
  TEST_ASSERT_EQUAL(USART1->SR,0);
  TEST_ASSERT_EQUAL(USART1->DR,0);
  //TEST_ASSERT_EQUAL(USART1->BRR,0x1d4c);
  TEST_ASSERT_EQUAL(USART1->CR1,0x202c);
  TEST_ASSERT_EQUAL(USART1->CR2,0);
  TEST_ASSERT_EQUAL(USART1->CR3,0);
  TEST_ASSERT_EQUAL(USART1->GTPR,0);
}
/*

     rxBufferHead = rxBufferTail

*/

void test_resetRxBuffer_the_rxBufferHead_is_equal_to_rxBufferTail(void){
  
  TEST_ASSERT_EQUAL(rxBufferHead,rxBufferTail);
}

/*
        rxBuffer                 rxBuffer
          ----                    ------
   Tail->| X |<-Head       Tail->| 'A' |
         | X |                   |  X  |<-Head 
         | X |        --->       |  X  |
         | X |                   |  X  |
           .                       .
           .                       .
           .                       .
*/

void test_insertDataIntoRxBuffer_insert_character_A_into_rxbuffer(void){
  
  insertDataIntoRxBuffer('A');
  TEST_ASSERT_EQUAL('A',rxBuffer[rxBufferTail]);
  TEST_ASSERT_EQUAL(rxBufferTail+1,rxBufferHead);
}

/*
        rxBuffer                 rxBuffer
          ----                    ------
           .                 (0) |  X  |<-Head 
           .                 (1) | 'A' |
           .        --->     (2) |  X  |
  (253)  | X |               (3) |  X  |
  (254)  | X |                      .
  (255)  | X |<-Head                .
         ----                       .
*/

void test_insertDataIntoRxBuffer_if_the_rxBuffer_reached_RX_BUFFER_SIZE_the_rxBuffer_will_be_reset_to_0(void){
  
  rxBufferTail = 1;
  rxBufferHead = 255;  
  insertDataIntoRxBuffer('A');
  TEST_ASSERT_EQUAL('A',rxBuffer[255]);
  TEST_ASSERT_EQUAL(0,rxBufferHead);
}
/*
        rxBuffer                 rxBuffer
          ----                    ------
         | A |<-Head             | 'A' |<-Head
   Tail->| X |             Tail->|  X  | 
         | X |        --->       |  X  |
         | X |                   |  X  |
           .                       .
           .                       .
           .                       .
*/

void test_insertDataIntoRxBuffer_if_the_rxBuffer_is_full_the_rxBufferHead_does_not_update(void){
  resetRxBuffer();
  rxBufferTail = 0;
  rxBufferHead = 255;  
  rxBuffer[rxBufferHead] = 'A';
  insertDataIntoRxBuffer('S');
  TEST_ASSERT_EQUAL(255,rxBufferHead);
  TEST_ASSERT_EQUAL('A',rxBuffer[rxBufferHead]);
}

void test_USART1_IRQHandler_If_the_function_is_called_the_RXNE_flag_is_cleared(void){
  uint8_t dataBuffer[] = {'A','B','C','D'};
  setUsartData(dataBuffer);
  USART1->SR |= (1<<5); // To set RXNE flag
  USART1_IRQHandler();
  TEST_ASSERT_EQUAL(0,USART_GetITStatus(USART1, USART_IT_RXNE));
  
}

void test_USART1_IRQHandler_get_a_data_and_put_into_rxBuffer(void){
  uint8_t dataBuffer[] = {'A','B','C','D'};
  setUsartData(dataBuffer);
  
  setRXNEflag;
  USART1_IRQHandler();
  TEST_ASSERT_EQUAL('A',rxBuffer[rxBufferHead-1]);
  TEST_ASSERT_EQUAL(rxBufferTail+1,rxBufferHead);
  TEST_ASSERT_EQUAL(0,USART_GetITStatus(USART1, USART_IT_RXNE));
  setRXNEflag;
  USART1_IRQHandler();
  TEST_ASSERT_EQUAL('B',rxBuffer[rxBufferHead-1]);
  TEST_ASSERT_EQUAL(rxBufferTail+2,rxBufferHead);
  TEST_ASSERT_EQUAL(0,USART_GetITStatus(USART1, USART_IT_RXNE));
}


void test_readRxbuffer_when_the_rxBuffer_is_empty_it_should_return_RxBuffer_Is_Empty(void){
  TEST_ASSERT_EQUAL_UINT8(RX_BUFFER_IS_EMPTY,readRxBuffer());
}

/*
        rxBuffer                      rxBuffer                rxBuffer
          ------                       ------                  -------
   Tail->| 'A' |                      | 'A' |                  | 'A' |
         | 'B' |                Tail->| 'B' |            Tail->| 'B' |
         |  X  |<-Head    -->         |  X  |<-Head  -->       |  X  |<-Head
         |  X  |                      |  X  |                  |  X  |
            .                            .                        .   
            .                            .                        .
            .                            .                        .
*/
void test_readRxbuffer_when_the_data_is_read_the_rxBufferTail_will_increase_by_1(void){
  uint8_t dataBuffer[] = {'A','B','C','D'};
  setUsartData(dataBuffer);
  setRXNEflag;
  USART1_IRQHandler();
  setRXNEflag;
  USART1_IRQHandler();
  TEST_ASSERT_EQUAL_UINT8('A',readRxBuffer());
  TEST_ASSERT_EQUAL_UINT16(1,rxBufferTail);
  TEST_ASSERT_EQUAL_UINT16(2,rxBufferHead);
  TEST_ASSERT_EQUAL_UINT8('B',readRxBuffer());
  TEST_ASSERT_EQUAL_UINT16(2,rxBufferTail);
  TEST_ASSERT_EQUAL_UINT16(2,rxBufferHead);
}

/*
               rxBuffer                       rxBuffer
                ------                         ------
                  .                 (0) Tail->|  X  |<-Head 
                  .                 (1)       |  X  |
                  .        --->     (2)       |  X  |
  (253)        |  X  |              (3)       |  X  |
  (254)        |  X  |                           .
  (255)  tail->| 'A' |                           .
               ------                            .
*/
void test_readRxbuffer_if_the_rxBufferTail_is_Max_size_when_the_function_is_called_the_rxBufferTail_will_be_reset(void){
  uint8_t dataBuffer[] = {'A','B','C','D'};
  setUsartData(dataBuffer);
  rxBufferTail = 255;
  rxBufferHead = 255;  
  setRXNEflag;
  USART1_IRQHandler();
  TEST_ASSERT_EQUAL_UINT8('A',readRxBuffer());
  TEST_ASSERT_EQUAL_UINT16(0,rxBufferTail);
  TEST_ASSERT_EQUAL_UINT16(0,rxBufferHead);
}



