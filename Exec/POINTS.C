#include "AllDefs.h"

#include "POINTS.h"
#include "COM.h"
#include "Angle.h"
#include "Speed.h"
#include "MAP.h"
#include <math.h>

#define MID_POINT_DIS 15 //the added mid points has dis longer than to target
#define DANGER_ANGLE_TAR_SPEED_ADD 40

//watchdog
#define POINTS_WATCHDOG_CNT 400

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
#define FIRST_RUN_TIME 5000

//#define mabs(x,y) ((x)>(y)?(x)-(y):(y)-(x))
#define abs(x) ((x)>0?(x):-(x))
__STATIC_INLINE float Distance(Point_t A, Point_t B)
{
	float dx = (float)A.x - (float)B.x, dy = (float)A.y - (float)B.y;
	return sqrtf(dx*dx+dy*dy);
}

__STATIC_INLINE float MinusDegree180(float A, float B)
{
	float input = A - B;
	if (input > 180)
		input -= 360;
	if (input < -180)
		input += 360;
	return input;
}

//RunControl

//Target
Point_t EL_POINTS_DisTarget, EL_POINTS_AngleTarget, EL_POINTS_MyPos;
EL_POINTS_Way_t EL_POINTS_Way = POINTS_Stop;
uint8_t TargetMinDis = 5;
uint16_t TargetArrivedTime = 0;//The time arrived in MinDis
uint8_t BorderDis[2] = {0,255};
#define MAKE_BORDER(x) ((x)=(x)<BorderDis[0]?BorderDis[0]:(x)>BorderDis[1]?BorderDis[1]:(x))
#define CHECK_BORDER(x) ((x)>=BorderDis[0]&&(x)<=BorderDis[1])

//Queue
#define POINTS_QUEUE_NUM 10
EL_POINTS_Queue_t EL_POINTS_Queue[POINTS_QUEUE_NUM];
uint8_t PointsNum = 0, PointsPointer = 0;
EL_POINTS_Queue_t EL_POINTS_ShadowStack[POINTS_QUEUE_NUM];
uint8_t ShadowPointer = 0;

//AngleAndSpeed
float EL_POINTS_Dis = 0;
uint16_t EL_POINTS_Speed = 0;
uint16_t CalcTargetWatchDog=0;
float EL_POINTS_AngleSet = 0;

//Color
EL_POINTS_Color_t TargetColor=POINTS_None;//0:white,1:black

//Flight
Point_t FlightPos={0,0};

//State
EL_POINTS_Way_t EL_POINTS_GetState(void)
{
	return EL_POINTS_Way;
}

//Queue
uint8_t EL_POINTS_InsertQueue(EL_POINTS_Queue_t input)
{
	if (PointsNum < POINTS_QUEUE_NUM)
	{
		uint8_t p = (PointsPointer+PointsNum)%POINTS_QUEUE_NUM;
		EL_POINTS_Queue[p] = input;
		MAKE_BORDER(EL_POINTS_Queue[p].Target.x);
		MAKE_BORDER(EL_POINTS_Queue[p].Target.y);
		PointsNum++;
		return 0;
	}
	else
		return 1;
}

void EL_POINTS_ClearQueue(void)
{
	if (PointsNum)
	{
		PointsNum=0;
		EL_POINTS_StopTarget();
		PointsPointer = 0;
	}
}

//ShadowPointer will be 0 when finished, so sometimes no use
void EL_POINTS_ClearShadowStack(void)
{
	ShadowPointer = 0;
}

uint8_t EL_POINTS_InsertShadowStack(EL_POINTS_Queue_t input)
{
	if (ShadowPointer < POINTS_QUEUE_NUM)
	{
		EL_POINTS_ShadowStack[ShadowPointer++] = input;
		return 0;
	}
	else
		return 1;
}

//The top one will always be the last one, so maybe we do not need this
EL_POINTS_Queue_t *EL_POINTS_ShadowStackTop(void)
{
	if (ShadowPointer)
		return &EL_POINTS_ShadowStack[ShadowPointer-1];
	else
		return 0;
}

void EL_POINTS_FinishShadowStack(void)
{
	EL_POINTS_ClearQueue();
	for (; ShadowPointer; ShadowPointer--)
		EL_POINTS_InsertQueue(EL_POINTS_ShadowStack[ShadowPointer-1]);
}

__STATIC_INLINE void POPQueue(void)
{
	PointsPointer++;
	PointsPointer%=POINTS_QUEUE_NUM;
	PointsNum--;
}

//Border
void EL_POINTS_SetBorderSafetyDis(uint8_t dis)
{
	BorderDis[0] = dis;
	BorderDis[1] = 255-dis;
}

