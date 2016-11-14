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

  TIM2_IRQHandler();

  UnityAssertEqualNumber((_U_SINT)(_UP)((&(blockBuffer[bufferTail]))), (_U_SINT)(_UP)((currentBlock)), (((void *)0)), (_U_UINT)(148), UNITY_DISPLAY_STYLE_HEX32);

}

void test_TIM2_IRQHandler_no_more_block_for_IRQ_to_request_and_IRQ_will_go_to_sleep(void){

  wakeUpState = 1;

  TIM2_IRQHandler();

  if ((((currentBlock)) == ((void *)0))) {} else {UnityFail( (((" Expected NULL"))), (_U_UINT)((_U_UINT)((_U_UINT)(164))));};

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((wakeUpState)), (((void *)0)), (_U_UINT)(165), UNITY_DISPLAY_STYLE_INT);

}



void test_transferInfoToStExecutor_when_IRQ_successfully_get_a_block_and_transfer_info_to_st_executor_from_block(void){

  block_t newBlock;

  newBlock.stepEventCount = 400;

  newBlock.directionBits = 0x70;

  newBlock.steps[0] = 400;

  newBlock.steps[1] = 300;

  newBlock.steps[2] = 200;

  transferInfoToStExecutor(&newBlock);

  UnityAssertEqualNumber((_U_SINT)((400)), (_U_SINT)((stExecutor.eventCount)), (((void *)0)), (_U_UINT)(176), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((-(stExecutor.eventCount>>1))), (_U_SINT)((stExecutor.error[0])), (((void *)0)), (_U_UINT)(177), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((-(stExecutor.eventCount>>1))), (_U_SINT)((stExecutor.error[1])), (((void *)0)), (_U_UINT)(178), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((-(stExecutor.eventCount>>1))), (_U_SINT)((stExecutor.error[2])), (((void *)0)), (_U_UINT)(179), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((400)), (_U_SINT)((stExecutor.steps[0])), (((void *)0)), (_U_UINT)(180), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((300)), (_U_SINT)((stExecutor.steps[1])), (((void *)0)), (_U_UINT)(181), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((stExecutor.steps[2])), (((void *)0)), (_U_UINT)(182), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0x70)), (_U_SINT)((stExecutor.xyzDireation)), (((void *)0)), (_U_UINT)(183), UNITY_DISPLAY_STYLE_INT);

}

