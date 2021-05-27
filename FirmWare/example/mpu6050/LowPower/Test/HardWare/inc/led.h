#ifndef _LED_H_
#define _LED_H_

#include "stm32f1xx_hal.h"

#define Led_Light()  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET)
#define Led_Dark()  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)

#endif
