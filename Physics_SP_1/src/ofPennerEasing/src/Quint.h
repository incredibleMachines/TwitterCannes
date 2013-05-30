#ifndef _PENNER_QUINT
#define _PENNER_QUINT

#include "ofMain.h"

class Quint {
	
	public:
		
		static ofPoint easeIn(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeOut(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeInOut(float t,ofPoint b , ofPoint c, float d);
};

#endif