void test_TIM2_IRQHandler_get_block_from_block_buffer_and_transfer_block_info_into_st_executor(void){

    bufferHead = nextBlockIndex(bufferHead);

    blockBuffer[bufferTail].stepEventCount = 400;

    blockBuffer[bufferTail].directionBits = 0x70;

    blockBuffer[bufferTail].steps[0] = 400;

    blockBuffer[bufferTail].steps[1] = 300;

    blockBuffer[bufferTail].steps[2] = 200;



    TIM2_IRQHandler();

    UnityAssertEqualNumber((_U_SINT)((400)), (_U_SINT)((stExecutor.eventCount)), (((void *)0)), (_U_UINT)(207), UNITY_DISPLAY_STYLE_INT);







    UnityAssertEqualNumber((_U_SINT)((400)), (_U_SINT)((stExecutor.steps[0])), (((void *)0)), (_U_UINT)(211), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((300)), (_U_SINT)((stExecutor.steps[1])), (((void *)0)), (_U_UINT)(212), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((stExecutor.steps[2])), (((void *)0)), (_U_UINT)(213), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((0x70)), (_U_SINT)((stExecutor.xyzDireation)), (((void *)0)), (_U_UINT)(214), UNITY_DISPLAY_STYLE_INT);

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

    UnityAssertEqualNumber((_U_SINT)((v)), (_U_SINT)((y)), (((void *)0)), (_U_UINT)(248), UNITY_DISPLAY_STYLE_INT);

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

    UnityAssertEqualNumber((_U_SINT)((positionX)), (_U_SINT)((sys.position[0])), (((void *)0)), (_U_UINT)(330), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepXstatus)), (_U_SINT)((getOutputFlagStatus(output,0))), (((void *)0)), (_U_UINT)(331), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((getDireationFlag(output,0))), (((void *)0)), (_U_UINT)(332), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((_U_SINT)((positionY)), (_U_SINT)((sys.position[1])), (((void *)0)), (_U_UINT)(334), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepYstatus)), (_U_SINT)((getOutputFlagStatus(output,1))), (((void *)0)), (_U_UINT)(335), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((getDireationFlag(output,1))), (((void *)0)), (_U_UINT)(336), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((_U_SINT)((positionZ)), (_U_SINT)((sys.position[2])), (((void *)0)), (_U_UINT)(338), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepZstatus)), (_U_SINT)((getOutputFlagStatus(output,2))), (((void *)0)), (_U_UINT)(339), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((getDireationFlag(output,2))), (((void *)0)), (_U_UINT)(340), UNITY_DISPLAY_STYLE_INT);

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



    UnityAssertEqualNumber((_U_SINT)((positionX)), (_U_SINT)((sys.position[0])), (((void *)0)), (_U_UINT)(388), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepXstatus)), (_U_SINT)((getOutputFlagStatus(output,0))), (((void *)0)), (_U_UINT)(389), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((getDireationFlag(output,0))), (((void *)0)), (_U_UINT)(390), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((_U_SINT)((positionY)), (_U_SINT)((sys.position[1])), (((void *)0)), (_U_UINT)(392), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepYstatus)), (_U_SINT)((getOutputFlagStatus(output,1))), (((void *)0)), (_U_UINT)(393), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((getDireationFlag(output,1))), (((void *)0)), (_U_UINT)(394), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((_U_SINT)((positionZ)), (_U_SINT)((sys.position[2])), (((void *)0)), (_U_UINT)(396), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepZstatus)), (_U_SINT)((getOutputFlagStatus(output,2))), (((void *)0)), (_U_UINT)(397), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((getDireationFlag(output,2))), (((void *)0)), (_U_UINT)(398), UNITY_DISPLAY_STYLE_INT);

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



    UnityAssertEqualNumber((_U_SINT)((positionX)), (_U_SINT)((sys.position[0])), (((void *)0)), (_U_UINT)(428), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepXstatus)), (_U_SINT)((getOutputFlagStatus(outputBits,0))), (((void *)0)), (_U_UINT)(429), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((getDireationFlag(outputBits,0))), (((void *)0)), (_U_UINT)(430), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((_U_SINT)((positionY)), (_U_SINT)((sys.position[1])), (((void *)0)), (_U_UINT)(432), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepYstatus)), (_U_SINT)((getOutputFlagStatus(outputBits,1))), (((void *)0)), (_U_UINT)(433), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((getDireationFlag(outputBits,1))), (((void *)0)), (_U_UINT)(434), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((_U_SINT)((positionZ)), (_U_SINT)((sys.position[2])), (((void *)0)), (_U_UINT)(436), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((stepZstatus)), (_U_SINT)((getOutputFlagStatus(outputBits,2))), (((void *)0)), (_U_UINT)(437), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((getDireationFlag(outputBits,2))), (((void *)0)), (_U_UINT)(438), UNITY_DISPLAY_STYLE_INT);

  }



}









void test_getStepStatus_when_x_y_z_rest(void){

  outputBits = 0b11100000;

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0xBF))), (_U_SINT)((getStepStatus(0))), (((void *)0)), (_U_UINT)(448), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0xBF))), (_U_SINT)((getStepStatus(1))), (((void *)0)), (_U_UINT)(449), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0xBF))), (_U_SINT)((getStepStatus(2))), (((void *)0)), (_U_UINT)(450), UNITY_DISPLAY_STYLE_INT);

}



void test_getStepStatus_when_x_y_z_move_(void){

  outputBits = 0b11111100;

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x40))), (_U_SINT)((getStepStatus(0))), (((void *)0)), (_U_UINT)(455), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x40))), (_U_SINT)((getStepStatus(1))), (((void *)0)), (_U_UINT)(456), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x40))), (_U_SINT)((getStepStatus(2))), (((void *)0)), (_U_UINT)(457), UNITY_DISPLAY_STYLE_INT);

}



void test_getMoveDireaction_when_x_y_z_are_moving_right_direation(void){

  outputBits = 0b00000000;

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x80))), (_U_SINT)((getMoveDireaction(0))), (((void *)0)), (_U_UINT)(462), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x80))), (_U_SINT)((getMoveDireaction(1))), (((void *)0)), (_U_UINT)(463), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x80))), (_U_SINT)((getMoveDireaction(2))), (((void *)0)), (_U_UINT)(464), UNITY_DISPLAY_STYLE_INT);

}



void test_getMoveDireaction_when_x_y_z_are_moving_left_direation(void){

  outputBits = 0b11111100;

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x00))), (_U_SINT)((getMoveDireaction(0))), (((void *)0)), (_U_UINT)(469), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x00))), (_U_SINT)((getMoveDireaction(1))), (((void *)0)), (_U_UINT)(470), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x00))), (_U_SINT)((getMoveDireaction(2))), (((void *)0)), (_U_UINT)(471), UNITY_DISPLAY_STYLE_INT);

}



