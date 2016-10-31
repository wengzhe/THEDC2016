#ifndef _COM_H
#define _COM_H

#include "stm32f10x.h"
#include "MyTypes.h"

typedef union
{
	struct{
		uint8_t TargetColor:1;
		uint8_t TargetExist:1;		//-40/Target
		uint8_t OutOfBounds:1;
		uint8_t DamagedByPlane:1;	//-8/s
		uint8_t HealedByPlane:1;	//+4/s
		uint8_t ControlPlane:1;
		uint8_t HugeHurt:1;				//-8/s
		uint8_t LittleHurt:1;			//-2/s
	}BITS;
	uint8_t data;
}CL_COM_Flags_t;

typedef struct
{
	GameStatus_t GameStatus;
	ItemType_t ItemType;
	CL_COM_Flags_t Flags;
	uint8_t ID;
	
	uint8_t MyHP;
	uint8_t EmyHP;
	uint16_t GameTime;
	
	Point_t MyPos;
	Point_t EmyPos;
	Point_t TarPos;
	Point_t AirPos;
	Point_t ItemPos;
}CL_COM_Data_t;

void CL_COM_Init(void);
uint8_t CL_COM_SetRxFunc(void (*f)(const CL_COM_Data_t*));
const CL_COM_Data_t* CL_COM_GetData(void);
void CL_COM_SendPos(const Point_t point);

#endif
