#ifndef _AllDefs_H
#define _AllDefs_H

//INF.C
#define INF_TRACK_MAX_SPEED 200 //very fast maybe wrong
#define INF_STOP_CLEAR_TRACK //Clear track data when game finished
#define AVG_SPEED_MAX 300 //How much points to count average speed

//PWM.C
#define PWM_HORN
	#define PWM_HORN_SINGLE //another freq
#define BIG_VOICE
	#define PWM_VOLUME 50 //25%

//MAP.C
#define MAP_NULL //make the map null at init
#define NORMAL_RULE //for the color or my point

//POINTS.C
#define BACK_OK //can run back
	#define BACK_ON_COLOR //can run back even on color <--> only run back when no color set : ONLY_BACK_NO_COLOR
	#define ONLY_BACK_WHEN_NEAR //only can run back when near <--> can run back at every dis : BACK_AT_EVERY_DIS
		#define NEAR_DIS 20
//watchdog
#define POINTS_WATCHDOG_CNT 400
//IR
#define ANGLE_DIFF 40 //different between 2 angles of IR
#define MAX_ANGLE_IN_COLOR 30 //max turn angle
#define MIN_ANGLE_TURN 20 //if angle < 20, we see it as the same with the front

//Speed.c
#define MAX_SPEED_VOLTAGE_L 8.7 //935 8.2
#define MAX_SPEED_VOLTAGE_R 8.5 //919 8.0
//#define SPEED_BYPASS //jump the speed and just set pwm, need to minus the speed to stop exactly

//MOTOR
#define P_ANGLE_GO 0.8
#define P_ANGLE_BACK 0.8
#ifdef SPEED_BYPASS
	#define P_SPEED_OF_DIS 1.5 //speed bypass
	#define SPEED_ADDER 10
	#define DANGER_ANGLE_TAR_SPEED_ADD 40 //middle point adder, when danger
#else
	#define P_SPEED_OF_DIS 1.8 //With the usage of speed module
	#define SPEED_ADDER 0 //actually, when near target, we'll get at least 30
	#define DANGER_ANGLE_TAR_SPEED_ADD 10
#endif
#define P_SPEED_BACK(x) (abs(x) > 80 ? (x) : (x)*7/8)

//Decision.c
//#define RULE_LIFE //only life item
	//The Rules for only life item
	//#define COMP_NO_COLOR
	//#define COMP_BLACK //Run on black when competition
	//#define BACK_TO_CENTER
#define PLAYMUSIC //play at pause
//#define PLAYMUSIC_ATSTART //play when start (will not set any target, no run needed)
#define FINAL_EASY //use easy logic instead of default final logic
#define ESTIMATE_DIS_SAME 20 //if the estimate target is in this dis, see it as the same
#define AIRPLANE_ATTACK_RANGE 37.5f //where can the airplane attack
#define AIRPLANE_HEAL_RANGE 60 //where can the airplane heal

//POINTS.C MUSIC.C Speed.c
//#define GAME_STATUS_START //SetGameStatus to start

//IMU.c POINTS.C Speed.c Calc.c
//#define NO_RUN //Will Not Run, For music@start

#endif
