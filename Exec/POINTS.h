#ifndef _POINTS_H
#define _POINTS_H

#include "stm32f10x.h"
#include "MyTypes.h"

typedef enum
{
	POINTS_FIRST_RUN, //run for 1s to get angle
	POINTS_RUNNING,
	POINTS_STOP
}EL_POINTS_Type_t;

void EL_POINTS_Init(void);
void EL_POINTS_DirectTarget(Point_t tar);
void EL_POINTS_SetDirectMinDistance(uint8_t dis);
void EL_POINTS_SetDirectMinTime(uint16_t tim);
void EL_POINTS_StopDirectTarget(void);
void EL_POINTS_Tick(void);

#endif
