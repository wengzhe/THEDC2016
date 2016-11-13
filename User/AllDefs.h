#ifndef _AllDefs_H
#define _AllDefs_H

//MAP.C
#define MAP_NULL //make the map null at init
//#define NORMAL_RULE //for the color or my point

//POINTS.C
#define BACK_OK //can run back
	#define BACK_ON_COLOR //can run back even on color <--> only run back when no color set : ONLY_BACK_NO_COLOR
	#define ONLY_BACK_WHEN_NEAR //only can run back when near <--> can run back at every dis : BACK_AT_EVERY_DIS
		#define NEAR_DIS 20
#define COMP_NO_COLOR
//#define COMP_BLACK //Run on black when competition

//Decision.c
//#define PLAYMUSIC

//POINTS.C MUSIC.C Speed.c
//#define GAME_STATUS_START //SetGameStatus to start

#endif
