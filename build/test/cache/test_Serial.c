#include "Planner.h"
#include "Serial.h"
#include "unity.h"






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

  UnityAssertEqualNumber((_U_SINT)(_UU8 )(('a')), (_U_SINT)(_UU8 )((str[0])), (((void *)0)), (_U_UINT)(26), UNITY_DISPLAY_STYLE_UINT8);

  UnityAssertEqualNumber((_U_SINT)(_UU8 )(('b')), (_U_SINT)(_UU8 )((str[1])), (((void *)0)), (_U_UINT)(27), UNITY_DISPLAY_STYLE_UINT8);

  UnityAssertEqualNumber((_U_SINT)(_UU8 )(('c')), (_U_SINT)(_UU8 )((str[2])), (((void *)0)), (_U_UINT)(28), UNITY_DISPLAY_STYLE_UINT8);

  UnityAssertEqualNumber((_U_SINT)(_UU8 )(('d')), (_U_SINT)(_UU8 )((str[3])), (((void *)0)), (_U_UINT)(29), UNITY_DISPLAY_STYLE_UINT8);

}



void xtest_str_reset_(void){

  if ((('a') != (str[0]))) {} else {UnityFail( ((" Expected Not-Equal")), (_U_UINT)((_U_UINT)(33)));};

  if ((('b') != (str[1]))) {} else {UnityFail( ((" Expected Not-Equal")), (_U_UINT)((_U_UINT)(34)));};

  if ((('c') != (str[2]))) {} else {UnityFail( ((" Expected Not-Equal")), (_U_UINT)((_U_UINT)(35)));};

  if ((('d') != (str[3]))) {} else {UnityFail( ((" Expected Not-Equal")), (_U_UINT)((_U_UINT)(36)));};

}



void xtest_strcat_combine_B00_and_X10000_(void){

 char* strB = "B00";

 char* strX = "X10000";

  strcat(str,strB);

  UnityAssertEqualString((const char*)(("B00")), (const char*)((str)), (((void *)0)), (_U_UINT)(43));

  strcat(str,strX);

  UnityAssertEqualString((const char*)(("B00X10000")), (const char*)((str)), (((void *)0)), (_U_UINT)(45));

  strcat(str,"\n");

  UnityAssertEqualString((const char*)(("B00X10000\n")), (const char*)((str)), (((void *)0)), (_U_UINT)(47));

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

  UnityAssertEqualString((const char*)(("B00X10000\n")), (const char*)((str)), (((void *)0)), (_U_UINT)(61));

}





void xtest_itoa_(void){



   int a=54325;

   char buffer[20];

   buffer[0] = 'B';

   itoa(a,buffer+1,10);

   printf("Binary value = %s\n", buffer);



}



void xtest_integerToSting_(void){



  char* string[3][256];

  int i;

  integerToSting('N',100000,(char*)string[0]);

  integerToSting('B',9000,(char*)string[1]);

  integerToSting('C',20000,(char*)string[2]);

  UnityAssertEqualString((const char*)(("N100000")), (const char*)((string[0])), (((void *)0)), (_U_UINT)(82));

  UnityAssertEqualString((const char*)(("B9000")), (const char*)((string[1])), (((void *)0)), (_U_UINT)(83));

  UnityAssertEqualString((const char*)(("C20000")), (const char*)((string[2])), (((void *)0)), (_U_UINT)(84));

  printf("%s\n",string[0]);

  printf("%s\n",string[1]);

  printf("%s\n",string[2]);

  for(i = 0; i < 3; i++){

   strcat(str,(char*)string[i]);

  }

  UnityAssertEqualString((const char*)(("N100000B9000C20000")), (const char*)((str)), (((void *)0)), (_U_UINT)(91));

  printf("%s\n",str);

}

void xtest_convertBlockInformToString_(void){

  block_t *block = &blockBuffer[bufferTail];

  planLineBlock(300,400,100,100,0);

  UnityAssertEqualNumber((_U_SINT)((0x00)), (_U_SINT)((block->directionBits)), (((void *)0)), (_U_UINT)(97), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((60000)), (_U_SINT)((block->steps[0])), (((void *)0)), (_U_UINT)(98), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block->steps[1])), (((void *)0)), (_U_UINT)(99), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((20000)), (_U_SINT)((block->steps[2])), (((void *)0)), (_U_UINT)(100), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((80000)), (_U_SINT)((block->stepEventCount)), (((void *)0)), (_U_UINT)(101), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((15690)), (_U_SINT)((block->nominalRate)), (((void *)0)), (_U_UINT)(102), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->initialRate)), (((void *)0)), (_U_UINT)(103), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((block->finalRate)), (((void *)0)), (_U_UINT)(104), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((314)), (_U_SINT)((block->rateDelta)), (((void *)0)), (_U_UINT)(105), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((18)), (_U_SINT)((block->accelerateUntil)), (((void *)0)), (_U_UINT)(106), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((79982)), (_U_SINT)((block->deccelerateAfter)), (((void *)0)), (_U_UINT)(107), UNITY_DISPLAY_STYLE_INT);

  convertBlockInformToString(block,str);

  UnityAssertEqualString((const char*)(("#B0X60000Y80000Z20000S80000I0F0N15690R314A18D79982\n")), (const char*)((str)), (((void *)0)), (_U_UINT)(109));

}





void xtest_strcomp_(void){

  char* SerialBuffer = "OKB";

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((strcmp(SerialBuffer,"OKB"))), (((void *)0)), (_U_UINT)(115), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((strcmp(SerialBuffer,"OFF"))), (((void *)0)), (_U_UINT)(116), UNITY_DISPLAY_STYLE_INT);

}



void test_serialCommunicationCentre(void){

  planLineBlock(1000,0,0,5000,0);











  serialCommunicationCentre();



}
