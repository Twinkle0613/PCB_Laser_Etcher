#include "unity.h"
#include "Stepper.h"


//For Test Driver Development
#include "Host.h"
#include "Registers.h"
#include "peripherals.h"
//Included library by Following Stepper.h 
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "Dma.h"
#include "Motor.h"
#include "Planner.h"
#include "Spi.h"
#include "malloc.h"
#include "System.h"


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
  wakeUpState = 0; //reset
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
  HostGpioA = NULL;
  HostTim2 = NULL;
  HostNVIC = NULL;
  HostDma1 = NULL;
  HostDma1_Channel3 = NULL;
}


void test_TIM2_IRQHandler_the_function_is_called_by_CPU(void){
 
  TIM2_IRQHandler();
  TEST_ASSERT_EQUAL(motorInfoBuffer[X_MOTOR_RESET],motorDriveBuffer[X_MOTOR]);
	TEST_ASSERT_EQUAL(motorInfoBuffer[Y_MOTOR_RESET],motorDriveBuffer[Y_MOTOR]);
  TEST_ASSERT_EQUAL(motorInfoBuffer[Z_MOTOR_RESET],motorDriveBuffer[Z_MOTOR]);
  //TEST_ASSERT_EQUAL(3,DMA1_Channel3->CNDTR);
  TEST_ASSERT_EQUAL(0x0010,GPIOA->BRR);
  TEST_ASSERT_EQUAL(0x0010,GPIOA->BSRR);
  TEST_ASSERT_EQUAL(0xfffe,TIM2->SR);
   
}


void test_initStepper_reset_and_initilize_all_of_the_data(void){
  stepperInit();
  TEST_ASSERT_NULL(currentBlock);
  TEST_ASSERT_EQUAL(0,stExecutor.error[X_AXIS]);
  TEST_ASSERT_EQUAL(0,stExecutor.error[Y_AXIS]);
  TEST_ASSERT_EQUAL(0,stExecutor.error[Z_AXIS]);
  TEST_ASSERT_EQUAL(0,stExecutor.eventCount);
  TEST_ASSERT_EQUAL(0,stExecutor.steps[X_AXIS]);
  TEST_ASSERT_EQUAL(0,stExecutor.steps[Y_AXIS]);
  TEST_ASSERT_EQUAL(0,stExecutor.steps[Z_AXIS]);
  TEST_ASSERT_EQUAL(0,stExecutor.stepEventsCompleted);
}

void test_sleep_the_timer_interrupt_is_disable(void){
  sleep();
  TEST_ASSERT_EQUAL(0,wakeUpState);
}

void test_wakeup_the_timer_interrupt_is_enabled(void){
  wakeUp();
  TEST_ASSERT_EQUAL(1,wakeUpState);
}
/*------------stExecutorInitProcess-------------*/
/*
 C = currentBlock
 T = bufferTail
 H = bufferHead
      TH                         C
      VV                         V
     ----------------          -------  
    | 0 | 1 | 2 | 3 |   ----> | NULL |
    ----------------          -------
*/

void test_TIM2_IRQHandler_IRQ_to_request_block_when_buffer_block_didnt_contain_any_block(void){
  TIM2_IRQHandler();
  TEST_ASSERT_NULL(currentBlock);
}

/*
 C = currentBlock
 T = bufferTail
 H = bufferHead
      T             H           CT           H
      V            V            VV           V 
     ----------------          ----------------
    | 0 | 1 | 2 | 3 |   ----> | 0 | 1 | 2 | 3 | 
    ----------------          ----------------
*/
void test_TIM2_IRQHandler_IRQ_to_request_block_when_buffer_block_contain_blocks(void){
  bufferHead = nextBlockIndex(bufferHead);
  bufferHead = nextBlockIndex(bufferHead);
  bufferHead = nextBlockIndex(bufferHead);
  TIM2_IRQHandler();
  TEST_ASSERT_EQUAL_PTR(&(blockBuffer[bufferTail]),currentBlock);  
}

