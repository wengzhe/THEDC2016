#ifndef _UART_H
#define _UART_H

#include "stm32f10x.h"
#include "MyTypes.h"

typedef struct
{
	//Byte0:LSB
	uint8_t GameStatus:2;
	uint8_t ID:2;
	uint8_t :4;
	//Byte0:MSB
	
	//Byte1:
	uint8_t Flags;
	
	//Byte2~9
	Point_t MyPos;
	Point_t EmyPos;
	Point_t TarPos;
	Point_t AirPos;
	
	//Byte10~11
	uint8_t MyHP;
	uint8_t EmyHP;

	//Byte12~13
	Point_t ItemPos;
	
	//Byte14
	uint8_t GameTimeL;
	
	//Byte15
	uint8_t GameTimeH:6;
	uint8_t ItemType:2;
}DL_UART_Data_t;

void DL_UART_Init(void);
uint16_t DL_UART_GetRandomSeed(void);
void DL_UART_SetTxData(uint8_t *data, uint8_t length);
uint8_t DL_UART_SetRxFunc(void (*f)(const DL_UART_Data_t*));

#endif
