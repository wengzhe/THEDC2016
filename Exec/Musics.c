//#include "AllDefs.h"

#include "TONE.h"
#include "Musics.h"
#include "Music.h"

#define TTLS_LENGTH 24
const uint8_t TTLS[TTLS_LENGTH][2]={{1,1},{5,1},{6,1},{5,1},{4,1},{3,1},{2,1},{1,1},{5,1},{4,1},{3,1},{2,1},{5,1},{4,1},{3,1},{2,1},{1,1},{5,1},{6,1},{5,1},{4,1},{3,1},{2,1},{1,1}};
	
uint8_t MusicFunction_Twinkle_Twinkle_Little_Star(uint32_t time, uint32_t start_time)
{
	uint32_t t = (time - start_time)/1000;
	if (t >= TTLS_LENGTH)
		return 1;
	else
		CL_TONE_SetTone(TTLS[t][0],TTLS[t][1],0);
	return 0;
}

#define THU_LENGTH 192
const uint8_t THU[THU_LENGTH][3]={
{1,2,0},{1,2,0},{1,2,0},{3,2,0},{5,2,0},{5,2,0},{5,2,0},{5,2,0},
{6,2,0},{6,2,0},{1,3,0},{6,2,0},{5,2,0},{5,2,0},{5,2,0},{5,2,0},
{3,2,0},{3,2,0},{3,2,0},{3,2,0},{5,2,0},{3,2,0},{1,2,0},{1,2,0},
{6,1,0},{6,1,0},{1,2,0},{3,2,0},{5,2,0},{5,2,0},{5,2,0},{5,2,0},

{6,2,0},{6,2,0},{6,2,0},{6,2,0},{6,2,0},{1,3,0},{5,2,0},{5,2,0},
{3,2,0},{3,2,0},{2,2,0},{2,2,0},{3,2,0},{2,2,0},{1,2,0},{1,2,0},
{2,2,0},{2,2,0},{5,2,0},{5,2,0},{5,2,0},{5,2,1},{5,2,0},{5,2,0},
{6,2,0},{6,2,0},{6,2,0},{6,2,0},{7,2,0},{6,2,0},{5,2,0},{5,2,0},

{1,3,0},{1,3,0},{1,3,0},{1,3,0},{6,2,0},{6,2,0},{1,3,0},{1,3,0},
{5,2,0},{5,2,0},{5,2,0},{6,2,0},{5,2,0},{5,2,0},{5,2,0},{5,2,0},
{6,2,0},{6,2,0},{6,2,0},{6,2,0},{5,2,0},{5,2,0},{3,2,0},{3,2,0},
{2,2,0},{2,2,0},{2,2,0},{3,2,0},{5,2,0},{5,2,0},{5,2,0},{5,2,0},

{1,2,0},{1,2,0},{1,2,0},{1,2,0},{1,2,0},{1,2,0},{3,2,0},{3,2,0},
{2,2,0},{2,2,0},{3,2,0},{2,2,0},{1,2,0},{1,2,0},{1,2,0},{1,2,0},
{6,2,0},{6,2,0},{6,2,0},{6,2,0},{5,2,0},{5,2,0},{3,2,0},{3,2,0},
{2,2,0},{2,2,0},{3,2,0},{2,2,0},{1,2,0},{1,2,0},{1,2,0},{1,2,0},

{1,3,0},{1,3,0},{1,3,0},{1,3,0},{1,3,0},{1,3,0},{0,0,0},{0,0,0},
{6,2,0},{6,2,0},{6,2,0},{6,2,0},{6,2,0},{6,2,0},{0,0,0},{0,0,0},
{5,2,0},{5,2,0},{5,2,0},{5,2,0},{6,2,0},{6,2,0},{5,2,0},{5,2,0},
{2,2,0},{2,2,0},{3,2,0},{3,2,0},{5,2,0},{5,2,0},{5,2,0},{5,2,0},

{1,3,0},{1,3,0},{1,3,0},{1,3,0},{1,3,0},{1,3,0},{0,0,0},{0,0,0},
{6,2,0},{6,2,0},{6,2,0},{6,2,0},{6,2,0},{6,2,0},{0,0,0},{0,0,0},
{5,2,0},{5,2,0},{5,2,0},{5,2,0},{6,2,0},{6,2,0},{5,2,0},{5,2,0},
{2,2,0},{2,2,0},{3,2,0},{2,2,0},{1,2,0},{1,2,0},{1,2,0},{1,2,0}
};

uint8_t MusicFunction_Tsinghua(uint32_t time, uint32_t start_time)
{
	uint32_t t = (time - start_time)/500;
	if (t >= THU_LENGTH)
		return 1;
	else
		CL_TONE_SetTone(THU[t][0],THU[t][1],THU[t][2]);
	return 0;
}