/*
 C = currentBlock
 T = bufferTail
 H = bufferHead
      TH                      
      VV                      
     ----------------         
    | 0 | 1 | 2 | 3 |   ---> sleep();
    ----------------          
*/
void test_TIM2_IRQHandler_no_more_block_for_IRQ_to_request_and_IRQ_will_go_to_sleep(void){
  wakeUpState = 1;
  TIM2_IRQHandler();
  TEST_ASSERT_NULL(currentBlock);
  TEST_ASSERT_EQUAL(0,wakeUpState);
}

void test_transferInfoToStExecutor_when_IRQ_successfully_get_a_block_and_transfer_info_to_st_executor_from_block(void){
  block_t newBlock;
  newBlock.stepEventCount = 400;
  newBlock.directionBits = 0x70;
  newBlock.steps[X_AXIS] = 400;
  newBlock.steps[Y_AXIS] = 300;
  newBlock.steps[Z_AXIS] = 200;  
  transferInfoToStExecutor(&newBlock);
  TEST_ASSERT_EQUAL(400,stExecutor.eventCount);
  TEST_ASSERT_EQUAL(-(stExecutor.eventCount>>1),stExecutor.error[X_AXIS]);
  TEST_ASSERT_EQUAL(-(stExecutor.eventCount>>1),stExecutor.error[Y_AXIS]);
  TEST_ASSERT_EQUAL(-(stExecutor.eventCount>>1),stExecutor.error[Z_AXIS]);
  TEST_ASSERT_EQUAL(400,stExecutor.steps[X_AXIS]);
  TEST_ASSERT_EQUAL(300,stExecutor.steps[Y_AXIS]);
  TEST_ASSERT_EQUAL(200,stExecutor.steps[Z_AXIS]);
  TEST_ASSERT_EQUAL(0x70,stExecutor.xyzDireation);
}

/*
 C = currentBlock
 T = bufferTail
 H = bufferHead

    C
    V
   ---   transferinfo
  |  |    --->    stExecutor
  --- 
*/

void test_TIM2_IRQHandler_get_block_from_block_buffer_and_transfer_block_info_into_st_executor(void){
    bufferHead = nextBlockIndex(bufferHead);
    blockBuffer[bufferTail].stepEventCount = 400;
    blockBuffer[bufferTail].directionBits = 0x70;
    blockBuffer[bufferTail].steps[X_AXIS] = 400;
    blockBuffer[bufferTail].steps[Y_AXIS] = 300;
    blockBuffer[bufferTail].steps[Z_AXIS] = 200;
    
    TIM2_IRQHandler();
    TEST_ASSERT_EQUAL(400,stExecutor.eventCount);
   // TEST_ASSERT_EQUAL(-(stExecutor.eventCount>>1),stExecutor.error[X_AXIS]);
   // TEST_ASSERT_EQUAL(-(stExecutor.eventCount>>1),stExecutor.error[Y_AXIS]);
   // TEST_ASSERT_EQUAL(-(stExecutor.eventCount>>1),stExecutor.error[Z_AXIS]);
    TEST_ASSERT_EQUAL(400,stExecutor.steps[X_AXIS]);
    TEST_ASSERT_EQUAL(300,stExecutor.steps[Y_AXIS]);
    TEST_ASSERT_EQUAL(200,stExecutor.steps[Z_AXIS]);
    TEST_ASSERT_EQUAL(0x70,stExecutor.xyzDireation);
}


/*-----------executeStepDisplacementProcess-------------*/
/*
        bresenhamAlgorithm
     Y
     |               P(10,10)
     |              *       
     |            *       
     |          *       
     |        * 
     |      *   
     |    *  
     |  * 
     |*
     ------------------------>X

*/
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
    TEST_ASSERT_EQUAL(v,y);
  }
  
}