void EL_POINTS_MakeBorder(void)
{
	MAKE_BORDER(EL_POINTS_DisTarget.x);
	MAKE_BORDER(EL_POINTS_DisTarget.y);
	MAKE_BORDER(EL_POINTS_AngleTarget.x);
	MAKE_BORDER(EL_POINTS_AngleTarget.y);
}

//We Need To Calculate Next Target(AngleTarget) and check the final point(DisTarget)
//return fullspeed?
//if fullspeed is set, we'll not stop and must go to next point at full speed
//if not set, when we're near the final point, we'll stop or be very slow
uint8_t EL_POINTS_CalcTarget(void)
{
	uint8_t i;
	EL_POINTS_AngleTarget = EL_POINTS_Queue[PointsPointer].Target;
	for (i = 0; i < PointsNum - 1 && EL_POINTS_Queue[(PointsPointer+i)%POINTS_QUEUE_NUM].StopTime == 0; i++);
	EL_POINTS_DisTarget = EL_POINTS_Queue[(PointsPointer+i)%POINTS_QUEUE_NUM].Target;
	TargetMinDis = EL_POINTS_Queue[(PointsPointer+i)%POINTS_QUEUE_NUM].MinDis;
	return i;
}

uint8_t EL_POINTS_CheckQueue(void)
{
	if (PointsNum)
	{
		if (EL_POINTS_Way == POINTS_Queue)
		{
			if (EL_POINTS_Queue[PointsPointer].MinDis == 0 && EL_POINTS_Queue[PointsPointer].StopTime == 0) //MidPoint
			{
				if ((PointsNum > 1
					&& Distance(EL_POINTS_Queue[PointsPointer].Target, EL_POINTS_Queue[(PointsPointer+1)%POINTS_QUEUE_NUM].Target) + MID_POINT_DIS
					 > Distance(EL_POINTS_Queue[(PointsPointer+1)%POINTS_QUEUE_NUM].Target, EL_POINTS_MyPos))
					|| (Distance(EL_POINTS_Queue[PointsPointer].Target, EL_POINTS_MyPos) <= MID_POINT_DIS)
					)
				{
					POPQueue();
				}
			}
			if (Distance(EL_POINTS_Queue[PointsPointer].Target, EL_POINTS_MyPos) <= EL_POINTS_Queue[PointsPointer].MinDis)
			{
				if (!EL_POINTS_Queue[PointsPointer].StopTime //Change target
						|| TargetArrivedTime/10 > EL_POINTS_Queue[PointsPointer].StopTime)
				{
					POPQueue();
					if (!PointsNum)
						EL_POINTS_Way = POINTS_Stop;
				}
			}
		}
		else
			EL_POINTS_Way = POINTS_Queue;
		return EL_POINTS_CalcTarget();
	}
	else
		return 0;
}

void EL_POINTS_CalcSpeed_SetAngle(void)
{
	uint8_t fullspeed=EL_POINTS_CheckQueue();
	if (EL_POINTS_Way)
	{
		//Check Target
		EL_POINTS_MakeBorder();
		//Calculate Dis&Angle
		EL_POINTS_Dis = fullspeed ? 80 : Distance(EL_POINTS_DisTarget, EL_POINTS_MyPos);
		EL_POINTS_Speed = EL_POINTS_Dis + 20;
		EL_POINTS_AngleSet = atan2f(EL_POINTS_AngleTarget.x - EL_POINTS_MyPos.x,
													EL_POINTS_AngleTarget.y - EL_POINTS_MyPos.y)/M_PI*180;
		CL_ANGLE_SetDegree(EL_POINTS_AngleSet, ANGLE_ABS);
		if (EL_POINTS_Speed > 80)
		{
			float Angle = EL_POINTS_AngleSet/180*M_PI;
			int16_t x = EL_POINTS_MyPos.x + EL_POINTS_Speed*sinf(Angle)*0.2;//will be out in 0.2s
			int16_t y = EL_POINTS_MyPos.y + EL_POINTS_Speed*cosf(Angle)*0.2;//will be out in 0.2s
			if (!(CHECK_BORDER(x) && CHECK_BORDER(y)))
			{
				EL_POINTS_Speed = Distance(EL_POINTS_AngleTarget, EL_POINTS_MyPos) + DANGER_ANGLE_TAR_SPEED_ADD;
			}
		}
	}
	else
	{
		EL_POINTS_Dis = EL_POINTS_Speed = 0;
		CL_ANGLE_SetDegree(0,ANGLE_REL);
	}
	CalcTargetWatchDog = POINTS_WATCHDOG_CNT;
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
	EL_POINTS_ClearQueue();
	EL_POINTS_DisTarget = tar;
	EL_POINTS_AngleTarget = tar;
	EL_POINTS_Way = POINTS_Single;
	EL_POINTS_CalcSpeed_SetAngle();
}

