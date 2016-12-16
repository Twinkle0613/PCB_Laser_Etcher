#include "System.h"
#include "unity.h"


void setUp(void)

{

}



void tearDown(void)

{

}



void test_sysInit(void){

  sys.position[0] = 1;

  sys.position[1] = 2;

  sys.position[2] = 3;

  sysInit();

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((sys.position[0])), (((void *)0)), (_U_UINT)(17), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((sys.position[1])), (((void *)0)), (_U_UINT)(18), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((sys.position[2])), (((void *)0)), (_U_UINT)(19), UNITY_DISPLAY_STYLE_INT);

}
