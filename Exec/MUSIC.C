#include "TONE.h"
#include "MUSIC.h"
#include "Musics.h"
#include "Speed.h"
#include "sysconfig.h"
#include "COM.h"

struct EL_MUSIC_Ctrl_t
{
	EL_MUSIC_Type_t Type;
	EL_MUSIC_Play_t Play;
	uint8_t MusicId;
	uint8_t ChangeMusic;
	uint8_t ChangeTone;
}EL_MUSIC_Ctrl={Stop,Single,0,0};

__STATIC_INLINE float EL_MUSIC_CalcFreq(uint16_t t, uint16_t i, const int16_t start[][2])
{
	return start[i-1][0]+(t-start[i-1][1])*(start[i][0]-start[i-1][0])/(start[i][1]-start[i-1][1]);
}

__STATIC_INLINE float EL_MUSIC_CalcFreq2(uint16_t t, uint16_t i, const int16_t start[][2])
{
	return start[i-1][0]*(pow((double)start[i][0]/start[i-1][0],(double)(t-start[i-1][1])/(double)(start[i][1]-start[i-1][1])));
}

#define START_NUM 9
const int16_t freq_start[START_NUM][2]={{600,0},{1200,200},{750,500},{1300,700},{1300,750},{900,1200},{1300,1400},{1300,1500},{600,3000}};

#define ALARM_NUM 6
//const int16_t freq_alarm[ALARM_NUM][2]={{880,0},{880,100},{1967,150},{1967,350},{880,400},{880,500}};
const int16_t freq_alarm[ALARM_NUM][2]={{1200,0},{1200,100},{2000,150},{2000,350},{1200,400},{1200,500}};

#define MUSIC_NUM 3
uint8_t (*const MusicFunctions[MUSIC_NUM])(uint32_t,uint32_t)={
MusicFunction_Twinkle_Twinkle_Little_Star,
MusicFunction_Tsinghua,
MusicFunction_MHH
};
const uint8_t MusicFKeys[MUSIC_NUM][2]={
FKey_TTLS,
FKey_THU,
FKey_MHH
};

uint32_t start_time=0;
uint32_t warning_time_back;
EL_MUSIC_Type_t warning_Type_bak;
uint8_t warning_flag=0;

void EL_MUSIC_Alarm(uint32_t time)
{
	uint16_t t=(time-start_time)%500,i;
	for (i=1;i<ALARM_NUM;i++)
	{
		if (t >= freq_alarm[i-1][1] && t < freq_alarm[i][1])
			CL_TONE_SetFreq(EL_MUSIC_CalcFreq(t,i,freq_alarm));
	}
}

const int16_t freq_test[3][2]={{200,0},{2200,450},{200,900}};
void EL_MUSIC_Test(uint32_t time)
{
	uint16_t t=time-start_time,i;
	if (t < 900)
		for (i=1;i<3;i++)
		{
			if (t >= freq_test[i-1][1] && t < freq_test[i][1])
				CL_TONE_SetFreq(EL_MUSIC_CalcFreq2(t,i,freq_test));
		}
	else
		EL_MUSIC_Ctrl.Type=Stop;
}

void EL_MUSIC_Info(uint32_t time)
{
	uint16_t t=time-start_time;
	if (t < 300)
		CL_TONE_SetFreq(1800);
	else if (t < 400)
		CL_TONE_SetFreq(20000);
	else if (t < 1000)
		CL_TONE_SetFreq(1800);
	else
		EL_MUSIC_Ctrl.Type=Stop;
}

uint8_t pause_flag = 0;
void EL_MUSIC_Music(uint32_t time)
{
	if (!EL_MUSIC_Ctrl.MusicId)
		EL_MUSIC_Ctrl.ChangeMusic=1;
	else if (pause_flag)
	{
		warning_time_back++;
		start_time++;
		CL_TONE_Disable();
	}
	else if(MusicFunctions[EL_MUSIC_Ctrl.MusicId-1](time, start_time))
	{
		switch (EL_MUSIC_Ctrl.Play)
		{
			case Random:
				EL_MUSIC_Ctrl.MusicId+=EL_Rand();
			case Order:
				EL_MUSIC_Ctrl.ChangeMusic=1;
				break;
			case Single:
				EL_MUSIC_Ctrl.ChangeTone=1;
				break;
			default:
				EL_MUSIC_Ctrl.Type=Stop;
		}
	}
}

void EL_MUSIC_SetPause(uint8_t p)
{
	pause_flag = p;
}

void EL_MUSIC_Start(uint32_t time)
{
	uint16_t t=(time-start_time)%3000,i;
	for (i=1;i<START_NUM;i++)
	{
		if (t >= freq_start[i-1][1] && t < freq_start[i][1])
			CL_TONE_SetFreq(EL_MUSIC_CalcFreq2(t,i,freq_start));
	}
}

