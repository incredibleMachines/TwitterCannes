#ifndef _PENNER_BACK
#define _PENNER_BACK

#include "ofMain.h"

class Back {
	
	public:
		
		static ofPoint easeIn(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeOut(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeInOut(float t,ofPoint b , ofPoint c, float d);
		
};

#endif