#define MHH_LENGTH 230
const uint8_t MHH[MHH_LENGTH][4]={ //{tone,high,time,connect=0}
{6,0,3},{3,1,3},{6,0,3},{5,1,3},{6,0,4},
{1,1,3},{5,1,3},{1,1,2},{2,1,2},{6,1,2},{2,1,2},{3,1,2},
{6,0,3},{3,1,3},{6,0,3},{5,1,3},{6,0,4},
{5,0,3},{1,1,3},{5,0,4},{2,1,2},{3,1,2},{2,1,2},
{6,0,3},{3,1,3},{6,0,3},{5,1,3},{6,0,4},
{1,1,3},{5,1,3},{1,1,2},{2,1,2},{6,1,2},{2,1,2},{3,1,2},
{6,0,3},{3,1,3},{5,1,4},{3,1,2},{3,1,4},
{7,0,3},{2,1,3},{5,0,3},{7,1,3},{0,0,2},{6,0,1},{6,0,1},
//start
{3,1,1},{3,1,1},{3,1,1},{3,1,2},{3,1,2},{2,1,2},{1,1,1},{3,1,5},{1,1,1},
{5,1,1},{5,1,2},{6,1,2},{5,1,2},{3,1,1},{2,1,1},{1,1,2},{3,1,2},{2,1,2},{1,1,1},
{6,1,3},{5,1,3},{1,1,10},
{0,0,2},{6,0,2},{1,1,2},{6,0,1},{2,1,2},{3,1,1,1},{2,1,5,1},{6,0,1},
{3,1,1},{3,1,1},{3,1,1},{3,1,2},{3,1,2},{1,1,1},{2,1,2},{1,1,1},{3,1,4},{1,1,1},
{2,1,1},{1,1,2},{6,1,2},{5,1,2},{3,1,1},{2,1,1},{1,1,2},{3,1,2},{2,1,2},{1,1,1},
{6,1,3},{5,1,3},{1,1,10},
{0,0,3},{1,1,1},{2,1,2},{1,1,1},{5,1,5},{6,1,2},{5,1,2},
{6,1,1},{6,1,1},{5,1,1},{6,1,2},{5,1,1},{6,1,1},{5,1,1},{6,1,1},{6,1,1},{5,1,1},{6,1,2},{1,2,2},{5,1,1},
{7,1,2},{7,1,1},{5,1,1},{7,1,1},{5,1,3},{2,2,2},{1,2,1},{7,1,2},{1,2,2},{6,1,1},
{1,2,2},{1,2,1},{6,1,1},{1,2,2},{1,2,1},{1,2,2},{1,2,1},{1,2,1},{1,2,1},{1,2,1},{6,1,2},{2,2,1},//fork
{2,2,1,1},{3,2,1,1},{2,2,5,1},{1,2,1,1},{7,1,4,1},{5,2,2},{1,2,2,1},
//join
{1,2,4},{2,2,2,1},{1,2,4,1},{6,1,2},{1,2,1},{6,1,2},{3,2,2},//one more
{2,2,1},{1,2,1,1},{1,2,3},{6,1,2},{7,1,3},{5,1,2},{2,2,3},
{1,2,4},{2,2,2,1},{1,2,4,1},{1,2,2},{1,2,1},{6,1,1,1},{6,1,1},{1,2,3},//two more
{7,1,2},{1,2,1},{5,1,1,1},{5,1,1},{1,2,1},{5,1,1,1},{5,1,2},{1,2,2},{2,2,3},
//fork
{0,0,2},{1,2,1},{1,2,1},{7,1,2},{1,2,1},{1,2,1},{5,1,2},{1,2,1},{7,1,2},{1,2,2},{1,2,2},//one more
{6,1,1},{1,2,1},{6,1,1},{1,2,2},{1,2,1},{6,1,1},{3,2,2},{2,2,1},{1,2,2},{2,2,3},
{1,2,1},{7,1,1},{1,2,1},{7,1,1},{1,2,2},{1,2,1},{6,1,1},{5,2,2},{3,2,1},{1,2,2},{6,1,2},{6,1,1},
//ending
{5,2,2},{3,2,1},{1,2,2},{1,2,1},{1,2,1},{6,1,1},{7,1,2},{1,2,1},{2,2,2},{1,2,2},{6,1,1},
{3,2,2},{3,2,1},{2,2,2},{1,2,5},{0,0,4},
{0,0,128}
};

#define MHH_INTERVAL 179 //84��

uint8_t MusicFunction_MHH(uint32_t time, uint32_t start_time)
{
	uint32_t t = (time - start_time),i;
	for (i=0;i<MHH_LENGTH;i++)
	{
		if (t<MHH[i][2]*MHH_INTERVAL)
		{
			if (t<20)
			{
				if(!MHH[i][3])
					CL_TONE_SetTone(0,0,0);
				else
					CL_TONE_DoubleTone(MHH[i-1][0],MHH[i-1][1]+1,0,MHH[i][0],MHH[i][1]+1,0,t/20.);
			}
			else
				CL_TONE_SetTone(MHH[i][0],MHH[i][1]+1,0);
			return 0;
		}
		t-=MHH[i][2]*MHH_INTERVAL;
	}
	return 1;
}

//Next Song, Use Tone Alone to Be Easily Understood
