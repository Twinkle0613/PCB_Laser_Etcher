#ifndef Stepper_H
#define Stepper_H
#include "stm32f10x_tim.h"
#include "Dma.h"
#include "Motor.h"
#include "Planner.h"
#include "string.h"
#include "Tim.h"
#include "SystemMap.h"
#include "System.h"
#include "Motor.h"

#define true 1
#define false 0

typedef struct{
  int32_t error[N_AXIS]; // Error that is used for the bresenham line tracer
  uint32_t eventCount;          // It is equal to delta X in bresenham's algoritm 
  uint32_t steps[N_AXIS]; // It is equal to delta Y in bresenham's algoritm        
  uint32_t stepEventsCompleted;  // The number of step events left in current motion
  uint8_t xyzDireation;
}stepper_t;

extern stepper_t stExecutor;
extern block_t *currentBlock; 
extern uint8_t wakeUpState;
extern uint8_t outputBits; 
void stepperInit(void);
void sleep(void);
void stExecutorInitProcess(void);
uint32_t bresenhamAlgorithm(int32_t* error,uint32_t dx,uint32_t dy, int* status);
uint32_t iterate(int32_t* error,uint32_t dx,uint32_t dy);
uint8_t executeStepDisplacement(int axis);
void executeStepDisplacementProcess(void);
int getDireationFlag(uint8_t reg,int axis);
int getOutputFlagStatus(uint8_t reg,int axis);
uint8_t getStepStatus(int axis);
uint8_t getMoveDireaction(int axis);
void planningDataSending(void);
#endif // Stepper_H
