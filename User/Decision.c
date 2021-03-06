#include "AllDefs.h"

#include "Decision.h"
#include "POINTS.h"
#include "INF.h"
#include "Exec.h"
#include "MUSIC.h"
#include <math.h>

EL_POINTS_Queue_t QueueNode={{0,0},19,0};
Point_t MyTarget = {128,128};
EL_POINTS_Color_t ColorSetNow = POINTS_None;
uint8_t MyDisSetNow = 19;

EL_INF_PlayerInf_t *MyInf, *EmyInf;
EL_INF_ItemInf_t *ItemInf,ItemInfBak;
EL_INF_GameInf_t *GameInf,GameInfBak;
EL_INF_AirPlaneInf_t *AirPlaneInf;
EL_INF_TargetInf_t *TargetInf,TargetInfBak;
EL_INF_AdditionInf_t *AdditionInf;
EL_INF_PlayerEstimate_t *MyEstimate, *EmyEstimate;

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
#ifndef RULE_LIFE
		|| AdditionInf->HugeHurt
#endif
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

const Point_t StopPoints[9] = {{128,128},{168,128},{128,168},{88,128},{128,88},{168,168},{168,88},{88,88},{88,168}};
u8 FlightToTarget = 0;
//Final:We need consider the enemy's move, see "��������.docx"
void Decision_MoveControl_Final(void)
{
	Point_t tar, MyPos, tarFinal;
	EL_POINTS_Color_t Color_Set = POINTS_None;
	uint8_t MinDis = 19, i;
	float NearestStopPointDis = 500;
	MyPos = MyInf->Pos;
	FlightToTarget = 0;
	
	//12
	//tar.x = 128;
	//tar.y = 128;
	for (i = 0; i < 9; i++)
	{
		if (Distance(AirPlaneInf->Pos, StopPoints[i]) > AIRPLANE_ATTACK_RANGE
			&& NearestStopPointDis > Distance(MyInf->Pos, StopPoints[i]))
		{
			tar = StopPoints[i];
			NearestStopPointDis = Distance(MyInf->Pos, StopPoints[i]);
		}
	}
	
	//12
	if (TargetInf->Black == 0)
		tar = AirPlaneInf->Pos;

	//11
	if (TargetInf->Black == 0 && AirPlaneInf->Control)
	{
		if (Distance(MyInf->Pos, AirPlaneInf->Pos) > AIRPLANE_HEAL_RANGE)
			tar = AirPlaneInf->Pos;
		else
			tar = EmyInf->Pos;
	}
	
	//10
	if(TargetInf->Exist)
	{
		tar = TargetInf->Pos;
	}
	
	tarFinal = tar;
	
	//9
	if (ItemInf->Type != ITEM_NULL)
	{
		tar = ItemInf->Pos;
	}
	
	//4-8
	if (TargetInf->Exist)
	{
		if (TargetInf->Black == 0)//white
		{
			tar = TargetInf->Pos; //4
			//Addition4
			if (Distance(AirPlaneInf->Pos,TargetInf->Pos) > AIRPLANE_HEAL_RANGE)
			{
				if (Distance(AirPlaneInf->Pos,MyInf->Pos) < Distance(MyInf->Pos,TargetInf->Pos) + AIRPLANE_HEAL_RANGE)
				{
					int16_t dx = (int16_t)TargetInf->Pos.x - (int16_t)AirPlaneInf->Pos.x;
					int16_t dy = (int16_t)TargetInf->Pos.y - (int16_t)AirPlaneInf->Pos.y;
					float scale = (float)(AIRPLANE_HEAL_RANGE - 10) / sqrtf(dx*dx+dy*dy);
					dx = AirPlaneInf->Pos.x + dx*scale;
					dy = AirPlaneInf->Pos.y + dy*scale;
					tar.x = dx > 255 ? 255 : dx < 0 ? 0 : dx;
					tar.y = dy > 255 ? 255 : dy < 0 ? 0 : dy;
					if (Distance(EmyInf->Pos,TargetInf->Pos) > 20)
						tar = CheckNearestColor(tar,0,10,0); //try to not be damaged
					tarFinal = tar;
					MinDis = 0;
					FlightToTarget = 1;
				}
				else
					tar = TargetInf->Pos;
			}
			//Addition3
			if (ItemInf->Type == ITEM_PLANE && Distance(EmyEstimate->TarPos,TargetInf->Pos) < ESTIMATE_DIS_SAME)
			{
				uint16_t HurtLeft = TargetInf->TotalHurtLeft;//the hurt left when I got to Target
				uint16_t MyTimeToTarget = 10 * Distance(MyInf->Pos,TargetInf->Pos) / MyEstimate->MaxSpeed;
				if (MyTimeToTarget > EmyEstimate->TimeEstimate)
					HurtLeft -= EmyEstimate->TimeEstimate*4 + (MyTimeToTarget-EmyEstimate->TimeEstimate)*8;
				else
					HurtLeft -= MyTimeToTarget*4;
				if (HurtLeft < 160)
					tar = ItemInf->Pos;
			}
		}
		else//black
		{
			if (ItemInf->Type == ITEM_PLANE)//5
			{
				if (Distance(EmyEstimate->TarPos,ItemInf->Pos) < ESTIMATE_DIS_SAME
						&& (float)EmyEstimate->TimeEstimate * P_TIME_TO_FIGHT < 10 * Distance(MyInf->Pos,ItemInf->Pos) / (float)MyEstimate->MaxSpeed)//cannot get
				{
					tar = TargetInf->Pos;
				}
				else
					tar = ItemInf->Pos;
			}
			else if (AirPlaneInf->Control || EmyEstimate->ControlUAV)//7,8
			{
				if (AirPlaneInf->Control) //8
				{
					if (!ItemInf->Type)
						tar = TargetInf->Pos;
				}
				else //7
				{
					tar = TargetInf->Pos;
				}
			}
			else if (Distance(TargetInf->Pos,AirPlaneInf->Pos) < 80)//6
			{
				if (ItemInf->Type)
					tar = ItemInf->Pos;
			}
		}
	}
	//Addition1-2
	if (EmyInf->HP <= KILL_LIFE_LINE && EmyInf->HP < MyInf->HP)
	{
		if (TargetInf->Black == 0 && ItemInf->Type == ITEM_CHANGE //4.4
			&& (AirPlaneInf->Control || Distance(AirPlaneInf->Pos, EmyInf->Pos) < AIRPLANE_ATTACK_RANGE))
		{
			tar = ItemInf->Pos;
		}
		else if (TargetInf->Exist)
			tar = TargetInf->Pos;
	}
	
	//3
	if (ItemInf->Type == ITEM_LIFE)
	{
		if (Distance(EmyEstimate->TarPos,ItemInf->Pos) < ESTIMATE_DIS_SAME
			&& (float)EmyEstimate->TimeEstimate * P_TIME_TO_FIGHT < 10 * Distance(MyInf->Pos,ItemInf->Pos) / (float)MyEstimate->MaxSpeed
			&& EmyInf->HP > KILL_LIFE_LINE)
			;
		else
			tar = ItemInf->Pos;
	}

	//2
	if (ItemInf->Type == ITEM_CHANGE && AdditionInf->HugeHurt && TargetInf->Exist
		&& Distance(ItemInf->Pos,MyPos) < Distance(TargetInf->Pos,MyPos))
	{
		tar = ItemInf->Pos;
	}
	
	//Color, 12
	if (TargetInf->Exist && POS_EQUAL(tar, TargetInf->Pos) && !AdditionInf->HugeHurt)
	{
		if (EmyEstimate->Speed < 50 || Distance(EmyEstimate->TarPos, TargetInf->Pos) > ESTIMATE_DIS_SAME)
			Color_Set = TargetInf->Black?POINTS_Black:POINTS_White;
	}

	//13
	if (TargetInf->Exist && POS_EQUAL(tar, TargetInf->Pos))
	{
		if (Distance(TargetInf->Pos,EmyInf->Pos) < 40)
			MinDis = 0;
	}

	//1
	if (!POS_EQUAL(tar, MyTarget) || ColorSetNow != Color_Set || MyDisSetNow != MinDis)
	{
		MyDisSetNow = QueueNode.MinDis = MinDis;
		if (QueueNode.MinDis > 5)
			QueueNode.MinDis = 5;//Final target need to be near
		QueueNode.StopTime = 1;
		MyTarget = QueueNode.Target = tarFinal;
		EL_POINTS_InsertShadowStack(QueueNode);
		if (!POS_EQUAL(tar,tarFinal))
		{
			QueueNode.MinDis = 19;
			QueueNode.StopTime = 0;
			MyTarget = QueueNode.Target = tar;
			EL_POINTS_InsertShadowStack(QueueNode);
		}
		ColorSetNow = Color_Set;
		EL_POINTS_SetColor(Color_Set);
		if (Color_Set)
		{
			EL_POINTS_FinishShadowStack(0);
			while (Distance(QueueNode.Target,MyPos) > 80)//Maybe Slow
			{
				tar.x = ((uint16_t)(QueueNode.Target.x) + (uint16_t)(MyPos.x))/2;
				tar.y = ((uint16_t)(QueueNode.Target.y) + (uint16_t)(MyPos.y))/2;
				//1@90'@10ms,0.7@45'@6ms,0.5@30'@3ms
				QueueNode.Target = CheckNearestColorSlow(tar,Color_Set-1,0.7*Distance(tar,MyPos),10,5);
				QueueNode.StopTime = 0;
				QueueNode.MinDis = 0;//always trying
				EL_POINTS_InsertShadowStack(QueueNode);
			}
		}
		EL_POINTS_FinishShadowStack(1);
	}
}
//Final Easy
void Decision_MoveControl_FinalEasy(void)
{
	Point_t tar, MyPos;
	EL_POINTS_Color_t Color_Set = POINTS_None;
	QueueNode.MinDis = 19;
	MyPos = MyInf->Pos;
	tar.x = tar.y = 128;
	if (Distance(tar,MyPos) > 20)
	{
		QueueNode.Target = tar;
		QueueNode.StopTime = 1;
		EL_POINTS_InsertShadowStack(QueueNode);
		MyTarget = tar;
	}
	if (ItemInf->Type == ITEM_PLANE)
	{
		QueueNode.Target = ItemInf->Pos;
		QueueNode.StopTime = 0;
		EL_POINTS_InsertShadowStack(QueueNode);
		MyTarget = ItemInf->Pos;
	}
	if(TargetInf->Exist)
	{
		QueueNode.Target = TargetInf->Pos;
		QueueNode.StopTime = 50;//5 seconds(til the end of the target)
		EL_POINTS_InsertShadowStack(QueueNode);
		if (!AdditionInf->HugeHurt)
			Color_Set = TargetInf->Black?POINTS_Black:POINTS_White;
		MyTarget = TargetInf->Pos;
	}
	if (ItemInf->Type == ITEM_CHANGE
		&& (!TargetInf->Exist || Distance(ItemInf->Pos,MyPos) < Distance(TargetInf->Pos,MyPos)))
	{
		QueueNode.Target = ItemInf->Pos;
		QueueNode.StopTime = 0;
		EL_POINTS_InsertShadowStack(QueueNode);
		MyTarget = ItemInf->Pos;
	}
	if (ItemInf->Type == ITEM_LIFE)
	{
		QueueNode.Target = ItemInf->Pos;
		QueueNode.StopTime = 0;
		EL_POINTS_InsertShadowStack(QueueNode);
		Color_Set = POINTS_None;
		MyTarget = ItemInf->Pos;
	}
	EL_POINTS_SetColor(Color_Set);
	if (Color_Set)
	{
		EL_POINTS_FinishShadowStack(0);
		while (Distance(QueueNode.Target,MyPos) > 80)//Maybe Slow
		{
			tar.x = ((uint16_t)(QueueNode.Target.x) + (uint16_t)(MyPos.x))/2;
			tar.y = ((uint16_t)(QueueNode.Target.y) + (uint16_t)(MyPos.y))/2;
			//1@90'@10ms,0.7@45'@6ms,0.5@30'@3ms
			QueueNode.Target = CheckNearestColorSlow(tar,Color_Set-1,0.7*Distance(tar,MyPos),10,5);
			QueueNode.StopTime = 0;
			QueueNode.MinDis = 0;//always trying
			EL_POINTS_InsertShadowStack(QueueNode);
		}
	}
	EL_POINTS_FinishShadowStack(1);
}

