#include "System.h"
#include "Spi.h"
#include "Stepper.h"
#include "Tim.h"
#include "Planner.h"
#include "Motor.h"
#include "Dma.h"
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
#include "Protocol.h"
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

  memset(blockBuffer, 0, sizeof(blockBuffer));

  memset(rxBuffer,0,sizeof(rxBuffer));

  memset(line,0,sizeof(line));

  UsartDataPointer = ((void *)0);

  USART_ReceiveData_StubWithCallback(fake_USART_ReceiveData);

  HostNVIC = malloc((sizeof(NVIC_Type)));

  HostUSART1 = malloc((sizeof(USART_TypeDef)));

  HostRcc = malloc((sizeof(USART_TypeDef)));

  resetRxBuffer();

  planInit();

  ((USART_TypeDef*)((uint32_t)HostUSART1))->SR = 0;

  charCounter = 0;

  USART_ITConfig(((USART_TypeDef*)((uint32_t)HostUSART1)),((uint16_t)0x0525),ENABLE);

}



void tearDown(void)

{

 free(HostNVIC);

 free(HostUSART1);

 free(HostRcc);

 HostRcc = ((void *)0);

 HostUSART1 = ((void *)0);

 HostNVIC = ((void *)0);

 rxBufferHead = 0;

 rxBufferTail = 0;

}









void test_isCharacter() {

  if ((((('a') >= 'a' && ('a') <= 'z') || (('a') >= 'A' && ('a') <= 'z')))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (_U_UINT)((_U_UINT)(78)));};

  if ((((('f') >= 'a' && ('f') <= 'z') || (('f') >= 'A' && ('f') <= 'z')))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (_U_UINT)((_U_UINT)(79)));};

  if ((((('z') >= 'a' && ('z') <= 'z') || (('z') >= 'A' && ('z') <= 'z')))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (_U_UINT)((_U_UINT)(80)));};

  if ((((('A') >= 'a' && ('A') <= 'z') || (('A') >= 'A' && ('A') <= 'z')))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (_U_UINT)((_U_UINT)(81)));};

  if ((((('Y') >= 'a' && ('Y') <= 'z') || (('Y') >= 'A' && ('Y') <= 'z')))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (_U_UINT)((_U_UINT)(82)));};

  if ((((('A') >= 'a' && ('A') <= 'z') || (('A') >= 'A' && ('A') <= 'z')))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (_U_UINT)((_U_UINT)(83)));};

  if (!(((('0') >= 'a' && ('0') <= 'z') || (('0') >= 'A' && ('0') <= 'z')))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (_U_UINT)((_U_UINT)(84)));};

  if (!(((('@') >= 'a' && ('@') <= 'z') || (('@') >= 'A' && ('@') <= 'z')))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (_U_UINT)((_U_UINT)(85)));};

  if (!(((('+') >= 'a' && ('+') <= 'z') || (('+') >= 'A' && ('+') <= 'z')))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (_U_UINT)((_U_UINT)(86)));};

  if (!((((' ') >= 'a' && (' ') <= 'z') || ((' ') >= 'A' && (' ') <= 'z')))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (_U_UINT)((_U_UINT)(87)));};

  if (!(((('\t') >= 'a' && ('\t') <= 'z') || (('\t') >= 'A' && ('\t') <= 'z')))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (_U_UINT)((_U_UINT)(88)));};

}



