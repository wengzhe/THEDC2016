#ifndef _SYSCONFIG_H
#define _SYSCONFIG_H

#include "stm32f10x_conf.h"
#include "stm32f10x.h"

void System_Init(void);

extern volatile uint32_t TimeTicket;
void TimeTicketSevice(void);
void Delay(uint32_t nTime);

#endif

