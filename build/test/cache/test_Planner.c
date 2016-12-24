#include "Planner.h"
#include "unity.h"


void setUp(void)

{

  bufferTail = 0;

  planInit();

  memset(blockBuffer, 0, sizeof(blockBuffer));

}



void tearDown(void)

{

}

void test_nextBlockIndex_Index_point_to_next_block(void){

  int index = 0;

  index = nextBlockIndex(index);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((index)), (((void *)0)), (_U_UINT)(27), UNITY_DISPLAY_STYLE_INT);

}

void test_nextBlockIndex_Index_point_to_next_block_with_overflow(void){

  int index = 17;

  index = nextBlockIndex(index);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((index)), (((void *)0)), (_U_UINT)(42), UNITY_DISPLAY_STYLE_INT);

}

void test_prevBlockIndex_Index_point_to_previous_block(void){

    int index = 1;

    index = prevBlockIndex(index);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((index)), (((void *)0)), (_U_UINT)(56), UNITY_DISPLAY_STYLE_INT);

}

void test_prevBlockIndex_Index_point_to_previous_block_with_underflow(void){

  int index = 0;

  index = prevBlockIndex(index);

  UnityAssertEqualNumber((_U_SINT)((17)), (_U_SINT)((index)), (((void *)0)), (_U_UINT)(69), UNITY_DISPLAY_STYLE_INT);

}

void test_planInit_initialize_the_bufferHead_bufferTail_and_nextBufferHead(void){

  planInit();

  UnityAssertEqualNumber((_U_SINT)((bufferHead)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(83), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((nextBufferHead)), (_U_SINT)((1)), (((void *)0)), (_U_UINT)(84), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.position[0])), (((void *)0)), (_U_UINT)(85), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.position[1])), (((void *)0)), (_U_UINT)(86), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.position[2])), (((void *)0)), (_U_UINT)(87), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.previousUnitVec[0])), (((void *)0)), (_U_UINT)(88), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.previousUnitVec[1])), (((void *)0)), (_U_UINT)(89), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.previousUnitVec[2])), (((void *)0)), (_U_UINT)(90), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.previousNominalSpeed)), (((void *)0)), (_U_UINT)(91), UNITY_DISPLAY_STYLE_INT);

}

void test_getBlock_to_get_the_block_when_buffer_contained_block(void){

  bufferHead = nextBlockIndex(bufferHead);

  bufferHead = nextBlockIndex(bufferHead);

  bufferHead = nextBlockIndex(bufferHead);

  UnityAssertEqualNumber((_U_SINT)((3)), (_U_SINT)((bufferHead)), (((void *)0)), (_U_UINT)(107), UNITY_DISPLAY_STYLE_INT);

  block_t* currentBlock = getCurrentBlock();

  UnityAssertEqualNumber((_U_SINT)(_UP)((currentBlock)), (_U_SINT)(_UP)((&(blockBuffer[bufferTail]))), (((void *)0)), (_U_UINT)(109), UNITY_DISPLAY_STYLE_HEX32);

}

void test_getBlock_to_get_NULL_when_buffer_dont_have_any_block(void){

  block_t* currentBlock = getCurrentBlock();

  if ((((currentBlock)) == ((void *)0))) {} else {UnityFail( (((" Expected NULL"))), (_U_UINT)((_U_UINT)((_U_UINT)(122))));};

}

void test_discardCurrrentBlock_if_blockBuffer_contain_block_the_buffetTail_will_be_increased(void){

  bufferHead = nextBlockIndex(bufferHead);

  bufferHead = nextBlockIndex(bufferHead);

  bufferHead = nextBlockIndex(bufferHead);

  discardCurrentBlock();

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(140), UNITY_DISPLAY_STYLE_INT);

}

void test_discardCurrrentBlock_if_blockBuffer_doesnt_contain_block_the_buffetTail_will_remain(void){

  discardCurrentBlock();

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(154), UNITY_DISPLAY_STYLE_INT);

}



void xtest_blockConfig_StepX_is_100_StepY_is_75_StepZ_is_10_the_StepX_is_largest(void){

   bufferHead = nextBlockIndex(bufferHead);

   blockConfig(&blockBuffer[bufferTail],0x00,10000,75,10,80000,160000,80000);

   UnityAssertEqualNumber((_U_SINT)((10000)), (_U_SINT)((blockBuffer[bufferTail].stepEventCount)), (((void *)0)), (_U_UINT)(160), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((0x00)), (_U_SINT)((blockBuffer[bufferTail].directionBits)), (((void *)0)), (_U_UINT)(161), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((10000)), (_U_SINT)((blockBuffer[bufferTail].steps[0])), (((void *)0)), (_U_UINT)(162), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((75)), (_U_SINT)((blockBuffer[bufferTail].steps[1])), (((void *)0)), (_U_UINT)(163), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((10)), (_U_SINT)((blockBuffer[bufferTail].steps[2])), (((void *)0)), (_U_UINT)(164), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((blockBuffer[bufferTail].initialRate)), (((void *)0)), (_U_UINT)(165), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((160000)), (_U_SINT)((blockBuffer[bufferTail].nominalRate)), (((void *)0)), (_U_UINT)(166), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((blockBuffer[bufferTail].finalRate)), (((void *)0)), (_U_UINT)(167), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((3334)), (_U_SINT)((blockBuffer[bufferTail].accelerateUntil)), (((void *)0)), (_U_UINT)(168), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((6666)), (_U_SINT)((blockBuffer[bufferTail].deccelerateAfter)), (((void *)0)), (_U_UINT)(169), UNITY_DISPLAY_STYLE_INT);

}



void xtest_blockConfig_StepX_is_100_StepY_is_300_StepZ_is_200_StepY_is_largest(void){

   bufferHead = nextBlockIndex(bufferHead);

   blockConfig(&blockBuffer[bufferTail],0x00,100,300,200,6000,8000,6000);

   UnityAssertEqualNumber((_U_SINT)((300)), (_U_SINT)((blockBuffer[bufferTail].stepEventCount)), (((void *)0)), (_U_UINT)(175), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((0x00)), (_U_SINT)((blockBuffer[bufferTail].directionBits)), (((void *)0)), (_U_UINT)(176), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((blockBuffer[bufferTail].steps[0])), (((void *)0)), (_U_UINT)(177), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((300)), (_U_SINT)((blockBuffer[bufferTail].steps[1])), (((void *)0)), (_U_UINT)(178), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((200)), (_U_SINT)((blockBuffer[bufferTail].steps[2])), (((void *)0)), (_U_UINT)(179), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((6000)), (_U_SINT)((blockBuffer[bufferTail].initialRate)), (((void *)0)), (_U_UINT)(180), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((6000)), (_U_SINT)((blockBuffer[bufferTail].finalRate)), (((void *)0)), (_U_UINT)(181), UNITY_DISPLAY_STYLE_INT);

}



void xtest_blockConfig_StepX_is_100_StepY_is_300_StepZ_is_1000_StepZ_is_largest(void){

   bufferHead = nextBlockIndex(bufferHead);

   blockConfig(&blockBuffer[bufferTail],0x00,100,300,1000,7000,9000,7000);

   UnityAssertEqualNumber((_U_SINT)((1000)), (_U_SINT)((blockBuffer[bufferTail].stepEventCount)), (((void *)0)), (_U_UINT)(187), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((0x00)), (_U_SINT)((blockBuffer[bufferTail].directionBits)), (((void *)0)), (_U_UINT)(188), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((blockBuffer[bufferTail].steps[0])), (((void *)0)), (_U_UINT)(189), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((300)), (_U_SINT)((blockBuffer[bufferTail].steps[1])), (((void *)0)), (_U_UINT)(190), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((1000)), (_U_SINT)((blockBuffer[bufferTail].steps[2])), (((void *)0)), (_U_UINT)(191), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((7000)), (_U_SINT)((blockBuffer[bufferTail].initialRate)), (((void *)0)), (_U_UINT)(192), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((_U_SINT)((9000)), (_U_SINT)((blockBuffer[bufferTail].nominalRate)), (((void *)0)), (_U_UINT)(193), UNITY_DISPLAY_STYLE_INT);

}



void test_ceil_put_the_value_from_3point0_to_3point9_into_the_function_(void){

  UnityAssertEqualNumber((_U_SINT)((3)), (_U_SINT)((ceil(3.0))), (((void *)0)), (_U_UINT)(197), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.1))), (((void *)0)), (_U_UINT)(198), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.2))), (((void *)0)), (_U_UINT)(199), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.3))), (((void *)0)), (_U_UINT)(200), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.4))), (((void *)0)), (_U_UINT)(201), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.5))), (((void *)0)), (_U_UINT)(202), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.6))), (((void *)0)), (_U_UINT)(203), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.7))), (((void *)0)), (_U_UINT)(204), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.8))), (((void *)0)), (_U_UINT)(205), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(3.9))), (((void *)0)), (_U_UINT)(206), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((ceil(4.0))), (((void *)0)), (_U_UINT)(207), UNITY_DISPLAY_STYLE_INT);

}

void xtest_estimateAccelStepAndDeccelStep_when_the_initRate_is_80000_nominalRate_is_160000_finalRate_is_80000(void){

  block_t block;

  block.stepEventCount = 10000;

  block.initialRate = 80000;

  block.nominalRate = 160000;

  block.finalRate = 80000;

  estimateAccelStepAndDeccelStep(&block);

  UnityAssertEqualNumber((_U_SINT)((3334)), (_U_SINT)((block.accelerateUntil)), (((void *)0)), (_U_UINT)(231), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((6667)), (_U_SINT)((block.deccelerateAfter)), (((void *)0)), (_U_UINT)(232), UNITY_DISPLAY_STYLE_INT);

}

