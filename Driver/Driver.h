#ifndef _DRIVER_H
#define _DRIVER_H

#include "stm32f10x.h"

void DL_Init(void);

void DL_Tick_Pre(uint32_t time);
void DL_Tick_After(void);

uint8_t DL_Rand(void);

#endif
