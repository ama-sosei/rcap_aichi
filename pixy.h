#pragma once

#include "D_Main.h"
#include "D_I2C.h"
#include "D_SIO.h"
#include "D_EIO.h"
#include "math.h"

#define PI 3.14159265358979323846
#define rad(n) (n * (PI/180))
#define deg(n) (n * (180/PI))

#define abs(n) (n < 0 ? -n: n)

#define ROBOT_X 160.0
#define ROBOT_Y 100.0

int getPixy(int num, UINT *p) {
	p[0] = get_pixydat_x(num); //x
	p[1] = abs(/*200 - */get_pixydat_y(num)); //y
	p[2] = get_pixydat_w(num); //w
	p[3] = get_pixydat_h(num); //h
	p[4] = p[2] * p[3]; //s
	return 0;
}

float get_angle(UINT *p){
	float result, x=abs(ROBOT_X - p[0]), y=abs(ROBOT_Y - p[1]);
	result = deg(atan2(x, y));
	if (result > 180){
		result -= 360;
	}else if (result <= -180){
		result += 360;
	}
	return result+180;
}


