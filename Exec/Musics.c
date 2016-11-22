//#include "AllDefs.h"

#include "TONE.h"
#include "Musics.h"
#include "Music.h"

//#define TTLS_LENGTH 30
//const uint8_t TTLS[TTLS_LENGTH][2]={{1,1},{5,1},{6,1},{5,1},{0,0},{4,1},{3,1},{2,1},{1,1},{0,0},{5,1},{4,1},{3,1},{2,1},{0,0},{5,1},{4,1},{3,1},{2,1},{0,0},{1,1},{5,1},{6,1},{5,1},{0,0},{4,1},{3,1},{2,1},{1,1},{0,0}};

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
{0,0,8}
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


//Interface & Contents Written by Peter Bee
uint32_t MusicFunction_PeterBee(uint32_t t, uint32_t length, uint8_t *Mtime, Music_Typical_Name_t *array, int8_t UpDown)
{
	uint32_t i;
	for (i=0;i<length;i++)
	{
		if (t<Mtime[i])
		{
			if (array[i] == BEAT && t > Mtime[i] * 0.25)
				CL_TONE_SetFreq(440*pow(2,(float)(REST)/12.0));
			else
				CL_TONE_SetFreq(440*pow(2,(float)(array[i]+UpDown)/12.0));
			return 0;
		}
		t-=Mtime[i];
	}
	return t;
}

void MusicFunction_PeterBee_float(float t, uint32_t length, float *Mtime, Music_Typical_Name_t *array, int8_t UpDown)
{
	uint32_t i;
	for (i=0;i<length;i++)
	{
		t-=Mtime[i];
		if (t<0)
		{
			if (array[i] == BEAT && t > Mtime[i] * -0.75f)
				CL_TONE_SetFreq(440*pow(2,(float)(REST)/12.0f));
			else
				CL_TONE_SetFreq(440*pow(2,(float)(array[i]+UpDown)/12.0f));
			return;
		}
	}
}

#undef MUSIC_ARRAY
#undef MUSIC_TIME
#undef MUSIC_SPEED
#define MUSIC_ARRAY Bee1
#define MUSIC_TIME Bee1_Time
#define MUSIC_SPEED (84*4) //84��

Music_Typical_Name_t MUSIC_ARRAY[] = {REST,E4,C5,REST,E4,B4,REST,E4,A4,G4,F4,G4,REST,C4,D4,F4,A4,C5,B4,G4,E4,D4,E4,REST,E4,C5,REST,E4,B4,REST,E4,A4,G4,F4,G4,REST,E4,D4,F4,A4,C5,D5,E5,B4,G4,A4,REST,E4,C5,REST,E4,B4,REST,E4,A4,G4,F4,G4,REST,C4,D4,F4,A4,C5,B4,G4,E4,D4,E4,REST,E4,C5,REST,E4,B4,REST,E4,A4,G4,F4,G4,REST,E4,D4,F4,A4,C5,D5,E5,B4,G4,A4,A4,B4,C5,C5,C5,C5,C5,B4,A4,B4,G4,REST,F4,G4,A4,A4,A4,A4,A4,G4,F4,G4,E4,REST,E4,CS4,D4,F4,A4,G4,F4,DS4,E4,G4,B4,D5,C5,B4,C5,E5,D5,E5,A4,REST,E4,C5,E4,C5,REST,E4,B4,REST,E4,A4,G4,F4,G4,REST,C4,D4,F4,A4,C5,B4,G4,E4,D4,E4,REST,E4,C5,REST,E4,B4,REST,E4,A4,G4,F4,G4,REST,E4,D4,F4,A4,C5,D5,E5,B4,G4,A4,REST};
uint8_t MUSIC_TIME[] = {14,2,4,2,2,4,2,2,4,2,2,4,2,2,2,2,2,2,2,2,2,2,12,2,2,4,2,2,4,2,2,4,2,2,4,2,2,2,2,2,2,2,2,2,2,12,2,2,4,2,2,4,2,2,4,2,2,4,2,2,2,2,2,2,2,2,2,2,12,2,2,4,2,2,4,2,2,4,2,2,4,2,2,2,2,2,2,2,2,2,2,12,2,2,2,2,2,2,2,2,2,2,12,2,1,1,2,2,2,2,2,2,2,2,12,2,2,2,2,2,2,2,6,2,2,2,2,2,2,2,2,2,6,2,2,6,14,2,2,4,2,2,4,2,2,4,2,2,4,2,2,2,2,2,2,2,2,2,2,12,2,2,4,2,2,4,2,2,4,2,2,4,2,2,2,2,2,2,2,2,2,2,12,4};

