#include "AllDefs.h"

#include "MAP.h"
#include "UART.h"
#include "IR.h"
#include "Angle.h"

#include <math.h>

const float AngleIncrement[IR_NUM] = {80,40,0,-40,-80,-150,150,180};
void CL_MAP_GetPos(const DL_UART_Data_t*);

#define DataBitNum(x,y) (((x)&0x02)+(((y)>>1)&0x01))
#define DataBitNum3(x,y,Color) (((x)&0x02)+(((y)>>1)&0x01)+((uint8_t)(Color) << 2))
uint8_t CL_MAP_Data[64][64];//H:Black,L:White, (x&0x02)+((y>>1)&0x01)

CL_MAP_IR_t CL_MAP_IR={{0},{MAP_NotSure,MAP_NotSure,MAP_NotSure,MAP_NotSure,MAP_NotSure,MAP_NotSure,MAP_NotSure,MAP_NotSure}};

#define abs(x) ((x)>0?(x):-(x))
#ifdef NORMAL_RULE
__STATIC_INLINE float Distance(Point_t A, Point_t B)
{
	float dx = (float)A.x - (float)B.x, dy = (float)A.y - (float)B.y;
	return sqrtf(dx*dx+dy*dy);
}
#endif

__STATIC_INLINE float AddDegree180(float A, float B)
{
	float input = A + B;
	if (input > 180)
		input -= 360;
	if (input < -180)
		input += 360;
	return input;
}

void CL_MAP_Init(void)
{
	uint8_t i,j;
	DL_UART_SetRxFunc(CL_MAP_GetPos);
	for (i=0;i<64;i++)
		for (j=0;j<64;j++)
#ifdef MAP_NULL
			CL_MAP_Data[i][j]=0;
#else
			CL_MAP_Data[i][j]=0xff;
#endif
}

/*typedef int (*PARR)[4];
PARR search(PARR arr)
{
  return arr;
}*/

uint8_t (*CL_MAP_GetMap(void))[64]
{
	return CL_MAP_Data;
}

void CL_MAP_SetColor(uint8_t x, uint8_t y, CL_MAP_Color_t Color)
{
	uint8_t pos = 1 << DataBitNum(x,y);
	CL_MAP_Data[x>>2][y>>2] &= ~(pos | (pos << 4));
	CL_MAP_Data[x>>2][y>>2] |= pos << ((uint8_t)Color << 2);
}

uint8_t CL_MAP_CheckColor(uint8_t x, uint8_t y, CL_MAP_Color_t Color)
{
	return (CL_MAP_Data[x>>2][y>>2] >> DataBitNum3(x,y,Color)) & 0x01;
}

CL_MAP_Color_t CL_MAP_GetColor(uint8_t x, uint8_t y)
{
	uint8_t white = (CL_MAP_Data[x>>2][y>>2] >> DataBitNum(x,y)) & 0x01;
	uint8_t black = (CL_MAP_Data[x>>2][y>>2] >> (4 + DataBitNum(x,y))) & 0x01;
	if (white && black)
		return MAP_NotSure;
	if (white)
		return MAP_White;
	return MAP_Black;
}

void CL_MAP_GetPos(const DL_UART_Data_t* DataIN)
{
	//FromIR
	if (CL_ANGLE_DegreeAbsOK())
	{
		uint8_t i;
		for (i=0; i<IR_NUM; i++)
		{
			float Angle = (CL_MAP_IR.Angle[i])/180*M_PI;
			int16_t x = DataIN->MyPos.x + IR_DIS*sinf(Angle);
			int16_t y = DataIN->MyPos.y + IR_DIS*cosf(Angle);
			//x = x > 255 ? 255 : x < 0 ? 0 : x;
			//y = y > 255 ? 255 : y < 0 ? 0 : y;
			if (x >= 0 && x <= 255 && y >= 0 && y <= 255)
				CL_MAP_SetColor(x,y,CL_MAP_IR.Color[i]);
		}
	}
	//FromUART
#ifdef NORMAL_RULE
	if (DataIN->Flags & 0x02) //Target Exists
	{
		CL_MAP_Color_t TarColor = (CL_MAP_Color_t)(DataIN->Flags & 0x01);
		CL_MAP_SetColor(DataIN->TarPos.x, DataIN->TarPos.y, TarColor);
		
		if (!(DataIN->Flags & 0xc0)) //Not be damaged
		{
			CL_MAP_SetColor(DataIN->MyPos.x, DataIN->MyPos.y, TarColor);
			if (Distance(DataIN->MyPos, DataIN->TarPos) > Distance(DataIN->EmyPos, DataIN->TarPos))
			{
				CL_MAP_SetColor(DataIN->EmyPos.x, DataIN->EmyPos.y, (CL_MAP_Color_t)(1-(uint8_t)TarColor));
			}
		}
		else if (DataIN->Flags & 0x80) //Be damaged by Target
		{
			if (Distance(DataIN->MyPos, DataIN->TarPos) < Distance(DataIN->EmyPos, DataIN->TarPos))
			{
				CL_MAP_SetColor(DataIN->MyPos.x, DataIN->MyPos.y, (CL_MAP_Color_t)(1-(uint8_t)TarColor));
			}
		}
	}
#endif
}

const CL_MAP_IR_t *CL_MAP_GetIR(void)
{
	return &CL_MAP_IR;
}

void CL_MAP_Tick(void)
{
	float HeadAngle=CL_ANGLE_GetDegreeAbs();
	uint8_t i, IR_Data=DL_IR_GetStatus();
	for (i=0; i<IR_NUM; i++)
	{
		CL_MAP_IR.Angle[i] = AddDegree180(HeadAngle,AngleIncrement[i]);
		CL_MAP_IR.Color[i] = (CL_MAP_Color_t)((IR_Data>>i)&0x01);
	}
}
