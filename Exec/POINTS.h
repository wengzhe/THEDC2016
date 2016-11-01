#ifndef _POINTS_H
#define _POINTS_H

#include "stm32f10x.h"
#include "MyTypes.h"

typedef enum
{
	POINTS_FIRST_RUN = 1, //run for 1s to get angle
	POINTS_RUNNING = 2,
	POINTS_STOP = 0
}EL_POINTS_Type_t;

typedef enum
{
	POINTS_Single = 1,
	POINTS_Queue = 2,
	POINTS_Stop = 0
}EL_POINTS_Way_t;

typedef enum
{
	POINTS_White = 1,
	POINTS_Black = 2,
	POINTS_None = 0
}EL_POINTS_Color_t;

typedef struct
{
	Point_t Target;
	uint8_t MinDis;
	uint8_t StopTime; //if t=0, will not stop, 10ms
}EL_POINTS_Queue_t;

void EL_POINTS_Init(void);

uint8_t EL_POINTS_InsertQueue(EL_POINTS_Queue_t input);
void EL_POINTS_ClearQueue(void);

//void EL_POINTS_ClearShadowStack(void);
uint8_t EL_POINTS_InsertShadowStack(EL_POINTS_Queue_t input);
//EL_POINTS_Queue_t *EL_POINTS_ShadowStackTop(void);
void EL_POINTS_FinishShadowStack(void);

void EL_POINTS_SetBorderSafetyDis(uint8_t dis);
void EL_POINTS_SetColor(EL_POINTS_Color_t color);
void EL_POINTS_SetMinDistance(uint8_t dis);

void EL_POINTS_DirectTarget(Point_t tar);
void EL_POINTS_StopTarget(void);

void EL_POINTS_Tick(void);
void EL_POINTS_SetFlightPos(const Point_t tar);
#endif
