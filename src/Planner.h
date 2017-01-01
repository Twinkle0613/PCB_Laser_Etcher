#ifndef Planner_H
#define Planner_H

#include "stdint.h"
#include <math.h>
#include "stdio.h"
#include "string.h"
#include "System.h"
#include "Setting.h"
#define BLOCK_BUFFER_SIZE 18


#define _max(a,b) ((a<b)?b:a)
#define _min(a,b) ((a>b)?b:a)
#define _lround(x) (x+0.5)
// #define _labs(x)  ((x<0)?(-x):x)

typedef struct{
  int32_t position[3];             // The planner position of the tool in absolute steps. Kept separate
                                   // from g-code position for movements requiring multiple line motions,
                                   // i.e. arcs, canned cycles, and backlash compensation.
  float previousUnitVec[3];     // Unit vector of previous path line segment
  float previousNominalSpeed;   // Nominal speed of previous path line segment
}planner_t;

planner_t plExecutor;

typedef struct {
   // Fields used by the bresenham algorithm for tracing the line
   uint8_t directionBits;   
   uint32_t steps[N_AXIS];
   int32_t  stepEventCount; 
   
   // Fields used by the motion planner to manage acceleration
   float nominalSpeed;
   float entrySpeed;
   float millimeters;
   float inverseMillimeters;
   
   // Settings for the trapezoid generator
   uint32_t initialRate;              // The step rate at start of block  
   uint32_t finalRate;                // The step rate at end of block
   int32_t rateDelta;                 // The steps/minute to add or subtract when changing speed (must be positive)
   uint32_t accelerateUntil;          // The index of the step event on which to stop acceleration
   uint32_t deccelerateAfter;          // The index of the step event on which to start decelerating
   uint32_t nominalRate;              // The nominal step rate for this block in step_events/minute
   
}block_t;

extern volatile uint8_t bufferHead;       // Index of the next block to be pushed
extern volatile uint8_t bufferTail;       // Index of the block to process now
extern uint8_t nextBufferHead;   // Index of the next buffer head
extern block_t blockBuffer[BLOCK_BUFFER_SIZE];

void planInit(void);
block_t* getCurrentBlock(void);
uint8_t nextBlockIndex(uint8_t blockIndex);
uint8_t prevBlockIndex(uint8_t blockIndex);

void blockConfig(block_t* block, 
                 uint8_t dir,
                 uint32_t stepX, 
                 uint32_t stepY, 
                 uint32_t stepZ, 
                 uint32_t initialRate, 
                 uint32_t nominalRate,
                 uint32_t finalRate
                 );
float estimateAccelerationStep(float initialRate, float targetRate, float acceleration);
float estimateDeccelerationStep(void);
uint32_t _ceil(float num);
void discardCurrentBlock(void);

void convertXYZMovingDistanceToStepAndStoreToArray(float x,float y,float z,int32_t target[]);
uint8_t determineDirection(int32_t target[]);
long int _labs (long int i);
uint32_t _floor(float num);
void calculateXYZaxisSteps(block_t* block, int32_t target[]);
int32_t getHighestStepsInXYZsteps(block_t* block);
void planLineBlock(float x, float y, float z, float feedRate, uint8_t InvertFeedRateMode);
float getDeltaInMilliMeter(int32_t target, int32_t prevPosition);
void calculateXYZDeltaInMilliMeterAndStoreToArray(int32_t target[], float delta[]);
float getVectorLength(float delta[]); 
float getInverse(float value);
void estimateNominalRateAndSpeed(block_t* block,float feedRate,uint8_t InvertFeedRateMode);
uint32_t getXYZDeltaInSteps(int32_t target[],int8_t axis);
float getUnitVector(block_t* block,float delta[], int8_t axis);
void calculateUnitVectorForXYZaxisAndStoreToArray(block_t* block,float delta[],float unitVector[]);
void blockMovingExecutionPlaning(block_t* block, float entryFactor, float exitFactor);
float getRampStep(float initialRate, float finalRate, float acceleration);
int32_t getNominalStep(int32_t evenStep, int32_t accelStep, int32_t deccelStep);
float getIntersectionDistance(block_t* block);
void ifNominatStepLessThanZeroRecaculateAccelStep(block_t* block,int32_t*nominalStep, int32_t* accelStep);
void configDirectionBist(block_t* block, int32_t target[]);
void saveDataToPlExecutor(float nominalSpeed,int32_t target[], float unitVector[]);
void updatebufferHead(void);
float _getUnitVector(float posX, float posY, float posZ, int8_t axis);
float getCosThetaBetweenTwoVector(float unitVector1[],float unitVector2[]);
void entrySpeedPlanning(block_t* block, float cosTheta);
float _getCosThetaBetweenTwoVector(float unitVector1[],float unitVector2[]);
void replanBlockBufferStructure(void);
void setRateDelta(block_t* block);
float maxAllowableSpeed(float acceleration, float targetVelocity, float distance);
#endif // Planner_H
