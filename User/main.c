#include <stm32f10x_conf.h>
#include "sysconfig.h"
#include "Exec.h"
#include "MUSIC.h"
#include "POINTS.h"
#include "COM.h"

#include "SPEED.h"


extern CL_COM_Data_t CL_COM_Data;
/*void TestFunc(DL_UART_Data_t data)
{
	uint8_t test[9],i;
	for (i=0;i<8;i++)
		test[i] = ((uint8_t*)&data)[2*i] + ((uint8_t*)&data)[2*i+1];
	test[8]=EL_Rand();
	DL_UART_SetTxData(test,9);
}*/

int main()
{
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO初始化所用结构
	uint32_t T1=0;
	unsigned int k=0;
	Point_t tar = {0,0};
	EL_POINTS_Queue_t QueueNode={{0,0},19,0};
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
	//CL_SPEED_SetSpeed(30,30);
	//DL_UART_SetRxFunc(TestFunc);
	//EL_POINTS_SetDirectMinDistance(20);//dis=20
	//EL_POINTS_SetDirectMinTime(100);//100ms
#if 1
	EL_POINTS_SetColor(POINTS_White);
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
		if (CL_COM_Data.GameStatus == GAME_START)
		{
			EL_POINTS_ClearQueue();
			if(CL_COM_Data.ItemType == ITEM_PLANE)
			{
				QueueNode.Target = CL_COM_Data.ItemPos;
				QueueNode.StopTime = 0;
				EL_POINTS_InsertQueue(QueueNode);
			}
			if(CL_COM_Data.Flags.BITS.TargetExist)
			{
				QueueNode.Target = CL_COM_Data.TarPos;
				QueueNode.StopTime = 1;
				EL_POINTS_InsertQueue(QueueNode);
			}
			if (CL_COM_Data.ItemType)
			{
				QueueNode.Target = CL_COM_Data.ItemPos;
				QueueNode.StopTime = 0;
				EL_POINTS_InsertQueue(QueueNode);
			}
			if(CL_COM_Data.Flags.BITS.ControlPlane) //need some pause
			{
				tar.x = 255-QueueNode.Target.x;
				tar.y = 255-QueueNode.Target.y;
				CL_COM_SendPos(tar);
			}
			EL_MUSIC_SetPause(1);
		}
		else if (CL_COM_Data.GameStatus == GAME_PAUSE)
			EL_MUSIC_SetPause(0);
		Delay(1);
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
