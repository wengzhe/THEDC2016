#ifndef _CALC_H
#define _CALC_H

#include "stm32f10x.h"

void CL_Init(void);

void CL_Tick_Pre(uint32_t time);
void CL_Tick_After(void);

uint8_t CL_Rand(void);

#endif
