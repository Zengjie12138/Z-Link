#ifndef __LCD_H
#define __LCD_H
#include "stm32f1xx_hal.h"
#include "main.h"

/*********************************************************************************
			  ___   _     _____  _____  _   _  _____  _____  _   __
			 / _ \ | |   |_   _||  ___|| \ | ||_   _||  ___|| | / /
			/ /_\ \| |     | |  | |__  |  \| |  | |  | |__  | |/ /
			|  _  || |     | |  |  __| | . ` |  | |  |  __| |    \
			| | | || |_____| |_ | |___ | |\  |  | |  | |___ | |\  \
			\_| |_/\_____/\___/ \____/ \_| \_/  \_/  \____/ \_| \_/

 *	******************************************************************************
 *	本程序只供学习使用，未经作者许可，不得用于其它任何用途
 *	ALIENTEK MINI STM32开发板
 *	1.3寸 240*240 TFT_LCD驱动代码
 *	正点原子@ALIENTEK
 *	技术论坛:www.openedv.com
 *	创建日期:2018/10/27
 *	版本：V1.0
 *	版权所有，盗版必究。
 *	Copyright(C) 广州市星翼电子科技有限公司 2014-2024
 *	All rights reserved
 *	******************************************************************************
 *	初始版本
 *	******************************************************************************/
 
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define LCD_RES_Clr()  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);//RES
#define LCD_RES_Set()  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);

#define LCD_DC_Clr()   HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_RESET);//DC
#define LCD_DC_Set()   HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_SET);
 		     
#define LCD_CS_Clr()   HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);//CS
#define LCD_CS_Set()   HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);

#define LCD_BLK_Clr()  HAL_GPIO_WritePin(LCD_PWR_GPIO_Port, LCD_PWR_Pin, GPIO_PIN_RESET);//BLK
#define LCD_BLK_Set()  HAL_GPIO_WritePin(LCD_PWR_GPIO_Port, LCD_PWR_Pin, GPIO_PIN_SET);

extern u16	POINT_COLOR;	//默认画笔颜色
extern u16	BACK_COLOR;		//默认背景颜色

//LCD的宽和高定义
#define LCD_Width 	240
#define LCD_Height 	240

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)



/*
    	LCD_PWR:	PG8
    	LCD_RST:	PG6
    	LCD_WR:		PB14
    	LCD_CS:		PG7
*/
#define	LCD_PWR		PGout(8)
#define	LCD_RST		PGout(6)
#define	LCD_WR		PBout(14)
#define	LCD_CS		PGout(7)

void LCD_Gpio_Init(void);
void LCD_Init(void);																	//初始化
void LCD_DisplayOn(void);																//开显示
void LCD_DisplayOff(void);																//关显示
void LCD_Write_HalfWord(const u16 da);													//写半个字节数据到LCD
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2);									//设置数据显示区域
void LCD_Clear(u16 color);																//清屏
void LCD_Fill(u16 x_start, u16 y_start, u16 x_end, u16 y_end, u16 color);				//填充单色
void LCD_Draw_Point(u16 x, u16 y);														//画点
void LCD_Draw_ColorPoint(u16 x, u16 y,u16 color);										//画带颜色点
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);										//画线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);									//画矩形
void LCD_Draw_Circle(u16 x0, u16 y0, u8 r);												//画圆
void LCD_ShowChar(u16 x, u16 y, char chr, u8 size);										//显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);									//显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);							//显示数字
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,char *p);					//显示字符串
void LCD_Show_Image(u16 x, u16 y, u16 width, u16 height, const u8 *p);					//显示图片
void Display_ALIENTEK_LOGO(u16 x,u16 y);												//显示ALIENTEK LOGO

#endif


