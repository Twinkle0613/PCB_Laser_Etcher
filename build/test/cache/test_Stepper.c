#include "System.h"
#include "Spi.h"
#include "Planner.h"
#include "Motor.h"
#include "Dma.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "peripherals.h"
#include "Registers.h"
#include "Host.h"
#include "Stepper.h"
#include "unity.h"






void initMotorDriveBuffer(void){

  int i;

  for(i = 0; i <= 2 ; i++){

    motorDriveBuffer[i] = 0;

  }

}



void initMotorInfoBuffer(void){

  int i;

  for(i = 0; i <= 5 ; i++){

    motorInfoBuffer[i] = i+1;

  }

}

const char *byte_to_binary(int x)

{

    static char b[9];

    b[0] = '\0';



    int z;

    for (z = 128; z > 0; z >>= 1)

    {

        strcat(b, ((x & z) == z) ? "1" : "0");

    }

    return b;

}



void setUp(void)

{

  availableInsertBlock = 0;

  outputBits = 0;

  motorInit();

  sysInit();

  stepperInit();

  planInit();

  wakeUpState = 0;

  HostTim2= malloc((sizeof(TIM_TypeDef)));

  HostGpioA= malloc((sizeof(GPIO_TypeDef)));

  HostNVIC = malloc((sizeof(NVIC_Type)));

  HostDma1_Channel3 = malloc((sizeof(DMA_Channel_TypeDef)));

  HostDma1 = malloc((sizeof(DMA_TypeDef)));

}



void tearDown(void)

{

  free(HostTim2);

  free(HostGpioA);

  free(HostNVIC);

  free(HostDma1_Channel3);

  free(HostDma1);

  HostGpioA = ((void *)0);

  HostTim2 = ((void *)0);

  HostNVIC = ((void *)0);

  HostDma1 = ((void *)0);

  HostDma1_Channel3 = ((void *)0);

}





void test_TIM2_IRQHandler_the_function_is_called_by_CPU(void){



  TIM2_IRQHandler();

  UnityAssertEqualNumber((_U_SINT)((motorInfoBuffer[0])), (_U_SINT)((motorDriveBuffer[0])), (((void *)0)), (_U_UINT)(83), UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((motorInfoBuffer[1])), (_U_SINT)((motorDriveBuffer[1])), (((void *)0)), (_U_UINT)(84), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((motorInfoBuffer[2])), (_U_SINT)((motorDriveBuffer[2])), (((void *)0)), (_U_UINT)(85), UNITY_DISPLAY_STYLE_INT);



  UnityAssertEqualNumber((_U_SINT)((0x0010)), (_U_SINT)((((GPIO_TypeDef *) ((uint32_t)HostGpioA))->BRR)), (((void *)0)), (_U_UINT)(87), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0x0010)), (_U_SINT)((((GPIO_TypeDef *) ((uint32_t)HostGpioA))->BSRR)), (((void *)0)), (_U_UINT)(88), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0xfffe)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->SR)), (((void *)0)), (_U_UINT)(89), UNITY_DISPLAY_STYLE_INT);



}





void test_initStepper_reset_and_initilize_all_of_the_data(void){

  stepperInit();

  if ((((currentBlock)) == ((void *)0))) {} else {UnityFail( (((" Expected NULL"))), (_U_UINT)((_U_UINT)((_U_UINT)(96))));};

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((stExecutor.error[0])), (((void *)0)), (_U_UINT)(97), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((stExecutor.error[1])), (((void *)0)), (_U_UINT)(98), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((stExecutor.error[2])), (((void *)0)), (_U_UINT)(99), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((stExecutor.eventCount)), (((void *)0)), (_U_UINT)(100), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((stExecutor.steps[0])), (((void *)0)), (_U_UINT)(101), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((stExecutor.steps[1])), (((void *)0)), (_U_UINT)(102), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((stExecutor.steps[2])), (((void *)0)), (_U_UINT)(103), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(104), UNITY_DISPLAY_STYLE_INT);

}



void test_sleep_the_timer_interrupt_is_disable(void){

  sleep();

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((wakeUpState)), (((void *)0)), (_U_UINT)(109), UNITY_DISPLAY_STYLE_INT);

}



void test_wakeup_the_timer_interrupt_is_enabled(void){

  wakeUp();

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((wakeUpState)), (((void *)0)), (_U_UINT)(114), UNITY_DISPLAY_STYLE_INT);

}

void test_TIM2_IRQHandler_IRQ_to_request_block_when_buffer_block_didnt_contain_any_block(void){

  TIM2_IRQHandler();

  if ((((currentBlock)) == ((void *)0))) {} else {UnityFail( (((" Expected NULL"))), (_U_UINT)((_U_UINT)((_U_UINT)(130))));};

}

void test_TIM2_IRQHandler_IRQ_to_request_block_when_buffer_block_contain_blocks(void){

  bufferHead = nextBlockIndex(bufferHead);

  bufferHead = nextBlockIndex(bufferHead);

  bufferHead = nextBlockIndex(bufferHead);

  blockConfig(&blockBuffer[bufferTail],0x00,10000,8000,2000,160000,160000,80000);

  TIM2_IRQHandler();

  UnityAssertEqualNumber((_U_SINT)(_UP)((&(blockBuffer[bufferTail]))), (_U_SINT)(_UP)((currentBlock)), (((void *)0)), (_U_UINT)(149), UNITY_DISPLAY_STYLE_HEX32);

}

void test_TIM2_IRQHandler_no_more_block_for_IRQ_to_request_and_IRQ_will_go_to_sleep(void){

  wakeUpState = 1;

  TIM2_IRQHandler();

  if ((((currentBlock)) == ((void *)0))) {} else {UnityFail( (((" Expected NULL"))), (_U_UINT)((_U_UINT)((_U_UINT)(165))));};

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((wakeUpState)), (((void *)0)), (_U_UINT)(166), UNITY_DISPLAY_STYLE_INT);

}



