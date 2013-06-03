#include "Sine.h"

ofPoint Sine::easeIn (float t,ofPoint b , ofPoint c, float d) {
	return -c * cos(t/d * (PI/2)) + c + b;
}
ofPoint Sine::easeOut(float t,ofPoint b , ofPoint c, float d) {	
	return c * sin(t/d * (PI/2)) + b;	
}

ofPoint Sine::easeInOut(float t,ofPoint b , ofPoint c, float d) {
	return -c/2 * (cos(PI*t/d) - 1) + b;
}