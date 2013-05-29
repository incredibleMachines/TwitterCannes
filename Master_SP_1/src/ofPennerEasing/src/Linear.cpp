#include "Linear.h"

ofPoint Linear::easeNone (float t,ofPoint b , ofPoint c, float d) {
	return c*t/d + b;
}
ofPoint Linear::easeIn (float t,ofPoint b , ofPoint c, float d) {
	return c*t/d + b;
}
ofPoint Linear::easeOut(float t,ofPoint b , ofPoint c, float d) {	
	return c*t/d + b;
}

ofPoint Linear::easeInOut(float t,ofPoint b , ofPoint c, float d) {
	return c*t/d + b;
}