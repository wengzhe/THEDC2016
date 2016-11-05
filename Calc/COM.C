//#include "AllDefs.h"

#include "COM.h"
#include "UART.h"

CL_COM_Data_t CL_COM_Data={GAME_WAIT,ITEM_NULL,0};
uint8_t SendData[4]={0,0,0x0D,0x0A};

#define RXFUNCSIZE 5
void (*RxFunc[RXFUNCSIZE])(const CL_COM_Data_t*) = {0};
uint8_t RxFP = 0;

void CL_COM_CalData(const DL_UART_Data_t*);

void CL_COM_Init(void)
{
	DL_UART_SetRxFunc(CL_COM_CalData);
}

uint8_t CL_COM_SetRxFunc(void (*f)(const CL_COM_Data_t*))
{
	if (RxFP < RXFUNCSIZE)
	{
		RxFunc[RxFP++]=f;
		return 0;
	}
	else
		return 1;
}

const CL_COM_Data_t* CL_COM_GetData(void)
{
	return &CL_COM_Data;
}

void CL_COM_CalData(const DL_UART_Data_t* DataIN)
{
	uint8_t i;
	CL_COM_Data.GameStatus = (GameStatus_t)DataIN->GameStatus;
	CL_COM_Data.Flags.data = DataIN->Flags;
	CL_COM_Data.ItemType = (ItemType_t)DataIN->ItemType;
	CL_COM_Data.ID = DataIN->ID;
	CL_COM_Data.MyPos = DataIN->MyPos;
	CL_COM_Data.EmyPos = DataIN->EmyPos;
	CL_COM_Data.TarPos = DataIN->TarPos;
	CL_COM_Data.AirPos = DataIN->AirPos;
	CL_COM_Data.ItemPos = DataIN->ItemPos;
	CL_COM_Data.MyHP = DataIN->MyHP;
	CL_COM_Data.EmyHP = DataIN->EmyHP;
	CL_COM_Data.GameTime = DataIN->GameTimeL + ((uint16_t)DataIN->GameTimeH << 8);
	//CL_COM_SendPos(CL_COM_Data.MyPos);
	for (i=0; i<RxFP; i++)
	{
		RxFunc[i](&CL_COM_Data);
	}
}

void CL_COM_SendPos(const Point_t point)
{
	SendData[0]=point.x;
	SendData[1]=point.y;
	DL_UART_SetTxData(SendData,4);
}
