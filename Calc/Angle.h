#ifndef _ANGLE_H
#define _ANGLE_H

#include "stm32f10x.h"
#include "MyTypes.h"

typedef enum
{
	ANGLE_REL,//relative
	ANGLE_ABS//absolute
}CL_ANGLE_Type_t;

typedef struct
{
	Point_t Point;
	uint16_t DegreeP;
	int16_t Speed;
}CL_ANGLE_Point_t;

extern float M_PI;

void CL_ANGLE_SetDegree(float degree, CL_ANGLE_Type_t type);
float CL_ANGLE_GetDegree(void);
void CL_ANGLE_Init(void);
void CL_ANGLE_Reset(void);
void CL_ANGLE_Tick(void);

#endif
