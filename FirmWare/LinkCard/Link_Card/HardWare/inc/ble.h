#ifndef _BLE_H_
#define _BLE_H_

#include "main.h"
#include "lvgl.h"

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;

extern uint8_t Ble_Buff[BleLength];
extern lv_obj_t *ta;

void USER_UART_IRQHandler(UART_HandleTypeDef *huart);
void USER_UART_IDLECallback(UART_HandleTypeDef *huart);

#endif
