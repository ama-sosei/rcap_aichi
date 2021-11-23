#ifndef __MYTIME_H
#define __MYTIME_H
#include "D_Main.h"

#define TIM_NUM 10 //独自タイマー個数

ULNG timer[TIM_NUM]; //タイマー用

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


#endif
