#include "Cubic.h"

ofPoint Cubic::easeIn (float t,ofPoint b , ofPoint c, float d) {
	return c*(t/=d)*t*t + b;
}
ofPoint Cubic::easeOut(float t,ofPoint b , ofPoint c, float d) {
	return c*((t=t/d-1)*t*t + 1) + b;
}

ofPoint Cubic::easeInOut(float t,ofPoint b , ofPoint c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t*t + b;
	return c/2*((t-=2)*t*t + 2) + b;	
}