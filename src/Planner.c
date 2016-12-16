#include "Planner.h"

#define max(a,b) ((a<b)?b:a)
#define min(a,b) ((a>b)?a:b)

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

uint32_t _ceil(float num){
    uint32_t inum = (uint32_t)num;
    if (num == (float)inum) {
        return inum;
    }
    return inum + 1;
}
uint32_t _floor(float num){
  return((uint32_t)num);
}
void blockConfig(block_t* block, 
                 uint8_t dir,
                 uint32_t stepX, 
                 uint32_t stepY, 
                 uint32_t stepZ, 
                 uint32_t initialRate, 
                 uint32_t nominalRate,
                 uint32_t finalRate
                 ){
    block->stepEventCount = max(stepX,max(stepY,stepZ));
    block->directionBits = dir;
    block->steps[X_AXIS] = stepX;
    block->steps[Y_AXIS] = stepY;
    block->steps[Z_AXIS] = stepZ;
    block->initialRate = initialRate;
    block->nominalRate = nominalRate;
    block->finalRate = finalRate;
    block->accelerateUntil = _ceil(estimateAccelerationStep(block->initialRate,block->nominalRate, ACCELERATION*STEP_PER_MM));
    block->deccelerateAfter = block->stepEventCount - block->accelerateUntil;
}

void estimateAccelStepAndDeccelStep(block_t* block){
  int32_t accelStep = _ceil(estimateAccelerationStep(block->initialRate,block->nominalRate, ACCELERATION*STEP_PER_MM));
  int32_t deccelStep = _floor(estimateAccelerationStep(block->nominalRate,block->finalRate, -(ACCELERATION*STEP_PER_MM)));
  int32_t NominalStep = block->stepEventCount - accelStep - deccelStep;
  block->accelerateUntil = accelStep;
  block->deccelerateAfter = NominalStep + accelStep;
}


float estimateDeccelerationStep(void){
  return 0;
};
float estimateAccelerationStep(float initialRate, float targetRate, float acceleration){
 return( ((targetRate*targetRate)-(initialRate*initialRate))/((float)(2.0*acceleration) ));
}