void xtest_estimateAccelStepAndDeccelStep_when_the_initRate_is_80000_nominalRate_is_200000_finalRate_is_800(void){

  block_t block;

  block.stepEventCount = 15000;

  block.initialRate = 80000;

  block.nominalRate = 200000;

  block.finalRate = 800;

  estimateAccelStepAndDeccelStep(&block);

  UnityAssertEqualNumber((_U_SINT)((5834)), (_U_SINT)((block.accelerateUntil)), (((void *)0)), (_U_UINT)(255), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((8056)), (_U_SINT)((block.deccelerateAfter)), (((void *)0)), (_U_UINT)(256), UNITY_DISPLAY_STYLE_INT);

}

void xtest_estimateAccelStepAndDeccelStep_when_the_initRate_is_800_nominalRate_is_200000_finalRate_is_800(void){

  block_t block;

  block.stepEventCount = 15000;

  block.initialRate = 800;

  block.nominalRate = 200000;

  block.finalRate = 800;

  estimateAccelStepAndDeccelStep(&block);

  UnityAssertEqualNumber((_U_SINT)((6945)), (_U_SINT)((block.accelerateUntil)), (((void *)0)), (_U_UINT)(279), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((8056)), (_U_SINT)((block.deccelerateAfter)), (((void *)0)), (_U_UINT)(280), UNITY_DISPLAY_STYLE_INT);

}

void xtest_estimateAccelStepAndDeccelStep_when_the_initRate_is_800_nominalRate_is_1000_finalRate_is_800(void){

  block_t block;

  block.stepEventCount = 15000;

  block.initialRate = 800;

  block.nominalRate = 1000;

  block.finalRate = 800;

  estimateAccelStepAndDeccelStep(&block);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((block.accelerateUntil)), (((void *)0)), (_U_UINT)(304), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((15000)), (_U_SINT)((block.deccelerateAfter)), (((void *)0)), (_U_UINT)(305), UNITY_DISPLAY_STYLE_INT);

}

void xtest_estimateAccelStepAndDeccelStep_when_the_initRate_is_80000_nominalRate_is_200000_finalRate_is_80000(void){

  block_t block;

  block.stepEventCount = 10000;

  block.initialRate = 80000;

  block.nominalRate = 200000;

  block.finalRate = 80000;

  estimateAccelStepAndDeccelStep(&block);

  UnityAssertEqualNumber((_U_SINT)((6945)), (_U_SINT)((block.accelerateUntil)), (((void *)0)), (_U_UINT)(329), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((8056)), (_U_SINT)((block.deccelerateAfter)), (((void *)0)), (_U_UINT)(330), UNITY_DISPLAY_STYLE_INT);

}



void test_convertXYZMovingDistanceToStepAndStoreToArray_when_x_is_100mm_y_is_200mm_and_z_is_300mm_the_function_will_return_x_is_20000_step_y_is_40000_step_x_is_60000_step(void){

  int32_t target[3];

  convertXYZMovingDistanceToStepAndStoreToArray(100,200,300,target);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((target[0])), (((void *)0)), (_U_UINT)(336), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((40000)), (_U_SINT)((target[1])), (((void *)0)), (_U_UINT)(337), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((60000)), (_U_SINT)((target[2])), (((void *)0)), (_U_UINT)(338), UNITY_DISPLAY_STYLE_INT);

}



void test_convertXYZMovingDistanceToStepAndStoreToArray_when_x_is_0mm_y_is_0mm_and_z_is_300mm_the_function_will_return_x_is_0_step_y_is_0_step_x_is_60000_step(void){

  int32_t target[3];

  convertXYZMovingDistanceToStepAndStoreToArray(0,0,300,target);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((target[0])), (((void *)0)), (_U_UINT)(344), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((target[1])), (((void *)0)), (_U_UINT)(345), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((60000)), (_U_SINT)((target[2])), (((void *)0)), (_U_UINT)(346), UNITY_DISPLAY_STYLE_INT);

}



void test_determineDirection_when_target_x_less_than_prev_x_target_the_direactionBits_is_equal_0x20(void){

  block_t *block = &blockBuffer[bufferTail];

  plExecutor.position[0] = 100000;

  plExecutor.position[1] = 0;

  plExecutor.position[2] = 0;

  int32_t target[3];

  convertXYZMovingDistanceToStepAndStoreToArray(100,200,300,target);

  block->directionBits |= determineDirection(target);

  UnityAssertEqualNumber((_U_SINT)((0x20)), (_U_SINT)((block->directionBits)), (((void *)0)), (_U_UINT)(357), UNITY_DISPLAY_STYLE_INT);

}



void test_determineDirection_when_target_y_less_than_prev_y_target_the_direactionBits_is_equal_0x40(void){

  block_t *block = &blockBuffer[bufferTail];

  plExecutor.position[0] = 0;

  plExecutor.position[1] = 100000;

  plExecutor.position[2] = 0;

  int32_t target[3];

  convertXYZMovingDistanceToStepAndStoreToArray(100,200,300,target);

  block->directionBits |= determineDirection(target);

  UnityAssertEqualNumber((_U_SINT)((0x40)), (_U_SINT)((block->directionBits)), (((void *)0)), (_U_UINT)(368), UNITY_DISPLAY_STYLE_INT);

}



void test_determineDirection_when_target_z_less_than_prev_z_target_the_direactionBits_is_equal_0x80(void){

  block_t *block = &blockBuffer[bufferTail];

  plExecutor.position[0] = 0;

  plExecutor.position[1] = 0;

  plExecutor.position[2] = 100000;

  int32_t target[3];

  convertXYZMovingDistanceToStepAndStoreToArray(100,200,300,target);

  block->directionBits |= determineDirection(target);

  UnityAssertEqualNumber((_U_SINT)((0x80)), (_U_SINT)((block->directionBits)), (((void *)0)), (_U_UINT)(379), UNITY_DISPLAY_STYLE_INT);

}



void test_determineDirection_when_target_xyz_less_than_prev_xyz_target_the_direactionBits_is_equal_0x80(void){

  block_t *block = &blockBuffer[bufferTail];

  plExecutor.position[0] = 100000;

  plExecutor.position[1] = 100000;

  plExecutor.position[2] = 100000;

  int32_t target[3];

  convertXYZMovingDistanceToStepAndStoreToArray(100,200,300,target);

  block->directionBits |= determineDirection(target);

  UnityAssertEqualNumber((_U_SINT)((0xe0)), (_U_SINT)((block->directionBits)), (((void *)0)), (_U_UINT)(390), UNITY_DISPLAY_STYLE_INT);

}



void test_labs_put_a_negative_value_into_function_it_should_the_absolute_value_(void){

  UnityAssertEqualNumber((_U_SINT)(_US32)((1000L)), (_U_SINT)(_US32)((_labs(1000L))), (((void *)0)), (_U_UINT)(394), UNITY_DISPLAY_STYLE_INT32);

  UnityAssertEqualNumber((_U_SINT)(_US32)((1000L)), (_U_SINT)(_US32)((_labs(-1000L))), (((void *)0)), (_U_UINT)(395), UNITY_DISPLAY_STYLE_INT32);

  UnityAssertEqualNumber((_U_SINT)(_US32)((0)), (_U_SINT)(_US32)((_labs(0))), (((void *)0)), (_U_UINT)(396), UNITY_DISPLAY_STYLE_INT32);

  UnityAssertEqualNumber((_U_SINT)(_US32)((9022233)), (_U_SINT)(_US32)((_labs(-9022233))), (((void *)0)), (_U_UINT)(397), UNITY_DISPLAY_STYLE_INT32);

  UnityAssertEqualNumber((_U_SINT)(_US32)((9022233)), (_U_SINT)(_US32)((_labs(9022233))), (((void *)0)), (_U_UINT)(398), UNITY_DISPLAY_STYLE_INT32);

  UnityAssertEqualNumber((_U_SINT)(_US32)((80000)), (_U_SINT)(_US32)((_labs(-80000))), (((void *)0)), (_U_UINT)(399), UNITY_DISPLAY_STYLE_INT32);

}



void test_calculateXYZaxisSteps_to_obtain_the_moving_step_when_the_target_x_is_100_targer_y_is_200_and_target_z_is_300(void){

  block_t *block = &blockBuffer[bufferHead];

  plExecutor.position[0] = 0;

  plExecutor.position[1] = 0;

  plExecutor.position[2] = 0;

  int32_t target[3];

  convertXYZMovingDistanceToStepAndStoreToArray(100,200,300,target);

  calculateXYZaxisSteps(block,target);

  UnityAssertEqualNumber((_U_SINT)((block->steps[0])), (_U_SINT)((20000)), (((void *)0)), (_U_UINT)(410), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((block->steps[1])), (_U_SINT)((40000)), (((void *)0)), (_U_UINT)(411), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((block->steps[2])), (_U_SINT)((60000)), (((void *)0)), (_U_UINT)(412), UNITY_DISPLAY_STYLE_INT);

}



void test_calculateXYZaxisSteps_to_obtain_the_moving_step_when_the_target_x_is_100_targer_y_is_200_and_target_z_is_300_the_prev_target_xyz_are_100000(void){

  block_t *block = &blockBuffer[bufferHead];

  plExecutor.position[0] = 100000;

  plExecutor.position[1] = 100000;

  plExecutor.position[2] = 100000;

  int32_t target[3];

  convertXYZMovingDistanceToStepAndStoreToArray(100,200,300,target);

  calculateXYZaxisSteps(block,target);

  UnityAssertEqualNumber((_U_SINT)(_US32)((block->steps[0])), (_U_SINT)(_US32)((80000)), (((void *)0)), (_U_UINT)(423), UNITY_DISPLAY_STYLE_INT32);

  UnityAssertEqualNumber((_U_SINT)(_US32)((block->steps[1])), (_U_SINT)(_US32)((60000)), (((void *)0)), (_U_UINT)(424), UNITY_DISPLAY_STYLE_INT32);

  UnityAssertEqualNumber((_U_SINT)(_US32)((block->steps[2])), (_U_SINT)(_US32)((40000)), (((void *)0)), (_U_UINT)(425), UNITY_DISPLAY_STYLE_INT32);

}





