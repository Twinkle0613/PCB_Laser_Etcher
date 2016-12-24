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

/*

       X-axis                    Y-axis                  Z-axis
       +-------------->+         +--------------->+      +---------->+
       0              300mm      0             400mm     0           100mm
         deltaX = 300               deltaY = 400mm          deltaZ = 100mm
        
       
       lengthVector = deltaX^2 + deltaY^2 + deltaZ^2
       
       unitVectorX = deltaX/lengthVector;
       unitVectorY = deltaY/lengthVector;
       unitVectorZ = deltaZ/lengthVector;
     
*/
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

void test_planLineBlock_put_Moving_X_axis_is_300_Y_axis_is_400_and_Z_axis_is_100_(void){
  block_t *block = &blockBuffer[bufferHead];
  planLineBlock(300,400,100,100,0);
  TEST_ASSERT_EQUAL(0x00,block->directionBits);
  TEST_ASSERT_EQUAL_FLOAT(509.902,block->millimeters);
  TEST_ASSERT_FLOAT_WITHIN(0.00002,0.0019611,block->inverseMillimeters);
  TEST_ASSERT_EQUAL(60000,block->steps[X_AXIS]); 
  TEST_ASSERT_EQUAL(80000,block->steps[Y_AXIS]); 
  TEST_ASSERT_EQUAL(20000,block->steps[Z_AXIS]); 
  TEST_ASSERT_EQUAL(80000,block->stepEventCount);
  TEST_ASSERT_EQUAL(15690,block->nominalRate);
  TEST_ASSERT_EQUAL(100,block->nominalSpeed); 
  TEST_ASSERT_EQUAL(0,bufferTail);
  TEST_ASSERT_EQUAL(1,bufferHead);
  TEST_ASSERT_EQUAL(2,nextBufferHead);
  TEST_ASSERT_EQUAL_FLOAT(0.58835,plExecutor.previousUnitVec[X_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0.78446,plExecutor.previousUnitVec[Y_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0.196116,plExecutor.previousUnitVec[Z_AXIS]);
  TEST_ASSERT_EQUAL(60000,plExecutor.position[X_AXIS]); 
  TEST_ASSERT_EQUAL(80000,plExecutor.position[Y_AXIS]); 
  TEST_ASSERT_EQUAL(20000,plExecutor.position[Z_AXIS]);
  TEST_ASSERT_EQUAL(100,plExecutor.previousNominalSpeed);
}




   
/*
          acceleration Step                decceleration Step
                   |                            |
                   |   - <---160000 step/min    |-  <---160000 step/min
                   |  /|                        | \
                   | / |                        |  \
80000 step/min---> |/  |                        |   \  <--- 80000 step/min
                   |   |                        |   |    
                   |   |                        |   |  
                   --------------->             ------------>
                       ^                            ^
                      250                          250
*/
      
void test_getRampStep_initialRate_is_80000_and_targeRate_is_160000_and_acceleration_is_36000mm_per_min_square(void){
  
  block_t *block = &blockBuffer[bufferHead];
  float accelStep = _ceil(getRampStep(80000,100000,ACCELERATION*STEP_PER_MM));
  float deccelStep = _ceil(getRampStep(100000,80000,-ACCELERATION*STEP_PER_MM));
  TEST_ASSERT_EQUAL(250,accelStep);
  TEST_ASSERT_EQUAL(250,deccelStep);
}

void test_getNominalStep_the_function_is_tested_by_putting_some_value(void){
  TEST_ASSERT_EQUAL(5000,getNominalStep(8000,1000,2000));
  TEST_ASSERT_EQUAL(2000,getNominalStep(8000,1000,5000));
  TEST_ASSERT_EQUAL(0,getNominalStep(8000,3000,5000));
  TEST_ASSERT_EQUAL(-3000,getNominalStep(8000,3000,8000));
}

/*       
   Condition: the FinalRate is equal the InitialRate
                          + <- some maximum rate we don't care about
                         /|\
                        / | \                    
       initial_rate -> +  |  + <- final_rate     
                       |  |  |                   
                       +--+--+                   
                          ^  ^                   
                          |  |                   
      intersection_distance  distance  
*/

void test_getIntersectionDistance_put_initialRate_and_finalRate_is_equal_80000_and_distance_is_change(void){
  block_t *block = &blockBuffer[bufferHead];
  block->initialRate = 80000;
  block->finalRate = 80000;
  block->stepEventCount = 300;
  //printf("intersectionDistance = %f\n",getIntersectionDistance(block));
  TEST_ASSERT_EQUAL_FLOAT(150,_ceil(getIntersectionDistance(block)));
  block->stepEventCount = 500;
  TEST_ASSERT_EQUAL_FLOAT(250,_ceil(getIntersectionDistance(block)));
  block->stepEventCount = 200;
  TEST_ASSERT_EQUAL_FLOAT(100,_ceil(getIntersectionDistance(block)));
  block->stepEventCount = 50;
  TEST_ASSERT_EQUAL_FLOAT(25,_ceil(getIntersectionDistance(block)));
  block->stepEventCount = 15;
  TEST_ASSERT_EQUAL_FLOAT(8,_ceil(getIntersectionDistance(block)));
  // printf("value = %f\n",ceil(getIntersectionDistance(80000,80000,ACCELERATION*STEP_PER_MM,-10)));
  // TEST_ASSERT_EQUAL_FLOAT(-5.0,ceil(getIntersectionDistance(80000,80000,ACCELERATION*STEP_PER_MM,-10)));
}

/*
  Condition: the FinalRate is much higher than the InitialRate
                              + <- some maximum rate we don't care about
                             /|\
                           /  | \
                         /    |  + <- final_rate
       initial_rate -> +      |  |
                       |      |  |
                       |      |  |
                       +------+--+ 
                              ^  ^                   
                              |  |                   
          intersection_distance  distance     
      when the intersection disctance is more than the stepEventCount,
      the intersection disctance will equal to stepEventCount.
*/
void xtest_getIntersectionDistance_when_finalRate_is_much_higher_than_initialRate_(void){
    block_t *block = &blockBuffer[bufferHead];
  block->initialRate = 80000;
  block->stepEventCount = 500;
  block->finalRate = 90000;
  TEST_ASSERT_EQUAL_FLOAT(310,_ceil(getIntersectionDistance(block)));
  block->finalRate = 100000;
  TEST_ASSERT_EQUAL_FLOAT(375,_ceil(getIntersectionDistance(block)));
  block->finalRate = 130000;
  TEST_ASSERT_EQUAL_FLOAT(500,_ceil(getIntersectionDistance(block)));
  block->finalRate = 150000;
  TEST_ASSERT_EQUAL_FLOAT(500,_ceil(getIntersectionDistance(block)));
}

/*
  Condition: the FinalRate is much less than the InitialRate
                             
                          +
                         /|\ 
                        / | \
                       +  |  \
       initial_rate -> |  |   + <- final_rate
                       |  |   |
                       |  |   |
                       +--+---+ 
                          ^   ^                   
                          |  |                   
     intersection_distance  distance     
      when the intersection disctance is less than the stepEventCount,
      the intersection disctance will equal to 0.
*/
void test_getIntersectionDistance_when_finalRate_is_much_less_than_initialRate_(void){
  block_t *block = &blockBuffer[bufferHead];
  block->finalRate = 80000;
  block->stepEventCount = 500;
  block->initialRate = 90000;
  TEST_ASSERT_EQUAL_FLOAT(191,_ceil(getIntersectionDistance(block)));
  block->initialRate = 100000;
  TEST_ASSERT_EQUAL_FLOAT(125,_ceil(getIntersectionDistance(block)));
  block->initialRate = 130000;
  TEST_ASSERT_EQUAL_FLOAT(0,_ceil(getIntersectionDistance(block)));
  block->initialRate = 150000;
  TEST_ASSERT_EQUAL_FLOAT(0,_ceil(getIntersectionDistance(block)));
}

/*
  nominalRate = 100000
  entryFactor = 0.8
  exitFactor = 0.8
                              +------------------+ <-nominalRate
                             /|                  | \
                           /  |                  |  \
entryFactor*nominalRate->+    |                  |   +<-exitFactor*nominalRate
                         |    |                  |   |
                         +----+------------------+---+ <- stepEventCount
                              ^                  ^
                              |                  |
                  accelerateUntil    deccelerateAfter

*/
void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_and_exitFactor_is_0pts8(void){
  block_t *block = &blockBuffer[bufferHead];
  block->nominalRate = 100000;
  block->stepEventCount = 1000;
  blockMovingExecutionPlaning(block,0.8,0.8);
  TEST_ASSERT_EQUAL(80000,block->initialRate);
  TEST_ASSERT_EQUAL(80000,block->finalRate);
  TEST_ASSERT_EQUAL(250,block->accelerateUntil);
  TEST_ASSERT_EQUAL(750,block->deccelerateAfter);
}

/*
  nominalRate = 100000
  entryFactor = 0.5
  exitFactor = 0.8
                              +------------------+ <-nominalRate
                             /|                  | \
                           /  |                  |  \
entryFactor*nominalRate->+    |                  |   +<-exitFactor*nominalRate
                         |    |                  |   |
                         +----+------------------+---+ <- stepEventCount
                              ^                  ^
                              |                  |
                  accelerateUntil    deccelerateAfter

*/

void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts5_and_exitFactor_is_0pts8(void){
  block_t *block = &blockBuffer[bufferHead];
  block->nominalRate = 100000;
  block->stepEventCount = 1000;
  blockMovingExecutionPlaning(block,0.5,0.8);
  TEST_ASSERT_EQUAL(50000,block->initialRate);
  TEST_ASSERT_EQUAL(80000,block->finalRate);
  TEST_ASSERT_EQUAL(521,block->accelerateUntil);
  TEST_ASSERT_EQUAL(750,block->deccelerateAfter);
}

/*
  nominalRate = 100000
  entryFactor = 0.8
  exitFactor = 0.5
                              +------------------+ <-nominalRate
                             /|                  | \
                           /  |                  |  \
entryFactor*nominalRate->+    |                  |   +<-exitFactor*nominalRate
                         |    |                  |   |
                         +----+------------------+---+ <- stepEventCount
                              ^                  ^
                              |                  |
                  accelerateUntil    deccelerateAfter

*/

void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts8_and_exitFactor_is_0pts5(void){
  block_t *block = &blockBuffer[bufferHead];
  block->nominalRate = 100000;
  block->stepEventCount = 1000;
  blockMovingExecutionPlaning(block,0.8,0.5);
  TEST_ASSERT_EQUAL(80000,block->initialRate);
  TEST_ASSERT_EQUAL(50000,block->finalRate);
  TEST_ASSERT_EQUAL(250,block->accelerateUntil);
  TEST_ASSERT_EQUAL(479,block->deccelerateAfter);
}


/*
  nominalRate = 100000
  entryFactor = 0.5
  exitFactor = 0.5
                              + <-nominalRate
                             /|\
                           /  | \
entryFactor*nominalRate->+    |  +<-exitFactor*nominalRate
                         |    |  |
                         +----+--+ <- stepEventCount
                              ^       
                              |            
            accelerateUntil  and  deccelerateAfter

*/

void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts5_and_exitFactor_is_0pts5(void){
  block_t *block = &blockBuffer[bufferHead];
  block->nominalRate = 100000;
  block->stepEventCount = 1000;
  blockMovingExecutionPlaning(block,0.5,0.5);
  TEST_ASSERT_EQUAL(50000,block->initialRate);
  TEST_ASSERT_EQUAL(50000,block->finalRate);
  TEST_ASSERT_EQUAL(500,block->accelerateUntil);
  TEST_ASSERT_EQUAL(500,block->deccelerateAfter);
}

/*
  nominalRate = 100000
  entryFactor = 0.5
  exitFactor = 0.2
                              + <-nominalRate
                             /|\
                           /  | \
entryFactor*nominalRate->+    |  +<-exitFactor*nominalRate
                         |    |  |
                         +----+--+ <- stepEventCount
                              ^       
                              |            
            accelerateUntil  and  deccelerateAfter

*/

void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts5_and_exitFactor_is_0pts2(void){
  block_t *block = &blockBuffer[bufferHead];
  block->nominalRate = 100000;
  block->stepEventCount = 1000;
  blockMovingExecutionPlaning(block,0.5,0.2);
  TEST_ASSERT_EQUAL(50000,block->initialRate);
  TEST_ASSERT_EQUAL(20000,block->finalRate);
  TEST_ASSERT_EQUAL(428,block->accelerateUntil);
  TEST_ASSERT_EQUAL(428,block->deccelerateAfter);
}

/*
  nominalRate = 100000
  entryFactor = 0.5
  exitFactor = 0.01
                              + <-nominalRate
                             /|\
                           /  | \
entryFactor*nominalRate->+    |  +<-exitFactor*nominalRate
                         |    |  |
                         +----+--+ <- stepEventCount
                              ^       
                              |            
            accelerateUntil  and  deccelerateAfter

*/

void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts5_and_exitFactor_is_0pts01(void){
  block_t *block = &blockBuffer[bufferHead];
  block->nominalRate = 100000;
  block->stepEventCount = 1000;
  blockMovingExecutionPlaning(block,0.5,0.01);
  TEST_ASSERT_EQUAL(50000,block->initialRate);
  TEST_ASSERT_EQUAL(1000,block->finalRate);
  TEST_ASSERT_EQUAL(414,block->accelerateUntil);
  TEST_ASSERT_EQUAL(414,block->deccelerateAfter);
}

/*
  nominalRate = 100000
  entryFactor = 1.3
  exitFactor = 0.01
                              + <-nominalRate
                             /|\
                           /  | \
entryFactor*nominalRate->+    |  +<-exitFactor*nominalRate
                         |    |  |
                         +----+--+ <- stepEventCount
                              ^       
                              |            
            accelerateUntil  and  deccelerateAfter

*/

void test_blockMovingExecutionPlaning_nominalRate_is_150000_entryFactor_is_1pts3_and_exitFactor_is_0pts01(void){
  block_t *block = &blockBuffer[bufferHead];
  block->nominalRate = 150000;
  block->stepEventCount = 1000;
  blockMovingExecutionPlaning(block,0.867,0.0067);
  TEST_ASSERT_EQUAL(130050,block->initialRate);
  TEST_ASSERT_EQUAL(1005,block->finalRate);
  TEST_ASSERT_EQUAL(0,block->accelerateUntil);
  TEST_ASSERT_EQUAL(0,block->deccelerateAfter);
}

/*
  nominalRate = 100000
  entryFactor = 0.2
  exitFactor = 0.5
                              + <-nominalRate
                             /|\
                           /  | \
entryFactor*nominalRate->+    |  +<-exitFactor*nominalRate
                         |    |  |
                         +----+--+ <- stepEventCount
                              ^       
                              |            
            accelerateUntil  and  deccelerateAfter

*/

void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts2_and_exitFactor_is_0pts5(void){
  block_t *block = &blockBuffer[bufferHead];
  block->nominalRate = 100000;
  block->stepEventCount = 1000;
  blockMovingExecutionPlaning(block,0.2,0.5);
  TEST_ASSERT_EQUAL(20000,block->initialRate);
  TEST_ASSERT_EQUAL(50000,block->finalRate);
  TEST_ASSERT_EQUAL(573,block->accelerateUntil);
  TEST_ASSERT_EQUAL(573,block->deccelerateAfter);
}


/*
  nominalRate = 100000
  entryFactor = 0.2
  exitFactor = 0.5
                              + <-nominalRate
                             /|\
                           /  | \
entryFactor*nominalRate->+    |  +<-exitFactor*nominalRate
                         |    |  |
                         +----+--+ <- stepEventCount
                              ^       
                              |            
            accelerateUntil  and  deccelerateAfter

*/

void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts01_and_exitFactor_is_0pts5(void){
  block_t *block = &blockBuffer[bufferHead];
  block->nominalRate = 100000;
  block->stepEventCount = 1000;
  blockMovingExecutionPlaning(block,0.01,0.5);
  TEST_ASSERT_EQUAL(1000,block->initialRate);
  TEST_ASSERT_EQUAL(50000,block->finalRate);
  TEST_ASSERT_EQUAL(587,block->accelerateUntil);
  TEST_ASSERT_EQUAL(587,block->deccelerateAfter);
}

/*
  nominalRate = 150000
  entryFactor = 0.1
  exitFactor = 0.9
                              + <-nominalRate
                             /|\
                           /  | \
entryFactor*nominalRate->+    |  +<-exitFactor*nominalRate
                         |    |  |
                         +----+--+ <- stepEventCount
                              ^       
                              |            
            accelerateUntil  and  deccelerateAfter

*/
void test_blockMovingExecutionPlaning_nominalRate_is_150000_entryFactor_is_0pts01_and_exitFactor_is_0pts9(void){
  block_t *block = &blockBuffer[bufferHead];
  block->nominalRate = 150000;
  block->stepEventCount = 1000;
  blockMovingExecutionPlaning(block,0.01,0.9);
  TEST_ASSERT_EQUAL(1500,block->initialRate);
  TEST_ASSERT_EQUAL(135000,block->finalRate);
  TEST_ASSERT_EQUAL(1000,block->accelerateUntil);
  TEST_ASSERT_EQUAL(1000,block->deccelerateAfter);
}


void test__getUnitVector_the_posX_is_3_posY_is_4_and_posZ_is_5(void){
  TEST_ASSERT_EQUAL_FLOAT(0.42426,_getUnitVector(3,4,5,X_AXIS));
  TEST_ASSERT_EQUAL_FLOAT(0.56569,_getUnitVector(3,4,5,Y_AXIS));
  TEST_ASSERT_EQUAL_FLOAT(0.70711,_getUnitVector(3,4,5,Z_AXIS));
}
/*
             vectorA (8,0,0)  vectorB (5,0,0)
          ----------------->+----------------->

*/

void test_getCosThetaBetweenTwoVector_when_vectorA_is_X8Y0Z0_and_vectorB_is_X5Y0Z0(void){
  float unitVectorA[3];
  float unitVectorB[3];
  unitVectorA[X_AXIS] = _getUnitVector(8,0,0,X_AXIS);
  unitVectorA[Y_AXIS] = _getUnitVector(8,0,0,Y_AXIS);
  unitVectorA[Z_AXIS] = _getUnitVector(8,0,0,Z_AXIS);
  unitVectorB[X_AXIS] = _getUnitVector(5,0,0,X_AXIS);
  unitVectorB[Y_AXIS] = _getUnitVector(5,0,0,Y_AXIS);
  unitVectorB[Z_AXIS] = _getUnitVector(5,0,0,Z_AXIS);
  TEST_ASSERT_EQUAL_FLOAT(1,unitVectorA[X_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0,unitVectorA[Y_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0,unitVectorA[Z_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(1,unitVectorB[X_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0,unitVectorB[Y_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0,unitVectorB[Z_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(-1,getCosThetaBetweenTwoVector(unitVectorA,unitVectorB));
}

/*            
                               
                              /
                    vectorB /
                          /
                        /
                      /
                    / ) theta
                   +------------------>
                        vetorA 
            
*/

void test_getCosThetaBetweenTwoVector_when_vectorA_is_X8Y0Z0_and_vectorB_is_X4Y3Z0(void){
  float unitVectorA[3];
  float unitVectorB[3];
  unitVectorA[X_AXIS] = _getUnitVector(8,0,0,X_AXIS);
  unitVectorA[Y_AXIS] = _getUnitVector(8,0,0,Y_AXIS);
  unitVectorA[Z_AXIS] = _getUnitVector(8,0,0,Z_AXIS);
  unitVectorB[X_AXIS] = _getUnitVector(4,3,0,X_AXIS);
  unitVectorB[Y_AXIS] = _getUnitVector(4,3,0,Y_AXIS);
  unitVectorB[Z_AXIS] = _getUnitVector(4,3,0,Z_AXIS);
  TEST_ASSERT_EQUAL_FLOAT(1,unitVectorA[X_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0,unitVectorA[Y_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0,unitVectorA[Z_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0.8,unitVectorB[X_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0.6,unitVectorB[Y_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0,unitVectorB[Z_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(-0.8,getCosThetaBetweenTwoVector(unitVectorA,unitVectorB));
}


/*



           vectorA       vectorB
     <--------------+-------------->



*/
void test_getCosThetaBetweenTwoVector_when_vectorA_is_X8Y0Z0_and_vectorB_is_Xnegate2Y0Z0(void){
  float unitVectorA[3];
  float unitVectorB[3];
  unitVectorA[X_AXIS] = _getUnitVector(8,0,0,X_AXIS);
  unitVectorA[Y_AXIS] = _getUnitVector(8,0,0,Y_AXIS);
  unitVectorA[Z_AXIS] = _getUnitVector(8,0,0,Z_AXIS);
  unitVectorB[X_AXIS] = _getUnitVector(-2,0,0,X_AXIS);
  unitVectorB[Y_AXIS] = _getUnitVector(-2,0,0,Y_AXIS);
  unitVectorB[Z_AXIS] = _getUnitVector(-2,0,0,Z_AXIS);
  TEST_ASSERT_EQUAL_FLOAT(1,unitVectorA[X_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0,unitVectorA[Y_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0,unitVectorA[Z_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(-1,unitVectorB[X_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0,unitVectorB[Y_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0,unitVectorB[Z_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(1,getCosThetaBetweenTwoVector(unitVectorA,unitVectorB));
}

void test_getCosThetaBetweenTwoVector_when_vectorA_is_X8Y10Z2_and_vectorB_is_X3Y4Z5(void){
  float unitVectorA[3];
  float unitVectorB[3];
  unitVectorA[X_AXIS] = _getUnitVector(8,10,2,X_AXIS);
  unitVectorA[Y_AXIS] = _getUnitVector(8,10,2,Y_AXIS);
  unitVectorA[Z_AXIS] = _getUnitVector(8,10,2,Z_AXIS);
  unitVectorB[X_AXIS] = _getUnitVector(3,4,5,X_AXIS);
  unitVectorB[Y_AXIS] = _getUnitVector(3,4,5,Y_AXIS);
  unitVectorB[Z_AXIS] = _getUnitVector(3,4,5,Z_AXIS);
  TEST_ASSERT_EQUAL_FLOAT(0.61721,unitVectorA[X_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0.77152,unitVectorA[Y_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0.154303,unitVectorA[Z_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0.42426,unitVectorB[X_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0.56569,unitVectorB[Y_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0.70711,unitVectorB[Z_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(-0.80740,getCosThetaBetweenTwoVector(unitVectorA,unitVectorB));
}

void test_entrySpeedPlanning_the_enntry_speed_first_block_is_0(void){
  block_t *block = &blockBuffer[bufferHead];
  TEST_ASSERT_EQUAL(bufferHead,bufferTail);
  entrySpeedPlanning(block,0);
  TEST_ASSERT_EQUAL_FLOAT(0,block->entrySpeed); 
}

/*
                                         
                                    firstBlock          secondBlock
                              +------------------+--+--+--------------+ 
                             /|                  |  |  |              |\
                           /  |                  |  |  |              | \    
entryFactor*nominalRate->+    |                  |  |  |              |  + <-exitFactor*nominalRate
                         |    |                  |  |  |              |  | 
                         +----+------------------+--+--+-----------------+
                  
          
*/

void test_entrySpeedPlanning_when_add_in_a_block_which_the_vector_is_same_with_previous_block_and_previousNominalSpeed_is_higher_than_the_current_nominalSpeed(void){
  updatebufferHead();
  block_t *block = &blockBuffer[bufferHead];
  block->nominalSpeed = 100;
  plExecutor.previousNominalSpeed = 150;
  entrySpeedPlanning(block,-1);
  TEST_ASSERT_EQUAL_FLOAT(100,block->entrySpeed);
}

/*
                                                            SecondBlock
                                                        +--------------+
                                    firstBlock        / |              |\
                              +------------------+---+  |              | \
                             /|                  |   |  |              |  \
                           /  |                  |   |  |              |   \    
entryFactor*nominalRate->+    |                  |   |  |              |    + <-exitFactor*nominalRate
                         |    |                  |   |  |              |    | 
                         +----+------------------+---+--+--------------+----+
                  
          
*/
void test_entrySpeedPlanning_when_add_in_a_block_which_the_vector_is_same_with_previous_block_and_previousNominalSpeed_is_less_than_the_current_nominalSpeed(void){
  updatebufferHead();
  block_t *block = &blockBuffer[bufferHead];
  block->nominalSpeed = 100;
  plExecutor.previousNominalSpeed = 80;
  entrySpeedPlanning(block,-1);
  TEST_ASSERT_EQUAL_FLOAT(80,block->entrySpeed);
}

void test_planLineBlock_and_entrySpeedPlanning_put_Moving_X_axis_is_300_Y_axis_is_400_and_Z_axis_is_100_(void){
  
  block_t *block = &blockBuffer[bufferHead];
  planLineBlock(300,400,100,100,0);
  TEST_ASSERT_EQUAL(0x00,block->directionBits);
  TEST_ASSERT_EQUAL_FLOAT(509.902,block->millimeters);
  TEST_ASSERT_FLOAT_WITHIN(0.00002,0.0019611,block->inverseMillimeters);
  TEST_ASSERT_EQUAL(60000,block->steps[X_AXIS]); 
  TEST_ASSERT_EQUAL(80000,block->steps[Y_AXIS]); 
  TEST_ASSERT_EQUAL(20000,block->steps[Z_AXIS]); 
  TEST_ASSERT_EQUAL(80000,block->stepEventCount);
  TEST_ASSERT_EQUAL(15690,block->nominalRate);
  TEST_ASSERT_EQUAL(100,block->nominalSpeed); 
  TEST_ASSERT_EQUAL(0,block->entrySpeed); 
  TEST_ASSERT_EQUAL(0,bufferTail);
  TEST_ASSERT_EQUAL(1,bufferHead);
  TEST_ASSERT_EQUAL(2,nextBufferHead);
  TEST_ASSERT_EQUAL_FLOAT(0.58835,plExecutor.previousUnitVec[X_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0.78446,plExecutor.previousUnitVec[Y_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0.196116,plExecutor.previousUnitVec[Z_AXIS]);
  TEST_ASSERT_EQUAL(60000,plExecutor.position[X_AXIS]); 
  TEST_ASSERT_EQUAL(80000,plExecutor.position[Y_AXIS]); 
  TEST_ASSERT_EQUAL(20000,plExecutor.position[Z_AXIS]);
  TEST_ASSERT_EQUAL(100,plExecutor.previousNominalSpeed);
  
  
  block = &blockBuffer[bufferHead];
  planLineBlock(400,400,100,100,0);
  TEST_ASSERT_EQUAL_FLOAT(100,block->millimeters);
  TEST_ASSERT_FLOAT_WITHIN(0.00002,0.01,block->inverseMillimeters);
  TEST_ASSERT_EQUAL(20000,block->steps[X_AXIS]); 
  TEST_ASSERT_EQUAL(0,block->steps[Y_AXIS]); 
  TEST_ASSERT_EQUAL(0,block->steps[Z_AXIS]); 
  TEST_ASSERT_EQUAL(20000,block->stepEventCount);
  TEST_ASSERT_EQUAL(20000,block->nominalRate);
  TEST_ASSERT_EQUAL(100,block->nominalSpeed); 
  TEST_ASSERT_EQUAL(100,block->entrySpeed); 
  TEST_ASSERT_EQUAL(0,bufferTail);
  TEST_ASSERT_EQUAL(2,bufferHead);
  TEST_ASSERT_EQUAL(3,nextBufferHead);
  TEST_ASSERT_EQUAL_FLOAT(1,plExecutor.previousUnitVec[X_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0,plExecutor.previousUnitVec[Y_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0,plExecutor.previousUnitVec[Z_AXIS]);
  TEST_ASSERT_EQUAL(80000,plExecutor.position[X_AXIS]); 
  TEST_ASSERT_EQUAL(80000,plExecutor.position[Y_AXIS]); 
  TEST_ASSERT_EQUAL(20000,plExecutor.position[Z_AXIS]);
  TEST_ASSERT_EQUAL(100,plExecutor.previousNominalSpeed);
  
  block = &blockBuffer[bufferHead];
  planLineBlock(500,400,100,90,0);
  TEST_ASSERT_EQUAL_FLOAT(100,block->millimeters);
  TEST_ASSERT_FLOAT_WITHIN(0.00002,0.01,block->inverseMillimeters);
  TEST_ASSERT_EQUAL(20000,block->steps[X_AXIS]); 
  TEST_ASSERT_EQUAL(0,block->steps[Y_AXIS]); 
  TEST_ASSERT_EQUAL(0,block->steps[Z_AXIS]); 
  TEST_ASSERT_EQUAL(20000,block->stepEventCount);
  TEST_ASSERT_EQUAL(18000,block->nominalRate);
  TEST_ASSERT_EQUAL(90,block->nominalSpeed); 
  TEST_ASSERT_EQUAL(90,block->entrySpeed); 
  TEST_ASSERT_EQUAL(0,bufferTail);
  TEST_ASSERT_EQUAL(3,bufferHead);
  TEST_ASSERT_EQUAL(4,nextBufferHead);
  TEST_ASSERT_EQUAL_FLOAT(1,plExecutor.previousUnitVec[X_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0,plExecutor.previousUnitVec[Y_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0,plExecutor.previousUnitVec[Z_AXIS]);
  TEST_ASSERT_EQUAL(100000,plExecutor.position[X_AXIS]); 
  TEST_ASSERT_EQUAL(80000,plExecutor.position[Y_AXIS]); 
  TEST_ASSERT_EQUAL(20000,plExecutor.position[Z_AXIS]);
  TEST_ASSERT_EQUAL(90,plExecutor.previousNominalSpeed);
}

/*




                              +------------------+ <-nominalRate
                             /|                  | \
                           /  |                  |  \
entryFactor*nominalRate->+    |                  |   +<-exitFactor*nominalRate
                         |    |                  |   |
                         +----+------------------+---+ <- stepEventCount
                              ^                  ^
                              |                  |
                  accelerateUntil    deccelerateAfter
*/
void test_planLineBlock_replanBlockBufferStructure_only_one_block_is_added_into_BlockBuffer_to_do_blockMovingExecutionPlaning(void){
  block_t *block = &blockBuffer[bufferHead];
  planLineBlock(300,400,100,100,0);
  
  TEST_ASSERT_EQUAL(0x00,block->directionBits);
  TEST_ASSERT_EQUAL_FLOAT(509.902,block->millimeters);
  TEST_ASSERT_FLOAT_WITHIN(0.00002,0.0019611,block->inverseMillimeters);
  TEST_ASSERT_EQUAL(60000,block->steps[X_AXIS]); 
  TEST_ASSERT_EQUAL(80000,block->steps[Y_AXIS]); 
  TEST_ASSERT_EQUAL(20000,block->steps[Z_AXIS]); 
  TEST_ASSERT_EQUAL(80000,block->stepEventCount);
  TEST_ASSERT_EQUAL(15690,block->nominalRate);
  TEST_ASSERT_EQUAL(100,block->nominalSpeed); 
  TEST_ASSERT_EQUAL(0,block->entrySpeed); 
  TEST_ASSERT_EQUAL(0,bufferTail);
  TEST_ASSERT_EQUAL(1,bufferHead);
  TEST_ASSERT_EQUAL(2,nextBufferHead);
  TEST_ASSERT_EQUAL_FLOAT(0.58835,plExecutor.previousUnitVec[X_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0.78446,plExecutor.previousUnitVec[Y_AXIS]);
  TEST_ASSERT_EQUAL_FLOAT(0.196116,plExecutor.previousUnitVec[Z_AXIS]);
  TEST_ASSERT_EQUAL(60000,plExecutor.position[X_AXIS]); 
  TEST_ASSERT_EQUAL(80000,plExecutor.position[Y_AXIS]); 
  TEST_ASSERT_EQUAL(20000,plExecutor.position[Z_AXIS]);
  TEST_ASSERT_EQUAL(100,plExecutor.previousNominalSpeed);
  TEST_ASSERT_EQUAL(0,block->initialRate);
  TEST_ASSERT_EQUAL(0,block->finalRate);
  TEST_ASSERT_EQUAL(18,block->accelerateUntil);
  TEST_ASSERT_EQUAL(79982,block->deccelerateAfter); 
}

/*
                                   firstBlock          secondBlock      
 nominalSpeed = 100(mm/min)-> +------------------+--+--+--------------+ 
                             /|                  |  |  |              |\
                           /  |                  |  |  |              | \    
             entrySpeed->+    |                  |  |  |              |  + <-exitSpeed
                         |    |                  |  |  |              |  | 
                         +----+------------------+--+--+-----------------+
       
       
       
                                     (stpes/min)      SecondBlock
                               nominalRate = 20000-> +--+--------------+
                   (stpes/min)        firstBlock     |  |              |\
        nominalRate = 15690-> +------------------+---+  |              | \
                             /|                  |   |  |              |  \
                           /  |                  |   |  |              |   \    
entryFactor*nominalRate->+    |                  |   |  |              |    + <-exitFactor*nominalRate
                         |    |                  |   |  |              |    | 
                         +----+------------------+---+--+--------------+----+
                         <--------------------------><---------------------->
                                80000 steps                  20000 steps
      

*/
void test_planLineBlock_replanBlockBufferStructure_two_block_are_added_with_same_nominal_speed_into_BlockBuffer_to_do_blockMovingExecutionPlaning(void){
  block_t *block = &blockBuffer[bufferTail];
  planLineBlock(300,400,100,100,0);
  
  TEST_ASSERT_EQUAL(15690,block->nominalRate);
  TEST_ASSERT_EQUAL(0,block->initialRate);
  TEST_ASSERT_EQUAL(0,block->finalRate);
  TEST_ASSERT_EQUAL(18,block->accelerateUntil);
  TEST_ASSERT_EQUAL(79982,block->deccelerateAfter); 
  
  planLineBlock(400,400,100,100,0);
  
  block_t *block1 = &blockBuffer[bufferTail];
  block_t *block2 = &blockBuffer[bufferTail+1];
  TEST_ASSERT_EQUAL(0,block1->initialRate);
  TEST_ASSERT_EQUAL(80000,block1->stepEventCount);
  TEST_ASSERT_EQUAL(0,block1->entrySpeed);
  TEST_ASSERT_EQUAL(15690,block1->nominalRate);
  TEST_ASSERT_EQUAL(15690,block1->finalRate);
  TEST_ASSERT_EQUAL(18,block1->accelerateUntil);
  TEST_ASSERT_EQUAL(80000,block1->deccelerateAfter); 
  
  TEST_ASSERT_EQUAL(20000,block2->stepEventCount);
  TEST_ASSERT_EQUAL(100,block2->entrySpeed);
  TEST_ASSERT_EQUAL(20000,block2->initialRate);
  TEST_ASSERT_EQUAL(20000,block2->nominalRate);
  TEST_ASSERT_EQUAL(0,block2->finalRate);
  TEST_ASSERT_EQUAL(0,block2->accelerateUntil);
  TEST_ASSERT_EQUAL(19972,block2->deccelerateAfter);
}

/*

                                    firstBlock        
  nominalSpeed = 100(mm/min)->+------------------+---+ 
                             /|                  |   |\  SecondBlock
                           /  |                  |   | +--------------+ <- nominalSpeed = 90(mm/min)
             entrySpeed->+    |                  |   | |              |\ 
                         |    |                  |   | |              | \   
                         +----+------------------+---+--+-------------+--+<-exitSpeed
                  
                                           
                                                            SecondBlock
                   (stpes/min)        firstBlock      +--+--------------+ <- nominalRate = 18000(stpes/min)
        nominalRate = 15690-> +------------------+   |  |              | \
                             /|                  | \ |  |              |  \
                           /  |                  |  \|  |              |   \    
entryFactor*nominalRate->+    |                  |   +  |              |    + <-exitFactor*nominalRate
                         |    |                  |   |  |              |    | 
                         +----+------------------+---+--+--------------+----+
                         <--------------------------><---------------------->
                                80000 steps                  20000 steps
      

*/

void test_planLineBlock_replanBlockBufferStructure_two_block_is_added_with_different_nominal_speed_NominalSpeed_of_Firblock_is_higher_than_SecBlock(void){
  block_t *block = &blockBuffer[bufferTail];
  planLineBlock(300,400,100,100,0);
  
  TEST_ASSERT_EQUAL(15690,block->nominalRate);
  TEST_ASSERT_EQUAL(0,block->initialRate);
  TEST_ASSERT_EQUAL(0,block->finalRate);
  TEST_ASSERT_EQUAL(18,block->accelerateUntil);
  TEST_ASSERT_EQUAL(79982,block->deccelerateAfter); 
  
  planLineBlock(400,400,100,90,0);
  
  block_t *block1 = &blockBuffer[bufferTail];
  block_t *block2 = &blockBuffer[bufferTail+1];
  TEST_ASSERT_EQUAL(0,block1->initialRate);
  TEST_ASSERT_EQUAL(80000,block1->stepEventCount);
  TEST_ASSERT_EQUAL(0,block1->entrySpeed);
  TEST_ASSERT_EQUAL(15690,block1->nominalRate);
  TEST_ASSERT_EQUAL(14121,block1->finalRate);
  TEST_ASSERT_EQUAL(18,block1->accelerateUntil);
  TEST_ASSERT_EQUAL(79996,block1->deccelerateAfter); 
  
  TEST_ASSERT_EQUAL(20000,block2->stepEventCount);
  TEST_ASSERT_EQUAL(90,block2->entrySpeed);
  TEST_ASSERT_EQUAL(18000,block2->initialRate);
  TEST_ASSERT_EQUAL(18000,block2->nominalRate);
  TEST_ASSERT_EQUAL(0,block2->finalRate);
  TEST_ASSERT_EQUAL(0,block2->accelerateUntil);
  TEST_ASSERT_EQUAL(19977,block2->deccelerateAfter);
}


/*
                                                            SecondBlock
                                                        +--------------+<-nominalSpeed = 120(mm/min)->
                                    firstBlock        / |              |\
  nominalSpeed = 100(mm/min)->+------------------+---+  |              | \
                             /|                  |   |  |              |  \
                           /  |                  |   |  |              |   \    
entryFactor*nominalRate->+    |                  |   |  |              |    + <-exitFactor*nominalRate
                         |    |                  |   |  |              |    | 
                         +----+------------------+---+--+--------------+----+
      
                                                           SecondBlock
                                                        +--------------+ <-nominalRate = 24000(stpes/min)
                                                      / |              |\
                   (stpes/min)        firstBlock     +  +              + + <-nominalRate = 20000(stpes/min)
        nominalRate = 15690-> +------------------+---|  |              |  \
                             /|                  |   |  |              |   \
                           /  |                  |   |  |              |    \    
entryFactor*nominalRate->+    |                  |   |  |              |    + <-exitFactor*nominalRate
                         |    |                  |   |  |              |    | 
                         +----+------------------+---+--+--------------+----+
                         <--------------------------><---------------------->
                                80000 steps                  20000 steps
         
      
*/
void test_planLineBlock_replanBlockBufferStructure_two_block_is_added_with_different_nominal_speed_NominalSpeed_of_Firblock_is_less_than_SecBlock(void){
  block_t *block = &blockBuffer[bufferTail];
  planLineBlock(300,400,100,100,0);
  
  TEST_ASSERT_EQUAL(15690,block->nominalRate);
  TEST_ASSERT_EQUAL(0,block->initialRate);
  TEST_ASSERT_EQUAL(0,block->finalRate);
  TEST_ASSERT_EQUAL(18,block->accelerateUntil);
  TEST_ASSERT_EQUAL(79982,block->deccelerateAfter); 
  planLineBlock(400,400,100,120,0);
  
  block_t *block1 = &blockBuffer[bufferTail];
  block_t *block2 = &blockBuffer[bufferTail+1];
  TEST_ASSERT_EQUAL(0,block1->initialRate);
  TEST_ASSERT_EQUAL(80000,block1->stepEventCount);
  TEST_ASSERT_EQUAL(0,block1->entrySpeed);
  TEST_ASSERT_EQUAL(15690,block1->nominalRate);
  TEST_ASSERT_EQUAL(15690,block1->finalRate);
  TEST_ASSERT_EQUAL(18,block1->accelerateUntil);
  TEST_ASSERT_EQUAL(80000,block1->deccelerateAfter); 
  
  TEST_ASSERT_EQUAL(20000,block2->stepEventCount);
  TEST_ASSERT_EQUAL(100,block2->entrySpeed);
  TEST_ASSERT_EQUAL(20000,block2->initialRate);
  TEST_ASSERT_EQUAL(24000,block2->nominalRate);
  TEST_ASSERT_EQUAL(0,block2->finalRate);
  TEST_ASSERT_EQUAL(13,block2->accelerateUntil);
  TEST_ASSERT_EQUAL(19960,block2->deccelerateAfter);
}