/*
        bresenhamAlgorithm
     Y
     |              
     |                 P(20,15)      
     |                  *       
     |              * *      
     |            * 
     |        * *   
     |      *  
     |  * *
     |*
     ------------------------>X

*/
void test_bresenhamAlgorithm_when_dx_is_20_and_dy_is_15_(void){
  
  uint32_t dx,dy;
  int32_t error;
  dx = 100;
  dy = 20;
  error = -(dx>>1);
//  printf("error = %d\n",error);
  int y = 0;
  int i;
  int v = 0;
  int stepXstatus = 0;
  for(i = 1; i <= dx; i++){
    v = (dy*i)/dx;
    y += bresenhamAlgorithm(&error,dx,dy,&stepXstatus);
//    printf("v = %d , y = %d\n",v,y);
    //TEST_ASSERT_EQUAL(v,y);
  }
}


/*
    StepX                     StepY                           StepZ
      |           (20,20)        |                             |
      |            *             |                             |
      |          *               |                             |           (20,15)
      |        *                 |           (20,10)           |            *
      |      *                   |          * *                |        * *   
      |    *                     |      * *                    |      *  
      |  *                       |  * *                        |  * * 
      |*                         |*                            |*
      ------------------->        ------------------->         --------------------->
          eventCount                  eventCount                    eventCount

*/

void test_executeStepDisplament_XYZ_direations_are_positive_(void){
  int8_t output;
  block_t newBlock;
  int32_t errorX,errorY,errorZ;
  newBlock.stepEventCount = 20;
  newBlock.directionBits = 0x00;
  newBlock.steps[X_AXIS] = 20;
  newBlock.steps[Y_AXIS] = 10;
  newBlock.steps[Z_AXIS] = 15;  
  transferInfoToStExecutor(&newBlock);
  int positionX = 0,positionY = 0,positionZ = 0;
  int stepXstatus = 0,stepYstatus = 0,stepZstatus = 0;
  errorX = -(stExecutor.eventCount>>1);
  errorY = -(stExecutor.eventCount>>1);
  errorZ = -(stExecutor.eventCount>>1);
  double z;
  int i;
  
  for(i = 1; i <= stExecutor.eventCount; i++){
    output =  newBlock.directionBits;
    positionX += bresenhamAlgorithm(&errorX,stExecutor.eventCount,stExecutor.steps[X_AXIS],&stepXstatus);
    positionY += bresenhamAlgorithm(&errorY,stExecutor.eventCount,stExecutor.steps[Y_AXIS],&stepYstatus);
    positionZ +=  bresenhamAlgorithm(&errorZ,stExecutor.eventCount,stExecutor.steps[Z_AXIS],&stepZstatus);
    output |= executeStepDisplacement(X_AXIS);
    output |= executeStepDisplacement(Y_AXIS);
    output |= executeStepDisplacement(Z_AXIS);
    TEST_ASSERT_EQUAL(positionX,sys.position[X_AXIS]);
    TEST_ASSERT_EQUAL(stepXstatus,getOutputFlagStatus(output,X_AXIS));
    TEST_ASSERT_EQUAL(0,getDireationFlag(output,X_AXIS));

    TEST_ASSERT_EQUAL(positionY,sys.position[Y_AXIS]);
    TEST_ASSERT_EQUAL(stepYstatus,getOutputFlagStatus(output,Y_AXIS));
    TEST_ASSERT_EQUAL(0,getDireationFlag(output,Y_AXIS));

    TEST_ASSERT_EQUAL(positionZ,sys.position[Z_AXIS]);
    TEST_ASSERT_EQUAL(stepZstatus,getOutputFlagStatus(output,Z_AXIS));
    TEST_ASSERT_EQUAL(0,getDireationFlag(output,Z_AXIS));
  } 

}

