#include "Angle.h"
#include "IMU.h"
#include "UART.h"
#include "pwm.h"
#include <math.h>

//L+ R-

#define MAX_ANGLE_DIFF 20
#define MIN_ANGLE_DIS 50

#define MIN_ANGLE_SPEED 100

#define DEGREE_QUEUE_NUM 1000
float Degree[DEGREE_QUEUE_NUM] = {0};
uint16_t DegreeNum = 0, DegreePointer = 0;
float DegreeSet = 0;
CL_ANGLE_Type_t DegreeSetType = ANGLE_REL;

#define POINTS_QUEUE_NUM 10
CL_ANGLE_Point_t Points[POINTS_QUEUE_NUM] = {0};
uint8_t Points_front = 0, Points_Num = 0;
float DegreeDiff = 0;//diff = abs - rel;
uint16_t DegreeDiffCnt = 0;

float M_PI = 0;

__STATIC_INLINE float GetDegree180(float input)
{
	if (input > 180)
		input -= 360;
	if (input < -180)
		input += 360;
	return input;
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

__STATIC_INLINE void InsertQueue(Point_t Point,uint16_t DegreeP)
{
	uint8_t p = (Points_front+Points_Num)%POINTS_QUEUE_NUM;
	Points[p].Point = Point;
	Points[p].DegreeP = DegreeP;
	Points[p].Speed = DL_PWM_Speed();
	Points_Num++;
	if (Points_Num > POINTS_QUEUE_NUM)
	{
		Points_front++;
		Points_Num--;
		Points_front%=POINTS_QUEUE_NUM;
	}
}

__STATIC_INLINE void POPQueue(uint8_t num)
{
	Points_Num-=num;
	Points_front+=num;
	Points_front%=POINTS_QUEUE_NUM;
}

/*__STATIC_INLINE void ClearQueue(void)
{
	Points_Num=0;
	Points_front=0;
}*/

#define mabs(x,y) ((x)>(y)?(x-y):(y-x))
__STATIC_INLINE uint16_t Distance(Point_t A, Point_t B)
{
	return mabs(A.x,B.x) + mabs(A.y,B.y);
}

void CL_ANGLE_SetDegree(float degree, CL_ANGLE_Type_t type)
{
	DegreeSet = DegreeSetType == ANGLE_REL ? GetDegree180(degree+Degree[DegreePointer]) : GetDegree180(degree);
	DegreeSetType = type;
}

float CL_ANGLE_GetDegreeDiff(void)
{
	float cur_deg = DegreeSetType == ANGLE_REL ? Degree[DegreePointer] : Degree[DegreePointer] + DegreeDiff;
	return MinusDegree180(DegreeSet, cur_deg);
}

float CL_ANGLE_GetDegreeAbs(void)
{
	float cur_deg = Degree[DegreePointer] + DegreeDiff;
	return GetDegree180(cur_deg);
}

uint8_t CL_ANGLE_DegreeAbsOK(void)
{
	return DegreeDiffCnt != 0;
}
void CL_ANGLE_GetPos(const DL_UART_Data_t* uartdata)
{
	int8_t pn;
	InsertQueue(uartdata->MyPos,DegreePointer);
	for (pn = Points_Num-2; pn >= 0; pn--)
	{
		uint8_t p = (Points_front+pn)%POINTS_QUEUE_NUM;
		if (Distance(uartdata->MyPos,Points[p].Point) >= MIN_ANGLE_DIS
			&& fabs(MinusDegree180(Degree[DegreePointer],Degree[Points[p].DegreeP])) < MAX_ANGLE_DIFF)
		{
			uint16_t DP = (((DEGREE_QUEUE_NUM + DegreePointer - Points[p].DegreeP) % DEGREE_QUEUE_NUM) / 2
												+ Points[p].DegreeP) % DEGREE_QUEUE_NUM;
			float diff = MinusDegree180(
											atan2f(uartdata->MyPos.x - Points[p].Point.x, uartdata->MyPos.y - Points[p].Point.y)/M_PI*180
											,Degree[DP]+DegreeDiff);
			if (!DegreeDiffCnt)
				DegreeDiff = diff;
			else
			{
				if (Points[p].Speed < -MIN_ANGLE_SPEED
						&& Points[(Points_front+Points_Num-1)%POINTS_QUEUE_NUM].Speed < -MIN_ANGLE_SPEED
						&& fabs(diff) > 90)
				{
					DegreeDiff += MinusDegree180(diff,180)*0.1;
				}
				if (Points[p].Speed > MIN_ANGLE_SPEED
						&& Points[(Points_front+Points_Num-1)%POINTS_QUEUE_NUM].Speed > MIN_ANGLE_SPEED)
				{
					DegreeDiff += diff * 0.1;
				}
			}
			DegreeDiff = GetDegree180(DegreeDiff);
			DegreeDiffCnt++;
			POPQueue(Points_Num-1);
			break;
		}
	}
}

void CL_ANGLE_Init(void)
{
	DL_UART_SetRxFunc(CL_ANGLE_GetPos);
	DegreeNum = 0;
	DegreePointer = 0;
	Degree[0] = 0;
	M_PI = acosf(-1.0);
	DegreeDiffCnt = 0;
	DegreeDiff = 0;
}

void CL_ANGLE_Reset(void)
{
	DegreeNum = 0;
	DegreePointer = 0;
	Degree[0] = 0;
	Points_Num = 0;
	DegreeDiffCnt = 0;
	DegreeDiff = 0;
}

void CL_ANGLE_Tick(void)
{
	float NextDegree = DL_IMU_GetGyro()+Degree[DegreePointer++];
	DegreePointer %= DEGREE_QUEUE_NUM;
	Degree[DegreePointer] = GetDegree180(NextDegree);
	if (DegreeNum < DEGREE_QUEUE_NUM)
		DegreeNum++;
	if (Points_Num && Points[Points_front].DegreeP == DegreePointer)
	{
		POPQueue(1);
	}
}
