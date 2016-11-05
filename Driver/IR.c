#include "IR.h"

#define IR_GPIO_PIN GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8
#define IR_GPIO_PORT GPIOA
#define IR_GPIO_CLOCK RCC_APB2Periph_GPIOA
#define IR_GPIO_MIN_BIT 1
#define IR_GPIO_MASK 0xFF

//PA4~PA8
void DL_IR_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(IR_GPIO_CLOCK,ENABLE);

	GPIO_InitStructure.GPIO_Pin = IR_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(IR_GPIO_PORT, &GPIO_InitStructure);
}

uint8_t DL_IR_GetStatus(void)
{
	return (GPIO_ReadInputData(IR_GPIO_PORT) >> IR_GPIO_MIN_BIT) & IR_GPIO_MASK;
}
