#include "Quint.h"

ofPoint Quint::easeIn (float t,ofPoint b , ofPoint c, float d) {
	return c*(t/=d)*t*t*t*t + b;
}
ofPoint Quint::easeOut(float t,ofPoint b , ofPoint c, float d) {
	return c*((t=t/d-1)*t*t*t*t + 1) + b;
}

ofPoint Quint::easeInOut(float t,ofPoint b , ofPoint c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t*t*t*t + b;
	return c/2*((t-=2)*t*t*t*t + 2) + b;
}