uint8_t MusicFunction_BEE1(uint32_t time, uint32_t start_time)
{
	return MusicFunction_PeterBee(((time - start_time)*MUSIC_SPEED)/60000, sizeof(MUSIC_TIME), MUSIC_TIME, MUSIC_ARRAY, 0);
}

#undef MUSIC_ARRAY
#undef MUSIC_TIME
#undef MUSIC_SPEED
#define MUSIC_ARRAY BadApple
#define MUSIC_TIME BadApple_Time
//#define MUSIC_SPEED (128*4) //128��
#define MUSIC_SPEED (500) //120ms

Music_Typical_Name_t MUSIC_ARRAY[] = {
BEAT,BEAT,BEAT,BEAT,BEAT,BEAT,BEAT,
BEAT,BEAT,BEAT,BEAT,BEAT,
BEAT,BEAT,BEAT,BEAT,BEAT,BEAT,BEAT,
BEAT,BEAT,BEAT,BEAT,BEAT,
BEAT,BEAT,BEAT,BEAT,BEAT,BEAT,BEAT,
BEAT,BEAT,BEAT,BEAT,BEAT,
BEAT,BEAT,BEAT,BEAT,BEAT,BEAT,BEAT,
BEAT,BEAT,BEAT,BEAT,//�����
REST,E4,REST,E4,D4,REST,E4,REST,E4,D4,REST,E4,REST,E4,D4,G4,E4,G4,A4,G4,
REST,E4,REST,E4,D4,REST,E4,REST,E4,D4,REST,E4,REST,E4,D4,A4,G4,A4,G4,E4,G4,
REST,E4,REST,E4,D4,REST,E4,REST,E4,D4,REST,E4,REST,E4,D4,E4,E4,G4,A4,G4,
REST,E4,REST,E4,D4,REST,E4,REST,E4,D4,REST,E4,REST,E4,D4,A4,G4,A4,G4,E4,G4,//ǰ��
D4,E4,F4,G4,A4,D5,C5,A4,D4,A4,G4,F4,E4,D4,E4,F4,G4,A4,G4,F4,E4,D4,E4,F4,E4,D4,CS4,E4,
D4,E4,F4,G4,A4,D5,C5,A4,D4,A4,G4,F4,E4,D4,E4,F4,G4,A4,G4,F4,E4,F4,G4,A4,
D4,E4,F4,G4,A4,D5,C5,A4,D4,A4,G4,F4,E4,D4,E4,F4,G4,A4,G4,F4,E4,D4,E4,F4,E4,D4,CS4,E4,
D4,E4,F4,G4,A4,D5,C5,A4,D4,A4,G4,F4,E4,D4,E4,F4,G4,A4,G4,F4,E4,F4,G4,A4,//Verse A*2
C5,D5,A4,G4,A4,G4,A4,C5,D5,A4,G4,A4,G4,A4,G4,F4,E4,C4,D4,C4,D4,E4,F4,G4,A4,D4,
A4,C5,C5,D5,A4,G4,A4,G4,A4,C5,D5,A4,G4,A4,G4,A4,G4,F4,E4,C4,D4,C4,D4,E4,F4,G4,A4,D4,
A4,C5,C5,D5,A4,G4,A4,G4,A4,C5,D5,A4,G4,A4,G4,A4,G4,F4,E4,C4,D4,C4,D4,E4,F4,G4,A4,D4,
A4,C5,C5,D5,A4,G4,A4,G4,A4,C5,D5,A4,G4,A4,D5,E5,F5,E5,D5,C5,A4,G4,A4,G4,F4,E4,C4,D4,
A4,C5,C5,D5,A4,G4,A4,G4,A4,C5,D5,A4,G4,A4,G4,A4,G4,F4,E4,C4,D4,C4,D4,E4,F4,G4,A4,D4,
A4,C5,C5,D5,A4,G4,A4,G4,A4,C5,D5,A4,G4,A4,G4,A4,G4,F4,E4,C4,D4,C4,D4,E4,F4,G4,A4,D4,
A4,C5,C5,D5,A4,G4,A4,G4,A4,C5,D5,A4,G4,A4,G4,A4,G4,F4,E4,C4,D4,C4,D4,E4,F4,G4,A4,D4,
A4,C5,C5,D5,A4,G4,A4,G4,A4,C5,D5,A4,G4,A4,D5,E5,F5,E5,D5,C5,A4,G4,A4,G4,F4,E4,C4,D4//Verse B*2
};
uint8_t MUSIC_TIME[] = {
4,4,4,1,1,1,1,
4,4,4,2,2,
4,4,4,1,1,1,1,
4,4,4,2,2,
4,4,4,1,1,1,1,
4,4,4,2,2,
4,4,4,1,1,1,1,
4,4,4,4,//�����
2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,1,1,2,2,
2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,1,1,2,1,1,
2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,1,1,2,2,
2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,1,1,2,1,1,//ǰ��
2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,2,2,4,2,2,4,4,4,4,
2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,2,2,4,2,2,4,4,4,4,//Verse A*2
2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,
2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,
2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,
2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,
2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,
2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,
2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,
2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4//Verse B*2
};

