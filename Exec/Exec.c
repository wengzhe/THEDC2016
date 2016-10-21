#include "Exec.h"
#include "Calc.h"
#include "MUSIC.h"
#include "POINTS.h"

void EL_Init(void)
{
	CL_Init();
	EL_MUSIC_Init();
	EL_POINTS_Init();
}

void EL_Tick(uint32_t time)
{
	CL_Tick_Pre(time);
	EL_MUSIC_Tick(time);
	EL_POINTS_Tick();
	CL_Tick_After();
}

uint8_t EL_Rand(void)
{
	return CL_Rand();
}
