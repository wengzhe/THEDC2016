#include "sysconfig.h"

void SysTickInit(void)
{
	SysTick_Config(72000);
}

volatile uint32_t TimeTicket=0;

void TimeTicketSevice(void)
{
	TimeTicket++;
}

void System_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SysTickInit();
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
}

void Delay(uint32_t nTime)
{ 
  uint32_t TimeTarget = nTime+TimeTicket;

  while(TimeTarget != TimeTicket);
}
