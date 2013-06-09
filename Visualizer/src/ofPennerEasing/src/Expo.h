#ifndef _PENNER_EXPO
#define _PENNER_EXPO

#include <math.h>
#include "ofMain.h"

class Expo {
	
	public:
		
		static ofPoint easeIn(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeOut(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeInOut(float t,ofPoint b , ofPoint c, float d);
};

#endif