#include "Circ.h"

ofPoint Circ::easeIn (float t,ofPoint b , ofPoint c, float d) {
	return -c * (sqrt(1 - (t/=d)*t) - 1) + b;
}
ofPoint Circ::easeOut(float t,ofPoint b , ofPoint c, float d) {
	return c * sqrt(1 - (t=t/d-1)*t) + b;
}

ofPoint Circ::easeInOut(float t,ofPoint b , ofPoint c, float d) {
	if ((t/=d/2) < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b;
	return c/2 * (sqrt(1 - t*(t-=2)) + 1) + b;
}