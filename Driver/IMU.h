#ifndef _IMU_H
#define _IMU_H

#include "stm32f10x.h"

typedef enum
{
	IMU_OFF,
	IMU_INIT,
	IMU_TEST,
	IMU_WORK
}DL_IMU_State_t;

void DL_IMU_Init(void);
void DL_IMU_StartCheck(uint8_t ID);
float DL_IMU_GetGyro(void);
void DL_IMU_Tick(uint32_t TimeTicket);

#endif