void test_transferInfoToStExecutor_when_IRQ_successfully_get_a_block_and_transfer_info_to_st_executor_from_block(void){

  block_t newBlock;

  newBlock.stepEventCount = 400;

  newBlock.directionBits = 0x70;

  newBlock.steps[0] = 400;

  newBlock.steps[1] = 300;

  newBlock.steps[2] = 200;

  newBlock.initialRate = 80000;

  transferInfoToStExecutor(&newBlock);

  UnityAssertEqualNumber((_U_SINT)((400)), (_U_SINT)((stExecutor.eventCount)), (((void *)0)), (_U_UINT)(178), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((-(stExecutor.eventCount>>1))), (_U_SINT)((stExecutor.error[0])), (((void *)0)), (_U_UINT)(179), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((-(stExecutor.eventCount>>1))), (_U_SINT)((stExecutor.error[1])), (((void *)0)), (_U_UINT)(180), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((-(stExecutor.eventCount>>1))), (_U_SINT)((stExecutor.error[2])), (((void *)0)), (_U_UINT)(181), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((400)), (_U_SINT)((stExecutor.steps[0])), (((void *)0)), (_U_UINT)(182), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((300)), (_U_SINT)((stExecutor.steps[1])), (((void *)0)), (_U_UINT)(183), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((stExecutor.steps[2])), (((void *)0)), (_U_UINT)(184), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0x70)), (_U_SINT)((stExecutor.xyzDireation)), (((void *)0)), (_U_UINT)(185), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(186), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(187), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((54000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(188), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((stExecutor.cyclePerStepCounter)), (((void *)0)), (_U_UINT)(189), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((750)), (_U_SINT)((stExecutor.minSafeRate)), (((void *)0)), (_U_UINT)(190), UNITY_DISPLAY_STYLE_INT);



}

void test_TIM2_IRQHandler_get_block_from_block_buffer_and_transfer_block_info_into_st_executor(void){

    bufferHead = nextBlockIndex(bufferHead);

    blockBuffer[bufferTail].stepEventCount = 400;

    blockBuffer[bufferTail].directionBits = 0x70;

    blockBuffer[bufferTail].steps[0] = 400;

    blockBuffer[bufferTail].steps[1] = 300;

    blockBuffer[bufferTail].steps[2] = 200;



    TIM2_IRQHandler();

    UnityAssertEqualNumber((_U_SINT)((400)), (_U_SINT)((stExecutor.eventCount)), (((void *)0)), (_U_UINT)(215), UNITY_DISPLAY_STYLE_INT);







    UnityAssertEqualNumber((_U_SINT)((400)), (_U_SINT)((stExecutor.steps[0])), (((void *)0)), (_U_UINT)(219), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((300)), (_U_SINT)((stExecutor.steps[1])), (((void *)0)), (_U_UINT)(220), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((stExecutor.steps[2])), (((void *)0)), (_U_UINT)(221), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((0x70)), (_U_SINT)((stExecutor.xyzDireation)), (((void *)0)), (_U_UINT)(222), UNITY_DISPLAY_STYLE_INT);

}

void test_bresenhamAlgorithm_when_dx_is_10_and_dy_is_10_(void){



  uint32_t dx,dy,error;

  dx = 10;

  dy = 10;

  error = -(dx>>1);

  int y = 0;

  int x = 10;

  int i;

  int v = 0;

  int stepXstatus = 0;

  for(i = 1; i <= 10; i++){

    v = (dy*i)/dx;

    y += bresenhamAlgorithm(&error,dx,dy,&stepXstatus);

    UnityAssertEqualNumber((_U_SINT)((v)), (_U_SINT)((y)), (((void *)0)), (_U_UINT)(256), UNITY_DISPLAY_STYLE_INT);

  }



}

void test_bresenhamAlgorithm_when_dx_is_20_and_dy_is_15_(void){



  uint32_t dx,dy;

  int32_t error;

  dx = 100;

  dy = 20;

  error = -(dx>>1);



  int y = 0;

  int i;

  int v = 0;

  int stepXstatus = 0;

  for(i = 1; i <= dx; i++){

    v = (dy*i)/dx;

    y += bresenhamAlgorithm(&error,dx,dy,&stepXstatus);





  }

}

void test_executeStepDisplament_XYZ_direations_are_positive_(void){

  int8_t output;

  block_t newBlock;

  int32_t errorX,errorY,errorZ;

  newBlock.stepEventCount = 20;

  newBlock.directionBits = 0x00;

  newBlock.steps[0] = 20;

  newBlock.steps[1] = 10;

  newBlock.steps[2] = 15;

  transferInfoToStExecutor(&newBlock);

  int positionX = 0,positionY = 0,positionZ = 0;

  int stepXstatus = 0,stepYstatus = 0,stepZstatus = 0;

  errorX = -(stExecutor.eventCount>>1);

  errorY = -(stExecutor.eventCount>>1);

  errorZ = -(stExecutor.eventCount>>1);

  double z;

  int i;



  for(i = 1; i <= stExecutor.eventCount; i++){

    output = newBlock.directionBits;

    positionX += bresenhamAlgorithm(&errorX,stExecutor.eventCount,stExecutor.steps[0],&stepXstatus);

    positionY += bresenhamAlgorithm(&errorY,stExecutor.eventCount,stExecutor.steps[1],&stepYstatus);

    positionZ += bresenhamAlgorithm(&errorZ,stExecutor.eventCount,stExecutor.steps[2],&stepZstatus);

    output |= executeStepDisplacement(0);

    output |= executeStepDisplacement(1);

    output |= executeStepDisplacement(2);

    UnityAssertEqualNumber((_U_SINT)((positionX)), (_U_SINT)((sys.position[0])), (((void *)0)), (_U_UINT)(338), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepXstatus)), (_U_SINT)((getOutputFlagStatus(output,0))), (((void *)0)), (_U_UINT)(339), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((getDireationFlag(output,0))), (((void *)0)), (_U_UINT)(340), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((_U_SINT)((positionY)), (_U_SINT)((sys.position[1])), (((void *)0)), (_U_UINT)(342), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepYstatus)), (_U_SINT)((getOutputFlagStatus(output,1))), (((void *)0)), (_U_UINT)(343), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((getDireationFlag(output,1))), (((void *)0)), (_U_UINT)(344), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((_U_SINT)((positionZ)), (_U_SINT)((sys.position[2])), (((void *)0)), (_U_UINT)(346), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepZstatus)), (_U_SINT)((getOutputFlagStatus(output,2))), (((void *)0)), (_U_UINT)(347), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((getDireationFlag(output,2))), (((void *)0)), (_U_UINT)(348), UNITY_DISPLAY_STYLE_INT);

  }



}

