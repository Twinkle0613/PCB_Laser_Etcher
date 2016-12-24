/* AUTOGENERATED FILE. DO NOT EDIT. */

//=======Test Runner Used To Run Each Test Below=====
#define RUN_TEST(TestFunc, TestLineNum) \
{ \
  Unity.CurrentTestName = #TestFunc; \
  Unity.CurrentTestLineNumber = TestLineNum; \
  Unity.NumberOfTests++; \
  if (TEST_PROTECT()) \
  { \
      setUp(); \
      TestFunc(); \
  } \
  if (TEST_PROTECT() && !TEST_IS_IGNORED) \
  { \
    tearDown(); \
  } \
  UnityConcludeTest(); \
}

//=======Automagically Detected Files To Include=====
#include "unity.h"
#include <setjmp.h>
#include <stdio.h>

int GlobalExpectCount;
int GlobalVerifyOrder;
char* GlobalOrderError;

//=======External Functions This Runner Calls=====
extern void setUp(void);
extern void tearDown(void);
extern void test_nextBlockIndex_Index_point_to_next_block(void);
extern void test_nextBlockIndex_Index_point_to_next_block_with_overflow(void);
extern void test_prevBlockIndex_Index_point_to_previous_block(void);
extern void test_prevBlockIndex_Index_point_to_previous_block_with_underflow(void);
extern void test_planInit_initialize_the_bufferHead_bufferTail_and_nextBufferHead(void);
extern void test_getBlock_to_get_the_block_when_buffer_contained_block(void);
extern void test_getBlock_to_get_NULL_when_buffer_dont_have_any_block(void);
extern void test_discardCurrrentBlock_if_blockBuffer_contain_block_the_buffetTail_will_be_increased(void);
extern void test_discardCurrrentBlock_if_blockBuffer_doesnt_contain_block_the_buffetTail_will_remain(void);
extern void test_ceil_put_the_value_from_3point0_to_3point9_into_the_function_(void);
extern void test_convertXYZMovingDistanceToStepAndStoreToArray_when_x_is_100mm_y_is_200mm_and_z_is_300mm_the_function_will_return_x_is_20000_step_y_is_40000_step_x_is_60000_step(void);
extern void test_convertXYZMovingDistanceToStepAndStoreToArray_when_x_is_0mm_y_is_0mm_and_z_is_300mm_the_function_will_return_x_is_0_step_y_is_0_step_x_is_60000_step(void);
extern void test_determineDirection_when_target_x_less_than_prev_x_target_the_direactionBits_is_equal_0x20(void);
extern void test_determineDirection_when_target_y_less_than_prev_y_target_the_direactionBits_is_equal_0x40(void);
extern void test_determineDirection_when_target_z_less_than_prev_z_target_the_direactionBits_is_equal_0x80(void);
extern void test_determineDirection_when_target_xyz_less_than_prev_xyz_target_the_direactionBits_is_equal_0x80(void);
extern void test_labs_put_a_negative_value_into_function_it_should_the_absolute_value_(void);
extern void test_calculateXYZaxisSteps_to_obtain_the_moving_step_when_the_target_x_is_100_targer_y_is_200_and_target_z_is_300(void);
extern void test_calculateXYZaxisSteps_to_obtain_the_moving_step_when_the_target_x_is_100_targer_y_is_200_and_target_z_is_300_the_prev_target_xyz_are_100000(void);
extern void test_getHighestStepsInXYZsteps_when_the_X_steps_is_20000_Y_steps_is_40000_and_Z_steps_is_60000_the_max_steps_is_60000(void);
extern void test_getHighestStepsInXYZsteps_when_the_X_steps_is_20000_Y_steps_is_100000_and_Z_steps_is_60000_the_max_steps_is_100000(void);
extern void test_getHighestStepsInXYZsteps_when_the_X_steps_is_120000_Y_steps_is_100000_and_Z_steps_is_60000_the_max_steps_is_120000(void);
extern void test_planLineBlock_initially_plan_the_motor_move_in_x_is_100mm_y_is_200mm_and_z_is_300mm_with_3000_mm_per_min(void);
extern void test_getDeltaInMilliMeter_put_several_input_to_test(void);
extern void test_calculateXYZDeltaInMilliMeterAndStoreToArray_targetXYZ_are_20000steps_40000steps_and_60000steps_and_the_prevTargetXYZ_are_0_0_and_0(void);
extern void test_calculateXYZDeltaInMilliMeterAndStoreToArray_targetXYZ_are_20000steps_40000steps_and_60000steps_and_the_prevTargetXYZ_are_100000_100000_and_100000(void);
extern void test_getVectorLength_when_deltaX_is_100_deltaY_is_200_and_deltaZ_is_300_the_function_should_return_374(void);
extern void test_getVectorLength_when_deltaX_is_0_deltaY_is_400_and_deltaZ_is_300_the_function_should_return_500(void);
extern void test_getInverse_(void);
extern void test_estimateNominalRateAndSpeed_when_the_vectorLenght_is_500mm_and_MaxStep_is_80000step(void);
extern void test_estimateNominalRateAndSpeed_when_the_vectorLenght_is_509_point_902mm_and_MaxStep_is_80000step(void);
extern void test_planLineBlock_move_in_the_x_axis_is_300mm_the_y_axis_is_200mm_and_z_is_100mm_(void);
extern void test_getUnitVector_the_deltaX_is_300_deltaY_is_400_and_deltaZ_is_100_and_vectorLength_is_509pt109(void);
extern void test_calculateUnitVectorForXYZaxisAndStoreToArray_the_deltaX_is_300_deltaY_is_400_and_deltaZ_is_100_and_vectorLength_is_509pt109(void);
extern void test_planLineBlock_put_Moving_X_axis_is_300_Y_axis_is_400_and_Z_axis_is_100_(void);
extern void test_getRampStep_initialRate_is_80000_and_targeRate_is_160000_and_acceleration_is_36000mm_per_min_square(void);
extern void test_getNominalStep_the_function_is_tested_by_putting_some_value(void);
extern void test_getIntersectionDistance_put_initialRate_and_finalRate_is_equal_80000_and_distance_is_change(void);
extern void test_getIntersectionDistance_when_finalRate_is_much_less_than_initialRate_(void);
extern void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_and_exitFactor_is_0pts8(void);
extern void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts5_and_exitFactor_is_0pts8(void);
extern void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts8_and_exitFactor_is_0pts5(void);
extern void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts5_and_exitFactor_is_0pts5(void);
extern void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts5_and_exitFactor_is_0pts2(void);
extern void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts5_and_exitFactor_is_0pts01(void);
extern void test_blockMovingExecutionPlaning_nominalRate_is_150000_entryFactor_is_1pts3_and_exitFactor_is_0pts01(void);
extern void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts2_and_exitFactor_is_0pts5(void);
extern void test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts01_and_exitFactor_is_0pts5(void);
extern void test_blockMovingExecutionPlaning_nominalRate_is_150000_entryFactor_is_0pts01_and_exitFactor_is_0pts9(void);
extern void test__getUnitVector_the_posX_is_3_posY_is_4_and_posZ_is_5(void);
extern void test_getCosThetaBetweenTwoVector_when_vectorA_is_X8Y0Z0_and_vectorB_is_X5Y0Z0(void);
extern void test_getCosThetaBetweenTwoVector_when_vectorA_is_X8Y0Z0_and_vectorB_is_X4Y3Z0(void);
extern void test_getCosThetaBetweenTwoVector_when_vectorA_is_X8Y0Z0_and_vectorB_is_Xnegate2Y0Z0(void);
extern void test_getCosThetaBetweenTwoVector_when_vectorA_is_X8Y10Z2_and_vectorB_is_X3Y4Z5(void);
extern void test_entrySpeedPlanning_the_enntry_speed_first_block_is_0(void);
extern void test_entrySpeedPlanning_when_add_in_a_block_which_the_vector_is_same_with_previous_block_and_previousNominalSpeed_is_higher_than_the_current_nominalSpeed(void);
extern void test_entrySpeedPlanning_when_add_in_a_block_which_the_vector_is_same_with_previous_block_and_previousNominalSpeed_is_less_than_the_current_nominalSpeed(void);
extern void test_planLineBlock_and_entrySpeedPlanning_put_Moving_X_axis_is_300_Y_axis_is_400_and_Z_axis_is_100_(void);
extern void test_planLineBlock_replanBlockBufferStructure_only_one_block_is_added_into_BlockBuffer_to_do_blockMovingExecutionPlaning(void);
extern void test_planLineBlock_replanBlockBufferStructure_two_block_are_added_with_same_nominal_speed_into_BlockBuffer_to_do_blockMovingExecutionPlaning(void);
extern void test_planLineBlock_replanBlockBufferStructure_two_block_is_added_with_different_nominal_speed_NominalSpeed_of_Firblock_is_higher_than_SecBlock(void);
extern void test_planLineBlock_replanBlockBufferStructure_two_block_is_added_with_different_nominal_speed_NominalSpeed_of_Firblock_is_less_than_SecBlock(void);


