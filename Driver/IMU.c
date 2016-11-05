#include "IMU.h"
#include "MPU6050.h"

//L+ R-

#define IIC_PORT     GPIOC
#define IIC_SDA_PIN  GPIO_Pin_3
#define IIC_SCL_PIN  GPIO_Pin_2
#define VCC1_PIN     GPIO_Pin_0
#define VCC2_PIN     GPIO_Pin_1
#define VCC1_ON      GPIO_SetBits(IIC_PORT, VCC1_PIN)
#define VCC1_OFF     GPIO_ResetBits(IIC_PORT, VCC1_PIN)
#define VCC2_ON      GPIO_SetBits(IIC_PORT, VCC2_PIN)
#define VCC2_OFF     GPIO_ResetBits(IIC_PORT, VCC2_PIN)

uint16_t MPUOFF[2] = {300,300};
DL_IMU_State_t IMU_State[2] = {IMU_OFF,IMU_OFF};
uint8_t IIC_Failure[2] = {0,0}; //failure cnt
float DL_IMU_Gyro_Data[2] = {0,0};
float DL_IMU_Gyro = 0;//+-500/65536

#define IMU_CHECK_NUM_MAX 1000
uint16_t IMU_Check_Cnt[2] = {0,0};
float IMU_Check[2] = {0,0};

void PowerMPU(void)
{
	if (MPUOFF[0])
	{
		MPUOFF[0]--;
		VCC1_OFF;
	}
	else
	{
		VCC1_ON;
		if (IMU_State[0] == IMU_OFF)
			IMU_State[0] = IMU_INIT;
	}
	if (MPUOFF[1])
	{
		MPUOFF[1]--;
		VCC2_OFF;
	}
	else
	{
		VCC2_ON;
		if (IMU_State[1] == IMU_OFF)
			IMU_State[1] = IMU_INIT;
	}
}

void ResetMPU(uint8_t ID)
{
	MPUOFF[ID] = 300;
	IMU_State[ID] = IMU_OFF;
	IIC_Failure[ID] = 0;
}

void DL_IMU_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//GPIO Initialize
	GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN | IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(IIC_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = VCC1_PIN | VCC2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(IIC_PORT, &GPIO_InitStructure);
	
	ResetMPU(0);
	ResetMPU(1);
	//DL_IMU_StartCheck();
}

void delay5us(void)//5us
{
	uint8_t time;
	for(time=5; time > 0; time--);
}

void iicstart(void)
{
	GPIO_SetBits(IIC_PORT,IIC_SDA_PIN); //sda=1;
	GPIO_SetBits(IIC_PORT,IIC_SCL_PIN); //scl=1;
	delay5us();
	GPIO_ResetBits(IIC_PORT,IIC_SDA_PIN); //sda=0;
}

void iicstop(void)
{
	GPIO_ResetBits(IIC_PORT,IIC_SDA_PIN); //sda=0;
	delay5us();
	GPIO_SetBits(IIC_PORT,IIC_SCL_PIN); //scl=1;
	GPIO_SetBits(IIC_PORT,IIC_SDA_PIN); //sda=1;
}

uint8_t nack(uint8_t needret)
{
	uint8_t i = needret ? 0:11;
	GPIO_SetBits(IIC_PORT,IIC_SCL_PIN);//scl=1;
	//delay5us();
	while(GPIO_ReadInputDataBit(IIC_PORT,IIC_SDA_PIN)&&(i<10)) {i++;}
	GPIO_ResetBits(IIC_PORT,IIC_SCL_PIN);//scl=0;
	delay5us();
	if (i==10)
		return 1;
	return 0;
}

void ack(void)
{
	GPIO_ResetBits(IIC_PORT,IIC_SDA_PIN);//sda=1;
	GPIO_SetBits(IIC_PORT,IIC_SCL_PIN);//scl=1;
	delay5us();
	GPIO_ResetBits(IIC_PORT,IIC_SCL_PIN);//scl=0;
	GPIO_SetBits(IIC_PORT,IIC_SDA_PIN);//sda=0;
	delay5us();
}

