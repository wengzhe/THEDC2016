#include "AllDefs.h"

#include "Decision.h"
#include "POINTS.h"
#include "INF.h"
#include "Exec.h"
#include "MUSIC.h"
#include <math.h>

EL_POINTS_Queue_t QueueNode={{0,0},19,0};

EL_INF_PlayerInf_t *MyInf, *EmyInf;
EL_INF_ItemInf_t *ItemInf,ItemInfBak;
EL_INF_GameInf_t *GameInf,GameInfBak;
EL_INF_AirPlaneInf_t *AirPlaneInf;
EL_INF_TargetInf_t *TargetInf,TargetInfBak;
EL_INF_AdditionInf_t *AdditionInf;

__STATIC_INLINE float Distance(Point_t A, Point_t B)
{
	float dx = (float)A.x - (float)B.x, dy = (float)A.y - (float)B.y;
	return sqrtf(dx*dx+dy*dy);
}

#define POS_EQUAL(a,b) ((a).x == (b).x && (a).y == (b).y)

uint8_t CheckInfDiff(void)
{
	if ( ItemInfBak.Type != ItemInf->Type
		|| !POS_EQUAL(ItemInfBak.Pos,ItemInf->Pos)
		|| GameInfBak.Status != GameInf->Status
		|| TargetInfBak.Exist != TargetInf->Exist
		|| TargetInfBak.Black != TargetInf->Black
		|| !POS_EQUAL(TargetInfBak.Pos,TargetInf->Pos)
		|| (EL_POINTS_GetState() == POINTS_Stop)//AdditionInf->HugeHurt && 
		)
	{
		ItemInfBak = *ItemInf;
		GameInfBak = *GameInf;
		TargetInfBak = *TargetInf;
		return 1;
	}
	else
		return 0;
}

void Decision_MoveControl_Final(void)
{
	Point_t tar, MyPos;
	EL_POINTS_Color_t Color_Set = POINTS_None;
	QueueNode.MinDis = 19;
	if(TargetInf->Exist)
	{
		QueueNode.Target = TargetInf->Pos;
		QueueNode.StopTime = 1;
		EL_POINTS_InsertShadowStack(QueueNode);
		Color_Set = TargetInf->Black?POINTS_Black:POINTS_White;
	}
	if (ItemInf->Type)
	{
		QueueNode.Target = ItemInf->Pos;
		QueueNode.StopTime = 0;
		EL_POINTS_InsertShadowStack(QueueNode);
	}
	MyPos = MyInf->Pos;
	while (Color_Set && Distance(QueueNode.Target,MyPos) > 80) //There must be a node if color is set
	{
		tar.x = ((uint16_t)(QueueNode.Target.x) + (uint16_t)(MyPos.x))/2;
		tar.y = ((uint16_t)(QueueNode.Target.y) + (uint16_t)(MyPos.y))/2;
		//1@90'@10ms,0.7@45'@6ms,0.5@30'@3ms
		QueueNode.Target = CheckNearestColor(tar,Color_Set-1,0.7*Distance(tar,MyPos),15);
		QueueNode.StopTime = 0;
		QueueNode.MinDis = 0;//always trying
		//0.34@20'
		//QueueNode.MinDis = 0.34 * Distance(tar,MyPos);
		//QueueNode.MinDis = QueueNode.MinDis < 20 ? 20 : QueueNode.MinDis;
		EL_POINTS_InsertShadowStack(QueueNode);
	}
	EL_POINTS_SetColor(Color_Set);
	EL_POINTS_FinishShadowStack();
}

