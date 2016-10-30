#include "POINTS.h"
#include "COM.h"
#include "Angle.h"
#include "Speed.h"
#include <math.h>


//GameStatus
#ifdef GAME_STATUS_START
GameStatus_t EL_POINTS_GameStatus = GAME_START;
#else
GameStatus_t EL_POINTS_GameStatus = GAME_STOP;
#endif
EL_POINTS_Type_t EL_POINTS_Type = POINTS_STOP;

//FirstRun
uint16_t FirstRunCnt = 0;
#define FIRST_RUN_SPEED 70
#define FIRST_RUN_TIME 600

//#define mabs(x,y) ((x)>(y)?(x-y):(y-x))
#define abs(x) ((x)>0?(x):(-x))
__STATIC_INLINE uint16_t Distance(Point_t A, Point_t B)
{
	float dx = (float)A.x - (float)B.x, dy = (float)A.y - (float)B.y;
	return sqrtf(dx*dx+dy*dy);
}

//RunControl
//Target
Point_t EL_POINTS_DisTarget, EL_POINTS_AngleTarget, EL_POINTS_MyPos;
EL_POINTS_Way_t EL_POINTS_Way = POINTS_Stop;
uint8_t TargetMinDis = 5;
uint16_t TargetArrivedTime = 0;//The time arrived in MinDis
//AngleAndSpeed
uint16_t EL_POINTS_Dis = 0, EL_POINTS_Speed = 0;
uint8_t CalcTargetWatchDog=0;
//Color
EL_POINTS_Color_t TargetColor=POINTS_None;//0:white,1:black

//We Need To Calculate Next Target(AngleTarget) and check the final point(DisTarget)
//return fullspeed?
//if fullspeed is set, we'll not stop and must go to next point at full speed
//if not set, when we're near the final point, we'll stop or be very slow
uint8_t EL_POINTS_CalcNextTarget(void)
{
	if (EL_POINTS_Way == POINTS_Queue)
	{
		if (TargetArrivedTime > 0) //Change target
		{
		}
	}
	if (TargetColor)
	{
	}
	return 0;
}

void EL_POINTS_CalcSpeed_SetAngle(void)
{
	if (EL_POINTS_Way)
	{
		uint8_t fullspeed=EL_POINTS_CalcNextTarget();
		EL_POINTS_Dis = fullspeed ? 80 : Distance(EL_POINTS_DisTarget, EL_POINTS_MyPos);
		EL_POINTS_Speed = EL_POINTS_Dis + 20;
		CL_ANGLE_SetDegree(atan2f(EL_POINTS_AngleTarget.x - EL_POINTS_MyPos.x,
			EL_POINTS_AngleTarget.y - EL_POINTS_MyPos.y)/M_PI*180,ANGLE_ABS);
	}
	else
	{
		EL_POINTS_Dis = EL_POINTS_Speed = 0;
		CL_ANGLE_SetDegree(0,ANGLE_REL);
	}
	CalcTargetWatchDog = 200;
}

void EL_POINTS_GetCom(const CL_COM_Data_t* data)
{
	EL_POINTS_MyPos = data->MyPos;
	EL_POINTS_GameStatus = data->GameStatus;
	EL_POINTS_CalcSpeed_SetAngle();
}

void EL_POINTS_Init(void)
{
	CL_COM_SetRxFunc(EL_POINTS_GetCom);
}

void EL_POINTS_SetColor(EL_POINTS_Color_t color)
{
	TargetColor = color;
	EL_POINTS_CalcSpeed_SetAngle();
}

void EL_POINTS_DirectTarget(Point_t tar)
{
	EL_POINTS_DisTarget = tar;
	EL_POINTS_AngleTarget = tar;
	EL_POINTS_Way = POINTS_Single;
	EL_POINTS_CalcSpeed_SetAngle();
}

void EL_POINTS_StopTarget(void)
{
	EL_POINTS_Way = POINTS_Stop;
}

#define P_GO 1
#define P_BACK 1
#define P_SPEED_BACK(x) ((x)*7/8)
//Distance change slow, but angle change fast
void EL_POINTS_Run(void)
{
	if (EL_POINTS_Type == POINTS_STOP)
	{
		EL_POINTS_Type = POINTS_FIRST_RUN;
		FirstRunCnt = FIRST_RUN_TIME;
		CL_SPEED_SetSpeed(FIRST_RUN_SPEED,FIRST_RUN_SPEED);
	}
	else if (EL_POINTS_Type == POINTS_FIRST_RUN)
	{
		if (!(--FirstRunCnt))
			EL_POINTS_Type = POINTS_RUNNING;
	}
	if (EL_POINTS_Type == POINTS_RUNNING)
	{
		int16_t SPEED[2] = {0,0};
		if (EL_POINTS_Dis >= TargetMinDis)
		{
			float TurnDegree = CL_ANGLE_GetDegreeDiff();
			if (abs(TurnDegree) < 90) //go
			{
				float ABS_TD;
				TurnDegree*=P_GO;
				ABS_TD = abs(TurnDegree);
				SPEED[1]=SPEED[0]=(EL_POINTS_Speed>(100-ABS_TD))?(100-ABS_TD):(EL_POINTS_Speed);
				SPEED[0] -= TurnDegree;
				SPEED[1] += TurnDegree;
			}
			else //back
			{
				float ABS_TD;
				if (TurnDegree < 0)
					TurnDegree += 180;
				else
					TurnDegree -= 180;
				TurnDegree*=P_BACK;
				ABS_TD = abs(TurnDegree);
				SPEED[1]=SPEED[0]=(EL_POINTS_Speed>(100-ABS_TD))?(ABS_TD-100):(-EL_POINTS_Speed);
				SPEED[0] -= TurnDegree;
				SPEED[1] += TurnDegree;
				SPEED[0] = P_SPEED_BACK(SPEED[0]);
				SPEED[1] = P_SPEED_BACK(SPEED[1]);
			}
			TargetArrivedTime = 0;
		}
		else
			TargetArrivedTime++;

		//end
		if (SPEED[0] > 100)
			SPEED[0] = 100;
		if (SPEED[0] < -100)
			SPEED[0] = -100;
		if (SPEED[1] > 100)
			SPEED[1] = 100;
		if (SPEED[1] < -100)
			SPEED[1] = -100;
		CL_SPEED_SetSpeed(SPEED[0],SPEED[1]);
	}
}

void EL_POINTS_SetMinDistance(uint8_t dis)
{
	TargetMinDis = dis;
}

void EL_POINTS_Tick()
{
	if (EL_POINTS_GameStatus == GAME_WAIT || EL_POINTS_GameStatus == GAME_STOP)
	{
		CL_ANGLE_Reset();
	}
	if (EL_POINTS_GameStatus == GAME_START)
	{
		//Distance change slow, but angle change fast
		EL_POINTS_Run();
	}
	else
	{
		CL_SPEED_SetSpeed(0,0);
	}
	if (!CalcTargetWatchDog || !(--CalcTargetWatchDog))
	{
		CL_SPEED_SetSpeed(0,0);
	}
}