/*
           eventCount                  eventCount                    eventCount
      ------------------->       ------------------->          ------------------->
      |*                         |*                            |*
      |  *                       |  * *                        |  * *
      |    *                     |      * *                    |      *
      |      *                   |          * *                |        * *   
      |        *                 |            (20,-10)         |            *
      |          *               |                             |           (20,-15)
      |            *             |                             |  
      |         (20,-20)         |                             |
    StepX                      StepY                         StepZ
          

*/
void test_executeStepDisplament_XYZ_direations_are_negative_(void){
  uint8_t output;
  block_t newBlock;
  int32_t errorX,errorY,errorZ;
  newBlock.stepEventCount = 20;
  newBlock.directionBits = 0xe0;
  newBlock.steps[X_AXIS] = 20;
  newBlock.steps[Y_AXIS] = 10;
  newBlock.steps[Z_AXIS] = 15;  
  transferInfoToStExecutor(&newBlock);
  int32_t positionX = 0,positionY = 0,positionZ = 0;
  int stepXstatus = 0,stepYstatus = 0,stepZstatus = 0;
  errorX = -(stExecutor.eventCount>>1);
  errorY = -(stExecutor.eventCount>>1);
  errorZ = -(stExecutor.eventCount>>1);
  
  int i;
  
  for(i = 1; i <= stExecutor.eventCount; i++){
    output =  newBlock.directionBits;
    
    positionX -= bresenhamAlgorithm(&errorX,stExecutor.eventCount,stExecutor.steps[X_AXIS],&stepXstatus);
    positionY -= bresenhamAlgorithm(&errorY,stExecutor.eventCount,stExecutor.steps[Y_AXIS],&stepYstatus);
    positionZ -=  bresenhamAlgorithm(&errorZ,stExecutor.eventCount,stExecutor.steps[Z_AXIS],&stepZstatus);
    output |= executeStepDisplacement(X_AXIS);
    output |= executeStepDisplacement(Y_AXIS);
    output |= executeStepDisplacement(Z_AXIS);
    
    TEST_ASSERT_EQUAL(positionX,sys.position[X_AXIS]);
    TEST_ASSERT_EQUAL(stepXstatus,getOutputFlagStatus(output,X_AXIS));
    TEST_ASSERT_EQUAL(1,getDireationFlag(output,X_AXIS));
    
    TEST_ASSERT_EQUAL(positionY,sys.position[Y_AXIS]);
    TEST_ASSERT_EQUAL(stepYstatus,getOutputFlagStatus(output,Y_AXIS));
    TEST_ASSERT_EQUAL(1,getDireationFlag(output,Y_AXIS));
    
    TEST_ASSERT_EQUAL(positionZ,sys.position[Z_AXIS]);
    TEST_ASSERT_EQUAL(stepZstatus,getOutputFlagStatus(output,Z_AXIS));
    TEST_ASSERT_EQUAL(1,getDireationFlag(output,Z_AXIS));
  } 

}

void test_executeStepDisplacementProcess_the_functon_is_called_and_using_for_loop(void){
  
  block_t newBlock;
  int32_t errorX,errorY,errorZ;
  newBlock.stepEventCount = 100;
  newBlock.directionBits = 0x00;
  newBlock.steps[X_AXIS] = 100;
  newBlock.steps[Y_AXIS] = 75;
  newBlock.steps[Z_AXIS] = 10;  
  transferInfoToStExecutor(&newBlock);
  int32_t positionX = 0,positionY = 0,positionZ = 0;
  int stepXstatus = 0,stepYstatus = 0,stepZstatus = 0;
  errorX = -(stExecutor.eventCount>>1);
  errorY = -(stExecutor.eventCount>>1);
  errorZ = -(stExecutor.eventCount>>1);
  int i;
  
   
  for(i = 1; i <= stExecutor.eventCount; i++){
    executeStepDisplacementProcess();
    
    positionX += bresenhamAlgorithm(&errorX,stExecutor.eventCount,stExecutor.steps[X_AXIS],&stepXstatus);
    positionY += bresenhamAlgorithm(&errorY,stExecutor.eventCount,stExecutor.steps[Y_AXIS],&stepYstatus);
    positionZ +=  bresenhamAlgorithm(&errorZ,stExecutor.eventCount,stExecutor.steps[Z_AXIS],&stepZstatus);
    
    TEST_ASSERT_EQUAL(positionX,sys.position[X_AXIS]);
    TEST_ASSERT_EQUAL(stepXstatus,getOutputFlagStatus(outputBits,X_AXIS));
    TEST_ASSERT_EQUAL(0,getDireationFlag(outputBits,X_AXIS));
    
    TEST_ASSERT_EQUAL(positionY,sys.position[Y_AXIS]);
    TEST_ASSERT_EQUAL(stepYstatus,getOutputFlagStatus(outputBits,Y_AXIS));
    TEST_ASSERT_EQUAL(0,getDireationFlag(outputBits,Y_AXIS));
    
    TEST_ASSERT_EQUAL(positionZ,sys.position[Z_AXIS]);
    TEST_ASSERT_EQUAL(stepZstatus,getOutputFlagStatus(outputBits,Z_AXIS));
    TEST_ASSERT_EQUAL(0,getDireationFlag(outputBits,Z_AXIS));
  }
 
}



