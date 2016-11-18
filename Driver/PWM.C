//#include "AllDefs.h"
#include "AllDefs.h"
#include "pwm.h"


//PC6:L_PWM PC7:R_PWM PC8:L_DIR PC9:R_DIR
//TIM8 CH1~2
uint8_t NeedTone = 0;
#ifdef BIG_VOICE
uint8_t minusTone = 0;
#endif
int16_t SpeedSet[2] = {0};
float FreqSet = 20000;
//PA11:Horn,TIM1_CH4

void DL_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
#ifdef BIG_VOICE
	NVIC_InitTypeDef NVIC_InitStructure;
#endif
	
	//clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);

	//gpio intialize
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	
	//MOTOR
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//timer8 initialize
	
	//timer8 base initialize
	TIM_TimeBaseStructure.TIM_Prescaler = 11;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned3;
	TIM_TimeBaseStructure.TIM_Period = 200;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);

	//timer8 channel 1 initial
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(TIM8,&TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);

	//timer8 channel 2 initial
	TIM_OC2Init(TIM8,&TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);

	//timer8 channel 3 initial
	TIM_OC3Init(TIM8,&TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
	
	//timer8 channel 4 initial
	TIM_OC4Init(TIM8,&TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);

#ifdef BIG_VOICE
	//NVIC
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

	TIM_Cmd(TIM8, ENABLE);
	TIM_ARRPreloadConfig(TIM8, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
	
#ifdef PWM_HORN
	//Horn
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_Pulse = 100;
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM1, ENABLE);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
#endif
}

void DL_PWM_SetPulse(int16_t LSpeed, int16_t RSpeed)
{
#ifdef BIG_VOICE
	if (NeedTone)
	{
		LSpeed = minusTone ? -PWM_VOLUME:PWM_VOLUME;
		RSpeed = minusTone ? -PWM_VOLUME:PWM_VOLUME;
	}
#endif
	if (LSpeed<=0)
	{
		TIM8->CCR3=200;
		TIM8->CCR1=200+LSpeed;
#ifndef BIG_VOICE
		if (NeedTone && TIM8->CCR1>190)
			TIM8->CCR1=190;
#endif
	}
	else
	{
		TIM8->CCR3=0;
		TIM8->CCR1=LSpeed;
#ifndef BIG_VOICE
		if (NeedTone && TIM8->CCR1<10)
			TIM8->CCR1=10;
#endif
	}
	if (RSpeed<0)
	{
		TIM8->CCR4=200;
		TIM8->CCR2=200+RSpeed;
#ifndef BIG_VOICE
		if (NeedTone && TIM8->CCR2>190)
			TIM8->CCR2=190;
#endif
	}
	else
	{
		TIM8->CCR4=0;
		TIM8->CCR2=RSpeed;
#ifndef BIG_VOICE
		if (NeedTone && TIM8->CCR2<10)
			TIM8->CCR2=10;
#endif
	}
	SpeedSet[0]=LSpeed;
	SpeedSet[1]=RSpeed;
}

void DL_PWM_SetFreq(float freq)
{
#ifdef BIG_VOICE
	if (NeedTone)
		TIM8->PSC=(90000/freq)-1;
	else
		TIM8->PSC=(180000/freq)-1;
#else
	TIM8->PSC=(180000/freq)-1;
#endif
#ifdef PWM_HORN
	TIM1->PSC=(180000/freq)-1;
#endif
	FreqSet = freq;
}

void DL_PWM_SetPrescaler(uint16_t div)
{
	TIM8->PSC=div-1;
#ifdef PWM_HORN
	TIM1->PSC=div-1;
#endif
}

void DL_PWM_NeedTone(uint8_t nt)
{
	if (NeedTone != nt)
	{
		NeedTone=nt;
		DL_PWM_SetPulse(SpeedSet[0],SpeedSet[1]);
#ifdef PWM_HORN
		TIM1->CCR4 = nt ? 100 : 0;
#endif
#ifdef BIG_VOICE
		if (nt)
			TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE);
		else
			TIM_ITConfig(TIM8,TIM_IT_Update,DISABLE);
		DL_PWM_SetFreq(FreqSet);
#endif
	}
}

int16_t DL_PWM_Speed(void)
{
	return SpeedSet[0]+SpeedSet[1];
}

uint32_t PWM_TEST = 0;

#ifdef BIG_VOICE
void TIM8_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
		if (NeedTone && TIM_GetCounter(TIM8) > 100)
		{
			TIM8->CCR1 = 200 - TIM8->CCR1;
			TIM8->CCR2 = 200 - TIM8->CCR2;
			TIM8->CCR3 = 200 - TIM8->CCR3;
			TIM8->CCR4 = 200 - TIM8->CCR4;
			minusTone = !minusTone;
			PWM_TEST++;
		}
	}
}
#endif
