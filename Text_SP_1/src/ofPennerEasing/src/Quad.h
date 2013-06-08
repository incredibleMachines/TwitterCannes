#ifndef _PENNER_QUAD
#define _PENNER_QUAD

#include "ofMain.h"

class Quad {
	
	public:
		
		static ofPoint easeIn(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeOut(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeInOut(float t,ofPoint b , ofPoint c, float d);
};

#endif