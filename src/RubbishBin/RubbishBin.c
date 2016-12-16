#include "RubbishBin.h"


void test_executeStepDisplament_XYZ_direations_are_positive_(void){
  int8_t output;
  block_t newBlock;
  int32_t errorX,errorY,errorZ;
  newBlock.stepEventCount = 20;
  newBlock.directionBits = 0x00;
  newBlock.steps[X_AXIS] = 20;
  newBlock.steps[Y_AXIS] = 10;
  newBlock.steps[Z_AXIS] = 15;  
  transferInfoToStExecutor(&newBlock);
  int positionX = 0,positionY = 0,positionZ = 0;
  int stepXstatus = 0,stepYstatus = 0,stepZstatus = 0;
  errorX = -(stExecutor.eventCount>>1);
  errorY = -(stExecutor.eventCount>>1);
  errorZ = -(stExecutor.eventCount>>1);
  double z;
  int i;
  
  for(i = 1; i <= stExecutor.eventCount; i++){
    output =  newBlock.directionBits;
    //positionX = (stExecutor.steps[X_AXIS] *i/stExecutor.eventCount);
    positionX += bresenhamAlgorithm(&errorX,stExecutor.eventCount,stExecutor.steps[X_AXIS],&stepXstatus);
    output |= executeStepDisplacement(X_AXIS);
    TEST_ASSERT_EQUAL(positionX,sys.position[X_AXIS]);
    TEST_ASSERT_EQUAL(stepXstatus,getOutputFlagStatus(output,X_AXIS));
    TEST_ASSERT_EQUAL(0,getDireation(output,X_AXIS));
    //printf("positionX = %d,sys.position[X_AXIS] = %d ",positionX,sys.position[X_AXIS]);
    //printf("output = %d ",getOutputFlagStatus(output,X_AXIS));
     //positionY = (stExecutor.steps[Y_AXIS] *i/stExecutor.eventCount);
     positionY += bresenhamAlgorithm(&errorY,stExecutor.eventCount,stExecutor.steps[Y_AXIS],&stepYstatus);
     output |= executeStepDisplacement(Y_AXIS);
     TEST_ASSERT_EQUAL(positionY,sys.position[Y_AXIS]);
     TEST_ASSERT_EQUAL(stepYstatus,getOutputFlagStatus(output,Y_AXIS));
     TEST_ASSERT_EQUAL(0,getDireation(output,Y_AXIS));
     //printf("positionY = %d,sys.position[Y_AXIS] = %d \n",positionY,sys.position[Y_AXIS]);
     //printf("outputY = %d ",getOutputFlagStatus(output,Y_AXIS));
    //positionZ = (stExecutor.steps[Z_AXIS] *i/(stExecutor.eventCount));
     positionZ +=  bresenhamAlgorithm(&errorZ,stExecutor.eventCount,stExecutor.steps[Z_AXIS],&stepZstatus);
     output |= executeStepDisplacement(Z_AXIS);
     TEST_ASSERT_EQUAL(positionZ,sys.position[Z_AXIS]);
     TEST_ASSERT_EQUAL(stepZstatus,getOutputFlagStatus(output,Z_AXIS));
     TEST_ASSERT_EQUAL(0,getDireation(output,Z_AXIS));
     // printf("positionZ = %d,sys.position[Z_AXIS] = %d ",positionZ,sys.position[Z_AXIS]);
     // printf("outputZ = %x\n",getOutputFlagStatus(output,Z_AXIS));
  } 

}


 /* stExecutor.error[axis] += steps;
  
  if(stExecutor.error[axis] > 0 ){
    
    stExecutor.error[axis] -= eventCount;
    output = (1 << stepBit);
    
    if(stExecutor.xyzDireation & (1<<direactionBit) ){
       sys.position[axis]--;
    }else{
       sys.position[axis]++;
    }
  }
  return output;
  */

    // printf("positionX = %d,sys.position[X_AXIS] = %d ",positionZ,sys.position[X_AXIS]);
    // printf("positionY = %d,sys.position[Y_AXIS] = %d ",positionY,sys.position[Y_AXIS]);
    // printf("positionZ = %d,sys.position[Z_AXIS] = %d ",positionZ,sys.position[Z_AXIS]);
     // printf("outputX = %d ",getOutputFlagStatus(outputBits,X_AXIS));
     // printf("outputY = %d ",getOutputFlagStatus(outputBits,Y_AXIS));
     // printf("outputZ = %d ",getOutputFlagStatus(outputBits,Z_AXIS));
     // printf("binary  = %s \n",byte_to_binary(outputBits));  
  

