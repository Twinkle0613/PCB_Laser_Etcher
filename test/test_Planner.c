#include "unity.h"
#include "Planner.h"

void setUp(void)
{
  bufferTail = 0;
  planInit();
  memset(blockBuffer, 0, sizeof(blockBuffer));
}

void tearDown(void)
{
}


/*

     Index                           Index
      V                               V
     --------   nextBlockIndex   --------
    | 0 | 1 |..... ------>      | 0 | 1 |
    --------                    --------
*/
void test_nextBlockIndex_Index_point_to_next_block(void){
  int index = 0;
  index = nextBlockIndex(index);
  TEST_ASSERT_EQUAL(1,index);
}


/*
               Index (Max)             Index
                V                        V
         ----------   nextBlockIndex   --------
    ....| 17 | 18 |      ------>      | 0 | 1 |
        ----------                    --------
*/

void test_nextBlockIndex_Index_point_to_next_block_with_overflow(void){
  int index = 17;
  index = nextBlockIndex(index);
  TEST_ASSERT_EQUAL(0,index);
}

/*

         Index                    Index
           V                       V
     --------   prevBlockIndex   --------
    | 0 | 1 |..... ------>      | 0 | 1 |
    --------                    --------
*/
void test_prevBlockIndex_Index_point_to_previous_block(void){
    int index = 1;
    index = prevBlockIndex(index);
    TEST_ASSERT_EQUAL(0,index);
}

/*
         Index                             Index(Max)
           V                                    V
         --------   prevBlockIndex       ----------
        | 0 | 1 |      ------>      ....| 17 | 18 |
        --------                        ----------
*/
void test_prevBlockIndex_Index_point_to_previous_block_with_underflow(void){
  int index = 0;
  index = prevBlockIndex(index);
  TEST_ASSERT_EQUAL(17,index);
}

/*

     T H  H'
     V V  V
     ----------------------------
    | 0 | 1 | 2 | 3 | 4 | 5 | 6 |.....
    ----------------------------
*/

void test_planInit_initialize_the_bufferHead_bufferTail_and_nextBufferHead(void){
  planInit();
  TEST_ASSERT_EQUAL(bufferHead,bufferTail);
  TEST_ASSERT_EQUAL(nextBufferHead,1);
  TEST_ASSERT_EQUAL(0,plExecutor.position[0]);
  TEST_ASSERT_EQUAL(0,plExecutor.position[1]);
  TEST_ASSERT_EQUAL(0,plExecutor.position[2]);
  TEST_ASSERT_EQUAL(0,plExecutor.previousUnitVec[0]);
  TEST_ASSERT_EQUAL(0,plExecutor.previousUnitVec[1]);
  TEST_ASSERT_EQUAL(0,plExecutor.previousUnitVec[2]);
  TEST_ASSERT_EQUAL(0,plExecutor.previousNominalSpeed);
}


/*

      T             H           CT           H
      V            V            VV           V 
     ----------------          ----------------
    | 0 | 1 | 2 | 3 |   ----> | 0 | 1 | 2 | 3 | 
    ----------------          ----------------
*/
void test_getBlock_to_get_the_block_when_buffer_contained_block(void){
  bufferHead = nextBlockIndex(bufferHead);
  bufferHead = nextBlockIndex(bufferHead);
  bufferHead = nextBlockIndex(bufferHead);
  TEST_ASSERT_EQUAL(3,bufferHead);
  block_t* currentBlock = getCurrentBlock();
  TEST_ASSERT_EQUAL_PTR(currentBlock,&(blockBuffer[bufferTail]));  
}

/*

      TH                         C
      VV                         V
     ----------------          -------  
    | 0 | 1 | 2 | 3 |   ----> | NULL |
    ----------------          -------
*/
void test_getBlock_to_get_NULL_when_buffer_dont_have_any_block(void){
  block_t* currentBlock = getCurrentBlock();
  TEST_ASSERT_NULL(currentBlock);  
}


/*

      T             H                T       H
      V            V                 V       V 
     ----------------          ----------------
    | 0 | 1 | 2 | 3 |   ----> | 0 | 1 | 2 | 3 | 
    ----------------          ----------------
*/

