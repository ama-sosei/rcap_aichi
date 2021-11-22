#include "mylib.h"

#define PIXY_BALL 1 //PIXY ボールのオブジェクトナンバー
#define PIXY_GOAL_Y 2 //黄色ゴールのオブジェクトナンバー
#define PIXY_GOAL_B 3 //青色ゴールのオブジェクトナンバー

#define run(n) (motors(n, n, n, n))

#if 1 //kurage
	#define LIMIT_LINE 45
#else //neko
#endif
int isOnline = FALSE;
int goal_num;

void startup(void){
	UINT y[6], b[6];
	sleep(10);
	setupTimer();
	initial_angle = get_bno(0);
	getPixy(PIXY_GOAL_Y, y);getPixy(PIXY_GOAL_B, b);
	if(b[4] > y[4]){
		set_Led(0, LED_ON);
		goal_num = PIXY_GOAL_B;
	}else{
		set_Led(1, LED_OFF);
		goal_num = PIXY_GOAL_Y;
	}
}

int chkPixy(float angle) {
	if (chkNum(-30, 0, angle)) {
		return 1;
	} else if (chkNum(-55, -30, angle)) {
		return 2;
	} else if (chkNum(0, 30, angle)) {
		return 3;
	} else if (chkNum(-105, -55, angle)) {
		return 4;
	} else if (chkNum(30, 120, angle)) {
		return 5;
	} else if (chkNum(-130 ,-105, angle)) {
		return 6;
	} else if (chkNum(120, 160, angle)) {
		return 7;
	} else if (chkNum(-130, -180, angle) || chkNum(160, 180, angle)) {
		return 8;
	} else {
		return 0;
	}
}

void processingGoal(int num, float angle,UINT* ball) {
	UINT i, y[6], b[6], *goal;
	getPixy(PIXY_GOAL_Y, y);getPixy(PIXY_GOAL_B, b);
	goal = goal_num==PIXY_GOAL_Y ? y : b;

	if(num == 1){
		run(50);
	}else if(num == 2){

	}else if(num == 3){

	}else if(num == 4 || num == 5){
		if (goal[4] > 1200){
			if(num == 4){
				motors(-40, 40, 40, -40);
			}else{
				motors(40, -40, -40, 40);
			}
		}else{
			run(-40);
		}
	}else if(num == 6){

	}else if(num == 7){

	}else if(num == 8){

	}else{
		brake();
	}
}

void user_sub_30(void){ //割り込み
	int n1;
	for (n1 = 0; n1 < 3; n1++) {
		if (getLine(n1) > LIMIT_LINE) {
			isOnline = TRUE;
		}else{
			isOnline = FALSE;
		}
	}
}

void user_main(void)
{
	UINT ball[5]; float angle = 0;
	startup();
	while (TRUE) {
		if (judge_bno(0, initial_angle, 20)) {
			if (isOnline == 0) {
				getPixy(PIXY_BALL, ball);
				angle = get_angle(ball);
				processingGoal(chkPixy(angle), angle, ball);
			} else {
			}
		} else {
			dir();
		}
	}
}
