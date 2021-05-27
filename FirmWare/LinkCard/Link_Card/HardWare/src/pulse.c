#include "pulse.h"

uint16_t Get_Adc1_val()               //ADC获取
{
    HAL_ADC_Start(&hadc1);

    HAL_ADC_PollForConversion(&hadc1, 100);

    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
    {
        return HAL_ADC_GetValue(&hadc1);
    }
    return 0;
}

uint16_t Get_Adc1_Average()          //50ms数据融合
{
	uint8_t i,j = 0;
	uint16_t buf[10];
	uint16_t tempval = 0;
	
	for(i=0;i<10;i++)
	{
		buf[i] = Get_Adc1_val();
		HAL_Delay(5);
	}
	
	for(i=0;i<9;i++)                 //冒泡排序
	{
		for(j=0;j<9-i;j++)
		{
			if(buf[j]>buf[j+1])
			{
				tempval = buf[j];
				buf[j] = buf[j+1];
				buf[j+1] = tempval;
			}
		}
	}
	
	tempval = 0;
	for(i=1;i<9;i++)
	{
		tempval += buf[i];
	}
	
	return tempval/8;
}

uint16_t Get_Array_Max(uint16_t *data,uint8_t num)
{
	int max = 0;
	int i = 0;
	
	for(i=0;i<num;i++)
	{
		if(max < data[i])
		{
			max = data[i];
		}
	}
	
	return max;
}

uint16_t pulse_process(uint8_t *BPM)
{
	static uint16_t adc_value,preReadData,readData = 0;
	static uint16_t max,target = 0;
	static uint8_t Pre_State,Now_State = 0;
	static uint16_t time1,time2 = 0;
	static uint16_t data[20] = {0};
	static uint8_t index,stateindex = 0;
	
	preReadData = readData;
	adc_value = Get_Adc1_Average()-1500;
	if(adc_value < 1000)
	{
		readData = adc_value;
	}
	
	data[index++] = readData;
	if(index >= 20)
	{
		index = 0;
		
		max = Get_Array_Max(data,20);
		target = max - 3;
	}
	
	Pre_State = Now_State;
	Now_State = (readData>target)?1:0;
	if((Pre_State == 0)&&(Now_State == 1))
	{
		stateindex++;
		stateindex %= 2;
		
		if(stateindex == 1)
		{
			time1 = HAL_GetTick();
		}
		if(stateindex == 0)
		{
			time2 = HAL_GetTick();
			*BPM = 60000/(time2-time1);
		}
	}
	
	return readData;
}









