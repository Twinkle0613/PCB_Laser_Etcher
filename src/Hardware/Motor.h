#ifndef Motor_H
#define Motor_H

#include <stdint.h>
#include <malloc.h>

typedef struct{
  uint8_t direation;
  uint8_t step;
  uint8_t sleep;
  uint8_t reset;
  uint8_t microstep;
  uint8_t motorState;
}MotorInfo;

extern MotorInfo* motor2;
extern MotorInfo* motor1;
extern MotorInfo* motor0;


#define motorX motor0
#define motorY motor1
#define motorZ motor2

#define MOTOR_STEP_HIGH  ((uint8_t)0x40)
#define MOTOR_STEP_LOW   ((uint8_t)0xBF)

#define MOTOR_LEFT       ((uint8_t)0x80)
#define MOTOR_RIGHT      ((uint8_t)0x00)

#define MOTOR_SLEEP_MODE_ENABLE   ((uint8_t)0x00)
#define MOTOR_SLEEP_MODE_DISABLE  ((uint8_t)0x20)

#define MOTOR_RESET_ENABLE  ((uint8_t)0x00)
#define MOTOR_RESET_DISABLE ((uint8_t)0x10)

#define MOTOR_FULL_STEP      ((uint8_t)0x00)
#define MOTOR_HALF_STEP      ((uint8_t)0x08)
#define MOTOR_QUATER_STEO    ((uint8_t)0x04)
#define MOTOR_EIGHTH_STEP    ((uint8_t)0x0C)
#define MOTOR_SIXTEENTH_STEP ((uint8_t)0x0E)

#define MOTOR_ENABLE   ((uint8_t)0x00)
#define MOTOR_DISABLE  ((uint8_t)0x01)

MotorInfo* motorConfig(uint8_t direation,uint8_t microstep);
uint8_t getMotorInfo(MotorInfo* whichMotor,uint8_t step,uint8_t direation);

#endif // Motor_H