void test_executeStepDisplament_XYZ_direations_are_negative_(void){

  uint8_t output;

  block_t newBlock;

  int32_t errorX,errorY,errorZ;

  newBlock.stepEventCount = 20;

  newBlock.directionBits = 0xe0;

  newBlock.steps[0] = 20;

  newBlock.steps[1] = 10;

  newBlock.steps[2] = 15;

  transferInfoToStExecutor(&newBlock);

  int32_t positionX = 0,positionY = 0,positionZ = 0;

  int stepXstatus = 0,stepYstatus = 0,stepZstatus = 0;

  errorX = -(stExecutor.eventCount>>1);

  errorY = -(stExecutor.eventCount>>1);

  errorZ = -(stExecutor.eventCount>>1);



  int i;



  for(i = 1; i <= stExecutor.eventCount; i++){

    output = newBlock.directionBits;



    positionX -= bresenhamAlgorithm(&errorX,stExecutor.eventCount,stExecutor.steps[0],&stepXstatus);

    positionY -= bresenhamAlgorithm(&errorY,stExecutor.eventCount,stExecutor.steps[1],&stepYstatus);

    positionZ -= bresenhamAlgorithm(&errorZ,stExecutor.eventCount,stExecutor.steps[2],&stepZstatus);

    output |= executeStepDisplacement(0);

    output |= executeStepDisplacement(1);

    output |= executeStepDisplacement(2);



    UnityAssertEqualNumber((_U_SINT)((positionX)), (_U_SINT)((sys.position[0])), (((void *)0)), (_U_UINT)(396), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepXstatus)), (_U_SINT)((getOutputFlagStatus(output,0))), (((void *)0)), (_U_UINT)(397), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((getDireationFlag(output,0))), (((void *)0)), (_U_UINT)(398), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((_U_SINT)((positionY)), (_U_SINT)((sys.position[1])), (((void *)0)), (_U_UINT)(400), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepYstatus)), (_U_SINT)((getOutputFlagStatus(output,1))), (((void *)0)), (_U_UINT)(401), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((getDireationFlag(output,1))), (((void *)0)), (_U_UINT)(402), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((_U_SINT)((positionZ)), (_U_SINT)((sys.position[2])), (((void *)0)), (_U_UINT)(404), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepZstatus)), (_U_SINT)((getOutputFlagStatus(output,2))), (((void *)0)), (_U_UINT)(405), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((getDireationFlag(output,2))), (((void *)0)), (_U_UINT)(406), UNITY_DISPLAY_STYLE_INT);

  }



}



void test_executeStepDisplacementProcess_the_functon_is_called_and_using_for_loop(void){



  block_t newBlock;

  int32_t errorX,errorY,errorZ;

  newBlock.stepEventCount = 100;

  newBlock.directionBits = 0x00;

  newBlock.steps[0] = 100;

  newBlock.steps[1] = 75;

  newBlock.steps[2] = 10;

  transferInfoToStExecutor(&newBlock);

  int32_t positionX = 0,positionY = 0,positionZ = 0;

  int stepXstatus = 0,stepYstatus = 0,stepZstatus = 0;

  errorX = -(stExecutor.eventCount>>1);

  errorY = -(stExecutor.eventCount>>1);

  errorZ = -(stExecutor.eventCount>>1);

  int i;





  for(i = 1; i <= stExecutor.eventCount; i++){

    executeStepDisplacementProcess();



    positionX += bresenhamAlgorithm(&errorX,stExecutor.eventCount,stExecutor.steps[0],&stepXstatus);

    positionY += bresenhamAlgorithm(&errorY,stExecutor.eventCount,stExecutor.steps[1],&stepYstatus);

    positionZ += bresenhamAlgorithm(&errorZ,stExecutor.eventCount,stExecutor.steps[2],&stepZstatus);



    UnityAssertEqualNumber((_U_SINT)((positionX)), (_U_SINT)((sys.position[0])), (((void *)0)), (_U_UINT)(436), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepXstatus)), (_U_SINT)((getOutputFlagStatus(outputBits,0))), (((void *)0)), (_U_UINT)(437), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((getDireationFlag(outputBits,0))), (((void *)0)), (_U_UINT)(438), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((_U_SINT)((positionY)), (_U_SINT)((sys.position[1])), (((void *)0)), (_U_UINT)(440), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepYstatus)), (_U_SINT)((getOutputFlagStatus(outputBits,1))), (((void *)0)), (_U_UINT)(441), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((getDireationFlag(outputBits,1))), (((void *)0)), (_U_UINT)(442), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((_U_SINT)((positionZ)), (_U_SINT)((sys.position[2])), (((void *)0)), (_U_UINT)(444), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepZstatus)), (_U_SINT)((getOutputFlagStatus(outputBits,2))), (((void *)0)), (_U_UINT)(445), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((getDireationFlag(outputBits,2))), (((void *)0)), (_U_UINT)(446), UNITY_DISPLAY_STYLE_INT);

  }



}









void test_getStepStatus_when_x_y_z_rest(void){

  outputBits = 0b11100000;

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0xBF))), (_U_SINT)((getStepStatus(0))), (((void *)0)), (_U_UINT)(456), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0xBF))), (_U_SINT)((getStepStatus(1))), (((void *)0)), (_U_UINT)(457), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0xBF))), (_U_SINT)((getStepStatus(2))), (((void *)0)), (_U_UINT)(458), UNITY_DISPLAY_STYLE_INT);

}



void test_getStepStatus_when_x_y_z_move_(void){

  outputBits = 0b11111100;

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x40))), (_U_SINT)((getStepStatus(0))), (((void *)0)), (_U_UINT)(463), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x40))), (_U_SINT)((getStepStatus(1))), (((void *)0)), (_U_UINT)(464), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x40))), (_U_SINT)((getStepStatus(2))), (((void *)0)), (_U_UINT)(465), UNITY_DISPLAY_STYLE_INT);

}



void test_getMoveDireaction_when_x_y_z_are_moving_right_direation(void){

  outputBits = 0b00000000;

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x80))), (_U_SINT)((getMoveDireaction(0))), (((void *)0)), (_U_UINT)(470), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x80))), (_U_SINT)((getMoveDireaction(1))), (((void *)0)), (_U_UINT)(471), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x80))), (_U_SINT)((getMoveDireaction(2))), (((void *)0)), (_U_UINT)(472), UNITY_DISPLAY_STYLE_INT);

}



void test_getMoveDireaction_when_x_y_z_are_moving_left_direation(void){

  outputBits = 0b11111100;

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x00))), (_U_SINT)((getMoveDireaction(0))), (((void *)0)), (_U_UINT)(477), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x00))), (_U_SINT)((getMoveDireaction(1))), (((void *)0)), (_U_UINT)(478), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x00))), (_U_SINT)((getMoveDireaction(2))), (((void *)0)), (_U_UINT)(479), UNITY_DISPLAY_STYLE_INT);

}



