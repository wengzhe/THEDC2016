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
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO��ʼ�����ýṹ
	uint32_t T1=0;
	uint8_t k=0;
	Point_t tar = {0,0};
	EL_POINTS_Queue_t QueueNode={{0,0},19,0};
	//uint8_t TargetCnt = 0;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);//ʹ��GPIOD
	
	//����2������ʹ��λ�����ӣ�����GPIO_Pin_2|GPIO_Pin_3������ͬʱ��ʼ���������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�
	GPIO_Init(GPIOD, &GPIO_InitStructure);//�˳�ʼ��������GPIOD������PD2����ʼ����
	
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
			//��PD2���ó�k��ֵ������ʹ�õ����ͱ���Ӧ����BitAction��ʹ����ǿ��ת��
			k = !k;
			GPIO_WriteBit(GPIOD, GPIO_Pin_2, (BitAction)k);
		}
		if (EL_INF_GetGameInf()->Status == GAME_START)
		{
			EL_POINTS_SetColor(POINTS_None);
			EL_POINTS_ClearShadowQueue();
			if (EL_INF_GetItemInf()->Type)
			{
				QueueNode.Target = EL_INF_GetItemInf()->Pos;
				QueueNode.StopTime = 0;
				EL_POINTS_InsertShadowQueue(QueueNode);
			}
			if(EL_INF_GetTargetInf()->Exist)
			{
				if (Distance(EL_INF_GetTargetInf()->Pos,EL_INF_GetMyInf()->Pos) > 80)
				{
					tar.x = ((uint16_t)(EL_INF_GetTargetInf()->Pos.x) + (uint16_t)(EL_INF_GetMyInf()->Pos.x))/2;
					tar.y = ((uint16_t)(EL_INF_GetTargetInf()->Pos.y) + (uint16_t)(EL_INF_GetMyInf()->Pos.y))/2;
					//1@90'@10ms,0.7@45'@6ms,0.5@30'@3ms
					QueueNode.Target = CheckNearestColor(tar,EL_INF_GetTargetInf()->Black,0.7*Distance(tar,EL_INF_GetMyInf()->Pos));
					QueueNode.StopTime = 0;
					EL_POINTS_InsertShadowQueue(QueueNode);
				}
				QueueNode.Target = EL_INF_GetTargetInf()->Pos;
				QueueNode.StopTime = 1;
				EL_POINTS_InsertShadowQueue(QueueNode);
				EL_POINTS_SetColor(EL_INF_GetTargetInf()->Black?POINTS_Black:POINTS_White);
			}
			if(EL_INF_GetAirPlaneInf()->Control)
			{
				tar.x = 255-QueueNode.Target.x;
				tar.y = 255-QueueNode.Target.y;
				EL_POINTS_SetFlightPos(tar);
			}
			EL_POINTS_FinishShadowQueue();
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
		//��PD2���ó�k��ֵ������ʹ�õ����ͱ���Ӧ����BitAction��ʹ����ǿ��ת��
		k = !k;
		GPIO_WriteBit(GPIOD, GPIO_Pin_2, (BitAction)k);
	}
#endif
}
