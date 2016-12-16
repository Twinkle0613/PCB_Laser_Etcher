#include "mock_MockFunction.h"
#include "peripherals.h"
#include "Registers.h"
#include "Host.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "Usart.h"
#include "unity.h"


























uint8_t* UsartDataPointer = ((void *)0);



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

  UsartDataPointer = ((void *)0);

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

 HostRcc = ((void *)0);

 HostUSART1 = ((void *)0);

 HostNVIC = ((void *)0);

}





void test_USART_ReceiveData_set_A_B_C_D_data_to_USART_DATA(void){

  uint8_t dataBuffer[] = {'A','B','C','D'};

  setUsartData(dataBuffer);

  UnityAssertEqualNumber((_U_SINT)(('A')), (_U_SINT)((USART_ReceiveData(((USART_TypeDef*)((uint32_t)HostUSART1))))), (((void *)0)), (_U_UINT)(58), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)(('B')), (_U_SINT)((USART_ReceiveData(((USART_TypeDef*)((uint32_t)HostUSART1))))), (((void *)0)), (_U_UINT)(59), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)(('C')), (_U_SINT)((USART_ReceiveData(((USART_TypeDef*)((uint32_t)HostUSART1))))), (((void *)0)), (_U_UINT)(60), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)(('D')), (_U_SINT)((USART_ReceiveData(((USART_TypeDef*)((uint32_t)HostUSART1))))), (((void *)0)), (_U_UINT)(61), UNITY_DISPLAY_STYLE_INT);





}



void test_USART_ReceiveData_set_string_data_to_USART_DATA(void){

  char* dataBuffer = "ABCD";

  setUsartData(dataBuffer);

  UnityAssertEqualNumber((_U_SINT)(('A')), (_U_SINT)((USART_ReceiveData(((USART_TypeDef*)((uint32_t)HostUSART1))))), (((void *)0)), (_U_UINT)(69), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)(('B')), (_U_SINT)((USART_ReceiveData(((USART_TypeDef*)((uint32_t)HostUSART1))))), (((void *)0)), (_U_UINT)(70), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)(('C')), (_U_SINT)((USART_ReceiveData(((USART_TypeDef*)((uint32_t)HostUSART1))))), (((void *)0)), (_U_UINT)(71), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)(('D')), (_U_SINT)((USART_ReceiveData(((USART_TypeDef*)((uint32_t)HostUSART1))))), (((void *)0)), (_U_UINT)(72), UNITY_DISPLAY_STYLE_INT);

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

  resetUsart(((USART_TypeDef*)((uint32_t)HostUSART1)));

  UnityAssertEqualNumber((_U_SINT)((((USART_TypeDef*)((uint32_t)HostUSART1))->SR)), (_U_SINT)((0)), (((void *)0)), (_U_UINT)(88), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((USART_TypeDef*)((uint32_t)HostUSART1))->DR)), (_U_SINT)((0)), (((void *)0)), (_U_UINT)(89), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((USART_TypeDef*)((uint32_t)HostUSART1))->BRR)), (_U_SINT)((0)), (((void *)0)), (_U_UINT)(90), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((USART_TypeDef*)((uint32_t)HostUSART1))->CR1)), (_U_SINT)((0)), (((void *)0)), (_U_UINT)(91), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((USART_TypeDef*)((uint32_t)HostUSART1))->CR2)), (_U_SINT)((0)), (((void *)0)), (_U_UINT)(92), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((USART_TypeDef*)((uint32_t)HostUSART1))->CR3)), (_U_SINT)((0)), (((void *)0)), (_U_UINT)(93), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((USART_TypeDef*)((uint32_t)HostUSART1))->GTPR)), (_U_SINT)((0)), (((void *)0)), (_U_UINT)(94), UNITY_DISPLAY_STYLE_INT);

}