void test_discardCurrrentBlock_if_blockBuffer_contain_block_the_buffetTail_will_be_increased(void){
  bufferHead = nextBlockIndex(bufferHead);
  bufferHead = nextBlockIndex(bufferHead);
  bufferHead = nextBlockIndex(bufferHead);
  discardCurrentBlock();
  TEST_ASSERT_EQUAL(1,bufferTail);
}

/*

      TH                        TH
      VV                        VV 
     ----------------          ----------------
    | 0 | 1 | 2 | 3 |   ----> | 0 | 1 | 2 | 3 | 
    ----------------          ----------------
*/

void test_discardCurrrentBlock_if_blockBuffer_doesnt_contain_block_the_buffetTail_will_remain(void){
  discardCurrentBlock();
  TEST_ASSERT_EQUAL(0,bufferTail);
}

void xtest_blockConfig_StepX_is_100_StepY_is_75_StepZ_is_10_the_StepX_is_largest(void){
   bufferHead = nextBlockIndex(bufferHead);
   blockConfig(&blockBuffer[bufferTail],0x00,10000,75,10,80000,160000,80000);
   TEST_ASSERT_EQUAL(10000,blockBuffer[bufferTail].stepEventCount);
   TEST_ASSERT_EQUAL(0x00,blockBuffer[bufferTail].directionBits);
   TEST_ASSERT_EQUAL(10000,blockBuffer[bufferTail].steps[X_AXIS]);
   TEST_ASSERT_EQUAL(75,blockBuffer[bufferTail].steps[Y_AXIS]);
   TEST_ASSERT_EQUAL(10,blockBuffer[bufferTail].steps[Z_AXIS]);
   TEST_ASSERT_EQUAL(80000,blockBuffer[bufferTail].initialRate);
   TEST_ASSERT_EQUAL(160000,blockBuffer[bufferTail].nominalRate);
   TEST_ASSERT_EQUAL(80000,blockBuffer[bufferTail].finalRate);
   TEST_ASSERT_EQUAL(3334,blockBuffer[bufferTail].accelerateUntil);
   TEST_ASSERT_EQUAL(6666,blockBuffer[bufferTail].deccelerateAfter);
}

void xtest_blockConfig_StepX_is_100_StepY_is_300_StepZ_is_200_StepY_is_largest(void){
   bufferHead = nextBlockIndex(bufferHead);
   blockConfig(&blockBuffer[bufferTail],0x00,100,300,200,6000,8000,6000);
   TEST_ASSERT_EQUAL(300,blockBuffer[bufferTail].stepEventCount);
   TEST_ASSERT_EQUAL(0x00,blockBuffer[bufferTail].directionBits);
   TEST_ASSERT_EQUAL(100,blockBuffer[bufferTail].steps[X_AXIS]);
   TEST_ASSERT_EQUAL(300,blockBuffer[bufferTail].steps[Y_AXIS]);
   TEST_ASSERT_EQUAL(200,blockBuffer[bufferTail].steps[Z_AXIS]);
   TEST_ASSERT_EQUAL(6000,blockBuffer[bufferTail].initialRate);
   TEST_ASSERT_EQUAL(6000,blockBuffer[bufferTail].finalRate);
}

void xtest_blockConfig_StepX_is_100_StepY_is_300_StepZ_is_1000_StepZ_is_largest(void){
   bufferHead = nextBlockIndex(bufferHead);
   blockConfig(&blockBuffer[bufferTail],0x00,100,300,1000,7000,9000,7000);
   TEST_ASSERT_EQUAL(1000,blockBuffer[bufferTail].stepEventCount);
   TEST_ASSERT_EQUAL(0x00,blockBuffer[bufferTail].directionBits);
   TEST_ASSERT_EQUAL(100,blockBuffer[bufferTail].steps[X_AXIS]);
   TEST_ASSERT_EQUAL(300,blockBuffer[bufferTail].steps[Y_AXIS]);
   TEST_ASSERT_EQUAL(1000,blockBuffer[bufferTail].steps[Z_AXIS]);
   TEST_ASSERT_EQUAL(7000,blockBuffer[bufferTail].initialRate);
   TEST_ASSERT_EQUAL(9000,blockBuffer[bufferTail].nominalRate);
}

