//#include "AllDefs.h"

#include "Driver.h"
#include "PWM.h"
#include "ADC.h"
#include "UART.h"
#include "IMU.h"
#include "IR.h"

void DL_Init(void)
{
	DL_PWM_Init();
	DL_ADC_Init();
	DL_UART_Init();
	DL_IMU_Init();
	DL_IR_Init();
}

void DL_Tick_Pre(uint32_t time)//things to be done before higher levels
{
	DL_ADC_Tick();
	DL_IMU_Tick(time);
}

void DL_Tick_After(void)//after the higher levels then do
{
}

uint8_t tmp[6] = {0};

uint8_t Hash(uint8_t *str)
{
    uint32_t seed = 31;
    uint32_t hash = 0;
    while (*str)
    {
        hash = hash * seed + *(str++);
    }
    return (uint8_t)hash;
}

uint8_t *GetNum(uint16_t num)
{
	uint8_t i;
	for (i=0;i<5;i++)
	{
		tmp[4-i]=num%10+'0';
		num/=10;
	}
	return tmp;
}

uint8_t DL_Rand(void)
{
	return Hash(GetNum(DL_UART_GetRandomSeed()));
}