//Rule:Life
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
		EL_POINTS_FinishShadowStack(0);//Run First
#ifndef COMP_BLACK
		WhitePos = ItemInf->Pos;
#endif
		while (Color_Set && Distance(QueueNode.Target,MyPos) > 80)//Maybe Slow
		{
			tar.x = ((uint16_t)(QueueNode.Target.x) + (uint16_t)(MyPos.x))/2;
			tar.y = ((uint16_t)(QueueNode.Target.y) + (uint16_t)(MyPos.y))/2;
			//1@90'@10ms,0.7@45'@6ms,0.5@30'@3ms
			QueueNode.Target = CheckNearestColorSlow(tar,Color_Set-1,0.7*Distance(tar,MyPos),10,5);
			QueueNode.StopTime = 0;
			QueueNode.MinDis = 0;//always trying
			//0.34@20'
			//QueueNode.MinDis = 0.34 * Distance(tar,MyPos);
			//QueueNode.MinDis = QueueNode.MinDis < 20 ? 20 : QueueNode.MinDis;
			EL_POINTS_InsertShadowStack(QueueNode);
		}
	}
#ifndef BACK_TO_CENTER
#ifndef COMP_BLACK
	else if (AdditionInf->HugeHurt || MyInf->OutOfBound)//make sure we're in the white
