#pragma once
#define MCU_2620
#define TJ3B
#define USE_SUBIO
#define CN9_OUT
#define CN10_OUT
#define INIT_CN9  LOW
#define INIT_CN10 LOW
#define USE_MCB
#define USE_BNO
#define USE_USS
#define USE_PIXY

#include "D_Main.h"
#include "D_I2C.h"
#include "D_SIO.h"
#include "D_EIO.h"

#include "mytime.h"
#include "pixy.h"

//使用済みタイマー
#define TIM_DIR_1 6
#define TIM_DIR_2 7
#define TIM_DIR_3 8
#define TIM_KICKER 9

#define RESET_TIME 50

#if 1 //kurage
	#define FRONT_LINE //前
	#define RIGHT_LINE  //右
	#define LEFT_LINE //左
	#define BACK_LINE //後
	#define LIMIT_LINE
#else //neko
#endif

volatile long initial_angle=999;

int chkNum(UINT u, UINT o, UINT val) {//最低値 最大値 値
	if (u < val && val < o){
		return TRUE;
	} else {
		return FALSE;
	}
}

void motors(BYTE a, BYTE b, BYTE c, BYTE d) {
	int i;
	gPwm[0]=a;gPwm[1]=b;gPwm[2]=c;gPwm[3]=d;
	for (i = 0; i < 4; i++)if (gPwm[i] == (BYTE)999)gPwm[i] = 128;
	pwm_out();
}

void brake(void){
	motors(999, 999, 999, 999);
}

void kick(void) { //燃えないように3秒以上開ける
	if (getTimer(TIM_KICKER) > 3000L) {
		motor(100, 100);
		sleep(100);
		motor(-100, -100);
		startTimer(TIM_KICKER);
	}
}

UINT getLine(int num) {
	int sensor[3] = {LEFT_LINE, RIGHT_LINE, BACK_LINE};
	return gAD[sensor[num]];
}

float EleD = 0; 
void dir() {
	float Dev1, Dev2, ConP, EleD;
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
	motors(
		speed < 0 ? speed * -1 : speed | 0x80,
		speed < 0 ? (speed * -1) | 0x80 : speed,
		speed < 0 ? speed * -1 : speed | 0x80,
		speed < 0 ? (speed * -1) | 0x80 : speed
	);
}

