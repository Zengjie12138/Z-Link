#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "stdio.h"
#include "stm32f1xx_hal.h"


extern UART_HandleTypeDef huart1;
int fputc(int ch,FILE *f);

#endif