void test_isDigit() {

  if (((('0') >= '0' && ('0') <= '9'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (_U_UINT)((_U_UINT)(92)));};

  if (((('6') >= '0' && ('6') <= '9'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (_U_UINT)((_U_UINT)(93)));};

  if (((('9') >= '0' && ('9') <= '9'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (_U_UINT)((_U_UINT)(94)));};

  if (!((('A') >= '0' && ('A') <= '9'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (_U_UINT)((_U_UINT)(95)));};

  if (!((('G') >= '0' && ('G') <= '9'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (_U_UINT)((_U_UINT)(96)));};

  if (!((('Z') >= '0' && ('Z') <= '9'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (_U_UINT)((_U_UINT)(97)));};

  if (!((('a') >= '0' && ('a') <= '9'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (_U_UINT)((_U_UINT)(98)));};

  if (!((('e') >= '0' && ('e') <= '9'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (_U_UINT)((_U_UINT)(99)));};

  if (!((('z') >= '0' && ('z') <= '9'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (_U_UINT)((_U_UINT)(100)));};

  if (!((('\n') >= '0' && ('\n') <= '9'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (_U_UINT)((_U_UINT)(101)));};

  if (!((('@') >= '0' && ('@') <= '9'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (_U_UINT)((_U_UINT)(102)));};

  if (!((('+') >= '0' && ('+') <= '9'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (_U_UINT)((_U_UINT)(103)));};

  if (!(((' ') >= '0' && (' ') <= '9'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (_U_UINT)((_U_UINT)(104)));};

  if (!((('\t') >= '0' && ('\t') <= '9'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (_U_UINT)((_U_UINT)(105)));};

}







void test_readValue_str_is_10000_the_function_shoule_give_value_is_10000(void){

  uint8_t charCounter = 0;

  UnityAssertEqualNumber((_U_SINT)((10000)), (_U_SINT)((readValue("10000",&charCounter))), (((void *)0)), (_U_UINT)(112), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((5)), (_U_SINT)((charCounter)), (((void *)0)), (_U_UINT)(113), UNITY_DISPLAY_STYLE_INT);

}



void test_readValue_str_is_1_the_function_shoule_give_value_is_1(void){

  uint8_t charCounter = 0;

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((readValue("1p",&charCounter))), (((void *)0)), (_U_UINT)(118), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((charCounter)), (((void *)0)), (_U_UINT)(119), UNITY_DISPLAY_STYLE_INT);

}



void test_readValue_str_is_1_exclamation_mark_the_function_shoule_give_value_is_1(void){

  uint8_t charCounter = 0;

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((readValue("1!",&charCounter))), (((void *)0)), (_U_UINT)(124), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((charCounter)), (((void *)0)), (_U_UINT)(125), UNITY_DISPLAY_STYLE_INT);

}



void test_readValue_str_is_2345__the_function_shoule_give_value_is_2345(void){

  uint8_t charCounter = 0;

  UnityAssertEqualNumber((_U_SINT)((2345)), (_U_SINT)((readValue("2345 ",&charCounter))), (((void *)0)), (_U_UINT)(130), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((charCounter)), (((void *)0)), (_U_UINT)(131), UNITY_DISPLAY_STYLE_INT);

}



void test_readValue_str_is_space_the_function_shoule_give_value_is_2345(void){

  uint8_t charCounter = 0;

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((readValue(" ",&charCounter))), (((void *)0)), (_U_UINT)(136), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((charCounter)), (((void *)0)), (_U_UINT)(137), UNITY_DISPLAY_STYLE_INT);

}



void test_getLetterAndValue_the_line_contain_B244_the_value_should_get_244_and_the_letter_should_get_B_character(void){

   char letter;

   int value;

   uint8_t charCounter = 0;

   char* line = "B224";

   getLetterAndValue(&letter,&value,line,&charCounter);

   UnityAssertEqualNumber((_U_SINT)(_UU8 )(('B')), (_U_SINT)(_UU8 )((letter)), (((void *)0)), (_U_UINT)(146), UNITY_DISPLAY_STYLE_UINT8);

   UnityAssertEqualNumber((_U_SINT)((224)), (_U_SINT)((value)), (((void *)0)), (_U_UINT)(147), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((charCounter)), (((void *)0)), (_U_UINT)(148), UNITY_DISPLAY_STYLE_INT);

}



void test_getLetterAndValue_the_line_contain_B224X10000Y3000Z40000(void){

   char letter;

   int value;

   uint8_t charCounter = 0;

   char* line = "B224X10000Y3000Z40000";

   getLetterAndValue(&letter,&value,line,&charCounter);

   UnityAssertEqualNumber((_U_SINT)(_UU8 )(('B')), (_U_SINT)(_UU8 )((letter)), (((void *)0)), (_U_UINT)(157), UNITY_DISPLAY_STYLE_UINT8);

   UnityAssertEqualNumber((_U_SINT)((224)), (_U_SINT)((value)), (((void *)0)), (_U_UINT)(158), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((charCounter)), (((void *)0)), (_U_UINT)(159), UNITY_DISPLAY_STYLE_INT);

   getLetterAndValue(&letter,&value,line,&charCounter);

   UnityAssertEqualNumber((_U_SINT)(_UU8 )(('X')), (_U_SINT)(_UU8 )((letter)), (((void *)0)), (_U_UINT)(161), UNITY_DISPLAY_STYLE_UINT8);

   UnityAssertEqualNumber((_U_SINT)((10000)), (_U_SINT)((value)), (((void *)0)), (_U_UINT)(162), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((10)), (_U_SINT)((charCounter)), (((void *)0)), (_U_UINT)(163), UNITY_DISPLAY_STYLE_INT);

   getLetterAndValue(&letter,&value,line,&charCounter);

   UnityAssertEqualNumber((_U_SINT)(_UU8 )(('Y')), (_U_SINT)(_UU8 )((letter)), (((void *)0)), (_U_UINT)(165), UNITY_DISPLAY_STYLE_UINT8);

   UnityAssertEqualNumber((_U_SINT)((3000)), (_U_SINT)((value)), (((void *)0)), (_U_UINT)(166), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((15)), (_U_SINT)((charCounter)), (((void *)0)), (_U_UINT)(167), UNITY_DISPLAY_STYLE_INT);

   getLetterAndValue(&letter,&value,line,&charCounter);

   UnityAssertEqualNumber((_U_SINT)(_UU8 )(('Z')), (_U_SINT)(_UU8 )((letter)), (((void *)0)), (_U_UINT)(169), UNITY_DISPLAY_STYLE_UINT8);

   UnityAssertEqualNumber((_U_SINT)((40000)), (_U_SINT)((value)), (((void *)0)), (_U_UINT)(170), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((21)), (_U_SINT)((charCounter)), (((void *)0)), (_U_UINT)(171), UNITY_DISPLAY_STYLE_INT);

}



void test_getLetterAndValue_the_line_contain_B224X(void){

   char letter;

   int value;

   uint8_t charCounter = 0;

   char* line = "B224X";

   getLetterAndValue(&letter,&value,line,&charCounter);

   UnityAssertEqualNumber((_U_SINT)(_UU8 )(('B')), (_U_SINT)(_UU8 )((letter)), (((void *)0)), (_U_UINT)(180), UNITY_DISPLAY_STYLE_UINT8);

   UnityAssertEqualNumber((_U_SINT)((224)), (_U_SINT)((value)), (((void *)0)), (_U_UINT)(181), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((charCounter)), (((void *)0)), (_U_UINT)(182), UNITY_DISPLAY_STYLE_INT);

   getLetterAndValue(&letter,&value,line,&charCounter);

   UnityAssertEqualNumber((_U_SINT)(_UU8 )(('X')), (_U_SINT)(_UU8 )((letter)), (((void *)0)), (_U_UINT)(184), UNITY_DISPLAY_STYLE_UINT8);

   UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((value)), (((void *)0)), (_U_UINT)(185), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((5)), (_U_SINT)((charCounter)), (((void *)0)), (_U_UINT)(186), UNITY_DISPLAY_STYLE_INT);

}



void test_lineInterpretToBlock_input_is_B224X10000Y3000Z40000S10000I8000F80000R500A3000D3000N160000_(void){

  lineInterpretToBlock("B224");

  block_t *block = &blockBuffer[bufferTail];

  UnityAssertEqualNumber((_U_SINT)((224)), (_U_SINT)((block->directionBits)), (((void *)0)), (_U_UINT)(192), UNITY_DISPLAY_STYLE_INT);

}



void test_lineInterpretToBlock_input_is_B224X10000_(void){

  lineInterpretToBlock("B224X10000");

  block_t *block = &blockBuffer[bufferTail];

  UnityAssertEqualNumber((_U_SINT)((224)), (_U_SINT)((block->directionBits)), (((void *)0)), (_U_UINT)(198), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((10000)), (_U_SINT)((block->steps[0])), (((void *)0)), (_U_UINT)(199), UNITY_DISPLAY_STYLE_INT);

}



void test_lineInterpretToBlock_input_is_B224X10000Y3000Z4000S10000I8000F8000R500A3000D6000N160000_(void){

  UnityAssertEqualNumber((_U_SINT)((bufferTail)), (_U_SINT)((bufferHead)), (((void *)0)), (_U_UINT)(203), UNITY_DISPLAY_STYLE_INT);

  lineInterpretToBlock("B224X10000Y3000Z4000S10000I8000F8000R500A3000D6000N160000");

  block_t *block = &blockBuffer[bufferTail];

  UnityAssertEqualNumber((_U_SINT)((224)), (_U_SINT)((block->directionBits)), (((void *)0)), (_U_UINT)(206), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((10000)), (_U_SINT)((block->steps[0])), (((void *)0)), (_U_UINT)(207), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((3000)), (_U_SINT)((block->steps[1])), (((void *)0)), (_U_UINT)(208), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4000)), (_U_SINT)((block->steps[2])), (((void *)0)), (_U_UINT)(209), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((10000)), (_U_SINT)((block->stepEventCount)), (((void *)0)), (_U_UINT)(210), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((8000)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(211), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((8000)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(212), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((block->nominalRate)), (((void *)0)), (_U_UINT)(213), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((500)), (_U_SINT)((block->rateDelta)), (((void *)0)), (_U_UINT)(214), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((3000)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(215), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((6000)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(216), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((bufferHead)), (((void *)0)), (_U_UINT)(217), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((2)), (_U_SINT)((nextBufferHead)), (((void *)0)), (_U_UINT)(218), UNITY_DISPLAY_STYLE_INT);

}



void test_dataProcess_send_a_B224X10000Y3000Z4000S10000I8000F8000R500A3000D6000N160000_through_the_usart_when_detect_endline(void){

  char* dataBuffer = "B224X10000Y3000Z4000S10000I8000F8000R500A3000D6000N160000\n";

  setUsartData(dataBuffer);

  int n = strlen(dataBuffer);

  while(n--){

    ((USART_TypeDef*)((uint32_t)HostUSART1))->SR |= (1<<5);

    USART1_IRQHandler();

    ((USART_TypeDef*)((uint32_t)HostUSART1))->SR = 0;

  }

  availableInsertBlock = 1;

  dataProcess();

  UnityAssertEqualString((const char*)(("B224X10000Y3000Z4000S10000I8000F8000R500A3000D6000N160000")), (const char*)((line)), (((void *)0)), (_U_UINT)(232));

  block_t *block = &blockBuffer[bufferTail];

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->directionBits)), (((void *)0)), (_U_UINT)(234), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->steps[0])), (((void *)0)), (_U_UINT)(235), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->steps[1])), (((void *)0)), (_U_UINT)(236), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->steps[2])), (((void *)0)), (_U_UINT)(237), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->stepEventCount)), (((void *)0)), (_U_UINT)(238), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(239), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(240), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->nominalRate)), (((void *)0)), (_U_UINT)(241), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->rateDelta)), (((void *)0)), (_U_UINT)(242), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(243), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(244), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((bufferHead)), (((void *)0)), (_U_UINT)(245), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((nextBufferHead)), (((void *)0)), (_U_UINT)(246), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((availableInsertBlock)), (((void *)0)), (_U_UINT)(247), UNITY_DISPLAY_STYLE_INT);

}



