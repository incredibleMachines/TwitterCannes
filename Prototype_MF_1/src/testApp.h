#pragma once

#include "ofMain.h"
#include "particle.h"



class testApp : public ofBaseApp {

	class pixel {
		
	public:
		ofPoint pos;
		int r;
		int g;
		int b;
		
	};
	
	
public:
	void setup();
	void update();
	void draw();
    
	int masterTimer;
	int outTimer;
	
	ofImage pic;
	unsigned char * imagePixels;
	
    
    vector <pixel> pixels;
    vector <Particle> particles;
    void populatePixels();
    void createParticles();
    
    bool bStart;

};

