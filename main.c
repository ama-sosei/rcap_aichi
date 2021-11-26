#include "mylib.h"
//マクロ
#define abs(n) (n < 0 ? -n: n) //絶対値 i++的なのは入れない前提
#define run(n) (motors(n, n, n, n)) //前後進
#define right(n) (motors(n, -n, -n, n)) //右
#define left(n) (motors(-n, n, n, -n)) //左

#define PIXY_BALL 1 //PIXY ボールのオブジェクトナンバー
#define PIXY_GOAL_Y 2 //黄色ゴールのオブジェクトナンバー
#define PIXY_GOAL_B 3 //青色ゴールのオブジェクトナンバー

#define GREEN 0
#define RED1 1

#define LIMIT_LINE 45
#define LIMIT_SPEED 30

#if 1 //kurage
#else //neko
#endif

int res_line = -1;
int goal_num;

void startup(void) {
	//初期化
	UINT y[6], b[6];
	sleep(10000);
	setupTimer();
	initial_angle = get_bno(0);
	getPixy(PIXY_GOAL_Y, y);getPixy(PIXY_GOAL_B, b);
	if(b[4] < y[4]) {
		set_Led(GREEN, LED_ON);
		goal_num = PIXY_GOAL_B;
	}else{
		set_Led(RED1, LED_ON);
		goal_num = PIXY_GOAL_Y;
	}
	sleep(2000);
	set_Led(GREEN,LED_OFF);set_Led(RED1, LED_OFF);
}

int chkPixy(float angle) {//だいたいの位置把握
	if (chkNum(-30, -10, angle)) {
		return 1;
	} else if (chkNum(-60, -30, angle)) {
		return 2;
	} else if (chkNum(-10, 0, angle) || chkNum(0, 40, angle)) {
		return 3;
	} else if (chkNum(-110, -60, angle)) {
		return 4;
	} else if (chkNum(40, 90, angle)) {
		return 5;
	} else if (chkNum(-140 ,-110, angle)) {
		return 6;
	} else if (chkNum(90, 120, angle)) {
		return 7;
	} else if (chkNum(-180, -110, angle) || chkNum(120, 180, angle)) {
		return 8;
	} else {
		return 0;
	}
}

void angle_control(float angle, int power) { //目標値, スピード
	motors(
		sin(angle - rad(45)) * power,
		sin(angle - rad(90+45)) * power,
		sin(angle - rad(180+45)) * power,
		sin(angle - rad(270+45)) * power
	);
}

void processingGoal(int num, float angle,UINT* ball) {
	// しゅーーーーーーーーーーーと
	UINT i, y[6], b[6], *goal;
	getPixy(PIXY_GOAL_Y, y);getPixy(PIXY_GOAL_B, b);
	goal = goal_num==PIXY_GOAL_Y ? y : b;
	if(num == 1) {
		run(20);
	}else if(num == 2 || num == 3) {
		angle_control(angle*1.5, 15);
	}else if(num == 4 || num == 5) {
		if (goal[4] > 1200) {
			if(num == 4) {
				motors(-15, 15, 15, -15);
			}else{
				motors(15, -15, -15, 15);
			}
		}else{
			run(-15);
		}
	}else if(num == 6 || num == 7 || num == 8) {
		angle_control(angle*1.5, 15);
	}else{
		brake();
	}
}

void comeback(int res_line) { //ライン処理
	if (res_line == FRONT_LINE) {
		run(-15);
	}else if(res_line == RIGHT_LINE) {
		left(15);
	}else if(res_line == LEFT_LINE) {
		right(15);
	}else if(res_line == BACK_LINE) {
		run(15);
	}
}

void user_sub_30(void) { //割り込み
	int n1;
	for (n1 = 0; n1 < 4; n1++) {
		if (getLine(n1) > LIMIT_LINE) {
			res_line = n1;
		}else{
			res_line = -1;
		}
	}
}

void user_main(void) {
	UINT ball[5]; float angle = 0;
	startup();
	while (TRUE) {
		if (judge_bno(0, initial_angle, 20)) {
			//if (res_line == -1) {
				getPixy(PIXY_BALL, ball);
				angle = get_angle(ball);
				processingGoal(chkPixy(angle), angle, ball);
			//} else {
			//	comeback(res_line);
			//}
		} else {
			dir();
		}
	}
}

