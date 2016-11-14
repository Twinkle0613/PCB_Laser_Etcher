#include "Planner.h"



block_t blockBuffer[BLOCK_BUFFER_SIZE]; 
volatile uint8_t bufferHead;       // Index of the next block to be pushed
volatile uint8_t bufferTail;       // Index of the block to process now
uint8_t nextBufferHead;   // Index of the next buffer head

void planLineBlock(float x, float y, float z, float feedRate, uint8_t InvertFeedRateMode){
  
  
  
  
}
void discardCurrentBlock(void){
  if (bufferHead != bufferTail) {
    bufferTail = nextBlockIndex( bufferTail );
  }
}

block_t* getCurrentBlock(void){
  if (bufferHead == bufferTail) { return(NULL); }
   return(&blockBuffer[bufferTail]);
}

void planInit(void){
  bufferHead = bufferTail;
  nextBufferHead = nextBlockIndex(bufferHead);
  memset(&plExecutor, 0, sizeof(plExecutor));
}

uint8_t nextBlockIndex(uint8_t blockIndex){
  blockIndex++;
  if (blockIndex == BLOCK_BUFFER_SIZE) { blockIndex = 0; }
  return(blockIndex);
}

uint8_t prevBlockIndex(uint8_t blockIndex){
  if (blockIndex == 0) { blockIndex = BLOCK_BUFFER_SIZE; }
  blockIndex--;
  return(blockIndex);
}