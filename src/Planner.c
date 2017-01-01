#include "Planner.h"



block_t blockBuffer[BLOCK_BUFFER_SIZE]; 
volatile uint8_t bufferHead;       // Index of the next block to be pushed
volatile uint8_t bufferTail;       // Index of the block to process now
uint8_t nextBufferHead;   // Index of the next buffer head

void planLineBlock(float x, float y, float z, float feedRate, uint8_t InvertFeedRateMode){
  block_t *block = &blockBuffer[bufferHead];
  int32_t target[3];
  float delta[3];
  float unitVector[3];
  convertXYZMovingDistanceToStepAndStoreToArray(x,y,z,target); // convert the movement distance to number of step for stepper motor
  calculateXYZDeltaInMilliMeterAndStoreToArray(target,delta); // Calculate Delta X, Y and X in mm and store to delta[]
  calculateXYZaxisSteps(block,target);  // Calculate Delta X, Y and X in steps and store to targe[]
  configDirectionBist(block,target);
  block->stepEventCount = getHighestStepsInXYZsteps(block); // To find which the steps of axis is highest
  if(block->stepEventCount == 0){return;}
  block->millimeters = getVectorLength(delta);  // Calulate the magnitude of vector. 
  block->inverseMillimeters = getInverse(block->millimeters); // inverse the magnitude of vector.
  estimateNominalRateAndSpeed(block,feedRate,InvertFeedRateMode); // Calculate nominalRate and nominalSpeed
  setRateDelta(block);
  calculateUnitVectorForXYZaxisAndStoreToArray(block,delta,unitVector); // Calculate XYZ unitVector
  float cosTheta = getCosThetaBetweenTwoVector(plExecutor.previousUnitVec,unitVector);
  entrySpeedPlanning(block,cosTheta);
  saveDataToPlExecutor(block->nominalSpeed,target,unitVector);
  updatebufferHead();
  replanBlockBufferStructure();
}


/*
                                   stepEvent (block->stepEventCount)
                 Acceleration * -------------
                                 vectorLength (block->millimeters)
   RateDelta = ----------------------------------
                      60 sec * _ACCELERATE_TICK_PER_SEC

*/
void setRateDelta(block_t* block){
  //printf("%f\n",(block->stepEventCount*block->inverseMillimeters*ACCELERATION)/(60*_ACCELERATE_TICK_PER_SEC));
  block->rateDelta = _floor((block->stepEventCount*block->inverseMillimeters*ACCELERATION)/(60*_ACCELERATE_TICK_PER_SEC));
}
void replanBlockBufferStructure(void){
  uint8_t blockIndex = bufferTail;
  block_t *current;
  block_t *next = NULL;
  while(blockIndex != bufferHead){
  current = next;
  next = &blockBuffer[blockIndex];
  if(current != NULL){
   blockMovingExecutionPlaning(current,current->entrySpeed/current->nominalSpeed,next->entrySpeed/current->nominalSpeed);
  }
  blockIndex = nextBlockIndex(blockIndex); 
  }
  blockMovingExecutionPlaning(next,next->entrySpeed/next->nominalSpeed,MINIMUM_SPEED/next->nominalSpeed);
}

void entrySpeedPlanning(block_t* block, float cosTheta){
    block->entrySpeed = MINIMUM_SPEED;
    if(bufferHead != bufferTail){
     if(cosTheta < 0.95){ 
       block->entrySpeed = _min(plExecutor.previousNominalSpeed,block->nominalSpeed);
     }
    
    }
    float AllowableSpeed = maxAllowableSpeed(-ACCELERATION,MINIMUM_SPEED,block->millimeters);
   block->entrySpeed = _min(block->entrySpeed,AllowableSpeed);
}

float getCosThetaBetweenTwoVector(float unitVector1[],float unitVector2[]){
  return -(unitVector1[X_AXIS]*unitVector2[X_AXIS] + unitVector1[Y_AXIS]*unitVector2[Y_AXIS] + unitVector1[Z_AXIS]*unitVector2[Z_AXIS]);
}



void configDirectionBist(block_t* block, int32_t target[]){
   block->directionBits = 0; //reset directionBits
  block->directionBits |= determineDirection(target);  // block->directionBits is a status register that is used to indicate the motor moving direaction
}


