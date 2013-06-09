#include "Quart.h"

ofPoint Quart::easeIn (float t,ofPoint b , ofPoint c, float d) {
	return c*(t/=d)*t*t*t + b;
}
ofPoint Quart::easeOut(float t,ofPoint b , ofPoint c, float d) {
	return -c * ((t=t/d-1)*t*t*t - 1) + b;
}

ofPoint Quart::easeInOut(float t,ofPoint b , ofPoint c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t*t*t + b;
	return -c/2 * ((t-=2)*t*t*t - 2) + b;
}