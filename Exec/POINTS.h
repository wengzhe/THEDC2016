#ifndef _POINTS_H
#define _POINTS_H

#include "stm32f10x.h"
#include "MyTypes.h"

typedef enum
{
	POINTS_FIRST_RUN, //run for 1s to get angle
	POINTS_RUNNING,
	POINTS_STOP = 0
}EL_POINTS_Type_t;

typedef enum
{
	POINTS_Single,
	POINTS_Queue,
	POINTS_Stop = 0
}EL_POINTS_Way_t;

typedef enum
{
	POINTS_White,
	POINTS_Black,
	POINTS_None = 0
}EL_POINTS_Color_t;

void EL_POINTS_Init(void);
void EL_POINTS_ColorTarget(Point_t tar, uint8_t isBlack);
void EL_POINTS_DirectTarget(Point_t tar);
void EL_POINTS_SetMinDistance(uint8_t dis);
void EL_POINTS_StopTarget(void);
void EL_POINTS_Tick(void);

#endif
