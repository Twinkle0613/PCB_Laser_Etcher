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