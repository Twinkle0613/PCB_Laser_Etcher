#include "Planner.h"
#include "unity.h"


void setUp(void)

{

  bufferTail = 0;

  planInit();

}



void tearDown(void)

{

}

void test_nextBlockIndex_Index_point_to_next_block(void){

  int index = 0;

  index = nextBlockIndex(index);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((index)), (((void *)0)), (_U_UINT)(26), UNITY_DISPLAY_STYLE_INT);

}

void test_nextBlockIndex_Index_point_to_next_block_with_overflow(void){

  int index = 17;

  index = nextBlockIndex(index);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((index)), (((void *)0)), (_U_UINT)(41), UNITY_DISPLAY_STYLE_INT);

}

void test_prevBlockIndex_Index_point_to_previous_block(void){

    int index = 1;

    index = prevBlockIndex(index);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((index)), (((void *)0)), (_U_UINT)(55), UNITY_DISPLAY_STYLE_INT);

}

void test_prevBlockIndex_Index_point_to_previous_block_with_underflow(void){

  int index = 0;

  index = prevBlockIndex(index);

  UnityAssertEqualNumber((_U_SINT)((17)), (_U_SINT)((index)), (((void *)0)), (_U_UINT)(68), UNITY_DISPLAY_STYLE_INT);

}

void test_planInit_initialize_the_bufferHead_bufferTail_and_nextBufferHead(void){

  planInit();

  UnityAssertEqualNumber((_U_SINT)((bufferHead)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(82), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((nextBufferHead)), (_U_SINT)((1)), (((void *)0)), (_U_UINT)(83), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.position[0])), (((void *)0)), (_U_UINT)(84), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.position[1])), (((void *)0)), (_U_UINT)(85), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.position[2])), (((void *)0)), (_U_UINT)(86), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.previousUnitVec[0])), (((void *)0)), (_U_UINT)(87), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.previousUnitVec[1])), (((void *)0)), (_U_UINT)(88), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.previousUnitVec[2])), (((void *)0)), (_U_UINT)(89), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.previousNominalSpeed)), (((void *)0)), (_U_UINT)(90), UNITY_DISPLAY_STYLE_INT);

}

void test_getBlock_to_get_the_block_when_buffer_contained_block(void){

  bufferHead = nextBlockIndex(bufferHead);

  bufferHead = nextBlockIndex(bufferHead);

  bufferHead = nextBlockIndex(bufferHead);

  UnityAssertEqualNumber((_U_SINT)((3)), (_U_SINT)((bufferHead)), (((void *)0)), (_U_UINT)(106), UNITY_DISPLAY_STYLE_INT);

  block_t* currentBlock = getCurrentBlock();

  UnityAssertEqualNumber((_U_SINT)(_UP)((currentBlock)), (_U_SINT)(_UP)((&(blockBuffer[bufferTail]))), (((void *)0)), (_U_UINT)(108), UNITY_DISPLAY_STYLE_HEX32);

}

void test_getBlock_to_get_NULL_when_buffer_dont_have_any_block(void){

  block_t* currentBlock = getCurrentBlock();

  if ((((currentBlock)) == ((void *)0))) {} else {UnityFail( (((" Expected NULL"))), (_U_UINT)((_U_UINT)((_U_UINT)(121))));};

}

void test_discardCurrrentBlock_if_blockBuffer_contain_block_the_buffetTail_will_be_increased(void){

  bufferHead = nextBlockIndex(bufferHead);

  bufferHead = nextBlockIndex(bufferHead);

  bufferHead = nextBlockIndex(bufferHead);

  discardCurrentBlock();

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(139), UNITY_DISPLAY_STYLE_INT);

}

void test_discardCurrrentBlock_if_blockBuffer_doesnt_contain_block_the_buffetTail_will_remain(void){

  discardCurrentBlock();

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(153), UNITY_DISPLAY_STYLE_INT);

}