//=======Test Reset Option=====
void resetTest(void);
void resetTest(void)
{
  tearDown();
  setUp();
}


//=======MAIN=====
int main(void)
{
  UnityBegin("test_Planner.c");
  RUN_TEST(test_nextBlockIndex_Index_point_to_next_block, 24);
  RUN_TEST(test_nextBlockIndex_Index_point_to_next_block_with_overflow, 39);
  RUN_TEST(test_prevBlockIndex_Index_point_to_previous_block, 53);
  RUN_TEST(test_prevBlockIndex_Index_point_to_previous_block_with_underflow, 66);
  RUN_TEST(test_planInit_initialize_the_bufferHead_bufferTail_and_nextBufferHead, 81);
  RUN_TEST(test_getBlock_to_get_the_block_when_buffer_contained_block, 103);
  RUN_TEST(test_getBlock_to_get_NULL_when_buffer_dont_have_any_block, 120);
  RUN_TEST(test_discardCurrrentBlock_if_blockBuffer_contain_block_the_buffetTail_will_be_increased, 135);
  RUN_TEST(test_discardCurrrentBlock_if_blockBuffer_doesnt_contain_block_the_buffetTail_will_remain, 152);
  RUN_TEST(test_ceil_put_the_value_from_3point0_to_3point9_into_the_function_, 196);
  RUN_TEST(test_convertXYZMovingDistanceToStepAndStoreToArray_when_x_is_100mm_y_is_200mm_and_z_is_300mm_the_function_will_return_x_is_20000_step_y_is_40000_step_x_is_60000_step, 333);
  RUN_TEST(test_convertXYZMovingDistanceToStepAndStoreToArray_when_x_is_0mm_y_is_0mm_and_z_is_300mm_the_function_will_return_x_is_0_step_y_is_0_step_x_is_60000_step, 341);
  RUN_TEST(test_determineDirection_when_target_x_less_than_prev_x_target_the_direactionBits_is_equal_0x20, 349);
  RUN_TEST(test_determineDirection_when_target_y_less_than_prev_y_target_the_direactionBits_is_equal_0x40, 360);
  RUN_TEST(test_determineDirection_when_target_z_less_than_prev_z_target_the_direactionBits_is_equal_0x80, 371);
  RUN_TEST(test_determineDirection_when_target_xyz_less_than_prev_xyz_target_the_direactionBits_is_equal_0x80, 382);
  RUN_TEST(test_labs_put_a_negative_value_into_function_it_should_the_absolute_value_, 393);
  RUN_TEST(test_calculateXYZaxisSteps_to_obtain_the_moving_step_when_the_target_x_is_100_targer_y_is_200_and_target_z_is_300, 402);
  RUN_TEST(test_calculateXYZaxisSteps_to_obtain_the_moving_step_when_the_target_x_is_100_targer_y_is_200_and_target_z_is_300_the_prev_target_xyz_are_100000, 415);
  RUN_TEST(test_getHighestStepsInXYZsteps_when_the_X_steps_is_20000_Y_steps_is_40000_and_Z_steps_is_60000_the_max_steps_is_60000, 429);
  RUN_TEST(test_getHighestStepsInXYZsteps_when_the_X_steps_is_20000_Y_steps_is_100000_and_Z_steps_is_60000_the_max_steps_is_100000, 437);
  RUN_TEST(test_getHighestStepsInXYZsteps_when_the_X_steps_is_120000_Y_steps_is_100000_and_Z_steps_is_60000_the_max_steps_is_120000, 445);
  RUN_TEST(test_planLineBlock_initially_plan_the_motor_move_in_x_is_100mm_y_is_200mm_and_z_is_300mm_with_3000_mm_per_min, 454);
  RUN_TEST(test_getDeltaInMilliMeter_put_several_input_to_test, 470);
  RUN_TEST(test_calculateXYZDeltaInMilliMeterAndStoreToArray_targetXYZ_are_20000steps_40000steps_and_60000steps_and_the_prevTargetXYZ_are_0_0_and_0, 480);
  RUN_TEST(test_calculateXYZDeltaInMilliMeterAndStoreToArray_targetXYZ_are_20000steps_40000steps_and_60000steps_and_the_prevTargetXYZ_are_100000_100000_and_100000, 495);
  RUN_TEST(test_getVectorLength_when_deltaX_is_100_deltaY_is_200_and_deltaZ_is_300_the_function_should_return_374, 510);
  RUN_TEST(test_getVectorLength_when_deltaX_is_0_deltaY_is_400_and_deltaZ_is_300_the_function_should_return_500, 518);
  RUN_TEST(test_getInverse_, 526);
  RUN_TEST(test_estimateNominalRateAndSpeed_when_the_vectorLenght_is_500mm_and_MaxStep_is_80000step, 559);
  RUN_TEST(test_estimateNominalRateAndSpeed_when_the_vectorLenght_is_509_point_902mm_and_MaxStep_is_80000step, 599);
  RUN_TEST(test_planLineBlock_move_in_the_x_axis_is_300mm_the_y_axis_is_200mm_and_z_is_100mm_, 628);
  RUN_TEST(test_getUnitVector_the_deltaX_is_300_deltaY_is_400_and_deltaZ_is_100_and_vectorLength_is_509pt109, 648);
  RUN_TEST(test_calculateUnitVectorForXYZaxisAndStoreToArray_the_deltaX_is_300_deltaY_is_400_and_deltaZ_is_100_and_vectorLength_is_509pt109, 686);
  RUN_TEST(test_planLineBlock_put_Moving_X_axis_is_300_Y_axis_is_400_and_Z_axis_is_100_, 707);
  RUN_TEST(test_getRampStep_initialRate_is_80000_and_targeRate_is_160000_and_acceleration_is_36000mm_per_min_square, 749);
  RUN_TEST(test_getNominalStep_the_function_is_tested_by_putting_some_value, 758);
  RUN_TEST(test_getIntersectionDistance_put_initialRate_and_finalRate_is_equal_80000_and_distance_is_change, 778);
  RUN_TEST(test_getIntersectionDistance_when_finalRate_is_much_less_than_initialRate_, 844);
  RUN_TEST(test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_and_exitFactor_is_0pts8, 873);
  RUN_TEST(test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts5_and_exitFactor_is_0pts8, 900);
  RUN_TEST(test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts8_and_exitFactor_is_0pts5, 927);
  RUN_TEST(test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts5_and_exitFactor_is_0pts5, 955);
  RUN_TEST(test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts5_and_exitFactor_is_0pts2, 982);
  RUN_TEST(test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts5_and_exitFactor_is_0pts01, 1009);
  RUN_TEST(test_blockMovingExecutionPlaning_nominalRate_is_150000_entryFactor_is_1pts3_and_exitFactor_is_0pts01, 1036);
  RUN_TEST(test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts2_and_exitFactor_is_0pts5, 1063);
  RUN_TEST(test_blockMovingExecutionPlaning_nominalRate_is_100000_entryFactor_is_0pts01_and_exitFactor_is_0pts5, 1091);
  RUN_TEST(test_blockMovingExecutionPlaning_nominalRate_is_150000_entryFactor_is_0pts01_and_exitFactor_is_0pts9, 1117);
  RUN_TEST(test__getUnitVector_the_posX_is_3_posY_is_4_and_posZ_is_5, 1129);
  RUN_TEST(test_getCosThetaBetweenTwoVector_when_vectorA_is_X8Y0Z0_and_vectorB_is_X5Y0Z0, 1140);
  RUN_TEST(test_getCosThetaBetweenTwoVector_when_vectorA_is_X8Y0Z0_and_vectorB_is_X4Y3Z0, 1171);
  RUN_TEST(test_getCosThetaBetweenTwoVector_when_vectorA_is_X8Y0Z0_and_vectorB_is_Xnegate2Y0Z0, 1200);
  RUN_TEST(test_getCosThetaBetweenTwoVector_when_vectorA_is_X8Y10Z2_and_vectorB_is_X3Y4Z5, 1218);
  RUN_TEST(test_entrySpeedPlanning_the_enntry_speed_first_block_is_0, 1236);
  RUN_TEST(test_entrySpeedPlanning_when_add_in_a_block_which_the_vector_is_same_with_previous_block_and_previousNominalSpeed_is_higher_than_the_current_nominalSpeed, 1256);
  RUN_TEST(test_entrySpeedPlanning_when_add_in_a_block_which_the_vector_is_same_with_previous_block_and_previousNominalSpeed_is_less_than_the_current_nominalSpeed, 1278);
  RUN_TEST(test_planLineBlock_and_entrySpeedPlanning_put_Moving_X_axis_is_300_Y_axis_is_400_and_Z_axis_is_100_, 1287);
  RUN_TEST(test_planLineBlock_replanBlockBufferStructure_only_one_block_is_added_into_BlockBuffer_to_do_blockMovingExecutionPlaning, 1373);
  RUN_TEST(test_planLineBlock_replanBlockBufferStructure_two_block_are_added_with_same_nominal_speed_into_BlockBuffer_to_do_blockMovingExecutionPlaning, 1428);
  RUN_TEST(test_planLineBlock_replanBlockBufferStructure_two_block_is_added_with_different_nominal_speed_NominalSpeed_of_Firblock_is_higher_than_SecBlock, 1484);
  RUN_TEST(test_planLineBlock_replanBlockBufferStructure_two_block_is_added_with_different_nominal_speed_NominalSpeed_of_Firblock_is_less_than_SecBlock, 1542);

  return (UnityEnd());
}
