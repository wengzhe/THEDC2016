#ifndef _MUSIC_H
#define _MUSIC_H

#include "stm32f10x.h"
#include "Exec.h"
#include <math.h>

typedef enum
{
	Start,
	Alarm,
	Music,
	Info,
	Test,
	Stop
}EL_MUSIC_Type_t;

typedef enum
{
	Once,
	Single,
	Random,
	Order,
}EL_MUSIC_Play_t;

void EL_MUSIC_SetPause(uint8_t p);//1:pause 0:resume
void EL_MUSIC_Tick(uint32_t time);
void EL_MUSIC_ChangeStatus(EL_MUSIC_Type_t Type, uint8_t MusicID);//MusicID=0 won't change music 
void EL_MUSIC_ChangeMode(EL_MUSIC_Play_t Play);
void EL_MUSIC_Init(void);

#endif
