#include "Driver.h"
#include "Calc.h"
#include "Speed.h"
#include "COM.h"
#include "Angle.h"

void CL_Init(void)
{
	DL_Init();
	CL_COM_Init();
	CL_ANGLE_Init();
	CL_SPEED_Init();
}

void CL_Tick_Pre(uint32_t time)
{
	DL_Tick_Pre(time);
	CL_SPEED_Tick_Pre();
	CL_ANGLE_Tick();
}

void CL_Tick_After(void)
{
	CL_SPEED_Tick_After();
	DL_Tick_After();
}

uint8_t CL_Rand(void)
{
	return DL_Rand();
}
