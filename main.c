#include "mylib.h"

#define PIXY_BALL 1 //PIXY ボールのオブジェクトナンバー
#define PIXY_GOAL_Y 2 //黄色ゴールのオブジェクトナンバー
#define PIXY_GOAL_B 3 //青色ゴールのオブジェクトナンバー

void startup(void){
	sleep(5);
	setupTimer();
	initial_angle = get_bno(0);
}

int chkPixy(UINT* ball) {
	UINT x=ball[0], y=ball[1];
	if (chkNum(140, 180, x) && chkNum(0, 70, y)) {
		return 1;
	} else if (chkNum(0, 140, x) && chkNum(0, 70, y)) {
		return 2;
	} else if (chkNum(0, 160, x) && chkNum(70, 110, y)) {
		return 4;
	} else if (chkNum(0, 140, x) && chkNum(110, 200, y)) {
		return 6;
	} else if (chkNum(180, 320, x) && chkNum(0, 70, y)) {
		return 3;
	} else if (chkNum(160, 320, x) && chkNum(70, 110, y)) {
		return 5;
	} else if (chkNum(180, 320, x) && chkNum(110, 200, y)) {
		return 7;
	} else if (chkNum(140, 180, x) && chkNum(110, 200, y)) {
		return 8;
	} else {
		return 0;
	}
}

void processingGoal(int angle, UINT* ball){
	UINT y[6], b[6];
	getPixy(PIXY_GOAL_Y, y);
	getPixy(PIXY_GOAL_B, b);
	if (angle==1){
		motors(40, 40, 40, 40);
	}else if(angle==3){
		motors(-25, 25, 25, -25);
	}else if(angle==2){
		motors(25, -25, -25, 25);
	}else if (angle==0){
		brake();
	}
	sleep(0.1);
}

void user_main(void){
	int angle=0;UINT ball[5];
	startup();
	while(1){
		getPixy(PIXY_BALL, ball);
		angle=get_angle(ball);
		printf("%4ld\n", (long)angle);
		if (angle<=30) {
			set_Led(3, LED_OFF);
			motors(30,30,30,30);
			sleep(0.1);
			//processingGoal(chkPixy(ball), ball);
		}else{
			//dir();
			set_Led(3, LED_ON);
		}
	}
}

