#include "unity.h"
#include "Protocol.h"


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
#include "Dma.h"
#include "Motor.h"
#include "Planner.h"
#include "string.h"
#include "Tim.h"
#include "Motor.h"
#include "Planner.h"
#include "Stepper.h"
#include "Spi.h"
#include "System.h"

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
  memset(blockBuffer, 0, sizeof(blockBuffer));
  memset(rxBuffer,0,sizeof(rxBuffer));
  memset(line,0,sizeof(line));
  UsartDataPointer = NULL;
  USART_ReceiveData_StubWithCallback(fake_USART_ReceiveData);
  HostNVIC = malloc((sizeof(NVIC_Type)));
  HostUSART1 = malloc((sizeof(USART_TypeDef)));
  HostRcc = malloc((sizeof(USART_TypeDef)));
  resetRxBuffer();
  planInit();
  USART1->SR = 0;
  charCounter = 0;
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
}

void tearDown(void)
{
 free(HostNVIC);
 free(HostUSART1);
 free(HostRcc);
 HostRcc = NULL;
 HostUSART1 = NULL;
 HostNVIC = NULL;
 rxBufferHead = 0;
 rxBufferTail = 0;
}




void test_isCharacter() {
  TEST_ASSERT_TRUE(isCharacter('a'));
  TEST_ASSERT_TRUE(isCharacter('f'));
  TEST_ASSERT_TRUE(isCharacter('z'));
  TEST_ASSERT_TRUE(isCharacter('A'));
  TEST_ASSERT_TRUE(isCharacter('Y'));
  TEST_ASSERT_TRUE(isCharacter('A'));
  TEST_ASSERT_FALSE(isCharacter('0'));
  TEST_ASSERT_FALSE(isCharacter('@'));
  TEST_ASSERT_FALSE(isCharacter('+'));
  TEST_ASSERT_FALSE(isCharacter(' '));
  TEST_ASSERT_FALSE(isCharacter('\t'));
}

void test_isDigit() {
  TEST_ASSERT_TRUE(isDigit('0'));
  TEST_ASSERT_TRUE(isDigit('6'));
  TEST_ASSERT_TRUE(isDigit('9'));
  TEST_ASSERT_FALSE(isDigit('A'));
  TEST_ASSERT_FALSE(isDigit('G'));
  TEST_ASSERT_FALSE(isDigit('Z'));
  TEST_ASSERT_FALSE(isDigit('a'));
  TEST_ASSERT_FALSE(isDigit('e'));
  TEST_ASSERT_FALSE(isDigit('z'));
  TEST_ASSERT_FALSE(isDigit('\n'));
  TEST_ASSERT_FALSE(isDigit('@'));
  TEST_ASSERT_FALSE(isDigit('+'));
  TEST_ASSERT_FALSE(isDigit(' '));
  TEST_ASSERT_FALSE(isDigit('\t'));
}



void test_readValue_str_is_10000_the_function_shoule_give_value_is_10000(void){
  uint8_t charCounter = 0;
  TEST_ASSERT_EQUAL(10000,readValue("10000",&charCounter));
  TEST_ASSERT_EQUAL(5,charCounter);
}

void test_readValue_str_is_1_the_function_shoule_give_value_is_1(void){
  uint8_t charCounter = 0;
  TEST_ASSERT_EQUAL(1,readValue("1p",&charCounter));
  TEST_ASSERT_EQUAL(1,charCounter);
}

void test_readValue_str_is_1_exclamation_mark_the_function_shoule_give_value_is_1(void){
  uint8_t charCounter = 0;
  TEST_ASSERT_EQUAL(1,readValue("1!",&charCounter));
  TEST_ASSERT_EQUAL(1,charCounter);
}

void test_readValue_str_is_2345__the_function_shoule_give_value_is_2345(void){
  uint8_t charCounter = 0;
  TEST_ASSERT_EQUAL(2345,readValue("2345 ",&charCounter));
  TEST_ASSERT_EQUAL(4,charCounter);
}

