#include "ble.h"
#include "fatfs.h"
#include "lv_port_fs.h"

uint8_t store_sd = 0;
uint8_t w_update = 0;
uint8_t ble_temp[BleLength] = {0};

void Ble_DataProcess(uint8_t pre_index,uint8_t now_index)
{
	uint8_t i,j,temp_index = 0;
	
	
	if(now_index < pre_index)
	{
		temp_index = now_index + BleLength;

	}
	else{
		temp_index = now_index;
	}
	
	j=0;
	for(i=0;i<BleLength;i++)
	{
		ble_temp[j] = 0;
		j++;
	}
	
	
	j=0;
	for(i=pre_index;i<temp_index;i++)
	{
		ble_temp[j] = Ble_Buff[i%BleLength];
		j++;
	}
	
	if(ble_temp[0] == 'D')//date
	{
		w_update = 1; //天气及时间更新
	}
	else{
		store_sd = 1;//笔记更新
	}
	
}


void USER_UART_IDLECallback(UART_HandleTypeDef *huart)
{
    static uint8_t pre_index,now_index = 0;
    now_index  = BleLength - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);   //当前指向的位置
	Ble_DataProcess(pre_index,now_index);
	pre_index = now_index;
}

void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    if(USART2 == huart2.Instance)                                   //判断是否是串口2
    {
        if(RESET != __HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE))   //判断是否是空闲中断
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart2);                     //清楚空闲中断标志（否则会一直不断进入中断）
            USER_UART_IDLECallback(huart);                          //调用中断处理函数
        }
    }
}
