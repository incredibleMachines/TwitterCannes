#include "Quad.h"

ofPoint Quad::easeIn (float t,ofPoint b , ofPoint c, float d) {
	return c*(t/=d)*t + b;
}
ofPoint Quad::easeOut(float t,ofPoint b , ofPoint c, float d) {
	return -c *(t/=d)*(t-2) + b;
}

ofPoint Quad::easeInOut(float t,ofPoint b , ofPoint c, float d) {
	if ((t/=d/2) < 1) return ((c/2)*(t*t)) + b;
	return -c/2 * (((t-2)*(--t)) - 1) + b;
	/*
	originally return -c/2 * (((t-2)*(--t)) - 1) + b;
	
	I've had to swap (--t)*(t-2) due to diffence in behaviour in 
	pre-increment operators between java and c++, after hours
	of joy
	*/
	
}