void test_blockConfig_StepX_is_100_StepY_is_75_StepZ_is_10_the_StepX_is_largest(void){

   bufferHead = nextBlockIndex(bufferHead);

   blockConfig(&blockBuffer[bufferTail],0x00,10000,75,10,80000,160000,80000);

   UnityAssertEqualNumber((_U_SINT)((10000)), (_U_SINT)((blockBuffer[bufferTail].stepEventCount)), (((void *)0)), (_U_UINT)(159), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((0x00)), (_U_SINT)((blockBuffer[bufferTail].directionBits)), (((void *)0)), (_U_UINT)(160), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((10000)), (_U_SINT)((blockBuffer[bufferTail].steps[0])), (((void *)0)), (_U_UINT)(161), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((75)), (_U_SINT)((blockBuffer[bufferTail].steps[1])), (((void *)0)), (_U_UINT)(162), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((10)), (_U_SINT)((blockBuffer[bufferTail].steps[2])), (((void *)0)), (_U_UINT)(163), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((blockBuffer[bufferTail].initialRate)), (((void *)0)), (_U_UINT)(164), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((blockBuffer[bufferTail].nominalRate)), (((void *)0)), (_U_UINT)(165), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((blockBuffer[bufferTail].finalRate)), (((void *)0)), (_U_UINT)(166), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((3334)), (_U_SINT)((blockBuffer[bufferTail].accelerateUntil)), (((void *)0)), (_U_UINT)(167), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((6666)), (_U_SINT)((blockBuffer[bufferTail].deccelerateAfter)), (((void *)0)), (_U_UINT)(168), UNITY_DISPLAY_STYLE_INT);

}



void test_blockConfig_StepX_is_100_StepY_is_300_StepZ_is_200_StepY_is_largest(void){

   bufferHead = nextBlockIndex(bufferHead);

   blockConfig(&blockBuffer[bufferTail],0x00,100,300,200,6000,8000,6000);

   UnityAssertEqualNumber((_U_SINT)((300)), (_U_SINT)((blockBuffer[bufferTail].stepEventCount)), (((void *)0)), (_U_UINT)(174), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((0x00)), (_U_SINT)((blockBuffer[bufferTail].directionBits)), (((void *)0)), (_U_UINT)(175), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((blockBuffer[bufferTail].steps[0])), (((void *)0)), (_U_UINT)(176), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((300)), (_U_SINT)((blockBuffer[bufferTail].steps[1])), (((void *)0)), (_U_UINT)(177), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((blockBuffer[bufferTail].steps[2])), (((void *)0)), (_U_UINT)(178), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((6000)), (_U_SINT)((blockBuffer[bufferTail].initialRate)), (((void *)0)), (_U_UINT)(179), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((6000)), (_U_SINT)((blockBuffer[bufferTail].finalRate)), (((void *)0)), (_U_UINT)(180), UNITY_DISPLAY_STYLE_INT);

}



void test_blockConfig_StepX_is_100_StepY_is_300_StepZ_is_1000_StepZ_is_largest(void){

   bufferHead = nextBlockIndex(bufferHead);

   blockConfig(&blockBuffer[bufferTail],0x00,100,300,1000,7000,9000,7000);

   UnityAssertEqualNumber((_U_SINT)((1000)), (_U_SINT)((blockBuffer[bufferTail].stepEventCount)), (((void *)0)), (_U_UINT)(186), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((0x00)), (_U_SINT)((blockBuffer[bufferTail].directionBits)), (((void *)0)), (_U_UINT)(187), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((blockBuffer[bufferTail].steps[0])), (((void *)0)), (_U_UINT)(188), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((300)), (_U_SINT)((blockBuffer[bufferTail].steps[1])), (((void *)0)), (_U_UINT)(189), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((1000)), (_U_SINT)((blockBuffer[bufferTail].steps[2])), (((void *)0)), (_U_UINT)(190), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((7000)), (_U_SINT)((blockBuffer[bufferTail].initialRate)), (((void *)0)), (_U_UINT)(191), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((9000)), (_U_SINT)((blockBuffer[bufferTail].nominalRate)), (((void *)0)), (_U_UINT)(192), UNITY_DISPLAY_STYLE_INT);

}



