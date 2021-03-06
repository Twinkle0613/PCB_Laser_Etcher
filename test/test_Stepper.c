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
#include "Setting.h"


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
  blockConfig(&blockBuffer[bufferTail],0x00,10000,8000,2000,160000,160000,80000);
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
  newBlock.initialRate = 80000;
  transferInfoToStExecutor(&newBlock);
  TEST_ASSERT_EQUAL(400,stExecutor.eventCount);
  TEST_ASSERT_EQUAL(-(stExecutor.eventCount>>1),stExecutor.error[X_AXIS]);
  TEST_ASSERT_EQUAL(-(stExecutor.eventCount>>1),stExecutor.error[Y_AXIS]);
  TEST_ASSERT_EQUAL(-(stExecutor.eventCount>>1),stExecutor.error[Z_AXIS]);
  TEST_ASSERT_EQUAL(400,stExecutor.steps[X_AXIS]);
  TEST_ASSERT_EQUAL(300,stExecutor.steps[Y_AXIS]);
  TEST_ASSERT_EQUAL(200,stExecutor.steps[Z_AXIS]);
  TEST_ASSERT_EQUAL(0x70,stExecutor.xyzDireation);
  TEST_ASSERT_EQUAL(80000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(53999,TIM2->ARR);
  TEST_ASSERT_EQUAL(54000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(0,stExecutor.cyclePerStepCounter);
  TEST_ASSERT_EQUAL(750,stExecutor.minSafeRate);
  
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





// /*---------motorRateControlProcess--------*/
void test_motorRateControlProcess_when_motor_moving_step_is_finish_the_current_block_will_be_discarded(void){
    bufferHead = nextBlockIndex(bufferHead);
    blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,80000,160000,80000);
    stExecutorInitProcess();
    stExecutor.stepEventsCompleted = currentBlock->stepEventCount;
    motorRateControlProcess();
    TEST_ASSERT_NULL(currentBlock);
    TEST_ASSERT_EQUAL(bufferHead,bufferTail);  
    TEST_ASSERT_EQUAL(1,availableInsertBlock);
}


/*----------setTickPerStepEventToTimer-----------*/

void test_setTickPerStepEventToTimer_if_cycle_15000000_the_actual_cycle_is_58593_and_prescale_is_256(void){
 TEST_ASSERT_EQUAL(58593,setCyclePerStepEventToTimer(15000000));
 TEST_ASSERT_EQUAL(58592,TIM2->ARR);
 TEST_ASSERT_EQUAL(255,TIM2->PSC); 
}

void test_setTickPerStepEventToTimer_if_cycle_72000000_the_actual_cycle_is_35156_and_prescale_is_2047(void){
 TEST_ASSERT_EQUAL(35156,setCyclePerStepEventToTimer(72000000));
 TEST_ASSERT_EQUAL(35155,TIM2->ARR);
 TEST_ASSERT_EQUAL(2047,TIM2->PSC); 
}

void test_setTickPerStepEventToTimer_if_cycle_50000_the_actual_cycle_is_50000_and_prescale_is_0(void){
 TEST_ASSERT_EQUAL(50000,setCyclePerStepEventToTimer(50000));
 TEST_ASSERT_EQUAL(49999,TIM2->ARR);
 TEST_ASSERT_EQUAL(0,TIM2->PSC); 
}

void test_setTickPerStepEventToTimer_if_cycle_1_the_actual_cycle_is_1_and_prescale_is_0(void){
 TEST_ASSERT_EQUAL(1,setCyclePerStepEventToTimer(1));
 TEST_ASSERT_EQUAL(1,TIM2->ARR);
 TEST_ASSERT_EQUAL(0,TIM2->PSC); 
}

void test_setStepEventPerMin_the_steps_per_min_is_80000_the_cycle_will_be_54000(void){
  setStepEventPerMin(80000);
  TEST_ASSERT_EQUAL(54000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(53999,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
}

void test_setStepEventPerMin_the_steps_per_min_is_100_less_than_800_and_steps_per_min_will_become_800(void){
  setStepEventPerMin(100);
  TEST_ASSERT_EQUAL(42187,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(42186,TIM2->ARR);
  TEST_ASSERT_EQUAL(1023,TIM2->PSC); 
}
/*-------------iterateCycleCounter----------------*/

void test_iterateCycleCounter_when_currentRate_is_80000_after_the_function_in_several_loop_cyclePerStepConter_will_remain_6000(void){
   uint32_t currentRate = 80000;
   setStepEventPerMin(currentRate);
   int limit = CYCLES_PER_ACCELERATION_TICK/stExecutor.cyclePerStepEvent;
   stExecutor.cyclePerStepCounter = 0; 
   TEST_ASSERT_EQUAL(54000,stExecutor.cyclePerStepEvent);
   TEST_ASSERT_EQUAL(300000,CYCLES_PER_ACCELERATION_TICK);
   int i = 0;
   for(i = 0; i < limit ; i++){
    TEST_ASSERT_EQUAL(false,iterateCycleCounter());
   }
   TEST_ASSERT_EQUAL(true,iterateCycleCounter());
   TEST_ASSERT_EQUAL(24000,stExecutor.cyclePerStepCounter);
}

void xtest_iterateCycleCounter_currentRate_is_100_after_the_function_in_several_loop_cyclePerStepConter_will_remain_9366(void){
   uint32_t currentRate = 100;
   setStepEventPerMin(currentRate);
   int limit = CYCLES_PER_ACCELERATION_TICK/stExecutor.cyclePerStepEvent;
   stExecutor.cyclePerStepCounter = 0; 
   TEST_ASSERT_EQUAL(42187,stExecutor.cyclePerStepEvent);
   TEST_ASSERT_EQUAL(42186,TIM2->ARR);
   TEST_ASSERT_EQUAL(1023,TIM2->PSC); 
   TEST_ASSERT_EQUAL(300000,CYCLES_PER_ACCELERATION_TICK);
   int i = 0;
   //printf("limit = %d\n",limit);
   for(i = 0; i < limit ; i++){
    TEST_ASSERT_EQUAL(false,iterateCycleCounter());
   }
   TEST_ASSERT_EQUAL(true,iterateCycleCounter());
   TEST_ASSERT_EQUAL(37500,stExecutor.cyclePerStepCounter);
   
   for(i = 0; i < limit-1 ; i++){
    TEST_ASSERT_EQUAL(false,iterateCycleCounter());
   }
   TEST_ASSERT_EQUAL(true,iterateCycleCounter());
   TEST_ASSERT_EQUAL(18750,stExecutor.cyclePerStepCounter);
}

/*-------------acceleratreRate----------------*/

void xtest_acceleratreRate_when_currentRate_is_80000_after_the_function_in_several_loop_the_currentRate_will_be_increased_to_80500(void){
   bufferHead = nextBlockIndex(bufferHead);
   blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,80000,160000,80000);
   stExecutorInitProcess();
   int limit = CYCLES_PER_ACCELERATION_TICK/stExecutor.cyclePerStepEvent;
   int i;
   for(i = 0; i <= limit ; i++){
    accelerateRate();
   }
    TEST_ASSERT_EQUAL(6000,stExecutor.cyclePerStepCounter);
    TEST_ASSERT_EQUAL(80500,stExecutor.currentRate);
    TEST_ASSERT_EQUAL(53640,stExecutor.cyclePerStepEvent);
    TEST_ASSERT_EQUAL(53639,TIM2->ARR);
    TEST_ASSERT_EQUAL(0,TIM2->PSC); 
}

void test_acceleratreRate_when_currentRate_is_160000_the_currentRate_does_not_increase_any_more(void){
   bufferHead = nextBlockIndex(bufferHead);
   blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,160000,160000,80000);
   stExecutorInitProcess();
   int limit = CYCLES_PER_ACCELERATION_TICK/stExecutor.cyclePerStepEvent;
   int i;
   for(i = 0; i <= limit ; i++){
    accelerateRate();
   }
    TEST_ASSERT_EQUAL(160000,stExecutor.currentRate);
    TEST_ASSERT_EQUAL(27000,stExecutor.cyclePerStepEvent);
    TEST_ASSERT_EQUAL(26999,TIM2->ARR);
    TEST_ASSERT_EQUAL(0,TIM2->PSC); 
}

void test_accelerateRate_the_currentRate_will_increase_from_80000_to_160000_the_total_step_is(void){
   bufferHead = nextBlockIndex(bufferHead);
   blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,80000,160000,80000);
   stExecutorInitProcess();
   currentBlock->accelerateUntil = ceil(estimateAccelerationStep(currentBlock->initialRate,currentBlock->nominalRate, ACCELERATION*80));
   //printf("currentBlock->accelerateUntil = %d\n",currentBlock->accelerateUntil);
   int steps = 0;
   while(steps < currentBlock->accelerateUntil){
     accelerateRate();
     steps++;
     //printf("stExecutor.currentRate = %d\n",stExecutor.currentRate);
   }
   TEST_ASSERT_EQUAL(currentBlock->accelerateUntil,steps);
   TEST_ASSERT_EQUAL(160000,stExecutor.currentRate);
   TEST_ASSERT_EQUAL(27000,stExecutor.cyclePerStepEvent);
   TEST_ASSERT_EQUAL(26999,TIM2->ARR);
   TEST_ASSERT_EQUAL(0,TIM2->PSC); 
}

/*-------------nominalRate----------------*/

void test_nominalRate_if_currentRate_is_not_equal_nominalRate_the_currentRate_will_be_equal_to_nominalRate(void){
   bufferHead = nextBlockIndex(bufferHead);
   blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,80000,160000,80000);
   stExecutorInitProcess();
   nominalRate();
   TEST_ASSERT_EQUAL(160000,stExecutor.currentRate);
   TEST_ASSERT_EQUAL(27000,stExecutor.cyclePerStepEvent);
   TEST_ASSERT_EQUAL(26999,TIM2->ARR);
   TEST_ASSERT_EQUAL(0,TIM2->PSC); 
}