void test_planningDataSending_XYZ_direations_are_negative_(void){

  outputBits = 0b11111100;

  planningDataSending();

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor0,((uint8_t)0x00),((uint8_t)0x40)))), (_U_SINT)((motorInfoBuffer[3])), (((void *)0)), (_U_UINT)(485), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor1,((uint8_t)0x00),((uint8_t)0x40)))), (_U_SINT)((motorInfoBuffer[4])), (((void *)0)), (_U_UINT)(486), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x00),((uint8_t)0x40)))), (_U_SINT)((motorInfoBuffer[5])), (((void *)0)), (_U_UINT)(487), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[0])), (((void *)0)), (_U_UINT)(488), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[1])), (((void *)0)), (_U_UINT)(489), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[2])), (((void *)0)), (_U_UINT)(490), UNITY_DISPLAY_STYLE_INT);

}



void test_planningDataSending_XYZ_direations_are_positive_(void){

  outputBits = 0b00011100;

  planningDataSending();

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor0,((uint8_t)0x80),((uint8_t)0x40)))), (_U_SINT)((motorInfoBuffer[3])), (((void *)0)), (_U_UINT)(496), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor1,((uint8_t)0x80),((uint8_t)0x40)))), (_U_SINT)((motorInfoBuffer[4])), (((void *)0)), (_U_UINT)(497), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x80),((uint8_t)0x40)))), (_U_SINT)((motorInfoBuffer[5])), (((void *)0)), (_U_UINT)(498), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x80),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[0])), (((void *)0)), (_U_UINT)(499), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x80),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[1])), (((void *)0)), (_U_UINT)(500), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x80),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[2])), (((void *)0)), (_U_UINT)(501), UNITY_DISPLAY_STYLE_INT);

}



void test_planningDataSending_XYZ_motor_take_a_rest_(void){

  outputBits = 0b11100000;

  planningDataSending();

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor0,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[3])), (((void *)0)), (_U_UINT)(507), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor1,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[4])), (((void *)0)), (_U_UINT)(508), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[5])), (((void *)0)), (_U_UINT)(509), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[0])), (((void *)0)), (_U_UINT)(510), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[1])), (((void *)0)), (_U_UINT)(511), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[2])), (((void *)0)), (_U_UINT)(512), UNITY_DISPLAY_STYLE_INT);

}









void test_executeStepDisplacementProcess_planningDataSending_the_functon_is_called_and_using_for_loop(void){



  block_t newBlock;

  int32_t errorX,errorY,errorZ;

  newBlock.stepEventCount = 100;

  newBlock.directionBits = 0x00;

  newBlock.steps[0] = 100;

  newBlock.steps[1] = 75;

  newBlock.steps[2] = 10;

  transferInfoToStExecutor(&newBlock);

  int32_t positionX = 0,positionY = 0,positionZ = 0;

  int stepXstatus = 0,stepYstatus = 0,stepZstatus = 0;

  errorX = -(stExecutor.eventCount>>1);

  errorY = -(stExecutor.eventCount>>1);

  errorZ = -(stExecutor.eventCount>>1);

  int i;





  for(i = 1; i <= stExecutor.eventCount; i++){

    executeStepDisplacementProcess();

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor0,getMoveDireaction(0),getStepStatus(0)))), (_U_SINT)((motorInfoBuffer[3])), (((void *)0)), (_U_UINT)(538), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor1,getMoveDireaction(1),getStepStatus(1)))), (_U_SINT)((motorInfoBuffer[4])), (((void *)0)), (_U_UINT)(539), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor0,getMoveDireaction(2),getStepStatus(2)))), (_U_SINT)((motorInfoBuffer[5])), (((void *)0)), (_U_UINT)(540), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor0,getMoveDireaction(0),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[0])), (((void *)0)), (_U_UINT)(542), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor1,getMoveDireaction(1),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[1])), (((void *)0)), (_U_UINT)(543), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,getMoveDireaction(2),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[2])), (((void *)0)), (_U_UINT)(544), UNITY_DISPLAY_STYLE_INT);

  }

}







void test_executeStepDisplacementProcess_planningDataSending_sendResetStep_the_functon_is_called_and_using_for_loop(void){



  block_t newBlock;

  int32_t errorX,errorY,errorZ;

  newBlock.stepEventCount = 100;

  newBlock.directionBits = 0x00;

  newBlock.steps[0] = 100;

  newBlock.steps[1] = 75;

  newBlock.steps[2] = 10;

  transferInfoToStExecutor(&newBlock);

  int32_t positionX = 0,positionY = 0,positionZ = 0;

  int stepXstatus = 0,stepYstatus = 0,stepZstatus = 0;

  errorX = -(stExecutor.eventCount>>1);

  errorY = -(stExecutor.eventCount>>1);

  errorZ = -(stExecutor.eventCount>>1);

  int i;





  for(i = 1; i <= stExecutor.eventCount; i++){

    executeStepDisplacementProcess();

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor0,getMoveDireaction(0),((uint8_t)0xBF)))), (_U_SINT)((motorDriveBuffer[0])), (((void *)0)), (_U_UINT)(570), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor1,getMoveDireaction(1),((uint8_t)0xBF)))), (_U_SINT)((motorDriveBuffer[1])), (((void *)0)), (_U_UINT)(571), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,getMoveDireaction(2),((uint8_t)0xBF)))), (_U_SINT)((motorDriveBuffer[2])), (((void *)0)), (_U_UINT)(572), UNITY_DISPLAY_STYLE_INT);

  }

}



void test_TIM2_IRQHandler_and_executeStepDisplacementProcess_simulate_when_TIM_interrupt_occur(void){

    bufferHead = nextBlockIndex(bufferHead);

    blockBuffer[bufferTail].stepEventCount = 100;

    blockBuffer[bufferTail].directionBits = 0x00;

    blockBuffer[bufferTail].steps[0] = 100;

    blockBuffer[bufferTail].steps[1] = 75;

    blockBuffer[bufferTail].steps[2] = 10;



  int i;

  for(i = 1; i <= blockBuffer[bufferTail].stepEventCount; i++){

    TIM2_IRQHandler();

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor0,getMoveDireaction(0),((uint8_t)0xBF)))), (_U_SINT)((motorDriveBuffer[0])), (((void *)0)), (_U_UINT)(587), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor1,getMoveDireaction(1),((uint8_t)0xBF)))), (_U_SINT)((motorDriveBuffer[1])), (((void *)0)), (_U_UINT)(588), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,getMoveDireaction(2),((uint8_t)0xBF)))), (_U_SINT)((motorDriveBuffer[2])), (((void *)0)), (_U_UINT)(589), UNITY_DISPLAY_STYLE_INT);

  }

  UnityAssertEqualNumber((_U_SINT)((stExecutor.eventCount)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(591), UNITY_DISPLAY_STYLE_INT);



}













