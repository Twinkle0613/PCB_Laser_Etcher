#ifndef Setting_H
#define Setting_H


#define ACCELERATION 10*60*60 //(min/min^2)
#define STEP_PER_MM 200 //(steps/mm)
#define TIMER_FREQUENCY 72000000 //(Ticks/sec)
#define INCREMENT_RESOLUTION 500 //(steps/min)
#define ACCELERATE_IN_STEP_DIVIDE_60_SEC ((ACCELERATION*STEP_PER_MM)/60)
#define MINIMUM_STEPS_PER_MINUTE 300 //(steps/min)
#define ACCELERATE_TICK_PER_SEC (ACCELERATE_IN_STEP_DIVIDE_60_SEC/INCREMENT_RESOLUTION)
#define CYCLES_PER_ACCELERATION_TICK (TIMER_FREQUENCY/ACCELERATE_TICK_PER_SEC)
#define MINIMUM_SPEED 0.0

#endif // Setting_H