#else
	else if (AdditionInf->LittleHurt || MyInf->OutOfBound)//make sure we're in the black
#endif
#else
	else
#endif
	{
		QueueNode.MinDis = 4;
		tar = MyPos;
#ifdef BACK_TO_CENTER
		tar.x = tar.y = 128; 
#endif
#ifndef COMP_BLACK
		QueueNode.Target = CheckNearestColorExceptHereSlow(tar,Color_Set-1,20,5,5);
		if (POS_EQUAL(tar,QueueNode.Target))
		{
			QueueNode.Target = WhitePos;
		}
#else
		QueueNode.Target = CheckNearestColorExceptHereSlow(tar,Color_Set-1,180,10,5);
#endif
		QueueNode.StopTime = 1;
		EL_POINTS_InsertShadowStack(QueueNode);
	}
	EL_POINTS_SetColor(Color_Set);
	EL_POINTS_FinishShadowStack(1);//Will Stop if no nodes
}

void Decision_MoveControl(void)
{
#if defined(RULE_LIFE)
	if(CheckInfDiff())
		Decision_MoveControl_Second();
#elif defined(FINAL_EASY)
	if(CheckInfDiff())
		Decision_MoveControl_FinalEasy();
#else
	Decision_MoveControl_Final();
#endif
}

