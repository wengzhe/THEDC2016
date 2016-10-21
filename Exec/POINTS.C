#include "POINTS.h"
#include "COM.h"
#include "Angle.h"
#include "Speed.h"
#include <math.h>

Point_t EL_POINTS_SingleTarget, EL_POINTS_MyPos;
uint8_t SingleTargetSet = 0;
uint8_t SingleTargetArrCnt = 0; //arrived count
uint16_t SingleTargetStopCnt[2] = {0,1000}; //now,max

uint8_t SingleTargetMinDis = 5;

#ifdef GAME_STATUS_START
GameStatus_t EL_POINTS_GameStatus = GAME_START;
#else
GameStatus_t EL_POINTS_GameStatus = GAME_STOP;
#endif
EL_POINTS_Type_t EL_POINTS_Type = POINTS_STOP;

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

void EL_POINTS_GetCom(const CL_COM_Data_t* data)
{
	EL_POINTS_MyPos = data->MyPos;
	EL_POINTS_GameStatus = data->GameStatus;
}

void EL_POINTS_Init(void)
{
	CL_COM_SetRxFunc(EL_POINTS_GetCom);
}

void EL_POINTS_DirectTarget(Point_t tar)
{
	EL_POINTS_SingleTarget = tar;
	SingleTargetSet = 1;
}

void EL_POINTS_StopDirectTarget(void)
{
	SingleTargetSet = 0;
}

#define P_GO 1
#define P_BACK 1
#define P_SPEED_BACK(x) ((x)*7/8)
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
		
		//SingleTarget
		if (SingleTargetSet)
		{
			uint16_t dis = Distance(EL_POINTS_SingleTarget, EL_POINTS_MyPos);
			if (dis >= SingleTargetMinDis)
			{
				float TurnDegree;
				uint16_t speed;
				CL_ANGLE_SetDegree(atan2f(EL_POINTS_SingleTarget.x - EL_POINTS_MyPos.x,
					EL_POINTS_SingleTarget.y - EL_POINTS_MyPos.y)/M_PI*180,ANGLE_ABS);
				TurnDegree = CL_ANGLE_GetDegree();
				speed = dis + 20;
				if (abs(TurnDegree) < 90) //go
				{
					float ABS_TD;
					TurnDegree*=P_GO;
					ABS_TD = abs(TurnDegree);
					SPEED[1]=SPEED[0]=(speed>(100-ABS_TD))?(100-ABS_TD):(speed);
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
					SPEED[1]=SPEED[0]=(speed>(100-ABS_TD))?(ABS_TD-100):(-speed);
					SPEED[0] -= TurnDegree;
					SPEED[1] += TurnDegree;
					SPEED[0] = P_SPEED_BACK(SPEED[0]);
					SPEED[1] = P_SPEED_BACK(SPEED[1]);
				}
				SingleTargetStopCnt[0] = SingleTargetStopCnt[1];
			}
			else
			{
				if (!--SingleTargetStopCnt[0])
				{
					SingleTargetSet = 0;
					SingleTargetArrCnt++;
				}
			}
		}
		
		//track
		
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

void EL_POINTS_SetDirectMinDistance(uint8_t dis)
{
	SingleTargetMinDis = dis;
}

void EL_POINTS_SetDirectMinTime(uint16_t tim)
{
	SingleTargetStopCnt[1] = tim;
}

void EL_POINTS_Tick()
{
	if (EL_POINTS_GameStatus == GAME_WAIT || EL_POINTS_GameStatus == GAME_STOP)
	{
		CL_ANGLE_Reset();
	}
	if (EL_POINTS_GameStatus == GAME_START)
	{
		EL_POINTS_Run();
	}
	else
	{
		CL_SPEED_SetSpeed(0,0);
	}
}