void EL_MUSIC_ChangeTone(uint32_t time)
{
	start_time=time;
}

void EL_MUSIC_ChangeStatus(EL_MUSIC_Type_t Type, uint8_t MusicID)
{
	EL_MUSIC_Ctrl.Type=Type;
	if (MusicID)
		EL_MUSIC_Ctrl.MusicId=MusicID;
	EL_MUSIC_Ctrl.ChangeTone=1;
	warning_flag=0;//Prepare For Higher Level Call
}

void EL_MUSIC_ChangeMode(EL_MUSIC_Play_t Play)
{
	EL_MUSIC_Ctrl.Play=Play;
}

//for motor warning
void EL_MUSIC_Warning(void)
{
	if (!warning_flag)
	{
		warning_time_back=start_time;
		warning_Type_bak=EL_MUSIC_Ctrl.Type;
		EL_MUSIC_ChangeStatus(Alarm, 0);
	}
	warning_flag=100;//100ms TimeOut
}

void EL_MUSIC_ClearWarning(void)
{
	start_time=warning_time_back;
	EL_MUSIC_Ctrl.Type=warning_Type_bak;
}

//for systick
void EL_MUSIC_Tick(uint32_t time)
{
	if (EL_MUSIC_Ctrl.ChangeTone || EL_MUSIC_Ctrl.ChangeMusic)
	{
		EL_MUSIC_ChangeTone(time);
		if (EL_MUSIC_Ctrl.ChangeMusic)
		{
			EL_MUSIC_Ctrl.MusicId%=MUSIC_NUM;
			CL_TONE_SetFKeyByKey(MusicFKeys[EL_MUSIC_Ctrl.MusicId]);
			EL_MUSIC_Ctrl.MusicId++;
		}
		EL_MUSIC_Ctrl.ChangeTone=0;
		EL_MUSIC_Ctrl.ChangeMusic=0;
	}
	if (warning_flag && (--warning_flag)==0)
		EL_MUSIC_ClearWarning();
	switch(EL_MUSIC_Ctrl.Type)
	{
		case Alarm:
			CL_TONE_Enable();
			EL_MUSIC_Alarm(time);
			break;
		case Start:
			CL_TONE_Enable();
			EL_MUSIC_Start(time);
			break;
		case Info:
			CL_TONE_Enable();
			EL_MUSIC_Info(time);
			break;
		case Music:
			CL_TONE_Enable();
			EL_MUSIC_Music(time);
			break;
		case Test:
			CL_TONE_Enable();
			EL_MUSIC_Test(time);
			break;
		default:
			CL_TONE_Disable();
	}
}

//get game status
#ifdef GAME_STATUS_START
GameStatus_t GameStatus = GAME_START;
#else
GameStatus_t GameStatus = GAME_WAIT;
#endif
void GetGameStatus(const CL_COM_Data_t* p)
{
	GameStatus = p->GameStatus;
}

//for init
void EL_MUSIC_Init(void)
{
	CL_COM_SetRxFunc(GetGameStatus);
	GameStatus = CL_COM_GetData()->GameStatus;
	EL_MUSIC_ChangeStatus(Stop,0);
	if (GameStatus!=GAME_START)
		CL_SPEED_SetCalibration_Type(Run);
	while(CL_SPEED_SetCalibration_Type(Run1) && GameStatus!=GAME_START);
	while(CL_SPEED_SetCalibration_Type(Tone) && GameStatus!=GAME_START);
	if (GameStatus!=GAME_START)
		EL_MUSIC_ChangeStatus(Test,0);
	while(CL_SPEED_SetCalibration_Type(Run2) && GameStatus!=GAME_START);
	while(CL_SPEED_SetCalibration_Type(Run) && GameStatus!=GAME_START);
	while(CL_SPEED_SetCalibration_Type(None));
	if (CL_SPEED_CheckCalibration())
	{
		uint8_t i;
		for (i=0; i<5 && CL_SPEED_CheckCalibration() && GameStatus!=GAME_START; i++)
		{
			CL_SPEED_Calibration_t num = (CL_SPEED_Calibration_t)CL_SPEED_CheckCalibration();
			CL_SPEED_SetCalibration_Type(num);
			if (num == Tone)
				EL_MUSIC_ChangeStatus(Test,0);
			while(CL_SPEED_SetCalibration_Type(None));
		}
		if (CL_SPEED_CheckCalibration())
		{
			EL_MUSIC_ChangeStatus(Info,0);
			CL_SPEED_ClearCalibration();
		}
	}
	CL_SPEED_SetWarningFunction(EL_MUSIC_Warning);
}
