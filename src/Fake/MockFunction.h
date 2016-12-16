#ifndef MockFunction_H
#define MockFunction_H
#include <stdint.h>
#include "stm32f10x_usart.h"
#include "stm32f10x.h"

uint16_t USART_ReceiveData(USART_TypeDef* USARTx);

#endif // MockFunction_H
