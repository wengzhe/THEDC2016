#include <stm32f10x_conf.h>
#include "sysconfig.h"
#include "Exec.h"
#include "MUSIC.h"
#include "POINTS.h"
#include "INF.h"

__STATIC_INLINE float Distance(Point_t A, Point_t B)
{
	float dx = (float)A.x - (float)B.x, dy = (float)A.y - (float)B.y;
	return sqrtf(dx*dx+dy*dy);
}

int main()
{
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO初始化所用结构
	uint32_t T1=0;
	uint8_t k=0;
	Point_t tar = {0,0}, MyPos;
	EL_POINTS_Queue_t QueueNode={{0,0},19,0};
	EL_POINTS_Color_t Color_Set = POINTS_None;
	//uint8_t TargetCnt = 0;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);//使能GPIOD
	
	//引脚2，可以使用位或连接，例如GPIO_Pin_2|GPIO_Pin_3，可以同时初始化多个引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//输出，推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度
	GPIO_Init(GPIOD, &GPIO_InitStructure);//此初始化作用于GPIOD，于是PD2被初始化了
	
	System_Init();
	EL_Init();
	
	EL_MUSIC_ChangeStatus(Music,3);
	EL_MUSIC_ChangeMode(Once);
	EL_MUSIC_SetPause(1);

#if 1
	EL_POINTS_SetBorderSafetyDis(10);
	while(1)
	{
		if(T1+500<=TimeTicket)
		{
			T1+=500;
			//将PD2设置成k的值，这里使用的类型本来应该是BitAction，使用了强制转换
			k = !k;
			GPIO_WriteBit(GPIOD, GPIO_Pin_2, (BitAction)k);
		}
		if (EL_INF_GetGameInf()->Status == GAME_START)
		{
			//Move
			Color_Set = POINTS_None;
			if(EL_INF_GetTargetInf()->Exist)
			{
				QueueNode.Target = EL_INF_GetTargetInf()->Pos;
				QueueNode.StopTime = 1;
				EL_POINTS_InsertShadowStack(QueueNode);
				Color_Set = EL_INF_GetTargetInf()->Black?POINTS_Black:POINTS_White;
			}
			if (EL_INF_GetItemInf()->Type)
			{
				QueueNode.Target = EL_INF_GetItemInf()->Pos;
				QueueNode.StopTime = 0;
				EL_POINTS_InsertShadowStack(QueueNode);
			}
			MyPos = EL_INF_GetMyInf()->Pos;
			while (Color_Set && Distance(QueueNode.Target,MyPos) > 80) //There must be a node if color is set
			{
				tar.x = ((uint16_t)(QueueNode.Target.x) + (uint16_t)(MyPos.x))/2;
				tar.y = ((uint16_t)(QueueNode.Target.y) + (uint16_t)(MyPos.y))/2;
				//1@90'@10ms,0.7@45'@6ms,0.5@30'@3ms
				QueueNode.Target = CheckNearestColor(tar,Color_Set-1,0.7*Distance(tar,MyPos));
				QueueNode.StopTime = 0;
				EL_POINTS_InsertShadowStack(QueueNode);
			}
			EL_POINTS_SetColor(Color_Set);
			EL_POINTS_FinishShadowStack();
			//Flight
			if(EL_INF_GetAirPlaneInf()->Control)
			{
				tar.x = 255-QueueNode.Target.x;
				tar.y = 255-QueueNode.Target.y;
				EL_POINTS_SetFlightPos(tar);
			}
			//Music
			EL_MUSIC_SetPause(1);
		}//GameStart
		else if (EL_INF_GetGameInf()->Status == GAME_PAUSE)
			EL_MUSIC_SetPause(0);
	}
#else
	while(1)
	{
		while(T1+500>TimeTicket);
		T1+=500;
		//将PD2设置成k的值，这里使用的类型本来应该是BitAction，使用了强制转换
		k = !k;
		GPIO_WriteBit(GPIOD, GPIO_Pin_2, (BitAction)k);
	}
#endif
}