/*-----------planningDataSending-------------*/
void test_getStepStatus_when_x_y_z_rest(void){
  outputBits = 0b11100000;
  TEST_ASSERT_EQUAL(MOTOR_STEP_LOW,getStepStatus(X_AXIS));
  TEST_ASSERT_EQUAL(MOTOR_STEP_LOW,getStepStatus(Y_AXIS));
  TEST_ASSERT_EQUAL(MOTOR_STEP_LOW,getStepStatus(Z_AXIS));
}

void test_getStepStatus_when_x_y_z_move_(void){
  outputBits = 0b11111100;
  TEST_ASSERT_EQUAL(MOTOR_STEP_HIGH,getStepStatus(X_AXIS));
  TEST_ASSERT_EQUAL(MOTOR_STEP_HIGH,getStepStatus(Y_AXIS));
  TEST_ASSERT_EQUAL(MOTOR_STEP_HIGH,getStepStatus(Z_AXIS));
}

void test_getMoveDireaction_when_x_y_z_are_moving_right_direation(void){
  outputBits = 0b00000000;
  TEST_ASSERT_EQUAL(MOTOR_LEFT,getMoveDireaction(X_AXIS));
  TEST_ASSERT_EQUAL(MOTOR_LEFT,getMoveDireaction(Y_AXIS));
  TEST_ASSERT_EQUAL(MOTOR_LEFT,getMoveDireaction(Z_AXIS));
}

void test_getMoveDireaction_when_x_y_z_are_moving_left_direation(void){
  outputBits = 0b11111100;
  TEST_ASSERT_EQUAL(MOTOR_RIGHT,getMoveDireaction(X_AXIS));
  TEST_ASSERT_EQUAL(MOTOR_RIGHT,getMoveDireaction(Y_AXIS));
  TEST_ASSERT_EQUAL(MOTOR_RIGHT,getMoveDireaction(Z_AXIS));
}

void test_planningDataSending_XYZ_direations_are_negative_(void){
  outputBits = 0b11111100;
  planningDataSending();
  TEST_ASSERT_EQUAL(getMotorInfo(motorX,MOTOR_RIGHT,MOTOR_STEP_HIGH),motorInfoBuffer[X_MOTOR_SET]);
  TEST_ASSERT_EQUAL(getMotorInfo(motorY,MOTOR_RIGHT,MOTOR_STEP_HIGH),motorInfoBuffer[Y_MOTOR_SET]);
  TEST_ASSERT_EQUAL(getMotorInfo(motorZ,MOTOR_RIGHT,MOTOR_STEP_HIGH),motorInfoBuffer[Z_MOTOR_SET]);
  TEST_ASSERT_EQUAL(getMotorInfo(motorZ,MOTOR_RIGHT,MOTOR_STEP_LOW),motorInfoBuffer[X_MOTOR_RESET]);
  TEST_ASSERT_EQUAL(getMotorInfo(motorZ,MOTOR_RIGHT,MOTOR_STEP_LOW),motorInfoBuffer[Y_MOTOR_RESET]);
  TEST_ASSERT_EQUAL(getMotorInfo(motorZ,MOTOR_RIGHT,MOTOR_STEP_LOW),motorInfoBuffer[Z_MOTOR_RESET]);
}

