#include "Bounce.h"

ofPoint Bounce::easeIn (float t,ofPoint b , ofPoint c, float d) {
	return c - easeOut (d-t, ofPoint(0,0,0), c, d) + b;
}
ofPoint Bounce::easeOut(float t,ofPoint b , ofPoint c, float d) {
	if ((t/=d) < (1/2.75f)) {
		return c*(7.5625f*t*t) + b;
	} else if (t < (2/2.75f)) {
		float postFix = t-=(1.5f/2.75f);
		return c*(7.5625f*(postFix)*t + .75f) + b;
	} else if (t < (2.5/2.75)) {
			float postFix = t-=(2.25f/2.75f);
		return c*(7.5625f*(postFix)*t + .9375f) + b;
	} else {
		float postFix = t-=(2.625f/2.75f);
		return c*(7.5625f*(postFix)*t + .984375f) + b;
	}
}

ofPoint Bounce::easeInOut(float t,ofPoint b , ofPoint c, float d) {
	if (t < d/2) return easeIn (t*2, ofPoint(0,0,0), c, d) * .5f + b;
	else return easeOut (t*2-d, ofPoint(0,0,0), c, d) * .5f + c*.5f + b;
}