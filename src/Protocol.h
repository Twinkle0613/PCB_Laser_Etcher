#ifndef Protocol_H
#define Protocol_H

//#include "unity.h"
#include "Usart.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_usart.h"

//For Test Driver Development
#include "string.h"
#include "Planner.h"
#include "Stepper.h"

#define isCharacter(x)      (((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'z'))
#define isDigit(x)          ((x) >= '0' && (x) <= '9')
#define isSpace(x)          ((x) == ' ' || (x) == '\t')


extern char line[256];
extern uint8_t charCounter;
int readValue(char* line,uint8_t *charCounter);
int getLetterAndValue(char* letter, int *value, char* line, uint8_t *charCounter);
void interpretLineToBlock(char* line);
void dataProcess(void);
void lineCheckingProcess(void);
#endif // Protocol_H