#define CONTROL_SIZE 3
#define MUSIC_CONTROL BadAppleCtl
int16_t MUSIC_CONTROL[CONTROL_SIZE][3] = {
{0,sizeof(MUSIC_TIME),0},
{47,sizeof(MUSIC_TIME)-47-112,0},
{sizeof(MUSIC_TIME)-112,112,1}
};

uint8_t MusicFunction_BadApple(uint32_t time, uint32_t start_time)
{
	uint32_t t = ((time - start_time)*MUSIC_SPEED)/60000.;
	uint8_t i;
	for (i = 0; i < CONTROL_SIZE; i++)
	{
		t = MusicFunction_PeterBee(t, MUSIC_CONTROL[i][1], MUSIC_TIME+MUSIC_CONTROL[i][0], MUSIC_ARRAY+MUSIC_CONTROL[i][0],MUSIC_CONTROL[i][2]);
		if (t == 0)
			return 0;
	}
	return t != 0;
}


#undef MUSIC_ARRAY
#undef MUSIC_TIME
#undef MUSIC_SPEED
#define MUSIC_ARRAY CastleInTheSky
#define MUSIC_TIME CastleInTheSky_Time
//#define MUSIC_SPEED (500) //120ms
#define MUSIC_SPEED (120*8)

Music_Typical_Name_t MUSIC_ARRAY[] = {
BEAT,BEAT,BEAT,BEAT,BEAT,BEAT,BEAT,
BEAT,BEAT,BEAT,BEAT,BEAT,
BEAT,BEAT,BEAT,BEAT,BEAT,BEAT,BEAT,
BEAT,BEAT,BEAT,BEAT,BEAT,
BEAT,BEAT,BEAT,BEAT,BEAT,BEAT,BEAT,
BEAT,BEAT,BEAT,BEAT,BEAT,
BEAT,BEAT,BEAT,BEAT,BEAT,BEAT,BEAT,
BEAT,BEAT,BEAT,BEAT,//�����
REST,E4,REST,E4,D4,REST,E4,REST,E4,D4,REST,E4,REST,E4,D4,G4,E4,G4,A4,G4,
REST,E4,REST,E4,D4,REST,E4,REST,E4,D4,REST,E4,REST,E4,D4,A4,G4,A4,G4,E4,G4,
REST,E4,REST,E4,D4,REST,E4,REST,E4,D4,REST,E4,REST,E4,D4,E4,E4,G4,A4,G4,
REST,E4,REST,E4,D4,REST,E4,REST,E4,D4,REST,E4,REST,E4,D4,A4,G4,A4,G4,E4,G4,//ǰ��
D4,E4,F4,G4,A4,D5,C5,A4,D4,A4,G4,F4,E4,D4,E4,F4,G4,A4,G4,F4,E4,D4,E4,F4,E4,D4,CS4,E4,
D4,E4,F4,G4,A4,D5,C5,A4,D4,A4,G4,F4,E4,D4,E4,F4,G4,A4,G4,F4,E4,F4,G4,A4,
D4,E4,F4,G4,A4,D5,C5,A4,D4,A4,G4,F4,E4,D4,E4,F4,G4,A4,G4,F4,E4,D4,E4,F4,E4,D4,CS4,E4,
D4,E4,F4,G4,A4,D5,C5,A4,D4,A4,G4,F4,E4,D4,E4,F4,G4,A4,G4,F4,E4,F4,G4,A4,//Verse A*2
C5,D5,A4,G4,A4,G4,A4,C5,D5,A4,G4,A4,G4,A4,G4,F4,E4,C4,D4,C4,D4,E4,F4,G4,A4,D4,
A4,C5,C5,D5,A4,G4,A4,G4,A4,C5,D5,A4,G4,A4,G4,A4,G4,F4,E4,C4,D4,C4,D4,E4,F4,G4,A4,D4,
A4,C5,C5,D5,A4,G4,A4,G4,A4,C5,D5,A4,G4,A4,G4,A4,G4,F4,E4,C4,D4,C4,D4,E4,F4,G4,A4,D4,
A4,C5,C5,D5,A4,G4,A4,G4,A4,C5,D5,A4,G4,A4,D5,E5,F5,E5,D5,C5,A4,G4,A4,G4,F4,E4,C4,D4,
A4,C5,C5,D5,A4,G4,A4,G4,A4,C5,D5,A4,G4,A4,G4,A4,G4,F4,E4,C4,D4,C4,D4,E4,F4,G4,A4,D4,
A4,C5,C5,D5,A4,G4,A4,G4,A4,C5,D5,A4,G4,A4,G4,A4,G4,F4,E4,C4,D4,C4,D4,E4,F4,G4,A4,D4,
A4,C5,C5,D5,A4,G4,A4,G4,A4,C5,D5,A4,G4,A4,G4,A4,G4,F4,E4,C4,D4,C4,D4,E4,F4,G4,A4,D4,
A4,C5,C5,D5,A4,G4,A4,G4,A4,C5,D5,A4,G4,A4,D5,E5,F5,E5,D5,C5,A4,G4,A4,G4,F4,E4,C4,D4//Verse B*2
};
float MUSIC_TIME[] = {
4,4,4,1,1,1,1,
4,4,4,2,2,
4,4,4,1,1,1,1,
4,4,4,2,2,
4,4,4,1,1,1,1,
4,4,4,2,2,
4,4,4,1,1,1,1,
4,4,4,4,//�����
2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,1,1,2,2,
2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,1,1,2,1,1,
2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,1,1,2,2,
2,2,1,2,1,2,2,1,2,1,2,2,1,2,1,2,1,1,2,1,1,//ǰ��
2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,2,2,4,2,2,4,4,4,4,
2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,2,2,4,2,2,4,4,4,4,//Verse A*2
2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,
2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,
2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,
2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,
2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,
2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,
2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,
2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4,2,2,2,2,2,2,4//Verse B*2
};

