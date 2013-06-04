#ifndef _PENNER_LINEAR
#define _PENNER_LINEAR

#include "ofMain.h"

class Linear {
	
	public:
		
		static ofPoint easeNone(float t,ofPoint b , ofPoint c, float d); // ??
		static ofPoint easeIn(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeOut(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeInOut(float t,ofPoint b , ofPoint c, float d);
		
};

#endif