void Decision_FlightControl(void)
{
	Point_t tar;
	if(AirPlaneInf->Control)
	{
		if (TargetInf->Black) //attack
		{
			tar = EmyEstimate->TarPos;
			if (FlightToTarget)
				tar = TargetInf->Pos;
			else if (Distance(tar, ItemInf->Pos) <= ESTIMATE_DIS_SAME && ItemInf->Type == ITEM_CHANGE)
			{
				tar = MyTarget;
			}
			else if (Distance(MyInf->Pos, EmyInf->Pos) > AIRPLANE_ATTACK_RANGE)
			{
				tar = EmyInf->Pos;
				if ((Distance(MyInf->Pos, EmyEstimate->TarPos) > AIRPLANE_ATTACK_RANGE))
					tar = EmyEstimate->TarPos;
			}
			else
			{
				int16_t dx = (int16_t)EmyInf->Pos.x - (int16_t)MyInf->Pos.x;
				int16_t dy = (int16_t)EmyInf->Pos.y - (int16_t)MyInf->Pos.y;
				float scale = (float)(AIRPLANE_ATTACK_RANGE + 10) / sqrtf(dx*dx+dy*dy);
				dx = MyInf->Pos.x + dx*scale;
				dy = MyInf->Pos.y + dy*scale;
				tar.x = dx > 255 ? 255 : dx < 0 ? 0 : dx;
				tar.y = dy > 255 ? 255 : dy < 0 ? 0 : dy;
			}
		}
		else //heal
		{
			if (FlightToTarget)
				tar = TargetInf->Pos;
			else if (Distance(MyInf->Pos, EmyInf->Pos) > AIRPLANE_HEAL_RANGE)
				tar = MyInf->Pos;
			else if (Distance(MyInf->Pos, AirPlaneInf->Pos) <= AIRPLANE_HEAL_RANGE)
			{
				int16_t dx = (int16_t)MyInf->Pos.x - (int16_t)EmyInf->Pos.x;
				int16_t dy = (int16_t)MyInf->Pos.y - (int16_t)EmyInf->Pos.y;
				float scale = (float)(AIRPLANE_HEAL_RANGE - 10) / sqrtf(dx*dx+dy*dy);
				dx = MyInf->Pos.x + dx*scale;
				dy = MyInf->Pos.y + dy*scale;
				tar.x = dx > 255 ? 255 : dx < 0 ? 0 : dx;
				tar.y = dy > 255 ? 255 : dy < 0 ? 0 : dy;
			}
			else
				tar = MyInf->Pos;
		}
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
	MyEstimate = EL_INF_GetMyEstimate();
	EmyEstimate = EL_INF_GetEmyEstimate();

	EL_MUSIC_ChangeStatus(Music,7);
	EL_MUSIC_ChangeMode(Random);
	EL_MUSIC_SetPause(1);

	EL_POINTS_SetBorderSafetyDis(10);
}

void Decision_MakeDecision(void)
{
	if (GameInf->Status == GAME_START)
	{
#ifdef PLAYMUSIC_ATSTART
		EL_MUSIC_SetPause(0);
#else
		//Move
		Decision_MoveControl();
		//Flight
		Decision_FlightControl();
		//Music
		EL_MUSIC_SetPause(1);
#endif
	}//GameStart
#ifdef PLAYMUSIC
	else if (GameInf->Status == GAME_PAUSE)
		EL_MUSIC_SetPause(0);
#endif
}
