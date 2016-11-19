#ifndef _AllDefs_H
#define _AllDefs_H

//INF.C
#define INF_TRACK_MAX_SPEED 400 //very fast maybe wrong
//#define INF_STOP_CLEAR_TRACK
#define AVG_SPEED_MAX 300

//PWM.C
//#define PWM_HORN
#define BIG_VOICE
	#define PWM_VOLUME 50

//MAP.C
#define MAP_NULL //make the map null at init
#define NORMAL_RULE //for the color or my point

//POINTS.C
#define BACK_OK //can run back
	#define BACK_ON_COLOR //can run back even on color <--> only run back when no color set : ONLY_BACK_NO_COLOR
	#define ONLY_BACK_WHEN_NEAR //only can run back when near <--> can run back at every dis : BACK_AT_EVERY_DIS
		#define NEAR_DIS 20

//MOTOR
#define P_ANGLE_GO 1
#define P_ANGLE_BACK 1
#define P_SPEED_OF_DIS 1.2
#define P_SPEED_BACK(x) (abs(x) > 80 ? (x) : (x)*7/8)

//Decision.c
//#define RULE_LIFE //only life item
	//The Rules for only life item
	//#define COMP_NO_COLOR
	//#define COMP_BLACK //Run on black when competition
	//#define BACK_TO_CENTER
#define PLAYMUSIC
#define FINAL_EASY

//Speed.c
#define MAX_SPEED_VOLTAGE_L 8.7 //935 8.2
#define MAX_SPEED_VOLTAGE_R 8.5 //919 8.0

//POINTS.C MUSIC.C Speed.c
//#define GAME_STATUS_START //SetGameStatus to start

#endif