void test_readValue_str_is_space_the_function_shoule_give_value_is_2345(void){
  uint8_t charCounter = 0;
  TEST_ASSERT_EQUAL(0,readValue(" ",&charCounter));
  TEST_ASSERT_EQUAL(0,charCounter);
}

void test_getLetterAndValue_the_line_contain_B244_the_value_should_get_244_and_the_letter_should_get_B_character(void){
   char letter;
   int value;
   uint8_t charCounter = 0;
   char* line = "B224";
   getLetterAndValue(&letter,&value,line,&charCounter);
   TEST_ASSERT_EQUAL_UINT8('B',letter);
   TEST_ASSERT_EQUAL(224,value);
   TEST_ASSERT_EQUAL(4,charCounter);   
}

void test_getLetterAndValue_the_line_contain_B224X10000Y3000Z40000(void){
   char letter;
   int value;
   uint8_t charCounter = 0;
   char* line = "B224X10000Y3000Z40000";
   getLetterAndValue(&letter,&value,line,&charCounter);
   TEST_ASSERT_EQUAL_UINT8('B',letter);
   TEST_ASSERT_EQUAL(224,value);
   TEST_ASSERT_EQUAL(4,charCounter);   
   getLetterAndValue(&letter,&value,line,&charCounter);
   TEST_ASSERT_EQUAL_UINT8('X',letter);
   TEST_ASSERT_EQUAL(10000,value);
   TEST_ASSERT_EQUAL(10,charCounter);   
   getLetterAndValue(&letter,&value,line,&charCounter);
   TEST_ASSERT_EQUAL_UINT8('Y',letter);
   TEST_ASSERT_EQUAL(3000,value);
   TEST_ASSERT_EQUAL(15,charCounter); 
   getLetterAndValue(&letter,&value,line,&charCounter);
   TEST_ASSERT_EQUAL_UINT8('Z',letter);
   TEST_ASSERT_EQUAL(40000,value);
   TEST_ASSERT_EQUAL(21,charCounter); 
}

void test_getLetterAndValue_the_line_contain_B224X(void){
   char letter;
   int value;
   uint8_t charCounter = 0;
   char* line = "B224X";
   getLetterAndValue(&letter,&value,line,&charCounter);
   TEST_ASSERT_EQUAL_UINT8('B',letter);
   TEST_ASSERT_EQUAL(224,value);
   TEST_ASSERT_EQUAL(4,charCounter);   
   getLetterAndValue(&letter,&value,line,&charCounter);
   TEST_ASSERT_EQUAL_UINT8('X',letter);
   TEST_ASSERT_EQUAL(0,value);
   TEST_ASSERT_EQUAL(5,charCounter);   
}

void test_lineInterpretToBlock_input_is_B224X10000Y3000Z40000S10000I8000F80000R500A3000D3000N160000_(void){
  lineInterpretToBlock("B224");
  block_t *block = &blockBuffer[bufferTail];
  TEST_ASSERT_EQUAL(224,block->directionBits);
}

void test_lineInterpretToBlock_input_is_B224X10000_(void){
  lineInterpretToBlock("B224X10000");
  block_t *block = &blockBuffer[bufferTail];
  TEST_ASSERT_EQUAL(224,block->directionBits); 
  TEST_ASSERT_EQUAL(10000,block->steps[X_AXIS]); 
}

void test_lineInterpretToBlock_input_is_B224X10000Y3000Z4000S10000I8000F8000R500A3000D6000N160000_(void){
  TEST_ASSERT_EQUAL(bufferTail,bufferHead);
  lineInterpretToBlock("B224X10000Y3000Z4000S10000I8000F8000R500A3000D6000N160000");
  block_t *block = &blockBuffer[bufferTail];
  TEST_ASSERT_EQUAL(224,block->directionBits); 
  TEST_ASSERT_EQUAL(10000,block->steps[X_AXIS]); 
  TEST_ASSERT_EQUAL(3000,block->steps[Y_AXIS]); 
  TEST_ASSERT_EQUAL(4000,block->steps[Z_AXIS]); 
  TEST_ASSERT_EQUAL(10000,block->stepEventCount); 
  TEST_ASSERT_EQUAL(8000,block->initialRate); 
  TEST_ASSERT_EQUAL(8000,block->finalRate); 
  TEST_ASSERT_EQUAL(160000,block->nominalRate); 
  TEST_ASSERT_EQUAL(500,block->rateDelta); 
  TEST_ASSERT_EQUAL(3000,block->accelerateUntil); 
  TEST_ASSERT_EQUAL(6000,block->deccelerateAfter); 
  TEST_ASSERT_EQUAL(1,bufferHead);
  TEST_ASSERT_EQUAL(2,nextBufferHead);
}