void test_getHighestStepsInXYZsteps_when_the_X_steps_is_20000_Y_steps_is_40000_and_Z_steps_is_60000_the_max_steps_is_60000(void){

  block_t *block = &blockBuffer[bufferHead];

  int32_t target[3];

  convertXYZMovingDistanceToStepAndStoreToArray(100,200,300,target);

  calculateXYZaxisSteps(block,target);

  UnityAssertEqualNumber((_U_SINT)((60000)), (_U_SINT)((getHighestStepsInXYZsteps(block))), (((void *)0)), (_U_UINT)(434), UNITY_DISPLAY_STYLE_INT);

}



void test_getHighestStepsInXYZsteps_when_the_X_steps_is_20000_Y_steps_is_100000_and_Z_steps_is_60000_the_max_steps_is_100000(void){

  block_t *block = &blockBuffer[bufferHead];

  int32_t target[3];

  convertXYZMovingDistanceToStepAndStoreToArray(100,500,300,target);

  calculateXYZaxisSteps(block,target);

  UnityAssertEqualNumber((_U_SINT)((100000)), (_U_SINT)((getHighestStepsInXYZsteps(block))), (((void *)0)), (_U_UINT)(442), UNITY_DISPLAY_STYLE_INT);

}



void test_getHighestStepsInXYZsteps_when_the_X_steps_is_120000_Y_steps_is_100000_and_Z_steps_is_60000_the_max_steps_is_120000(void){

  block_t *block = &blockBuffer[bufferHead];

  int32_t target[3];

  convertXYZMovingDistanceToStepAndStoreToArray(600,500,300,target);

  calculateXYZaxisSteps(block,target);

  UnityAssertEqualNumber((_U_SINT)((120000)), (_U_SINT)((getHighestStepsInXYZsteps(block))), (((void *)0)), (_U_UINT)(450), UNITY_DISPLAY_STYLE_INT);

}





void test_planLineBlock_initially_plan_the_motor_move_in_x_is_100mm_y_is_200mm_and_z_is_300mm_with_3000_mm_per_min(void){

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(455), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((bufferHead)), (((void *)0)), (_U_UINT)(456), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((nextBufferHead)), (((void *)0)), (_U_UINT)(457), UNITY_DISPLAY_STYLE_INT);

  planLineBlock(100,200,300,3000,0);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(459), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((bufferHead)), (((void *)0)), (_U_UINT)(460), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((2)), (_U_SINT)((nextBufferHead)), (((void *)0)), (_U_UINT)(461), UNITY_DISPLAY_STYLE_INT);

  block_t *block = &blockBuffer[bufferTail];

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->directionBits)), (((void *)0)), (_U_UINT)(463), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block->steps[0])), (((void *)0)), (_U_UINT)(464), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((40000)), (_U_SINT)((block->steps[1])), (((void *)0)), (_U_UINT)(465), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((60000)), (_U_SINT)((block->steps[2])), (((void *)0)), (_U_UINT)(466), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((60000)), (_U_SINT)((block->stepEventCount)), (((void *)0)), (_U_UINT)(467), UNITY_DISPLAY_STYLE_INT);

}



void test_getDeltaInMilliMeter_put_several_input_to_test(void){

  UnityAssertFloatsWithin((_UF)((_UF)((100)) * (_UF)(0.00001f)), (_UF)((_UF)((100))), (_UF)((_UF)((getDeltaInMilliMeter(20000,0)))), ((((void *)0))), (_U_UINT)((_U_UINT)(471)));

  UnityAssertFloatsWithin((_UF)((_UF)((10)) * (_UF)(0.00001f)), (_UF)((_UF)((10))), (_UF)((_UF)((getDeltaInMilliMeter(2000,0)))), ((((void *)0))), (_U_UINT)((_U_UINT)(472)));

  UnityAssertFloatsWithin((_UF)((_UF)((-400)) * (_UF)(0.00001f)), (_UF)((_UF)((-400))), (_UF)((_UF)((getDeltaInMilliMeter(20000,100000)))), ((((void *)0))), (_U_UINT)((_U_UINT)(473)));

  UnityAssertFloatsWithin((_UF)((_UF)((1)) * (_UF)(0.00001f)), (_UF)((_UF)((1))), (_UF)((_UF)((getDeltaInMilliMeter(200,0)))), ((((void *)0))), (_U_UINT)((_U_UINT)(474)));

  UnityAssertFloatsWithin((_UF)((_UF)((-0.1)) * (_UF)(0.00001f)), (_UF)((_UF)((-0.1))), (_UF)((_UF)((getDeltaInMilliMeter(20,40)))), ((((void *)0))), (_U_UINT)((_U_UINT)(475)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.1)) * (_UF)(0.00001f)), (_UF)((_UF)((0.1))), (_UF)((_UF)((getDeltaInMilliMeter(20,0)))), ((((void *)0))), (_U_UINT)((_U_UINT)(476)));

}





void test_calculateXYZDeltaInMilliMeterAndStoreToArray_targetXYZ_are_20000steps_40000steps_and_60000steps_and_the_prevTargetXYZ_are_0_0_and_0(void){

  float delta[3];

  int32_t target[3];

  plExecutor.position[0] = 0;

  plExecutor.position[1] = 0;

  plExecutor.position[2] = 0;

  target[0] = 20000;

  target[1] = 40000;

  target[2] = 60000;

  calculateXYZDeltaInMilliMeterAndStoreToArray(target,delta);

  UnityAssertFloatsWithin((_UF)((_UF)((delta[0])) * (_UF)(0.00001f)), (_UF)((_UF)((delta[0]))), (_UF)((_UF)((100))), ((((void *)0))), (_U_UINT)((_U_UINT)(490)));

  UnityAssertFloatsWithin((_UF)((_UF)((delta[1])) * (_UF)(0.00001f)), (_UF)((_UF)((delta[1]))), (_UF)((_UF)((200))), ((((void *)0))), (_U_UINT)((_U_UINT)(491)));

  UnityAssertFloatsWithin((_UF)((_UF)((delta[2])) * (_UF)(0.00001f)), (_UF)((_UF)((delta[2]))), (_UF)((_UF)((300))), ((((void *)0))), (_U_UINT)((_U_UINT)(492)));

}



void test_calculateXYZDeltaInMilliMeterAndStoreToArray_targetXYZ_are_20000steps_40000steps_and_60000steps_and_the_prevTargetXYZ_are_100000_100000_and_100000(void){

  float delta[3];

  int32_t target[3];

  plExecutor.position[0] = 100000;

  plExecutor.position[1] = 100000;

  plExecutor.position[2] = 100000;

  target[0] = 20000;

  target[1] = 40000;

  target[2] = 60000;

  calculateXYZDeltaInMilliMeterAndStoreToArray(target,delta);

  UnityAssertFloatsWithin((_UF)((_UF)((delta[0])) * (_UF)(0.00001f)), (_UF)((_UF)((delta[0]))), (_UF)((_UF)((-400))), ((((void *)0))), (_U_UINT)((_U_UINT)(505)));

  UnityAssertFloatsWithin((_UF)((_UF)((delta[1])) * (_UF)(0.00001f)), (_UF)((_UF)((delta[1]))), (_UF)((_UF)((-300))), ((((void *)0))), (_U_UINT)((_U_UINT)(506)));

  UnityAssertFloatsWithin((_UF)((_UF)((delta[2])) * (_UF)(0.00001f)), (_UF)((_UF)((delta[2]))), (_UF)((_UF)((-200))), ((((void *)0))), (_U_UINT)((_U_UINT)(507)));

}



void test_getVectorLength_when_deltaX_is_100_deltaY_is_200_and_deltaZ_is_300_the_function_should_return_374(void){

  float delta[3];

  delta[0] = 100;

  delta[1] = 200;

  delta[2] = 300;

  UnityAssertFloatsWithin((_UF)((_UF)((374)) * (_UF)(0.00001f)), (_UF)((_UF)((374))), (_UF)((_UF)(((int32_t)getVectorLength(delta)))), ((((void *)0))), (_U_UINT)((_U_UINT)(515)));

}



void test_getVectorLength_when_deltaX_is_0_deltaY_is_400_and_deltaZ_is_300_the_function_should_return_500(void){

  float delta[3];

  delta[0] = 0;

  delta[1] = 400;

  delta[2] = 300;

  UnityAssertFloatsWithin((_UF)((_UF)((500)) * (_UF)(0.00001f)), (_UF)((_UF)((500))), (_UF)((_UF)(((int32_t)getVectorLength(delta)))), ((((void *)0))), (_U_UINT)((_U_UINT)(523)));

}



void test_getInverse_(void){

  block_t *block = &blockBuffer[bufferHead];

  float delta[3];

  delta[0] = 0;

  delta[1] = 400;

  delta[2] = 300;

  block->millimeters = getVectorLength(delta);

  block->inverseMillimeters = getInverse(block->millimeters);

  UnityAssertFloatsWithin((_UF)((_UF)((0.002)) * (_UF)(0.00001f)), (_UF)((_UF)((0.002))), (_UF)((_UF)((block->inverseMillimeters))), ((((void *)0))), (_U_UINT)((_U_UINT)(534)));



}

