#include "Protocol.h"

char line[256];
uint8_t charCounter;


void dataProcess(void){
  uint8_t character;
  while((character = readRxBuffer() ) != RX_BUFFER_IS_EMPTY ){
    
    if(character == '\n'){
     lineCheckingProcess();
    }else{
     line[charCounter++] = character;
    }
    
  }
  
}

void lineCheckingProcess(void){
   line[charCounter] = 0;
      printString(line);
   if((availableInsertBlock != 0) && line[0] == '#'){
     interpretLineToBlock(line);
     availableInsertBlock--;
     if(availableInsertBlock){
       printString("OK");
     }
    }
    charCounter = 0;
}


void interpretLineToBlock(char* line){
  block_t *block = &blockBuffer[bufferHead];
  uint8_t charCounter = 0;
  char letter;
  int value = 0;
  
  while(getLetterAndValue(&letter,&value,line,&charCounter)){
    switch(letter){
      case 'B': block->directionBits = value; break;
      case 'X': block->steps[X_AXIS] = value; break;
      case 'Y': block->steps[Y_AXIS] = value; break;
      case 'Z': block->steps[Z_AXIS] = value; break;
      case 'S': block->stepEventCount = value; break;
      case 'I': block->initialRate = value; break;
      case 'F': block->finalRate = value; break;
      case 'N': block->nominalRate = value; break;
      case 'R': block->rateDelta = value; break;
      case 'A': block->accelerateUntil = value; break;
      case 'D': block->deccelerateAfter = value; break;
    }
  }
  
  bufferHead = nextBufferHead;  
  nextBufferHead = nextBlockIndex(bufferHead);
  NVIC_EnableIRQ(TIM2_IRQn); //Wake up TIM Interrupt
}

int getLetterAndValue(char* letter, int *value, char* line, uint8_t *charCounter){
  
  if(line[*charCounter] == 0){
    return 0;  
  }
  
  *letter = line[*charCounter];

  if( ((*letter < 'A') || (*letter > 'Z')) & (*letter != '#')) {
    return 0;
  }

  (*charCounter)++;
   *value = readValue(line,charCounter);
  return 1;
}

int readValue(char* line,uint8_t *charCounter){
  int intval = 0;
  char *ptr = line + *charCounter;
  unsigned char c = *ptr;
  while(1){
    c -= '0';
    if(c <= 9){
      intval = intval*10 + c;
    }else{
      break;
    }
    c = *++ptr;
  }
  *charCounter = ptr - line;
  return intval;
}