void iicwritebyte(uint8_t a)
{
	uint8_t i;
	GPIO_ResetBits(IIC_PORT,IIC_SCL_PIN); //scl=0;
	delay5us();
	for(i=0;i<8;i++)
	{
		if(a&0x80)
			GPIO_SetBits(IIC_PORT,IIC_SDA_PIN);//sda=1;
		else
			GPIO_ResetBits(IIC_PORT,IIC_SDA_PIN);
		a<<=1;
		delay5us();
		GPIO_SetBits(IIC_PORT,IIC_SCL_PIN);//scl=1;
		delay5us();
		GPIO_ResetBits(IIC_PORT,IIC_SCL_PIN);//scl=0;
	}
	GPIO_SetBits(IIC_PORT,IIC_SDA_PIN);//sda=1;
	delay5us();
}

uint8_t iicreadbyte()
{
	uint8_t i,temp;
	temp=0;
	GPIO_SetBits(IIC_PORT,IIC_SDA_PIN);//sda=1;
	GPIO_ResetBits(IIC_PORT,IIC_SCL_PIN); //scl=0;
	for(i=0;i<8;i++)
	{
		delay5us();
		GPIO_SetBits(IIC_PORT,IIC_SCL_PIN); //scl=1;
		delay5us();
		temp=(temp<<1)|GPIO_ReadInputDataBit(IIC_PORT,IIC_SDA_PIN);
		GPIO_ResetBits(IIC_PORT,IIC_SCL_PIN); //scl=0;
	}
	GPIO_SetBits(IIC_PORT,IIC_SDA_PIN);//sda=1;
	delay5us();
	return temp;
}

uint8_t W_6050(uint8_t REG_add,uint8_t REG_data, uint8_t ID)
{
	uint8_t ret = 0;
	iicstart();

	iicwritebyte(0xD0|ID<<1);
	ret += nack(1);
	
	iicwritebyte(REG_add);
	ret += nack(1);
	iicwritebyte(REG_data);
	ret += nack(1);
	
	iicstop();
	delay5us();
	return ret;
}

uint8_t R_6050(uint8_t REG_add, uint8_t ID, uint8_t *data)
{
	uint8_t ret = 0;
	iicstart();
	
	iicwritebyte(0xD0|ID<<1);
	ret += nack(1);
	iicwritebyte(REG_add);
	ret += nack(1);

  iicstart();
	
	iicwritebyte(0xD1|ID<<1);
	ret += nack(1);
	*data=iicreadbyte();
	nack(0);
	iicstop();
	
  return ret;
}

uint8_t R_6050_2(uint8_t REG_add, uint8_t ID, int16_t *data)
{
	uint8_t ret = 0;
  *data = 0;
	iicstart();
	
	iicwritebyte(0xD0|ID<<1);
	ret+=nack(1);
	iicwritebyte(REG_add);
	ret+=nack(1);

  iicstart();
	
	iicwritebyte(0xD1|ID<<1);
	ret+=nack(1);
	*data+=iicreadbyte() << 8;
	ack();
	*data+=iicreadbyte();
	nack(0);
	iicstop();
	
  return ret;
}