void test_estimateNominalRateAndSpeed_when_the_vectorLenght_is_500mm_and_MaxStep_is_80000step(void){

  block_t *block = &blockBuffer[bufferHead];

  int32_t target[3];

  float delta[3];

  target[0] = 80000;

  target[1] = 60000;

  target[2] = 0;

  calculateXYZaxisSteps(block,target);

  block->stepEventCount = getHighestStepsInXYZsteps(block);

  calculateXYZDeltaInMilliMeterAndStoreToArray(target,delta);

  block->millimeters = getVectorLength(delta);

  block->inverseMillimeters = getInverse(block->millimeters);

  estimateNominalRateAndSpeed(block,100,0);

  UnityAssertEqualNumber((_U_SINT)((16000)), (_U_SINT)((block->nominalRate)), (((void *)0)), (_U_UINT)(572), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((block->nominalSpeed)), (((void *)0)), (_U_UINT)(573), UNITY_DISPLAY_STYLE_INT);

}

void test_estimateNominalRateAndSpeed_when_the_vectorLenght_is_509_point_902mm_and_MaxStep_is_80000step(void){

  block_t *block = &blockBuffer[bufferHead];

  int32_t target[3];

  float delta[3];

  plExecutor.position[0] = 100000;

  plExecutor.position[1] = 20000;

  plExecutor.position[2] = 20000;

  target[0] = 60000;

  target[1] = 80000;

  target[2] = 20000;

  calculateXYZaxisSteps(block,target);

  block->stepEventCount = getHighestStepsInXYZsteps(block);

  calculateXYZDeltaInMilliMeterAndStoreToArray(target,delta);

  block->millimeters = getVectorLength(delta);

  block->inverseMillimeters = getInverse(block->millimeters);

  estimateNominalRateAndSpeed(block,100,0);

  UnityAssertFloatsWithin((_UF)((_UF)((360.555)) * (_UF)(0.00001f)), (_UF)((_UF)((360.555))), (_UF)((_UF)((block->millimeters))), ((((void *)0))), (_U_UINT)((_U_UINT)(615)));

  UnityAssertFloatsWithin((_UF)((0.00002)), (_UF)((0.0027735)), (_UF)((block->inverseMillimeters)), (((void *)0)), (_U_UINT)(616));

  UnityAssertEqualNumber((_U_SINT)((40000)), (_U_SINT)((block->steps[0])), (((void *)0)), (_U_UINT)(617), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((60000)), (_U_SINT)((block->steps[1])), (((void *)0)), (_U_UINT)(618), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->steps[2])), (((void *)0)), (_U_UINT)(619), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((60000)), (_U_SINT)((block->stepEventCount)), (((void *)0)), (_U_UINT)(620), UNITY_DISPLAY_STYLE_INT);

  UnityAssertFloatsWithin((_UF)((_UF)((delta[0])) * (_UF)(0.00001f)), (_UF)((_UF)((delta[0]))), (_UF)((_UF)((-200))), ((((void *)0))), (_U_UINT)((_U_UINT)(621)));

  UnityAssertFloatsWithin((_UF)((_UF)((delta[1])) * (_UF)(0.00001f)), (_UF)((_UF)((delta[1]))), (_UF)((_UF)((300))), ((((void *)0))), (_U_UINT)((_U_UINT)(622)));

  UnityAssertFloatsWithin((_UF)((_UF)((delta[2])) * (_UF)(0.00001f)), (_UF)((_UF)((delta[2]))), (_UF)((_UF)((0))), ((((void *)0))), (_U_UINT)((_U_UINT)(623)));

  UnityAssertEqualNumber((_U_SINT)((16642)), (_U_SINT)((block->nominalRate)), (((void *)0)), (_U_UINT)(624), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((block->nominalSpeed)), (((void *)0)), (_U_UINT)(625), UNITY_DISPLAY_STYLE_INT);

}



void test_planLineBlock_move_in_the_x_axis_is_300mm_the_y_axis_is_200mm_and_z_is_100mm_(void){

  block_t *block = &blockBuffer[bufferHead];

  plExecutor.position[0] = 100000;

  plExecutor.position[1] = 20000;

  plExecutor.position[2] = 20000;

  planLineBlock(300,400,100,100,0);

  UnityAssertEqualNumber((_U_SINT)((0x20)), (_U_SINT)((block->directionBits)), (((void *)0)), (_U_UINT)(634), UNITY_DISPLAY_STYLE_INT);

  UnityAssertFloatsWithin((_UF)((_UF)((360.555)) * (_UF)(0.00001f)), (_UF)((_UF)((360.555))), (_UF)((_UF)((block->millimeters))), ((((void *)0))), (_U_UINT)((_U_UINT)(635)));

  UnityAssertFloatsWithin((_UF)((0.00002)), (_UF)((0.0027735)), (_UF)((block->inverseMillimeters)), (((void *)0)), (_U_UINT)(636));

  UnityAssertEqualNumber((_U_SINT)((40000)), (_U_SINT)((block->steps[0])), (((void *)0)), (_U_UINT)(637), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((60000)), (_U_SINT)((block->steps[1])), (((void *)0)), (_U_UINT)(638), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->steps[2])), (((void *)0)), (_U_UINT)(639), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((60000)), (_U_SINT)((block->stepEventCount)), (((void *)0)), (_U_UINT)(640), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((16642)), (_U_SINT)((block->nominalRate)), (((void *)0)), (_U_UINT)(641), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((block->nominalSpeed)), (((void *)0)), (_U_UINT)(642), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(643), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((bufferHead)), (((void *)0)), (_U_UINT)(644), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((2)), (_U_SINT)((nextBufferHead)), (((void *)0)), (_U_UINT)(645), UNITY_DISPLAY_STYLE_INT);

}



void test_getUnitVector_the_deltaX_is_300_deltaY_is_400_and_deltaZ_is_100_and_vectorLength_is_509pt109(void){

  int32_t target[3];

  float delta[3];

  plExecutor.position[0] = 0;

  plExecutor.position[1] = 0;

  plExecutor.position[2] = 0;

  target[0] = 60000;

  target[1] = 80000;

  target[2] = 20000;

  calculateXYZDeltaInMilliMeterAndStoreToArray(target,delta);

  block_t *block = &blockBuffer[bufferHead];

  block->millimeters = getVectorLength(delta);

  block->inverseMillimeters = getInverse(block->millimeters);

  UnityAssertFloatsWithin((_UF)((_UF)((delta[0])) * (_UF)(0.00001f)), (_UF)((_UF)((delta[0]))), (_UF)((_UF)((300))), ((((void *)0))), (_U_UINT)((_U_UINT)(661)));

  UnityAssertFloatsWithin((_UF)((_UF)((delta[1])) * (_UF)(0.00001f)), (_UF)((_UF)((delta[1]))), (_UF)((_UF)((400))), ((((void *)0))), (_U_UINT)((_U_UINT)(662)));

  UnityAssertFloatsWithin((_UF)((_UF)((delta[2])) * (_UF)(0.00001f)), (_UF)((_UF)((delta[2]))), (_UF)((_UF)((100))), ((((void *)0))), (_U_UINT)((_U_UINT)(663)));

  UnityAssertFloatsWithin((_UF)((0.00002)), (_UF)((0.0019611)), (_UF)((block->inverseMillimeters)), (((void *)0)), (_U_UINT)(664));

  UnityAssertFloatsWithin((_UF)((_UF)((0.58835)) * (_UF)(0.00001f)), (_UF)((_UF)((0.58835))), (_UF)((_UF)((getUnitVector(block,delta,0)))), ((((void *)0))), (_U_UINT)((_U_UINT)(665)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.78446)) * (_UF)(0.00001f)), (_UF)((_UF)((0.78446))), (_UF)((_UF)((getUnitVector(block,delta,1)))), ((((void *)0))), (_U_UINT)((_U_UINT)(666)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.196116)) * (_UF)(0.00001f)), (_UF)((_UF)((0.196116))), (_UF)((_UF)((getUnitVector(block,delta,2)))), ((((void *)0))), (_U_UINT)((_U_UINT)(667)));



}

void test_calculateUnitVectorForXYZaxisAndStoreToArray_the_deltaX_is_300_deltaY_is_400_and_deltaZ_is_100_and_vectorLength_is_509pt109(void){

  int32_t target[3];

  float delta[3];

  float unitVector[3];

  plExecutor.position[0] = 0;

  plExecutor.position[1] = 0;

  plExecutor.position[2] = 0;

  target[0] = 60000;

  target[1] = 80000;

  target[2] = 20000;

  calculateXYZDeltaInMilliMeterAndStoreToArray(target,delta);

  block_t *block = &blockBuffer[bufferHead];

  block->millimeters = getVectorLength(delta);

  block->inverseMillimeters = getInverse(block->millimeters);

  calculateUnitVectorForXYZaxisAndStoreToArray(block,delta,unitVector);

  UnityAssertFloatsWithin((_UF)((0.00002)), (_UF)((0.0019611)), (_UF)((block->inverseMillimeters)), (((void *)0)), (_U_UINT)(701));

  UnityAssertFloatsWithin((_UF)((_UF)((0.58835)) * (_UF)(0.00001f)), (_UF)((_UF)((0.58835))), (_UF)((_UF)((unitVector[0]))), ((((void *)0))), (_U_UINT)((_U_UINT)(702)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.78446)) * (_UF)(0.00001f)), (_UF)((_UF)((0.78446))), (_UF)((_UF)((unitVector[1]))), ((((void *)0))), (_U_UINT)((_U_UINT)(703)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.196116)) * (_UF)(0.00001f)), (_UF)((_UF)((0.196116))), (_UF)((_UF)((unitVector[2]))), ((((void *)0))), (_U_UINT)((_U_UINT)(704)));

}



