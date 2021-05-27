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
 *	������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 *	ALIENTEK MINI STM32������
 *	1.3�� 240*240 TFT_LCD��������
 *	����ԭ��@ALIENTEK
 *	������̳:www.openedv.com
 *	��������:2018/10/27
 *	�汾��V1.0
 *	��Ȩ���У�����ؾ���
 *	Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 *	All rights reserved
 *	******************************************************************************
 *	��ʼ�汾
 *	******************************************************************************/

//LCD�����С���ã��޸Ĵ�ֵʱ��ע�⣡�������޸�������ֵʱ���ܻ�Ӱ�����º���	LCD_Clear/LCD_Fill/LCD_DrawLine
#define LCD_TOTAL_BUF_SIZE	(240*240*2)
#define LCD_Buf_Size 1152
static u8 lcd_buf[LCD_Buf_Size];

u16	POINT_COLOR = BLACK;	//������ɫ	Ĭ��Ϊ��ɫ
u16	BACK_COLOR 	= WHITE;	//������ɫ	Ĭ��Ϊ��ɫ

/**
 * @brief	LCD���ƽӿڳ�ʼ��
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
 * @brief	LCD�ײ�SPI�������ݺ���
 *
 * @param   data	���ݵ���ʼ��ַ
 * @param   size	�������ݴ�С
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
        if( i==delta )  /* �������һ֡���� */
            SPI2_WriteData(&data[i*0xFFFF], size%0xFFFF);
        
        else    /* ��������һ�η���0xFFFF�ֽ����� */
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
 * @brief	д���LCD
 *
 * @param   cmd		��Ҫ���͵�����
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
 * @brief	д���ݵ�LCD
 *
 * @param   cmd		��Ҫ���͵�����
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
 * @brief	д����ֵ����ݵ�LCD
 *
 * @param   cmd		��Ҫ���͵�����
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
 * ��������д��LCD��������
 *
 * @param   x1,y1	�������
 * @param   x2,y2	�յ�����
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
 * ��LCD��ʾ
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
 * �ر�LCD��ʾ
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
 * ��һ����ɫ���LCD��
 *
 * @param   color	������ɫ
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
 * ��һ����ɫ�����������
 *
 * @param   x_start,y_start     �������
 * @param   x_end,y_end			�յ�����
 * @param   color       		�����ɫ
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
	width=ex-sx+1; 			//�õ����Ŀ��
	height=ey-sy+1;			//�߶�
	num = width*height*2;
	
	LCD_Address_Set(sx, sy, ex, ey);
	LCD_DC_Set();
	
	uint8_t *p = (uint8_t*)color;//����color��������λ�ã�����һ�ξʹ������
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
 * @brief	LCD��ʼ��
 *
 * @param   void
 *
 * @return  void
 */
 void LCD_Init(void)
{
    LCD_Gpio_Init();	//Ӳ���ӿڳ�ʼ��

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

    /*����ʾ*/
    LCD_BLK_Set();
}