/*
void updateMotorDriveBuffer(int state){
	if(state == Motor_Step_High){
		 motorDriveBuffer[X_MOTOR] = motorInfoBuffer[3];
		 motorDriveBuffer[Y_MOTOR] = motorInfoBuffer[4];
		 motorDriveBuffer[Z_MOTOR] = motorInfoBuffer[5];
		 NVIC_DisableIRQ(DMA1_Channel3_IRQn);
	}else if(state == Motor_Step_Low){
		 motorDriveBuffer[X_MOTOR] = motorInfoBuffer[0];
		 motorDriveBuffer[Y_MOTOR] = motorInfoBuffer[1];
		 motorDriveBuffer[Z_MOTOR] = motorInfoBuffer[2];
		 NVIC_EnableIRQ(DMA1_Channel3_IRQn);
	}
	setDataNumber(DMA1_Channel3,NUMBER_OF_MOTOR);
}


*/
/*	uint32_t checkCCR = DMA1_Channel3->CCR;
	uint32_t checkCNDTR = DMA1_Channel3->CNDTR;
    uint32_t checkCMAR = DMA1_Channel3->CMAR;
    uint32_t checkCPAR= DMA1_Channel3->CPAR; */
  
  
  //printf("motorInfoBuffer[Y_MOTOR_RESET] = %d, motorInfoBuffer[Y_MOTOR_SET] = %d\n",motorInfoBuffer[Y_MOTOR_RESET],motorInfoBuffer[Y_MOTOR_SET]);
  
  
   // static int i;
 // if(i = (~i)){
   // GPIO_SetBits(GPIOC, GPIO_Pin_13); // LED ON
 // }else{
   // GPIO_ResetBits(GPIOC, GPIO_Pin_13); // LED OFF
 // }
 
 
// estimateAccelerationStep
 // printf("initialRate = %f\n",initialRate);
  // printf("targetRate = %f\n",targetRate);
   //float i = (initialRate + (targetRate-initialRate)/2.0);
  // printf("i = %f\n",i);
  // float j = targetRate/(float)acceleration;
  // printf("j = %f\n",j);
  // float z = j*i;
  // printf("z = %f\n", z/2.0);
  //return ((((targetRate+initialRate)/2.0)*(targetRate/(float)(acceleration)))/2.0);
  // printf("return = %f\n",((targetRate*targetRate)-(initialRate*initialRate))/((float)(2.0*acceleration) ));
  
  
  
  	// uint16_t checkUSART1_CR1= USART1->CR1;
	// uint16_t checkUSART1_CR2= USART1->CR2;
    // uint16_t checkUSART1_CR3= USART1->CR3;
	// uint16_t checkUSART1_BRR= USART1->BRR;
    // uint32_t checkGPIOA_CRH = GPIOA->CRH;
    
    
    
    	 //bufferHead = nextBlockIndex(bufferHead);
	//bufferHead = nextBlockIndex(bufferHead);
	//bufferHead = nextBlockIndex(bufferHead);
	//fferHead = nextBlockIndex(bufferHead);
	//blockConfig(&blockBuffer[bufferTail+3],0xe0,20000,3000,1,80000,160000,30);
	//blockConfig(&blockBuffer[bufferTail+2],0x00,20000,3000,1,80000,160000,30);
	//blockConfig(&blockBuffer[bufferTail+1],0xe0,20000,3000,1,80000,160000,30);
	//blockConfig(&blockBuffer[bufferTail],0x00,50000,3000,1,80000,160000,30);