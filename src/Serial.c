#include "Serial.h"

DWORD dwEvtMask;
HANDLE hSerial = 0;
char SerialBuffer[256];
DWORD size;
char str[256];
void serialCommunicationCentre(void){
  const char *comPort = "COM3";
  hSerial = initSerialComm(comPort, 9600);
  setCommMaskCheck(hSerial);
  sendResetCommand(hSerial);
  receiveResetCommandFeedBack(hSerial);

  
  stateMachine();
  //blockSendingProcess();
 
   
  closeSerialPort(hSerial,comPort);
}

void blockSendingProcess(void){
  sendWakeUp();
  while(bufferHead != bufferTail){
    waitForFeedBack();
    memset(str, 0, sizeof(str));
    convertBlockInformToString(&blockBuffer[bufferTail],str);
    printf("Data: %s\n",str);
    writeToSerialPort(hSerial,str,strlen(str));
    bufferTail = nextBlockIndex(bufferTail);
  }
  
}

void stateMachine(void){
  communicationState state;
  state = INIT;
  while(bufferHead != bufferTail){
    switch(state){
      case INIT:
      sendWakeUp();
      waitForFeedBack();
      state = SEND;
      break;
      
      case SEND:
      memset(str, 0, sizeof(str));
      convertBlockInformToString(&blockBuffer[bufferTail],str);
      writeToSerialPort(hSerial,str,strlen(str));
      bufferTail = nextBlockIndex(bufferTail);
      state = WAIT;
      break;

      
      case WAIT:
      // while(!(WaitCommEvent(hSerial, &dwEvtMask,NULL)) );
      // if(dwEvtMask & EV_RXCHAR){
         // size = _readFromSerialPort(hSerial,SerialBuffer);
         // printf("Received[%d] : %s\n",size,SerialBuffer);
      // }
      
      waitForFeedBack();
      state = SEND;
      break;
      
    }
  }  
}


void waitForFeedBack(void){
  if(bufferHead != bufferTail){
   while(!waitForCorrectSignal());
  }
}

int waitForCorrectSignal(void){
  //memset(SerialBuffer,0,sizeof(SerialBuffer));
  while(!(WaitCommEvent(hSerial, &dwEvtMask,NULL)));
  if(dwEvtMask & EV_RXCHAR){
   size = _readFromSerialPort(hSerial,SerialBuffer);
   if(strcmp(SerialBuffer,"OK") == 0){
     return 1;
   }else{
     return 0;
   }
  }
  return 0;
}

void convertBlockInformToString(block_t* block, char* str){
  int index;
  char* string[11][256];
  printf("---------------------------------------------------\n");
  printf("InitialRate = %d NominalRate = %d FinalRate = %d\n",block->initialRate,block->nominalRate,block->finalRate);
  printf("accelerateUntil = %d deccelerateAfter = %d\n",block->accelerateUntil,block->deccelerateAfter);
  printf("rateDelta = %d stepEventCount = %d\n",block->rateDelta,block->stepEventCount);
  printf("---------------------------------------------------\n");
  memset(str, 0, sizeof(str));
  strcat(str,"#"); 
  integerToSting('B',block->directionBits,(char*)string[DIR_BIT_STR]);
  integerToSting('X',block->steps[X_AXIS],(char*)string[X_STP_STR]);
  integerToSting('Y',block->steps[Y_AXIS],(char*)string[Y_STP_STR]);
  integerToSting('Z',block->steps[Z_AXIS],(char*)string[Z_STP_STR]);
  integerToSting('S',block->stepEventCount,(char*)string[SPT_EVENT_STR]);
  integerToSting('I',block->initialRate,(char*)string[INI_RATE_STR]);
  integerToSting('F',block->finalRate,(char*)string[FINAL_RATE_STR]);
  integerToSting('N',block->nominalRate,(char*)string[NOMI_RATE_STR]);
  integerToSting('R',block->rateDelta,(char*)string[RATE_DELTA_STR]);
  integerToSting('A',block->accelerateUntil,(char*)string[ACCELE_STP]);
  integerToSting('D',block->deccelerateAfter,(char*)string[DECCELE_STP]);
  for(index = 0; index <= 10; index++){
   strcat(str,(char*)string[index]); 
  }
  int strSize = strlen(str);
  str[strSize] = '\n';
  str[strSize+1] = 0;
}

void integerToSting(char firstChar,uint32_t value,char* str){
  str[0] = firstChar;
  itoa(value,&str[1],10);
}

