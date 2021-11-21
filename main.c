#include "mylib.h"

#define PIXY_BALL 1 //PIXY ボールのオブジェクトナンバー
#define PIXY_GOAL_Y 2 //黄色ゴールのオブジェクトナンバー
#define PIXY_GOAL_B 3 //青色ゴールのオブジェクトナンバー

#if 1 //kurage
	#define LIMIT_LINE
#else //neko
#endif

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

void processingGoal(int num, UINT* ball, int noback) {
	UINT i, y[6], b[6], goal[5];
	getPixy(PIXY_GOAL_Y, y);
	getPixy(PIXY_GOAL_B, b);
	if(y[5]){
		for(i=0; i<5; i++)goal[i]=y[i];
	}else{
		for(i=0; i<5; i++)goal[i]=b[i];
	}
	last = y[5]>b[5] ? 'y' : 'b';
	range = 20;
	if (num == 1) {
		if (goal[4]>300){
			motors(30,30,30,30);
		}else{
			motors(40,40,40,40);
		}
	} else if (num == 2) {
		if (noback) {
			motors(35,35,35,35);
		} else {
			motors(35,-35,-35,35);
		}
	} else if (num == 3) {
		if (noback) {
			motors(35,35,35,35);
		} else {
			motors(-35,35,35,-35);
		}
	} else if (num == 4 || num == 5 || num == 6 || num == 7) {
		motors(35,35,35,35);
	} else if (num == 8) {
		UINT y[5], b[5], l; getPixy(PIXY_GOAL_Y, y); getPixy(PIXY_GOAL_B, b);
		if (y[4] != 0 && b[4] != 0) {
			l = b[4] > y[4] ? b[0] : y[0];
			if (l < 160) {
				motors(40,-40,-40,40);
			} else {
				motors(-20,20,20,-20);
			}
			wait_ms(100);
		}
	} else if (num == 0) {
		motors(999,999,999,999);
	} else {
		motors(999,999,999,999);
	}
	wait_ms(100);
}

void user_sub_30(void){ //割り込み
	int n1;
	for (n1 = 0; n1 < 3; n1++) {
		if (getLine(n1) > LINE_LIMIT) {
			touchLine = (n1 + 1) * -1;
			printf("%d\r\n", touchLine);
			break;
		}else{
			touchLine=0;
		}
	}
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

