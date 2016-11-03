#include "UART.h"

//PA9:TX PA10:RX

uint16_t RandomSeed=0;

#define RXBUFSIZE 234
uint8_t DataBuf[RXBUFSIZE] = {0x0a, 0};
uint8_t RxBufFront = 0, RxBufRear = 1;

#define RXDATASIZE 18

DL_UART_Data_t DL_UART_Data = {0};

#define RXFUNCSIZE 10
void (*RxFunctions[RXFUNCSIZE])(const DL_UART_Data_t*) = {0};
uint8_t RxFuncPointer = 0;

#define TXBUFSIZE 12
uint8_t TxBuf[TXBUFSIZE]={0};
uint8_t TxBufFront = 0, TxBufRear = 0, TxSending = 0;

void DL_UART_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_DeInit(USART1);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART_InitStructure);	
	
	USART_Cmd(USART1, ENABLE);

  //initialize interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//enable usart interrupt
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	USART_ClearFlag(USART1, USART_FLAG_RXNE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

uint16_t DL_UART_GetRandomSeed(void)
{
	return RandomSeed++;
}

uint8_t GetRxData(void)
{
  uint8_t ret = 0;
  while ((RxBufRear + RXBUFSIZE - RxBufFront) % RXBUFSIZE >= RXDATASIZE)
  {
    if (DataBuf[(RxBufFront) % RXBUFSIZE] == 0x0a && DataBuf[(RxBufFront + RXDATASIZE - 1) % RXBUFSIZE] == 0x0d
			&& (DataBuf[(RxBufFront+1) % RXBUFSIZE] & 0xF0) == 0xF0)
    {
      uint8_t i;
      uint8_t *Data = (uint8_t*)&DL_UART_Data;
      for (i = 1; i < RXDATASIZE-1; i++)
      {
        Data[i - 1] = DataBuf[(RxBufFront + i) % RXBUFSIZE];
      }
      RxBufFront = (RxBufFront + RXDATASIZE) % RXBUFSIZE;
      ret = 1;
    }
    else
      RxBufFront++;
  }
  return ret;
}

uint8_t UART_SoftLock = 0;
void DL_UART_SetTxData(uint8_t *data, uint8_t length)
{
	UART_SoftLock = 1;
	for (;length ; length--)
	{
		TxBuf[TxBufRear++]=*(data++);
		TxBufRear%=TXBUFSIZE;
	}
	UART_SoftLock = 0;
	if (!TxSending)
	{
		USART_SendData(USART1, TxBuf[TxBufFront++]);
		TxBufFront%=TXBUFSIZE;
		TxSending=1;
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}
}

uint8_t DL_UART_SetRxFunc(void (*f)(const DL_UART_Data_t*))
{
	if (RxFuncPointer < RXFUNCSIZE)
	{
		RxFunctions[RxFuncPointer++]=f;
		return 0;
	}
	else
		return 1;
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		if (RxBufRear >= RXBUFSIZE)
			RxBufRear = 0;
		DataBuf[RxBufRear++] = USART_ReceiveData(USART1);
		if (GetRxData())
		{
			uint8_t i;
			for (i=0; i<RxFuncPointer; i++)
			{
				RxFunctions[i](&DL_UART_Data);
			}
		}
	}
	if(USART_GetITStatus(USART1, USART_IT_TXE))
	{
		if (TxBufFront != TxBufRear && !UART_SoftLock)
		{
			USART_SendData(USART1, TxBuf[TxBufFront++]);
			TxBufFront%=TXBUFSIZE;
		}
		else
		{
			TxSending = 0;
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}
	}
	RandomSeed++;
}
