//#include "AllDefs.h"

#include "TONE.h"
#include "PWM.h"
#include <math.h>

#define STOP_FREQ 20000

void CL_TONE_Enable(void)
{
	DL_PWM_NeedTone(1);
}

void CL_TONE_Disable(void)
{
	DL_PWM_NeedTone(0);
	DL_PWM_SetFreq(STOP_FREQ);
}

float FKey=220;

void CL_TONE_SetFundamentalKey(int8_t key)//-1~10
{
	FKey=220*pow(2,key/12.0);
}

const int8_t FKeys[7][2]={{0,-1},{2,1},{3,3},{5,4},{7,6},{8,8},{10,9}};

void CL_TONE_SetFKeyByKey(const uint8_t key[2])//0~6
{
	FKey=220*pow(2,FKeys[key[0]][key[1]]/12.0);
}

const int8_t Tones[7][2]={{0,0},{2,1},{4,3},{5,5},{7,6},{9,8},{11,10}};

#define BEAT_FREQ 10
void CL_TONE_SetTone(uint8_t Tone, uint8_t High, uint8_t Low)//1~7,1~3
{
	if (Tone)
		DL_PWM_SetFreq(FKey*pow(2,(Tones[Tone-1][Low]/12.0)+High-2));
	else
		DL_PWM_SetFreq(STOP_FREQ);
}

__STATIC_INLINE float CL_TONE_CalcFreq(float f1, float f2, float percent)
{
	return f1*pow(f2/f1,percent);
}

void CL_TONE_DoubleTone(uint8_t Tone1, uint8_t High1, uint8_t Low1, uint8_t Tone2, uint8_t High2, uint8_t Low2, float percent)//0~1
{
	float F1=FKey*pow(2,(Tones[Tone1-1][Low1]/12.0)+High1-2);
	float F2=FKey*pow(2,(Tones[Tone2-1][Low2]/12.0)+High2-2);
	DL_PWM_SetFreq(CL_TONE_CalcFreq(F1,F2,percent));
}

void CL_TONE_SetFreq(float freq)
{
	DL_PWM_SetFreq(freq);
}
