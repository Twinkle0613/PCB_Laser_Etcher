#include "unity.h"
#include "Serial.h"
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include "Planner.h"
void setUp(void)
{
  bufferTail = 0;
  planInit();
  memset(blockBuffer, 0, sizeof(blockBuffer));
  memset(str, 0, sizeof(str));
}

void tearDown(void)
{
}



void xtest_str_insert_data_into_str_(void){
  str[0] = 'a';
  str[1] = 'b';
  str[2] = 'c';
  str[3] = 'd';
  TEST_ASSERT_EQUAL_UINT8('a',str[0]);
  TEST_ASSERT_EQUAL_UINT8('b',str[1]);
  TEST_ASSERT_EQUAL_UINT8('c',str[2]);
  TEST_ASSERT_EQUAL_UINT8('d',str[3]);
}

void xtest_str_reset_(void){ 
  TEST_ASSERT_NOT_EQUAL('a',str[0]);
  TEST_ASSERT_NOT_EQUAL('b',str[1]);
  TEST_ASSERT_NOT_EQUAL('c',str[2]);
  TEST_ASSERT_NOT_EQUAL('d',str[3]);
}

void xtest_strcat_combine_B00_and_X10000_(void){
 char* strB = "B00";
 char* strX = "X10000";
  strcat(str,strB);
  TEST_ASSERT_EQUAL_STRING("B00",str);
  strcat(str,strX);
  TEST_ASSERT_EQUAL_STRING("B00X10000",str);
  strcat(str,"\n");
  TEST_ASSERT_EQUAL_STRING("B00X10000\n",str); 
}


void xtest_string_array(void){
  
  char* strB[3];
  strB[0] = "B00";
  strB[1] = "X10000";
  strB[2] = "\n";
  int i;
  for(i = 0; i < 3; i++){
   strcat(str,strB[i]); 
  }
  TEST_ASSERT_EQUAL_STRING("B00X10000\n",str); 
}


void xtest_itoa_(void){
  
   int a=54325;
   char buffer[20];
   buffer[0] = 'B';
   itoa(a,buffer+1,10);   // here 2 means binary
   printf("Binary value = %s\n", buffer);  
  
}

void xtest_integerToSting_(void){
  
  char* string[3][256];
  int i;
  integerToSting('N',100000,(char*)string[0]);
  integerToSting('B',9000,(char*)string[1]);
  integerToSting('C',20000,(char*)string[2]);
  TEST_ASSERT_EQUAL_STRING("N100000",string[0]);
  TEST_ASSERT_EQUAL_STRING("B9000",string[1]);
  TEST_ASSERT_EQUAL_STRING("C20000",string[2]);
  printf("%s\n",string[0]);
  printf("%s\n",string[1]);
  printf("%s\n",string[2]);
  for(i = 0; i < 3; i++){
   strcat(str,(char*)string[i]); 
  }
  TEST_ASSERT_EQUAL_STRING("N100000B9000C20000",str); 
  printf("%s\n",str);
}
void xtest_convertBlockInformToString_(void){
  block_t *block = &blockBuffer[bufferTail];
  planLineBlock(300,400,100,100,0);
  TEST_ASSERT_EQUAL(0x00,block->directionBits);
  TEST_ASSERT_EQUAL(60000,block->steps[X_AXIS]); 
  TEST_ASSERT_EQUAL(80000,block->steps[Y_AXIS]); 
  TEST_ASSERT_EQUAL(20000,block->steps[Z_AXIS]); 
  TEST_ASSERT_EQUAL(80000,block->stepEventCount);
  TEST_ASSERT_EQUAL(15690,block->nominalRate);
  TEST_ASSERT_EQUAL(0,block->initialRate);
  TEST_ASSERT_EQUAL(0,block->finalRate);
  TEST_ASSERT_EQUAL(314,block->rateDelta);
  TEST_ASSERT_EQUAL(18,block->accelerateUntil);
  TEST_ASSERT_EQUAL(79982,block->deccelerateAfter); 
  convertBlockInformToString(block,str);
  TEST_ASSERT_EQUAL_STRING("#B0X60000Y80000Z20000S80000I0F0N15690R314A18D79982\n",str);
}


void xtest_strcomp_(void){
  char* SerialBuffer = "OKB";
  TEST_ASSERT_EQUAL(0,strcmp(SerialBuffer,"OKB"));
  TEST_ASSERT_EQUAL(1,strcmp(SerialBuffer,"OFF"));
}

void test_serialCommunicationCentre(void){
  planLineBlock(1000,0,0,5000,0);
  //planLineBlock(700,0,0,3000,0);
  //planLineBlock(1400,0,0,4000,0);
 // planLineBlock(2000,0,0,5000,0);
  //planLineBlock(2700,0,0,6000,0);

  serialCommunicationCentre();
  
}