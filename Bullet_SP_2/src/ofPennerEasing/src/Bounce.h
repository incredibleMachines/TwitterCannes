#ifndef _PENNER_BOUNCE
#define _PENNER_BOUNCE

#include "ofMain.h"

class Bounce {
	
	public:
		
		static ofPoint easeIn(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeOut(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeInOut(float t,ofPoint b , ofPoint c, float d);
};

#endif