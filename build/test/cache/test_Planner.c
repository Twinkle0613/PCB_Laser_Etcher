#include "Planner.h"
#include "unity.h"


void setUp(void)

{

  bufferTail = 0;

  planInit();

}



void tearDown(void)

{

}

void test_nextBlockIndex_Index_point_to_next_block(void){

  int index = 0;

  index = nextBlockIndex(index);

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((index)), (((void *)0)), (_U_UINT)(26), UNITY_DISPLAY_STYLE_INT);

}

void test_nextBlockIndex_Index_point_to_next_block_with_overflow(void){

  int index = 17;

  index = nextBlockIndex(index);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((index)), (((void *)0)), (_U_UINT)(41), UNITY_DISPLAY_STYLE_INT);

}

void test_prevBlockIndex_Index_point_to_previous_block(void){

    int index = 1;

    index = prevBlockIndex(index);

    UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((index)), (((void *)0)), (_U_UINT)(55), UNITY_DISPLAY_STYLE_INT);

}

void test_prevBlockIndex_Index_point_to_previous_block_with_underflow(void){

  int index = 0;

  index = prevBlockIndex(index);

  UnityAssertEqualNumber((_U_SINT)((17)), (_U_SINT)((index)), (((void *)0)), (_U_UINT)(68), UNITY_DISPLAY_STYLE_INT);

}

void test_planInit_initialize_the_bufferHead_bufferTail_and_nextBufferHead(void){

  planInit();

  UnityAssertEqualNumber((_U_SINT)((bufferHead)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(82), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((nextBufferHead)), (_U_SINT)((1)), (((void *)0)), (_U_UINT)(83), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.position[0])), (((void *)0)), (_U_UINT)(84), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.position[1])), (((void *)0)), (_U_UINT)(85), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.position[2])), (((void *)0)), (_U_UINT)(86), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.previousUnitVec[0])), (((void *)0)), (_U_UINT)(87), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.previousUnitVec[1])), (((void *)0)), (_U_UINT)(88), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.previousUnitVec[2])), (((void *)0)), (_U_UINT)(89), UNITY_DISPLAY_STYLE_INT);

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((plExecutor.previousNominalSpeed)), (((void *)0)), (_U_UINT)(90), UNITY_DISPLAY_STYLE_INT);

}

void test_getBlock_to_get_the_block_when_buffer_contained_block(void){

  bufferHead = nextBlockIndex(bufferHead);

  bufferHead = nextBlockIndex(bufferHead);

  bufferHead = nextBlockIndex(bufferHead);

  UnityAssertEqualNumber((_U_SINT)((3)), (_U_SINT)((bufferHead)), (((void *)0)), (_U_UINT)(106), UNITY_DISPLAY_STYLE_INT);

  block_t* currentBlock = getCurrentBlock();

  UnityAssertEqualNumber((_U_SINT)(_UP)((currentBlock)), (_U_SINT)(_UP)((&(blockBuffer[bufferTail]))), (((void *)0)), (_U_UINT)(108), UNITY_DISPLAY_STYLE_HEX32);

}

void test_getBlock_to_get_NULL_when_buffer_dont_have_any_block(void){

  block_t* currentBlock = getCurrentBlock();

  if ((((currentBlock)) == ((void *)0))) {} else {UnityFail( (((" Expected NULL"))), (_U_UINT)((_U_UINT)((_U_UINT)(121))));};

}

void test_discardCurrrentBlock_if_blockBuffer_contain_block_the_buffetTail_will_be_increased(void){

  bufferHead = nextBlockIndex(bufferHead);

  bufferHead = nextBlockIndex(bufferHead);

  bufferHead = nextBlockIndex(bufferHead);

  discardCurrentBlock();

  UnityAssertEqualNumber((_U_SINT)((1)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(139), UNITY_DISPLAY_STYLE_INT);

}

void test_discardCurrrentBlock_if_blockBuffer_doesnt_contain_block_the_buffetTail_will_remain(void){

  discardCurrentBlock();

  UnityAssertEqualNumber((_U_SINT)((0)), (_U_SINT)((bufferTail)), (((void *)0)), (_U_UINT)(153), UNITY_DISPLAY_STYLE_INT);

}
