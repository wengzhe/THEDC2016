#include "adc.h"

volatile uint16_t ADCValue[3]={0};

//PC4,PC5 -> in14,in15

void ADC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	ADC_InitTypeDef ADC_InitStructure;

	//clock initial
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	//ADC clock initial
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//12MHz
	
	//GPIO Initial
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	//ADC mode initial
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 4;
	ADC_Init(ADC1,&ADC_InitStructure);
	
  //regular channel config
	ADC_RegularChannelConfig(ADC1,ADC_Channel_14, 1, ADC_SampleTime_239Cycles5);//PC4
	ADC_RegularChannelConfig(ADC1,ADC_Channel_15, 2, ADC_SampleTime_239Cycles5);//PC5
	ADC_RegularChannelConfig(ADC1,ADC_Channel_17, 3, ADC_SampleTime_239Cycles5);//VRef
  
	//enable temperature
	ADC_TempSensorVrefintCmd(ENABLE);
	
	//enable dma
	ADC_DMACmd(ADC1,ENABLE);

  //enable ADC
	ADC_Cmd(ADC1,ENABLE);

	//ADC Calibration
	ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);

}


void ADC1_DMAInit(void)
{
	DMA_InitTypeDef DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 4;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC, ENABLE);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
  //initialize interrupt
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

void DL_ADC_Init(void)
{
	ADC1_Init();
	ADC1_DMAInit();
}

//-------------------------------------------------------------------------------------

uint32_t ADC_Value_SMD[3]={0}; 

#define SMOOTHING_LEVEL		10
uint8_t ADC_Smoothing(void)
{
	static uint16_t count;
	uint8_t channel;
	if(count==0)
	{
		for(channel=0;channel<3;channel++)
			ADC_Value_SMD[channel]=0;
	}
	
	for(channel=0;channel<3;channel++)
		ADC_Value_SMD[channel]+=ADCValue[channel];
	if(++count<SMOOTHING_LEVEL)
		return 1;
	else
	{
		count=0;
		for(channel=0;channel<3;channel++)
			ADC_Value_SMD[channel]/=SMOOTHING_LEVEL;
		return 0;
	}
}


#define ADC_AIN14				0
#define ADC_AIN15				1
#define ADC_VOLREF		  2

volatile uint16_t DL_ADC_Voltage[3];
#define REF_ADC				ADC_Value_SMD[ADC_VOLREF]
#define REF_VOL				1200

void ADC_Calculate_Voltage(void)
{
	uint8_t channel;
	for(channel=0;channel<2;channel++)
		DL_ADC_Voltage[channel]=((uint32_t)ADC_Value_SMD[channel]*REF_VOL)/REF_ADC;
	DL_ADC_Voltage[ADC_VOLREF]=(4096*REF_VOL)/REF_ADC;
}

void DMA1_Channel1_IRQHandler(void)//60us*10
{
	DMA_ClearFlag(DMA1_FLAG_TC1);
	DMA_ClearITPendingBit(DMA1_IT_TC1);
	
	if(ADC_Smoothing()==0)
		ADC_Calculate_Voltage();
	else
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

//-------------------------------------------------------------------------------------

void DL_ADC_Tick(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE); //About 0.6ms, make sure it will be done before next tick.
}
