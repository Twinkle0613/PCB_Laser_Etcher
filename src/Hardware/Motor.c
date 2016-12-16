#include "Motor.h"

MotorInfo* motor2;
MotorInfo* motor1;
MotorInfo* motor0;


void motorInit(void){
    motorX  = motorConfig(MOTOR_RIGHT,MOTOR_EIGHTH_STEP);
	motorY  = motorConfig(MOTOR_RIGHT,MOTOR_EIGHTH_STEP);
	motorZ  = motorConfig(MOTOR_RIGHT,MOTOR_EIGHTH_STEP);
}

MotorInfo* motorConfig(uint8_t direation,uint8_t microstep){
	MotorInfo* whichMotor = malloc(sizeof(MotorInfo));
	whichMotor->direation = direation;
	whichMotor->microstep = microstep;
	whichMotor->motorState = MOTOR_ENABLE;
	whichMotor->reset = MOTOR_RESET_DISABLE;
	whichMotor->sleep = MOTOR_SLEEP_MODE_DISABLE;
	whichMotor->step = MOTOR_STEP_LOW;
	return whichMotor;
}

uint8_t getMotorInfo(MotorInfo* whichMotor,uint8_t direation ,uint8_t step){
  whichMotor->direation = direation;
	uint8_t whichMotorInfo = (whichMotor->direation
	         |whichMotor->microstep
	         |whichMotor->sleep
	         |whichMotor->reset
	         |whichMotor->motorState);
	if(step == MOTOR_STEP_HIGH){
		return whichMotorInfo|MOTOR_STEP_HIGH;
	}else if(step == MOTOR_STEP_LOW){
		return whichMotorInfo&MOTOR_STEP_LOW;
	}else{
		return 0;
	}
}
