#ifndef _TONE_H
#define _TONE_H

#include "stm32f10x.h"

void CL_TONE_Enable(void);
void CL_TONE_Disable(void);
void CL_TONE_SetFKeyByKey(const uint8_t key[2]);//0~6
void CL_TONE_SetTone(uint8_t Tone, uint8_t High, uint8_t Low);//1~7,1~3
void CL_TONE_DoubleTone(uint8_t Tone1, uint8_t High1, uint8_t Low1, uint8_t Tone2, uint8_t High2, uint8_t Low2, float percent);//0~1
void CL_TONE_SetFreq(float freq);

#endif