void sendResetCommand(HANDLE hSerial){
   writeToSerialPort(hSerial, "$", 1);
   while(!(WaitCommEvent(hSerial, &dwEvtMask,NULL)) );
}

void sendWakeUp(void){
   writeToSerialPort(hSerial, "!", 1);
}


void receiveResetCommandFeedBack(HANDLE hSerial){
  while(!(WaitCommEvent(hSerial, &dwEvtMask,NULL)) );
   if(dwEvtMask & EV_RXCHAR){
    size = _readFromSerialPort(hSerial,SerialBuffer);
    printf("Received[%d] : %s\n",size,SerialBuffer);
   }
}

void setCommMaskCheck(HANDLE hSerial){
 if(!SetCommMask(hSerial, EV_RXCHAR)) 
  {
   printf("SetCommMask failed with error %d.\n", GetLastError());
   return;
  } 
}

void PrintCommState(DCB dcb)
{
    //  Print some of the DCB structure values
    printf( TEXT("\nBaudRate = %d, ByteSize = %d, Parity = %d, StopBits = %d\n"), 
              dcb.BaudRate, 
              dcb.ByteSize, 
              dcb.Parity,
              dcb.StopBits );
}

HANDLE initSerialComm(LPCSTR portname, DWORD baudrate) {
  COMMTIMEOUTS timeouts={0};
  DCB dcbSerialParams = {0};
  DWORD  accessdirection = GENERIC_READ | GENERIC_WRITE;
  HANDLE hSerial = CreateFile(portname, 
                              accessdirection, 
                              0,  
                              0,  
                              OPEN_EXISTING,
                              0,
                              0);
  if (hSerial == INVALID_HANDLE_VALUE) {
      //call GetLastError(); to gain more information
     DWORD errId = GetLastError();
     printf("Invalid handle. Error: %d\n", errId);
  }

  dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
  if (!GetCommState(hSerial, &dcbSerialParams)) {
    //could not get the state of the comport
  }
//  dcbSerialParams.BaudRate = 460800;
  dcbSerialParams.BaudRate = baudrate;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;
  if(!SetCommState(hSerial, &dcbSerialParams)){
     //analyse error
     DWORD errId = GetLastError();
     printf("SetCommState Error: %d\n", errId);
  }
  
  // The interval 
  timeouts.ReadIntervalTimeout = 50;
  timeouts.ReadTotalTimeoutConstant = 50;
  timeouts.ReadTotalTimeoutMultiplier = 50;
  timeouts.WriteTotalTimeoutConstant = 50;
  timeouts.WriteTotalTimeoutMultiplier = 10;
  if(!SetCommTimeouts(hSerial, &timeouts)){
    //handle error
     DWORD errId = GetLastError();
     printf("SetCommTimeouts Error: %d\n", errId);
  }  
  PrintCommState(dcbSerialParams);  
  printf (TEXT("Serial port %s successfully reconfigured.\n"), portname);
  printf("Opening %s\n", portname);
  return hSerial;  
}

DWORD readFromSerialPort(HANDLE hSerial, uint8_t * buffer, int buffersize) {
  DWORD dwBytesRead = 0;
  if(!ReadFile(hSerial, buffer, buffersize, &dwBytesRead, NULL)){
    DWORD errId = GetLastError(); //handle error
    printf("ReadFile Error: %d\n", errId);
  }
  return dwBytesRead;
}

DWORD _readFromSerialPort(HANDLE hSerial, uint8_t buffer[]){
  DWORD dwBytesRead = 0;
  char TempChar;
  int i = 0;
  do{
    
  if(!ReadFile(hSerial,&TempChar, sizeof(TempChar), &dwBytesRead, NULL)){
    DWORD errId = GetLastError(); //handle error
    printf("ReadFile Error: %d\n", errId);
  }
  if(dwBytesRead>0){
    buffer[i] = TempChar;
    i++;
    //printf("[%d]dwBytesRead = %d temp = %c\n",i,dwBytesRead,TempChar);
  }
  }while(dwBytesRead>0);
  buffer[i] = 0;
  return i;
}


DWORD writeToSerialPort(HANDLE hSerial, uint8_t * data, int length){
	DWORD dwBytesRead = 0;
	if(!WriteFile(hSerial, data, length, &dwBytesRead, NULL)){
    DWORD errId = GetLastError();
    printf("WriteFile Error: %d\n", errId);
	}
	return dwBytesRead;
}

void closeSerialPort(HANDLE hSerial,LPCSTR portname) {
  printf("Closing %s\n",portname); 
	CloseHandle(hSerial);
}