void updatebufferHead(void){
  bufferHead = nextBufferHead;  // Update buffer head and next buffer head indices
  nextBufferHead = nextBlockIndex(bufferHead);
}
void saveDataToPlExecutor(float nominalSpeed,int32_t target[], float unitVector[]){
  plExecutor.previousNominalSpeed = nominalSpeed; // update planner executor nominal speed
  memcpy(plExecutor.previousUnitVec,unitVector,sizeof(float)*3); // update planner executor previous path unit vector
  memcpy(plExecutor.position,target,sizeof(int32_t)*3); // update planner executor position
}

void blockMovingExecutionPlaning(block_t* block, float entryFactor, float exitFactor){
  block->initialRate = _ceil(block->nominalRate*entryFactor);
  block->finalRate = _ceil(block->nominalRate*exitFactor);
  int32_t accelerationPerMinute = block->rateDelta*_ACCELERATE_TICK_PER_SEC*60.0;
  printf("accelerationPerMinute = %d\n",accelerationPerMinute);
  int32_t accelStep = _ceil(getRampStep(block->initialRate,block->nominalRate,accelerationPerMinute));
  int32_t deccelStep = _floor(getRampStep(block->nominalRate,block->finalRate,-accelerationPerMinute));
  printf("accelStep = %d deccelStep = %d\n",accelStep,deccelStep);
  int32_t nominalStep = getNominalStep(block->stepEventCount,accelStep,deccelStep);
  ifNominatStepLessThanZeroRecaculateAccelStep(block,&nominalStep,&accelStep);
  block->accelerateUntil = accelStep;
  block->deccelerateAfter = accelStep+nominalStep;
  printf("accelerateUntil = %d deccelerateAfter = %d\n",block->accelerateUntil,block->deccelerateAfter);
}

void ifNominatStepLessThanZeroRecaculateAccelStep(block_t* block,int32_t*nominalStep, int32_t* accelStep){

    if(*nominalStep < 0){
    *accelStep = _ceil(getIntersectionDistance(block));
    *nominalStep = 0;
  }
}


/*                        + <- some maximum rate we don't care about
                         /|\
                        / | \                    
                       /  |  + <- final_rate     
                      /   |  |                   
     initial_rate -> +----+--+                   
                       X1 ^X2^                   
                          |  |                   
      intersection_distance  Distance  
               
      Distance = X1(intersection_distance) + X2
      
      X1 = (MaxRate^2 - initialRate^2)/(2*accel)
      X2 = (MaxRate^2 - finalRate^2)/(2*accel)
      
      Distance*(2*accel) = 2*MaxRate^2 - initialRate^2 - finalRate^2
      MaxRate^2 = (2*accel*Distance + initialRate^2 + finalRate^2)/2
      
             2*accel*distance + InitialRate^2 + FinalRate*2    2*InitialRate^2
             ---------------------------------------------  - ---------------
                                2                                  2
      X1 =       ---------------------------------------------------------------
                                             2*accel
                                             
                    2*accel*distance - InitialRate^2 + finalRate^2
      X1 =          ------------------------------------------------
                                        4*accel                   
         
*/

float getIntersectionDistance(block_t* block){
  float initialRate = block->initialRate;
  float finalRate = block->finalRate;
  float distance = block->stepEventCount;
  float acceleration = ACCELERATION*STEP_PER_MM;
  float intersectionDistance = ((2*acceleration*distance - initialRate*initialRate + finalRate*finalRate)/(4*acceleration));
  intersectionDistance = _min(intersectionDistance,block->stepEventCount);
  intersectionDistance = _max(intersectionDistance,0);
  
  return intersectionDistance;
}

int32_t getNominalStep(int32_t evenStep, int32_t accelStep, int32_t deccelStep){
  return (evenStep - accelStep - deccelStep);
}

/*

                        +---<-nominalRate
                      / |
                    /   |
     initialRate->+     |
                  |     |
                  +-----+---
                       time

      nominalRate = accel*time + initialRate
      time = (nominalRate - initialRate)/accel
      
     rampStep = initialRate*time + (nominalRate - initialRate)*time/2
              = time(initialRate + (nominalRate - initialRate)/2)
              = time((initialRate + nominalRate)/2)
                
                nominalRate - initialRate      initialRate + nominalRate
     rampStep =  -------------------------  X   -------------------------
                          accel                           2

                nominalRate^2 + initialRate^2
     rampStep = ----------------------------
                         2*accel     
*/


float getRampStep(float initialRate, float targetRate, float acceleration){
  return( ((targetRate*targetRate)-(initialRate*initialRate))/((float)(2.0*acceleration) ));
}

float maxAllowableSpeed(float acceleration, float targetVelocity, float distance) 
{
  return( sqrt(targetVelocity*targetVelocity-2*acceleration*distance) );
}