void test_dataProcess_send_a_B224X10000Y3000Z4000S10000I8000F8000R500A3000D6000N160000_through_the_usart_when_detect_endline(void){
  char* dataBuffer = "B224X10000Y3000Z4000S10000I8000F8000R500A3000D6000N160000\n";
  setUsartData(dataBuffer);
  int n = strlen(dataBuffer);
  while(n--){
    setRXNEflag;
    USART1_IRQHandler();
    resetSR;
  }
  availableInsertBlock = 1;
  dataProcess();
  TEST_ASSERT_EQUAL_STRING("B224X10000Y3000Z4000S10000I8000F8000R500A3000D6000N160000",line);
  block_t *block = &blockBuffer[bufferTail];
  TEST_ASSERT_EQUAL(0,block->directionBits); 
  TEST_ASSERT_EQUAL(0,block->steps[X_AXIS]); 
  TEST_ASSERT_EQUAL(0,block->steps[Y_AXIS]); 
  TEST_ASSERT_EQUAL(0,block->steps[Z_AXIS]); 
  TEST_ASSERT_EQUAL(0,block->stepEventCount); 
  TEST_ASSERT_EQUAL(0,block->initialRate); 
  TEST_ASSERT_EQUAL(0,block->finalRate); 
  TEST_ASSERT_EQUAL(0,block->nominalRate); 
  TEST_ASSERT_EQUAL(0,block->rateDelta); 
  TEST_ASSERT_EQUAL(0,block->accelerateUntil); 
  TEST_ASSERT_EQUAL(0,block->deccelerateAfter); 
  TEST_ASSERT_EQUAL(0,bufferHead);
  TEST_ASSERT_EQUAL(1,nextBufferHead);
  TEST_ASSERT_EQUAL(1,availableInsertBlock);
}

void test_dataProcess_send_NumberSignB224X10000Y3000Z4000S10000I8000F8000R500A3000D6000N160000_through_the_usart_when_detect_endline(void){
  char* dataBuffer = "#B224X10000Y3000Z4000S10000I8000F8000R500A3000D6000N160000\n";
  setUsartData(dataBuffer);
  int n = strlen(dataBuffer);
  while(n--){
    setRXNEflag;
    USART1_IRQHandler();
    resetSR;
  }
  availableInsertBlock = 1;
  dataProcess();
  TEST_ASSERT_EQUAL_STRING("#B224X10000Y3000Z4000S10000I8000F8000R500A3000D6000N160000",line);
  block_t *block = &blockBuffer[bufferTail];
  TEST_ASSERT_EQUAL(224,block->directionBits); 
  TEST_ASSERT_EQUAL(10000,block->steps[X_AXIS]); 
  TEST_ASSERT_EQUAL(3000,block->steps[Y_AXIS]); 
  TEST_ASSERT_EQUAL(4000,block->steps[Z_AXIS]); 
  TEST_ASSERT_EQUAL(10000,block->stepEventCount); 
  TEST_ASSERT_EQUAL(8000,block->initialRate); 
  TEST_ASSERT_EQUAL(8000,block->finalRate); 
  TEST_ASSERT_EQUAL(160000,block->nominalRate); 
  TEST_ASSERT_EQUAL(500,block->rateDelta); 
  TEST_ASSERT_EQUAL(3000,block->accelerateUntil); 
  TEST_ASSERT_EQUAL(6000,block->deccelerateAfter); 
  TEST_ASSERT_EQUAL(1,bufferHead);
  TEST_ASSERT_EQUAL(2,nextBufferHead);
  TEST_ASSERT_EQUAL(0,availableInsertBlock);
}