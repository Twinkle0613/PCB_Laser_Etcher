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
extern void test_TIM2_IRQHandler_the_function_is_called_by_CPU(void);
extern void test_initStepper_reset_and_initilize_all_of_the_data(void);
extern void test_sleep_the_timer_interrupt_is_disable(void);
extern void test_wakeup_the_timer_interrupt_is_enabled(void);
extern void test_TIM2_IRQHandler_IRQ_to_request_block_when_buffer_block_didnt_contain_any_block(void);
extern void test_TIM2_IRQHandler_IRQ_to_request_block_when_buffer_block_contain_blocks(void);
extern void test_TIM2_IRQHandler_no_more_block_for_IRQ_to_request_and_IRQ_will_go_to_sleep(void);
extern void test_transferInfoToStExecutor_when_IRQ_successfully_get_a_block_and_transfer_info_to_st_executor_from_block(void);
extern void test_TIM2_IRQHandler_get_block_from_block_buffer_and_transfer_block_info_into_st_executor(void);
extern void test_bresenhamAlgorithm_when_dx_is_10_and_dy_is_10_(void);
extern void test_bresenhamAlgorithm_when_dx_is_20_and_dy_is_15_(void);
extern void test_executeStepDisplament_XYZ_direations_are_positive_(void);
extern void test_executeStepDisplament_XYZ_direations_are_negative_(void);
extern void test_executeStepDisplacementProcess_the_functon_is_called_and_using_for_loop(void);
extern void test_getStepStatus_when_x_y_z_rest(void);
extern void test_getStepStatus_when_x_y_z_move_(void);
extern void test_getMoveDireaction_when_x_y_z_are_moving_right_direation(void);
extern void test_getMoveDireaction_when_x_y_z_are_moving_left_direation(void);
extern void test_planningDataSending_XYZ_direations_are_negative_(void);
extern void test_planningDataSending_XYZ_direations_are_positive_(void);
extern void test_planningDataSending_XYZ_motor_take_a_rest_(void);
extern void test_executeStepDisplacementProcess_planningDataSending_the_functon_is_called_and_using_for_loop(void);
extern void test_executeStepDisplacementProcess_planningDataSending_sendResetStep_the_functon_is_called_and_using_for_loop(void);
extern void test_TIM2_IRQHandler_and_executeStepDisplacementProcess_simulate_when_TIM_interrupt_occur(void);


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
  UnityBegin("test_Stepper.c");
  RUN_TEST(test_TIM2_IRQHandler_the_function_is_called_by_CPU, 80);
  RUN_TEST(test_initStepper_reset_and_initilize_all_of_the_data, 94);
  RUN_TEST(test_sleep_the_timer_interrupt_is_disable, 107);
  RUN_TEST(test_wakeup_the_timer_interrupt_is_enabled, 112);
  RUN_TEST(test_TIM2_IRQHandler_IRQ_to_request_block_when_buffer_block_didnt_contain_any_block, 128);
  RUN_TEST(test_TIM2_IRQHandler_IRQ_to_request_block_when_buffer_block_contain_blocks, 143);
  RUN_TEST(test_TIM2_IRQHandler_no_more_block_for_IRQ_to_request_and_IRQ_will_go_to_sleep, 161);
  RUN_TEST(test_transferInfoToStExecutor_when_IRQ_successfully_get_a_block_and_transfer_info_to_st_executor_from_block, 168);
  RUN_TEST(test_TIM2_IRQHandler_get_block_from_block_buffer_and_transfer_block_info_into_st_executor, 198);
  RUN_TEST(test_bresenhamAlgorithm_when_dx_is_10_and_dy_is_10_, 234);
  RUN_TEST(test_bresenhamAlgorithm_when_dx_is_20_and_dy_is_15_, 268);
  RUN_TEST(test_executeStepDisplament_XYZ_direations_are_positive_, 304);
  RUN_TEST(test_executeStepDisplament_XYZ_direations_are_negative_, 360);
  RUN_TEST(test_executeStepDisplacementProcess_the_functon_is_called_and_using_for_loop, 403);
  RUN_TEST(test_getStepStatus_when_x_y_z_rest, 446);
  RUN_TEST(test_getStepStatus_when_x_y_z_move_, 453);
  RUN_TEST(test_getMoveDireaction_when_x_y_z_are_moving_right_direation, 460);
  RUN_TEST(test_getMoveDireaction_when_x_y_z_are_moving_left_direation, 467);
  RUN_TEST(test_planningDataSending_XYZ_direations_are_negative_, 474);
  RUN_TEST(test_planningDataSending_XYZ_direations_are_positive_, 485);
  RUN_TEST(test_planningDataSending_XYZ_motor_take_a_rest_, 496);
  RUN_TEST(test_executeStepDisplacementProcess_planningDataSending_the_functon_is_called_and_using_for_loop, 510);
  RUN_TEST(test_executeStepDisplacementProcess_planningDataSending_sendResetStep_the_functon_is_called_and_using_for_loop, 542);
  RUN_TEST(test_TIM2_IRQHandler_and_executeStepDisplacementProcess_simulate_when_TIM_interrupt_occur, 568);

  return (UnityEnd());
}
