#ifndef _EXEC_H
#define _EXEC_H

#include "stm32f10x.h"

void EL_Init(void);
void EL_Tick(uint32_t time);
uint8_t EL_Rand(void);


#endif
