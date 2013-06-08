#ifndef _PENNER_CIRC
#define _PENNER_CIRC

#include <math.h>
#include "ofMain.h"

class Circ {
	
	public:
		
		static ofPoint easeIn(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeOut(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeInOut(float t,ofPoint b , ofPoint c, float d);
};

#endif