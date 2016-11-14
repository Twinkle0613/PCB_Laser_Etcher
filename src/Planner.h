#ifndef Planner_H
#define Planner_H

#include "stdint.h"
#include "string.h"
#include "SystemMap.h"
#define BLOCK_BUFFER_SIZE 18

typedef struct{
  int32_t position[3];             // The planner position of the tool in absolute steps. Kept separate
                                   // from g-code position for movements requiring multiple line motions,
                                   // i.e. arcs, canned cycles, and backlash compensation.
  float previousUnitVec[3];     // Unit vector of previous path line segment
  float previousNominalSpeed;   // Nominal speed of previous path line segment
}planner_t;

planner_t plExecutor;

typedef struct {
   uint8_t directionBits;   
   uint32_t steps[N_AXIS];
   int32_t  stepEventCount; 
}block_t;

extern volatile uint8_t bufferHead;       // Index of the next block to be pushed
extern volatile uint8_t bufferTail;       // Index of the block to process now
extern uint8_t nextBufferHead;   // Index of the next buffer head
extern block_t blockBuffer[BLOCK_BUFFER_SIZE];



void planInit(void);
block_t* getCurrentBlock(void);
uint8_t nextBlockIndex(uint8_t blockIndex);
uint8_t prevBlockIndex(uint8_t blockIndex);

#endif // Planner_H