void test_planLineBlock_put_Moving_X_axis_is_300_Y_axis_is_400_and_Z_axis_is_100_(void){

  block_t *block = &blockBuffer[bufferHead];

  planLineBlock(300,400,100,100,0);

  UnityAssertEqualNumber((_U_SINT)((0x00)), (_U_SINT)((block->directionBits)), (((void *)0)), (_U_UINT)(710), UNITY_DISPLAY_STYLE_INT);

  UnityAssertFloatsWithin((_UF)((_UF)((509.902)) * (_UF)(0.00001f)), (_UF)((_UF)((509.902))), (_UF)((_UF)((block->millimeters))), ((((void *)0))), (_U_UINT)((_U_UINT)(711)));

  UnityAssertFloatsWithin((_UF)((0.00002)), (_UF)((0.0019611)), (_UF)((block->inverseMillimeters)), (((void *)0)), (_U_UINT)(712));

  UnityAssertEqualNumber((_U_SINT)((60000)), (_U_SINT)((block->steps[0])), (((void *)0)), (_U_UINT)(713), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block->steps[1])), (((void *)0)), (_U_UINT)(714), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block->steps[2])), (((void *)0)), (_U_UINT)(715), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block->stepEventCount)), (((void *)0)), (_U_UINT)(716), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((15690)), (_U_SINT)((block->nominalRate)), (((void *)0)), (_U_UINT)(717), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((block->nominalSpeed)), (((void *)0)), (_U_UINT)(718), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(719), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((bufferHead)), (((void *)0)), (_U_UINT)(720), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((2)), (_U_SINT)((nextBufferHead)), (((void *)0)), (_U_UINT)(721), UNITY_DISPLAY_STYLE_INT);

  UnityAssertFloatsWithin((_UF)((_UF)((0.58835)) * (_UF)(0.00001f)), (_UF)((_UF)((0.58835))), (_UF)((_UF)((plExecutor.previousUnitVec[0]))), ((((void *)0))), (_U_UINT)((_U_UINT)(722)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.78446)) * (_UF)(0.00001f)), (_UF)((_UF)((0.78446))), (_UF)((_UF)((plExecutor.previousUnitVec[1]))), ((((void *)0))), (_U_UINT)((_U_UINT)(723)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.196116)) * (_UF)(0.00001f)), (_UF)((_UF)((0.196116))), (_UF)((_UF)((plExecutor.previousUnitVec[2]))), ((((void *)0))), (_U_UINT)((_U_UINT)(724)));

  UnityAssertEqualNumber((_U_SINT)((60000)), (_U_SINT)((plExecutor.position[0])), (((void *)0)), (_U_UINT)(725), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((plExecutor.position[1])), (((void *)0)), (_U_UINT)(726), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((plExecutor.position[2])), (((void *)0)), (_U_UINT)(727), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((plExecutor.previousNominalSpeed)), (((void *)0)), (_U_UINT)(728), UNITY_DISPLAY_STYLE_INT);

}

void test_getRampStep_initialRate_is_80000_and_targeRate_is_160000_and_acceleration_is_36000mm_per_min_square(void){



  block_t *block = &blockBuffer[bufferHead];

  float accelStep = _ceil(getRampStep(80000,100000,10*60*60*200));

  float deccelStep = _ceil(getRampStep(100000,80000,-10*60*60*200));

  UnityAssertEqualNumber((_U_SINT)((250)), (_U_SINT)((accelStep)), (((void *)0)), (_U_UINT)(754), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((250)), (_U_SINT)((deccelStep)), (((void *)0)), (_U_UINT)(755), UNITY_DISPLAY_STYLE_INT);

}



void test_getNominalStep_the_function_is_tested_by_putting_some_value(void){

  UnityAssertEqualNumber((_U_SINT)((5000)), (_U_SINT)((getNominalStep(8000,1000,2000))), (((void *)0)), (_U_UINT)(759), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((2000)), (_U_SINT)((getNominalStep(8000,1000,5000))), (((void *)0)), (_U_UINT)(760), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((getNominalStep(8000,3000,5000))), (((void *)0)), (_U_UINT)(761), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((-3000)), (_U_SINT)((getNominalStep(8000,3000,8000))), (((void *)0)), (_U_UINT)(762), UNITY_DISPLAY_STYLE_INT);

}

void test_getIntersectionDistance_put_initialRate_and_finalRate_is_equal_80000_and_distance_is_change(void){

  block_t *block = &blockBuffer[bufferHead];

  block->initialRate = 80000;

  block->finalRate = 80000;

  block->stepEventCount = 300;



  UnityAssertFloatsWithin((_UF)((_UF)((150)) * (_UF)(0.00001f)), (_UF)((_UF)((150))), (_UF)((_UF)((_ceil(getIntersectionDistance(block))))), ((((void *)0))), (_U_UINT)((_U_UINT)(784)));

  block->stepEventCount = 500;

  UnityAssertFloatsWithin((_UF)((_UF)((250)) * (_UF)(0.00001f)), (_UF)((_UF)((250))), (_UF)((_UF)((_ceil(getIntersectionDistance(block))))), ((((void *)0))), (_U_UINT)((_U_UINT)(786)));

  block->stepEventCount = 200;

  UnityAssertFloatsWithin((_UF)((_UF)((100)) * (_UF)(0.00001f)), (_UF)((_UF)((100))), (_UF)((_UF)((_ceil(getIntersectionDistance(block))))), ((((void *)0))), (_U_UINT)((_U_UINT)(788)));

  block->stepEventCount = 50;

  UnityAssertFloatsWithin((_UF)((_UF)((25)) * (_UF)(0.00001f)), (_UF)((_UF)((25))), (_UF)((_UF)((_ceil(getIntersectionDistance(block))))), ((((void *)0))), (_U_UINT)((_U_UINT)(790)));

  block->stepEventCount = 15;

  UnityAssertFloatsWithin((_UF)((_UF)((8)) * (_UF)(0.00001f)), (_UF)((_UF)((8))), (_UF)((_UF)((_ceil(getIntersectionDistance(block))))), ((((void *)0))), (_U_UINT)((_U_UINT)(792)));





}

void xtest_getIntersectionDistance_when_finalRate_is_much_higher_than_initialRate_(void){

    block_t *block = &blockBuffer[bufferHead];

  block->initialRate = 80000;

  block->stepEventCount = 500;

  block->finalRate = 90000;

  UnityAssertFloatsWithin((_UF)((_UF)((310)) * (_UF)(0.00001f)), (_UF)((_UF)((310))), (_UF)((_UF)((_ceil(getIntersectionDistance(block))))), ((((void *)0))), (_U_UINT)((_U_UINT)(818)));

  block->finalRate = 100000;

  UnityAssertFloatsWithin((_UF)((_UF)((375)) * (_UF)(0.00001f)), (_UF)((_UF)((375))), (_UF)((_UF)((_ceil(getIntersectionDistance(block))))), ((((void *)0))), (_U_UINT)((_U_UINT)(820)));

  block->finalRate = 130000;

  UnityAssertFloatsWithin((_UF)((_UF)((500)) * (_UF)(0.00001f)), (_UF)((_UF)((500))), (_UF)((_UF)((_ceil(getIntersectionDistance(block))))), ((((void *)0))), (_U_UINT)((_U_UINT)(822)));

  block->finalRate = 150000;

  UnityAssertFloatsWithin((_UF)((_UF)((500)) * (_UF)(0.00001f)), (_UF)((_UF)((500))), (_UF)((_UF)((_ceil(getIntersectionDistance(block))))), ((((void *)0))), (_U_UINT)((_U_UINT)(824)));

}

void test_getIntersectionDistance_when_finalRate_is_much_less_than_initialRate_(void){

  block_t *block = &blockBuffer[bufferHead];

  block->finalRate = 80000;

  block->stepEventCount = 500;

  block->initialRate = 90000;

  UnityAssertFloatsWithin((_UF)((_UF)((191)) * (_UF)(0.00001f)), (_UF)((_UF)((191))), (_UF)((_UF)((_ceil(getIntersectionDistance(block))))), ((((void *)0))), (_U_UINT)((_U_UINT)(849)));

  block->initialRate = 100000;

  UnityAssertFloatsWithin((_UF)((_UF)((125)) * (_UF)(0.00001f)), (_UF)((_UF)((125))), (_UF)((_UF)((_ceil(getIntersectionDistance(block))))), ((((void *)0))), (_U_UINT)((_U_UINT)(851)));

  block->initialRate = 130000;

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((_ceil(getIntersectionDistance(block))))), ((((void *)0))), (_U_UINT)((_U_UINT)(853)));

  block->initialRate = 150000;

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((_ceil(getIntersectionDistance(block))))), ((((void *)0))), (_U_UINT)((_U_UINT)(855)));

}

