#include "unity.h"
#include "System.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_sysInit(void){
  sys.position[X_AXIS] = 1;
  sys.position[Y_AXIS] = 2;
  sys.position[Z_AXIS] = 3;
  sysInit();
  TEST_ASSERT_EQUAL(0,sys.position[X_AXIS]);
  TEST_ASSERT_EQUAL(0,sys.position[Y_AXIS]);
  TEST_ASSERT_EQUAL(0,sys.position[Z_AXIS]);
}