void test_ceil_put_the_value_from_3point0_to_3point9_into_the_function_(void){
  TEST_ASSERT_EQUAL(3,ceil(3.0));
  TEST_ASSERT_EQUAL(4,ceil(3.1));
  TEST_ASSERT_EQUAL(4,ceil(3.2));
  TEST_ASSERT_EQUAL(4,ceil(3.3));
  TEST_ASSERT_EQUAL(4,ceil(3.4));
  TEST_ASSERT_EQUAL(4,ceil(3.5));
  TEST_ASSERT_EQUAL(4,ceil(3.6));
  TEST_ASSERT_EQUAL(4,ceil(3.7));
  TEST_ASSERT_EQUAL(4,ceil(3.8));
  TEST_ASSERT_EQUAL(4,ceil(3.9));
  TEST_ASSERT_EQUAL(4,ceil(4.0));
}


/*
   
                   |
                   |   ---------------------------  <---160000 step/min
                   |  /|                        | \
                   | / |                        |  \
80000 step/min---> |/  |                        |   \  <--- 80000 step/min
                   |   |                        |   |    
                   |   |                        |   |  
                   ----------------------------------------->
                       ^                        ^
                      3334                    6667
*/
void xtest_estimateAccelStepAndDeccelStep_when_the_initRate_is_80000_nominalRate_is_160000_finalRate_is_80000(void){
  block_t block;
  block.stepEventCount = 10000;
  block.initialRate = 80000;
  block.nominalRate = 160000;
  block.finalRate = 80000;
  estimateAccelStepAndDeccelStep(&block);
  TEST_ASSERT_EQUAL(3334,block.accelerateUntil);
  TEST_ASSERT_EQUAL(6667,block.deccelerateAfter);
}

/*
   
                   |
                   |   ---------------------------  <---160000 step/min
                   |  /|                        | \
                   | / |                        |  \
80000 step/min---> |/  |                        |   \  <--- 800 step/min
                   |   |                        |   |    
                   |   |                        |   |  
                   ----------------------------------------->
                       ^                        ^
                      3334                    6667
*/
void xtest_estimateAccelStepAndDeccelStep_when_the_initRate_is_80000_nominalRate_is_200000_finalRate_is_800(void){
  block_t block;
  block.stepEventCount = 15000;
  block.initialRate = 80000;
  block.nominalRate = 200000;
  block.finalRate = 800;  
  estimateAccelStepAndDeccelStep(&block);
  TEST_ASSERT_EQUAL(5834,block.accelerateUntil);
  TEST_ASSERT_EQUAL(8056,block.deccelerateAfter);
}

/*
   
                   |
                   |   ---------------------------  <---160000 step/min
                   |  /|                        | \
                   | / |                        |  \
  800 step/min---> |/  |                        |   \  <--- 800 step/min
                   |   |                        |   |    
                   |   |                        |   |  
                   ----------------------------------------->
                       ^                        ^
                      3334                    6667
*/
void xtest_estimateAccelStepAndDeccelStep_when_the_initRate_is_800_nominalRate_is_200000_finalRate_is_800(void){
  block_t block;
  block.stepEventCount = 15000;
  block.initialRate = 800;
  block.nominalRate = 200000;
  block.finalRate = 800;  
  estimateAccelStepAndDeccelStep(&block);
  TEST_ASSERT_EQUAL(6945,block.accelerateUntil);
  TEST_ASSERT_EQUAL(8056,block.deccelerateAfter);
}

/*
   
                   |
                   |   ---------------------------  <---1000 step/min
                   |  /|                        | \
                   | / |                        |  \
  800 step/min---> |/  |                        |   \  <--- 800 step/min
                   |   |                        |   |    
                   |   |                        |   |  
                   ----------------------------------------->
                       ^                        ^
                      3334                    6667
*/ 

void xtest_estimateAccelStepAndDeccelStep_when_the_initRate_is_800_nominalRate_is_1000_finalRate_is_800(void){
  block_t block;
  block.stepEventCount = 15000;
  block.initialRate = 800;
  block.nominalRate = 1000;
  block.finalRate = 800;  
  estimateAccelStepAndDeccelStep(&block);
  TEST_ASSERT_EQUAL(1,block.accelerateUntil);
  TEST_ASSERT_EQUAL(15000,block.deccelerateAfter);
}