void test_planningDataSending_XYZ_direations_are_positive_(void){
  outputBits = 0b00011100;
  planningDataSending();
  TEST_ASSERT_EQUAL(getMotorInfo(motorX,MOTOR_LEFT,MOTOR_STEP_HIGH),motorInfoBuffer[X_MOTOR_SET]);
  TEST_ASSERT_EQUAL(getMotorInfo(motorY,MOTOR_LEFT,MOTOR_STEP_HIGH),motorInfoBuffer[Y_MOTOR_SET]);
  TEST_ASSERT_EQUAL(getMotorInfo(motorZ,MOTOR_LEFT,MOTOR_STEP_HIGH),motorInfoBuffer[Z_MOTOR_SET]);
  TEST_ASSERT_EQUAL(getMotorInfo(motorZ,MOTOR_LEFT,MOTOR_STEP_LOW),motorInfoBuffer[X_MOTOR_RESET]);
  TEST_ASSERT_EQUAL(getMotorInfo(motorZ,MOTOR_LEFT,MOTOR_STEP_LOW),motorInfoBuffer[Y_MOTOR_RESET]);
  TEST_ASSERT_EQUAL(getMotorInfo(motorZ,MOTOR_LEFT,MOTOR_STEP_LOW),motorInfoBuffer[Z_MOTOR_RESET]);
}

void test_planningDataSending_XYZ_motor_take_a_rest_(void){
  outputBits = 0b11100000;
  planningDataSending();
  TEST_ASSERT_EQUAL(getMotorInfo(motorX,MOTOR_RIGHT,MOTOR_STEP_LOW),motorInfoBuffer[X_MOTOR_SET]);
  TEST_ASSERT_EQUAL(getMotorInfo(motorY,MOTOR_RIGHT,MOTOR_STEP_LOW),motorInfoBuffer[Y_MOTOR_SET]);
  TEST_ASSERT_EQUAL(getMotorInfo(motorZ,MOTOR_RIGHT,MOTOR_STEP_LOW),motorInfoBuffer[Z_MOTOR_SET]);
  TEST_ASSERT_EQUAL(getMotorInfo(motorZ,MOTOR_RIGHT,MOTOR_STEP_LOW),motorInfoBuffer[X_MOTOR_RESET]);
  TEST_ASSERT_EQUAL(getMotorInfo(motorZ,MOTOR_RIGHT,MOTOR_STEP_LOW),motorInfoBuffer[Y_MOTOR_RESET]);
  TEST_ASSERT_EQUAL(getMotorInfo(motorZ,MOTOR_RIGHT,MOTOR_STEP_LOW),motorInfoBuffer[Z_MOTOR_RESET]);
}


/*--------executeStepDisplacementProcess + splanningDataSending------*/

void test_executeStepDisplacementProcess_planningDataSending_the_functon_is_called_and_using_for_loop(void){
  
  block_t newBlock;
  int32_t errorX,errorY,errorZ;
  newBlock.stepEventCount = 100;
  newBlock.directionBits = 0x00;
  newBlock.steps[X_AXIS] = 100;
  newBlock.steps[Y_AXIS] = 75;
  newBlock.steps[Z_AXIS] = 10;  
  transferInfoToStExecutor(&newBlock);
  int32_t positionX = 0,positionY = 0,positionZ = 0;
  int stepXstatus = 0,stepYstatus = 0,stepZstatus = 0;
  errorX = -(stExecutor.eventCount>>1);
  errorY = -(stExecutor.eventCount>>1);
  errorZ = -(stExecutor.eventCount>>1);
  int i;
  
   
  for(i = 1; i <= stExecutor.eventCount; i++){
    executeStepDisplacementProcess();    
    TEST_ASSERT_EQUAL(getMotorInfo(motorX,getMoveDireaction(X_AXIS),getStepStatus(X_AXIS)),motorInfoBuffer[X_MOTOR_SET]);
    TEST_ASSERT_EQUAL(getMotorInfo(motorY,getMoveDireaction(Y_AXIS),getStepStatus(Y_AXIS)),motorInfoBuffer[Y_MOTOR_SET]);
    TEST_ASSERT_EQUAL(getMotorInfo(motorX,getMoveDireaction(Z_AXIS),getStepStatus(Z_AXIS)),motorInfoBuffer[Z_MOTOR_SET]);
    
    TEST_ASSERT_EQUAL(getMotorInfo(motorX,getMoveDireaction(X_AXIS),MOTOR_STEP_LOW),motorInfoBuffer[X_MOTOR_RESET]);
    TEST_ASSERT_EQUAL(getMotorInfo(motorY,getMoveDireaction(Y_AXIS),MOTOR_STEP_LOW),motorInfoBuffer[Y_MOTOR_RESET]);
    TEST_ASSERT_EQUAL(getMotorInfo(motorZ,getMoveDireaction(Z_AXIS),MOTOR_STEP_LOW),motorInfoBuffer[Z_MOTOR_RESET]);
  }
}

