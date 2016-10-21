#ifndef _PWM_H
#define _PWM_H

#include "stm32f10x.h"

void DL_PWM_Init(void);
void DL_PWM_SetPulse(int16_t LSpeed, int16_t RSpeed);//-199~200
void DL_PWM_SetFreq(float freq);//360k max
void DL_PWM_SetPrescaler(uint16_t div);
void DL_PWM_NeedTone(uint8_t);
int16_t DL_PWM_Speed(void);

#endif