/*
   
                   |
                   |    ---1000 step/min
                   |  /\                       | \
                   | /  |                        |  \
  800 step/min---> |/                            |   \  <--- 800 step/min
                   |   |                         |   |    
                   |   |                        |   |  
                   ----------------------------------------->
                       ^                        ^
                      3334                    6667
*/ 

void xtest_estimateAccelStepAndDeccelStep_when_the_initRate_is_80000_nominalRate_is_200000_finalRate_is_80000(void){
  block_t block;
  block.stepEventCount = 10000;
  block.initialRate = 80000;
  block.nominalRate = 200000;
  block.finalRate = 80000;  
  estimateAccelStepAndDeccelStep(&block);
  TEST_ASSERT_EQUAL(6945,block.accelerateUntil);
  TEST_ASSERT_EQUAL(8056,block.deccelerateAfter);
}

void test_convertXYZMovingDistanceToStepAndStoreToArray_when_x_is_100mm_y_is_200mm_and_z_is_300mm_the_function_will_return_x_is_20000_step_y_is_40000_step_x_is_60000_step(void){
  int32_t target[3];
  convertXYZMovingDistanceToStepAndStoreToArray(100,200,300,target);
  TEST_ASSERT_EQUAL(20000,target[X_AXIS]); 
  TEST_ASSERT_EQUAL(40000,target[Y_AXIS]); 
  TEST_ASSERT_EQUAL(60000,target[Z_AXIS]); 
}

void test_convertXYZMovingDistanceToStepAndStoreToArray_when_x_is_0mm_y_is_0mm_and_z_is_300mm_the_function_will_return_x_is_0_step_y_is_0_step_x_is_60000_step(void){
  int32_t target[3];
  convertXYZMovingDistanceToStepAndStoreToArray(0,0,300,target);
  TEST_ASSERT_EQUAL(0,target[X_AXIS]); 
  TEST_ASSERT_EQUAL(0,target[Y_AXIS]); 
  TEST_ASSERT_EQUAL(60000,target[Z_AXIS]); 
}

void test_determineDirection_when_target_x_less_than_prev_x_target_the_direactionBits_is_equal_0x20(void){
  block_t *block = &blockBuffer[bufferTail];
  plExecutor.position[X_AXIS] = 100000;
  plExecutor.position[Y_AXIS] = 0;
  plExecutor.position[Z_AXIS] = 0;
  int32_t target[3];
  convertXYZMovingDistanceToStepAndStoreToArray(100,200,300,target);
  block->directionBits |= determineDirection(target);
  TEST_ASSERT_EQUAL(0x20,block->directionBits);
}

void test_determineDirection_when_target_y_less_than_prev_y_target_the_direactionBits_is_equal_0x40(void){
  block_t *block = &blockBuffer[bufferTail];
  plExecutor.position[X_AXIS] = 0;
  plExecutor.position[Y_AXIS] = 100000;
  plExecutor.position[Z_AXIS] = 0;
  int32_t target[3];
  convertXYZMovingDistanceToStepAndStoreToArray(100,200,300,target);
  block->directionBits |= determineDirection(target);
  TEST_ASSERT_EQUAL(0x40,block->directionBits);
}

void test_determineDirection_when_target_z_less_than_prev_z_target_the_direactionBits_is_equal_0x80(void){
  block_t *block = &blockBuffer[bufferTail];
  plExecutor.position[X_AXIS] = 0;
  plExecutor.position[Y_AXIS] = 0;
  plExecutor.position[Z_AXIS] = 100000;
  int32_t target[3];
  convertXYZMovingDistanceToStepAndStoreToArray(100,200,300,target);
  block->directionBits |= determineDirection(target);
  TEST_ASSERT_EQUAL(0x80,block->directionBits);
}

