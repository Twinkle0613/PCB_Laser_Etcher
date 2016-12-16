#include "unity.h"
#include "Planner.h"

void setUp(void)
{
  bufferTail = 0;
  planInit();
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

void test_blockConfig_StepX_is_100_StepY_is_75_StepZ_is_10_the_StepX_is_largest(void){
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

void test_blockConfig_StepX_is_100_StepY_is_300_StepZ_is_200_StepY_is_largest(void){
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

void test_blockConfig_StepX_is_100_StepY_is_300_StepZ_is_1000_StepZ_is_largest(void){
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
void test_estimateAccelStepAndDeccelStep_when_the_initRate_is_80000_nominalRate_is_160000_finalRate_is_80000(void){
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
void test_estimateAccelStepAndDeccelStep_when_the_initRate_is_80000_nominalRate_is_200000_finalRate_is_800(void){
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
void test_estimateAccelStepAndDeccelStep_when_the_initRate_is_800_nominalRate_is_200000_finalRate_is_800(void){
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

void test_estimateAccelStepAndDeccelStep_when_the_initRate_is_800_nominalRate_is_1000_finalRate_is_800(void){
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
  800 step/min---> |/                          |   \  <--- 800 step/min
                   |   |                        |   |    
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