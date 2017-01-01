#ifndef Serial_H
#define Serial_H

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include "Planner.h"

#define DIR_BIT_STR 0      
#define X_STP_STR 1      
#define Y_STP_STR 2      
#define Z_STP_STR 3      
#define SPT_EVENT_STR 4      
#define INI_RATE_STR 5      
#define FINAL_RATE_STR 6      
#define NOMI_RATE_STR 7      
#define RATE_DELTA_STR 8      
#define ACCELE_STP 9
#define DECCELE_STP 10
      
typedef enum {
  INIT,
  WAIT,
  SEND 
}communicationState;

extern char str[256];
void receiveResetCommandFeedBack(HANDLE hSerial);
void closeSerialPort(HANDLE hSerial,LPCSTR portname);
void sendResetCommand(HANDLE hSerial);
DWORD writeToSerialPort(HANDLE hSerial, uint8_t * data, int length);
DWORD readFromSerialPort(HANDLE hSerial, uint8_t * buffer, int buffersize);
HANDLE initSerialComm(LPCSTR portname, DWORD baudrate);
DWORD _readFromSerialPort(HANDLE hSerial, uint8_t buffer[]);
void serialCommunicationCentre(void);
void setCommMaskCheck(HANDLE hSerial);

void integerToSting(char firstChar,uint32_t value,char* str);
void convertBlockInformToString(block_t* block, char* str);
void waitForFeedBack(void);
void sendWakeUp(void);
void stateMachine(void);
void blockSendingProcess(void);
#endif // Serial_H