void test_ceil_put_the_value_from_3point0_to_3point9_into_the_function_(void){

  UnityAssertEqualNumber((_U_SINT)((3)), (_U_SINT)((ceil(3.0))), (((void *)0)), (_U_UINT)(196), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.1))), (((void *)0)), (_U_UINT)(197), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.2))), (((void *)0)), (_U_UINT)(198), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.3))), (((void *)0)), (_U_UINT)(199), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.4))), (((void *)0)), (_U_UINT)(200), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.5))), (((void *)0)), (_U_UINT)(201), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.6))), (((void *)0)), (_U_UINT)(202), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.7))), (((void *)0)), (_U_UINT)(203), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.8))), (((void *)0)), (_U_UINT)(204), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.9))), (((void *)0)), (_U_UINT)(205), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(4.0))), (((void *)0)), (_U_UINT)(206), UNITY_DISPLAY_STYLE_INT);

}

void test_estimateAccelStepAndDeccelStep_when_the_initRate_is_80000_nominalRate_is_160000_finalRate_is_80000(void){

  block_t block;

  block.stepEventCount = 10000;

  block.initialRate = 80000;

  block.nominalRate = 160000;

  block.finalRate = 80000;

  estimateAccelStepAndDeccelStep(&block);

  UnityAssertEqualNumber((_U_SINT)((3334)), (_U_SINT)((block.accelerateUntil)), (((void *)0)), (_U_UINT)(230), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((6667)), (_U_SINT)((block.deccelerateAfter)), (((void *)0)), (_U_UINT)(231), UNITY_DISPLAY_STYLE_INT);

}

void test_estimateAccelStepAndDeccelStep_when_the_initRate_is_80000_nominalRate_is_200000_finalRate_is_800(void){

  block_t block;

  block.stepEventCount = 15000;

  block.initialRate = 80000;

  block.nominalRate = 200000;

  block.finalRate = 800;

  estimateAccelStepAndDeccelStep(&block);

  UnityAssertEqualNumber((_U_SINT)((5834)), (_U_SINT)((block.accelerateUntil)), (((void *)0)), (_U_UINT)(254), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((8056)), (_U_SINT)((block.deccelerateAfter)), (((void *)0)), (_U_UINT)(255), UNITY_DISPLAY_STYLE_INT);

}

void test_estimateAccelStepAndDeccelStep_when_the_initRate_is_800_nominalRate_is_200000_finalRate_is_800(void){

  block_t block;

  block.stepEventCount = 15000;

  block.initialRate = 800;

  block.nominalRate = 200000;

  block.finalRate = 800;

  estimateAccelStepAndDeccelStep(&block);

  UnityAssertEqualNumber((_U_SINT)((6945)), (_U_SINT)((block.accelerateUntil)), (((void *)0)), (_U_UINT)(278), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((8056)), (_U_SINT)((block.deccelerateAfter)), (((void *)0)), (_U_UINT)(279), UNITY_DISPLAY_STYLE_INT);

}

void test_estimateAccelStepAndDeccelStep_when_the_initRate_is_800_nominalRate_is_1000_finalRate_is_800(void){

  block_t block;

  block.stepEventCount = 15000;

  block.initialRate = 800;

  block.nominalRate = 1000;

  block.finalRate = 800;

  estimateAccelStepAndDeccelStep(&block);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((block.accelerateUntil)), (((void *)0)), (_U_UINT)(303), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((15000)), (_U_SINT)((block.deccelerateAfter)), (((void *)0)), (_U_UINT)(304), UNITY_DISPLAY_STYLE_INT);

}

void xtest_estimateAccelStepAndDeccelStep_when_the_initRate_is_80000_nominalRate_is_200000_finalRate_is_80000(void){

  block_t block;

  block.stepEventCount = 10000;

  block.initialRate = 80000;

  block.nominalRate = 200000;

  block.finalRate = 80000;

  estimateAccelStepAndDeccelStep(&block);

  UnityAssertEqualNumber((_U_SINT)((6945)), (_U_SINT)((block.accelerateUntil)), (((void *)0)), (_U_UINT)(328), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((8056)), (_U_SINT)((block.deccelerateAfter)), (((void *)0)), (_U_UINT)(329), UNITY_DISPLAY_STYLE_INT);

}
