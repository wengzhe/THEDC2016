#ifndef _MYTYPES_H
#define _MYTYPES_H

#include "stm32f10x.h"

typedef struct
{
	uint8_t x;
	uint8_t y;
}Point_t;

typedef enum
{
	GAME_WAIT = 0,
	GAME_START = 1,
	GAME_PAUSE = 2,
	GAME_STOP = 3
}GameStatus_t;

//enum PropType { PropET = 0, PropHP = 1, PropBW = 2, PropAC = 3 }; //(EmpTy),(Black-White),(+HP),(Air Command)
typedef enum
{
	ITEM_NULL = 0,
	ITEM_LIFE = 1,
	ITEM_CHANGE = 2,
	ITEM_PLANE = 3
}ItemType_t;

#endif
