#ifndef System_H
#define System_H

#include <stdint.h>
#define NUMBER_OF_AXIS 3 // Number of axes
#define X_AXIS 0 // Axis indexing value
#define Y_AXIS 1
#define Z_AXIS 2

#define ACCELERATION 10*60*60 //(min/min^2)
#define STEP_PER_MM 80 //(steps/mm)
#define TIMER_FREQUENCY 72000000 //(Ticks/sec)
#define INCREMENT_RESOLUTION 500 //(steps/min)
#define ACCELERATE_IN_STEP_DIVIDE_60_SEC ((ACCELERATION*STEP_PER_MM)/60)
#define MINIMUM_STEPS_PER_MINUTE 300 //(steps/min)
#define ACCELERATE_TICK_PER_SEC (ACCELERATE_IN_STEP_DIVIDE_60_SEC/INCREMENT_RESOLUTION)
#define CYCLES_PER_ACCELERATION_TICK (TIMER_FREQUENCY/ACCELERATE_TICK_PER_SEC)

typedef struct{
  int32_t position[NUMBER_OF_AXIS]; 
  
}system_t;

extern system_t sys;

void sysInit(void);
#endif // System_H
  
