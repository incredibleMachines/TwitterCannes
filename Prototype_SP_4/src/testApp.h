#pragma once

#include "ofMain.h"
#include "particle.h"
#include "ofxDOF.h"


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
	
	void mouseDragged(int x, int y, int button);
    
	int masterTimer;
	int outTimer;
	
	ofImage pic;
	unsigned char * imagePixels;
	
    
    vector <pixel> pixels;
    vector <Particle> particles;
    void populatePixels();
    void createParticles();
    
    bool bStart;
	
	ofEasyCam camera;
	ofxDOF dof;
    
    ofVec3f camPos;
    
    bool bZ;
    bool bX;
    
    ofLight light;
        ofLight light2;
    ofMaterial material;
    
 
    

};

