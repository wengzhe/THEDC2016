#ifndef _MUSICS_H
#define _MUSICS_H

#include "stm32f10x.h"

uint8_t MusicFunction_Twinkle_Twinkle_Little_Star(uint32_t time, uint32_t start_time);
uint8_t MusicFunction_Tsinghua(uint32_t time, uint32_t start_time);
uint8_t MusicFunction_MHH(uint32_t time, uint32_t start_time);

#define FKey_TTLS {2,0} //C
#define FKey_THU {1,1} //#B
#define FKey_MHH {0,0} //A


#endif
