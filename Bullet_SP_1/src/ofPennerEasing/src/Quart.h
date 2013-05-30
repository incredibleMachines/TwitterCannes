#ifndef _PENNER_QUART
#define _PENNER_QUART

#include "ofMain.h"

class Quart {
	
	public:
		
		static ofPoint easeIn(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeOut(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeInOut(float t,ofPoint b , ofPoint c, float d);
};

#endif