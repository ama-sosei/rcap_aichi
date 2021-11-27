#ifndef __PIXY_H
#define __PIXY_H
#include "D_I2C.h"
#include "math.h"

#ifndef PI //NASAが15桁らしいから15桁((
	#define PI 3.141592653589793
#endif

#define rad(n) ((n) * (PI/180.0)) //degree to radian
#define deg(n) ((n) * (180.0/PI)) //radian to degree

//計算時のロボットの座標(default:pixy中心)
#define ROBOT_X 160.0
#define ROBOT_Y 100.0

void getPixy(int num, UINT *p) { //引数の配列に代入 構造体返すのもありかも
	p[0] = get_pixydat_x(num); //x
	p[1] = get_pixydat_y(num); //y
	p[2] = get_pixydat_w(num); //w
	p[3] = get_pixydat_h(num); //h
	p[4] = p[2] * p[3]; //s
}

float get_angle(UINT *p){/*
		引数はgetPixyで代入済みの配列, 返り値は角度
		真ん中(define値)調整すればちゃんと前を0にできるかも
	*/
	float x=(ROBOT_X - p[0]), y=(ROBOT_Y - p[1]);
	return deg(atan2(x, y));
}

#endif