void InitMPU6050(uint32_t TimeTicket, uint8_t ID)
{
	static uint8_t step[2] = {0,0};
	static uint32_t time[2] = {0,0};
	if (IMU_State[ID] == IMU_INIT)
	{
		if (step[ID] == 0)
		{
			step[ID]++;
			time[ID] = TimeTicket+100;
		}
		else if (step[ID] == 1 && time[ID] <= TimeTicket)
		{
			if (W_6050(PWR_MGMT_1, 0x80, ID))
			{
				ResetMPU(ID);
				step[ID] = 0;
			}
			else
			{
				step[ID]++;
				time[ID] = TimeTicket+100;
			}
		}
		else if (step[ID] == 2 && time[ID] <= TimeTicket)
		{
			uint8_t temp = 0;
			temp += W_6050(PWR_MGMT_1, 0x00, ID);	
			temp += W_6050(SMPLRT_DIV, 0x00, ID);//1kHz
			temp += W_6050(CONFIG, 0x06, ID);//6,5Hz,18.6ms;3,44Hz,4.9ms
			temp += W_6050(GYRO_CONFIG, 0x10, ID);//1000degree/s
			temp += W_6050(ACCEL_CONFIG, 0x01, ID);
			if (!temp && !R_6050(WHO_AM_I, ID, &temp) && (temp & 0x7E) == 0x68)
			{
				IMU_State[ID] = IMU_WORK;
				if (!IMU_Check[ID])
					DL_IMU_StartCheck(ID);
			}
			else
			{
				ResetMPU(ID);
			}
			step[ID] = 0;
		}
	}
}

#define abs(x) ((x)>0?(x):(-x))
#define mabs(x,y) ((x)>(y)?(x-y):(y-x))
void DL_IMU_Work(uint8_t IIC_Success[2], int16_t Data[2])
{
	uint8_t i;
	for (i = 0; i < 2; i++)
	{
		if (IIC_Success[i])
		{
			DL_IMU_Gyro_Data[i] = Data[i];
			if (IMU_State[i] == IMU_WORK)
			{
				DL_IMU_Gyro_Data[i] -= IMU_Check[i];
			}
			else
			{
				if (IMU_Check_Cnt[i] < IMU_CHECK_NUM_MAX && abs(Data[i]) < 150)
				{
					IMU_Check_Cnt[i]++;
					IMU_Check[i] += Data[i];
				}
				if (IMU_Check_Cnt[i] >= IMU_CHECK_NUM_MAX)
				{
					float TMP = IMU_Check[i]/IMU_Check_Cnt[i];
					if (mabs(TMP,Data[i]) < 2)
					{
						IMU_Check[i] = TMP;
						IMU_State[i] = IMU_WORK;
					}
					else
						IMU_Check[i] = 0;
					IMU_Check_Cnt[i] = 0;
				}
			}
		}
	}
	if (IIC_Success[0] && IIC_Success[1])
		DL_IMU_Gyro = DL_IMU_Gyro_Data[0] + DL_IMU_Gyro_Data[1];
	else if (IIC_Success[0] || IIC_Success[1])
		DL_IMU_Gyro = 2*(IIC_Success[0]?DL_IMU_Gyro_Data[0]:DL_IMU_Gyro_Data[1]);
}

void DL_IMU_StartCheck(uint8_t ID)
{
	IMU_Check[ID] = 0;
	IMU_Check_Cnt[ID] = 0;
	if (IMU_State[ID] == IMU_WORK)
		IMU_State[ID] = IMU_TEST;
}

float DL_IMU_GetGyro(void)
{
	return DL_IMU_Gyro/65536;
}

void DL_IMU_Tick(uint32_t TimeTicket)
{
	int16_t Data[2];
	uint8_t IIC_Success[2]={0,0};
	PowerMPU();
	InitMPU6050(TimeTicket, 0);
	InitMPU6050(TimeTicket, 1);
	if (IMU_State[0] == IMU_WORK || IMU_State[0] == IMU_TEST)
	{
		if (!R_6050_2(GYRO_ZOUT_H, 0, Data))
			IIC_Success[0]=1;
		if (!IIC_Success[0])
			IIC_Failure[0]++;
		else
			IIC_Failure[0] = 0;
		if (IIC_Failure[0] > 80)
			ResetMPU(0);
	}
	if (IMU_State[1] == IMU_WORK || IMU_State[1] == IMU_TEST)
	{
		if (!R_6050_2(GYRO_ZOUT_H, 1, &Data[1]))
			IIC_Success[1]=1;
		if (!IIC_Success[1])
			IIC_Failure[1]++;
		else
			IIC_Failure[1] = 0;
		if (IIC_Failure[1] > 80)
			ResetMPU(1);
	}
	DL_IMU_Work(IIC_Success, Data);
}
