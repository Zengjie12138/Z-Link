#ifndef __MPUIIC_H
#define __MPUIIC_H

#include "stm32f1xx_hal.h"
#include "main.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

//IO方向设置
#define MPU_SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define MPU_SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

//IO操作函数
#define MPU_IIC_SCL_RESET()    HAL_GPIO_WritePin(MPU_SCL_GPIO_Port, MPU_SCL_Pin, GPIO_PIN_RESET)		//SCL0
#define MPU_IIC_SCL_SET()      HAL_GPIO_WritePin(MPU_SCL_GPIO_Port, MPU_SCL_Pin, GPIO_PIN_SET) 		//SCL1
#define MPU_IIC_SDA_RESET()    HAL_GPIO_WritePin(MPU_SDA_GPIO_Port, MPU_SDA_Pin, GPIO_PIN_RESET) 		//SDA0
#define MPU_IIC_SDA_SET()      HAL_GPIO_WritePin(MPU_SDA_GPIO_Port, MPU_SDA_Pin, GPIO_PIN_SET) 		//SDA1
#define MPU_READ_SDA()         HAL_GPIO_ReadPin(MPU_SDA_GPIO_Port, MPU_SDA_Pin) 		//输入SDA 

//IIC所有操作函数
void MPU_IIC_Delay(void);				//MPU IIC延时函数
void MPU_IIC_Init(void);                //初始化IIC的IO口
void MPU_IIC_Start(void);				//发送IIC开始信号
void MPU_IIC_Stop(void);	  			//发送IIC停止信号
void MPU_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 MPU_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void MPU_IIC_Ack(void);					//IIC发送ACK信号
void MPU_IIC_NAck(void);				//IIC不发送ACK信号

void IMPU_IC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr, u8 addr);
#endif
