void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_and_exitFactor_is_0pts8(void){

  block_t *block = &blockBuffer[bufferHead];

  block->nominalRate = 100000;

  block->stepEventCount = 1000;

  blockMovingExecutionPlaning(block,0.8,0.8);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(878), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(879), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((250)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(880), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((750)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(881), UNITY_DISPLAY_STYLE_INT);

}

void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts5_and_exitFactor_is_0pts8(void){

  block_t *block = &blockBuffer[bufferHead];

  block->nominalRate = 100000;

  block->stepEventCount = 1000;

  blockMovingExecutionPlaning(block,0.5,0.8);

  UnityAssertEqualNumber((_U_SINT)((50000)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(905), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(906), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((521)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(907), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((750)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(908), UNITY_DISPLAY_STYLE_INT);

}

void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts8_and_exitFactor_is_0pts5(void){

  block_t *block = &blockBuffer[bufferHead];

  block->nominalRate = 100000;

  block->stepEventCount = 1000;

  blockMovingExecutionPlaning(block,0.8,0.5);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(932), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((50000)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(933), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((250)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(934), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((479)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(935), UNITY_DISPLAY_STYLE_INT);

}

void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts5_and_exitFactor_is_0pts5(void){

  block_t *block = &blockBuffer[bufferHead];

  block->nominalRate = 100000;

  block->stepEventCount = 1000;

  blockMovingExecutionPlaning(block,0.5,0.5);

  UnityAssertEqualNumber((_U_SINT)((50000)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(960), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((50000)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(961), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((500)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(962), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((500)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(963), UNITY_DISPLAY_STYLE_INT);

}

void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts5_and_exitFactor_is_0pts2(void){

  block_t *block = &blockBuffer[bufferHead];

  block->nominalRate = 100000;

  block->stepEventCount = 1000;

  blockMovingExecutionPlaning(block,0.5,0.2);

  UnityAssertEqualNumber((_U_SINT)((50000)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(987), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(988), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((428)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(989), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((428)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(990), UNITY_DISPLAY_STYLE_INT);

}

void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts5_and_exitFactor_is_0pts01(void){

  block_t *block = &blockBuffer[bufferHead];

  block->nominalRate = 100000;

  block->stepEventCount = 1000;

  blockMovingExecutionPlaning(block,0.5,0.01);

  UnityAssertEqualNumber((_U_SINT)((50000)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(1014), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1000)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(1015), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((414)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(1016), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((414)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(1017), UNITY_DISPLAY_STYLE_INT);

}

void test_blockMovingExecutionPlaning_nominalRate_is_150000_entryFactor_is_1pts3_and_exitFactor_is_0pts01(void){

  block_t *block = &blockBuffer[bufferHead];

  block->nominalRate = 150000;

  block->stepEventCount = 1000;

  blockMovingExecutionPlaning(block,0.867,0.0067);

  UnityAssertEqualNumber((_U_SINT)((130050)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(1041), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1005)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(1042), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(1043), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(1044), UNITY_DISPLAY_STYLE_INT);

}

void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts2_and_exitFactor_is_0pts5(void){

  block_t *block = &blockBuffer[bufferHead];

  block->nominalRate = 100000;

  block->stepEventCount = 1000;

  blockMovingExecutionPlaning(block,0.2,0.5);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(1068), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((50000)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(1069), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((573)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(1070), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((573)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(1071), UNITY_DISPLAY_STYLE_INT);

}

void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts01_and_exitFactor_is_0pts5(void){

  block_t *block = &blockBuffer[bufferHead];

  block->nominalRate = 100000;

  block->stepEventCount = 1000;

  blockMovingExecutionPlaning(block,0.01,0.5);

  UnityAssertEqualNumber((_U_SINT)((1000)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(1096), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((50000)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(1097), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((587)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(1098), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((587)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(1099), UNITY_DISPLAY_STYLE_INT);

}

void test_blockMovingExecutionPlaning_nominalRate_is_150000_entryFactor_is_0pts01_and_exitFactor_is_0pts9(void){

  block_t *block = &blockBuffer[bufferHead];

  block->nominalRate = 150000;

  block->stepEventCount = 1000;

  blockMovingExecutionPlaning(block,0.01,0.9);

  UnityAssertEqualNumber((_U_SINT)((1500)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(1122), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((135000)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(1123), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1000)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(1124), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1000)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(1125), UNITY_DISPLAY_STYLE_INT);

}





void test__getUnitVector_the_posX_is_3_posY_is_4_and_posZ_is_5(void){

  UnityAssertFloatsWithin((_UF)((_UF)((0.42426)) * (_UF)(0.00001f)), (_UF)((_UF)((0.42426))), (_UF)((_UF)((_getUnitVector(3,4,5,0)))), ((((void *)0))), (_U_UINT)((_U_UINT)(1130)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.56569)) * (_UF)(0.00001f)), (_UF)((_UF)((0.56569))), (_UF)((_UF)((_getUnitVector(3,4,5,1)))), ((((void *)0))), (_U_UINT)((_U_UINT)(1131)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.70711)) * (_UF)(0.00001f)), (_UF)((_UF)((0.70711))), (_UF)((_UF)((_getUnitVector(3,4,5,2)))), ((((void *)0))), (_U_UINT)((_U_UINT)(1132)));

}













void test_getCosThetaBetweenTwoVector_when_vectorA_is_X8Y0Z0_and_vectorB_is_X5Y0Z0(void){

  float unitVectorA[3];

  float unitVectorB[3];

  unitVectorA[0] = _getUnitVector(8,0,0,0);

  unitVectorA[1] = _getUnitVector(8,0,0,1);

  unitVectorA[2] = _getUnitVector(8,0,0,2);

  unitVectorB[0] = _getUnitVector(5,0,0,0);

  unitVectorB[1] = _getUnitVector(5,0,0,1);

  unitVectorB[2] = _getUnitVector(5,0,0,2);

  UnityAssertFloatsWithin((_UF)((_UF)((1)) * (_UF)(0.00001f)), (_UF)((_UF)((1))), (_UF)((_UF)((unitVectorA[0]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1149)));

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((unitVectorA[1]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1150)));

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((unitVectorA[2]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1151)));

  UnityAssertFloatsWithin((_UF)((_UF)((1)) * (_UF)(0.00001f)), (_UF)((_UF)((1))), (_UF)((_UF)((unitVectorB[0]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1152)));

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((unitVectorB[1]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1153)));

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((unitVectorB[2]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1154)));

  UnityAssertFloatsWithin((_UF)((_UF)((-1)) * (_UF)(0.00001f)), (_UF)((_UF)((-1))), (_UF)((_UF)((getCosThetaBetweenTwoVector(unitVectorA,unitVectorB)))), ((((void *)0))), (_U_UINT)((_U_UINT)(1155)));

}

void test_getCosThetaBetweenTwoVector_when_vectorA_is_X8Y0Z0_and_vectorB_is_X4Y3Z0(void){

  float unitVectorA[3];

  float unitVectorB[3];

  unitVectorA[0] = _getUnitVector(8,0,0,0);

  unitVectorA[1] = _getUnitVector(8,0,0,1);

  unitVectorA[2] = _getUnitVector(8,0,0,2);

  unitVectorB[0] = _getUnitVector(4,3,0,0);

  unitVectorB[1] = _getUnitVector(4,3,0,1);

  unitVectorB[2] = _getUnitVector(4,3,0,2);

  UnityAssertFloatsWithin((_UF)((_UF)((1)) * (_UF)(0.00001f)), (_UF)((_UF)((1))), (_UF)((_UF)((unitVectorA[0]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1180)));

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((unitVectorA[1]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1181)));

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((unitVectorA[2]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1182)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.8)) * (_UF)(0.00001f)), (_UF)((_UF)((0.8))), (_UF)((_UF)((unitVectorB[0]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1183)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.6)) * (_UF)(0.00001f)), (_UF)((_UF)((0.6))), (_UF)((_UF)((unitVectorB[1]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1184)));

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((unitVectorB[2]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1185)));

  UnityAssertFloatsWithin((_UF)((_UF)((-0.8)) * (_UF)(0.00001f)), (_UF)((_UF)((-0.8))), (_UF)((_UF)((getCosThetaBetweenTwoVector(unitVectorA,unitVectorB)))), ((((void *)0))), (_U_UINT)((_U_UINT)(1186)));

}

void test_getCosThetaBetweenTwoVector_when_vectorA_is_X8Y0Z0_and_vectorB_is_Xnegate2Y0Z0(void){

  float unitVectorA[3];

  float unitVectorB[3];

  unitVectorA[0] = _getUnitVector(8,0,0,0);

  unitVectorA[1] = _getUnitVector(8,0,0,1);

  unitVectorA[2] = _getUnitVector(8,0,0,2);

  unitVectorB[0] = _getUnitVector(-2,0,0,0);

  unitVectorB[1] = _getUnitVector(-2,0,0,1);

  unitVectorB[2] = _getUnitVector(-2,0,0,2);

  UnityAssertFloatsWithin((_UF)((_UF)((1)) * (_UF)(0.00001f)), (_UF)((_UF)((1))), (_UF)((_UF)((unitVectorA[0]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1209)));

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((unitVectorA[1]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1210)));

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((unitVectorA[2]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1211)));

  UnityAssertFloatsWithin((_UF)((_UF)((-1)) * (_UF)(0.00001f)), (_UF)((_UF)((-1))), (_UF)((_UF)((unitVectorB[0]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1212)));

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((unitVectorB[1]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1213)));

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((unitVectorB[2]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1214)));

  UnityAssertFloatsWithin((_UF)((_UF)((1)) * (_UF)(0.00001f)), (_UF)((_UF)((1))), (_UF)((_UF)((getCosThetaBetweenTwoVector(unitVectorA,unitVectorB)))), ((((void *)0))), (_U_UINT)((_U_UINT)(1215)));

}



void test_getCosThetaBetweenTwoVector_when_vectorA_is_X8Y10Z2_and_vectorB_is_X3Y4Z5(void){

  float unitVectorA[3];

  float unitVectorB[3];

  unitVectorA[0] = _getUnitVector(8,10,2,0);

  unitVectorA[1] = _getUnitVector(8,10,2,1);

  unitVectorA[2] = _getUnitVector(8,10,2,2);

  unitVectorB[0] = _getUnitVector(3,4,5,0);

  unitVectorB[1] = _getUnitVector(3,4,5,1);

  unitVectorB[2] = _getUnitVector(3,4,5,2);

  UnityAssertFloatsWithin((_UF)((_UF)((0.61721)) * (_UF)(0.00001f)), (_UF)((_UF)((0.61721))), (_UF)((_UF)((unitVectorA[0]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1227)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.77152)) * (_UF)(0.00001f)), (_UF)((_UF)((0.77152))), (_UF)((_UF)((unitVectorA[1]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1228)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.154303)) * (_UF)(0.00001f)), (_UF)((_UF)((0.154303))), (_UF)((_UF)((unitVectorA[2]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1229)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.42426)) * (_UF)(0.00001f)), (_UF)((_UF)((0.42426))), (_UF)((_UF)((unitVectorB[0]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1230)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.56569)) * (_UF)(0.00001f)), (_UF)((_UF)((0.56569))), (_UF)((_UF)((unitVectorB[1]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1231)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.70711)) * (_UF)(0.00001f)), (_UF)((_UF)((0.70711))), (_UF)((_UF)((unitVectorB[2]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1232)));

  UnityAssertFloatsWithin((_UF)((_UF)((-0.80740)) * (_UF)(0.00001f)), (_UF)((_UF)((-0.80740))), (_UF)((_UF)((getCosThetaBetweenTwoVector(unitVectorA,unitVectorB)))), ((((void *)0))), (_U_UINT)((_U_UINT)(1233)));

}



void test_entrySpeedPlanning_the_enntry_speed_first_block_is_0(void){

  block_t *block = &blockBuffer[bufferHead];

  UnityAssertEqualNumber((_U_SINT)((bufferHead)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(1238), UNITY_DISPLAY_STYLE_INT);

  entrySpeedPlanning(block,0);

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((block->entrySpeed))), ((((void *)0))), (_U_UINT)((_U_UINT)(1240)));

}

void test_entrySpeedPlanning_when_add_in_a_block_which_the_vector_is_same_with_previous_block_and_previousNominalSpeed_is_higher_than_the_current_nominalSpeed(void){

  updatebufferHead();

  block_t *block = &blockBuffer[bufferHead];

  block->nominalSpeed = 100;

  plExecutor.previousNominalSpeed = 150;

  entrySpeedPlanning(block,-1);

  UnityAssertFloatsWithin((_UF)((_UF)((100)) * (_UF)(0.00001f)), (_UF)((_UF)((100))), (_UF)((_UF)((block->entrySpeed))), ((((void *)0))), (_U_UINT)((_U_UINT)(1262)));

}

void test_entrySpeedPlanning_when_add_in_a_block_which_the_vector_is_same_with_previous_block_and_previousNominalSpeed_is_less_than_the_current_nominalSpeed(void){

  updatebufferHead();

  block_t *block = &blockBuffer[bufferHead];

  block->nominalSpeed = 100;

  plExecutor.previousNominalSpeed = 80;

  entrySpeedPlanning(block,-1);

  UnityAssertFloatsWithin((_UF)((_UF)((80)) * (_UF)(0.00001f)), (_UF)((_UF)((80))), (_UF)((_UF)((block->entrySpeed))), ((((void *)0))), (_U_UINT)((_U_UINT)(1284)));

}



void test_planLineBlock_and_entrySpeedPlanning_put_Moving_X_axis_is_300_Y_axis_is_400_and_Z_axis_is_100_(void){



  block_t *block = &blockBuffer[bufferHead];

  planLineBlock(300,400,100,100,0);

  UnityAssertEqualNumber((_U_SINT)((0x00)), (_U_SINT)((block->directionBits)), (((void *)0)), (_U_UINT)(1291), UNITY_DISPLAY_STYLE_INT);

  UnityAssertFloatsWithin((_UF)((_UF)((509.902)) * (_UF)(0.00001f)), (_UF)((_UF)((509.902))), (_UF)((_UF)((block->millimeters))), ((((void *)0))), (_U_UINT)((_U_UINT)(1292)));

  UnityAssertFloatsWithin((_UF)((0.00002)), (_UF)((0.0019611)), (_UF)((block->inverseMillimeters)), (((void *)0)), (_U_UINT)(1293));

  UnityAssertEqualNumber((_U_SINT)((60000)), (_U_SINT)((block->steps[0])), (((void *)0)), (_U_UINT)(1294), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block->steps[1])), (((void *)0)), (_U_UINT)(1295), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block->steps[2])), (((void *)0)), (_U_UINT)(1296), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block->stepEventCount)), (((void *)0)), (_U_UINT)(1297), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((15690)), (_U_SINT)((block->nominalRate)), (((void *)0)), (_U_UINT)(1298), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((block->nominalSpeed)), (((void *)0)), (_U_UINT)(1299), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->entrySpeed)), (((void *)0)), (_U_UINT)(1300), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(1301), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((bufferHead)), (((void *)0)), (_U_UINT)(1302), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((2)), (_U_SINT)((nextBufferHead)), (((void *)0)), (_U_UINT)(1303), UNITY_DISPLAY_STYLE_INT);

  UnityAssertFloatsWithin((_UF)((_UF)((0.58835)) * (_UF)(0.00001f)), (_UF)((_UF)((0.58835))), (_UF)((_UF)((plExecutor.previousUnitVec[0]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1304)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.78446)) * (_UF)(0.00001f)), (_UF)((_UF)((0.78446))), (_UF)((_UF)((plExecutor.previousUnitVec[1]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1305)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.196116)) * (_UF)(0.00001f)), (_UF)((_UF)((0.196116))), (_UF)((_UF)((plExecutor.previousUnitVec[2]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1306)));

  UnityAssertEqualNumber((_U_SINT)((60000)), (_U_SINT)((plExecutor.position[0])), (((void *)0)), (_U_UINT)(1307), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((plExecutor.position[1])), (((void *)0)), (_U_UINT)(1308), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((plExecutor.position[2])), (((void *)0)), (_U_UINT)(1309), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((plExecutor.previousNominalSpeed)), (((void *)0)), (_U_UINT)(1310), UNITY_DISPLAY_STYLE_INT);





  block = &blockBuffer[bufferHead];

  planLineBlock(400,400,100,100,0);

  UnityAssertFloatsWithin((_UF)((_UF)((100)) * (_UF)(0.00001f)), (_UF)((_UF)((100))), (_UF)((_UF)((block->millimeters))), ((((void *)0))), (_U_UINT)((_U_UINT)(1315)));

  UnityAssertFloatsWithin((_UF)((0.00002)), (_UF)((0.01)), (_UF)((block->inverseMillimeters)), (((void *)0)), (_U_UINT)(1316));

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block->steps[0])), (((void *)0)), (_U_UINT)(1317), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->steps[1])), (((void *)0)), (_U_UINT)(1318), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->steps[2])), (((void *)0)), (_U_UINT)(1319), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block->stepEventCount)), (((void *)0)), (_U_UINT)(1320), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block->nominalRate)), (((void *)0)), (_U_UINT)(1321), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((block->nominalSpeed)), (((void *)0)), (_U_UINT)(1322), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((block->entrySpeed)), (((void *)0)), (_U_UINT)(1323), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(1324), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((2)), (_U_SINT)((bufferHead)), (((void *)0)), (_U_UINT)(1325), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((3)), (_U_SINT)((nextBufferHead)), (((void *)0)), (_U_UINT)(1326), UNITY_DISPLAY_STYLE_INT);

  UnityAssertFloatsWithin((_UF)((_UF)((1)) * (_UF)(0.00001f)), (_UF)((_UF)((1))), (_UF)((_UF)((plExecutor.previousUnitVec[0]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1327)));

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((plExecutor.previousUnitVec[1]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1328)));

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((plExecutor.previousUnitVec[2]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1329)));

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((plExecutor.position[0])), (((void *)0)), (_U_UINT)(1330), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((plExecutor.position[1])), (((void *)0)), (_U_UINT)(1331), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((plExecutor.position[2])), (((void *)0)), (_U_UINT)(1332), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((plExecutor.previousNominalSpeed)), (((void *)0)), (_U_UINT)(1333), UNITY_DISPLAY_STYLE_INT);



  block = &blockBuffer[bufferHead];

  planLineBlock(500,400,100,90,0);

  UnityAssertFloatsWithin((_UF)((_UF)((100)) * (_UF)(0.00001f)), (_UF)((_UF)((100))), (_UF)((_UF)((block->millimeters))), ((((void *)0))), (_U_UINT)((_U_UINT)(1337)));

  UnityAssertFloatsWithin((_UF)((0.00002)), (_UF)((0.01)), (_UF)((block->inverseMillimeters)), (((void *)0)), (_U_UINT)(1338));

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block->steps[0])), (((void *)0)), (_U_UINT)(1339), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->steps[1])), (((void *)0)), (_U_UINT)(1340), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->steps[2])), (((void *)0)), (_U_UINT)(1341), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block->stepEventCount)), (((void *)0)), (_U_UINT)(1342), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((18000)), (_U_SINT)((block->nominalRate)), (((void *)0)), (_U_UINT)(1343), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((90)), (_U_SINT)((block->nominalSpeed)), (((void *)0)), (_U_UINT)(1344), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((90)), (_U_SINT)((block->entrySpeed)), (((void *)0)), (_U_UINT)(1345), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(1346), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((3)), (_U_SINT)((bufferHead)), (((void *)0)), (_U_UINT)(1347), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((4)), (_U_SINT)((nextBufferHead)), (((void *)0)), (_U_UINT)(1348), UNITY_DISPLAY_STYLE_INT);

  UnityAssertFloatsWithin((_UF)((_UF)((1)) * (_UF)(0.00001f)), (_UF)((_UF)((1))), (_UF)((_UF)((plExecutor.previousUnitVec[0]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1349)));

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((plExecutor.previousUnitVec[1]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1350)));

  UnityAssertFloatsWithin((_UF)((_UF)((0)) * (_UF)(0.00001f)), (_UF)((_UF)((0))), (_UF)((_UF)((plExecutor.previousUnitVec[2]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1351)));

  UnityAssertEqualNumber((_U_SINT)((100000)), (_U_SINT)((plExecutor.position[0])), (((void *)0)), (_U_UINT)(1352), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((plExecutor.position[1])), (((void *)0)), (_U_UINT)(1353), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((plExecutor.position[2])), (((void *)0)), (_U_UINT)(1354), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((90)), (_U_SINT)((plExecutor.previousNominalSpeed)), (((void *)0)), (_U_UINT)(1355), UNITY_DISPLAY_STYLE_INT);

}