/*--------executeStepDisplacementProcess + splanningDataSending + sendResetStep------*/

void test_executeStepDisplacementProcess_planningDataSending_sendResetStep_the_functon_is_called_and_using_for_loop(void){
  
  block_t newBlock;
  int32_t errorX,errorY,errorZ;
  newBlock.stepEventCount = 100;
  newBlock.directionBits = 0x00;
  newBlock.steps[X_AXIS] = 100;
  newBlock.steps[Y_AXIS] = 75;
  newBlock.steps[Z_AXIS] = 10;  
  transferInfoToStExecutor(&newBlock);
  int32_t positionX = 0,positionY = 0,positionZ = 0;
  int stepXstatus = 0,stepYstatus = 0,stepZstatus = 0;
  errorX = -(stExecutor.eventCount>>1);
  errorY = -(stExecutor.eventCount>>1);
  errorZ = -(stExecutor.eventCount>>1);
  int i;
  
   
  for(i = 1; i <= stExecutor.eventCount; i++){
    executeStepDisplacementProcess();    
    TEST_ASSERT_EQUAL(getMotorInfo(motorX,getMoveDireaction(X_AXIS),MOTOR_STEP_LOW),motorDriveBuffer[X_MOTOR]);
    TEST_ASSERT_EQUAL(getMotorInfo(motorY,getMoveDireaction(Y_AXIS),MOTOR_STEP_LOW),motorDriveBuffer[Y_MOTOR]);
    TEST_ASSERT_EQUAL(getMotorInfo(motorZ,getMoveDireaction(Z_AXIS),MOTOR_STEP_LOW),motorDriveBuffer[Z_MOTOR]);
  }
}

void test_TIM2_IRQHandler_and_executeStepDisplacementProcess_simulate_when_TIM_interrupt_occur(void){
    bufferHead = nextBlockIndex(bufferHead);
    blockBuffer[bufferTail].stepEventCount = 100;
    blockBuffer[bufferTail].directionBits = 0x00;
    blockBuffer[bufferTail].steps[X_AXIS] = 100;
    blockBuffer[bufferTail].steps[Y_AXIS] = 75;
    blockBuffer[bufferTail].steps[Z_AXIS] = 10;
      
  int i;
  for(i = 1; i <= blockBuffer[bufferTail].stepEventCount; i++){
    TIM2_IRQHandler();
    TEST_ASSERT_EQUAL(getMotorInfo(motorX,getMoveDireaction(X_AXIS),MOTOR_STEP_LOW),motorDriveBuffer[X_MOTOR]);
    TEST_ASSERT_EQUAL(getMotorInfo(motorY,getMoveDireaction(Y_AXIS),MOTOR_STEP_LOW),motorDriveBuffer[Y_MOTOR]);
    TEST_ASSERT_EQUAL(getMotorInfo(motorZ,getMoveDireaction(Z_AXIS),MOTOR_STEP_LOW),motorDriveBuffer[Z_MOTOR]);
  }
  TEST_ASSERT_EQUAL(stExecutor.eventCount,stExecutor.stepEventsCompleted);
  //printf("stExecutor.stepEventsCompleted = %d\n",stExecutor.stepEventsCompleted);
}