void test_determineDirection_when_target_xyz_less_than_prev_xyz_target_the_direactionBits_is_equal_0x80(void){
  block_t *block = &blockBuffer[bufferTail];
  plExecutor.position[X_AXIS] = 100000;
  plExecutor.position[Y_AXIS] = 100000;
  plExecutor.position[Z_AXIS] = 100000;
  int32_t target[3];
  convertXYZMovingDistanceToStepAndStoreToArray(100,200,300,target);
  block->directionBits |= determineDirection(target);
  TEST_ASSERT_EQUAL(0xe0,block->directionBits);
}

void test_labs_put_a_negative_value_into_function_it_should_the_absolute_value_(void){
  TEST_ASSERT_EQUAL_INT32(1000L,_labs(1000L));
  TEST_ASSERT_EQUAL_INT32(1000L,_labs(-1000L));
  TEST_ASSERT_EQUAL_INT32(0,_labs(0));
  TEST_ASSERT_EQUAL_INT32(9022233,_labs(-9022233));
  TEST_ASSERT_EQUAL_INT32(9022233,_labs(9022233));
  TEST_ASSERT_EQUAL_INT32(80000,_labs(-80000));
}

void test_calculateXYZaxisSteps_to_obtain_the_moving_step_when_the_target_x_is_100_targer_y_is_200_and_target_z_is_300(void){
  block_t *block = &blockBuffer[bufferHead];
  plExecutor.position[X_AXIS] = 0;
  plExecutor.position[Y_AXIS] = 0;
  plExecutor.position[Z_AXIS] = 0;
  int32_t target[3];
  convertXYZMovingDistanceToStepAndStoreToArray(100,200,300,target);
  calculateXYZaxisSteps(block,target);
  TEST_ASSERT_EQUAL(block->steps[X_AXIS],20000);
  TEST_ASSERT_EQUAL(block->steps[Y_AXIS],40000);
  TEST_ASSERT_EQUAL(block->steps[Z_AXIS],60000);
}

void test_calculateXYZaxisSteps_to_obtain_the_moving_step_when_the_target_x_is_100_targer_y_is_200_and_target_z_is_300_the_prev_target_xyz_are_100000(void){
  block_t *block = &blockBuffer[bufferHead];
  plExecutor.position[X_AXIS] = 100000;
  plExecutor.position[Y_AXIS] = 100000;
  plExecutor.position[Z_AXIS] = 100000;
  int32_t target[3];
  convertXYZMovingDistanceToStepAndStoreToArray(100,200,300,target);
  calculateXYZaxisSteps(block,target);
  TEST_ASSERT_EQUAL_INT32(block->steps[X_AXIS],80000);
  TEST_ASSERT_EQUAL_INT32(block->steps[Y_AXIS],60000);
  TEST_ASSERT_EQUAL_INT32(block->steps[Z_AXIS],40000);
}


void test_getHighestStepsInXYZsteps_when_the_X_steps_is_20000_Y_steps_is_40000_and_Z_steps_is_60000_the_max_steps_is_60000(void){
  block_t *block = &blockBuffer[bufferHead];
  int32_t target[3];
  convertXYZMovingDistanceToStepAndStoreToArray(100,200,300,target);
  calculateXYZaxisSteps(block,target);
  TEST_ASSERT_EQUAL(60000,getHighestStepsInXYZsteps(block));  
}

void test_getHighestStepsInXYZsteps_when_the_X_steps_is_20000_Y_steps_is_100000_and_Z_steps_is_60000_the_max_steps_is_100000(void){
  block_t *block = &blockBuffer[bufferHead];
  int32_t target[3];
  convertXYZMovingDistanceToStepAndStoreToArray(100,500,300,target);
  calculateXYZaxisSteps(block,target);
  TEST_ASSERT_EQUAL(100000,getHighestStepsInXYZsteps(block));  
}

void test_getHighestStepsInXYZsteps_when_the_X_steps_is_120000_Y_steps_is_100000_and_Z_steps_is_60000_the_max_steps_is_120000(void){
  block_t *block = &blockBuffer[bufferHead];
  int32_t target[3];
  convertXYZMovingDistanceToStepAndStoreToArray(600,500,300,target);
  calculateXYZaxisSteps(block,target);
  TEST_ASSERT_EQUAL(120000,getHighestStepsInXYZsteps(block));  
}


