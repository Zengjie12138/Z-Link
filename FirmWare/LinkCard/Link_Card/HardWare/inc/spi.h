#ifndef __SPI_H
#define __SPI_H
#include "stm32f1xx_hal.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//SPI驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

extern SPI_HandleTypeDef hspi2;
 				  	    													  
u8  SPI2_WriteData(u8 *data,u16 size);	 
#endif