void test_dataProcess_send_NumberSignB224X10000Y3000Z4000S10000I8000F8000R500A3000D6000N160000_through_the_usart_when_detect_endline(void){

  char* dataBuffer = "#B224X10000Y3000Z4000S10000I8000F8000R500A3000D6000N160000\n";

  setUsartData(dataBuffer);

  int n = strlen(dataBuffer);

  while(n--){

    ((USART_TypeDef*)((uint32_t)HostUSART1))->SR |= (1<<5);

    USART1_IRQHandler();

    ((USART_TypeDef*)((uint32_t)HostUSART1))->SR = 0;

  }

  availableInsertBlock = 1;

  dataProcess();

  UnityAssertEqualString((const char*)(("#B224X10000Y3000Z4000S10000I8000F8000R500A3000D6000N160000")), (const char*)((line)), (((void *)0)), (_U_UINT)(261));

  block_t *block = &blockBuffer[bufferTail];

  UnityAssertEqualNumber((_U_SINT)((224)), (_U_SINT)((block->directionBits)), (((void *)0)), (_U_UINT)(263), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((10000)), (_U_SINT)((block->steps[0])), (((void *)0)), (_U_UINT)(264), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((3000)), (_U_SINT)((block->steps[1])), (((void *)0)), (_U_UINT)(265), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4000)), (_U_SINT)((block->steps[2])), (((void *)0)), (_U_UINT)(266), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((10000)), (_U_SINT)((block->stepEventCount)), (((void *)0)), (_U_UINT)(267), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((8000)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(268), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((8000)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(269), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((block->nominalRate)), (((void *)0)), (_U_UINT)(270), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((500)), (_U_SINT)((block->rateDelta)), (((void *)0)), (_U_UINT)(271), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((3000)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(272), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((6000)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(273), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((bufferHead)), (((void *)0)), (_U_UINT)(274), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((2)), (_U_SINT)((nextBufferHead)), (((void *)0)), (_U_UINT)(275), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((availableInsertBlock)), (((void *)0)), (_U_UINT)(276), UNITY_DISPLAY_STYLE_INT);

}