void test_planLineBlock_replanBlockBufferStructure_only_one_block_is_added_into_BlockBuffer_to_do_blockMovingExecutionPlaning(void){

  block_t *block = &blockBuffer[bufferHead];

  planLineBlock(300,400,100,100,0);



  UnityAssertEqualNumber((_U_SINT)((0x00)), (_U_SINT)((block->directionBits)), (((void *)0)), (_U_UINT)(1377), UNITY_DISPLAY_STYLE_INT);

  UnityAssertFloatsWithin((_UF)((_UF)((509.902)) * (_UF)(0.00001f)), (_UF)((_UF)((509.902))), (_UF)((_UF)((block->millimeters))), ((((void *)0))), (_U_UINT)((_U_UINT)(1378)));

  UnityAssertFloatsWithin((_UF)((0.00002)), (_UF)((0.0019611)), (_UF)((block->inverseMillimeters)), (((void *)0)), (_U_UINT)(1379));

  UnityAssertEqualNumber((_U_SINT)((60000)), (_U_SINT)((block->steps[0])), (((void *)0)), (_U_UINT)(1380), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block->steps[1])), (((void *)0)), (_U_UINT)(1381), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block->steps[2])), (((void *)0)), (_U_UINT)(1382), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block->stepEventCount)), (((void *)0)), (_U_UINT)(1383), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((15690)), (_U_SINT)((block->nominalRate)), (((void *)0)), (_U_UINT)(1384), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((block->nominalSpeed)), (((void *)0)), (_U_UINT)(1385), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->entrySpeed)), (((void *)0)), (_U_UINT)(1386), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(1387), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((bufferHead)), (((void *)0)), (_U_UINT)(1388), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((2)), (_U_SINT)((nextBufferHead)), (((void *)0)), (_U_UINT)(1389), UNITY_DISPLAY_STYLE_INT);

  UnityAssertFloatsWithin((_UF)((_UF)((0.58835)) * (_UF)(0.00001f)), (_UF)((_UF)((0.58835))), (_UF)((_UF)((plExecutor.previousUnitVec[0]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1390)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.78446)) * (_UF)(0.00001f)), (_UF)((_UF)((0.78446))), (_UF)((_UF)((plExecutor.previousUnitVec[1]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1391)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.196116)) * (_UF)(0.00001f)), (_UF)((_UF)((0.196116))), (_UF)((_UF)((plExecutor.previousUnitVec[2]))), ((((void *)0))), (_U_UINT)((_U_UINT)(1392)));

  UnityAssertEqualNumber((_U_SINT)((60000)), (_U_SINT)((plExecutor.position[0])), (((void *)0)), (_U_UINT)(1393), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((plExecutor.position[1])), (((void *)0)), (_U_UINT)(1394), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((plExecutor.position[2])), (((void *)0)), (_U_UINT)(1395), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((plExecutor.previousNominalSpeed)), (((void *)0)), (_U_UINT)(1396), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(1397), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(1398), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((18)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(1399), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((79982)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(1400), UNITY_DISPLAY_STYLE_INT);

}

void test_planLineBlock_replanBlockBufferStructure_two_block_are_added_with_same_nominal_speed_into_BlockBuffer_to_do_blockMovingExecutionPlaning(void){

  block_t *block = &blockBuffer[bufferTail];

  planLineBlock(300,400,100,100,0);



  UnityAssertEqualNumber((_U_SINT)((15690)), (_U_SINT)((block->nominalRate)), (((void *)0)), (_U_UINT)(1432), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(1433), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(1434), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((18)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(1435), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((79982)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(1436), UNITY_DISPLAY_STYLE_INT);



  planLineBlock(400,400,100,100,0);



  block_t *block1 = &blockBuffer[bufferTail];

  block_t *block2 = &blockBuffer[bufferTail+1];

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block1->initialRate)), (((void *)0)), (_U_UINT)(1442), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block1->stepEventCount)), (((void *)0)), (_U_UINT)(1443), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block1->entrySpeed)), (((void *)0)), (_U_UINT)(1444), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((15690)), (_U_SINT)((block1->nominalRate)), (((void *)0)), (_U_UINT)(1445), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((15690)), (_U_SINT)((block1->finalRate)), (((void *)0)), (_U_UINT)(1446), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((18)), (_U_SINT)((block1->accelerateUntil)), (((void *)0)), (_U_UINT)(1447), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block1->deccelerateAfter)), (((void *)0)), (_U_UINT)(1448), UNITY_DISPLAY_STYLE_INT);



  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block2->stepEventCount)), (((void *)0)), (_U_UINT)(1450), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((block2->entrySpeed)), (((void *)0)), (_U_UINT)(1451), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block2->initialRate)), (((void *)0)), (_U_UINT)(1452), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block2->nominalRate)), (((void *)0)), (_U_UINT)(1453), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block2->finalRate)), (((void *)0)), (_U_UINT)(1454), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block2->accelerateUntil)), (((void *)0)), (_U_UINT)(1455), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((19972)), (_U_SINT)((block2->deccelerateAfter)), (((void *)0)), (_U_UINT)(1456), UNITY_DISPLAY_STYLE_INT);

}