void test_motorRateControlProcess_when_motor_moving_step_is_finish_the_current_block_will_be_discarded(void){

    bufferHead = nextBlockIndex(bufferHead);

    blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,80000,160000,80000);

    stExecutorInitProcess();

    stExecutor.stepEventsCompleted = currentBlock->stepEventCount;

    motorRateControlProcess();

    if ((((currentBlock)) == ((void *)0))) {} else {UnityFail( (((" Expected NULL"))), (_U_UINT)((_U_UINT)((_U_UINT)(606))));};

    UnityAssertEqualNumber((_U_SINT)((bufferHead)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(607), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((availableInsertBlock)), (((void *)0)), (_U_UINT)(608), UNITY_DISPLAY_STYLE_INT);

}









void test_setTickPerStepEventToTimer_if_cycle_15000000_the_actual_cycle_is_58593_and_prescale_is_256(void){

 UnityAssertEqualNumber((_U_SINT)((58593)), (_U_SINT)((setCyclePerStepEventToTimer(15000000))), (((void *)0)), (_U_UINT)(615), UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((58592)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(616), UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((255)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(617), UNITY_DISPLAY_STYLE_INT);

}



void test_setTickPerStepEventToTimer_if_cycle_72000000_the_actual_cycle_is_35156_and_prescale_is_2047(void){

 UnityAssertEqualNumber((_U_SINT)((35156)), (_U_SINT)((setCyclePerStepEventToTimer(72000000))), (((void *)0)), (_U_UINT)(621), UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((35155)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(622), UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((2047)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(623), UNITY_DISPLAY_STYLE_INT);

}



void test_setTickPerStepEventToTimer_if_cycle_50000_the_actual_cycle_is_50000_and_prescale_is_0(void){

 UnityAssertEqualNumber((_U_SINT)((50000)), (_U_SINT)((setCyclePerStepEventToTimer(50000))), (((void *)0)), (_U_UINT)(627), UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((49999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(628), UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(629), UNITY_DISPLAY_STYLE_INT);

}



void test_setTickPerStepEventToTimer_if_cycle_1_the_actual_cycle_is_1_and_prescale_is_0(void){

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((setCyclePerStepEventToTimer(1))), (((void *)0)), (_U_UINT)(633), UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(634), UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(635), UNITY_DISPLAY_STYLE_INT);

}



void test_setStepEventPerMin_the_steps_per_min_is_80000_the_cycle_will_be_54000(void){

  setStepEventPerMin(80000);

  UnityAssertEqualNumber((_U_SINT)((54000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(640), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(641), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(642), UNITY_DISPLAY_STYLE_INT);

}



void test_setStepEventPerMin_the_steps_per_min_is_100_less_than_800_and_steps_per_min_will_become_800(void){

  setStepEventPerMin(100);

  UnityAssertEqualNumber((_U_SINT)((56250)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(647), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((56249)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(648), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((255)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(649), UNITY_DISPLAY_STYLE_INT);

}





void test_iterateCycleCounter_when_currentRate_is_80000_after_the_function_in_several_loop_cyclePerStepConter_will_remain_6000(void){

   uint32_t currentRate = 80000;

   setStepEventPerMin(currentRate);

   int limit = (72000000/(((10*60*60*80)/60)/500))/stExecutor.cyclePerStepEvent;

   stExecutor.cyclePerStepCounter = 0;

   UnityAssertEqualNumber((_U_SINT)((54000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(658), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((750000)), (_U_SINT)(((72000000/(((10*60*60*80)/60)/500)))), (((void *)0)), (_U_UINT)(659), UNITY_DISPLAY_STYLE_INT);

   int i = 0;

   for(i = 0; i < limit ; i++){

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((iterateCycleCounter())), (((void *)0)), (_U_UINT)(662), UNITY_DISPLAY_STYLE_INT);

   }

   UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((iterateCycleCounter())), (((void *)0)), (_U_UINT)(664), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((6000)), (_U_SINT)((stExecutor.cyclePerStepCounter)), (((void *)0)), (_U_UINT)(665), UNITY_DISPLAY_STYLE_INT);

}



void test_iterateCycleCounter_currentRate_is_100_after_the_function_in_several_loop_cyclePerStepConter_will_remain_9366(void){

   uint32_t currentRate = 100;

   setStepEventPerMin(currentRate);

   int limit = (72000000/(((10*60*60*80)/60)/500))/stExecutor.cyclePerStepEvent;

   stExecutor.cyclePerStepCounter = 0;

   UnityAssertEqualNumber((_U_SINT)((56250)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(673), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((56249)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(674), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((255)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(675), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((750000)), (_U_SINT)(((72000000/(((10*60*60*80)/60)/500)))), (((void *)0)), (_U_UINT)(676), UNITY_DISPLAY_STYLE_INT);

   int i = 0;



   for(i = 0; i < limit ; i++){

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((iterateCycleCounter())), (((void *)0)), (_U_UINT)(680), UNITY_DISPLAY_STYLE_INT);

   }

   UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((iterateCycleCounter())), (((void *)0)), (_U_UINT)(682), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((37500)), (_U_SINT)((stExecutor.cyclePerStepCounter)), (((void *)0)), (_U_UINT)(683), UNITY_DISPLAY_STYLE_INT);



   for(i = 0; i < limit-1 ; i++){

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((iterateCycleCounter())), (((void *)0)), (_U_UINT)(686), UNITY_DISPLAY_STYLE_INT);

   }

   UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((iterateCycleCounter())), (((void *)0)), (_U_UINT)(688), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((18750)), (_U_SINT)((stExecutor.cyclePerStepCounter)), (((void *)0)), (_U_UINT)(689), UNITY_DISPLAY_STYLE_INT);

}







void test_acceleratreRate_when_currentRate_is_80000_after_the_function_in_several_loop_the_currentRate_will_be_increased_to_80500(void){

   bufferHead = nextBlockIndex(bufferHead);

   blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,80000,160000,80000);

   stExecutorInitProcess();

   int limit = (72000000/(((10*60*60*80)/60)/500))/stExecutor.cyclePerStepEvent;

   int i;

   for(i = 0; i <= limit ; i++){

    accelerateRate();

   }

    UnityAssertEqualNumber((_U_SINT)((6000)), (_U_SINT)((stExecutor.cyclePerStepCounter)), (((void *)0)), (_U_UINT)(703), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((80500)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(704), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((53640)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(705), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((53639)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(706), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(707), UNITY_DISPLAY_STYLE_INT);

}



void test_acceleratreRate_when_currentRate_is_160000_the_currentRate_does_not_increase_any_more(void){

   bufferHead = nextBlockIndex(bufferHead);

   blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,160000,160000,80000);

   stExecutorInitProcess();

   int limit = (72000000/(((10*60*60*80)/60)/500))/stExecutor.cyclePerStepEvent;

   int i;

   for(i = 0; i <= limit ; i++){

    accelerateRate();

   }

    UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(719), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((27000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(720), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((26999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(721), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(722), UNITY_DISPLAY_STYLE_INT);

}



void test_accelerateRate_the_currentRate_will_increase_from_80000_to_160000_the_total_step_is(void){

   bufferHead = nextBlockIndex(bufferHead);

   blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,80000,160000,80000);

   stExecutorInitProcess();

   currentBlock->accelerateUntil = ceil(estimateAccelerationStep(currentBlock->initialRate,currentBlock->nominalRate, 10*60*60*80));



   int steps = 0;

   while(steps < currentBlock->accelerateUntil){

     accelerateRate();

     steps++;



   }

   UnityAssertEqualNumber((_U_SINT)((currentBlock->accelerateUntil)), (_U_SINT)((steps)), (((void *)0)), (_U_UINT)(737), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(738), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((27000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(739), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((26999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(740), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(741), UNITY_DISPLAY_STYLE_INT);

}







void test_nominalRate_if_currentRate_is_not_equal_nominalRate_the_currentRate_will_be_equal_to_nominalRate(void){

   bufferHead = nextBlockIndex(bufferHead);

   blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,80000,160000,80000);

   stExecutorInitProcess();

   nominalRate();

   UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(751), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((27000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(752), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((26999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(753), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(754), UNITY_DISPLAY_STYLE_INT);

}



void test_nominalRate_if_currentRate_is_equal_nominalRate_the_currentRate_will_be_remain(void){

   bufferHead = nextBlockIndex(bufferHead);

   blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,160000,160000,80000);

   stExecutorInitProcess();

   nominalRate();

   UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(762), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((27000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(763), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((26999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(764), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(765), UNITY_DISPLAY_STYLE_INT);

}









void test_decelerationAbjustment_if_currentRate_is_more_than_minSafeRate_the_currentRate_will_be_mimus_by_increment_resolution(void){

  bufferHead = nextBlockIndex(bufferHead);

  blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,80000,160000,70000);

  stExecutorInitProcess();

  int limit = (72000000/(((10*60*60*80)/60)/500))/stExecutor.cyclePerStepEvent;

   int i;

  for(i = 0; i <= limit ; i++){

     deccelerationAbjustment();

  }

  UnityAssertEqualNumber((_U_SINT)((79500)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(780), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((54300)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(781), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((54299)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(782), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(783), UNITY_DISPLAY_STYLE_INT);

}



void test_decelerationAbjustment_if_currentRate_is_less_than_minSafeRate_the_currentRate_will_be_divide_by_2(void){

  bufferHead = nextBlockIndex(bufferHead);

  blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,700,160000,80000);

  stExecutorInitProcess();

   int limit = (72000000/(((10*60*60*80)/60)/500))/stExecutor.cyclePerStepEvent;

   int i;

   for(i = 0; i <= limit ; i++){

     deccelerationAbjustment();

   }



  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(796), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(797), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((54000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(798), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(799), UNITY_DISPLAY_STYLE_INT);

}



void test_decelerationAbjustment_if_currentRate_is_less_than_finalRate_the_currentRate_will_become_finalRate(void){

  bufferHead = nextBlockIndex(bufferHead);

  blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,80000,160000,80000);

  stExecutorInitProcess();

  deccelerationAbjustment();

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(807), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((54000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(808), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(809), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(810), UNITY_DISPLAY_STYLE_INT);

}









void test_initializeDeccelerate_when_the_currentRate_is_equal_to_nominalRate_the_cyclePerStepCounter_will_be_set_to_0(void){

  bufferHead = nextBlockIndex(bufferHead);

  blockConfig(&blockBuffer[bufferTail],0x00,10000,8000,2000,160000,160000,80000);

  stExecutorInitProcess();

  currentBlock->accelerateUntil = ceil(estimateAccelerationStep(currentBlock->initialRate,currentBlock->nominalRate, 10*60*60*80));

  currentBlock->deccelerateAfter = estimateDeccelerationStep();

  stExecutor.stepEventsCompleted = currentBlock->deccelerateAfter;

  initializeDecceleration();

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((stExecutor.cyclePerStepCounter)), (((void *)0)), (_U_UINT)(824), UNITY_DISPLAY_STYLE_INT);



}



void test_initializeDeccelerate_when_the_currentRate_is_not_equal_to_nominalRate_the_cyclePerStepCounter_will_be_set_to_cyclesPerAccelerationTick_divide_by_2(void){

  bufferHead = nextBlockIndex(bufferHead);

  blockConfig(&blockBuffer[bufferTail],0x00,10000,8000,2000,80000,160000,80000);

  stExecutorInitProcess();

  currentBlock->accelerateUntil = ceil(estimateAccelerationStep(currentBlock->initialRate,currentBlock->nominalRate, 10*60*60*80));

  currentBlock->deccelerateAfter = estimateDeccelerationStep();

  stExecutor.stepEventsCompleted = currentBlock->deccelerateAfter;

  stExecutor.cyclePerStepCounter = 54000;

  initializeDecceleration();

  UnityAssertEqualNumber((_U_SINT)(((72000000/(((10*60*60*80)/60)/500))-54000)), (_U_SINT)((stExecutor.cyclePerStepCounter)), (((void *)0)), (_U_UINT)(837), UNITY_DISPLAY_STYLE_INT);

}









void test_deccelerateRate_when_the_currentRate_is_equal_to_nominalRate_the_cyclePerStepCounter_will_be_set_to_0(void){

  bufferHead = nextBlockIndex(bufferHead);

  blockConfig(&blockBuffer[bufferTail],0x00,10000,8000,2000,160000,160000,80000);

  stExecutorInitProcess();

  currentBlock->accelerateUntil = ceil(estimateAccelerationStep(currentBlock->initialRate,currentBlock->nominalRate, 10*60*60*80));

  currentBlock->deccelerateAfter = estimateDeccelerationStep();

  stExecutor.stepEventsCompleted = currentBlock->deccelerateAfter;UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((stExecutor.cyclePerStepCounter)), (((void *)0)), (_U_UINT)(849), UNITY_DISPLAY_STYLE_INT);

  deccelerateRate();

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((stExecutor.cyclePerStepCounter)), (((void *)0)), (_U_UINT)(851), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(852), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((27000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(853), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((26999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(854), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(855), UNITY_DISPLAY_STYLE_INT);

}



void test_deccelerateRate_when_the_currentRate_is_not_equal_to_nominalRate_the_cyclePerStepCounter_will_be_set_to_cyclesPerAccelerationTick_divide_by_2(void){

  bufferHead = nextBlockIndex(bufferHead);

  blockConfig(&blockBuffer[bufferTail],0x00,10000,8000,2000,80000,160000,80000);

  stExecutorInitProcess();

  currentBlock->accelerateUntil = ceil(estimateAccelerationStep(currentBlock->initialRate,currentBlock->nominalRate, 10*60*60*80));

  currentBlock->deccelerateAfter = estimateDeccelerationStep();

  stExecutor.stepEventsCompleted = currentBlock->deccelerateAfter;

  stExecutor.cyclePerStepCounter = 54000;

  deccelerateRate();

  UnityAssertEqualNumber((_U_SINT)(((72000000/(((10*60*60*80)/60)/500))-54000)), (_U_SINT)((stExecutor.cyclePerStepCounter)), (((void *)0)), (_U_UINT)(867), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(868), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((54000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(869), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(870), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(871), UNITY_DISPLAY_STYLE_INT);

}





void test_accelerateAndDeccelerateEvent_(void){



  bufferHead = nextBlockIndex(bufferHead);

  blockConfig(&blockBuffer[bufferTail],0x00,10000,8000,2000,80000,160000,80000);

  stExecutorInitProcess();







  UnityAssertEqualNumber((_U_SINT)((3334)), (_U_SINT)((currentBlock->accelerateUntil)), (((void *)0)), (_U_UINT)(883), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((6666)), (_U_SINT)((currentBlock->deccelerateAfter)), (((void *)0)), (_U_UINT)(884), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(885), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((54000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(886), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(887), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(888), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(889), UNITY_DISPLAY_STYLE_INT);



  int numberOfCycleToIncreaseRateAtNorminalRate = (72000000/(((10*60*60*80)/60)/500))/27000 + 1;

  int numberOfCycleToIncreaseRateAtInitRate = (72000000/(((10*60*60*80)/60)/500))/54000 + 1;







  while(stExecutor.stepEventsCompleted < numberOfCycleToIncreaseRateAtInitRate ){

     stExecutor.stepEventsCompleted++;

     accelerateAndDeccelerateEvent();

  }

  UnityAssertEqualNumber((_U_SINT)((80500)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(900), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53640)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(901), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53639)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(902), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(903), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((14)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(904), UNITY_DISPLAY_STYLE_INT);





  while(stExecutor.stepEventsCompleted < currentBlock->accelerateUntil){

     stExecutor.stepEventsCompleted++;

     accelerateAndDeccelerateEvent();

  }



  UnityAssertEqualNumber((_U_SINT)((currentBlock->accelerateUntil)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(912), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(913), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((27000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(914), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((26999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(915), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(916), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((3334)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(917), UNITY_DISPLAY_STYLE_INT);



  while(stExecutor.stepEventsCompleted < (currentBlock->accelerateUntil+1000) ){

     stExecutor.stepEventsCompleted++;

     accelerateAndDeccelerateEvent();

  }



  UnityAssertEqualNumber((_U_SINT)((1000+currentBlock->accelerateUntil)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(924), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(925), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((27000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(926), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((26999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(927), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(928), UNITY_DISPLAY_STYLE_INT);



  while(stExecutor.stepEventsCompleted < (currentBlock->deccelerateAfter) ){

     stExecutor.stepEventsCompleted++;

     accelerateAndDeccelerateEvent();

  }

  UnityAssertEqualNumber((_U_SINT)((currentBlock->deccelerateAfter)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(934), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(935), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((27000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(936), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((26999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(937), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(938), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((stExecutor.cyclePerStepCounter)), (_U_SINT)((0)), (((void *)0)), (_U_UINT)(939), UNITY_DISPLAY_STYLE_INT);



  while(stExecutor.stepEventsCompleted < (currentBlock->deccelerateAfter) + numberOfCycleToIncreaseRateAtNorminalRate ){

     stExecutor.stepEventsCompleted++;

     accelerateAndDeccelerateEvent();

  }



  UnityAssertEqualNumber((_U_SINT)((159500)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(946), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((currentBlock->deccelerateAfter + numberOfCycleToIncreaseRateAtNorminalRate)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(947), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((6000)), (_U_SINT)((stExecutor.cyclePerStepCounter)), (((void *)0)), (_U_UINT)(948), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((27060)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(949), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((27059)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(950), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(951), UNITY_DISPLAY_STYLE_INT);



  while(stExecutor.stepEventsCompleted < currentBlock->stepEventCount - 1 ){

     stExecutor.stepEventsCompleted++;

     accelerateAndDeccelerateEvent();

  }





  UnityAssertEqualNumber((_U_SINT)((80500)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(959), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53640)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(960), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53639)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(961), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(962), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((currentBlock->stepEventCount - 1)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(963), UNITY_DISPLAY_STYLE_INT);





}





void test_motorRateControlProcess_(void){



  bufferHead = nextBlockIndex(bufferHead);

  blockConfig(&blockBuffer[bufferTail],0x00,10000,8000,2000,80000,160000,80000);

  stExecutorInitProcess();





  UnityAssertEqualNumber((_U_SINT)((3334)), (_U_SINT)((currentBlock->accelerateUntil)), (((void *)0)), (_U_UINT)(976), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((6666)), (_U_SINT)((currentBlock->deccelerateAfter)), (((void *)0)), (_U_UINT)(977), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(978), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((54000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(979), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(980), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(981), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(982), UNITY_DISPLAY_STYLE_INT);



 int numberOfCycleToIncreaseRateAtInitRate = (72000000/(((10*60*60*80)/60)/500))/((72000000/currentBlock->initialRate)*60) + 1;

 int numberOfCycleToIncreaseRateAtNorminalRate = (72000000/(((10*60*60*80)/60)/500))/((72000000/currentBlock->nominalRate)*60) + 1;





  while(stExecutor.stepEventsCompleted < numberOfCycleToIncreaseRateAtInitRate ){

     stExecutor.stepEventsCompleted++;

     motorRateControlProcess();

  }

  UnityAssertEqualNumber((_U_SINT)((80500)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(992), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53640)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(993), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53639)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(994), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(995), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((14)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(996), UNITY_DISPLAY_STYLE_INT);





  while(stExecutor.stepEventsCompleted < currentBlock->accelerateUntil){

     stExecutor.stepEventsCompleted++;

     motorRateControlProcess();

  }



  UnityAssertEqualNumber((_U_SINT)((currentBlock->accelerateUntil)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(1004), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(1005), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((27000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(1006), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((26999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(1007), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(1008), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((3334)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(1009), UNITY_DISPLAY_STYLE_INT);



  while(stExecutor.stepEventsCompleted < (currentBlock->accelerateUntil+1000) ){

     stExecutor.stepEventsCompleted++;

     motorRateControlProcess();

  }



  UnityAssertEqualNumber((_U_SINT)((1000+currentBlock->accelerateUntil)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(1016), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(1017), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((27000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(1018), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((26999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(1019), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(1020), UNITY_DISPLAY_STYLE_INT);



  while(stExecutor.stepEventsCompleted < (currentBlock->deccelerateAfter) ){

     stExecutor.stepEventsCompleted++;

     motorRateControlProcess();

  }

  UnityAssertEqualNumber((_U_SINT)((currentBlock->deccelerateAfter)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(1026), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(1027), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((27000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(1028), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((26999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(1029), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(1030), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((stExecutor.cyclePerStepCounter)), (_U_SINT)((0)), (((void *)0)), (_U_UINT)(1031), UNITY_DISPLAY_STYLE_INT);



  while(stExecutor.stepEventsCompleted < (currentBlock->deccelerateAfter) + numberOfCycleToIncreaseRateAtNorminalRate ){

     stExecutor.stepEventsCompleted++;

     motorRateControlProcess();

  }



  UnityAssertEqualNumber((_U_SINT)((159500)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(1038), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((currentBlock->deccelerateAfter + numberOfCycleToIncreaseRateAtNorminalRate)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(1039), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((6000)), (_U_SINT)((stExecutor.cyclePerStepCounter)), (((void *)0)), (_U_UINT)(1040), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((27060)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(1041), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((27059)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(1042), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(1043), UNITY_DISPLAY_STYLE_INT);



  while(stExecutor.stepEventsCompleted < currentBlock->stepEventCount - 1 ){

     stExecutor.stepEventsCompleted++;

     motorRateControlProcess();

  }



  UnityAssertEqualNumber((_U_SINT)((80500)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(1050), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53640)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(1051), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53639)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(1052), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(1053), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((currentBlock->stepEventCount - 1)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(1054), UNITY_DISPLAY_STYLE_INT);





  stExecutor.stepEventsCompleted++;

  motorRateControlProcess();



  UnityAssertEqualNumber((_U_SINT)((stExecutor.eventCount)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(1060), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((bufferHead)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(1061), UNITY_DISPLAY_STYLE_INT);

  if ((((currentBlock)) == ((void *)0))) {} else {UnityFail( (((" Expected NULL"))), (_U_UINT)((_U_UINT)((_U_UINT)(1062))));};

}







void test_TIM2_IRQHandler_Add_a_block_into_blockBuffer_(void){



  bufferHead = nextBlockIndex(bufferHead);

  blockConfig(&blockBuffer[bufferTail],0x00,10000,8000,2000,80000,160000,80000);

  TIM2_IRQHandler();

  UnityAssertEqualNumber((_U_SINT)((3334)), (_U_SINT)((currentBlock->accelerateUntil)), (((void *)0)), (_U_UINT)(1072), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((6666)), (_U_SINT)((currentBlock->deccelerateAfter)), (((void *)0)), (_U_UINT)(1073), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(1074), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((54000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(1075), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(1076), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(1077), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(1078), UNITY_DISPLAY_STYLE_INT);



  int numberOfCycleToIncreaseRateAtNorminalRate = (72000000/(((10*60*60*80)/60)/500))/((72000000/currentBlock->nominalRate)*60) + 1;

  int numberOfCycleToIncreaseRateAtInitRate = (72000000/(((10*60*60*80)/60)/500))/((72000000/currentBlock->initialRate)*60) + 1;











  while(stExecutor.stepEventsCompleted < currentBlock->accelerateUntil){

     TIM2_IRQHandler();

  }



  UnityAssertEqualNumber((_U_SINT)((currentBlock->accelerateUntil)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(1091), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(1092), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((27000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(1093), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((26999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(1094), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(1095), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((3334)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(1096), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((3334)), (_U_SINT)((sys.position[0])), (((void *)0)), (_U_UINT)(1097), UNITY_DISPLAY_STYLE_INT);



  while(stExecutor.stepEventsCompleted < (currentBlock->accelerateUntil+1000) ){

     TIM2_IRQHandler();

  }



  UnityAssertEqualNumber((_U_SINT)((1000+currentBlock->accelerateUntil)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(1103), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(1104), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((27000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(1105), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((26999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(1106), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(1107), UNITY_DISPLAY_STYLE_INT);



  while(stExecutor.stepEventsCompleted < (currentBlock->deccelerateAfter) ){

     TIM2_IRQHandler();

  }

  UnityAssertEqualNumber((_U_SINT)((currentBlock->deccelerateAfter)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(1112), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(1113), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((27000)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(1114), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((26999)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(1115), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(1116), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((stExecutor.cyclePerStepCounter)), (_U_SINT)((0)), (((void *)0)), (_U_UINT)(1117), UNITY_DISPLAY_STYLE_INT);







  while(stExecutor.stepEventsCompleted < currentBlock->stepEventCount - 1 ){

     TIM2_IRQHandler();

  }



  UnityAssertEqualNumber((_U_SINT)((80500)), (_U_SINT)((stExecutor.currentRate)), (((void *)0)), (_U_UINT)(1125), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53640)), (_U_SINT)((stExecutor.cyclePerStepEvent)), (((void *)0)), (_U_UINT)(1126), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((53639)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->ARR)), (((void *)0)), (_U_UINT)(1127), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((((TIM_TypeDef*) ((uint32_t)HostTim2))->PSC)), (((void *)0)), (_U_UINT)(1128), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((currentBlock->stepEventCount - 1)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(1129), UNITY_DISPLAY_STYLE_INT);



  TIM2_IRQHandler();



  UnityAssertEqualNumber((_U_SINT)((stExecutor.eventCount)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(1133), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((bufferHead)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(1134), UNITY_DISPLAY_STYLE_INT);

  if ((((currentBlock)) == ((void *)0))) {} else {UnityFail( (((" Expected NULL"))), (_U_UINT)((_U_UINT)((_U_UINT)(1135))));};

}