void test_nominalRate_if_currentRate_is_equal_nominalRate_the_currentRate_will_be_remain(void){
   bufferHead = nextBlockIndex(bufferHead);
   blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,160000,160000,80000);
   stExecutorInitProcess();
   nominalRate();
   TEST_ASSERT_EQUAL(160000,stExecutor.currentRate);
   TEST_ASSERT_EQUAL(27000,stExecutor.cyclePerStepEvent);
   TEST_ASSERT_EQUAL(26999,TIM2->ARR);
   TEST_ASSERT_EQUAL(0,TIM2->PSC); 
}


/*-------------decelerationAbjustment----------------*/

void test_decelerationAbjustment_if_currentRate_is_more_than_minSafeRate_the_currentRate_will_be_mimus_by_increment_resolution(void){
  bufferHead = nextBlockIndex(bufferHead);
  blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,80000,160000,70000);
  stExecutorInitProcess();
  int limit = CYCLES_PER_ACCELERATION_TICK/stExecutor.cyclePerStepEvent;
   int i;
  for(i = 0; i <= limit ; i++){
     deccelerationAbjustment();
  }
  TEST_ASSERT_EQUAL(79500,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(54300,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(54299,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC);  
}

void test_decelerationAbjustment_if_currentRate_is_less_than_minSafeRate_the_currentRate_will_be_divide_by_2(void){
  bufferHead = nextBlockIndex(bufferHead);
  blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,700,160000,80000);
  stExecutorInitProcess();
   int limit = CYCLES_PER_ACCELERATION_TICK/stExecutor.cyclePerStepEvent;
   int i;
   for(i = 0; i <= limit ; i++){
     deccelerationAbjustment();
   }
  
  TEST_ASSERT_EQUAL(80000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(0,TIM2->PSC);  
  TEST_ASSERT_EQUAL(54000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(53999,TIM2->ARR);
}

void test_decelerationAbjustment_if_currentRate_is_less_than_finalRate_the_currentRate_will_become_finalRate(void){
  bufferHead = nextBlockIndex(bufferHead);
  blockConfig(&blockBuffer[bufferTail],0x00,100,75,10,80000,160000,80000);
  stExecutorInitProcess();
  deccelerationAbjustment();
  TEST_ASSERT_EQUAL(80000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(54000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(53999,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC);  
}


/*-------------initializeDeccelerate----------------*/

void xtest_initializeDeccelerate_when_the_currentRate_is_equal_to_nominalRate_the_cyclePerStepCounter_will_be_set_to_0(void){
  bufferHead = nextBlockIndex(bufferHead);
  blockConfig(&blockBuffer[bufferTail],0x00,10000,8000,2000,160000,160000,80000);
  stExecutorInitProcess();
  currentBlock->accelerateUntil = ceil(estimateAccelerationStep(currentBlock->initialRate,currentBlock->nominalRate, ACCELERATION*80));
  currentBlock->deccelerateAfter = estimateDeccelerationStep();
  stExecutor.stepEventsCompleted = currentBlock->deccelerateAfter;
  initializeDecceleration();
  TEST_ASSERT_EQUAL(0,stExecutor.cyclePerStepCounter);
  
}

void test_initializeDeccelerate_when_the_currentRate_is_not_equal_to_nominalRate_the_cyclePerStepCounter_will_be_set_to_cyclesPerAccelerationTick_divide_by_2(void){
  bufferHead = nextBlockIndex(bufferHead);
  blockConfig(&blockBuffer[bufferTail],0x00,10000,8000,2000,80000,160000,80000);
  stExecutorInitProcess();
  currentBlock->accelerateUntil = ceil(estimateAccelerationStep(currentBlock->initialRate,currentBlock->nominalRate, ACCELERATION*80));
  currentBlock->deccelerateAfter = estimateDeccelerationStep();
  stExecutor.stepEventsCompleted = currentBlock->deccelerateAfter;
  stExecutor.cyclePerStepCounter = 54000;
  initializeDecceleration();
  TEST_ASSERT_EQUAL(CYCLES_PER_ACCELERATION_TICK-54000,stExecutor.cyclePerStepCounter);
}


/*-------------deccelerateRate----------------*/

void xtest_deccelerateRate_when_the_currentRate_is_equal_to_nominalRate_the_cyclePerStepCounter_will_be_set_to_0(void){
  bufferHead = nextBlockIndex(bufferHead);
  blockConfig(&blockBuffer[bufferTail],0x00,10000,8000,2000,160000,160000,80000);
  stExecutorInitProcess();
  currentBlock->accelerateUntil = ceil(estimateAccelerationStep(currentBlock->initialRate,currentBlock->nominalRate, ACCELERATION*80));
  currentBlock->deccelerateAfter = estimateDeccelerationStep();
  stExecutor.stepEventsCompleted = currentBlock->deccelerateAfter;TEST_ASSERT_EQUAL(0,stExecutor.cyclePerStepCounter);  
  deccelerateRate();
  TEST_ASSERT_EQUAL(0,stExecutor.cyclePerStepCounter);
  TEST_ASSERT_EQUAL(160000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(27000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(26999,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC);  
}

void test_deccelerateRate_when_the_currentRate_is_not_equal_to_nominalRate_the_cyclePerStepCounter_will_be_set_to_cyclesPerAccelerationTick_divide_by_2(void){
  bufferHead = nextBlockIndex(bufferHead);
  blockConfig(&blockBuffer[bufferTail],0x00,10000,8000,2000,80000,160000,80000);
  stExecutorInitProcess();
  currentBlock->accelerateUntil = ceil(estimateAccelerationStep(currentBlock->initialRate,currentBlock->nominalRate, ACCELERATION*80));
  currentBlock->deccelerateAfter = estimateDeccelerationStep();
  stExecutor.stepEventsCompleted = currentBlock->deccelerateAfter;
  stExecutor.cyclePerStepCounter = 54000;
  deccelerateRate();
  TEST_ASSERT_EQUAL(CYCLES_PER_ACCELERATION_TICK-54000,stExecutor.cyclePerStepCounter);
  TEST_ASSERT_EQUAL(80000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(54000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(53999,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC);  
}

/*-------------accelerateAndDeccelerateEvent----------------*/
void xtest_accelerateAndDeccelerateEvent_(void){
  
  bufferHead = nextBlockIndex(bufferHead);
  blockConfig(&blockBuffer[bufferTail],0x00,10000,8000,2000,80000,160000,80000);
  stExecutorInitProcess();
  //currentBlock->accelerateUntil = ceil(estimateAccelerationStep(currentBlock->initialRate,currentBlock->nominalRate, ACCELERATION*80));
  //currentBlock->deccelerateAfter = estimateDeccelerationStep();
  
  TEST_ASSERT_EQUAL(3334,currentBlock->accelerateUntil);
  TEST_ASSERT_EQUAL(6666,currentBlock->deccelerateAfter);
  TEST_ASSERT_EQUAL(80000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(54000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(53999,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  TEST_ASSERT_EQUAL(0,stExecutor.stepEventsCompleted);
  
  int numberOfCycleToIncreaseRateAtNorminalRate = CYCLES_PER_ACCELERATION_TICK/27000 + 1;
  int numberOfCycleToIncreaseRateAtInitRate = CYCLES_PER_ACCELERATION_TICK/54000 + 1;
  //printf("stExecutor.cyclePerStepEvent = %d\n",stExecutor.cyclePerStepEvent); 
  //printf("updateCurrentCycle = %d\n",updateCurrentCycle); 
  
  while(stExecutor.stepEventsCompleted < numberOfCycleToIncreaseRateAtInitRate ){
     stExecutor.stepEventsCompleted++;
     accelerateAndDeccelerateEvent();
  }
  TEST_ASSERT_EQUAL(80500,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(53640,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(53639,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  TEST_ASSERT_EQUAL(14,stExecutor.stepEventsCompleted);
  
  
  while(stExecutor.stepEventsCompleted < currentBlock->accelerateUntil){
     stExecutor.stepEventsCompleted++;
     accelerateAndDeccelerateEvent();
  }
  
  TEST_ASSERT_EQUAL(currentBlock->accelerateUntil,stExecutor.stepEventsCompleted);
  TEST_ASSERT_EQUAL(160000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(27000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(26999,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  TEST_ASSERT_EQUAL(3334,stExecutor.stepEventsCompleted);
  
  while(stExecutor.stepEventsCompleted <  (currentBlock->accelerateUntil+1000) ){
     stExecutor.stepEventsCompleted++;
     accelerateAndDeccelerateEvent();
  }
  
  TEST_ASSERT_EQUAL(1000+currentBlock->accelerateUntil,stExecutor.stepEventsCompleted);
  TEST_ASSERT_EQUAL(160000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(27000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(26999,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  
  while(stExecutor.stepEventsCompleted <  (currentBlock->deccelerateAfter) ){
     stExecutor.stepEventsCompleted++;
     accelerateAndDeccelerateEvent();
  }
  TEST_ASSERT_EQUAL(currentBlock->deccelerateAfter,stExecutor.stepEventsCompleted);
  TEST_ASSERT_EQUAL(160000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(27000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(26999,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  TEST_ASSERT_EQUAL(stExecutor.cyclePerStepCounter,0); 
  
  while(stExecutor.stepEventsCompleted <  (currentBlock->deccelerateAfter) + numberOfCycleToIncreaseRateAtNorminalRate ){
     stExecutor.stepEventsCompleted++;
     accelerateAndDeccelerateEvent();
  }
  
  TEST_ASSERT_EQUAL(159500,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(currentBlock->deccelerateAfter + numberOfCycleToIncreaseRateAtNorminalRate ,stExecutor.stepEventsCompleted);
  TEST_ASSERT_EQUAL(6000,stExecutor.cyclePerStepCounter);
  TEST_ASSERT_EQUAL(27060,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(27059,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 

  while(stExecutor.stepEventsCompleted <  currentBlock->stepEventCount - 1  ){
     stExecutor.stepEventsCompleted++;
     accelerateAndDeccelerateEvent();
  }
  
  
  TEST_ASSERT_EQUAL(80500,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(53640,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(53639,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  TEST_ASSERT_EQUAL(currentBlock->stepEventCount - 1,stExecutor.stepEventsCompleted);
  
  
}

/*-------------motorRateControlProcess----------------*/
void xtest_motorRateControlProcess_(void){
  
  bufferHead = nextBlockIndex(bufferHead);
  blockConfig(&blockBuffer[bufferTail],0x00,10000,8000,2000,80000,160000,80000);
  stExecutorInitProcess();

  
  TEST_ASSERT_EQUAL(3334,currentBlock->accelerateUntil);
  TEST_ASSERT_EQUAL(6666,currentBlock->deccelerateAfter);
  TEST_ASSERT_EQUAL(80000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(54000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(53999,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  TEST_ASSERT_EQUAL(0,stExecutor.stepEventsCompleted);
  
 int numberOfCycleToIncreaseRateAtInitRate = CYCLES_PER_ACCELERATION_TICK/((TIMER_FREQUENCY/currentBlock->initialRate)*60) + 1;
 int numberOfCycleToIncreaseRateAtNorminalRate = CYCLES_PER_ACCELERATION_TICK/((TIMER_FREQUENCY/currentBlock->nominalRate)*60) + 1;

  
  while(stExecutor.stepEventsCompleted < numberOfCycleToIncreaseRateAtInitRate ){
     stExecutor.stepEventsCompleted++;
     motorRateControlProcess();
  }
  TEST_ASSERT_EQUAL(80500,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(53640,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(53639,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  TEST_ASSERT_EQUAL(14,stExecutor.stepEventsCompleted);
  
  
  while(stExecutor.stepEventsCompleted < currentBlock->accelerateUntil){
     stExecutor.stepEventsCompleted++;
     motorRateControlProcess();
  }
  
  TEST_ASSERT_EQUAL(currentBlock->accelerateUntil,stExecutor.stepEventsCompleted);
  TEST_ASSERT_EQUAL(160000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(27000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(26999,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  TEST_ASSERT_EQUAL(3334,stExecutor.stepEventsCompleted);
  
  while(stExecutor.stepEventsCompleted <  (currentBlock->accelerateUntil+1000) ){
     stExecutor.stepEventsCompleted++;
     motorRateControlProcess();
  }
  
  TEST_ASSERT_EQUAL(1000+currentBlock->accelerateUntil,stExecutor.stepEventsCompleted);
  TEST_ASSERT_EQUAL(160000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(27000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(26999,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  
  while(stExecutor.stepEventsCompleted <  (currentBlock->deccelerateAfter) ){
     stExecutor.stepEventsCompleted++;
     motorRateControlProcess();
  }
  TEST_ASSERT_EQUAL(currentBlock->deccelerateAfter,stExecutor.stepEventsCompleted);
  TEST_ASSERT_EQUAL(160000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(27000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(26999,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  TEST_ASSERT_EQUAL(stExecutor.cyclePerStepCounter,0); 
  
  while(stExecutor.stepEventsCompleted <  (currentBlock->deccelerateAfter) + numberOfCycleToIncreaseRateAtNorminalRate ){
     stExecutor.stepEventsCompleted++;
     motorRateControlProcess();
  }
  
  TEST_ASSERT_EQUAL(159500,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(currentBlock->deccelerateAfter + numberOfCycleToIncreaseRateAtNorminalRate ,stExecutor.stepEventsCompleted);
  TEST_ASSERT_EQUAL(6000,stExecutor.cyclePerStepCounter);
  TEST_ASSERT_EQUAL(27060,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(27059,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 

  while(stExecutor.stepEventsCompleted <  currentBlock->stepEventCount - 1  ){
     stExecutor.stepEventsCompleted++;
     motorRateControlProcess();
  }
  
  TEST_ASSERT_EQUAL(80500,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(53640,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(53639,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  TEST_ASSERT_EQUAL(currentBlock->stepEventCount - 1,stExecutor.stepEventsCompleted);
  
  
  stExecutor.stepEventsCompleted++;
  motorRateControlProcess();
  
  TEST_ASSERT_EQUAL(stExecutor.eventCount,stExecutor.stepEventsCompleted);
  TEST_ASSERT_EQUAL(bufferHead,bufferTail);
  TEST_ASSERT_NULL(currentBlock);
}


/*-------------TIM2_IRQHandler----------------*/
void xtest_TIM2_IRQHandler_Add_a_block_into_blockBuffer_(void){
  
  bufferHead = nextBlockIndex(bufferHead);
  blockConfig(&blockBuffer[bufferTail],0x00,10000,8000,2000,80000,160000,80000);  
  TIM2_IRQHandler();
  TEST_ASSERT_EQUAL(3334,currentBlock->accelerateUntil);
  TEST_ASSERT_EQUAL(6666,currentBlock->deccelerateAfter);
  TEST_ASSERT_EQUAL(80000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(54000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(53999,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  TEST_ASSERT_EQUAL(1,stExecutor.stepEventsCompleted);
  
  int numberOfCycleToIncreaseRateAtNorminalRate = CYCLES_PER_ACCELERATION_TICK/((TIMER_FREQUENCY/currentBlock->nominalRate)*60) + 1;
  int numberOfCycleToIncreaseRateAtInitRate = CYCLES_PER_ACCELERATION_TICK/((TIMER_FREQUENCY/currentBlock->initialRate)*60) + 1;
 // printf("stExecutor.cyclePerStepEvent = %d\n",stExecutor.cyclePerStepEvent); 
 // printf("numberOfCycleToIncreaseRateAtInitRate = %d\n",numberOfCycleToIncreaseRateAtInitRate); 
 // printf("numberOfCycleToIncreaseRateAtNorminalRate = %d\n",numberOfCycleToIncreaseRateAtNorminalRate); 
  
  
  while(stExecutor.stepEventsCompleted < currentBlock->accelerateUntil){
     TIM2_IRQHandler();
  }
  
  TEST_ASSERT_EQUAL(currentBlock->accelerateUntil,stExecutor.stepEventsCompleted);
  TEST_ASSERT_EQUAL(160000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(27000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(26999,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  TEST_ASSERT_EQUAL(3334,stExecutor.stepEventsCompleted);
  TEST_ASSERT_EQUAL(3334, sys.position[X_AXIS]);
  
  while(stExecutor.stepEventsCompleted <  (currentBlock->accelerateUntil+1000) ){
     TIM2_IRQHandler();
  }
  
  TEST_ASSERT_EQUAL(1000+currentBlock->accelerateUntil,stExecutor.stepEventsCompleted);
  TEST_ASSERT_EQUAL(160000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(27000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(26999,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  
  while(stExecutor.stepEventsCompleted <  (currentBlock->deccelerateAfter) ){
     TIM2_IRQHandler();
  }
  TEST_ASSERT_EQUAL(currentBlock->deccelerateAfter,stExecutor.stepEventsCompleted);
  TEST_ASSERT_EQUAL(160000,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(27000,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(26999,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  TEST_ASSERT_EQUAL(stExecutor.cyclePerStepCounter,0); 
  


  while(stExecutor.stepEventsCompleted <  currentBlock->stepEventCount - 1  ){
     TIM2_IRQHandler();
  }
  
  TEST_ASSERT_EQUAL(80500,stExecutor.currentRate);
  TEST_ASSERT_EQUAL(53640,stExecutor.cyclePerStepEvent);
  TEST_ASSERT_EQUAL(53639,TIM2->ARR);
  TEST_ASSERT_EQUAL(0,TIM2->PSC); 
  TEST_ASSERT_EQUAL(currentBlock->stepEventCount - 1,stExecutor.stepEventsCompleted);
  
  TIM2_IRQHandler();
  
  TEST_ASSERT_EQUAL(stExecutor.eventCount,stExecutor.stepEventsCompleted);
  TEST_ASSERT_EQUAL(bufferHead,bufferTail);
  TEST_ASSERT_NULL(currentBlock);
}