void test_planLineBlock_replanBlockBufferStructure_two_block_is_added_with_different_nominal_speed_NominalSpeed_of_Firblock_is_higher_than_SecBlock(void){

  block_t *block = &blockBuffer[bufferTail];

  planLineBlock(300,400,100,100,0);



  UnityAssertEqualNumber((_U_SINT)((15690)), (_U_SINT)((block->nominalRate)), (((void *)0)), (_U_UINT)(1488), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(1489), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(1490), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((18)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(1491), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((79982)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(1492), UNITY_DISPLAY_STYLE_INT);



  planLineBlock(400,400,100,90,0);



  block_t *block1 = &blockBuffer[bufferTail];

  block_t *block2 = &blockBuffer[bufferTail+1];

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block1->initialRate)), (((void *)0)), (_U_UINT)(1498), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block1->stepEventCount)), (((void *)0)), (_U_UINT)(1499), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block1->entrySpeed)), (((void *)0)), (_U_UINT)(1500), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((15690)), (_U_SINT)((block1->nominalRate)), (((void *)0)), (_U_UINT)(1501), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((14121)), (_U_SINT)((block1->finalRate)), (((void *)0)), (_U_UINT)(1502), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((18)), (_U_SINT)((block1->accelerateUntil)), (((void *)0)), (_U_UINT)(1503), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((79996)), (_U_SINT)((block1->deccelerateAfter)), (((void *)0)), (_U_UINT)(1504), UNITY_DISPLAY_STYLE_INT);



  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block2->stepEventCount)), (((void *)0)), (_U_UINT)(1506), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((90)), (_U_SINT)((block2->entrySpeed)), (((void *)0)), (_U_UINT)(1507), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((18000)), (_U_SINT)((block2->initialRate)), (((void *)0)), (_U_UINT)(1508), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((18000)), (_U_SINT)((block2->nominalRate)), (((void *)0)), (_U_UINT)(1509), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block2->finalRate)), (((void *)0)), (_U_UINT)(1510), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block2->accelerateUntil)), (((void *)0)), (_U_UINT)(1511), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((19977)), (_U_SINT)((block2->deccelerateAfter)), (((void *)0)), (_U_UINT)(1512), UNITY_DISPLAY_STYLE_INT);

}

void test_planLineBlock_replanBlockBufferStructure_two_block_is_added_with_different_nominal_speed_NominalSpeed_of_Firblock_is_less_than_SecBlock(void){

  block_t *block = &blockBuffer[bufferTail];

  planLineBlock(300,400,100,100,0);



  UnityAssertEqualNumber((_U_SINT)((15690)), (_U_SINT)((block->nominalRate)), (((void *)0)), (_U_UINT)(1546), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(1547), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(1548), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((18)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(1549), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((79982)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(1550), UNITY_DISPLAY_STYLE_INT);

  planLineBlock(400,400,100,120,0);



  block_t *block1 = &blockBuffer[bufferTail];

  block_t *block2 = &blockBuffer[bufferTail+1];

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block1->initialRate)), (((void *)0)), (_U_UINT)(1555), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block1->stepEventCount)), (((void *)0)), (_U_UINT)(1556), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block1->entrySpeed)), (((void *)0)), (_U_UINT)(1557), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((15690)), (_U_SINT)((block1->nominalRate)), (((void *)0)), (_U_UINT)(1558), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((15690)), (_U_SINT)((block1->finalRate)), (((void *)0)), (_U_UINT)(1559), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((18)), (_U_SINT)((block1->accelerateUntil)), (((void *)0)), (_U_UINT)(1560), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block1->deccelerateAfter)), (((void *)0)), (_U_UINT)(1561), UNITY_DISPLAY_STYLE_INT);



  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block2->stepEventCount)), (((void *)0)), (_U_UINT)(1563), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((100)), (_U_SINT)((block2->entrySpeed)), (((void *)0)), (_U_UINT)(1564), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block2->initialRate)), (((void *)0)), (_U_UINT)(1565), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((24000)), (_U_SINT)((block2->nominalRate)), (((void *)0)), (_U_UINT)(1566), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block2->finalRate)), (((void *)0)), (_U_UINT)(1567), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((13)), (_U_SINT)((block2->accelerateUntil)), (((void *)0)), (_U_UINT)(1568), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((19960)), (_U_SINT)((block2->deccelerateAfter)), (((void *)0)), (_U_UINT)(1569), UNITY_DISPLAY_STYLE_INT);

}
