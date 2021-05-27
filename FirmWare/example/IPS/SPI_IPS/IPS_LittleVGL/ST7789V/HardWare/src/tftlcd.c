#include "tftlcd.h"
#include "spi.h"

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

//LCD缓存大小设置，修改此值时请注意！！！！修改这两个值时可能会影响以下函数	LCD_Clear/LCD_Fill/LCD_DrawLine
#define LCD_TOTAL_BUF_SIZE	(240*240*2)
#define LCD_Buf_Size 1152
static u8 lcd_buf[LCD_Buf_Size];

u16	POINT_COLOR = BLACK;	//画笔颜色	默认为黑色
u16	BACK_COLOR 	= WHITE;	//背景颜色	默认为白色

/**
 * @brief	LCD控制接口初始化
 *
 * @param   void
 *
 * @return  void
 */
void LCD_Gpio_Init(void)
{
    LCD_CS_Clr();
    LCD_BLK_Clr();

    LCD_RES_Clr();
    HAL_Delay(120);
    LCD_RES_Set();
}


/**
 * @brief	LCD底层SPI发送数据函数
 *
 * @param   data	数据的起始地址
 * @param   size	发送数据大小
 *
 * @return  void
 */
static void LCD_SPI_Send(u8 *data, u32 size)
{
    u32 i;
    u32 delta;
    
    delta = size/0xFFFF;
    
    for(i = 0; i<=delta; i++)
    {
        if( i==delta )  /* 发送最后一帧数据 */
            SPI2_WriteData(&data[i*0xFFFF], size%0xFFFF);
        
        else    /* 超长数据一次发送0xFFFF字节数据 */
            SPI2_WriteData(&data[i*0xFFFF], 0xFFFF);
    }
}

//static u8 Data_Trans(u8 data)
//{
//	data=(data<<4)|(data>>4);
//	data=((data<<2)&0xcc)|((data>>2)&0x33);
//	data=((data<<1)&0xaa)|((data>>1)&0x55);
//	
//	return data;
//}


/**
 * @brief	写命令到LCD
 *
 * @param   cmd		需要发送的命令
 *
 * @return  void
 */
static void LCD_Write_Cmd(u8 cmd)
{
    LCD_DC_Clr();
	
//    cmd = Data_Trans(cmd);
    LCD_SPI_Send(&cmd, 1);
}

/**
 * @brief	写数据到LCD
 *
 * @param   cmd		需要发送的数据
 *
 * @return  void
 */
static void LCD_Write_Data(u8 data)
{
    LCD_DC_Set();

//	data = Data_Trans(data);
    LCD_SPI_Send(&data, 1);
}

/**
 * @brief	写半个字的数据到LCD
 *
 * @param   cmd		需要发送的数据
 *
 * @return  void
 */
void LCD_Write_HalfWord(const u16 da)
{
    u8 data[2] = {0};

    data[0] = da >> 8;
    data[1] = da;

    LCD_DC_Set();
    LCD_SPI_Send(data, 2);
}


/**
 * 设置数据写入LCD缓存区域
 *
 * @param   x1,y1	起点坐标
 * @param   x2,y2	终点坐标
 *
 * @return  void
 */
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_Write_Cmd(0x2a);
    LCD_Write_Data(x1 >> 8);
    LCD_Write_Data(x1);
    LCD_Write_Data(x2 >> 8);
    LCD_Write_Data(x2);

    LCD_Write_Cmd(0x2b);
    LCD_Write_Data(y1 >> 8);
    LCD_Write_Data(y1);
    LCD_Write_Data(y2 >> 8);
    LCD_Write_Data(y2);

    LCD_Write_Cmd(0x2C);
}

/**
 * 打开LCD显示
 *
 * @param   void
 *
 * @return  void
 */
void LCD_DisplayOn(void)
{
    LCD_BLK_Set();
}
/**
 * 关闭LCD显示
 *
 * @param   void
 *
 * @return  void
 */
void LCD_DisplayOff(void)
{
    LCD_BLK_Clr();
}

/**
 * 以一种颜色清空LCD屏
 *
 * @param   color	清屏颜色
 *
 * @return  void
 */
void LCD_Clear(u16 color)
{
    u16 i, j;
    u8 data[2] = {0};

    data[0] = color >> 8;
    data[1] = color;

    LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

    for(j = 0; j < LCD_Buf_Size / 2; j++)
    {
        lcd_buf[j * 2] =  data[0];
        lcd_buf[j * 2 + 1] =  data[1];
    }

    LCD_DC_Set();

    for(i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_Buf_Size); i++)
    {
        HAL_SPI_Transmit_DMA(&hspi2,lcd_buf,LCD_Buf_Size);
		while(HAL_DMA_GetState(&hdma_spi2_tx) == HAL_DMA_STATE_BUSY);
    }
}

