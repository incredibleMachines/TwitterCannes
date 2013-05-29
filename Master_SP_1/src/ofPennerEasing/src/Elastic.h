#ifndef _PENNER_ELASTIC
#define _PENNER_ELASTIC

#include <math.h>
#include "ofMain.h"

#ifndef PI
#define PI  3.14159265
#endif

class Elastic {
	
	public:
		
		static ofPoint easeIn(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeOut(float t,ofPoint b , ofPoint c, float d);
		static ofPoint easeInOut(float t,ofPoint b , ofPoint c, float d);
};

#endif