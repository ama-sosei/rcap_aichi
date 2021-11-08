#include "D_Main.h"
#include "D_I2C.h"
#include "D_SIO.h"
#include "D_EIO.h"
#include "math.h"
//#include "stdarg.h"

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

#define TIM_NUM 10 //独自タイマー個数

//使用済みタイマー
#define TIM_DIR_1 6
#define TIM_DIR_2 7
#define TIM_DIR_3 8
#define TIM_KICKER 9

#define abs(n) (n < 0 ? n*-1: n)

#define PIXY_BALL 1 //PIXY ボールのオブジェクトナンバー
#define PIXY_GOAL_Y 2 //黄色ゴールのオブジェクトナンバー
#define PIXY_GOAL_B 3 //青色ゴールのオブジェクトナンバー


#if 1 //kurage
	#define CENTER_X 100
	#define CENTER_Y 160
#else //neko
	#define CENTER_X 
	#define CENTER_Y 
#endif

ULNG timer[TIM_NUM]; //タイマー用

/*#if USE_MCB
	const int numTire=6;
#endif*/
int chkNum(UINT u, UINT o, UINT val) {//最低値 最大値 値
	if (u < val && val < o)
	{
		return 1;
	} else {
		return 0;
	}
}

ULNG getTimer(int num) {
	return get_timer((BYTE)0) - timer[num];
}

int startTimer(int num) {
	timer[num] = get_timer((BYTE)0);
	return 0;
}

int setupTimer(void) {
	int i;
	clr_timer((BYTE)0);
	for (i = 0; i < sizeof(timer) / sizeof(ULNG); ++i)timer[i] = get_timer((BYTE)0);
	return 0;
}

void sleep(int num){
	wait_ms(num);
}

void motors(BYTE a, BYTE b, BYTE c, BYTE d) {
	int i;
	gPwm[0]=a;gPwm[1]=b;
	gPwm[2]=c;gPwm[3]=d;

	for (i = 0; i < 4; i++){
		if (gPwm[i] == (BYTE)999){
			gPwm[i] = 128;
		}
	}
	pwm_out();
}


void brake(void){
	motors(999, 999, 999, 999);
}

void kick(void) { //燃えないように3秒以上開ける
	if (getTimer(TIM_KICKER) > 3000L) {
		motor(100, 100);
		wait_ms(100);
		motor(-100, -100);
		startTimer(TIM_KICKER);
	}
}

int getPixy(int num, UINT *p) {
	p[0] = get_pixydat_x(num); //x
	p[1] = get_pixydat_y(num); //y
	p[2] = get_pixydat_w(num); //w
	p[3] = get_pixydat_h(num); //h
	p[4] = p[2] * p[3]; //s
	return 0;
}

void pixy_normalization(UINT* p){ //正規化
	p[1] = abs(200 - p[1]);
}

float get_angle(UINT *p){
	double c = sqrt(pow(abs(CENTER_X+p[0]), 2) + pow(abs(CENTER_Y+p[0]), 2));
	return acos(abs(CENTER_X / c));
}

float ConI = 0, EleD = 0; //pid用
void dir() {
	float Dev1, Dev2, ConP;
	startTimer(TIM_DIR_1);
	gV[VAR_B] = get_bno(0);
	Dev1 = gV[VAR_B] - gV[VAR_V];
	if (Dev1 > 180) {
		Dev1 = Dev1 - 360;
	} else if (Dev1 < -179) {
		Dev1 = Dev1 + 360;
	}
	gV[VAR_C] = get_bno(0);
	Dev2 = gV[VAR_C] - gV[VAR_V];
	if (Dev2 > 180) {
		Dev2 = Dev2 - 360;
	} else if (Dev2 < -179) {
		Dev2 = Dev2 + 360;
	}
	startTimer(TIM_DIR_2);
	gV[VAR_T] = getTimer(TIM_DIR_1) - getTimer(TIM_DIR_2);
	if (gV[VAR_M] > getTimer(TIM_DIR_3)) {
		ConI = ConI + Dev1;
	} else {
		ConI = 0;
		startTimer(TIM_DIR_3);
	}
	ConP = Dev1 * 0.3024;
	ConI = ConI * 0.0060;
	EleD = (Dev2 - Dev1) / gV[VAR_T];
	gV[VAR_O] = ConP + ConI + (EleD * 40.00);
	if (gV[VAR_O] < -30) {
		gV[VAR_O] = -30;
	} else if (gV[VAR_O] > 30) {
		gV[VAR_O] = 30;
	}
	gPwm[0] = gV[VAR_O] < 0 ? gV[VAR_O] * -1 : gV[VAR_O] | 0x80;
	gPwm[1] = gV[VAR_O] < 0 ? (gV[VAR_O] * -1) | 0x80 : gV[VAR_O];
	gPwm[2] = gV[VAR_O] < 0 ? gV[VAR_O] * -1 : gV[VAR_O] | 0x80;
	gPwm[3] = gV[VAR_O] < 0 ? (gV[VAR_O] * -1) | 0x80 : gV[VAR_O];
	pwm_out();
}