#define CONTROL_SIZE 3
#undef MUSIC_CONTROL
#undef MUSIC_PERIED_LEN
#define MUSIC_CONTROL CastleInTheSkyCtl
#define MUSIC_PERIED_LEN CastleInTheSkyLen
int16_t MUSIC_CONTROL[CONTROL_SIZE][3] = {
{0,sizeof(MUSIC_ARRAY),0},
{47,sizeof(MUSIC_ARRAY)-47-112,0},
{sizeof(MUSIC_ARRAY)-112,112,1}
};
float MUSIC_PERIED_LEN[CONTROL_SIZE]={0,0,0};

uint8_t MusicFunction_CastleInTheSky(uint32_t time, uint32_t start_time)
{
	if (time & 0x01)
	{
		float t = ((time - start_time)*MUSIC_SPEED)/60000.f;
		uint8_t i;
		for (i = 0; i < CONTROL_SIZE && t >= 0; i++)
		{
			if (t < MUSIC_PERIED_LEN[i])
				MusicFunction_PeterBee_float(t, MUSIC_CONTROL[i][1], MUSIC_TIME+MUSIC_CONTROL[i][0], MUSIC_ARRAY+MUSIC_CONTROL[i][0], MUSIC_CONTROL[i][2]);
			t -= MUSIC_PERIED_LEN[i];
		}
		return t > 0;
	}
	else
		return 0;
}

void MusicInit_CastleInTheSky(void)
{
	uint8_t i;
	uint16_t j;
	for (i = 0; i < CONTROL_SIZE; i++)
	{
		for (j = MUSIC_CONTROL[i][0]; j < MUSIC_CONTROL[i][0]+MUSIC_CONTROL[i][1]; j++)
			MUSIC_PERIED_LEN[i]+=MUSIC_TIME[j];
	}
}

void MusicInit(void)
{
	MusicInit_CastleInTheSky();
}
