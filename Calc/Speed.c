#include "AllDefs.h"

#include "Speed.h"
#include "ADC.h"
#include "PWM.h"
#include "UART.h"
#include <math.h>

#define SPEED_SMOOTH_TIME 10
uint16_t Speed_Voltage_Tmp[2]={0};
uint16_t Speed_Voltage[2]={0};

#define SPEED_CALIBRATION_TIME 100
CL_SPEED_Calibration_t CType = None;
int32_t Center_Voltage[4][2]={0};
uint8_t Calibration_Cal=0;
uint8_t CL_SPEED_CheckCalibration_Result = 0;
#ifdef GAME_STATUS_START
uint8_t CL_SPEED_GameStatus = 1;
#else
uint8_t CL_SPEED_GameStatus = 0;
#endif
float CL_SPEED_SpeedNow[2]={0,0}; //-100~100

void CL_SPEED_GetGameStatus(const DL_UART_Data_t *uart)
{
	CL_SPEED_GameStatus = uart->GameStatus;
}

void CL_SPEED_Init(void)
{
	DL_UART_SetRxFunc(CL_SPEED_GetGameStatus);
}

uint8_t CL_SPEED_SetCalibration_Type(CL_SPEED_Calibration_t C)
{
	if (CType)
		return 1;
	CType = C;
	Center_Voltage[CType-1][0] = 0;
	Center_Voltage[CType-1][1] = 0;
	if (C == Run1)
		DL_PWM_SetPulse(10,10);
	else if (C == Run2)
		DL_PWM_SetPulse(-10,-10);
	else
		DL_PWM_SetPulse(0,0);
	return 0;
}

#define SPEED_CALIBRATION_DELAY 10
void CL_SPEED_Calibration(void)
{
	if (CType && ++Calibration_Cal > SPEED_CALIBRATION_DELAY) //10 delay
	{
		Center_Voltage[CType-1][0] += Speed_Voltage[0];
		Center_Voltage[CType-1][1] += Speed_Voltage[1];
		if (Calibration_Cal >= SPEED_CALIBRATION_TIME + SPEED_CALIBRATION_DELAY)
		{
			Center_Voltage[CType-1][0] /= SPEED_CALIBRATION_TIME;
			Center_Voltage[CType-1][1] /= SPEED_CALIBRATION_TIME;
			Calibration_Cal = 0;
			CType = None;
		}
	}
}

#define abs(x) ((x)>0?(x):(-x))
#define mabs(x,y) ((x)>(y)?(x-y):(y-x))
uint8_t CL_SPEED_CheckCalibration(void)
{
	uint8_t i;
	CL_SPEED_CheckCalibration_Result = 1;
	if (mabs(Center_Voltage[0][0],Center_Voltage[0][1]) > 10)
		return 1;
	for (i=1; i<4;i++)
	{
		CL_SPEED_CheckCalibration_Result = i+1;
		if (mabs(Center_Voltage[0][0],Center_Voltage[i][0]) > 35)
			return i+1;
		if (mabs(Center_Voltage[0][1],Center_Voltage[i][1]) > 35)
			return i+1;
	}
	CL_SPEED_CheckCalibration_Result = 0;
	return 0;
}

#define CENTER_VOLTAGE 1490
//Try to fix if the Center Voltage is wrong
//Clear the wrong flag to make sure it works
void CL_SPEED_ClearCalibration(void)
{
	if (CL_SPEED_CheckCalibration_Result == 1)
	{
		Center_Voltage[0][0] = CENTER_VOLTAGE;
		Center_Voltage[0][1] = CENTER_VOLTAGE;
	}
	CL_SPEED_CheckCalibration_Result=0;
}

#define MAX_SPEED_VOLTAGE_L 8.2 //935
#define MAX_SPEED_VOLTAGE_R 8.0 //919
void CL_SPEED_GetSpeedNow(void)
{
	CL_SPEED_SpeedNow[0] = ((int32_t)Speed_Voltage[0] - Center_Voltage[0][0])/MAX_SPEED_VOLTAGE_L;
	CL_SPEED_SpeedNow[1] = ((int32_t)Speed_Voltage[1] - Center_Voltage[0][1])/MAX_SPEED_VOLTAGE_R;
}

