#ifndef __MYLIB_H
#define __MYLIB_H
#include "D_Main.h"
#include "D_I2C.h"
#include "D_SIO.h"
#include "D_EIO.h"

#include "mytime.h"
#include "pixy.h"

#define BRK 0x80
#define brake() motors(BRK, BRK, BRK, BRK)

//使用済みタイマー
#define TIM_DIR_1 6
#define TIM_DIR_2 7
#define TIM_DIR_3 8
#define TIM_KICKER 9

//pid timer reset time
#define RESET_TIME 50

//センサー番号
#define FRONT_LINE CN1//前
#define RIGHT_LINE  CN2//右
#define LEFT_LINE CN3//左
#define BACK_LINE CN4//後

#define LIMIT_SPEED 100


#if 1 //kurage
#else //neko
#endif

volatile long initial_angle=0;

int chkNum(UINT min, UINT max, UINT val) {//最低値 最大値 値
	if (min < val && val < max){
		return TRUE;
	} else {
		return FALSE;
	}
}

int _motor(int n){
	if (n == 999 || n== BRK){
		return BRK;
	}else if(100 < n){
		return 100;
	}else if(n < -100){
		return -100;
#if 0
	}else if(n > LIMIT_SPEED){
		return LIMIT_SPEED;
	}else if(n < -LIMIT_SPEED){
		return -LIMIT_SPEED;
#endif
	}else{
		return n;
	}
}

void _motors(int *p){
	int i;
	printf("motor: ");
	for (i = 0; i < 4; i++){
		//p[i]=_motor(p[i]);
		printf("%ld, ", (long)p[i]);
	};
	printf("\n");
	for (i = 0; i < 4; i++)gPwm[i]=(BYTE)p[i];
	pwm_out();
}

void motors(int a, int b, int c, int d) {
	int speed[]={a, b, c, d};
	_motors(speed);
}

void kick(void) { //燃えないように3秒以上開ける
	if (getTimer(TIM_KICKER) > (long)3000) {
		motor(100, 100);
		sleep(100);
		motor(-100, -100);
		startTimer(TIM_KICKER);
	}
}

UINT getLine(int num) {
	int sensor[] = {FRONT_LINE, LEFT_LINE, RIGHT_LINE, BACK_LINE};
	return gAD[sensor[num]];
}

float EleD = 0;
void dir(void) {
	float Dev1, Dev2, ConP, EleD, ConI;
	long speed;
	startTimer(TIM_DIR_1);
	Dev1 = get_bno(0) - initial_angle;
	if (Dev1 > 180) {
		Dev1 = Dev1 - 360;
	} else if (Dev1 < -179) {
		Dev1 = Dev1 + 360;
	}
	Dev2 = get_bno(0) - initial_angle;
	if (Dev2 > 180) {
		Dev2 -= 360;
	} else if (Dev2 < -179) {
		Dev2 += 360;
	}
	startTimer(TIM_DIR_2);
	EleD = (Dev2 - Dev1) / (getTimer(TIM_DIR_1) - getTimer(TIM_DIR_2));
	if (RESET_TIME > getTimer(TIM_DIR_3)) {
		ConI = ConI + Dev1;
	} else {
		ConI = 0;
		startTimer(TIM_DIR_3);
	}
	ConP = Dev1 * 0.3024;
	ConI = ConI * 0.0060;
	speed = ConP + ConI + (EleD * 40.00);
	if (speed < -30) {
		speed = -30;
	} else if (speed > 30) {
		speed = 30;
	}
	gPwm[0] = speed < 0 ? speed * -1 : speed | 0x80;
	gPwm[1] = speed < 0 ? (speed * -1) | 0x80 : speed;
	gPwm[2] = speed < 0 ? speed * -1 : speed | 0x80;
	gPwm[3] = speed < 0 ? (speed * -1) | 0x80 : speed;
	pwm_out();
}


#endif
