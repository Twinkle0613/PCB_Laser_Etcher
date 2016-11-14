#include "System.h"
system_t sys;

void sysInit(void){
  sys.position[X_AXIS] = 0;
  sys.position[Y_AXIS] = 0;
  sys.position[Z_AXIS] = 0;
}