void convertXYZMovingDistanceToStepAndStoreToArray(float x,float y,float z,int32_t target[]){
   target[X_AXIS] = _lround(x*STEP_PER_MM);
   target[Y_AXIS] = _lround(y*STEP_PER_MM);
   target[Z_AXIS] = _lround(z*STEP_PER_MM);
}

uint8_t determineDirection(int32_t target[]){
  uint8_t directionBits = 0;
  if(plExecutor.position[X_AXIS] > target[X_AXIS]){directionBits |= (1<<X_DIRECTION_BIT);};
  if(plExecutor.position[Y_AXIS] > target[Y_AXIS]){directionBits |= (1<<Y_DIRECTION_BIT);};
  if(plExecutor.position[Z_AXIS] > target[Z_AXIS]){directionBits |= (1<<Z_DIRECTION_BIT);};
  return directionBits;
}

long int _labs (long int i){
   return i < 0 ? -i : i;
}

void calculateXYZaxisSteps(block_t* block, int32_t target[]){
  block->steps[X_AXIS] = getXYZDeltaInSteps(target,X_AXIS);
  block->steps[Y_AXIS] = getXYZDeltaInSteps(target,Y_AXIS);
  block->steps[Z_AXIS] = getXYZDeltaInSteps(target,Z_AXIS);
}

uint32_t getXYZDeltaInSteps(int32_t target[],int8_t axis){
  return _labs(target[axis] - plExecutor.position[axis]);
}

int32_t getHighestStepsInXYZsteps(block_t* block){
   return _max(block->steps[X_AXIS],_max(block->steps[Y_AXIS],block->steps[Z_AXIS]));
}


float getDeltaInMilliMeter(int32_t target, int32_t prevPosition){
  return (((target - prevPosition)/(float)STEP_PER_MM));  
}

void calculateXYZDeltaInMilliMeterAndStoreToArray(int32_t target[], float delta[]){
  delta[X_AXIS] = getDeltaInMilliMeter(target[X_AXIS],plExecutor.position[X_AXIS]);
  delta[Y_AXIS] = getDeltaInMilliMeter(target[Y_AXIS],plExecutor.position[Y_AXIS]);
  delta[Z_AXIS] = getDeltaInMilliMeter(target[Z_AXIS],plExecutor.position[Z_AXIS]);
}

float getVectorLength(float delta[]){
  return(sqrt((delta[X_AXIS]*delta[X_AXIS])+(delta[Y_AXIS]*delta[Y_AXIS])+(delta[Z_AXIS]*delta[Z_AXIS])) );   
}

float getInverse(float value){
  return (1.0/value);
}
void estimateNominalRateAndSpeed(block_t* block,float feedRate,uint8_t InvertFeedRateMode){
  float inverseMinute;
  if(!InvertFeedRateMode){
    inverseMinute = feedRate * block->inverseMillimeters;
  }else{
    //inverseMinute = 1.0/feedRate;
  }
  block->nominalSpeed = block->millimeters * inverseMinute;  
  block->nominalRate = _ceil(block->stepEventCount*inverseMinute);  // stepSpeed(step/min) = (stepEventCount(steps)/vectorLength(mm)) * feedRate(mm/min)
}

float getUnitVector(block_t* block,float delta[], int8_t axis){
  return (delta[axis]*block->inverseMillimeters);
}

void calculateUnitVectorForXYZaxisAndStoreToArray(block_t* block,float delta[],float unitVector[]){
  unitVector[X_AXIS] = getUnitVector(block,delta,X_AXIS);
  unitVector[Y_AXIS] = getUnitVector(block,delta,Y_AXIS);
  unitVector[Z_AXIS] = getUnitVector(block,delta,Z_AXIS);
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
// Testing purpose
void blockConfig(block_t* block, 
                 uint8_t dir,
                 uint32_t stepX, 
                 uint32_t stepY, 
                 uint32_t stepZ, 
                 uint32_t initialRate, 
                 uint32_t nominalRate,
                 uint32_t finalRate
                 ){
    block->stepEventCount = _max(stepX,_max(stepY,stepZ));
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
// Testing purpose
float _getUnitVector(float posX, float posY, float posZ, int8_t axis){
  if(axis == X_AXIS){
    return posX/ sqrt(posX*posX+posY*posY+posZ*posZ);
  }else if(axis == Y_AXIS){
    return posY/ sqrt(posX*posX+posY*posY+posZ*posZ);
  }else if(axis == Z_AXIS){
    return posZ/ sqrt(posX*posX+posY*posY+posZ*posZ);
  }
  
}