//---------------------------------------------------------
int8_t Speed_Set[2]={0}; //-100~100
uint16_t CheckSpeedCount = 0;
void (*WarningFunction)(void) = 0;
int16_t TargetPWM[2];

void CL_SPEED_SetSpeed(int8_t L, int8_t R)
{
	if (L <= 100 && L >= -100)
		Speed_Set[0] = L;
	else
		Speed_Set[0] = L < 0 ? -100:100;
	if (R <= 100 && R >= -100)
		Speed_Set[1] = R;
	else
		Speed_Set[1] = R < 0 ? -100:100;
	TargetPWM[0] = 20*fabs(sqrt(abs(Speed_Set[0])));
	TargetPWM[1] = 20*fabs(sqrt(abs(Speed_Set[1])));
	TargetPWM[0] = Speed_Set[0] > 0 ? TargetPWM[0] : -TargetPWM[0];
	TargetPWM[1] = Speed_Set[1] > 0 ? TargetPWM[1] : -TargetPWM[1];
	//DL_PWM_SetPulse(TargetPWM[0],TargetPWM[1]);
}

void CL_SPEED_SetWarningFunction(void (*f)(void))
{
	WarningFunction = f;
}

void CL_SPEED_CheckSpeed(void)
{
	if ((abs(Speed_Set[0]) >= 30 && abs(CL_SPEED_SpeedNow[0]) < 25) 
		|| (abs(Speed_Set[1]) >= 30 && abs(CL_SPEED_SpeedNow[1]) < 25))
	{
		CheckSpeedCount++;
	}
	else
		CheckSpeedCount/=2;
	if (CheckSpeedCount > 10)//more than 0.1s
	{
		if (WarningFunction)
			WarningFunction();
	}
}

void CL_SPEED_CalPWM(void)
{
	int32_t PWM[2];
	int16_t SpeedDiff[2];
	SpeedDiff[0] = Speed_Set[0] - CL_SPEED_SpeedNow[0];
	SpeedDiff[1] = Speed_Set[1] - CL_SPEED_SpeedNow[1];
	PWM[0] = TargetPWM[0] * pow((double)Speed_Set[0] / CL_SPEED_SpeedNow[0],2) + SpeedDiff[0] / 2;
	PWM[1] = TargetPWM[1] * pow((double)Speed_Set[1] / CL_SPEED_SpeedNow[1],2) + SpeedDiff[1] / 2;
	if (abs(PWM[0]) > 200)
		PWM[0] = PWM[0] > 0 ? 200 : -200;
	if (abs(PWM[1]) > 200)
		PWM[1] = PWM[1] > 0 ? 200 : -200;
	DL_PWM_SetPulse(PWM[0],PWM[1]);
}

//---------------------------------------------------------
uint8_t Tick_Counter = 0;
void CL_SPEED_Tick_Pre(void) //Get Speed
{
	Speed_Voltage_Tmp[0] += DL_ADC_Voltage[0];
	Speed_Voltage_Tmp[1] += DL_ADC_Voltage[1];
	if (++Tick_Counter >= SPEED_SMOOTH_TIME)
	{
		Speed_Voltage[0] = Speed_Voltage_Tmp[0] / SPEED_SMOOTH_TIME;
		Speed_Voltage[1] = Speed_Voltage_Tmp[1] / SPEED_SMOOTH_TIME;
		Speed_Voltage_Tmp[0]=0;
		Speed_Voltage_Tmp[1]=0;
		Tick_Counter=0;
		CL_SPEED_Calibration();
		if (!CType && !CL_SPEED_CheckCalibration_Result)
		{
			CL_SPEED_GetSpeedNow();
			CL_SPEED_CheckSpeed();
		}
	}
}

void CL_SPEED_Tick_After(void) //Set Speed
{
	if (!Tick_Counter && !CType && !CL_SPEED_CheckCalibration_Result) //Got Current Speed and Not Testing
	{
		if(CL_SPEED_GameStatus == 1) //Game started
			CL_SPEED_CalPWM();
		else
			DL_PWM_SetPulse(0,0);
	}
}
