#ifndef __SPI_H
#define __SPI_H
#include "stm32f1xx_hal.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//SPI���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

extern SPI_HandleTypeDef hspi2;
 				  	    													  
u8  SPI2_WriteData(u8 *data,u16 size);	 
#endif

