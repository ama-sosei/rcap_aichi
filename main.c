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

#define LIMIT_LINE 50

#if 1 //kurage
#else //neko
#endif

int res_line = -1;
int goal_num;
int history[10]={0};

void set_history(int num){
	int i;
	for (i = 0; i < 9; ++i){
		history[i]=history[i+1]; //シフト
	}
	history[9]=num;
}
int get_history_avg(){
	int i, max=0, count[10]={0};
	for (i = 0; i < 10; ++i){
		count[history[i]]++;
	}
	for(i = 0; i < 10; ++i)if(count[i]>count[max])max=i;
	return i;
}


void startup(void) {
	//初期化
	UINT y[6], b[6];
	sleep(8000);
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
	int i,max=0;double speed[4];double _max=0;
	if (angle<0)angle=360+angle;
	speed[0] = sin(rad(angle - 315)) * -1 * power;//-0.7
	speed[1] = sin(rad(angle - 45)) * -1 * power;//0.7
	speed[2] = sin(rad(angle - 225)) * power;//-0.7
	speed[3] = sin(rad(angle - 135)) * power;//-0.7
	for (i = 0; i < 4; i++)if(abs((int)speed[i])>_max)_max=(int)speed[i];
	//for (i = 0; i < 4; i++)speed[i]/=_max;
	printf("out_speed %ld\n", (long)(int)speed[0]);
	_motors(speed);
	sleep(5000);
}

void processingGoal(int num, float angle,UINT* ball, int loop) {
	// しゅーーーーーーーーーーーと
	UINT i, y[6], b[6], *goal;
	getPixy(PIXY_GOAL_Y, y);
	getPixy(PIXY_GOAL_B, b);
	goal = goal_num==PIXY_GOAL_Y ? y : b;
	if (!loop)set_history(num);
	if(num == 1) {
		run(30);
	}else if(num == 2) {
		motors(30, 0, 0, 30);
	}else if(num == 3) {
		motors(0, 30, 30, 0);
	}else if(num == 4 || num == 5) {
		//if (goal[4] > 1200) {
			if(num == 4) {
				motors(-30, 30, 30, -30);
			}else{
				motors(30, -30, -30, 30);
			}
		//}else run(-30);
	}else if(num == 6 || num == 7) {
		run(-10);
	}else if (num == 8){
		motors(30, 0, 0, 30);
	}else{
		int his = get_history_avg();
		if (his != 0)processingGoal(his, angle, ball, 1);
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
			if (res_line == -1) {
				getPixy(PIXY_BALL, ball);
				angle = get_angle(ball);
				processingGoal(chkPixy(angle), angle, ball, 0);
			} else {
				comeback(res_line);
			}
		} else {
			dir();
		}
	}
}

/*
void user_main(void) {
	//while(TRUE){
		angle_control(90, 50);
	//	sleep(100);
		//brake();
	//}
}
*/