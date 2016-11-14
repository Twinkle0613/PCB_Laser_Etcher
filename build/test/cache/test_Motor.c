#include "Motor.h"
#include "unity.h"


void setUp(void)

{

  motor0 = ((void *)0);

  motor1 = ((void *)0);

  motor2 = ((void *)0);

}



void tearDown(void)

{

}





void test_motorConfig_initialize_the_motor0(void){



  motor0 = motorConfig(((uint8_t)0x80),((uint8_t)0x00));

 UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x80))), (_U_SINT)((motor0->direation)), (((void *)0)), (_U_UINT)(19), UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x00))), (_U_SINT)((motor0->microstep)), (((void *)0)), (_U_UINT)(20), UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x10))), (_U_SINT)((motor0->reset)), (((void *)0)), (_U_UINT)(21), UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x20))), (_U_SINT)((motor0->sleep)), (((void *)0)), (_U_UINT)(22), UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((((uint8_t)0x00))), (_U_SINT)((motor0->motorState)), (((void *)0)), (_U_UINT)(23), UNITY_DISPLAY_STYLE_INT);

 UnityAssertEqualNumber((_U_SINT)((((uint8_t)0xBF))), (_U_SINT)((motor0->step)), (((void *)0)), (_U_UINT)(24), UNITY_DISPLAY_STYLE_INT);



}





void test_getMotorInfo_get_motor_info_when_the_motor_step_is_low(void){



  motor0 = motorConfig(((uint8_t)0x80),((uint8_t)0x00));

  uint8_t info = getMotorInfo(motor0,((uint8_t)0x80),((uint8_t)0xBF));

  uint8_t info2 = (((uint8_t)0x80)|

                 ((uint8_t)0x00)|

                 ((uint8_t)0x10)|

                 ((uint8_t)0x20)|

                 ((uint8_t)0x00))&((uint8_t)0xBF);

  UnityAssertEqualNumber((_U_SINT)((info2)), (_U_SINT)((info)), (((void *)0)), (_U_UINT)(38), UNITY_DISPLAY_STYLE_INT);



}



void test_getMotorInfo_get_motor_info_when_the_motor_step_is_high(void){



  motor0 = motorConfig(((uint8_t)0x80),((uint8_t)0x00));

  uint8_t info = getMotorInfo(motor0,((uint8_t)0x00),((uint8_t)0x40));

  uint8_t info2 = (((uint8_t)0x00)|

                 ((uint8_t)0x00)|

                 ((uint8_t)0x10)|

                 ((uint8_t)0x20)|

                 ((uint8_t)0x00))|((uint8_t)0x40);

  UnityAssertEqualNumber((_U_SINT)((info2)), (_U_SINT)((info)), (((void *)0)), (_U_UINT)(51), UNITY_DISPLAY_STYLE_INT);



}
