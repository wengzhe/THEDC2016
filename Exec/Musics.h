#ifndef _MUSICS_H
#define _MUSICS_H

#include "stm32f10x.h"

uint8_t MusicFunction_Twinkle_Twinkle_Little_Star(uint32_t time, uint32_t start_time);
uint8_t MusicFunction_Tsinghua(uint32_t time, uint32_t start_time);
uint8_t MusicFunction_MHH(uint32_t time, uint32_t start_time);
uint8_t MusicFunction_BEE1(uint32_t time, uint32_t start_time);

#define FKey_TTLS {2,0} //C
#define FKey_THU {1,1} //#B
#define FKey_MHH {0,0} //A
#define FKey_NONE {0,0} //WhatEver

typedef enum
{
	C2 = -33,
	CS2 = -32,
	D2 = -31,
	DS2 = -30,
	E2 = -29,
	F2 = -28,
	FS2 = -27,
	G2 = -26,
	GS2 = -25,
	A2 = -24,
	AS2 = -23,
	B2 = -22,
	C3 = -21,
	CS3 = -20,
	D3 = -19,
	DS3 = -18,
	E3 = -17,
	F3 = -16,
	FS3 = -15,
	G3 = -14,
	GS3 = -13,
	A3 = -12,
	AS3 = -11,
	B3 = -10,
	C4 = -9,
	CS4 = -8,
	D4 = -7,
	DS4 = -6,
	E4 = -5,
	F4 = -4,
	FS4 = -3,
	G4 = -2,
	GS4 = -1,
	A4 = 0,
	AS4 = 1,
	B4 = 2,
	C5 = 3,
	CS5 = 4,
	D5 = 5,
	DS5 = 6,
	E5 = 7,
	F5 = 8,
	FS5 = 9,
	G5 = 10,
	GS5 = 11,
	A5 = 12,
	AS5 = 13,
	B5 = 14,
	REST = 72
}Music_Typical_Name_t;


#endif
