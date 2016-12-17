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

  UnityAssertFloatsWithin((_UF)((0.00002)), (_UF)((0.0019611)), (_UF)((block->inverseMillimeters)), (((void *)0)), (_U_UINT)(686));

  UnityAssertFloatsWithin((_UF)((_UF)((0.58835)) * (_UF)(0.00001f)), (_UF)((_UF)((0.58835))), (_UF)((_UF)((unitVector[0]))), ((((void *)0))), (_U_UINT)((_U_UINT)(687)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.78446)) * (_UF)(0.00001f)), (_UF)((_UF)((0.78446))), (_UF)((_UF)((unitVector[1]))), ((((void *)0))), (_U_UINT)((_U_UINT)(688)));

  UnityAssertFloatsWithin((_UF)((_UF)((0.196116)) * (_UF)(0.00001f)), (_UF)((_UF)((0.196116))), (_UF)((_UF)((unitVector[2]))), ((((void *)0))), (_U_UINT)((_U_UINT)(689)));

}