void test_planningDataSending_XYZ_direations_are_negative_(void){

  outputBits = 0b11111100;

  planningDataSending();

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor0,((uint8_t)0x00),((uint8_t)0x40)))), (_U_SINT)((motorInfoBuffer[3])), (((void *)0)), (_U_UINT)(477), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor1,((uint8_t)0x00),((uint8_t)0x40)))), (_U_SINT)((motorInfoBuffer[4])), (((void *)0)), (_U_UINT)(478), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x00),((uint8_t)0x40)))), (_U_SINT)((motorInfoBuffer[5])), (((void *)0)), (_U_UINT)(479), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[0])), (((void *)0)), (_U_UINT)(480), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[1])), (((void *)0)), (_U_UINT)(481), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[2])), (((void *)0)), (_U_UINT)(482), UNITY_DISPLAY_STYLE_INT);

}



void test_planningDataSending_XYZ_direations_are_positive_(void){

  outputBits = 0b00011100;

  planningDataSending();

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor0,((uint8_t)0x80),((uint8_t)0x40)))), (_U_SINT)((motorInfoBuffer[3])), (((void *)0)), (_U_UINT)(488), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor1,((uint8_t)0x80),((uint8_t)0x40)))), (_U_SINT)((motorInfoBuffer[4])), (((void *)0)), (_U_UINT)(489), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x80),((uint8_t)0x40)))), (_U_SINT)((motorInfoBuffer[5])), (((void *)0)), (_U_UINT)(490), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x80),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[0])), (((void *)0)), (_U_UINT)(491), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x80),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[1])), (((void *)0)), (_U_UINT)(492), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x80),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[2])), (((void *)0)), (_U_UINT)(493), UNITY_DISPLAY_STYLE_INT);

}



void test_planningDataSending_XYZ_motor_take_a_rest_(void){

  outputBits = 0b11100000;

  planningDataSending();

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor0,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[3])), (((void *)0)), (_U_UINT)(499), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor1,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[4])), (((void *)0)), (_U_UINT)(500), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[5])), (((void *)0)), (_U_UINT)(501), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[0])), (((void *)0)), (_U_UINT)(502), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[1])), (((void *)0)), (_U_UINT)(503), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,((uint8_t)0x00),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[2])), (((void *)0)), (_U_UINT)(504), UNITY_DISPLAY_STYLE_INT);

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

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor0,getMoveDireaction(0),getStepStatus(0)))), (_U_SINT)((motorInfoBuffer[3])), (((void *)0)), (_U_UINT)(530), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor1,getMoveDireaction(1),getStepStatus(1)))), (_U_SINT)((motorInfoBuffer[4])), (((void *)0)), (_U_UINT)(531), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor0,getMoveDireaction(2),getStepStatus(2)))), (_U_SINT)((motorInfoBuffer[5])), (((void *)0)), (_U_UINT)(532), UNITY_DISPLAY_STYLE_INT);



    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor0,getMoveDireaction(0),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[0])), (((void *)0)), (_U_UINT)(534), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor1,getMoveDireaction(1),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[1])), (((void *)0)), (_U_UINT)(535), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,getMoveDireaction(2),((uint8_t)0xBF)))), (_U_SINT)((motorInfoBuffer[2])), (((void *)0)), (_U_UINT)(536), UNITY_DISPLAY_STYLE_INT);

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

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor0,getMoveDireaction(0),((uint8_t)0xBF)))), (_U_SINT)((motorDriveBuffer[0])), (((void *)0)), (_U_UINT)(562), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor1,getMoveDireaction(1),((uint8_t)0xBF)))), (_U_SINT)((motorDriveBuffer[1])), (((void *)0)), (_U_UINT)(563), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,getMoveDireaction(2),((uint8_t)0xBF)))), (_U_SINT)((motorDriveBuffer[2])), (((void *)0)), (_U_UINT)(564), UNITY_DISPLAY_STYLE_INT);

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

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor0,getMoveDireaction(0),((uint8_t)0xBF)))), (_U_SINT)((motorDriveBuffer[0])), (((void *)0)), (_U_UINT)(579), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor1,getMoveDireaction(1),((uint8_t)0xBF)))), (_U_SINT)((motorDriveBuffer[1])), (((void *)0)), (_U_UINT)(580), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((_U_SINT)((getMotorInfo(motor2,getMoveDireaction(2),((uint8_t)0xBF)))), (_U_SINT)((motorDriveBuffer[2])), (((void *)0)), (_U_UINT)(581), UNITY_DISPLAY_STYLE_INT);

  }

  UnityAssertEqualNumber((_U_SINT)((stExecutor.eventCount)), (_U_SINT)((stExecutor.stepEventsCompleted)), (((void *)0)), (_U_UINT)(583), UNITY_DISPLAY_STYLE_INT);



}
