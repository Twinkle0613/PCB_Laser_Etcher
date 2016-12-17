#ifndef System_H
#define System_H

#include <stdint.h>
#define NUMBER_OF_AXIS 3 // Number of axes
#define X_AXIS 0 // Axis indexing value
#define Y_AXIS 1
#define Z_AXIS 2

#define X_STEP_BIT         2  
#define Y_STEP_BIT         3  
#define Z_STEP_BIT         4  
#define X_DIRECTION_BIT    5  
#define Y_DIRECTION_BIT    6  
#define Z_DIRECTION_BIT    7  

#define N_AXIS 3
#define X_AXIS 0 
#define Y_AXIS 1
#define Z_AXIS 2

typedef struct{
  int32_t position[NUMBER_OF_AXIS]; 
}system_t;

extern system_t sys;

void sysInit(void);
#endif // System_H
  