void test_usartConfig_(void){

  resetUsart(((USART_TypeDef*)((uint32_t)HostUSART1)));

  usartConfig(((USART_TypeDef*)((uint32_t)HostUSART1)),9600);

  UnityAssertEqualNumber((_U_SINT)((((USART_TypeDef*)((uint32_t)HostUSART1))->SR)), (_U_SINT)((0)), (((void *)0)), (_U_UINT)(99), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((USART_TypeDef*)((uint32_t)HostUSART1))->DR)), (_U_SINT)((0)), (((void *)0)), (_U_UINT)(100), UNITY_DISPLAY_STYLE_INT);



  UnityAssertEqualNumber((_U_SINT)((((USART_TypeDef*)((uint32_t)HostUSART1))->CR1)), (_U_SINT)((0x202c)), (((void *)0)), (_U_UINT)(102), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((USART_TypeDef*)((uint32_t)HostUSART1))->CR2)), (_U_SINT)((0)), (((void *)0)), (_U_UINT)(103), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((USART_TypeDef*)((uint32_t)HostUSART1))->CR3)), (_U_SINT)((0)), (((void *)0)), (_U_UINT)(104), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((USART_TypeDef*)((uint32_t)HostUSART1))->GTPR)), (_U_SINT)((0)), (((void *)0)), (_U_UINT)(105), UNITY_DISPLAY_STYLE_INT);

}













void test_resetRxBuffer_the_rxBufferHead_is_equal_to_rxBufferTail(void){



  UnityAssertEqualNumber((_U_SINT)((rxBufferHead)), (_U_SINT)((rxBufferTail)), (((void *)0)), (_U_UINT)(115), UNITY_DISPLAY_STYLE_INT);

}

void test_insertDataIntoRxBuffer_insert_character_A_into_rxbuffer(void){



  insertDataIntoRxBuffer('A');

  UnityAssertEqualNumber((_U_SINT)(('A')), (_U_SINT)((rxBuffer[rxBufferTail])), (((void *)0)), (_U_UINT)(133), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((rxBufferTail+1)), (_U_SINT)((rxBufferHead)), (((void *)0)), (_U_UINT)(134), UNITY_DISPLAY_STYLE_INT);

}

void test_insertDataIntoRxBuffer_if_the_rxBuffer_reached_RX_BUFFER_SIZE_the_rxBuffer_will_be_reset_to_0(void){



  rxBufferTail = 1;

  rxBufferHead = 255;

  insertDataIntoRxBuffer('A');

  UnityAssertEqualNumber((_U_SINT)(('A')), (_U_SINT)((rxBuffer[255])), (((void *)0)), (_U_UINT)(154), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((rxBufferHead)), (((void *)0)), (_U_UINT)(155), UNITY_DISPLAY_STYLE_INT);

}

void test_insertDataIntoRxBuffer_if_the_rxBuffer_is_full_the_rxBufferHead_does_not_update(void){

  resetRxBuffer();

  rxBufferTail = 0;

  rxBufferHead = 255;

  rxBuffer[rxBufferHead] = 'A';

  insertDataIntoRxBuffer('S');

  UnityAssertEqualNumber((_U_SINT)((255)), (_U_SINT)((rxBufferHead)), (((void *)0)), (_U_UINT)(175), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)(('A')), (_U_SINT)((rxBuffer[rxBufferHead])), (((void *)0)), (_U_UINT)(176), UNITY_DISPLAY_STYLE_INT);

}



void test_USART1_IRQHandler_If_the_function_is_called_the_RXNE_flag_is_cleared(void){

  uint8_t dataBuffer[] = {'A','B','C','D'};

  setUsartData(dataBuffer);

  ((USART_TypeDef*)((uint32_t)HostUSART1))->SR |= (1<<5);

  USART1_IRQHandler();

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((USART_GetITStatus(((USART_TypeDef*)((uint32_t)HostUSART1)), ((uint16_t)0x0525)))), (((void *)0)), (_U_UINT)(184), UNITY_DISPLAY_STYLE_INT);



}



void test_USART1_IRQHandler_get_a_data_and_put_into_rxBuffer(void){

  uint8_t dataBuffer[] = {'A','B','C','D'};

  setUsartData(dataBuffer);



  ((USART_TypeDef*)((uint32_t)HostUSART1))->SR |= (1<<5);

  USART1_IRQHandler();

  UnityAssertEqualNumber((_U_SINT)(('A')), (_U_SINT)((rxBuffer[rxBufferHead-1])), (((void *)0)), (_U_UINT)(194), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((rxBufferTail+1)), (_U_SINT)((rxBufferHead)), (((void *)0)), (_U_UINT)(195), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((USART_GetITStatus(((USART_TypeDef*)((uint32_t)HostUSART1)), ((uint16_t)0x0525)))), (((void *)0)), (_U_UINT)(196), UNITY_DISPLAY_STYLE_INT);

  ((USART_TypeDef*)((uint32_t)HostUSART1))->SR |= (1<<5);

  USART1_IRQHandler();

  UnityAssertEqualNumber((_U_SINT)(('B')), (_U_SINT)((rxBuffer[rxBufferHead-1])), (((void *)0)), (_U_UINT)(199), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((rxBufferTail+2)), (_U_SINT)((rxBufferHead)), (((void *)0)), (_U_UINT)(200), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((USART_GetITStatus(((USART_TypeDef*)((uint32_t)HostUSART1)), ((uint16_t)0x0525)))), (((void *)0)), (_U_UINT)(201), UNITY_DISPLAY_STYLE_INT);

}





