#include "unity.h"
#include "Motor.h"

void setUp(void)
{
  motor0 = NULL;
  motor1 = NULL;
  motor2 = NULL;
}

void tearDown(void)
{
}


void test_motorConfig_initialize_the_motor0(void){
  
  motor0  = motorConfig(MOTOR_LEFT,MOTOR_FULL_STEP);
	TEST_ASSERT_EQUAL(MOTOR_LEFT,motor0->direation);
	TEST_ASSERT_EQUAL(MOTOR_FULL_STEP,motor0->microstep);
	TEST_ASSERT_EQUAL(MOTOR_RESET_DISABLE,motor0->reset);
	TEST_ASSERT_EQUAL(MOTOR_SLEEP_MODE_DISABLE,motor0->sleep);
	TEST_ASSERT_EQUAL(MOTOR_ENABLE,motor0->motorState);
	TEST_ASSERT_EQUAL(MOTOR_STEP_LOW,motor0->step);
	
}


void test_getMotorInfo_get_motor_info_when_the_motor_step_is_low(void){
  
  motor0  = motorConfig(MOTOR_LEFT,MOTOR_FULL_STEP);
  uint8_t info = getMotorInfo(motor0,MOTOR_LEFT,MOTOR_STEP_LOW);
  uint8_t info2 = (MOTOR_LEFT|
                 MOTOR_FULL_STEP|
                 MOTOR_RESET_DISABLE|
                 MOTOR_SLEEP_MODE_DISABLE|
                 MOTOR_ENABLE)&MOTOR_STEP_LOW;
  TEST_ASSERT_EQUAL(info2,info);
  
}

void test_getMotorInfo_get_motor_info_when_the_motor_step_is_high(void){
  
  motor0  = motorConfig(MOTOR_LEFT,MOTOR_FULL_STEP);
  uint8_t info = getMotorInfo(motor0,MOTOR_RIGHT,MOTOR_STEP_HIGH);
  uint8_t info2 = (MOTOR_RIGHT|
                 MOTOR_FULL_STEP|
                 MOTOR_RESET_DISABLE|
                 MOTOR_SLEEP_MODE_DISABLE|
                 MOTOR_ENABLE)|MOTOR_STEP_HIGH;
  TEST_ASSERT_EQUAL(info2,info);
  
}