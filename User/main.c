//#include "AllDefs.h"

#include <stm32f10x_conf.h>
#include "sysconfig.h"
#include "Decision.h"
#include "music.h"

//uint32_t T2,T3;

int main()
{
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO��ʼ�����ýṹ
	uint32_t T1=0;
	uint8_t k=0;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);//ʹ��GPIOD
	
	//����2������ʹ��λ�����ӣ�����GPIO_Pin_2|GPIO_Pin_3������ͬʱ��ʼ���������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�
	GPIO_Init(GPIOD, &GPIO_InitStructure);//�˳�ʼ��������GPIOD������PD2����ʼ����
	
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
			//��PD2���ó�k��ֵ������ʹ�õ����ͱ���Ӧ����BitAction��ʹ����ǿ��ת��
			k = !k;
			GPIO_WriteBit(GPIOD, GPIO_Pin_2, (BitAction)k);
		}
		Decision_MakeDecision();
	}
}
