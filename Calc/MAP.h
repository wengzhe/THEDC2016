#ifndef _MAP_H
#define _MAP_H

#include "stm32f10x.h"
#include "MyTypes.h"

#define IR_NUM 5
#define IR_DIS 9.

typedef enum
{
	MAP_White = 0,
	MAP_Black = 1,
	MAP_NotSure = 2
}CL_MAP_Color_t;

typedef struct
{
	float Angle[5];
	CL_MAP_Color_t Color[5];
}CL_MAP_IR_t;

void CL_MAP_Init(void);
uint8_t (*CL_MAP_GetMap(void))[64];
uint8_t CL_MAP_CheckColor(uint8_t x, uint8_t y, CL_MAP_Color_t Color);
CL_MAP_Color_t CL_MAP_GetColor(uint8_t x, uint8_t y);
const CL_MAP_IR_t *CL_MAP_GetIR(void);
void CL_MAP_Tick(void);

#endif
