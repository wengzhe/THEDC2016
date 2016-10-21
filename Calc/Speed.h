#ifndef _SPEED_H
#define _SPEED_H

#include "stm32f10x.h"

typedef enum
{
	None = 0,
	Run = 1,
	Run1 = 2, //at 5% voltage
	Tone = 3,
	Run2 = 4 // at -5% voltage
}CL_SPEED_Calibration_t;

void CL_SPEED_Init(void);
uint8_t CL_SPEED_SetCalibration_Type(CL_SPEED_Calibration_t C);//Get Data of Motor Speed
uint8_t CL_SPEED_CheckCalibration(void);//Check whether the motor speed is OK
void CL_SPEED_ClearCalibration(void);
void CL_SPEED_SetWarningFunction(void (*f)(void));
void CL_SPEED_SetSpeed(int8_t L, int8_t R);//Set Motor Target Speed

void CL_SPEED_Tick_Pre(void);
void CL_SPEED_Tick_After(void);

#endif