void test_readRxbuffer_when_the_rxBuffer_is_empty_it_should_return_RxBuffer_Is_Empty(void){

  UnityAssertEqualNumber((_U_SINT)(_UU8 )((0xFF)), (_U_SINT)(_UU8 )((readRxBuffer())), (((void *)0)), (_U_UINT)(206), UNITY_DISPLAY_STYLE_UINT8);

}

void test_readRxbuffer_when_the_data_is_read_the_rxBufferTail_will_increase_by_1(void){

  uint8_t dataBuffer[] = {'A','B','C','D'};

  setUsartData(dataBuffer);

  ((USART_TypeDef*)((uint32_t)HostUSART1))->SR |= (1<<5);

  USART1_IRQHandler();

  ((USART_TypeDef*)((uint32_t)HostUSART1))->SR |= (1<<5);

  USART1_IRQHandler();

  UnityAssertEqualNumber((_U_SINT)(_UU8 )(('A')), (_U_SINT)(_UU8 )((readRxBuffer())), (((void *)0)), (_U_UINT)(227), UNITY_DISPLAY_STYLE_UINT8);

  UnityAssertEqualNumber((_U_SINT)(_UU16)((1)), (_U_SINT)(_UU16)((rxBufferTail)), (((void *)0)), (_U_UINT)(228), UNITY_DISPLAY_STYLE_UINT16);

  UnityAssertEqualNumber((_U_SINT)(_UU16)((2)), (_U_SINT)(_UU16)((rxBufferHead)), (((void *)0)), (_U_UINT)(229), UNITY_DISPLAY_STYLE_UINT16);

  UnityAssertEqualNumber((_U_SINT)(_UU8 )(('B')), (_U_SINT)(_UU8 )((readRxBuffer())), (((void *)0)), (_U_UINT)(230), UNITY_DISPLAY_STYLE_UINT8);

  UnityAssertEqualNumber((_U_SINT)(_UU16)((2)), (_U_SINT)(_UU16)((rxBufferTail)), (((void *)0)), (_U_UINT)(231), UNITY_DISPLAY_STYLE_UINT16);

  UnityAssertEqualNumber((_U_SINT)(_UU16)((2)), (_U_SINT)(_UU16)((rxBufferHead)), (((void *)0)), (_U_UINT)(232), UNITY_DISPLAY_STYLE_UINT16);

}

void test_readRxbuffer_if_the_rxBufferTail_is_Max_size_when_the_function_is_called_the_rxBufferTail_will_be_reset(void){

  uint8_t dataBuffer[] = {'A','B','C','D'};

  setUsartData(dataBuffer);

  rxBufferTail = 255;

  rxBufferHead = 255;

  ((USART_TypeDef*)((uint32_t)HostUSART1))->SR |= (1<<5);

  USART1_IRQHandler();

  UnityAssertEqualNumber((_U_SINT)(_UU8 )(('A')), (_U_SINT)(_UU8 )((readRxBuffer())), (((void *)0)), (_U_UINT)(253), UNITY_DISPLAY_STYLE_UINT8);

  UnityAssertEqualNumber((_U_SINT)(_UU16)((0)), (_U_SINT)(_UU16)((rxBufferTail)), (((void *)0)), (_U_UINT)(254), UNITY_DISPLAY_STYLE_UINT16);

  UnityAssertEqualNumber((_U_SINT)(_UU16)((0)), (_U_SINT)(_UU16)((rxBufferHead)), (((void *)0)), (_U_UINT)(255), UNITY_DISPLAY_STYLE_UINT16);

}
