#include "mpuiic.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������V3
//MPU6050 IIC���� ����
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/17
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

//MPU IIC ��ʱ����
void MPU_IIC_Delay(void)
{
    u8 i = 0;
    for(i = 0; i < 10; i++);
}

//��ʼ��IIC
void MPU_IIC_Init(void)
{

}
//����IIC��ʼ�ź�
void MPU_IIC_Start(void)
{
    MPU_SDA_OUT();     //sda�����
    MPU_IIC_SDA_SET();
    MPU_IIC_SCL_SET();
    MPU_IIC_Delay();
    MPU_IIC_SDA_RESET();//START:when CLK is high,DATA change form high to low
    MPU_IIC_Delay();
    MPU_IIC_SCL_RESET();//ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void MPU_IIC_Stop(void)
{
    MPU_SDA_OUT();//sda�����
    MPU_IIC_SCL_RESET();
    MPU_IIC_SDA_RESET();//STOP:when CLK is high DATA change form low to high
    MPU_IIC_Delay();
    MPU_IIC_SCL_SET();
    MPU_IIC_SDA_SET();//����I2C���߽����ź�
    MPU_IIC_Delay();
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 MPU_IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    MPU_SDA_IN();      //SDA����Ϊ����
    MPU_IIC_SDA_SET();
    MPU_IIC_Delay();
    MPU_IIC_SCL_SET();
    MPU_IIC_Delay();
    while(MPU_READ_SDA())
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            MPU_IIC_Stop();
            return 1;
        }
    }
    MPU_IIC_SCL_RESET();//ʱ�����0
    return 0;
}
//����ACKӦ��
void MPU_IIC_Ack(void)
{
    MPU_IIC_SCL_RESET();
    MPU_SDA_OUT();
    MPU_IIC_SDA_RESET();
    MPU_IIC_Delay();
    MPU_IIC_SCL_SET();
    MPU_IIC_Delay();
    MPU_IIC_SCL_RESET();
}
//������ACKӦ��
void MPU_IIC_NAck(void)
{
    MPU_IIC_SCL_RESET();
    MPU_SDA_OUT();
    MPU_IIC_SDA_SET();
    MPU_IIC_Delay();
    MPU_IIC_SCL_SET();
    MPU_IIC_Delay();
    MPU_IIC_SCL_RESET();
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void MPU_IIC_Send_Byte(u8 txd)
{
    u8 t;
    MPU_SDA_OUT();
    MPU_IIC_SCL_RESET();//����ʱ�ӿ�ʼ���ݴ���
    for(t = 0; t < 8; t++)
    {
        HAL_GPIO_WritePin(GPIOB, IIC_SDA_Pin, (GPIO_PinState)((txd & 0x80) >> 7));
        txd <<= 1;
        MPU_IIC_SCL_SET();
        MPU_IIC_Delay();
        MPU_IIC_SCL_RESET();
        MPU_IIC_Delay();
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 MPU_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    MPU_SDA_IN();//SDA����Ϊ����
    for(i = 0; i < 8; i++)
    {
        MPU_IIC_SCL_RESET();
        MPU_IIC_Delay();
        MPU_IIC_SCL_SET();
        receive <<= 1;
        if(MPU_READ_SDA())
			receive++;
        MPU_IIC_Delay();
    }
    if(!ack)
        MPU_IIC_NAck();//����nACK
    else
        MPU_IIC_Ack(); //����ACK
    return receive;
}

