void EL_POINTS_StopTarget(void)
{
	EL_POINTS_Way = POINTS_Stop;
}

//IR
#define ANGLE_DIFF 40 //different between 2 angles of IR
#define MAX_ANGLE_IN_COLOR 45 //max turn angle
#define MIN_ANGLE_TURN 20 //if angle < 20, we see it as the same with the front
//Distance change slow, but angle change fast
EL_POINTS_Dir_t RunDir = POINTS_Front;
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
		if (!(--FirstRunCnt) || CL_ANGLE_DegreeAbsOK())
			EL_POINTS_Type = POINTS_RUNNING;
	}
	if (EL_POINTS_Type == POINTS_RUNNING)
	{
		int16_t SPEED[2] = {0,0};
		if (TargetColor)
		{
			const CL_MAP_IR_t *pIR = CL_MAP_GetIR();
			uint8_t i;
			float minAngle = 300;
			CL_ANGLE_SetDegree(EL_POINTS_AngleSet, ANGLE_ABS);
			for (i = 0; i < IR_NUM; i++)
			{
				float Angle = (pIR->Angle[i])/180*M_PI;
				int16_t x = EL_POINTS_MyPos.x + IR_DIS*sinf(Angle);
				int16_t y = EL_POINTS_MyPos.y + IR_DIS*cosf(Angle);
			
				if (pIR->Color[i] == (uint8_t)TargetColor - 1
						&& (CHECK_BORDER(x) && CHECK_BORDER(y))
						&& fabs(MinusDegree180(CL_ANGLE_GetDegreeAbs() + (uint8_t)RunDir * 180,pIR->Angle[i])) < 90)
				{
					float anglediff = fabs(MinusDegree180(EL_POINTS_AngleSet,pIR->Angle[i]));
					if (anglediff < MAX_ANGLE_IN_COLOR + ANGLE_DIFF && anglediff < minAngle)
					{
						minAngle = anglediff;
						if (anglediff > MIN_ANGLE_TURN)
							CL_ANGLE_SetDegree(pIR->Angle[i], ANGLE_ABS); //maybe not the nearest, but the second, so choose.
					}
				}
				else
				{
					//how about the different color?
				}
			}
			if (minAngle <= MIN_ANGLE_TURN) //The nearest is right, go ahead
				CL_ANGLE_SetDegree(EL_POINTS_AngleSet, ANGLE_ABS);
		}
		if (EL_POINTS_Dis > TargetMinDis)
		{
			float TurnDegree = CL_ANGLE_GetDegreeDiff();
#ifdef BACK_OK
			if ((abs(TurnDegree) < 90 //go
#ifndef BACK_ON_COLOR
			|| TargetColor
#endif
#ifdef ONLY_BACK_WHEN_NEAR
			|| EL_POINTS_Dis > NEAR_DIS
#endif
			) //ORs above

			) //ANDs above
#endif //BACK_OK
			{
				float ABS_TD;
				TurnDegree*=P_ANGLE_GO;
				ABS_TD = abs(TurnDegree);
				SPEED[1]=SPEED[0]=(EL_POINTS_Speed>(100-ABS_TD))?(100-ABS_TD):(EL_POINTS_Speed);
				SPEED[0] -= TurnDegree;
				SPEED[1] += TurnDegree;
				RunDir = POINTS_Front;
			}
#ifdef BACK_OK
			else //back
			{
				float ABS_TD;
				if (TurnDegree < 0)
					TurnDegree += 180;
				else
					TurnDegree -= 180;
				TurnDegree*=P_ANGLE_BACK;
				ABS_TD = abs(TurnDegree);
				SPEED[1]=SPEED[0]=(EL_POINTS_Speed>(100-ABS_TD))?(ABS_TD-100):(-EL_POINTS_Speed-5);
				SPEED[0] -= TurnDegree;
				SPEED[1] += TurnDegree;
				SPEED[0] = P_SPEED_BACK(SPEED[0]);
				SPEED[1] = P_SPEED_BACK(SPEED[1]);
				RunDir = POINTS_Back;
			}
#endif
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

void EL_POINTS_Tick(void)
{
	if (EL_POINTS_GameStatus == GAME_WAIT || EL_POINTS_GameStatus == GAME_STOP)
	{
		CL_ANGLE_Reset();
		EL_POINTS_Type = POINTS_STOP;
	}
	if (EL_POINTS_GameStatus == GAME_START)
	{
		//Distance change slow, but angle change fast
		EL_POINTS_Run();
		CL_COM_SendPos(FlightPos);
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

void EL_POINTS_SetFlightPos(const Point_t tar)
{
	FlightPos = tar;
}
