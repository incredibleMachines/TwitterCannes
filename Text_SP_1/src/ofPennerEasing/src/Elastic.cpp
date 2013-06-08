#include "Elastic.h"

ofPoint Elastic::easeIn (float t,ofPoint b , ofPoint c, float d) {
	if (t==0) return b;  if ((t/=d)==1) return b+c;  
	float p=d*.3f;
	ofPoint a=c; 
	float s=p/4;
	ofPoint postFix =a*pow(2,10*(t-=1)); // this is a fix, again, with post-increment operators
	return -(postFix * sin((t*d-s)*(2*PI)/p )) + b;
}

ofPoint Elastic::easeOut(float t,ofPoint b , ofPoint c, float d) {
	if (t==0) return b;  if ((t/=d)==1) return b+c;  
	float p=d*.3f;
	ofPoint a=c; 
	float s=p/4;
	return (a*pow(2,-10*t) * sin( (t*d-s)*(2*PI)/p ) + c + b);	
}

ofPoint Elastic::easeInOut(float t,ofPoint b , ofPoint c, float d) {
	if (t==0) return b;  if ((t/=d/2)==2) return b+c; 
	float p=d*(.3f*1.5f);
	ofPoint a=c; 
	float s=p/4;
	 
	if (t < 1) {
		ofPoint postFix =a*pow(2,10*(t-=1)); // postIncrement is evil
		return -.5f*(postFix* sin( (t*d-s)*(2*PI)/p )) + b;
	} 
	ofPoint postFix =  a*pow(2,-10*(t-=1)); // postIncrement is evil
	return postFix * sin( (t*d-s)*(2*PI)/p )*.5f + c + b;
}