void test_planLineBlock_initially_plan_the_motor_move_in_x_is_100mm_y_is_200mm_and_z_is_300mm_with_3000_mm_per_min(void){
  TEST_ASSERT_EQUAL(0,bufferTail);
  TEST_ASSERT_EQUAL(0,bufferHead);
  TEST_ASSERT_EQUAL(1,nextBufferHead);
  planLineBlock(100,200,300,3000,0);
  TEST_ASSERT_EQUAL(0,bufferTail);
  TEST_ASSERT_EQUAL(1,bufferHead);
  TEST_ASSERT_EQUAL(2,nextBufferHead);
  block_t *block = &blockBuffer[bufferTail];
  TEST_ASSERT_EQUAL(0,block->directionBits); 
  TEST_ASSERT_EQUAL(20000,block->steps[X_AXIS]); 
  TEST_ASSERT_EQUAL(40000,block->steps[Y_AXIS]); 
  TEST_ASSERT_EQUAL(60000,block->steps[Z_AXIS]); 
  TEST_ASSERT_EQUAL(60000,block->stepEventCount);
}

void test_getDeltaInMilliMeter_put_several_input_to_test(void){
  TEST_ASSERT_EQUAL_FLOAT(100,getDeltaInMilliMeter(20000,0));
  TEST_ASSERT_EQUAL_FLOAT(10,getDeltaInMilliMeter(2000,0));
  TEST_ASSERT_EQUAL_FLOAT(-400,getDeltaInMilliMeter(20000,100000));
  TEST_ASSERT_EQUAL_FLOAT(1,getDeltaInMilliMeter(200,0));
  TEST_ASSERT_EQUAL_FLOAT(-0.1,getDeltaInMilliMeter(20,40));
  TEST_ASSERT_EQUAL_FLOAT(0.1,getDeltaInMilliMeter(20,0));
}


void test_calculateXYZDeltaInMilliMeterAndStoreToArray_targetXYZ_are_20000steps_40000steps_and_60000steps_and_the_prevTargetXYZ_are_0_0_and_0(void){
  float delta[3];
  int32_t target[3];
  plExecutor.position[X_AXIS] = 0;
  plExecutor.position[Y_AXIS] = 0;
  plExecutor.position[Z_AXIS] = 0;
  target[X_AXIS] = 20000;
  target[Y_AXIS] = 40000;
  target[Z_AXIS] = 60000;
  calculateXYZDeltaInMilliMeterAndStoreToArray(target,delta);
  TEST_ASSERT_EQUAL_FLOAT(delta[X_AXIS],100);
  TEST_ASSERT_EQUAL_FLOAT(delta[Y_AXIS],200);
  TEST_ASSERT_EQUAL_FLOAT(delta[Z_AXIS],300);
}

void test_calculateXYZDeltaInMilliMeterAndStoreToArray_targetXYZ_are_20000steps_40000steps_and_60000steps_and_the_prevTargetXYZ_are_100000_100000_and_100000(void){
  float delta[3];
  int32_t target[3];
  plExecutor.position[X_AXIS] = 100000;
  plExecutor.position[Y_AXIS] = 100000;
  plExecutor.position[Z_AXIS] = 100000;
  target[X_AXIS] = 20000;
  target[Y_AXIS] = 40000;
  target[Z_AXIS] = 60000;
  calculateXYZDeltaInMilliMeterAndStoreToArray(target,delta);
  TEST_ASSERT_EQUAL_FLOAT(delta[X_AXIS],-400);
  TEST_ASSERT_EQUAL_FLOAT(delta[Y_AXIS],-300);
  TEST_ASSERT_EQUAL_FLOAT(delta[Z_AXIS],-200);
}

void test_getVectorLength_when_deltaX_is_100_deltaY_is_200_and_deltaZ_is_300_the_function_should_return_374(void){
  float delta[3];
  delta[X_AXIS] = 100;
  delta[Y_AXIS] = 200;
  delta[Z_AXIS] = 300;
  TEST_ASSERT_EQUAL_FLOAT(374,(int32_t)getVectorLength(delta));
}

void test_getVectorLength_when_deltaX_is_0_deltaY_is_400_and_deltaZ_is_300_the_function_should_return_500(void){
  float delta[3];
  delta[X_AXIS] = 0;
  delta[Y_AXIS] = 400;
  delta[Z_AXIS] = 300;
  TEST_ASSERT_EQUAL_FLOAT(500,(int32_t)getVectorLength(delta));
}

