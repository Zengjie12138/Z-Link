#ifndef _PULSE_H_
#define _PULSE_H_

#include "stm32f1xx_hal.h"

extern ADC_HandleTypeDef hadc1;

uint16_t pulse_process(uint8_t *BPM);

#endif