void Decision_MoveControl_Second(void)
{
	Point_t tar, MyPos;
#ifndef COMP_BLACK
	static Point_t WhitePos;
#ifdef COMP_NO_COLOR
	EL_POINTS_Color_t Color_Set = POINTS_None;
#else
	EL_POINTS_Color_t Color_Set = POINTS_White;
#endif
#else
	EL_POINTS_Color_t Color_Set = POINTS_Black;
#endif
	QueueNode.MinDis = 19;
	MyPos = MyInf->Pos;
	if (ItemInf->Type)
	{
		QueueNode.Target = ItemInf->Pos;
		QueueNode.StopTime = 1;
		EL_POINTS_InsertShadowStack(QueueNode);
		EL_POINTS_FinishShadowStack();//Run First
		EL_POINTS_InsertShadowStack(QueueNode);
#ifndef COMP_BLACK
		WhitePos = ItemInf->Pos;
#endif
		while (Color_Set && Distance(QueueNode.Target,MyPos) > 80)//Maybe Slow
		{
			tar.x = ((uint16_t)(QueueNode.Target.x) + (uint16_t)(MyPos.x))/2;
			tar.y = ((uint16_t)(QueueNode.Target.y) + (uint16_t)(MyPos.y))/2;
			//1@90'@10ms,0.7@45'@6ms,0.5@30'@3ms
			QueueNode.Target = CheckNearestColorSlow(tar,Color_Set-1,0.7*Distance(tar,MyPos),15);
			QueueNode.StopTime = 0;
			QueueNode.MinDis = 0;//always trying
			//0.34@20'
			//QueueNode.MinDis = 0.34 * Distance(tar,MyPos);
			//QueueNode.MinDis = QueueNode.MinDis < 20 ? 20 : QueueNode.MinDis;
			EL_POINTS_InsertShadowStack(QueueNode);
		}
	}
#ifndef COMP_BLACK
	else if (AdditionInf->HugeHurt || MyInf->OutOfBound)//make sure we're in the white
#else
	else if (AdditionInf->LittleHurt || MyInf->OutOfBound)//make sure we're in the black
#endif
	{
		QueueNode.MinDis = 4;
#ifndef COMP_BLACK
		QueueNode.Target = CheckNearestColorExceptHereSlow(MyPos,Color_Set-1,20,5);
		if (POS_EQUAL(MyPos,QueueNode.Target))
		{
			QueueNode.Target = WhitePos;
		}
#else
		QueueNode.Target = CheckNearestColorExceptHereSlow(MyPos,Color_Set-1,150,10);
#endif
		QueueNode.StopTime = 1;
		EL_POINTS_InsertShadowStack(QueueNode);
	}
	EL_POINTS_SetColor(Color_Set);
	EL_POINTS_FinishShadowStack();//Will Stop if no nodes
}

void Decision_MoveControl(void)
{
	Decision_MoveControl_Second();
	//Decision_MoveControl_Final();
}

void Decision_FlightControl(void)
{
	Point_t tar;
	if(AirPlaneInf->Control)
	{
		tar.x = 255-QueueNode.Target.x;
		tar.y = 255-QueueNode.Target.y;
		EL_POINTS_SetFlightPos(tar);
	}
}

void Decision_Init(void)
{
	EL_Init();
	
	MyInf = EL_INF_GetMyInf();
	EmyInf = EL_INF_GetEmyInf();
	ItemInf = EL_INF_GetItemInf();
	GameInf = EL_INF_GetGameInf();
	AirPlaneInf = EL_INF_GetAirPlaneInf();
	TargetInf = EL_INF_GetTargetInf();
	AdditionInf = EL_INF_GetAdditionInf();

	EL_MUSIC_ChangeStatus(Music,1);
	EL_MUSIC_ChangeMode(Random);
	EL_MUSIC_SetPause(1);

	EL_POINTS_SetBorderSafetyDis(10);
}

void Decision_MakeDecision(void)
{
	if (GameInf->Status == GAME_START)
	{
		//Move
		if(CheckInfDiff())
			Decision_MoveControl();
		//Flight
		Decision_FlightControl();
		//Music
		EL_MUSIC_SetPause(1);
	}//GameStart
#ifdef PLAYMUSIC
	else if (GameInf->Status == GAME_PAUSE)
		EL_MUSIC_SetPause(0);
#endif
}