void test_getInverse_(void){
  block_t *block = &blockBuffer[bufferHead];
  float delta[3];
  delta[X_AXIS] = 0;
  delta[Y_AXIS] = 400;
  delta[Z_AXIS] = 300;
  block->millimeters = getVectorLength(delta);
  block->inverseMillimeters = getInverse(block->millimeters);
  TEST_ASSERT_EQUAL_FLOAT(0.002,block->inverseMillimeters);
  
}


/*

             
                          |            /
        maxStep           |          /
    400mm => 80000steps   |        /
                          |      /  vectorlength = 500 mm 
                          |    /
                          |  /
                          |/
                          ---------------
                          300mm => 60000steps

      nominalSpeed = feedrate
      nominalRate = MaxStep/distance * feedrate
                = 80000/500 * 100
                = 16000 steps/min


*/
void test_estimateNominalRateAndSpeed_when_the_vectorLenght_is_500mm_and_MaxStep_is_80000step(void){
  block_t *block = &blockBuffer[bufferHead];
  int32_t target[3];
  float delta[3];
  target[X_AXIS] = 80000;
  target[Y_AXIS] = 60000;
  target[Z_AXIS] = 0;
  calculateXYZaxisSteps(block,target);
  block->stepEventCount = getHighestStepsInXYZsteps(block);
  calculateXYZDeltaInMilliMeterAndStoreToArray(target,delta);
  block->millimeters = getVectorLength(delta);
  block->inverseMillimeters = getInverse(block->millimeters);
  estimateNominalRateAndSpeed(block,100,0);
  TEST_ASSERT_EQUAL(16000,block->nominalRate);
  TEST_ASSERT_EQUAL(100,block->nominalSpeed); 
}

/*

vectorlength = 360.555 mm ^
                   \      |            
                    \     |          maxStep 
                     \    |    300mm => 60000steps
                      \   |        
                       \  |    
                        \ |  
                         \|
          <----------------
 -200mm => -40000steps    
                        
                        
                               
                      
      nominalSpeed = feedrate
      nominalRate = MaxStep/distance * feedrate
                = 80000/500 * 100
                = 16000 steps/min


*/
void test_estimateNominalRateAndSpeed_when_the_vectorLenght_is_509_point_902mm_and_MaxStep_is_80000step(void){
  block_t *block = &blockBuffer[bufferHead];
  int32_t target[3];
  float delta[3];
  plExecutor.position[X_AXIS] = 100000;
  plExecutor.position[Y_AXIS] = 20000;
  plExecutor.position[Z_AXIS] = 20000;
  target[X_AXIS] = 60000;
  target[Y_AXIS] = 80000;
  target[Z_AXIS] = 20000;
  calculateXYZaxisSteps(block,target);
  block->stepEventCount = getHighestStepsInXYZsteps(block);
  calculateXYZDeltaInMilliMeterAndStoreToArray(target,delta);
  block->millimeters = getVectorLength(delta);
  block->inverseMillimeters = getInverse(block->millimeters);
  estimateNominalRateAndSpeed(block,100,0);
  TEST_ASSERT_EQUAL_FLOAT(360.555,block->millimeters);
  TEST_ASSERT_FLOAT_WITHIN(0.00002,0.0027735,block->inverseMillimeters);
  TEST_ASSERT_EQUAL(40000,block->steps[X_AXIS]); 
  TEST_ASSERT_EQUAL(60000,block->steps[Y_AXIS]); 
  TEST_ASSERT_EQUAL(0,block->steps[Z_AXIS]); 
  TEST_ASSERT_EQUAL(60000,block->stepEventCount);
  TEST_ASSERT_EQUAL_FLOAT(delta[X_AXIS],-200);
  TEST_ASSERT_EQUAL_FLOAT(delta[Y_AXIS],300);
  TEST_ASSERT_EQUAL_FLOAT(delta[Z_AXIS],0);
  TEST_ASSERT_EQUAL(16642,block->nominalRate);
  TEST_ASSERT_EQUAL(100,block->nominalSpeed); 
}

