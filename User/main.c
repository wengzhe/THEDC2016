//#include "AllDefs.h"

#include <stm32f10x_conf.h>
#include "sysconfig.h"
#include "Decision.h"
#include "music.h"

//uint32_t T2,T3;

int main()
{
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO初始化所用结构
	uint32_t T1=0;
	uint8_t k=0;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);//使能GPIOD
	
	//引脚2，可以使用位或连接，例如GPIO_Pin_2|GPIO_Pin_3，可以同时初始化多个引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//输出，推挽
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度
	GPIO_Init(GPIOD, &GPIO_InitStructure);//此初始化作用于GPIOD，于是PD2被初始化了
	
	System_Init();
	Decision_Init();

	/*EL_MUSIC_ChangeStatus(Music,6);
	EL_MUSIC_SetPause(0);
	EL_MUSIC_ChangeMode(Single);
	EL_MUSIC_ChangeMode(Once);*/
	EL_MUSIC_ChangeMode(Order);
	while(1)
	{
		if(T1+500<=TimeTicket)
		{
			T1+=500;
			//将PD2设置成k的值，这里使用的类型本来应该是BitAction，使用了强制转换
			k = !k;
			GPIO_WriteBit(GPIOD, GPIO_Pin_2, (BitAction)k);
		}
		Decision_MakeDecision();
	}
}
