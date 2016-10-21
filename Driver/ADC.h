#ifndef _ADC_H
#define _ADC_H

#include "stm32f10x.h"

void DL_ADC_Init(void);
void DL_ADC_Tick(void);

extern volatile uint16_t DL_ADC_Voltage[3];

#endif