void test_planLineBlock_move_in_the_x_axis_is_300mm_the_y_axis_is_200mm_and_z_is_100mm_(void){
  block_t *block = &blockBuffer[bufferHead];
  plExecutor.position[X_AXIS] = 100000;
  plExecutor.position[Y_AXIS] = 20000;
  plExecutor.position[Z_AXIS] = 20000;
  planLineBlock(300,400,100,100,0);
  TEST_ASSERT_EQUAL(0x20,block->directionBits);
  TEST_ASSERT_EQUAL_FLOAT(360.555,block->millimeters);
  TEST_ASSERT_FLOAT_WITHIN(0.00002,0.0027735,block->inverseMillimeters);
  TEST_ASSERT_EQUAL(40000,block->steps[X_AXIS]); 
  TEST_ASSERT_EQUAL(60000,block->steps[Y_AXIS]); 
  TEST_ASSERT_EQUAL(0,block->steps[Z_AXIS]); 
  TEST_ASSERT_EQUAL(60000,block->stepEventCount);
  TEST_ASSERT_EQUAL(16642,block->nominalRate);
  TEST_ASSERT_EQUAL(100,block->nominalSpeed); 
  TEST_ASSERT_EQUAL(0,bufferTail);
  TEST_ASSERT_EQUAL(1,bufferHead);
  TEST_ASSERT_EQUAL(2,nextBufferHead);
}

void test_getUnitVector_the_deltaX_is_300_deltaY_is_400_and_deltaZ_is_100_and_vectorLength_is_509pt109(void){
  int32_t target[3];
  float delta[3];
  plExecutor.position[X_AXIS] = 0;
  plExecutor.position[Y_AXIS] = 0;
  plExecutor.position[Z_AXIS] = 0;
  target[X_AXIS] = 60000; //300mm
  target[Y_AXIS] = 80000; //400mm
  target[Z_AXIS] = 20000; //100mm
  calculateXYZDeltaInMilliMeterAndStoreToArray(target,delta);
  block_t *block = &blockBuffer[bufferHead];
  block->millimeters = getVectorLength(delta);
  block->inverseMillimeters = getInverse(block->millimeters);
  TEST_ASSERT_EQUAL_FLOAT(delta[X_AXIS],300);
  TEST_ASSERT_EQUAL_FLOAT(delta[Y_AXIS],400);
  TEST_ASSERT_EQUAL_FLOAT(delta[Z_AXIS],100);
  TEST_ASSERT_FLOAT_WITHIN(0.00002,0.0019611,block->inverseMillimeters);
  TEST_ASSERT_EQUAL_FLOAT(0.58835,getUnitVector(block,delta,X_AXIS));
  TEST_ASSERT_EQUAL_FLOAT(0.78446,getUnitVector(block,delta,Y_AXIS));
  TEST_ASSERT_EQUAL_FLOAT(0.196116,getUnitVector(block,delta,Z_AXIS));
  
}

void test_calculateUnitVectorForXYZaxisAndStoreToArray_the_deltaX_is_300_deltaY_is_400_and_deltaZ_is_100_and_vectorLength_is_509pt109(void){
  int32_t target[3];
  float delta[3];
  float unitVector[3];
  plExecutor.position[X_AXIS] = 0;
  plExecutor.position[Y_AXIS] = 0;
  plExecutor.position[Z_AXIS] = 0;
  target[X_AXIS] = 60000; //300mm
  target[Y_AXIS] = 80000; //400mm
  target[Z_AXIS] = 20000; //100mm
  calculateXYZDeltaInMilliMeterAndStoreToArray(target,delta);
  block_t *block = &blockBuffer[bufferHead];
  block->millimeters = getVectorLength(delta);
  block->inverseMillimeters = getInverse(block->millimeters);
  calculateUnitVectorForXYZaxisAndStoreToArray(block,delta,unitVector);
  TEST_ASSERT_FLOAT_WITHIN(0.00002,0.0019611,block->inverseMillimeters);
  TEST_ASSERT_EQUAL_FLOAT(0.58835,unitVector[X_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0.78446,unitVector[Y_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0.196116,unitVector[Z_AXIS]); 
}