/**
 * 用一个颜色填充整个区域
 *
 * @param   x_start,y_start     起点坐标
 * @param   x_end,y_end			终点坐标
 * @param   color       		填充颜色
 *
 * @return  void
 */
void LCD_Fill(u16 x_start, u16 y_start, u16 x_end, u16 y_end, u16 color)
{
    u16 i, j;
    u8 data[2] = {0};

    data[0] = color >> 8;
    data[1] = color;
	
    for(j = 0; j < (x_end-x_start)*(y_end-y_start); j++)
    {
        lcd_buf[j * 2] =  data[0];
        lcd_buf[j * 2 + 1] =  data[1];
    }

	LCD_Address_Set(x_start, y_start-1, x_end, y_end-1);
    LCD_DC_Set();

    for(i = 0; i < (y_end-y_start); i++)
    {
        HAL_SPI_Transmit_DMA(&hspi2,lcd_buf,(x_end-x_start)*2);
		while(HAL_DMA_GetState(&hdma_spi2_tx) == HAL_DMA_STATE_BUSY);
    }

}

void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{
	u32 num;
	u16 i,j;
	u16 height,width;
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
	num = width*height*2;
	
	LCD_Address_Set(sx, sy, ex, ey);
	LCD_DC_Set();
	
	uint8_t *p = (uint8_t*)color;//交换color数据区的位置，方便一次就传送完成
	uint8_t temp;
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			temp = *(p+(width*i+j)*2);
			*(p+(width*i+j)*2) = *(p+(width*i+j)*2+1);
			*(p+(width*i+j)*2+1) = temp;
		}
			
	}
	HAL_SPI_Transmit(&hspi2,(uint8_t*)color,num,0xFFFF);	

}
/**
 * @brief	LCD初始化
 *
 * @param   void
 *
 * @return  void
 */
 void LCD_Init(void)
{
    LCD_Gpio_Init();	//硬件接口初始化

    HAL_Delay(120);
    /* Sleep Out */
    LCD_Write_Cmd(0x11);
    /* wait for power stability */
    HAL_Delay(120);

    /* Memory Data Access Control */
    LCD_Write_Cmd(0x36);
    LCD_Write_Data(0x00);

    /* RGB 5-6-5-bit  */
    LCD_Write_Cmd(0x3A);
    LCD_Write_Data(0x65);

    /* Porch Setting */
    LCD_Write_Cmd(0xB2);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x00);
    LCD_Write_Data(0x33);
    LCD_Write_Data(0x33);

    /*  Gate Control */
    LCD_Write_Cmd(0xB7);
    LCD_Write_Data(0x72);

    /* VCOM Setting */
    LCD_Write_Cmd(0xBB);
    LCD_Write_Data(0x3D);   //Vcom=1.625V

    /* LCM Control */
    LCD_Write_Cmd(0xC0);
    LCD_Write_Data(0x2C);

    /* VDV and VRH Command Enable */
    LCD_Write_Cmd(0xC2);
    LCD_Write_Data(0x01);

    /* VRH Set */
    LCD_Write_Cmd(0xC3);
    LCD_Write_Data(0x19);

    /* VDV Set */
    LCD_Write_Cmd(0xC4);
    LCD_Write_Data(0x20);

    /* Frame Rate Control in Normal Mode */
    LCD_Write_Cmd(0xC6);
    LCD_Write_Data(0x0F);	//60MHZ

    /* Power Control 1 */
    LCD_Write_Cmd(0xD0);
    LCD_Write_Data(0xA4);
    LCD_Write_Data(0xA1);

    /* Positive Voltage Gamma Control */
    LCD_Write_Cmd(0xE0);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x11);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x2B);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x54);
    LCD_Write_Data(0x4C);
    LCD_Write_Data(0x18);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x0B);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x23);

    /* Negative Voltage Gamma Control */
    LCD_Write_Cmd(0xE1);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x11);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x2C);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x44);
    LCD_Write_Data(0x51);
    LCD_Write_Data(0x2F);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x20);
    LCD_Write_Data(0x23);

    /* Display Inversion On */
    LCD_Write_Cmd(0x21);

    LCD_Write_Cmd(0x29);

    LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

    LCD_Clear(WHITE);

    /*打开显示*/
    LCD_BLK_Set();
}


