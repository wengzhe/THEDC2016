#ifndef _INF_H
#define _INF_H

#include "stm32f10x.h"
#include "MyTypes.h"

typedef struct
{
	Point_t Pos;
	uint8_t HP;
	uint8_t OutOfBound;
}EL_INF_PlayerInf_t;

typedef struct
{
	Point_t Pos;
	ItemType_t Type;
}EL_INF_ItemInf_t;

typedef struct
{
	GameStatus_t Status;
	uint8_t ID;
	uint16_t Time;
	uint8_t (*Map)[64];
}EL_INF_GameInf_t;

typedef struct
{
	Point_t Pos;
	uint8_t Control;
	uint8_t BeAffected;
}EL_INF_AirPlaneInf_t;

typedef struct
{
	Point_t Pos;
	uint8_t Black;
	uint8_t Exist;					//-40/Target
	uint8_t BeDamaged;
}EL_INF_TargetInf_t;

typedef struct
{
	uint8_t DamagedByPlane;	//-8/s
	uint8_t HealedByPlane;	//+4/s
	uint8_t HugeHurt;				//-8/s
	uint8_t LittleHurt;			//-2/s
}EL_INF_AdditionInf_t;

void EL_INF_Init(void);

EL_INF_PlayerInf_t *EL_INF_GetMyInf(void);
EL_INF_PlayerInf_t *EL_INF_GetEmyInf(void);
EL_INF_ItemInf_t *EL_INF_GetItemInf(void);
EL_INF_GameInf_t *EL_INF_GetGameInf(void);
EL_INF_AirPlaneInf_t *EL_INF_GetAirPlaneInf(void);
EL_INF_TargetInf_t *EL_INF_GetTargetInf(void);
EL_INF_AdditionInf_t *EL_INF_GetAdditionInf(void);

//13ms(@(127,127),r=179)
Point_t CheckNearestColor(Point_t curPos, uint8_t black, uint8_t radius, int16_t minDisFromColorBorder);

#endif
